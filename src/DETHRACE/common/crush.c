#include "crush.h"
#include "brender/brender.h"
#include "car.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrpb.h"
#include "graphics.h"
#include "harness/trace.h"
#include "loading.h"
#include "netgame.h"
#include "oil.h"
#include "opponent.h"
#include "piping.h"
#include "replay.h"
#include "spark.h"
#include "utility.h"
#include "world.h"
#include <stdlib.h>

float gWobble_spam_y[8] = { 0.0f, -0.15f, 0.4f, 0.15f, -0.4f, 0.25f, 0.0f, -0.25f };
float gWobble_spam_z[8] = { 0.4f, -0.25f, 0.0f, 0.25f, 0.0f, 0.15f, -0.4f, -0.15f };
br_scalar gWheel_circ_to_width = 0.16f;
tU8 gSmoke_damage_step[12] = { 20u, 20u, 0u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u };
int gSteal_ranks[5];

// IDA: int __usercall ReadCrushData@<EAX>(FILE *pF@<EAX>, tCrush_data *pCrush_data@<EDX>)
int ReadCrushData(FILE* pF, tCrush_data* pCrush_data) {
    char s[256];
    char* str;
    int i;
    int j;
    int k;
    tCrush_point_spec* the_spec;
    tCrush_neighbour* the_neighbour;
    LOG_TRACE("(%p, %p)", pF, pCrush_data);

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
float SkipCrushData(FILE* pF) {
    int i;
    int j;
    int count_1;
    int count_2;
    char s[256];
    float softness;
    LOG_TRACE("(%p)", pF);

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
int WriteCrushData(FILE* pF, tCrush_data* pCrush_data) {
    int i;
    int j;
    int k;
    tCrush_point_spec* the_spec;
    tCrush_neighbour* the_neighbour;
    LOG_TRACE("(%p, %p)", pF, pCrush_data);

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
void DisposeCrushData(tCrush_data* pCrush_data) {
    int i;
    LOG_TRACE("(%p)", pCrush_data);

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
    LOG_TRACE("(%p, %d, %p, %d, %p, %f, %p)", pCar, pModel_index, pModel, pCrush_point_index, pEnergy_vector, total_energy, pCrush_data);

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
                float v12;
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
                int axis_tmp = (((int)((target_point->v[2] + target_point->v[1] + target_point->v[0]) * 100.0f) + bend_axis - 1) & 1) % 3;
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
    LOG_TRACE("(%p, %d, %p, %p, %p, %p)", pCar, pModel_index, pActor, pImpact_point, pEnergy_vector, pCrush_data);

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
void JitModelUpdate(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", actor, model, material, render_data, style, on_screen);

    BrModelUpdate(model, BR_MODU_NORMALS);
    model->flags &= ~(BR_MODF_CUSTOM);
    BrZbModelRender(actor, model, material, style, BrOnScreenCheck(&model->bounds), 0);
}

// IDA: void __usercall SetModelForUpdate(br_model *pModel@<EAX>, tCar_spec *pCar@<EDX>, int crush_only@<EBX>)
void SetModelForUpdate(br_model* pModel, tCar_spec* pCar, int crush_only) {
    LOG_TRACE("(%p, %p, %d)", pModel, pCar, crush_only);

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
void TotallySpamTheModel(tCar_spec* pCar, int pModel_index, br_actor* pActor, tCrush_data* pCrush_data, br_scalar pMagnitude) {
    br_scalar total_energy;
    br_vector3 energy_vector_model;
    int i;
    int the_index;
    br_vertex* the_vertex;
    br_vertex* vertices;
    LOG_TRACE("(%p, %d, %p, %p, %f)", pCar, pModel_index, pActor, pCrush_data, pMagnitude);

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
br_scalar RepairModel(tCar_spec* pCar, int pModel_index, br_actor* pActor, br_vertex* pUndamaged_vertices, br_scalar pAmount, br_scalar* pTotal_deflection) {
    int i;
    int j;
    int pipe_vertex_count;
    br_vector3 old_point;
    br_vertex* model_vertex;
    br_scalar amount;
    br_scalar deviation;
    tChanged_vertex pipe_array[600];
    LOG_TRACE("(%p, %d, %p, %p, %f, %p)", pCar, pModel_index, pActor, pUndamaged_vertices, pAmount, pTotal_deflection);

    pipe_vertex_count = 0;
    amount = 0.0f;
    *pTotal_deflection = 0.0f;

    for (i = 0; i < pActor->model->nvertices; i++) {
        model_vertex = &pActor->model->vertices[i];
        old_point = model_vertex->p;
        for (j = 0; j < 3; ++j) {
            *pTotal_deflection = fabsf(pUndamaged_vertices->p.v[j] - old_point.v[j]) + *pTotal_deflection;
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
float RepairCar2(tCar_spec* pCar, tU32 pFrame_period, br_scalar* pTotal_deflection) {
    int i;
    tCar_actor* the_car_actor;
    br_scalar amount;
    br_scalar dummy;
    LOG_TRACE("(%p, %d, %p)", pCar, pFrame_period, pTotal_deflection);

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
float RepairCar(tU16 pCar_ID, tU32 pFrame_period, br_scalar* pTotal_deflection) {
    LOG_TRACE("(%d, %d, %p)", pCar_ID, pFrame_period, pTotal_deflection);

    if (VEHICLE_TYPE_FROM_ID(pCar_ID) == eVehicle_self) {
        return RepairCar2(&gProgram_state.current_car, pFrame_period, pTotal_deflection);
    }

    return RepairCar2(GetCarSpec(VEHICLE_TYPE_FROM_ID(pCar_ID), VEHICLE_INDEX_FROM_ID(pCar_ID)), pFrame_period, pTotal_deflection);
}

// IDA: void __usercall TotallyRepairACar(tCar_spec *pCar@<EAX>)
void TotallyRepairACar(tCar_spec* pCar) {
    int i;
    int j;
    int k;
    int pipe_vertex_count;
    tCar_actor* the_car_actor;
    tChanged_vertex pipe_array[600];
    br_bounds storage_bounds;
    LOG_TRACE("(%p)", pCar);

    STUB();
}

// IDA: void __cdecl TotallyRepairCar()
void TotallyRepairCar() {
    LOG_TRACE("()");

    if (!gArrow_mode) {
        TotallyRepairACar(&gProgram_state.current_car);
    }
}

// IDA: void __cdecl CheckLastCar()
void CheckLastCar() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall KnackerThisCar(tCar_spec *pCar@<EAX>)
void KnackerThisCar(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetKnackeredFlag(tCar_spec *pCar@<EAX>)
void SetKnackeredFlag(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DamageUnit2(tCar_spec *pCar@<EAX>, int pUnit_type@<EDX>, int pDamage_amount@<EBX>)
void DamageUnit2(tCar_spec* pCar, int pUnit_type, int pDamage_amount) {
    tDamage_unit* the_damage;
    int last_level;
    LOG_TRACE("(%p, %d, %d)", pCar, pUnit_type, pDamage_amount);

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
void RecordLastDamage(tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p)", pCar);

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
void DoDamage(tCar_spec* pCar, tDamage_type pDamage_type, float pMagnitude, float pNastiness) {
    LOG_TRACE("(%p, %d, %f, %f)", pCar, pDamage_type, pMagnitude, pNastiness);

    if (pCar->driver < eDriver_net_human) {
        DamageUnit2(pCar, pDamage_type, ((gCurrent_race.suggested_rank < 10 ? 0.5f : gCurrent_race.suggested_rank) / 20.0f + 1.0f) * (pNastiness * pMagnitude * 10.0f));
    } else if (gNet_mode != eNet_mode_none) {
        DamageUnit2(pCar, pDamage_type, pNastiness * pMagnitude * 15.0f);
    } else if (PercentageChance(pNastiness * pMagnitude * 1500.0f)) {
        DamageUnit2(pCar, pDamage_type, pNastiness * pMagnitude * 30.0f);
    }
}

// IDA: void __usercall CheckPiledriverBonus(tCar_spec *pCar@<EAX>, br_vector3 *pImpact_point@<EDX>, br_vector3 *pEnergy@<EBX>)
void CheckPiledriverBonus(tCar_spec* pCar, br_vector3* pImpact_point, br_vector3* pEnergy) {
    br_actor* child;
    br_vector3 norm_impact;
    br_vector3 norm_child;
    br_vector3 norm_energy;
    br_scalar dp;
    LOG_TRACE("(%p, %p, %p)", pCar, pImpact_point, pEnergy);

    STUB();
}

// IDA: tImpact_location __usercall CalcModifiedLocation@<EAX>(tCar_spec *pCar@<EAX>)
tImpact_location CalcModifiedLocation(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoPratcamHit(br_vector3 *pHit_vector@<EAX>)
void DoPratcamHit(br_vector3* pHit_vector) {
    int strength_modifier;
    br_scalar strength;
    LOG_TRACE("(%p)", pHit_vector);

    STUB_ONCE();
}

// IDA: void __usercall DamageSystems(tCar_spec *pCar@<EAX>, br_vector3 *pImpact_point@<EDX>, br_vector3 *pEnergy_vector@<EBX>, int pWas_hitting_a_car@<ECX>)
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
    LOG_TRACE("(%p, %p, %p, %d)", pCar, pImpact_point, pEnergy_vector, pWas_hitting_a_car);

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
tImpact_location GetDirection(br_vector3* pVelocity) {
    br_scalar mag_x;
    br_scalar mag_y;
    br_scalar mag_z;
    LOG_TRACE("(%p)", pVelocity);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetSmokeLastDamageLevel(tCar_spec *pCar@<EAX>)
void SetSmokeLastDamageLevel(tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p)", pCar);

    for (i = 0; i < COUNT_OF(pCar->damage_units); i++) {
        pCar->damage_units[i].smoke_last_level = pCar->damage_units[i].damage_level;
    }
}

// IDA: void __usercall SortOutSmoke(tCar_spec *pCar@<EAX>)
void SortOutSmoke(tCar_spec* pCar) {
    int i;
    int colour;
    int old_colour;
    int step;
    int pass;
    int repeat;
    LOG_TRACE("(%p)", pCar);

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
void StealCar(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);

    pCar->has_been_stolen = 1;
    gProgram_state.cars_available[gProgram_state.number_of_cars] = pCar->index;
    gProgram_state.number_of_cars++;
    gOpponents[pCar->index].dead = 1;
}

// IDA: int __usercall DoCrashEarnings@<EAX>(tCar_spec *pCar1@<EAX>, tCar_spec *pCar2@<EDX>)
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
    LOG_TRACE("(%p, %p)", pCar1, pCar2);

    STUB();
}

// IDA: void __usercall DoWheelDamage(tU32 pFrame_period@<EAX>)
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
    LOG_TRACE("(%d)", pFrame_period);

    if (gAction_replay_mode && ReplayIsPaused()) {
        return;
    }

    for (i = 0; i < gNum_active_cars; i++) {
        car = gActive_car_list[i];
        for (j = 0; j < COUNT_OF(car->wheel_actors); j++) {
            if (!car->wheel_actors[j]) {
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
                break;
            }
            if (gNet_mode == eNet_mode_none || car->driver == eDriver_local_human) {
                temp_vector.v[0] = wheel_circum * gWheel_circ_to_width;
                temp_vector.v[1] = 0.0f;
                temp_vector.v[2] = 0.0f;
                BrMatrix34ApplyV(&wonky_vector, &temp_vector, &car->wheel_actors[j]->t.t.mat);
                car->wheel_dam_offset[j] = fabsf(wonky_vector.v[1]);
            }
        }
    }
}

// IDA: void __usercall CrashEarnings(tCar_spec *pCar1@<EAX>, tCar_spec *pCar2@<EDX>)
void CrashEarnings(tCar_spec* pCar1, tCar_spec* pCar2) {
    LOG_TRACE("(%p, %p)", pCar1, pCar2);

    if (DoCrashEarnings(pCar1, pCar2)) {
        SortOutSmoke(pCar1);
        SortOutSmoke(pCar2);
    }
}
