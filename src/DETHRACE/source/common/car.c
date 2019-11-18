#include "car.h"

float gEngine_powerup_factor[6];
tCar_spec *gLast_car_to_skid[2];
tS3_sound_tag gSkid_tag[2];
tSave_camera gSave_camera[2];
tFace_ref gFace_list[150];
void (*ControlCar[6])(tCar_spec*, br_scalar);
tNon_car_spec *gActive_non_car_list[50];
float gCar_simplification_factor[2][5];
float gDefensive_powerup_factor[6];
float gOffensive_powerup_factor[6];
int gNet_player_to_view_index;
int gOver_shoot;
int gDouble_pling_water;
int gOpponent_viewing_mode;
br_scalar gMin_world_y;
int gMetal_crunch_sound_id[5];
int gFreeze_mechanics;
int gWoz_upside_down_at_all;
tU32 gQuite_wild_end;
tU32 gLast_mechanics_time;
tU32 gOn_me_wheels_start;
int gCar_car_collisions;
br_scalar gAccel;
br_vector3 gAverage_grid_position;
br_actor *gPed_actor;
tU32 gWild_start;
tU32 gLast_cunning_stunt;
int gCollision_count;
tU32 gQuite_wild_start;
int gCamera_frozen;
tU32 gSwitch_time;
int gMaterial_index;
int gEliminate_faces;
int gInTheSea;
int gMetal_scrape_sound_id[3];
br_vector3 gZero_v;
int gCamera_mode;
int gDoing_physics;
br_scalar gOur_yaw;
br_vector3 gCamera_pos_before_collide;
br_scalar gGravity;
br_vector3 gNew_ground_normal;
br_scalar gDt;
int gFace_num;
int gCollision_detection_on;
int gControl;
br_vector3 gGround_normal;
char gNon_car_spec_list[100];
int gCamera_has_collided;
int gCar_simplification_level;
int gNum_active_non_cars;
br_scalar gPanning_camera_height;
int gStop_opponents_moving;
tU32 gMechanics_time_sync;
int gNum_cars_and_non_cars;
int gFace_count;
br_angle gOld_yaw;
br_angle gOld_zoom;
br_angle gPanning_camera_angle;

// Offset: 0
// Size: 91
// EAX: pCar
// EDX: pUnit_type
// EBX: pDamage_amount
void DamageUnit(tCar_spec *pCar, int pUnit_type, int pDamage_amount) {
    tDamage_unit *the_damage;
}

// Offset: 92
// Size: 61
// EAX: pCar
// EDX: pUnit_type
// EBX: pDamage_amount
void DamageUnitWithSmoke(tCar_spec *pCar, int pUnit_type, int pDamage_amount) {
}

// Offset: 156
// Size: 52
// EAX: pDamage_amount
void DamageEngine(int pDamage_amount) {
}

// Offset: 208
// Size: 55
// EAX: pDamage_amount
void DamageTrans(int pDamage_amount) {
}

// Offset: 264
// Size: 55
// EAX: pDamage_amount
void DamageSteering(int pDamage_amount) {
}

// Offset: 320
// Size: 55
// EAX: pDamage_amount
void DamageLFWheel(int pDamage_amount) {
}

// Offset: 376
// Size: 55
// EAX: pDamage_amount
void DamageLFBrake(int pDamage_amount) {
}

// Offset: 432
// Size: 55
// EAX: pDamage_amount
void DamageLRBrake(int pDamage_amount) {
}

// Offset: 488
// Size: 55
// EAX: pDamage_amount
void DamageLRWheel(int pDamage_amount) {
}

// Offset: 544
// Size: 55
// EAX: pDamage_amount
void DamageRFWheel(int pDamage_amount) {
}

// Offset: 600
// Size: 55
// EAX: pDamage_amount
void DamageRFBrake(int pDamage_amount) {
}

// Offset: 656
// Size: 55
// EAX: pDamage_amount
void DamageRRBrake(int pDamage_amount) {
}

// Offset: 712
// Size: 55
// EAX: pDamage_amount
void DamageRRWheel(int pDamage_amount) {
}

// Offset: 768
// Size: 508
// EAX: pP1
// EDX: pP2
// EBX: pP3
// ECX: pNormal
void CalculatePlaneNormal(br_vector3 *pP1, br_vector3 *pP2, br_vector3 *pP3, br_vector3 *pNormal) {
    br_vector3 p0;
    br_vector3 p1;
    br_vector3 cross_product;
    br_vector3 temp_vector;
}

// Offset: 1276
// Size: 143
// EAX: pThe_model
// EDX: pFace_index
void CalculateGroundNormal(br_model *pThe_model, int pFace_index) {
}

// Offset: 1420
// Size: 399
void ChangeYCoordinate(br_scalar pNew_y, tU32 pTime_taken, br_model *pThe_model, int pFace_index) {
    br_scalar y_change;
    br_transform new_transform;
    br_vector3 side_window;
}

// Offset: 1820
// Size: 127
// EAX: pCar_spec
// EDX: pModel_index
void SwitchCarActor(tCar_spec *pCar_spec, int pModel_index) {
    int i;
}

// Offset: 1948
// Size: 1602
// EAX: pCar
// EDX: pClear_disabled_flag
void InitialiseCar2(tCar_spec *pCar, int pClear_disabled_flag) {
    int index;
    int j;
    int cat;
    int car_count;
    br_actor *car_actor;
    br_angle initial_yaw;
    br_scalar nearest_y_above;
    br_scalar nearest_y_below;
    br_scalar speed;
    int below_face_index;
    int above_face_index;
    br_model *below_model;
    br_model *above_model;
    br_vector3 grid_offset;
    br_matrix34 initial_yaw_matrix;
    br_matrix34 safe_position;
}

// Offset: 3552
// Size: 50
// EAX: pCar
void InitialiseCar(tCar_spec *pCar) {
}

// Offset: 3604
// Size: 270
// EAX: pThe_race
void InitialiseCarsEtc(tRace_info *pThe_race) {
    int i;
    int cat;
    int car_count;
    tCar_spec *car;
    br_bounds bnds;
}

// Offset: 3876
// Size: 231
// EAX: pThe_race
void GetAverageGridPosition(tRace_info *pThe_race) {
    int i;
    br_scalar total_cars;
    tCar_spec *car;
}

// Offset: 4108
// Size: 1067
// EAX: pThe_race
// EDX: pCar_index
// EBX: pGrid_index
void SetInitialPosition(tRace_info *pThe_race, int pCar_index, int pGrid_index) {
    int place_on_grid;
    int i;
    int start_i;
    int j;
    br_actor *car_actor;
    br_angle initial_yaw;
    br_scalar nearest_y_above;
    br_scalar nearest_y_below;
    br_scalar speed;
    int below_face_index;
    int above_face_index;
    br_model *below_model;
    br_model *above_model;
    tCar_spec *car;
    br_vector3 grid_offset;
    br_vector3 dist;
    br_vector3 real_pos;
    br_matrix34 initial_yaw_matrix;
    br_bounds bnds;
}

// Offset: 5176
// Size: 84
// EAX: pThe_race
void SetInitialPositions(tRace_info *pThe_race) {
    int i;
}

// Offset: 5260
// Size: 534
// EAX: non_car
void InitialiseNonCar(tNon_car_spec *non_car) {
    tCollision_info *c;
}

// Offset: 5796
// Size: 2073
// EAX: c
void GetFacesInBox(tCollision_info *c) {
    tBounds bnds;
    br_bounds new_in_old;
    br_bounds predicted_bounds;
    br_matrix34 mat;
    br_matrix34 mat2;
    br_matrix34 mat3;
    br_matrix34 mat4;
    br_matrix34 mat5;
    br_matrix34 mat6;
    br_scalar old_d;
    int i;
    br_bounds current_bounds;
}

// Offset: 7872
// Size: 45
int IsCarInTheSea() {
}

// Offset: 7920
// Size: 570
// EAX: car
// EDX: pTime
void RememberSafePosition(tCar_spec *car, tU32 pTime) {
    tU32 time_count;
    int j;
    br_vector3 r;
    br_scalar ts;
}

// Offset: 8492
// Size: 1055
// EAX: pTime_difference
void ControlOurCar(tU32 pTime_difference) {
    br_scalar ts;
    br_vector3 minus_k;
    tCar_spec *car;
    int steering_locked;
    int i;
    tU32 time;
}

// Offset: 9548
// Size: 1603
// EAX: c
void CalcEngineForce(tCar_spec *c, br_scalar dt) {
    br_scalar torque;
    br_scalar ts;
    br_scalar ts2;
    br_scalar brake_temp;
    int sign;
    tS32 temp_for_swap;
}

// Offset: 11152
// Size: 515
// EAX: pFrame_start_time
void PrepareCars(tU32 pFrame_start_time) {
    tCar_spec *car;
    int i;
    tU32 last_frame_start;
}

// Offset: 11668
// Size: 1539
// EAX: pLast_frame_time
// EDX: pTime
void FinishCars(tU32 pLast_frame_time, tU32 pTime) {
    tCar_spec *car;
    br_vector3 minus_k;
    int i;
    int wheel;
}

// Offset: 13208
// Size: 337
// EAX: pLast_frame_time
// EDX: pTime
void InterpolateCars(tU32 pLast_frame_time, tU32 pTime) {
    br_scalar dt;
    tCar_spec *car;
    int i;
}

// Offset: 13548
// Size: 218
void ResetOldmat() {
    tCar_spec *car;
    int i;
    br_matrix34 mat;
    int normalise_count;
}

// Offset: 13768
// Size: 118
void GetNonCars() {
    int i;
    int j;
}

// Offset: 13888
// Size: 1003
// EAX: pCar
void GetNetPos(tCar_spec *pCar) {
    int j;
    float amount;
    br_scalar total_deflection;
}

// Offset: 14892
// Size: 1298
// EAX: last_frame_time
// EDX: pTime_difference
void ApplyPhysicsToCars(tU32 last_frame_time, tU32 pTime_difference) {
    br_vector3 minus_k;
    int i;
    int old_num_cars;
    int step_number;
    int dam_index;
    int steering_locked;
    tCar_spec *car;
    tCollision_info *car_info;
    tNon_car_spec *non_car;
    tU32 time_step;
    tU32 frame_end_time;
}

// Offset: 16192
// Size: 436
// EAX: pCar
void MungeSpecialVolume(tCollision_info *pCar) {
    tSpecial_volume *new_special_volume;
    tCar_spec *car;
}

// Offset: 16628
// Size: 199
// EAX: pCar
void ResetCarSpecialVolume(tCollision_info *pCar) {
    br_vector3 cast_v;
    br_vector3 norm;
    br_scalar t;
    int id_len;
    char *mat_id;
    tSpecial_volume *new_special_volume;
    br_material *material;
}

// Offset: 16828
// Size: 1052
// EAX: pCar
void TestAutoSpecialVolume(tCollision_info *pCar) {
    br_vector3 pos;
    br_scalar car_d;
    br_scalar d;
    br_scalar d2;
    br_vector3 dir;
    br_vector3 tv;
    br_vector3 lp;
    br_vector3 hp;
    tSpecial_volume *vol;
    br_matrix34 *mat;
    br_scalar lowest_p;
    br_scalar highest_p;
    br_scalar val;
    int i;
}

// Offset: 17880
// Size: 421
// EAX: car
void MoveAndCollideCar(tCar_spec *car, br_scalar dt) {
    tCollision_info *car_info;
    int wheel;
}

// Offset: 18304
// Size: 271
// EAX: non_car
void MoveAndCollideNonCar(tNon_car_spec *non_car, br_scalar dt) {
    tCollision_info *car_info;
}

// Offset: 18576
// Size: 336
// EAX: car
int CollideCarWithWall(tCollision_info *car, br_scalar dt) {
}

// Offset: 18912
// Size: 268
void ToggleControls() {
}

// Offset: 19180
// Size: 675
// EAX: c
void ControlCar2(tCar_spec *c, br_scalar dt) {
}

// Offset: 19856
// Size: 686
// EAX: c
void ControlCar3(tCar_spec *c, br_scalar dt) {
}

// Offset: 20544
// Size: 1374
// EAX: c
void ControlCar4(tCar_spec *c, br_scalar dt) {
    br_scalar ts;
}

// Offset: 21920
// Size: 1097
// EAX: c
void ControlCar5(tCar_spec *c, br_scalar dt) {
}

// Offset: 23020
// Size: 530
// EAX: c
void ControlCar1(tCar_spec *c, br_scalar dt) {
}

// Offset: 23552
// Size: 138
// EAX: wdt
// EDX: m
void setrotate(br_vector3 *wdt, br_matrix34 *m) {
    br_euler e;
}

// Offset: 23692
// Size: 454
// EAX: c
void RotateCar2(tCollision_info *c, br_scalar dt) {
    br_vector3 wdt;
    br_vector3 wdt2;
    br_vector3 L;
    br_vector3 L2;
    br_matrix34 m;
}

// Offset: 24148
// Size: 673
// EAX: c
void RotateCarSecondOrder(tCollision_info *c, br_scalar dt) {
    br_vector3 L;
    br_vector3 L2;
    br_vector3 axis;
    br_vector3 omega;
    br_scalar rad;
    br_scalar rad_rate;
    br_matrix34 m;
}

// Offset: 24824
// Size: 486
// EAX: c
void RotateCarFirstOrder(tCollision_info *c, br_scalar dt) {
    br_vector3 axis;
    br_vector3 L;
    br_vector3 L2;
    br_matrix34 m;
    br_scalar rad_rate;
    br_scalar rad;
    br_scalar e1;
    br_scalar e2;
    br_scalar max_rad;
}

// Offset: 25312
// Size: 218
// EAX: c
void SimpleRotate(tCollision_info *c, br_scalar dt) {
    br_vector3 axis;
    br_scalar rad_rate;
    br_scalar rad;
}

// Offset: 25532
// Size: 305
// EAX: c
void RotateCar(tCollision_info *c, br_scalar dt) {
    br_scalar rad_squared;
    int steps;
    int i;
}

// Offset: 25840
// Size: 434
// EAX: c
void SteeringSelfCentre(tCar_spec *c, br_scalar dt, br_vector3 *n) {
    br_scalar ts;
    br_scalar ts2;
}

// Offset: 26276
// Size: 923
// EAX: nc
void NonCarCalcForce(tNon_car_spec *nc, br_scalar dt) {
    tCollision_info *c;
    tSpecial_volume *vol;
    br_scalar ts;
    br_vector3 tv;
    br_vector3 v;
}

// Offset: 27200
// Size: 402
// EAX: c
void AddDrag(tCar_spec *c, br_scalar dt) {
    br_scalar drag_multiplier;
    br_scalar ts;
    tSpecial_volume *vol;
    br_vector3 b;
}

// Offset: 27604
// Size: 419
// EAX: c
// EDX: wheel_pos
// EBX: norm
// ECX: d
void DoBumpiness(tCar_spec *c, br_vector3 *wheel_pos, br_vector3 *norm, br_scalar *d, int n) {
    br_vector3 tv;
    int delta;
    int x;
    int y;
    tMaterial_modifiers *mat_list;
}

// Offset: 28024
// Size: 12038
// EAX: c
void CalcForce(tCar_spec *c, br_scalar dt) {
    int n;
    int normnum;
    int i;
    int x;
    int y;
    br_scalar force[4];
    br_scalar d[4];
    br_scalar ts;
    br_scalar ts2;
    br_scalar ts3;
    br_scalar ts4;
    br_scalar ts5;
    br_scalar ts6;
    br_scalar deltaomega;
    br_scalar wheelratio;
    br_scalar modf;
    br_scalar maxfl;
    br_scalar maxfr;
    br_scalar max_retardation;
    br_scalar front_retardation;
    br_scalar friction_number;
    br_vector3 a;
    br_vector3 b;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 norm[4];
    br_vector3 v_batwick;
    br_vector3 vplane;
    br_vector3 rightplane;
    br_vector3 t;
    br_vector3 f;
    br_vector3 ff;
    br_vector3 fb;
    br_scalar ffi;
    br_scalar ffk;
    br_scalar fbi;
    br_scalar fbk;
    br_vector3 wheel_pos[4];
    br_scalar direction;
    br_scalar wheel_spin_force;
    br_scalar d_damage;
    br_scalar fl_oil_factor;
    br_scalar fr_oil_factor;
    br_scalar rl_oil_factor;
    br_scalar rr_oil_factor;
    br_matrix34 *mat;
    tMaterial_modifiers *mat_list;
    br_scalar stop_timer;
    br_scalar slide_dist;
    tDamage_type dam;
    br_scalar v;
    tSpecial_volume *vol;
}

// Offset: 40064
// Size: 1156
// EAX: c
void DoRevs(tCar_spec *c, br_scalar dt) {
    br_scalar wheel_spin_force;
    br_scalar ts;
    int revs_increase;
}

// Offset: 41220
// Size: 127
// EAX: c
// EDX: tdt
void ApplyTorque(tCar_spec *c, br_vector3 *tdt) {
}

// Offset: 41348
// Size: 120
// EAX: c
void TranslateCar(tCollision_info *c, br_scalar dt) {
    br_vector3 t;
}

// Offset: 41468
// Size: 9029
// EAX: c
int CollCheck(tCollision_info *c, br_scalar dt) {
    br_vector3 a;
    br_vector3 a1;
    br_vector3 aa;
    br_vector3 bb;
    br_vector3 cc;
    br_vector3 pos;
    br_vector3 r[8];
    br_vector3 norm;
    br_vector3 n[8];
    br_vector3 dir;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 tau[4];
    br_vector3 old_val;
    br_vector3 ftau;
    br_vector3 max_friction;
    br_vector3 vel;
    br_vector3 p_vel;
    br_vector3 normal_force;
    br_vector3 friction_force;
    br_scalar d[4];
    br_scalar f[4];
    br_scalar total_force;
    br_scalar point_vel;
    br_scalar batwick_length;
    br_matrix4 M;
    br_scalar dist;
    br_scalar min_d;
    br_scalar ts;
    br_scalar ts2;
    br_scalar v_diff;
    br_matrix34 *mat;
    br_matrix34 *oldmat;
    br_matrix34 mat_to_oldmat;
    br_matrix34 oldmat_to_mat;
    br_matrix34 tm;
    int collision;
    br_bounds bnds;
    tFace_ref *f_ref;
    int i;
    int j;
    int l;
    int k;
    int material;
    int noise_defeat;
    int oldk;
    br_scalar min;
    br_scalar max;
    br_vector3 edges[3];
    br_vector3 corner;
    br_vector3 test_dir;
    br_scalar min_acc;
    br_scalar max_acc;
    br_matrix34 message_mat;
}

// Offset: 50500
// Size: 916
// EAX: c
// EDX: vel
// EBX: normal_force
// ECX: pos
br_scalar AddFriction(tCollision_info *c, br_vector3 *vel, br_vector3 *normal_force, br_vector3 *pos, br_scalar total_force, br_vector3 *max_friction) {
    br_vector3 norm;
    br_vector3 tv;
    br_vector3 ftau;
    br_scalar ts;
    br_scalar point_vel;
}

// Offset: 51416
// Size: 1179
// EAX: car1
// EDX: car2
// EBX: vel1
// ECX: vel2
void AddFrictionCarToCar(tCollision_info *car1, tCollision_info *car2, br_vector3 *vel1, br_vector3 *vel2, br_vector3 *normal_force1, br_vector3 *pos1, br_vector3 *pos2, br_scalar total_force, br_vector3 *max_friction) {
    br_vector3 v_diff1;
    br_vector3 v_diff2;
    br_vector3 tau1;
    br_vector3 tau2;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 vel2_in_frame_1;
    br_scalar ts;
    br_scalar ts2;
    br_scalar v_diff;
    br_scalar stopping_impulse;
    br_scalar total_friction;
    int i;
}

// Offset: 52596
// Size: 233
void ScrapeNoise(br_scalar vel, br_vector3 *position, int material) {
    tS3_volume vol;
    tS3_sound_tag scrape_tag;
    tS3_volume last_scrape_vol;
    br_vector3 velocity;
    br_vector3 position_in_br;
}

// Offset: 52832
// Size: 747
// EAX: pC
// EDX: pWheel_num
void SkidNoise(tCar_spec *pC, int pWheel_num, br_scalar pV, int material) {
    br_vector3 pos;
    br_vector3 world_pos;
    br_vector3 wv;
    br_vector3 wvw;
    br_scalar ts;
    tS3_volume last_skid_vol[2];
    int i;
}

// Offset: 53580
// Size: 77
// EAX: pC
void StopSkid(tCar_spec *pC) {
}

// Offset: 53660
// Size: 255
// EAX: pForce
// EDX: position
// EBX: material
void CrashNoise(br_vector3 *pForce, br_vector3 *position, int material) {
    tS3_sound_tag crunch_tag;
    tS3_volume last_crunch_vol;
    tS3_volume vol;
    br_vector3 velocity;
}

// Offset: 53916
// Size: 1541
// EAX: c
// EDX: pPosition
// EBX: pForce_car_space
// ECX: car2
void CrushAndDamageCar(tCar_spec *c, br_vector3 *pPosition, br_vector3 *pForce_car_space, tCar_spec *car2) {
    br_vector3 force;
    br_vector3 force2;
    br_vector3 position;
    br_vector3 pos_w;
    br_vector3 car_to_cam;
    br_vector3 force_for_bodywork;
    br_scalar ts;
    int i;
    br_matrix34 m;
    br_scalar fudge_multiplier;
}

// Offset: 55460
// Size: 472
// EAX: c
int ExpandBoundingBox(tCar_spec *c) {
    br_scalar min_z;
    br_scalar max_z;
    br_scalar dist;
    br_vector3 tv;
    br_vector3 old_pos;
    int l;
    br_matrix34 mat;
}

// Offset: 55932
// Size: 795
// EAX: c
// EDX: crush_only
void CrushBoundingBox(tCar_spec *c, int crush_only) {
    br_vector3 min;
    br_vector3 max;
    int i;
    br_actor *actor;
}

// Offset: 56728
// Size: 279
void AddCollPoint(br_scalar dist, br_vector3 *p, br_vector3 *norm, br_vector3 *r, br_vector3 *n, br_vector3 *dir, int num, tCollision_info *c) {
    br_scalar d[4];
    int i;
    int furthest;
}

// Offset: 57008
// Size: 88
// EAX: f
// EDX: m
// EBX: d
br_scalar SinglePointColl(br_scalar *f, br_matrix4 *m, br_scalar *d) {
}

// Offset: 57096
// Size: 359
// EAX: f
// EDX: m
// EBX: d
// ECX: tau
br_scalar TwoPointColl(br_scalar *f, br_matrix4 *m, br_scalar *d, br_vector3 *tau, br_vector3 *n) {
    br_scalar ts;
}

// Offset: 57456
// Size: 62
// EAX: mi
// EDX: mc
br_scalar DrMatrix4Inverse(br_matrix4 *mi, br_matrix4 *mc) {
}

// Offset: 57520
// Size: 172
// EAX: f
// EDX: m
// EBX: d
br_scalar ThreePointColl(br_scalar *f, br_matrix4 *m, br_scalar *d) {
    br_matrix4 mc;
    br_matrix4 mi;
    br_scalar ts;
}

// Offset: 57692
// Size: 638
// EAX: f
// EDX: m
// EBX: d
// ECX: tau
br_scalar ThreePointCollRec(br_scalar *f, br_matrix4 *m, br_scalar *d, br_vector3 *tau, br_vector3 *n, tCollision_info *c) {
    int i;
    int j;
    br_scalar ts;
}

// Offset: 58332
// Size: 639
// EAX: f
// EDX: m
// EBX: d
// ECX: tau
br_scalar FourPointColl(br_scalar *f, br_matrix4 *m, br_scalar *d, br_vector3 *tau, br_vector3 *n, tCollision_info *c) {
    int i;
    int j;
    int l;
    br_scalar ts;
}

// Offset: 58972
// Size: 228
// EAX: pNum_rays
// EDX: a
// EBX: b
// ECX: nor
void MultiFindFloorInBoxM(int pNum_rays, br_vector3 *a, br_vector3 *b, br_vector3 *nor, br_scalar *d, tCar_spec *c, int *mat_ref) {
    br_vector3 aa[4];
    br_vector3 bb;
    int i;
}

// Offset: 59200
// Size: 338
// EAX: pNum_rays
// EDX: a
// EBX: b
// ECX: nor
void MultiFindFloorInBoxBU(int pNum_rays, br_vector3 *a, br_vector3 *b, br_vector3 *nor, br_scalar *d, tCar_spec *c, int *mat_ref) {
    br_vector3 nor2;
    int i;
    int j;
    int l;
    br_scalar dist[4];
    tFace_ref *face_ref;
}

// Offset: 59540
// Size: 149
// EAX: a
// EDX: b
// EBX: nor
// ECX: d
void findfloor(br_vector3 *a, br_vector3 *b, br_vector3 *nor, br_scalar *d) {
    br_material *material;
    br_vector3 aa;
    br_vector3 bb;
}

// Offset: 59692
// Size: 157
// EAX: a
// EDX: b
// EBX: nor
// ECX: d
int FindFloorInBoxM(br_vector3 *a, br_vector3 *b, br_vector3 *nor, br_scalar *d, tCollision_info *c) {
    br_vector3 aa;
    br_vector3 bb;
}

// Offset: 59852
// Size: 278
// EAX: a
// EDX: b
// EBX: nor
// ECX: d
int FindFloorInBoxBU(br_vector3 *a, br_vector3 *b, br_vector3 *nor, br_scalar *d, tCollision_info *c) {
    br_vector3 nor2;
    int i;
    int j;
    br_scalar dist;
    tFace_ref *face_ref;
}

// Offset: 60132
// Size: 443
// EAX: a
// EDX: b
// EBX: nor
// ECX: d
int FindFloorInBoxBU2(br_vector3 *a, br_vector3 *b, br_vector3 *nor, br_scalar *d, tCollision_info *c) {
    br_vector3 nor2;
    br_vector3 tv;
    int i;
    int j;
    br_scalar dist;
    tFace_ref *face_ref;
}

// Offset: 60576
// Size: 157
// EAX: a
// EDX: b
// EBX: nor
// ECX: d
int FindFloorInBoxM2(br_vector3 *a, br_vector3 *b, br_vector3 *nor, br_scalar *d, tCollision_info *c) {
    br_vector3 aa;
    br_vector3 bb;
}

// Offset: 60736
// Size: 1324
// EAX: pB
// EDX: pM
// EBX: pMold
// ECX: pPoint_list
int BoxFaceIntersect(br_bounds *pB, br_matrix34 *pM, br_matrix34 *pMold, br_vector3 *pPoint_list, br_vector3 *pNorm_list, br_scalar *pDist_list, int pMax_pnts, tCollision_info *c) {
    br_vector3 p[3];
    br_vector3 tv;
    br_vector3 pos;
    br_bounds bnds;
    int i;
    int j;
    int n;
    int flag;
    int m;
    tFace_ref *f_ref;
    br_face *face;
}

// Offset: 62060
// Size: 3591
// EAX: p1
// EDX: p2
// EBX: pB
// ECX: pMold
int AddEdgeCollPoints(br_vector3 *p1, br_vector3 *p2, br_bounds *pB, br_matrix34 *pMold, br_vector3 *pPoint_list, br_vector3 *pNorm_list, int n, int pMax_pnts, tCollision_info *c) {
    br_vector3 op1;
    br_vector3 op2;
    br_vector3 a;
    br_vector3 b;
    br_vector3 edge;
    br_vector3 hp1;
    br_vector3 hp2;
    br_vector3 hp3;
    int plane1;
    int plane2;
    int plane3;
    int d;
}

// Offset: 65652
// Size: 118
// EAX: n
// EDX: p
void GetPlaneNormal(br_vector3 *n, int p) {
    int d;
}

// Offset: 65772
// Size: 534
// EAX: pos
// EDX: edge
// EBX: pB
// ECX: plane1
int GetBoundsEdge(br_vector3 *pos, br_vector3 *edge, br_bounds *pB, int plane1, int plane2, br_vector3 *a, br_vector3 *b, br_vector3 *c, int flag) {
    int d1;
    int d2;
    int d3;
    br_vector3 n;
    br_vector3 p;
    br_vector3 q;
}

// Offset: 66308
// Size: 603
// EAX: pTime_difference
void oldMoveOurCar(tU32 pTime_difference) {
    br_vector3 thrust_vector;
    br_matrix34 direction_matrix;
    br_matrix34 old_mat;
    double rotate_amount;
    br_scalar nearest_y_above;
    br_scalar nearest_y_below;
    br_scalar speed;
    int below_face_index;
    int above_face_index;
    br_model *below_model;
    br_model *above_model;
}

// Offset: 66912
// Size: 34
void ToggleCollisionDetection() {
}

// Offset: 66948
// Size: 84
void CancelPendingCunningStunt() {
}

// Offset: 67032
// Size: 61
float frac(float pN) {
}

// Offset: 67096
// Size: 601
// EAX: pCar
void SetAmbientPratCam(tCar_spec *pCar) {
    br_scalar vcs_x;
    br_scalar vcs_y;
    br_scalar vcs_z;
    br_scalar abs_vcs_x;
    br_scalar abs_vcs_y;
    br_scalar abs_vcs_z;
    br_scalar abs_omega_x;
    br_scalar abs_omega_y;
    br_scalar abs_omega_z;
    tU32 the_time;
    tU32 last_time_on_ground;
}

// Offset: 67700
// Size: 3599
// EAX: pFrame_period
void MungeCarGraphics(tU32 pFrame_period) {
    int i;
    int j;
    int update_mat;
    int spinning_wildly;
    int spinning_mildly;
    int car_count;
    int oily_count;
    int car;
    int cat;
    int new_special_screen;
    tCar_spec *the_car;
    br_scalar distance_from_camera;
    br_scalar car_x;
    br_scalar car_z;
    br_scalar oily_size;
    br_scalar car_radius;
    br_scalar abs_omega_x;
    br_scalar abs_omega_y;
    br_scalar abs_omega_z;
    float wheel_speed;
    float speed_mph;
    float rev_angle;
    float sine_angle;
    float raw_revs;
    float rev_reducer;
    tSpecial_screen *the_special_screen;
    br_material *the_material;
    tU32 the_time;
    br_actor *oily_actor;
}

// Offset: 71300
// Size: 179
void ResetCarScreens() {
    int car_count;
    int i;
    tCar_spec *the_car;
}

// Offset: 71480
// Size: 208
tCar_spec* GetRaceLeader() {
    int i;
    int score;
    tCar_spec *car;
}

// Offset: 71688
// Size: 555
void AmIGettingBoredWatchingCameraSpin() {
    tU32 time_of_death;
    tU32 headup_timer;
    tCar_spec *car;
    char s[256];
}

// Offset: 72244
// Size: 161
void ViewNetPlayer() {
}

// Offset: 72408
// Size: 251
void ViewOpponent() {
}

// Offset: 72660
// Size: 134
void ToggleCarToCarCollisions() {
}

// Offset: 72796
// Size: 34
void SwapCar() {
}

// Offset: 72832
// Size: 178
void AdjustDownForce() {
    tCar_spec *c;
}

// Offset: 73012
// Size: 134
void FreezeMechanics() {
}

// Offset: 73148
// Size: 134
void PutOpponentsInNeutral() {
}

// Offset: 73284
// Size: 133
void SetPanningFieldOfView() {
    br_camera *camera_ptr;
}

// Offset: 73420
// Size: 232
// EAX: pCar
void CheckDisablePlingMaterials(tCar_spec *pCar) {
    br_matrix34 *mat;
    br_scalar height;
    int i;
}

// Offset: 73652
// Size: 346
// EAX: c
// EDX: pTime
void PositionExternalCamera(tCar_spec *c, tU32 pTime) {
    int old_camera_mode;
    br_camera *camera_ptr;
}

// Offset: 74000
// Size: 102
// EAX: c
// EDX: pTime
void CameraBugFix(tCar_spec *c, tU32 pTime) {
    br_matrix34 mat;
    br_matrix34 *m2;
    br_vector3 tv;
}

// Offset: 74104
// Size: 141
// EAX: pActor
int PossibleRemoveNonCarFromWorld(br_actor *pActor) {
    tU8 cx;
    tU8 cz;
    tTrack_spec *track_spec;
}

// Offset: 74248
// Size: 117
// EAX: pActor
void PutNonCarBackInWorld(br_actor *pActor) {
    tU8 cx;
    tU8 cz;
    tTrack_spec *track_spec;
}

// Offset: 74368
// Size: 2633
// EAX: c
// EDX: pTime
int IncidentCam(tCar_spec *c, tU32 pTime) {
    br_matrix34 *m2;
    br_matrix34 mat;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 perp;
    br_vector3 vertical;
    br_vector3 murderer_pos;
    br_scalar ts;
    tCar_spec *car2;
    tU32 next_incident_time;
    tIncident_type type;
    float severity;
    tIncident_info info;
    int random;
    int count;
    br_scalar temp;
    br_vector3 old_cam_pos;
    int removed;
}

// Offset: 77004
// Size: 1267
// EAX: c
// EDX: type
// EBX: severity
// ECX: info
int MoveCamToIncident(tCar_spec *c, tIncident_type *type, float *severity, tIncident_info *info, tU32 *next_incident_time) {
    tU32 next_incident_time2;
    tU32 t;
    tIncident_type type2;
    float severity2;
    tIncident_info info2;
    br_vector3 pos;
    br_vector3 left;
    br_vector3 right;
    br_vector3 vertical;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 perp;
    int test;
}

// Offset: 78272
// Size: 368
// EAX: c
// EDX: pTime
void PanningExternalCamera(tCar_spec *c, tU32 pTime) {
    br_matrix34 *m2;
    br_matrix34 *m1;
    br_vector3 tv;
    br_scalar ts;
    int inside_camera_zone;
}

// Offset: 78640
// Size: 136
// EAX: start
// EDX: end
int CheckForWall(br_vector3 *start, br_vector3 *end) {
    br_vector3 dir;
    br_material *material;
    br_vector3 normal;
    br_scalar d;
}

// Offset: 78776
// Size: 1383
// EAX: c
void SetUpPanningCamera(tCar_spec *c) {
    br_vector3 pos;
    br_vector3 perp;
    br_vector3 dir;
    br_vector3 tv;
    br_vector3 tv2;
    br_scalar ts;
    tU32 time;
    tU32 t;
    tU32 t2;
    tU32 time_step;
    br_matrix34 *m2;
    br_matrix34 *m1;
    br_vector3 left;
    br_vector3 right;
    br_vector3 car_centre;
    int left_score;
    int right_score;
}

// Offset: 80160
// Size: 97
// EAX: i
void SaveCameraPosition(int i) {
}

// Offset: 80260
// Size: 97
// EAX: i
void RestoreCameraPosition(int i) {
}

// Offset: 80360
// Size: 2107
// EAX: c
// EDX: pTime
void NormalPositionExternalCamera(tCar_spec *c, tU32 pTime) {
    br_matrix34 *m2;
    br_matrix34 *m1;
    br_scalar time;
    br_scalar ts;
    br_scalar ts2;
    br_scalar dist;
    br_scalar height_inc;
    br_scalar l;
    br_scalar frac;
    br_vector3 vn;
    br_vector3 a;
    br_vector3 b;
    br_vector3 tv;
    br_angle yaw;
    br_angle theta;
    br_scalar d;
    int face;
    int i;
    int swoop;
    int manual_swing;
    int manual_zoom;
    br_vector3 old_camera_pos;
}

// Offset: 82468
// Size: 446
// EAX: c
// EDX: vn
// EBX: manual_swing
void MoveWithWheels(tCar_spec *c, br_vector3 *vn, int manual_swing) {
    br_angle yaw;
    br_angle theta;
    int move_with_wheels;
}

// Offset: 82916
// Size: 1828
// EAX: c
// EDX: m1
// EBX: m2
// ECX: vn
void SwingCamera(tCar_spec *c, br_matrix34 *m1, br_matrix34 *m2, br_vector3 *vn, tU32 pTime) {
    int i;
    br_scalar ts;
    br_angle yaw;
    br_angle theta;
    br_angle alpha;
    br_scalar sin_dtheta;
    br_scalar cos_dtheta;
    br_scalar sign;
    int manual_swing;
    br_angle omega;
    int elapsed_time;
    br_vector3 old_vn;
}

// Offset: 84744
// Size: 776
// EAX: c
// EDX: m1
// EBX: m2
void PointCameraAtCar(tCar_spec *c, br_matrix34 *m1, br_matrix34 *m2) {
    br_vector3 vn;
    br_vector3 tv;
    br_vector3 tv2;
    br_scalar dist;
    br_scalar frac;
    br_angle theta;
    br_vector3 *pos;
    br_camera *camera_ptr;
    br_angle off_centre_angle;
    int swoop;
}

// Offset: 85520
// Size: 465
// EAX: pos
// EDX: m2
void PointCamera(br_vector3 *pos, br_matrix34 *m2) {
    br_vector3 vn;
    br_scalar dist;
    br_angle theta;
    br_camera *camera_ptr;
}

// Offset: 85988
// Size: 2411
// EAX: car_pos
// EDX: cam_pos
// EBX: old_camera_pos
// ECX: manual_move
int CollideCamera2(br_vector3 *car_pos, br_vector3 *cam_pos, br_vector3 *old_camera_pos, int manual_move) {
    int i;
    int k;
    br_vector3 a;
    br_vector3 b;
    br_vector3 vn;
    br_vector3 tv;
    br_vector3 tv2;
    br_scalar l;
    br_scalar d;
    br_scalar ts;
    br_scalar ts2;
    br_scalar dist;
    br_scalar hither;
    br_angle theta;
    tBounds bnds;
    br_matrix34 mat;
    br_material *material;
    br_scalar alpha;
    br_scalar sa;
    br_scalar sb;
    br_scalar sc;
    tFace_ref face_list[3];
}

// Offset: 88400
// Size: 141
// EAX: bnds
// EDX: p
int BoundsTest(br_bounds *bnds, br_vector3 *p) {
    int j;
}

// Offset: 88544
// Size: 494
// EAX: car_pos
// EDX: cam_pos
int CollideCameraWithOtherCars(br_vector3 *car_pos, br_vector3 *cam_pos) {
    int i;
    int plane;
    br_scalar ts;
    tCar_spec *c;
    br_vector3 tv;
    br_vector3 pos_car_space;
    br_vector3 dir;
    br_vector3 p;
    br_vector3 n;
    br_bounds bnds;
}

// Offset: 89040
// Size: 511
void InitialiseExternalCamera() {
    tCar_spec *c;
    br_vector3 r;
    br_angle yaw;
}

// Offset: 89552
// Size: 44
void FreezeCamera() {
}

// Offset: 89596
// Size: 2248
// EAX: c
void FlyCar(tCar_spec *c, br_scalar dt) {
    int accflag;
    int turnflag;
    br_vector3 step;
    br_matrix34 *mat;
    br_angle theta;
    br_scalar vel;
    tFace_ref faces[20];
    tBounds bnds;
}

// Offset: 91844
// Size: 157
// EAX: v
// EDX: t
void DrVector3RotateY(br_vector3 *v, br_angle t) {
    br_scalar c;
    br_scalar s;
    br_scalar ts;
}

// Offset: 92004
// Size: 237
void CrashCarsTogether(br_scalar dt) {
    int pass;
    int k;
    int i;
    tCollison_data collide_list[32];
}

// Offset: 92244
// Size: 2384
int CrashCarsTogetherSinglePass(br_scalar dt, int pPass, tCollison_data *collide_list) {
    int i;
    int j;
    int l;
    int m;
    int n;
    int collided;
    int k;
    int ref1;
    int ref2;
    int c1im;
    int c2im;
    tCollision_info *car_1;
    tCollision_info *car_2;
    tCollision_info *car_3;
    tCollision_info *car_in_middle;
    tCollision_info *car_on_wall;
}

// Offset: 94628
// Size: 142
// EAX: car
void BringCarToAGrindingHalt(tCollision_info *car) {
}

// Offset: 94940
// Size: 99
// EAX: car1
// EDX: car2
int SimpleCarCarCollisionTest(tCollision_info *car1, tCollision_info *car2) {
}

// Offset: 95040
// Size: 877
// EAX: car1
// EDX: car2
int CollideTwoCarsWithWalls(tCollision_info *car1, tCollision_info *car2, br_scalar dt) {
    br_vector3 mom1;
    br_vector3 mom2;
    int l;
    int m;
    int n;
    int p;
    int im1;
    int im2;
}

// Offset: 95920
// Size: 298
// EAX: car1
// EDX: car2
int CollideTwoCarsRepeatedly(tCollision_info *car1, tCollision_info *car2, br_scalar dt) {
    int l;
    int collide;
    br_scalar ts;
}

// Offset: 96220
// Size: 3499
// EAX: car1
// EDX: car2
// EBX: pPass
int CollideTwoCars(tCollision_info *car1, tCollision_info *car2, int pPass) {
    int k;
    int old_k;
    int i;
    int j;
    br_scalar dist;
    br_scalar ts;
    br_bounds new_car1_bnds;
    br_bounds new_car2_bnds;
    br_bounds bnds;
    br_matrix34 *mat1;
    br_matrix34 *mat2;
    br_matrix34 *oldmat1;
    br_matrix34 *oldmat2;
    br_matrix34 inv_mat1;
    br_matrix34 inv_mat2;
    br_matrix34 inv_oldmat1;
    br_matrix34 inv_oldmat2;
    br_matrix34 car2_to_car1;
    br_matrix34 car1_to_car2;
    br_matrix34 old_car2_to_car1;
    br_matrix34 old_car1_to_car2;
    br_matrix34 car1_to_old_car1;
    br_matrix34 car2_to_old_car2;
    br_vector3 r[16];
    br_vector3 n[16];
    br_vector3 sep;
    br_vector3 tv;
    int add_point;
    br_vector3 oldr1;
    br_vector3 oldr2;
    br_vector3 oldn1;
    br_vector3 oldn2;
    int is_old_point_available;
}

// Offset: 99720
// Size: 3598
// EAX: pB1
// EDX: pB2
// EBX: pM21
// ECX: pM12
int GetEdgeEdgeCollisions(br_bounds *pB1, br_bounds *pB2, br_matrix34 *pM21, br_matrix34 *pM12, br_matrix34 *pMo21, br_matrix34 *pMo12, br_matrix34 *pM1o1, br_vector3 *pPoint_list, br_vector3 *pNorm_list, int pMax) {
    br_vector3 p1;
    br_vector3 p2;
    br_vector3 tp1;
    br_vector3 tp2;
    br_vector3 tp3;
    br_vector3 hp1;
    br_vector3 hp2;
    br_vector3 hp3;
    br_vector3 shp1;
    br_vector3 shp2;
    br_vector3 edge;
    int plane1;
    int plane2;
    int plane3;
    br_scalar ts;
    int i;
    int j;
    int n;
}

// Offset: 103320
// Size: 1920
// EAX: car1
// EDX: car2
// EBX: pMms
// ECX: pMoms
int FacePointCarCarCollide(tCollision_info *car1, tCollision_info *car2, br_matrix34 *pMms, br_matrix34 *pMoms, br_matrix34 *pMsos, br_vector3 *pPoint_list, br_vector3 *pNorm_list, int pMax, int order) {
    int k;
    int i;
    int j;
    int l;
    int plane;
    br_vector3 a;
    br_vector3 a1;
    br_vector3 aa;
    br_vector3 bb;
    br_vector3 norm;
    br_vector3 hp;
    br_vector3 centre;
    br_scalar dist;
    br_bounds *pStat_box;
    br_bounds *pMove_box;
}

// Offset: 105240
// Size: 111
// EAX: pCar
void MungeCarsMass(tCollision_info *pCar, br_scalar pFactor) {
}

// Offset: 105352
// Size: 124
// EAX: pCar_1
// EDX: pCar_2
void ModifyCarsMass(tCollision_info *pCar_1, tCollision_info *pCar_2) {
}

// Offset: 105476
// Size: 144
// EAX: pCar_1
// EDX: pCar_2
void ResetCarsMass(tCollision_info *pCar_1, tCollision_info *pCar_2) {
}

// Offset: 105620
// Size: 8941
// EAX: car1
// EDX: car2
// EBX: r
// ECX: n
int DoCollide(tCollision_info *car1, tCollision_info *car2, br_vector3 *r, br_vector3 *n, int k, int pPass, br_matrix34 *mat1_to_mat2) {
    br_matrix34 *mat1;
    br_matrix34 *mat2;
    br_matrix34 *oldmat1;
    br_matrix34 *oldmat2;
    br_matrix4 M;
    br_vector3 tau1[8];
    br_vector3 a;
    br_vector3 norm;
    br_vector3 f1;
    br_vector3 f2;
    br_vector3 pos1;
    br_vector3 pos2;
    br_vector3 max_friction;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 *tau2;
    br_vector3 torque1;
    br_vector3 torque2;
    br_scalar f[4];
    br_scalar d[4];
    br_scalar ts;
    br_scalar tforce;
    int i;
    int j;
    int car1_point;
    int car2_point;
    int plane;
    int move_car1;
    int move_car2;
    br_vector3 a2;
    br_vector3 f12;
    br_vector3 f22;
    br_vector3 point_vel1;
    br_vector3 point_vel2;
    br_scalar fudge_multiplier;
    br_scalar factor;
    int need_to_fudge;
}

// Offset: 114564
// Size: 417
// EAX: f
// EDX: m
// EBX: d
// ECX: tau
br_scalar TwoPointCollB(br_scalar *f, br_matrix4 *m, br_scalar *d, br_vector3 *tau, br_vector3 *n) {
    br_scalar ts;
}

// Offset: 114984
// Size: 827
// EAX: f
// EDX: m
// EBX: d
// ECX: tau
br_scalar ThreePointCollRecB(br_scalar *f, br_matrix4 *m, br_scalar *d, br_vector3 *tau, br_vector3 *n) {
    int i;
    int j;
    br_scalar ts;
}

// Offset: 115812
// Size: 789
// EAX: f
// EDX: m
// EBX: d
// ECX: tau
br_scalar FourPointCollB(br_scalar *f, br_matrix4 *m, br_scalar *d, br_vector3 *tau, br_vector3 *n) {
    int i;
    int j;
    int l;
    br_scalar ts;
}

// Offset: 116604
// Size: 89
// EAX: f
int TestForNan(float *f) {
    tU32 i;
}

// Offset: 116696
// Size: 89
void CheckCameraHither() {
    int old_hither;
}

// Offset: 116788
// Size: 525
// EAX: pCar
void SetCarSuspGiveAndHeight(tCar_spec *pCar, br_scalar pFront_give_factor, br_scalar pRear_give_factor, br_scalar pDamping_factor, br_scalar pExtra_front_height, br_scalar pExtra_rear_height) {
    br_scalar front_give;
    br_scalar rear_give;
    br_scalar damping;
    br_scalar ratio;
    int i;
}

// Offset: 117316
// Size: 1023
// EAX: car
int TestForCarInSensiblePlace(tCar_spec *car) {
    br_bounds bnds;
    br_matrix34 mat;
    br_matrix34 *mat1;
    br_matrix34 *mat2;
    int i;
    int j;
    int k;
    tCollision_info *c2;
    tCollision_info *car_info;
    br_vector3 sep;
    br_vector3 tv;
    br_vector3 tv2;
}

// Offset: 118340
// Size: 528
// EAX: c1
// EDX: c2
// EBX: newmats
int TestOldMats(tCollision_info *c1, tCollision_info *c2, int newmats) {
    br_vector3 p1;
    br_vector3 p2;
    br_vector3 tp1;
    br_vector3 tp2;
    br_vector3 tp3;
    br_vector3 hp1;
    br_vector3 hp2;
    br_vector3 hp3;
    br_vector3 shp1;
    br_vector3 shp2;
    br_vector3 edge;
    int plane1;
    int plane2;
    int plane3;
    br_scalar ts;
    int i;
    int j;
    int n;
    br_bounds *b1;
    br_bounds *b2;
    br_matrix34 invmat1;
    br_matrix34 mat21;
}

// Offset: 118868
// Size: 69
// EAX: pActor
int PullActorFromWorld(br_actor *pActor) {
}

// Offset: 118940
// Size: 681
// EAX: pActor
int DoPullActorFromWorld(br_actor *pActor) {
    int num;
    int i;
    tCollision_info *c;
    tNon_car_spec *non_car;
}

// Offset: 119624
// Size: 904
// EAX: pTime
void CheckForDeAttachmentOfNonCars(tU32 pTime) {
    tU32 total_time;
    br_bounds bnds;
    int i;
    int j;
    int last_free_slot;
    int drop;
    tCollision_info *c;
    tCollision_info *c2;
    br_actor *actor;
    tU8 cx;
    tU8 cz;
    tTrack_spec *track_spec;
    br_matrix34 mat;
}

// Offset: 120528
// Size: 248
// EAX: pActor
// EDX: pMat
void AdjustNonCar(br_actor *pActor, br_matrix34 *pMat) {
    tU8 cx;
    tU8 cz;
    tTrack_spec *track_spec;
}

// Offset: 120776
// Size: 240
// EAX: c
void PipeSingleNonCar(tCollision_info *c) {
}

// Offset: 121016
// Size: 169
// EAX: pCar
// EDX: pFace_refs
int GetPrecalculatedFacesUnderCar(tCar_spec *pCar, tFace_ref **pFace_refs) {
}

// Offset: 121188
// Size: 53
br_material* SomeNearbyMaterial() {
}

