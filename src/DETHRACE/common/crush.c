#include "crush.h"
#include "brender.h"
#include "loading.h"
#include "utility.h"
#include <stdlib.h>

float gWobble_spam_y[8];
float gWobble_spam_z[8];
br_scalar gWheel_circ_to_width;
tU8 gSmoke_damage_step[12];
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

    the_spec = pCrush_data->crush_points;
    for (i = 0; i < pCrush_data->number_of_crush_points; i++) {
        the_spec->vertex_index = GetAnInt(pF);
        GetThreeFloats(pF, &the_spec->limits_neg.v[0], &the_spec->limits_neg.v[1], &the_spec->limits_neg.v[2]);
        GetThreeFloats(pF, &the_spec->limits_pos.v[0], &the_spec->limits_pos.v[1], &the_spec->limits_pos.v[2]);
        GetThreeFloats(pF, &the_spec->softness_neg.v[0], &the_spec->softness_neg.v[1], &the_spec->softness_neg.v[2]);
        GetThreeFloats(pF, &the_spec->softness_pos.v[0], &the_spec->softness_pos.v[1], &the_spec->softness_pos.v[2]);
        the_spec->number_of_neighbours = GetAnInt(pF);
        the_spec->neighbours = BrMemAllocate(sizeof(tCrush_neighbour) * the_spec->number_of_neighbours, kMem_crush_neighbours);

        the_neighbour = the_spec->neighbours;
        for (j = 0; j < the_spec->number_of_neighbours; j++) {
            the_neighbour->vertex_index = GetAnInt(pF);
            the_neighbour->factor = GetAnInt(pF);
            the_neighbour++;
        }
        the_spec++;
    }
    LOG_DEBUG("exit");
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DisposeCrushData(tCrush_data *pCrush_data@<EAX>)
void DisposeCrushData(tCrush_data* pCrush_data) {
    int i;
    LOG_TRACE("(%p)", pCrush_data);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl JitModelUpdate(br_actor *actor, br_model *model, br_material *material, void *render_data, br_uint_8 style, int on_screen)
void JitModelUpdate(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", actor, model, material, render_data, style, on_screen);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetModelForUpdate(br_model *pModel@<EAX>, tCar_spec *pCar@<EDX>, int crush_only@<EBX>)
void SetModelForUpdate(br_model* pModel, tCar_spec* pCar, int crush_only) {
    LOG_TRACE("(%p, %p, %d)", pModel, pCar, crush_only);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: float __usercall RepairCar2@<ST0>(tCar_spec *pCar@<EAX>, tU32 pFrame_period@<EDX>, br_scalar *pTotal_deflection@<EBX>)
float RepairCar2(tCar_spec* pCar, tU32 pFrame_period, br_scalar* pTotal_deflection) {
    int i;
    tCar_actor* the_car_actor;
    br_scalar amount;
    br_scalar dummy;
    LOG_TRACE("(%p, %d, %p)", pCar, pFrame_period, pTotal_deflection);
    NOT_IMPLEMENTED();
}

// IDA: float __usercall RepairCar@<ST0>(tU16 pCar_ID@<EAX>, tU32 pFrame_period@<EDX>, br_scalar *pTotal_deflection@<EBX>)
float RepairCar(tU16 pCar_ID, tU32 pFrame_period, br_scalar* pTotal_deflection) {
    LOG_TRACE("(%d, %d, %p)", pCar_ID, pFrame_period, pTotal_deflection);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RecordLastDamage(tCar_spec *pCar@<EAX>)
void RecordLastDamage(tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p)", pCar);

    STUB();
}

// IDA: void __usercall DoDamage(tCar_spec *pCar@<EAX>, tDamage_type pDamage_type@<EDX>, float pMagnitude, float pNastiness)
void DoDamage(tCar_spec* pCar, tDamage_type pDamage_type, float pMagnitude, float pNastiness) {
    LOG_TRACE("(%p, %d, %f, %f)", pCar, pDamage_type, pMagnitude, pNastiness);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CheckPiledriverBonus(tCar_spec *pCar@<EAX>, br_vector3 *pImpact_point@<EDX>, br_vector3 *pEnergy@<EBX>)
void CheckPiledriverBonus(tCar_spec* pCar, br_vector3* pImpact_point, br_vector3* pEnergy) {
    br_actor* child;
    br_vector3 norm_impact;
    br_vector3 norm_child;
    br_vector3 norm_energy;
    br_scalar dp;
    LOG_TRACE("(%p, %p, %p)", pCar, pImpact_point, pEnergy);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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

    STUB();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall StealCar(tCar_spec *pCar@<EAX>)
void StealCar(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    SILENT_STUB();
}

// IDA: void __usercall CrashEarnings(tCar_spec *pCar1@<EAX>, tCar_spec *pCar2@<EDX>)
void CrashEarnings(tCar_spec* pCar1, tCar_spec* pCar2) {
    LOG_TRACE("(%p, %p)", pCar1, pCar2);
    NOT_IMPLEMENTED();
}
