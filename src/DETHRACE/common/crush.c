#include "crush.h"
#include "brender.h"
#include "car.h"
#include "displays.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrpb.h"
#include "graphics.h"
#include "harness/trace.h"
#include "loading.h"
#include "mainloop.h"
#include "netgame.h"
#include "network.h"
#include "oil.h"
#include "opponent.h"
#include "pd/sys.h"
#include "pedestrn.h"
#include "piping.h"
#include "pratcam.h"
#include "raycast.h"
#include "replay.h"
#include "spark.h"
#include "structur.h"
#include "utility.h"
#include "world.h"
#include <stdlib.h>

// GLOBAL: CARM95 0x00521370
float gWobble_spam_y[8] = { 0.0f, -0.15f, 0.4f, 0.15f, -0.4f, 0.25f, 0.0f, -0.25f };

// GLOBAL: CARM95 0x00521390
float gWobble_spam_z[8] = { 0.4f, -0.25f, 0.0f, 0.25f, 0.0f, 0.15f, -0.4f, -0.15f };

// GLOBAL: CARM95 0x005213b0
br_scalar gWheel_circ_to_width = 0.16f;

// GLOBAL: CARM95 0x005213b8
tU8 gSmoke_damage_step[12] = { 20u, 20u, 0u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u };

// GLOBAL: CARM95 0x005213c4
int gSteal_ranks[5] = { 89, 72, 55, 38, 21 };

#define BIGAPC_OPPONENT_INDEX 4

// IDA: int __usercall ReadCrushData@<EAX>(FILE *pF@<EAX>, tCrush_data *pCrush_data@<EDX>)
// FUNCTION: CARM95 0x004bca50
int ReadCrushData(FILE* pF, tCrush_data* pCrush_data) {
    char s[256];
    char* str;
    int i;
    int j;
    int k;
    tCrush_point_spec* the_spec;
    tCrush_neighbour* the_neighbour;

    pCrush_data->softness_factor = GetAFloat(pF);
    GetPairOfFloats(pF, &pCrush_data->min_fold_factor, &pCrush_data->max_fold_factor);
    pCrush_data->wibble_factor = GetAFloat(pF);
    pCrush_data->limit_deviant = GetAFloat(pF);
    pCrush_data->split_chance = GetAFloat(pF);
    pCrush_data->min_y_fold_down = GetAFloat(pF);
    pCrush_data->number_of_crush_points = GetAnInt(pF);
    pCrush_data->crush_points = (tCrush_point_spec*)BrMemAllocate(sizeof(tCrush_point_spec) * pCrush_data->number_of_crush_points, kMem_crush_data);

    for (i = 0, the_spec = pCrush_data->crush_points; i < pCrush_data->number_of_crush_points; i++, the_spec++) {
        the_spec->vertex_index = GetAnInt(pF);
        GetThreeFloats(pF, &the_spec->limits_neg.v[0], &the_spec->limits_neg.v[1], &the_spec->limits_neg.v[2]);
        GetThreeFloats(pF, &the_spec->limits_pos.v[0], &the_spec->limits_pos.v[1], &the_spec->limits_pos.v[2]);
        GetThreeFloats(pF, &the_spec->softness_neg.v[0], &the_spec->softness_neg.v[1], &the_spec->softness_neg.v[2]);
        GetThreeFloats(pF, &the_spec->softness_pos.v[0], &the_spec->softness_pos.v[1], &the_spec->softness_pos.v[2]);
        the_spec->number_of_neighbours = GetAnInt(pF);
        the_spec->neighbours = BrMemAllocate(sizeof(tCrush_neighbour) * the_spec->number_of_neighbours, kMem_crush_neighbours);

        for (j = 0, the_neighbour = the_spec->neighbours; j < the_spec->number_of_neighbours; j++, the_neighbour++) {
            the_neighbour->vertex_index = GetAnInt(pF);
            the_neighbour->factor = GetAnInt(pF);
        }
    }
    return 0;
}

// IDA: float __usercall SkipCrushData@<ST0>(FILE *pF@<EAX>)
// FUNCTION: CARM95 0x004bcd40
float SkipCrushData(FILE* pF) {
    int i;
    int j;
    int count_1;
    int count_2;
    char s[256];
    float softness;

    softness = GetAFloat(pF);
    for (i = 0; i < 5; ++i) {
        GetALineAndDontArgue(pF, s);
    }
    count_1 = GetAnInt(pF);
    for (i = 0; i < count_1; i++) {
        for (j = 0; j < 5; j++) {
            GetALineAndDontArgue(pF, s);
        }
        count_2 = GetAnInt(pF);
        for (j = 0; j < 2 * count_2; j++) {
            GetALineAndDontArgue(pF, s);
        }
    }
    return softness;
}

// IDA: int __usercall WriteCrushData@<EAX>(FILE *pF@<EAX>, tCrush_data *pCrush_data@<EDX>)
// FUNCTION: CARM95 0x004bce73
int WriteCrushData(FILE* pF, tCrush_data* pCrush_data) {
    int i;
    int j;
    int k;
    tCrush_point_spec* the_spec;
    tCrush_neighbour* the_neighbour;

    fprintf(pF, "%f\n\r", pCrush_data->softness_factor);
    fprintf(pF, "%f,%f\n\r", pCrush_data->min_fold_factor, pCrush_data->max_fold_factor);
    fprintf(pF, "%f\n\r", pCrush_data->wibble_factor);
    fprintf(pF, "%f\n\r", pCrush_data->limit_deviant);
    fprintf(pF, "%f\n\r", pCrush_data->split_chance);
    fprintf(pF, "%f\n\r", pCrush_data->min_y_fold_down);
    fprintf(pF, "%d\n\r", pCrush_data->number_of_crush_points);
    for (i = 0, the_spec = pCrush_data->crush_points; i < pCrush_data->number_of_crush_points; i++, the_spec++) {
        fprintf(pF, "%d\n\r", the_spec->vertex_index);
        fprintf(pF, "%f, %f, %f\n\r", the_spec->limits_neg.v[0], the_spec->limits_neg.v[1], the_spec->limits_neg.v[2]);
        fprintf(pF, "%f, %f, %f\n\r", the_spec->limits_pos.v[0], the_spec->limits_pos.v[1], the_spec->limits_pos.v[2]);
        fprintf(pF, "%f, %f, %f\n\r", the_spec->softness_neg.v[0], the_spec->softness_neg.v[1], the_spec->softness_neg.v[2]);
        fprintf(pF, "%f, %f, %f\n\r", the_spec->softness_pos.v[0], the_spec->softness_pos.v[1], the_spec->softness_pos.v[2]);
        fprintf(pF, "%d\n\r", the_spec->number_of_neighbours);
        for (j = 0, the_neighbour = the_spec->neighbours; j < the_spec->number_of_neighbours; j++, the_neighbour++) {
            fprintf(pF, "%d\n\r", the_neighbour->vertex_index);
            fprintf(pF, "%d\n\r", the_neighbour->factor);
        }
    }
    return 0;
}

// IDA: void __usercall DisposeCrushData(tCrush_data *pCrush_data@<EAX>)
// FUNCTION: CARM95 0x004bd0f7
void DisposeCrushData(tCrush_data* pCrush_data) {
    int i;

    for (i = 0; i < pCrush_data->number_of_crush_points; i++) {
        if (pCrush_data->crush_points[i].neighbours != NULL) {
            BrMemFree(pCrush_data->crush_points[i].neighbours);
        }
    }
    if (pCrush_data->crush_points != NULL) {
        BrMemFree(pCrush_data->crush_points);
    }
}

// IDA: void __usercall CrushModelPoint(tCar_spec *pCar@<EAX>, int pModel_index@<EDX>, br_model *pModel@<EBX>, int pCrush_point_index@<ECX>, br_vector3 *pEnergy_vector, br_scalar total_energy, tCrush_data *pCrush_data)
// FUNCTION: CARM95 0x004bd17b
void CrushModelPoint(tCar_spec* pCar, int pModel_index, br_model* pModel, int pCrush_point_index, br_vector3* pEnergy_vector, br_scalar total_energy, tCrush_data* pCrush_data) {
    int i;
    int j;
    int k;
    int pipe_vertex_count;
    int neighbour_index;
    int bend_axis;
    int default_bend_axis[3];
    tCrush_point_spec* the_crush_point;
    tCrush_neighbour* the_neighbour;
    br_vector3* target_point;
    br_vector3 old_vector;
    br_vector3 softnesss;
    br_vector3 movement;
    br_scalar random_range;
    br_scalar bend_amount;
    br_scalar min_y_fold_down;
    float default_bend_factor[3];
    float working_min_fold;
    float working_max_fold;
    float working_wibble;
    float working_limit_deviant;
    float working_split_chance;
    tChanged_vertex pipe_array[600];
    tCar_spec* car;

    float v12;
    int axis_tmp;

    pipe_vertex_count = 0;
    if (gNet_mode == eNet_mode_host && pCar->car_model_actors[pModel_index].min_distance_squared == 0.0f) {
        NetSendPointCrush(pCar, pCrush_point_index, pEnergy_vector);
    }
    working_min_fold = pCrush_data->min_fold_factor * gCar_crush_min_fold;
    working_max_fold = pCrush_data->max_fold_factor * gCar_crush_max_fold;
    working_wibble = pCrush_data->wibble_factor * gCar_crush_wibble;
    working_limit_deviant = pCrush_data->limit_deviant * gCar_crush_limit_deviant;
    working_split_chance = pCrush_data->split_chance * gCar_crush_split_chance;
    min_y_fold_down = pCrush_data->min_y_fold_down;
    the_crush_point = &pCrush_data->crush_points[pCrush_point_index];
    if (pModel->nvertices <= the_crush_point->vertex_index) {
        return;
    }
    target_point = &pModel->vertices[the_crush_point->vertex_index].p;
    old_vector = *target_point;
    for (i = 0; i < 3; i++) {
        pEnergy_vector->v[i] = SRandomPosNeg(working_wibble * total_energy) + pEnergy_vector->v[i];
        random_range = (the_crush_point->limits_pos.v[i] - the_crush_point->limits_neg.v[i]) * working_limit_deviant;
        if (pEnergy_vector->v[i] >= 0.0f) {
            softnesss.v[i] = the_crush_point->softness_pos.v[i];
        } else {
            softnesss.v[i] = the_crush_point->softness_neg.v[i];
        }
        movement.v[i] = target_point->v[i];
        target_point->v[i] += pEnergy_vector->v[i] * softnesss.v[i];
        if (the_crush_point->limits_neg.v[i] <= target_point->v[i]) {
            if (target_point->v[i] > the_crush_point->limits_pos.v[i]) {
                target_point->v[i] = SRandomPosNeg(random_range) + the_crush_point->limits_pos.v[i];
            }
        } else {
            target_point->v[i] = SRandomPosNeg(random_range) + the_crush_point->limits_neg.v[i];
        }
        movement.v[i] = target_point->v[i] - movement.v[i];
        if (pEnergy_vector->v[i] * movement.v[i] < 0.0f) {
            movement.v[i] = 0.0f;
            target_point->v[i] = old_vector.v[i];
        }
    }

    if (IsActionReplayAvailable()) {
        pipe_array[pipe_vertex_count].vertex_index = the_crush_point->vertex_index;
        BrVector3Sub(&pipe_array[pipe_vertex_count].delta_coordinates, target_point, &old_vector);
        pipe_vertex_count++;
    }
    neighbour_index = -1;
    for (bend_axis = 0; bend_axis < 3; bend_axis++) {
        default_bend_axis[bend_axis] = (bend_axis + IRandomBetween(1, 2)) % 3;
        default_bend_factor[bend_axis] = FRandomBetween(working_min_fold, working_max_fold);
    }

    the_neighbour = the_crush_point->neighbours;
    for (j = 0; j < the_crush_point->number_of_neighbours; j++, the_neighbour++) {
        if (the_neighbour->vertex_index) {
            neighbour_index += the_neighbour->vertex_index;
            if (neighbour_index < 0 || pModel->nvertices <= neighbour_index) {
                return;
            }
            target_point = &pModel->vertices[neighbour_index].p;
            old_vector = *target_point;
            for (bend_axis = 0; bend_axis < 3; bend_axis++) {
                target_point->v[bend_axis] += (1.0f - the_neighbour->factor / 256.0f) * movement.v[bend_axis];
                
                if (the_neighbour->factor <= 128) {
                    v12 = the_neighbour->factor / 128.0f;
                } else {
                    v12 = 2.0 - the_neighbour->factor / 128.0f;
                }
                if (((int)((target_point->v[2] + target_point->v[1] + target_point->v[0]) * 63.0f) & 1) * v12 == 0.0) {
                    bend_amount = -default_bend_factor[bend_axis];
                } else {
                    bend_amount = default_bend_factor[bend_axis];
                }
                axis_tmp = (((int)((target_point->v[2] + target_point->v[1] + target_point->v[0]) * 100.0f) + bend_axis - 1) & 1) % 3;
                target_point->v[axis_tmp] += fabs(movement.v[bend_axis]) * bend_amount;
            }
            if (IsActionReplayAvailable() && pipe_vertex_count < 600) {
                pipe_array[pipe_vertex_count].vertex_index = neighbour_index;
                BrVector3Sub(&pipe_array[pipe_vertex_count].delta_coordinates, target_point, &old_vector);
                pipe_vertex_count++;
            }
        } else {
            neighbour_index += the_neighbour->factor;
        }
    }
    if (IsActionReplayAvailable() && pipe_vertex_count) {
        PipeSingleModelGeometry(pCar->car_ID, pModel_index, pipe_vertex_count, pipe_array);
    }
}

// IDA: void __usercall CrushModel(tCar_spec *pCar@<EAX>, int pModel_index@<EDX>, br_actor *pActor@<EBX>, br_vector3 *pImpact_point@<ECX>, br_vector3 *pEnergy_vector, tCrush_data *pCrush_data)
// FUNCTION: CARM95 0x004bd8da
void CrushModel(tCar_spec* pCar, int pModel_index, br_actor* pActor, br_vector3* pImpact_point, br_vector3* pEnergy_vector, tCrush_data* pCrush_data) {
    br_scalar this_distance;
    br_scalar total_energy;
    br_scalar nearest_so_far;
    br_vector3 impact_point_model;
    br_vector3 energy_vector_scaled;
    br_vector3 energy_vector_model;
    int i;
    int nearest_index;
    br_vertex* vertices;
    br_vertex* the_vertex;
    br_matrix34 inverse_transform;

    if (gArrow_mode) {
        return;
    }
    if (pCrush_data->number_of_crush_points == 0) {
        return;
    }
    BrVector3Sub(&impact_point_model, pImpact_point, (br_vector3*)pActor->t.t.mat.m[3]);
    BrVector3Scale(&energy_vector_model, pEnergy_vector, pCrush_data->softness_factor * gCar_crush_softness);
    total_energy = BrVector3Length(&energy_vector_model);
    if (total_energy < 0.06f) {
        return;
    }
    BrVector3Scale(&energy_vector_scaled, &energy_vector_model, (total_energy - 0.06f) / total_energy);
    nearest_so_far = BR_SCALAR_MAX;
    vertices = pActor->model->vertices;
    nearest_index = -1;
    for (i = 0; i < pCrush_data->number_of_crush_points; i++) {
        the_vertex = &vertices[pCrush_data->crush_points[i].vertex_index];
        this_distance = (impact_point_model.v[2] - the_vertex->p.v[2]) * (impact_point_model.v[2] - the_vertex->p.v[2]) + (impact_point_model.v[1] - the_vertex->p.v[1]) * (impact_point_model.v[1] - the_vertex->p.v[1]) + (impact_point_model.v[0] - the_vertex->p.v[0]) * (impact_point_model.v[0] - the_vertex->p.v[0]);
        if (this_distance < nearest_so_far) {
            nearest_so_far = this_distance;
            nearest_index = i;
        }
    }
    if (nearest_index >= 0) {
        CrushModelPoint(pCar, pModel_index, pActor->model, nearest_index, &energy_vector_scaled, total_energy, pCrush_data);
        SetModelForUpdate(pActor->model, pCar, 1);
    }
}

// IDA: void __cdecl JitModelUpdate(br_actor *actor, br_model *model, br_material *material, void *render_data, br_uint_8 style, int on_screen)
// FUNCTION: CARM95 0x004bdad9
void JitModelUpdate(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {

    BrModelUpdate(model, BR_MODU_VERTEX_POSITIONS);
    model->flags &= ~(BR_MODF_CUSTOM);
    BrZbModelRender(actor, model, material, style, BrOnScreenCheck(&model->bounds), 0);
}

// IDA: void __usercall SetModelForUpdate(br_model *pModel@<EAX>, tCar_spec *pCar@<EDX>, int crush_only@<EBX>)
// FUNCTION: CARM95 0x004bdb2f
void SetModelForUpdate(br_model* pModel, tCar_spec* pCar, int crush_only) {

    if (crush_only && pCar != NULL && pCar->car_model_actors[pCar->principal_car_actor].actor->model == pModel) {
        CrushBoundingBox(pCar, crush_only);
    }
    if ((pModel->flags & BR_MODF_CUSTOM) != 0) {
        pModel->user = JitModelUpdate;
    } else {
        pModel->custom = JitModelUpdate;
        pModel->flags |= BR_MODF_CUSTOM;
    }
}

// IDA: void __usercall TotallySpamTheModel(tCar_spec *pCar@<EAX>, int pModel_index@<EDX>, br_actor *pActor@<EBX>, tCrush_data *pCrush_data@<ECX>, br_scalar pMagnitude)
// FUNCTION: CARM95 0x004bdbc1
void TotallySpamTheModel(tCar_spec* pCar, int pModel_index, br_actor* pActor, tCrush_data* pCrush_data, br_scalar pMagnitude) {
    br_scalar total_energy;
    br_vector3 energy_vector_model;
    int i;
    int the_index;
    br_vertex* the_vertex;
    br_vertex* vertices;

    if (gArrow_mode || pCrush_data->number_of_crush_points == 0) {
        return;
    }
    the_vertex = pActor->model->vertices;
    for (i = 0; i < 15; i++) {
        the_index = IRandomBetween(0, pCrush_data->number_of_crush_points - 1);
        energy_vector_model = the_vertex[pCrush_data->crush_points[the_index].vertex_index].p;
        BrVector3Normalise(&energy_vector_model, &energy_vector_model);
        BrVector3Scale(&energy_vector_model, &energy_vector_model, -pMagnitude);
        CrushModelPoint(pCar, pModel_index, pActor->model, the_index, &energy_vector_model, pMagnitude, pCrush_data);
    }
    SetModelForUpdate(pActor->model, pCar, 1);
}

// IDA: br_scalar __usercall RepairModel@<ST0>(tCar_spec *pCar@<EAX>, int pModel_index@<EDX>, br_actor *pActor@<EBX>, br_vertex *pUndamaged_vertices@<ECX>, br_scalar pAmount, br_scalar *pTotal_deflection)
// FUNCTION: CARM95 0x004bde6c
br_scalar RepairModel(tCar_spec* pCar, int pModel_index, br_actor* pActor, br_vertex* pUndamaged_vertices, br_scalar pAmount, br_scalar* pTotal_deflection) {
    int i;
    int j;
    int pipe_vertex_count;
    br_vector3 old_point;
    br_vertex* model_vertex;
    br_scalar amount;
    br_scalar deviation;
    tChanged_vertex pipe_array[600];

    pipe_vertex_count = 0;
    amount = 0.0f;
    *pTotal_deflection = 0.0f;

    for (i = 0; i < pActor->model->nvertices; i++) {
        model_vertex = &pActor->model->vertices[i];
        old_point = model_vertex->p;
        for (j = 0; j < 3; ++j) {
            *pTotal_deflection = fabs(pUndamaged_vertices->p.v[j] - old_point.v[j]) + *pTotal_deflection;
            if (pUndamaged_vertices->p.v[j] >= old_point.v[j]) {
                if (pUndamaged_vertices->p.v[j] > old_point.v[j]) {
                    model_vertex->p.v[j] = model_vertex->p.v[j] + pAmount;
                    if (pUndamaged_vertices->p.v[j] < model_vertex->p.v[j]) {
                        model_vertex->p.v[j] = pUndamaged_vertices->p.v[j];
                    }
                    amount = model_vertex->p.v[j] - old_point.v[j] + amount;
                }
            } else {
                model_vertex->p.v[j] = model_vertex->p.v[j] - pAmount;
                if (pUndamaged_vertices->p.v[j] > model_vertex->p.v[j]) {
                    model_vertex->p.v[j] = pUndamaged_vertices->p.v[j];
                }
                amount = old_point.v[j] - model_vertex->p.v[j] + amount;
            }
        }
        if (amount != 0.0 && IsActionReplayAvailable() && pipe_vertex_count < COUNT_OF(pipe_array)) {
            pipe_array[pipe_vertex_count].vertex_index = i;
            BrVector3Sub(&pipe_array[pipe_vertex_count].delta_coordinates, &model_vertex->p, &old_point);
            pipe_vertex_count++;
        }
        pUndamaged_vertices++;
    }
    SetModelForUpdate(pActor->model, pCar, 0);
    if (IsActionReplayAvailable() && pipe_vertex_count) {
        PipeSingleModelGeometry(pCar->car_ID, pModel_index, pipe_vertex_count, pipe_array);
    }
    return amount;
}

// IDA: float __usercall RepairCar2@<ST0>(tCar_spec *pCar@<EAX>, tU32 pFrame_period@<EDX>, br_scalar *pTotal_deflection@<EBX>)
// FUNCTION: CARM95 0x004bdd3b
float RepairCar2(tCar_spec* pCar, tU32 pFrame_period, br_scalar* pTotal_deflection) {
    int i;
    tCar_actor* the_car_actor;
    br_scalar amount;
    br_scalar dummy;

    if (gArrow_mode) {
        return 0.0f;
    }
    *pTotal_deflection = 0.0;
    amount = 0.0;

    for (i = 0; i < gProgram_state.current_car.car_actor_count; i++) {
        the_car_actor = &pCar->car_model_actors[i];
        if (the_car_actor->min_distance_squared == 0.0 || !the_car_actor->undamaged_vertices) {
            if (the_car_actor->undamaged_vertices) {
                amount = RepairModel(pCar, i, the_car_actor->actor, the_car_actor->undamaged_vertices, pFrame_period * 0.00005f, pTotal_deflection);
            }
        } else {
            RepairModel(pCar, i, the_car_actor->actor, the_car_actor->undamaged_vertices, pFrame_period * 0.00005f, &dummy);
        }
    }
    pCar->repair_time += pFrame_period;
    return amount;
}

// IDA: float __usercall RepairCar@<ST0>(tU16 pCar_ID@<EAX>, tU32 pFrame_period@<EDX>, br_scalar *pTotal_deflection@<EBX>)
// FUNCTION: CARM95 0x004be159
float RepairCar(tU16 pCar_ID, tU32 pFrame_period, br_scalar* pTotal_deflection) {

    if (VEHICLE_TYPE_FROM_ID(pCar_ID) == eVehicle_self) {
        return RepairCar2(&gProgram_state.current_car, pFrame_period, pTotal_deflection);
    }

    return RepairCar2(GetCarSpec(VEHICLE_TYPE_FROM_ID(pCar_ID), VEHICLE_INDEX_FROM_ID(pCar_ID)), pFrame_period, pTotal_deflection);
}

// IDA: void __usercall TotallyRepairACar(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x004be1ca
void TotallyRepairACar(tCar_spec* pCar) {
    int i;
    int j;
    int k;
    int pipe_vertex_count;
    tCar_actor* the_car_actor;
    tChanged_vertex pipe_array[600];
    br_bounds storage_bounds;

    StopCarSmokingInstantly(pCar);
    if (IsActionReplayAvailable()) {
        PipeInstantUnSmudge(pCar);
    }
    pCar->repair_time += 100000;
    for (i = 0; i < COUNT_OF(pCar->damage_units); i++) {
        pCar->damage_units[i].damage_level = 0;
        pCar->damage_units[i].last_level = 0;
        pCar->damage_units[i].smoke_last_level = 0;
    }
    memcpy(&storage_bounds, &pCar->bounds[1], sizeof(br_bounds));
    memcpy(&pCar->bounds[1], &pCar->max_bounds[1], sizeof(br_bounds));
    if (TestForCarInSensiblePlace(pCar)) {
        for (j = 0; j < pCar->car_actor_count; j++) {
            the_car_actor = &pCar->car_model_actors[j];
            if (the_car_actor->undamaged_vertices != NULL) {
                pipe_vertex_count = 0;
                for (k = 0; k < the_car_actor->actor->model->nvertices; k++) {
                    if (pipe_vertex_count < COUNT_OF(pipe_array)) {
                        BrVector3Sub(&pipe_array[pipe_vertex_count].delta_coordinates,
                            &the_car_actor->undamaged_vertices[k].p, &the_car_actor->actor->model->vertices[k].p);
                        if (!Vector3IsZero(&pipe_array[pipe_vertex_count].delta_coordinates)) {
                            pipe_array[pipe_vertex_count].vertex_index = k;
                            pipe_vertex_count++;
                        }
                    }
                }
                memcpy(the_car_actor->actor->model->vertices,
                    the_car_actor->undamaged_vertices,
                    the_car_actor->actor->model->nvertices * sizeof(br_vertex));
                BrModelUpdate(the_car_actor->actor->model, BR_MODU_VERTEX_COLOURS | BR_MODU_VERTEX_POSITIONS);
                if (pipe_vertex_count != 0 && IsActionReplayAvailable()) {
                    PipeSingleModelGeometry(pCar->car_ID, j, pipe_vertex_count, pipe_array);
                }
            }
        }
    } else {
        memcpy(&pCar->bounds[1], &storage_bounds, sizeof(br_bounds));
    }
}

// IDA: void __cdecl TotallyRepairCar()
// FUNCTION: CARM95 0x004be52a
void TotallyRepairCar(void) {

    if (!gArrow_mode) {
        TotallyRepairACar(&gProgram_state.current_car);
    }
}

// IDA: void __cdecl CheckLastCar()
// FUNCTION: CARM95 0x004be5b5
void CheckLastCar(void) {

    if (gNet_mode == eNet_mode_none && GetCarCount(eVehicle_opponent) != 0 && NumberOfOpponentsLeft() == 0) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 5000, -4, GetMiscString(kMiscString_EveryOpponentWasted));
        RaceCompleted(eRace_over_opponents);
    }
}

// IDA: void __usercall KnackerThisCar(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x004be555
void KnackerThisCar(tCar_spec* pCar) {

    pCar->knackered = 1;
    QueueWastedMassage(pCar->index);
    CheckLastCar();
    QueueOilSpill(pCar);
    if (gNet_mode == eNet_mode_none) {
        KillGroovadelic(pCar->index);
        KillFunkotronic(pCar->index);
    }
}

// IDA: void __usercall SetKnackeredFlag(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x004be619
void SetKnackeredFlag(tCar_spec* pCar) {

    if (gNet_mode != eNet_mode_client
        && !pCar->knackered
        && (pCar->damage_units[eDamage_engine].damage_level >= 99
            || pCar->damage_units[eDamage_transmission].damage_level >= 99
            || pCar->damage_units[eDamage_driver].damage_level >= 99
            || (pCar->damage_units[eDamage_lf_wheel].damage_level >= 99
                && pCar->damage_units[eDamage_rf_wheel].damage_level >= 99
                && pCar->damage_units[eDamage_lr_wheel].damage_level >= 99
                && pCar->damage_units[eDamage_rr_wheel].damage_level >= 99))) {
        KnackerThisCar(pCar);
        if (gNet_mode == eNet_mode_none) {
            if (IRandomBetween(0, 1)) {
                if (gNet_mode == eNet_mode_none) {
                    StopCarSmoking(pCar);
                    CreateSmokeColumn(pCar, 0, IRandomBetween(0, 11), 20000);
                }
            }
            CreateSmokeColumn(pCar, 0, IRandomBetween(0, 11), 180000);
        }
    }
}

// IDA: void __usercall DamageUnit2(tCar_spec *pCar@<EAX>, int pUnit_type@<EDX>, int pDamage_amount@<EBX>)
// FUNCTION: CARM95 0x004be737
void DamageUnit2(tCar_spec* pCar, int pUnit_type, int pDamage_amount) {
    tDamage_unit* the_damage;
    int last_level;

    the_damage = &pCar->damage_units[pUnit_type];
    if ((pCar->driver < eDriver_net_human || pUnit_type != eDamage_driver) && pDamage_amount >= 5 && !pCar->invulnerable) {
        last_level = the_damage->damage_level;
        the_damage->damage_level = pDamage_amount + last_level;
        if (the_damage->damage_level >= 99) {
            if (pDamage_amount >= 10) {
                the_damage->damage_level = 99;
            } else {
                the_damage->damage_level = last_level;
            }
        }
        if (pCar->driver == eDriver_oppo || gNet_mode != eNet_mode_none) {
            SetKnackeredFlag(pCar);
        } else if ((pCar->damage_units[eDamage_engine].damage_level >= 99 && pCar->damage_units[eDamage_engine].last_level < 99 && pCar->damage_units[eDamage_transmission].last_level < 99)
            || (pCar->damage_units[eDamage_transmission].damage_level >= 99 && pCar->damage_units[eDamage_engine].last_level < 99 && pCar->damage_units[eDamage_transmission].last_level < 99)) {
            QueueOilSpill(pCar);
        }
    }
}

// IDA: void __usercall RecordLastDamage(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x004be86b
void RecordLastDamage(tCar_spec* pCar) {
    int i;

    for (i = 0; i < COUNT_OF(pCar->damage_units); i++) {
        pCar->damage_units[i].last_level = pCar->damage_units[i].damage_level;
    }
    pCar->damage_magnitude_accumulator = 0.0;
    pCar->last_impact_location = eImpact_unknown;
    pCar->pre_car_col_mat = pCar->car_master_actor->t.t.mat;
    pCar->pre_car_col_speed = pCar->speed;
    pCar->pre_car_col_knackered = pCar->knackered;
    pCar->pre_car_col_direction = pCar->direction;
    pCar->pre_car_col_velocity = pCar->v;
    pCar->pre_car_col_velocity_car_space = pCar->velocity_car_space;
}

// IDA: void __usercall DoDamage(tCar_spec *pCar@<EAX>, tDamage_type pDamage_type@<EDX>, float pMagnitude, float pNastiness)
// FUNCTION: CARM95 0x004bf3b9
void DoDamage(tCar_spec* pCar, tDamage_type pDamage_type, float pMagnitude, float pNastiness) {

    if (pCar->driver < eDriver_net_human) {
        DamageUnit2(pCar, pDamage_type, ((gCurrent_race.suggested_rank < 10 ? 0.5f : gCurrent_race.suggested_rank) / 20.0f + 1.0f) * (pNastiness * pMagnitude * 10.0f));
    } else if (gNet_mode != eNet_mode_none) {
        DamageUnit2(pCar, pDamage_type, pNastiness * pMagnitude * 15.0f);
    } else if (PercentageChance(pNastiness * pMagnitude * 1500.0f)) {
        DamageUnit2(pCar, pDamage_type, pNastiness * pMagnitude * 30.0f);
    }
}

// IDA: void __usercall CheckPiledriverBonus(tCar_spec *pCar@<EAX>, br_vector3 *pImpact_point@<EDX>, br_vector3 *pEnergy@<EBX>)
// FUNCTION: CARM95 0x004bf4b2
void CheckPiledriverBonus(tCar_spec* pCar, br_vector3* pImpact_point, br_vector3* pEnergy) {
    br_actor* child;
    br_vector3 norm_impact;
    br_vector3 norm_child;
    br_vector3 norm_energy;
    br_scalar dp;

    if (pCar->current_car_actor < 0) {
        return;
    }

    BrVector3Normalise(&norm_impact, pImpact_point);
    norm_impact.v[1] = 0.f;
    BrVector3Normalise(&norm_energy, pEnergy);

    for (child = pCar->car_master_actor->children; child != NULL; child = child->next) {
        if (ActorIsPedestrian(child) && PedestrianActorIsPerson(child) && pCar->speed > 0.001f) {
            BrVector3Normalise(&norm_child, &child->t.t.translate.t);
            norm_child.v[1] = 0.f;
            if (BrVector3Dot(&norm_child, &norm_impact) > 0.8f && BrVector3Dot(&norm_energy, &norm_child) < -.65) {
                DoFancyHeadup(kFancyHeadupPileDriverBonus);
                EarnCredits(((GetPedestrianValue(child) / 2 + 12) / 25) * 25);
                return;
            }
        }
    }
}

// IDA: tImpact_location __usercall CalcModifiedLocation@<EAX>(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x004c13e0
tImpact_location CalcModifiedLocation(tCar_spec* pCar) {

    if (pCar->last_impact_location != eImpact_left && pCar->last_impact_location != eImpact_right && pCar->last_impact_location != eImpact_top && pCar->last_impact_location != eImpact_bottom) {
        return pCar->last_impact_location;
    }
    if (pCar->last_col_prop_z < 0.25f) {
        return eImpact_front;
    }
    if (pCar->last_col_prop_z > 0.75f) {
        return eImpact_back;
    } else {
        return pCar->last_impact_location;
    }
}

// IDA: void __usercall DoPratcamHit(br_vector3 *pHit_vector@<EAX>)
// FUNCTION: CARM95 0x004be97b
void DoPratcamHit(br_vector3* pHit_vector) {
    int strength_modifier;
    br_scalar strength;

    strength = BrVector3LengthSquared(pHit_vector);
    if (strength > 0.2f) {
        strength_modifier = 8;
    } else if (strength > 0.015f) {
        strength_modifier = 4;
    } else if (strength >= 0.001f) {
        strength_modifier = 0;
    } else {
        return;
    }
    if (fabs(pHit_vector->v[2]) >= fabs(pHit_vector->v[0])) {
        if (pHit_vector->v[2] >= 0.f) {
            PratcamEvent(kPratcam_small_hit_front + strength_modifier);
        } else {
            PratcamEvent(kPratcam_small_hit_behind + strength_modifier);
        }
    } else {
        if (pHit_vector->v[0] >= 0.f) {
            PratcamEvent(kPratcam_small_hit_left + strength_modifier);
        } else {
            PratcamEvent(kPratcam_small_hit_right + strength_modifier);
        }
    }
}

// IDA: void __usercall DamageSystems(tCar_spec *pCar@<EAX>, br_vector3 *pImpact_point@<EDX>, br_vector3 *pEnergy_vector@<EBX>, int pWas_hitting_a_car@<ECX>)
// FUNCTION: CARM95 0x004beaa3
void DamageSystems(tCar_spec* pCar, br_vector3* pImpact_point, br_vector3* pEnergy_vector, int pWas_hitting_a_car) {
    int i;
    int j;
    int result;
    br_bounds crushed_car_bounds;
    float proportion_x;
    float proportion_y;
    float proportion_z;
    float energy_magnitude;
    float pure_energy_magnitude;
    br_scalar x;
    br_scalar y;
    br_scalar z;
    br_scalar x1;
    br_scalar x2;
    br_scalar y1;
    br_scalar y2;
    br_scalar z1;
    br_scalar z2;
    br_scalar distance;
    tImpact_location impact_location;
    tDamage_program* the_program;
    tDamage_clause* the_clause;
    tDamage_condition* the_condition;
    tDamage_effect* the_effect;
    tImpact_location modified_location;

#if defined(DETHRACE_FIX_BUGS)
    proportion_x = 0;
    proportion_y = 0;
    proportion_z = 0;
#endif

    pure_energy_magnitude = BrVector3Length(pEnergy_vector);
    if (pure_energy_magnitude == 0.0f && !pWas_hitting_a_car) {
        return;
    }

    energy_magnitude = pCar->car_model_actors[pCar->principal_car_actor].crush_data.softness_factor * pure_energy_magnitude / 0.7f;
    BrVector3InvScale(&crushed_car_bounds.min, &pCar->bounds[1].min, WORLD_SCALE);
    BrVector3InvScale(&crushed_car_bounds.max, &pCar->bounds[1].max, WORLD_SCALE);

    x1 = pImpact_point->v[0] - crushed_car_bounds.min.v[0];
    x2 = crushed_car_bounds.max.v[0] - pImpact_point->v[0];
    if (x1 >= x2) {
        x = x2;
    } else {
        x = x1;
    }
    y1 = pImpact_point->v[1] - crushed_car_bounds.min.v[1];
    y2 = crushed_car_bounds.max.v[1] - pImpact_point->v[1];
    if (y1 >= y2) {
        y = y2;
    } else {
        y = y1;
    }
    z1 = pImpact_point->v[2] - crushed_car_bounds.min.v[2];
    z2 = crushed_car_bounds.max.v[2] - pImpact_point->v[2];
    if (z1 >= z2) {
        z = z2;
    } else {
        z = z1;
    }
    if (z > x || z > y) {
        if (x > y || x > z) {
            impact_location = y1 < y2 ? eImpact_bottom : eImpact_top;
            proportion_z = z1 / (crushed_car_bounds.max.v[2] - crushed_car_bounds.min.v[2]);
            proportion_x = x1 / (crushed_car_bounds.max.v[0] - crushed_car_bounds.min.v[0]);
        } else {
            impact_location = x1 >= x2 ? eImpact_right : eImpact_left;
            proportion_z = z1 / (crushed_car_bounds.max.v[2] - crushed_car_bounds.min.v[2]);
            proportion_y = y1 / (crushed_car_bounds.max.v[1] - crushed_car_bounds.min.v[1]);
        }
    } else {
        impact_location = z1 >= z2 ? eImpact_back : eImpact_front;
        proportion_x = x1 / (crushed_car_bounds.max.v[0] - crushed_car_bounds.min.v[0]);
        proportion_y = y1 / (crushed_car_bounds.max.v[1] - crushed_car_bounds.min.v[1]);
    }
    if (pWas_hitting_a_car && pCar->last_impact_location == eImpact_unknown) {
        pCar->last_impact_location = impact_location;
        pCar->last_col_prop_x = proportion_x;
        pCar->last_col_prop_y = proportion_y;
        pCar->last_col_prop_z = proportion_z;
    }

    if (energy_magnitude != 0.0f && !pCar->invulnerable) {
        if (!pWas_hitting_a_car && impact_location == eImpact_bottom) {
            energy_magnitude = energy_magnitude / 2.0f;
        }

        the_program = &pCar->damage_programs[impact_location];
        the_clause = the_program->clauses;
        for (i = 0; i < the_program->clause_count; i++) {
            result = 1;
            the_condition = the_clause->conditions;
            for (j = 0; j < the_clause->condition_count; j++) {
                switch (the_condition->axis_comp) {
                case eAxis_x:
                    if (the_condition->condition_operator == eCondition_greater_than) {
                        if (the_condition->comparitor >= proportion_x) {
                            result = 0;
                        }
                    } else if (the_condition->comparitor <= proportion_x) {
                        result = 0;
                    }
                    break;

                case eAxis_y:
                    if (the_condition->condition_operator == eCondition_greater_than) {
                        if (the_condition->comparitor >= proportion_y) {
                            result = 0;
                        }
                    } else if (the_condition->comparitor <= proportion_y) {
                        result = 0;
                    }
                    break;

                case eAxis_z:
                    if (the_condition->condition_operator == eCondition_greater_than) {
                        if (the_condition->comparitor >= proportion_z) {
                            result = 0;
                        }
                    } else if (the_condition->comparitor <= proportion_z) {
                        result = 0;
                    }
                    break;
                }

                if (!result) {
                    break;
                }
                the_condition++;
            }
            if (result) {
                for (j = 0; j < the_clause->effect_count; j++) {
                    the_effect = &the_clause->effects[j];
                    DoDamage(pCar, the_effect->type, energy_magnitude, the_effect->weakness_factor);
                }
            }
            the_clause++;
        }
        if (pCar->driver == eDriver_local_human) {
            switch (impact_location) {
            case eImpact_top:
            case eImpact_bottom:
                NewScreenWobble(
                    FRandomBetween(energy_magnitude * 5.0f, energy_magnitude * 20.0f),
                    FRandomBetween(energy_magnitude * 30.0f, energy_magnitude * 60.0f),
                    FRandomBetween(1.0f / energy_magnitude, 5.0f / energy_magnitude));
                break;
            case eImpact_left:
                NewScreenWobble(
                    FRandomBetween(energy_magnitude * 50.0f, energy_magnitude * 100.0f),
                    FRandomBetween(energy_magnitude * 5.0f, energy_magnitude * 20.0f),
                    FRandomBetween(4.0f / energy_magnitude, 7.0 / energy_magnitude));
                break;
            case eImpact_right:
                NewScreenWobble(
                    -FRandomBetween(energy_magnitude * 50.0f, energy_magnitude * 100.0f),
                    FRandomBetween(energy_magnitude * 5.0f, energy_magnitude * 20.0f),
                    FRandomBetween(4.0f / energy_magnitude, 7.0f / energy_magnitude));

                break;
            case eImpact_front:
                NewScreenWobble(
                    FRandomBetween(energy_magnitude * 5.0f, energy_magnitude * 20.0f),
                    FRandomBetween(energy_magnitude * 50.0f, energy_magnitude * 150.0f),
                    FRandomBetween(7.0f / energy_magnitude, 25.0f / energy_magnitude));
                break;
            case eImpact_back:
                NewScreenWobble(
                    FRandomBetween(energy_magnitude * 5.0f, energy_magnitude * 20.0f),
                    FRandomBetween(-energy_magnitude * 50.0f, energy_magnitude * 150.0f),
                    FRandomBetween(7.0f / energy_magnitude, 25.0f / energy_magnitude));
                break;
            default:
                break;
            }
            CheckPiledriverBonus(pCar, pImpact_point, pEnergy_vector);
        }
    }
}

// IDA: tImpact_location __usercall GetDirection@<EAX>(br_vector3 *pVelocity@<EAX>)
// FUNCTION: CARM95 0x004c1486
tImpact_location GetDirection(br_vector3* pVelocity) {
    br_scalar mag_x;
    br_scalar mag_y;
    br_scalar mag_z;

    mag_x = fabs(pVelocity->v[0]);
    mag_y = fabs(pVelocity->v[1]);
    mag_z = fabs(pVelocity->v[2]);
    if (mag_y >= mag_x || mag_z >= mag_x) {
        if (mag_y <= mag_x || mag_z >= mag_y) {
            if (pVelocity->v[2] >= 0.0f) {
                return eImpact_back;
            } else {
                return eImpact_front;
            }
        } else {
            return pVelocity->v[1] < 0.0;
        }
    } else if (pVelocity->v[0] >= 0.0) {
        return eImpact_right;
    } else {
        return eImpact_left;
    }
}

// IDA: void __usercall SetSmokeLastDamageLevel(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x004bf768
void SetSmokeLastDamageLevel(tCar_spec* pCar) {
    int i;

    for (i = 0; i < COUNT_OF(pCar->damage_units); i++) {
        pCar->damage_units[i].smoke_last_level = pCar->damage_units[i].damage_level;
    }
}

// IDA: void __usercall SortOutSmoke(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x004bf7c2
void SortOutSmoke(tCar_spec* pCar) {
    int i;
    int colour;
    int old_colour;
    int step;
    int pass;
    int repeat;

    if (!pCar || pCar->driver <= eDriver_non_car) {
        return;
    }
    for (i = 0; i < COUNT_OF(pCar->damage_units); i++) {
        if (pCar->damage_units[i].damage_level != pCar->damage_units[i].smoke_last_level) {
            step = gSmoke_damage_step[i];
            if (step) {
                if (pCar->damage_units[i].damage_level > pCar->damage_units[i].smoke_last_level) {
                    old_colour = (99 - pCar->damage_units[i].smoke_last_level) / step;
                    colour = (99 - pCar->damage_units[i].damage_level) / step;
                    if (old_colour != colour && colour <= 2) {
                        ConditionalSmokeColumn(pCar, i, colour);
                    }
                }
            }
        }
    }
    SetSmokeLastDamageLevel(pCar);
}

// IDA: void __usercall StealCar(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x004bf923
void StealCar(tCar_spec* pCar) {

    pCar->has_been_stolen = 1;
    gProgram_state.cars_available[gProgram_state.number_of_cars] = pCar->index;
    gProgram_state.number_of_cars++;
    gOpponents[pCar->index].dead = 1;
}

// IDA: int __usercall DoCrashEarnings@<EAX>(tCar_spec *pCar1@<EAX>, tCar_spec *pCar2@<EDX>)
// FUNCTION: CARM95 0x004bfdb3
int DoCrashEarnings(tCar_spec* pCar1, tCar_spec* pCar2) {
    tCar_spec* culprit;
    tCar_spec* victim;
    int i;
    int net_loop;
    int head_on;
    int bonus_level;
    int credits;
    int impact_in_moving_direction_1;
    int impact_in_moving_direction_2;
    int car_off_ground_1;
    int car_off_ground_2;
    int total_units_of_damage;
    int inherited_damage;
    int dam_acc_1;
    int dam_acc_2;
    int car_1_culpable;
    int car_2_culpable;
    int mutual_culpability;
    tU32 the_time;
    tU32 time;
    float credits_squared;
    // GLOBAL: CARM95 0x5213dc
    static tU32 last_earn_time;
    char s[256];
    tImpact_location modified_location_1;
    tImpact_location modified_location_2;
    tImpact_location car_direction_1;
    tImpact_location car_direction_2;
    br_scalar car_1_height;
    br_scalar car_2_height;
    br_scalar dp;
    br_vector3 car_1_pos;
    br_vector3 car_2_pos;
    br_vector3 car_1_offset;
    br_vector3 car_2_offset;
    tNet_message* message;

    culprit = 0;
    victim = 0;
    head_on = 0;
    bonus_level = 1;
    car_1_culpable = 0;
    car_2_culpable = 0;
    mutual_culpability = 0;
    the_time = PDGetTotalTime();
    inherited_damage = 0;
#if defined(DETHRACE_FIX_BUGS)
    total_units_of_damage = 0;
#endif
    if (pCar1->driver <= eDriver_non_car) {
        dam_acc_1 = 0;
    } else {
        dam_acc_1 = pCar1->damage_magnitude_accumulator;
    }

    dam_acc_2 = 0;
    if (pCar2 != NULL) {
        if (pCar2->driver <= eDriver_non_car) {
            dam_acc_2 = 0;
        } else {
            dam_acc_2 = pCar2->damage_magnitude_accumulator != 0;
        }
    }

    if (pCar1->driver <= eDriver_non_car) {
        if (pCar2 == NULL || pCar2->driver <= eDriver_non_car) {
            return 0;
        }
        pCar1 = pCar2;
        pCar2 = NULL;
    }
    if (pCar2 != NULL && pCar2->driver <= eDriver_non_car) {
        pCar2 = NULL;
    }

    if (pCar1->pre_car_col_knackered || (pCar2 && pCar2->pre_car_col_knackered) || (pCar2 && pCar2->damage_magnitude_accumulator <= 0.00005f && pCar1->damage_magnitude_accumulator <= 0.00005f)) {
        return dam_acc_1 || (pCar2 && dam_acc_2);
    }

    modified_location_1 = CalcModifiedLocation(pCar1);
    car_direction_1 = GetDirection(&pCar1->pre_car_col_velocity_car_space);
    impact_in_moving_direction_1 = car_direction_1 == modified_location_1;
    if (pCar2 != NULL) {
        modified_location_2 = CalcModifiedLocation(pCar2);
        car_direction_2 = GetDirection(&pCar2->pre_car_col_velocity_car_space);
        impact_in_moving_direction_2 = car_direction_2 == modified_location_2;
    }
    if (pCar1->driver >= eDriver_net_human && pCar2) {
        if (impact_in_moving_direction_1 && (pCar1->driver < eDriver_net_human || (pCar1->pre_car_col_velocity_car_space.v[2] != 0.0 && (pCar1->pre_car_col_velocity_car_space.v[2] > 0.0) != (pCar1->gear > 0) && (pCar1->keys.acc != 0 || pCar1->joystick.acc > 0x8000)))) {
            pCar2->time_last_hit = the_time;
            pCar2->last_hit_by = pCar1;
        }
    } else if (pCar2 && pCar2->driver >= eDriver_net_human && impact_in_moving_direction_2 && (pCar2->driver < eDriver_net_human || (pCar2->pre_car_col_velocity_car_space.v[2] != 0.0f && (pCar2->pre_car_col_velocity_car_space.v[2] > 0.0f) != (pCar2->gear > 0) && (pCar2->keys.acc != 0 || pCar2->joystick.acc > 0x8000)))) {
        pCar1->time_last_hit = the_time;
        pCar1->last_hit_by = pCar2;
    }
    if (pCar2) {
        if (impact_in_moving_direction_1
            && pCar1->pre_car_col_speed * 5.0f > pCar2->pre_car_col_speed
            && pCar1->pre_car_col_speed > 0.0005f
            && (pCar1->driver < eDriver_net_human
                || (pCar1->pre_car_col_velocity_car_space.v[2] != 0.0f
                    && (pCar1->pre_car_col_velocity_car_space.v[2] > 0.0f) != (pCar1->gear > 0)
                    && (pCar1->keys.acc != 0 || pCar1->joystick.acc > 0x8000)))) {
            car_1_culpable = 1;
        }
        if (impact_in_moving_direction_2
            && pCar2->pre_car_col_speed * 5.0f > pCar1->pre_car_col_speed
            && pCar2->pre_car_col_speed > 0.0005f
            && (pCar2->driver < eDriver_net_human
                || (pCar2->pre_car_col_velocity_car_space.v[2] != 0.0f
                    && (pCar2->pre_car_col_velocity_car_space.v[2] > 0.0f) != (pCar2->gear > 0)
                    && (pCar2->keys.acc != 0 || pCar2->joystick.acc > 0x8000)))) {
            car_2_culpable = 1;
        }
        if (gNet_mode && car_1_culpable && car_2_culpable) {
            mutual_culpability = 1;
        } else {
            if (car_2_culpable && pCar2->driver == eDriver_local_human) {
                car_1_culpable = 0;
            }
            if (car_1_culpable) {
                culprit = pCar1;
                victim = pCar2;
                dp = BrVector3Dot(&pCar1->pre_car_col_direction, &pCar2->pre_car_col_direction);
                if (modified_location_1 == eImpact_front && modified_location_2 == eImpact_front && pCar1->pre_car_col_speed > 0.001f && pCar2->pre_car_col_speed > 0.001f && dp < -0.7f) {
                    head_on = 1;
                    bonus_level = 2;
                } else {
                    bonus_level = 1;
                }
            } else if (car_2_culpable) {
                culprit = pCar2;
                victim = pCar1;
                dp = BrVector3Dot(&pCar1->pre_car_col_direction, &pCar2->pre_car_col_direction);
                if (modified_location_1 == eImpact_front && modified_location_2 == eImpact_front && pCar1->pre_car_col_speed > 0.001f && pCar2->pre_car_col_speed > 0.001f && dp < -0.7f) {
                    head_on = 1;
                    bonus_level = 2;
                } else {
                    bonus_level = 1;
                }
            }
        }
    } else {
        if (the_time - pCar1->time_last_hit >= 3000) {
            return 1;
        }
        culprit = pCar1->last_hit_by;
        victim = pCar1;
        bonus_level = 1;
        inherited_damage = 1;
    }
    if (!mutual_culpability && (!victim || culprit->driver < eDriver_net_human)) {
        if (pCar2 && pCar2->last_culprit == pCar1 && the_time - pCar2->time_last_victim < 750) {
            inherited_damage = 1;
            culprit = pCar1;
            victim = pCar2;
        } else if (pCar2 && pCar1->last_culprit == pCar2 && the_time - pCar1->time_last_victim < 750) {
            inherited_damage = 1;
            culprit = pCar2;
            victim = pCar1;
        } else if (!pCar2 && the_time - pCar1->time_last_victim < 750) {
            inherited_damage = 1;
            culprit = pCar1->last_culprit;
            victim = pCar1;
        }
    }
    if (culprit && victim) {
        RecordOpponentTwattageOccurrence(culprit, victim);
        total_units_of_damage = 0;
        for (i = 0; i < COUNT_OF(victim->damage_units); i++) {
            if (victim->damage_units[i].damage_level > victim->damage_units[i].last_level) {
                victim->damage_units[i].damage_level = (victim->damage_units[i].damage_level - victim->damage_units[i].last_level) * 2.0f + victim->damage_units[i].last_level;
                if (victim->damage_units[i].damage_level >= 99) {
                    victim->damage_units[i].damage_level = 99;
                }
                total_units_of_damage += victim->damage_units[i].damage_level - victim->damage_units[i].last_level;
            }
            if (culprit->damage_units[i].damage_level > culprit->damage_units[i].last_level) {
                culprit->damage_units[i].damage_level = (culprit->damage_units[i].damage_level - culprit->damage_units[i].last_level) * 0.1f + (double)culprit->damage_units[i].last_level;
                if (culprit->damage_units[i].damage_level < 0) {
                    culprit->damage_units[i].damage_level = 0;
                }
            }
        }
    }
    // TODO: tidy this up
    for (net_loop = 0; 2 - (mutual_culpability == 0) > net_loop; net_loop++) {
        if (mutual_culpability) {
            if (net_loop) {
                culprit = pCar1;
                victim = pCar2;
            } else {
                culprit = pCar2;
                victim = pCar1;
            }
            total_units_of_damage = 0;
            for (i = 0; i < COUNT_OF(victim->damage_units); i++) {
                if (victim->damage_units[i].damage_level > victim->damage_units[i].last_level) {
                    total_units_of_damage += victim->damage_units[i].damage_level - victim->damage_units[i].last_level;
                }
            }
        }
        if (culprit && (culprit->driver == eDriver_local_human || gNet_mode) && victim) {
            SetKnackeredFlag(victim);
            if (victim->knackered && !victim->pre_car_col_knackered) {
                victim->pre_car_col_knackered = 1;
                credits_squared = sqr(0.7f / victim->car_model_actors[victim->principal_car_actor].crush_data.softness_factor) * gWasted_creds[gProgram_state.skill_level] + 50.0f;
                credits = 100 * (int)(credits_squared / 100.0f);
                if (gNet_mode) {
                    message = NetBuildMessage(NETMSGID_WASTED, 0);
                    message->contents.data.wasted.victim = NetPlayerFromCar(victim)->ID;
                    if (NetPlayerFromCar(culprit)) {
                        message->contents.data.wasted.culprit = NetPlayerFromCar(culprit)->ID;
                    } else {
                        message->contents.data.wasted.culprit = -2;
                    }
                    NetGuaranteedSendMessageToEverybody(gCurrent_net_game, message, NULL);
                    NetEarnCredits(NetPlayerFromCar(culprit), credits);
                } else {
                    PratcamEvent(kPratcam_opponent_wasted);
                    DoFancyHeadup(kFancyHeadupYouWastedEm);
                    credits_squared = sqr(0.7f / victim->car_model_actors[victim->principal_car_actor].crush_data.softness_factor) * gWasted_creds[gProgram_state.skill_level] + 50.0f;
                    credits = 100 * (int)(credits_squared / 100.0);
                    AwardTime(gWasted_time[gProgram_state.skill_level]);
                    EarnCredits(credits);
                    if (victim->can_be_stolen && !gOpponents[victim->index].dead
                        // strength_rating is between 1 and 5
                        && ((PercentageChance(50) && gProgram_state.rank <= gSteal_ranks[gOpponents[victim->index].strength_rating - 1]) || victim->index == BIGAPC_OPPONENT_INDEX)) {
                        StealCar(victim);
                    }
                }
            }
            victim->time_last_hit = the_time;
            victim->last_hit_by = culprit;
            if (!inherited_damage) {
                victim->time_last_victim = the_time;
                victim->last_culprit = culprit;
            }
            if (victim && (fabs(victim->omega.v[0]) > 4.0f || fabs(victim->omega.v[1]) > 6.0f || fabs(victim->omega.v[2]) > 4.0f)) {
                bonus_level *= 2;
            }
            if (pCar1->number_of_wheels_on_ground) {
                car_off_ground_1 = 0;
            } else {
                BrVector3InvScale(&car_1_pos, &pCar1->car_master_actor->t.t.translate.t, WORLD_SCALE);
                BrMatrix34ApplyV(&car_1_offset, &pCar1->car_model_actors[pCar1->principal_car_actor].actor->t.t.translate.t, &pCar1->car_master_actor->t.t.mat);
                BrVector3Accumulate(&car_1_pos, &car_1_offset);
                car_1_pos.v[1] += 0.15f;
                car_1_height = FindYVerticallyBelow2(&car_1_pos);
                car_off_ground_1 = car_1_height > -100.0f
                    && pCar1->car_model_actors[pCar1->principal_car_actor].actor->t.t.translate.t.v[1] * 4.0f <= car_1_pos.v[1] - car_1_height - 0.15f;
            }
            if (!pCar2 || pCar2->number_of_wheels_on_ground) {
                car_off_ground_2 = 0;
            } else {
                BrVector3InvScale(&car_2_pos, &pCar2->car_master_actor->t.t.translate.t, WORLD_SCALE);
                BrMatrix34ApplyV(&car_2_offset, &pCar2->car_model_actors[pCar2->principal_car_actor].actor->t.t.translate.t, &pCar2->car_master_actor->t.t.mat);
                BrVector3Accumulate(&car_2_pos, &car_2_offset);
                car_2_pos.v[1] += 0.15f;
                car_2_height = FindYVerticallyBelow2(&car_2_pos);
                car_off_ground_2 = car_2_height > -100.0f
                    && pCar2->car_model_actors[pCar2->principal_car_actor].actor->t.t.translate.t.v[1] * 4.0f <= car_2_pos.v[1] - car_2_height - 0.15f;
            }
            if (car_off_ground_1) {
                bonus_level *= 2;
            }
            if (car_off_ground_2) {
                bonus_level *= 2;
            }
            total_units_of_damage = 0.7f / victim->car_model_actors[victim->principal_car_actor].crush_data.softness_factor * total_units_of_damage;
            if (!victim->has_been_stolen) {
                credits = 100 * (int)((gCar_cred_value[gProgram_state.skill_level] * MIN(bonus_level, 8) * total_units_of_damage + 50.0f) / 100.0f);
                if (credits || victim->knackered) {
                    if (!victim->knackered) {
                        if (gNet_mode) {
                            NetEarnCredits(NetPlayerFromCar(culprit), MIN(credits, 2000));
                        } else {
                            EarnCredits(MIN(credits, 2000));
                        }
                        last_earn_time = the_time;
                        if (gNet_mode == eNet_mode_none) {
                            time = 5 * (int)((total_units_of_damage * gCar_time_value[gProgram_state.skill_level] + 2.5f) / 5.0f);
                            AwardTime(MIN(time, 90));
                            if (pCar2) {
                                if (head_on) {
                                    DoFancyHeadup(kFancyHeadupHeadOnBonus);
                                } else if (bonus_level <= 2) {
                                    if (bonus_level > 1) {
                                        DoFancyHeadup(kFancyHeadupExtraStyleBonus);
                                    }
                                } else {
                                    DoFancyHeadup(kFancyHeadupBonusForArtisticImpression);
                                }
                            }
                        }
                    }
                    for (i = 0; i < COUNT_OF(victim->damage_units); i++) {
                        victim->damage_units[i].last_level = victim->damage_units[i].damage_level;
                    }
                }
            }
        } else {
            pCar1->time_last_hit = 0;
            if (pCar2) {
                pCar2->time_last_hit = 0;
            }
        }
    }
    pCar1->damage_magnitude_accumulator = 0.0f;
    if (pCar2) {
        pCar2->damage_magnitude_accumulator = 0.0f;
    }
    return 1;
}

// IDA: void __usercall DoWheelDamage(tU32 pFrame_period@<EAX>)
// FUNCTION: CARM95 0x004bf96c
void DoWheelDamage(tU32 pFrame_period) {
    int i;
    int j;
    int damage;
    tCar_spec* car;
    br_scalar y_amount;
    br_scalar z_amount;
    br_scalar wheel_circum;
    br_scalar old_offset;
    br_vector3 temp_vector;
    br_vector3 wonky_vector;
    static int kev_index[4];

    if (gAction_replay_mode && ReplayIsPaused()) {
        return;
    }

    for (i = 0; i < gNum_active_cars; i++) {
        car = gActive_car_list[i];
        for (j = 0; j < COUNT_OF(car->wheel_dam_offset); j++) {
            if (car->wheel_actors[j] == NULL) {
                continue;
            }
            old_offset = car->wheel_dam_offset[j];
            damage = car->damage_units[j + 8].damage_level;
            if (damage <= 30 || gRace_finished) {
                car->wheel_dam_offset[j] = 0.0f;
                continue;
            }
            if (PointOutOfSight(&car->pos, 32.0f)) {
                break;
            }
            y_amount = (damage - 30) * gWobble_spam_y[damage & 7];
            z_amount = (damage - 30) * gWobble_spam_z[damage & 7];
            BrMatrix34PreRotateY(&car->wheel_actors[j]->t.t.mat, BrDegreeToAngle(y_amount < 0 ? y_amount + 360.f : y_amount));
            BrMatrix34PreRotateZ(&car->wheel_actors[j]->t.t.mat, BrDegreeToAngle(z_amount < 0 ? z_amount + 360.f : z_amount));
            if (j < 2 && car->wheel_actors[j + 4] != NULL) {
                BrMatrix34PreRotateY(&car->wheel_actors[j + 4]->t.t.mat, BrDegreeToAngle(y_amount < 0 ? y_amount + 360.f : y_amount));
                BrMatrix34PreRotateZ(&car->wheel_actors[j + 4]->t.t.mat, BrDegreeToAngle(z_amount < 0 ? z_amount + 360.f : z_amount));
            }
            switch (j) {
            case 0:
                if (car->driven_wheels_spin_ref_1 < 0) {
                    wheel_circum = car->non_driven_wheels_circum;
                } else {
                    wheel_circum = car->driven_wheels_circum;
                }
                break;
            case 1:
                if (car->driven_wheels_spin_ref_2 < 0) {
                    wheel_circum = car->non_driven_wheels_circum;
                } else {
                    wheel_circum = car->driven_wheels_circum;
                }
                break;
            case 2:
                if (car->driven_wheels_spin_ref_3 < 0) {
                    wheel_circum = car->non_driven_wheels_circum;
                } else {
                    wheel_circum = car->driven_wheels_circum;
                }
                break;
            case 3:
                if (car->driven_wheels_spin_ref_4 < 0) {
                    wheel_circum = car->non_driven_wheels_circum;
                } else {
                    wheel_circum = car->driven_wheels_circum;
                }
                break;
            default:
                TELL_ME_IF_WE_PASS_THIS_WAY();
                break;
            }
            if (gNet_mode == eNet_mode_none || car->driver == eDriver_local_human) {
                BrVector3Set(&temp_vector, wheel_circum * gWheel_circ_to_width, 0.f, 0.f);
                BrMatrix34ApplyV(&wonky_vector, &temp_vector, &car->wheel_actors[j]->t.t.mat);
                car->wheel_dam_offset[j] = fabs(wonky_vector.v[1]);
            }
        }
    }
}

// IDA: void __usercall CrashEarnings(tCar_spec *pCar1@<EAX>, tCar_spec *pCar2@<EDX>)
// FUNCTION: CARM95 0x004bfd78
void CrashEarnings(tCar_spec* pCar1, tCar_spec* pCar2) {

    if (DoCrashEarnings(pCar1, pCar2)) {
        SortOutSmoke(pCar1);
        SortOutSmoke(pCar2);
    }
}
