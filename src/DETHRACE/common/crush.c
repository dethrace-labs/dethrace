#include "crush.h"
#include <stdlib.h>

float gWobble_spam_y[8];
float gWobble_spam_z[8];
int gSteal_ranks[5];
tU8 gSmoke_damage_step[12];
br_scalar gWheel_circ_to_width;

// Offset: 0
// Size: 620
int ReadCrushData(FILE* pF, tCrush_data* pCrush_data) {
    char s[256];
    char* str;
    int i;
    int j;
    int k;
    tCrush_point_spec* the_spec;
    tCrush_neighbour* the_neighbour;
    NOT_IMPLEMENTED();
}

// Offset: 620
// Size: 230
float SkipCrushData(FILE* pF) {
    int i;
    int j;
    int count_1;
    int count_2;
    char s[256];
    float softness;
    NOT_IMPLEMENTED();
}

// Offset: 852
// Size: 702
int WriteCrushData(FILE* pF, tCrush_data* pCrush_data) {
    int i;
    int j;
    int k;
    tCrush_point_spec* the_spec;
    tCrush_neighbour* the_neighbour;
    NOT_IMPLEMENTED();
}

// Offset: 1556
// Size: 130
void DisposeCrushData(tCrush_data* pCrush_data) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 1688
// Size: 1656
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
    NOT_IMPLEMENTED();
}

// Offset: 3344
// Size: 527
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
    NOT_IMPLEMENTED();
}

// Offset: 3872
// Size: 99
void JitModelUpdate(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    NOT_IMPLEMENTED();
}

// Offset: 3972
// Size: 131
void SetModelForUpdate(br_model* pModel, tCar_spec* pCar, int crush_only) {
    NOT_IMPLEMENTED();
}

// Offset: 4104
// Size: 380
void TotallySpamTheModel(tCar_spec* pCar, int pModel_index, br_actor* pActor, tCrush_data* pCrush_data, br_scalar pMagnitude) {
    br_scalar total_energy;
    br_vector3 energy_vector_model;
    int i;
    int the_index;
    br_vertex* the_vertex;
    br_vertex* vertices;
    NOT_IMPLEMENTED();
}

// Offset: 4484
// Size: 662
br_scalar RepairModel(tCar_spec* pCar, int pModel_index, br_actor* pActor, br_vertex* pUndamaged_vertices, br_scalar pAmount, br_scalar* pTotal_deflection) {
    int i;
    int j;
    int pipe_vertex_count;
    br_vector3 old_point;
    br_vertex* model_vertex;
    br_scalar amount;
    br_scalar deviation;
    tChanged_vertex pipe_array[600];
    NOT_IMPLEMENTED();
}

// Offset: 5148
// Size: 298
float RepairCar2(tCar_spec* pCar, tU32 pFrame_period, br_scalar* pTotal_deflection) {
    int i;
    tCar_actor* the_car_actor;
    br_scalar amount;
    br_scalar dummy;
    NOT_IMPLEMENTED();
}

// Offset: 5448
// Size: 120
float RepairCar(tU16 pCar_ID, tU32 pFrame_period, br_scalar* pTotal_deflection) {
    NOT_IMPLEMENTED();
}

// Offset: 5568
// Size: 710
void TotallyRepairACar(tCar_spec* pCar) {
    int i;
    int j;
    int k;
    int pipe_vertex_count;
    tCar_actor* the_car_actor;
    tChanged_vertex pipe_array[600];
    br_bounds storage_bounds;
    NOT_IMPLEMENTED();
}

// Offset: 6280
// Size: 53
void TotallyRepairCar() {
    NOT_IMPLEMENTED();
}

// Offset: 6336
// Size: 111
void CheckLastCar() {
    NOT_IMPLEMENTED();
}

// Offset: 6448
// Size: 102
void KnackerThisCar(tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 6552
// Size: 278
void SetKnackeredFlag(tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 6832
// Size: 270
void DamageUnit2(tCar_spec* pCar, int pUnit_type, int pDamage_amount) {
    tDamage_unit* the_damage;
    int last_level;
    NOT_IMPLEMENTED();
}

// Offset: 7104
// Size: 236
void RecordLastDamage(tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 7340
// Size: 249
void DoDamage(tCar_spec* pCar, tDamage_type pDamage_type, float pMagnitude, float pNastiness) {
    NOT_IMPLEMENTED();
}

// Offset: 7592
// Size: 650
void CheckPiledriverBonus(tCar_spec* pCar, br_vector3* pImpact_point, br_vector3* pEnergy) {
    br_actor* child;
    br_vector3 norm_impact;
    br_vector3 norm_child;
    br_vector3 norm_energy;
    br_scalar dp;
    NOT_IMPLEMENTED();
}

// Offset: 8244
// Size: 152
tImpact_location CalcModifiedLocation(tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 8396
// Size: 222
void DoPratcamHit(br_vector3* pHit_vector) {
    int strength_modifier;
    br_scalar strength;
    NOT_IMPLEMENTED();
}

// Offset: 8620
// Size: 2217
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
    NOT_IMPLEMENTED();
}

// Offset: 10840
// Size: 210
tImpact_location GetDirection(br_vector3* pVelocity) {
    br_scalar mag_x;
    br_scalar mag_y;
    br_scalar mag_z;
    NOT_IMPLEMENTED();
}

// Offset: 11052
// Size: 92
void SetSmokeLastDamageLevel(tCar_spec* pCar) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 11144
// Size: 318
void SortOutSmoke(tCar_spec* pCar) {
    int i;
    int colour;
    int old_colour;
    int step;
    int pass;
    int repeat;
    NOT_IMPLEMENTED();
}

// Offset: 11464
// Size: 103
void StealCar(tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

// Offset: 11568
// Size: 5298
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
    NOT_IMPLEMENTED();
}

// Offset: 16868
// Size: 1028
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
    NOT_IMPLEMENTED();
}

// Offset: 17896
// Size: 69
void CrashEarnings(tCar_spec* pCar1, tCar_spec* pCar2) {
    NOT_IMPLEMENTED();
}
