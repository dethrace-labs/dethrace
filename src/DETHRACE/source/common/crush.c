#include "crush.h"

float gWobble_spam_y[8];
float gWobble_spam_z[8];
int gSteal_ranks[5];
tU8 gSmoke_damage_step[12];
br_scalar gWheel_circ_to_width;

// Offset: 0
// Size: 620
// EAX: pF
// EDX: pCrush_data
int ReadCrushData(FILE *pF, tCrush_data *pCrush_data) {
    char s[256];
    char *str;
    int i;
    int j;
    int k;
    tCrush_point_spec *the_spec;
    tCrush_neighbour *the_neighbour;
}

// Offset: 620
// Size: 230
// EAX: pF
float SkipCrushData(FILE *pF) {
    int i;
    int j;
    int count_1;
    int count_2;
    char s[256];
    float softness;
}

// Offset: 852
// Size: 702
// EAX: pF
// EDX: pCrush_data
int WriteCrushData(FILE *pF, tCrush_data *pCrush_data) {
    int i;
    int j;
    int k;
    tCrush_point_spec *the_spec;
    tCrush_neighbour *the_neighbour;
}

// Offset: 1556
// Size: 130
// EAX: pCrush_data
void DisposeCrushData(tCrush_data *pCrush_data) {
    int i;
}

// Offset: 1688
// Size: 1656
// EAX: pCar
// EDX: pModel_index
// EBX: pModel
// ECX: pCrush_point_index
void CrushModelPoint(tCar_spec *pCar, int pModel_index, br_model *pModel, int pCrush_point_index, br_vector3 *pEnergy_vector, br_scalar total_energy, tCrush_data *pCrush_data) {
    int i;
    int j;
    int k;
    int pipe_vertex_count;
    int neighbour_index;
    int bend_axis;
    int default_bend_axis[3];
    tCrush_point_spec *the_crush_point;
    tCrush_neighbour *the_neighbour;
    br_vector3 *target_point;
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
    tCar_spec *car;
}

// Offset: 3344
// Size: 527
// EAX: pCar
// EDX: pModel_index
// EBX: pActor
// ECX: pImpact_point
void CrushModel(tCar_spec *pCar, int pModel_index, br_actor *pActor, br_vector3 *pImpact_point, br_vector3 *pEnergy_vector, tCrush_data *pCrush_data) {
    br_scalar this_distance;
    br_scalar total_energy;
    br_scalar nearest_so_far;
    br_vector3 impact_point_model;
    br_vector3 energy_vector_scaled;
    br_vector3 energy_vector_model;
    int i;
    int nearest_index;
    br_vertex *vertices;
    br_vertex *the_vertex;
    br_matrix34 inverse_transform;
}

// Offset: 3872
// Size: 99
void JitModelUpdate(br_actor *actor, br_model *model, br_material *material, void *render_data, br_uint_8 style, int on_screen) {
}

// Offset: 3972
// Size: 131
// EAX: pModel
// EDX: pCar
// EBX: crush_only
void SetModelForUpdate(br_model *pModel, tCar_spec *pCar, int crush_only) {
}

// Offset: 4104
// Size: 380
// EAX: pCar
// EDX: pModel_index
// EBX: pActor
// ECX: pCrush_data
void TotallySpamTheModel(tCar_spec *pCar, int pModel_index, br_actor *pActor, tCrush_data *pCrush_data, br_scalar pMagnitude) {
    br_scalar total_energy;
    br_vector3 energy_vector_model;
    int i;
    int the_index;
    br_vertex *the_vertex;
    br_vertex *vertices;
}

// Offset: 4484
// Size: 662
// EAX: pCar
// EDX: pModel_index
// EBX: pActor
// ECX: pUndamaged_vertices
br_scalar RepairModel(tCar_spec *pCar, int pModel_index, br_actor *pActor, br_vertex *pUndamaged_vertices, br_scalar pAmount, br_scalar *pTotal_deflection) {
    int i;
    int j;
    int pipe_vertex_count;
    br_vector3 old_point;
    br_vertex *model_vertex;
    br_scalar amount;
    br_scalar deviation;
    tChanged_vertex pipe_array[600];
}

// Offset: 5148
// Size: 298
// EAX: pCar
// EDX: pFrame_period
// EBX: pTotal_deflection
float RepairCar2(tCar_spec *pCar, tU32 pFrame_period, br_scalar *pTotal_deflection) {
    int i;
    tCar_actor *the_car_actor;
    br_scalar amount;
    br_scalar dummy;
}

// Offset: 5448
// Size: 120
// EAX: pCar_ID
// EDX: pFrame_period
// EBX: pTotal_deflection
float RepairCar(tU16 pCar_ID, tU32 pFrame_period, br_scalar *pTotal_deflection) {
}

// Offset: 5568
// Size: 710
// EAX: pCar
void TotallyRepairACar(tCar_spec *pCar) {
    int i;
    int j;
    int k;
    int pipe_vertex_count;
    tCar_actor *the_car_actor;
    tChanged_vertex pipe_array[600];
    br_bounds storage_bounds;
}

// Offset: 6280
// Size: 53
void TotallyRepairCar() {
}

// Offset: 6336
// Size: 111
void CheckLastCar() {
}

// Offset: 6448
// Size: 102
// EAX: pCar
void KnackerThisCar(tCar_spec *pCar) {
}

// Offset: 6552
// Size: 278
// EAX: pCar
void SetKnackeredFlag(tCar_spec *pCar) {
}

// Offset: 6832
// Size: 270
// EAX: pCar
// EDX: pUnit_type
// EBX: pDamage_amount
void DamageUnit2(tCar_spec *pCar, int pUnit_type, int pDamage_amount) {
    tDamage_unit *the_damage;
    int last_level;
}

// Offset: 7104
// Size: 236
// EAX: pCar
void RecordLastDamage(tCar_spec *pCar) {
    int i;
}

// Offset: 7340
// Size: 249
// EAX: pCar
// EDX: pDamage_type
void DoDamage(tCar_spec *pCar, tDamage_type pDamage_type, float pMagnitude, float pNastiness) {
}

// Offset: 7592
// Size: 650
// EAX: pCar
// EDX: pImpact_point
// EBX: pEnergy
void CheckPiledriverBonus(tCar_spec *pCar, br_vector3 *pImpact_point, br_vector3 *pEnergy) {
    br_actor *child;
    br_vector3 norm_impact;
    br_vector3 norm_child;
    br_vector3 norm_energy;
    br_scalar dp;
}

// Offset: 8244
// Size: 152
// EAX: pCar
tImpact_location CalcModifiedLocation(tCar_spec *pCar) {
}

// Offset: 8396
// Size: 222
// EAX: pHit_vector
void DoPratcamHit(br_vector3 *pHit_vector) {
    int strength_modifier;
    br_scalar strength;
}

// Offset: 8620
// Size: 2217
// EAX: pCar
// EDX: pImpact_point
// EBX: pEnergy_vector
// ECX: pWas_hitting_a_car
void DamageSystems(tCar_spec *pCar, br_vector3 *pImpact_point, br_vector3 *pEnergy_vector, int pWas_hitting_a_car) {
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
    tDamage_program *the_program;
    tDamage_clause *the_clause;
    tDamage_condition *the_condition;
    tDamage_effect *the_effect;
    tImpact_location modified_location;
}

// Offset: 10840
// Size: 210
// EAX: pVelocity
tImpact_location GetDirection(br_vector3 *pVelocity) {
    br_scalar mag_x;
    br_scalar mag_y;
    br_scalar mag_z;
}

// Offset: 11052
// Size: 92
// EAX: pCar
void SetSmokeLastDamageLevel(tCar_spec *pCar) {
    int i;
}

// Offset: 11144
// Size: 318
// EAX: pCar
void SortOutSmoke(tCar_spec *pCar) {
    int i;
    int colour;
    int old_colour;
    int step;
    int pass;
    int repeat;
}

// Offset: 11464
// Size: 103
// EAX: pCar
void StealCar(tCar_spec *pCar) {
}

// Offset: 11568
// Size: 5298
// EAX: pCar1
// EDX: pCar2
int DoCrashEarnings(tCar_spec *pCar1, tCar_spec *pCar2) {
    tCar_spec *culprit;
    tCar_spec *victim;
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
    tU32 last_earn_time;
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
    tNet_message *message;
}

// Offset: 16868
// Size: 1028
// EAX: pFrame_period
void DoWheelDamage(tU32 pFrame_period) {
    int i;
    int j;
    int damage;
    tCar_spec *car;
    br_scalar y_amount;
    br_scalar z_amount;
    br_scalar wheel_circum;
    br_scalar old_offset;
    br_vector3 temp_vector;
    br_vector3 wonky_vector;
    int kev_index[4];
}

// Offset: 17896
// Size: 69
// EAX: pCar1
// EDX: pCar2
void CrashEarnings(tCar_spec *pCar1, tCar_spec *pCar2) {
}

