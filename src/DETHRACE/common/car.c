#include "car.h"
#include <stdlib.h>

float gEngine_powerup_factor[6];
tCar_spec* gLast_car_to_skid[2];
tS3_sound_tag gSkid_tag[2];
tSave_camera gSave_camera[2];
tFace_ref gFace_list[150];
void (*ControlCar[6])(tCar_spec*, br_scalar);
tNon_car_spec* gActive_non_car_list[50];
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
br_actor* gPed_actor;
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

void DamageUnit(tCar_spec* pCar, int pUnit_type, int pDamage_amount) {
    tDamage_unit* the_damage;
    NOT_IMPLEMENTED();
}

void DamageUnitWithSmoke(tCar_spec* pCar, int pUnit_type, int pDamage_amount) {
    NOT_IMPLEMENTED();
}

void DamageEngine(int pDamage_amount) {
    NOT_IMPLEMENTED();
}

void DamageTrans(int pDamage_amount) {
    NOT_IMPLEMENTED();
}

void DamageSteering(int pDamage_amount) {
    NOT_IMPLEMENTED();
}

void DamageLFWheel(int pDamage_amount) {
    NOT_IMPLEMENTED();
}

void DamageLFBrake(int pDamage_amount) {
    NOT_IMPLEMENTED();
}

void DamageLRBrake(int pDamage_amount) {
    NOT_IMPLEMENTED();
}

void DamageLRWheel(int pDamage_amount) {
    NOT_IMPLEMENTED();
}

void DamageRFWheel(int pDamage_amount) {
    NOT_IMPLEMENTED();
}

void DamageRFBrake(int pDamage_amount) {
    NOT_IMPLEMENTED();
}

void DamageRRBrake(int pDamage_amount) {
    NOT_IMPLEMENTED();
}

void DamageRRWheel(int pDamage_amount) {
    NOT_IMPLEMENTED();
}

void CalculatePlaneNormal(br_vector3* pP1, br_vector3* pP2, br_vector3* pP3, br_vector3* pNormal) {
    br_vector3 p0;
    br_vector3 p1;
    br_vector3 cross_product;
    br_vector3 temp_vector;
    NOT_IMPLEMENTED();
}

void CalculateGroundNormal(br_model* pThe_model, int pFace_index) {
    NOT_IMPLEMENTED();
}

void ChangeYCoordinate(br_scalar pNew_y, tU32 pTime_taken, br_model* pThe_model, int pFace_index) {
    br_scalar y_change;
    br_transform new_transform;
    br_vector3 side_window;
    NOT_IMPLEMENTED();
}

void SwitchCarActor(tCar_spec* pCar_spec, int pModel_index) {
    int i;
    NOT_IMPLEMENTED();
}

void InitialiseCar2(tCar_spec* pCar, int pClear_disabled_flag) {
    int index;
    int j;
    int cat;
    int car_count;
    br_actor* car_actor;
    br_angle initial_yaw;
    br_scalar nearest_y_above;
    br_scalar nearest_y_below;
    br_scalar speed;
    int below_face_index;
    int above_face_index;
    br_model* below_model;
    br_model* above_model;
    br_vector3 grid_offset;
    br_matrix34 initial_yaw_matrix;
    br_matrix34 safe_position;
    NOT_IMPLEMENTED();
}

void InitialiseCar(tCar_spec* pCar) {
    NOT_IMPLEMENTED();
}

void InitialiseCarsEtc(tRace_info* pThe_race) {
    int i;
    int cat;
    int car_count;
    tCar_spec* car;
    br_bounds bnds;
    NOT_IMPLEMENTED();
}

void GetAverageGridPosition(tRace_info* pThe_race) {
    int i;
    br_scalar total_cars;
    tCar_spec* car;
    NOT_IMPLEMENTED();
}

void SetInitialPosition(tRace_info* pThe_race, int pCar_index, int pGrid_index) {
    int place_on_grid;
    int i;
    int start_i;
    int j;
    br_actor* car_actor;
    br_angle initial_yaw;
    br_scalar nearest_y_above;
    br_scalar nearest_y_below;
    br_scalar speed;
    int below_face_index;
    int above_face_index;
    br_model* below_model;
    br_model* above_model;
    tCar_spec* car;
    br_vector3 grid_offset;
    br_vector3 dist;
    br_vector3 real_pos;
    br_matrix34 initial_yaw_matrix;
    br_bounds bnds;
    NOT_IMPLEMENTED();
}

void SetInitialPositions(tRace_info* pThe_race) {
    int i;
    NOT_IMPLEMENTED();
}

void InitialiseNonCar(tNon_car_spec* non_car) {
    tCollision_info* c;
    NOT_IMPLEMENTED();
}

void GetFacesInBox(tCollision_info* c) {
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
    NOT_IMPLEMENTED();
}

int IsCarInTheSea() {
    NOT_IMPLEMENTED();
}

void RememberSafePosition(tCar_spec* car, tU32 pTime) {
    static tU32 time_count;
    int j;
    br_vector3 r;
    br_scalar ts;
    NOT_IMPLEMENTED();
}

void ControlOurCar(tU32 pTime_difference) {
    br_scalar ts;
    br_vector3 minus_k;
    tCar_spec* car;
    static int steering_locked;
    int i;
    tU32 time;
    NOT_IMPLEMENTED();
}

void CalcEngineForce(tCar_spec* c, br_scalar dt) {
    br_scalar torque;
    br_scalar ts;
    br_scalar ts2;
    br_scalar brake_temp;
    int sign;
    tS32 temp_for_swap;
    NOT_IMPLEMENTED();
}

void PrepareCars(tU32 pFrame_start_time) {
    tCar_spec* car;
    int i;
    static tU32 last_frame_start;
    NOT_IMPLEMENTED();
}

void FinishCars(tU32 pLast_frame_time, tU32 pTime) {
    tCar_spec* car;
    br_vector3 minus_k;
    int i;
    int wheel;
    NOT_IMPLEMENTED();
}

void InterpolateCars(tU32 pLast_frame_time, tU32 pTime) {
    br_scalar dt;
    tCar_spec* car;
    int i;
    NOT_IMPLEMENTED();
}

void ResetOldmat() {
    tCar_spec* car;
    int i;
    br_matrix34 mat;
    static int normalise_count;
    NOT_IMPLEMENTED();
}

void GetNonCars() {
    int i;
    int j;
    NOT_IMPLEMENTED();
}

void GetNetPos(tCar_spec* pCar) {
    int j;
    float amount;
    br_scalar total_deflection;
    NOT_IMPLEMENTED();
}

void ApplyPhysicsToCars(tU32 last_frame_time, tU32 pTime_difference) {
    br_vector3 minus_k;
    int i;
    int old_num_cars;
    int step_number;
    int dam_index;
    static int steering_locked;
    tCar_spec* car;
    tCollision_info* car_info;
    tNon_car_spec* non_car;
    tU32 time_step;
    tU32 frame_end_time;
    NOT_IMPLEMENTED();
}

void MungeSpecialVolume(tCollision_info* pCar) {
    tSpecial_volume* new_special_volume;
    tCar_spec* car;
    NOT_IMPLEMENTED();
}

void ResetCarSpecialVolume(tCollision_info* pCar) {
    br_vector3 cast_v;
    br_vector3 norm;
    br_scalar t;
    int id_len;
    char* mat_id;
    tSpecial_volume* new_special_volume;
    br_material* material;
    NOT_IMPLEMENTED();
}

void TestAutoSpecialVolume(tCollision_info* pCar) {
    br_vector3 pos;
    br_scalar car_d;
    br_scalar d;
    br_scalar d2;
    br_vector3 dir;
    br_vector3 tv;
    br_vector3 lp;
    br_vector3 hp;
    tSpecial_volume* vol;
    br_matrix34* mat;
    br_scalar lowest_p;
    br_scalar highest_p;
    br_scalar val;
    int i;
    NOT_IMPLEMENTED();
}

void MoveAndCollideCar(tCar_spec* car, br_scalar dt) {
    tCollision_info* car_info;
    int wheel;
    NOT_IMPLEMENTED();
}

void MoveAndCollideNonCar(tNon_car_spec* non_car, br_scalar dt) {
    tCollision_info* car_info;
    NOT_IMPLEMENTED();
}

int CollideCarWithWall(tCollision_info* car, br_scalar dt) {
    NOT_IMPLEMENTED();
}

void ToggleControls() {
    NOT_IMPLEMENTED();
}

void ControlCar2(tCar_spec* c, br_scalar dt) {
    NOT_IMPLEMENTED();
}

void ControlCar3(tCar_spec* c, br_scalar dt) {
    NOT_IMPLEMENTED();
}

void ControlCar4(tCar_spec* c, br_scalar dt) {
    br_scalar ts;
    NOT_IMPLEMENTED();
}

void ControlCar5(tCar_spec* c, br_scalar dt) {
    NOT_IMPLEMENTED();
}

void ControlCar1(tCar_spec* c, br_scalar dt) {
    NOT_IMPLEMENTED();
}

void setrotate(br_vector3* wdt, br_matrix34* m) {
    br_euler e;
    NOT_IMPLEMENTED();
}

void RotateCar2(tCollision_info* c, br_scalar dt) {
    br_vector3 wdt;
    br_vector3 wdt2;
    br_vector3 L;
    br_vector3 L2;
    br_matrix34 m;
    NOT_IMPLEMENTED();
}

void RotateCarSecondOrder(tCollision_info* c, br_scalar dt) {
    br_vector3 L;
    br_vector3 L2;
    br_vector3 axis;
    br_vector3 omega;
    br_scalar rad;
    br_scalar rad_rate;
    br_matrix34 m;
    NOT_IMPLEMENTED();
}

void RotateCarFirstOrder(tCollision_info* c, br_scalar dt) {
    br_vector3 axis;
    br_vector3 L;
    br_vector3 L2;
    br_matrix34 m;
    br_scalar rad_rate;
    br_scalar rad;
    br_scalar e1;
    br_scalar e2;
    static br_scalar max_rad;
    NOT_IMPLEMENTED();
}

void SimpleRotate(tCollision_info* c, br_scalar dt) {
    br_vector3 axis;
    br_scalar rad_rate;
    br_scalar rad;
    NOT_IMPLEMENTED();
}

void RotateCar(tCollision_info* c, br_scalar dt) {
    br_scalar rad_squared;
    int steps;
    int i;
    NOT_IMPLEMENTED();
}

void SteeringSelfCentre(tCar_spec* c, br_scalar dt, br_vector3* n) {
    br_scalar ts;
    br_scalar ts2;
    NOT_IMPLEMENTED();
}

void NonCarCalcForce(tNon_car_spec* nc, br_scalar dt) {
    tCollision_info* c;
    tSpecial_volume* vol;
    br_scalar ts;
    br_vector3 tv;
    br_vector3 v;
    NOT_IMPLEMENTED();
}

void AddDrag(tCar_spec* c, br_scalar dt) {
    br_scalar drag_multiplier;
    br_scalar ts;
    tSpecial_volume* vol;
    br_vector3 b;
    NOT_IMPLEMENTED();
}

void DoBumpiness(tCar_spec* c, br_vector3* wheel_pos, br_vector3* norm, br_scalar* d, int n) {
    br_vector3 tv;
    int delta;
    int x;
    int y;
    tMaterial_modifiers* mat_list;
    NOT_IMPLEMENTED();
}

void CalcForce(tCar_spec* c, br_scalar dt) {
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
    br_matrix34* mat;
    tMaterial_modifiers* mat_list;
    static br_scalar stop_timer;
    static br_scalar slide_dist;
    tDamage_type dam;
    br_scalar v;
    tSpecial_volume* vol;
    NOT_IMPLEMENTED();
}

void DoRevs(tCar_spec* c, br_scalar dt) {
    br_scalar wheel_spin_force;
    br_scalar ts;
    int revs_increase;
    NOT_IMPLEMENTED();
}

void ApplyTorque(tCar_spec* c, br_vector3* tdt) {
    NOT_IMPLEMENTED();
}

void TranslateCar(tCollision_info* c, br_scalar dt) {
    br_vector3 t;
    NOT_IMPLEMENTED();
}

int CollCheck(tCollision_info* c, br_scalar dt) {
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
    br_matrix34* mat;
    br_matrix34* oldmat;
    br_matrix34 mat_to_oldmat;
    br_matrix34 oldmat_to_mat;
    br_matrix34 tm;
    int collision;
    br_bounds bnds;
    tFace_ref* f_ref;
    int i;
    int j;
    int l;
    int k;
    int material;
    int noise_defeat;
    static int oldk;
    br_scalar min;
    br_scalar max;
    br_vector3 edges[3];
    br_vector3 corner;
    br_vector3 test_dir;
    br_scalar min_acc;
    br_scalar max_acc;
    br_matrix34 message_mat;
    NOT_IMPLEMENTED();
}

br_scalar AddFriction(tCollision_info* c, br_vector3* vel, br_vector3* normal_force, br_vector3* pos, br_scalar total_force, br_vector3* max_friction) {
    br_vector3 norm;
    br_vector3 tv;
    br_vector3 ftau;
    br_scalar ts;
    br_scalar point_vel;
    NOT_IMPLEMENTED();
}

void AddFrictionCarToCar(tCollision_info* car1, tCollision_info* car2, br_vector3* vel1, br_vector3* vel2, br_vector3* normal_force1, br_vector3* pos1, br_vector3* pos2, br_scalar total_force, br_vector3* max_friction) {
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
    NOT_IMPLEMENTED();
}

void ScrapeNoise(br_scalar vel, br_vector3* position, int material) {
    tS3_volume vol;
    static tS3_sound_tag scrape_tag;
    static tS3_volume last_scrape_vol;
    br_vector3 velocity;
    br_vector3 position_in_br;
    NOT_IMPLEMENTED();
}

void SkidNoise(tCar_spec* pC, int pWheel_num, br_scalar pV, int material) {
    br_vector3 pos;
    br_vector3 world_pos;
    br_vector3 wv;
    br_vector3 wvw;
    br_scalar ts;
    static tS3_volume last_skid_vol[2];
    int i;
    NOT_IMPLEMENTED();
}

void StopSkid(tCar_spec* pC) {
    NOT_IMPLEMENTED();
}

void CrashNoise(br_vector3* pForce, br_vector3* position, int material) {
    static tS3_sound_tag crunch_tag;
    static tS3_volume last_crunch_vol;
    tS3_volume vol;
    br_vector3 velocity;
    NOT_IMPLEMENTED();
}

void CrushAndDamageCar(tCar_spec* c, br_vector3* pPosition, br_vector3* pForce_car_space, tCar_spec* car2) {
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
    NOT_IMPLEMENTED();
}

int ExpandBoundingBox(tCar_spec* c) {
    br_scalar min_z;
    br_scalar max_z;
    br_scalar dist;
    br_vector3 tv;
    br_vector3 old_pos;
    int l;
    br_matrix34 mat;
    NOT_IMPLEMENTED();
}

void CrushBoundingBox(tCar_spec* c, int crush_only) {
    br_vector3 min;
    br_vector3 max;
    int i;
    br_actor* actor;
    NOT_IMPLEMENTED();
}

void AddCollPoint(br_scalar dist, br_vector3* p, br_vector3* norm, br_vector3* r, br_vector3* n, br_vector3* dir, int num, tCollision_info* c) {
    static br_scalar d[4];
    int i;
    int furthest;
    NOT_IMPLEMENTED();
}

br_scalar SinglePointColl(br_scalar* f, br_matrix4* m, br_scalar* d) {
    NOT_IMPLEMENTED();
}

br_scalar TwoPointColl(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n) {
    br_scalar ts;
    NOT_IMPLEMENTED();
}

br_scalar DrMatrix4Inverse(br_matrix4* mi, br_matrix4* mc) {
    NOT_IMPLEMENTED();
}

br_scalar ThreePointColl(br_scalar* f, br_matrix4* m, br_scalar* d) {
    br_matrix4 mc;
    br_matrix4 mi;
    br_scalar ts;
    NOT_IMPLEMENTED();
}

br_scalar ThreePointCollRec(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n, tCollision_info* c) {
    int i;
    int j;
    br_scalar ts;
    NOT_IMPLEMENTED();
}

br_scalar FourPointColl(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n, tCollision_info* c) {
    int i;
    int j;
    int l;
    br_scalar ts;
    NOT_IMPLEMENTED();
}

void MultiFindFloorInBoxM(int pNum_rays, br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCar_spec* c, int* mat_ref) {
    br_vector3 aa[4];
    br_vector3 bb;
    int i;
    NOT_IMPLEMENTED();
}

void MultiFindFloorInBoxBU(int pNum_rays, br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCar_spec* c, int* mat_ref) {
    br_vector3 nor2;
    int i;
    int j;
    int l;
    br_scalar dist[4];
    tFace_ref* face_ref;
    NOT_IMPLEMENTED();
}

void findfloor(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d) {
    br_material* material;
    br_vector3 aa;
    br_vector3 bb;
    NOT_IMPLEMENTED();
}

int FindFloorInBoxM(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c) {
    br_vector3 aa;
    br_vector3 bb;
    NOT_IMPLEMENTED();
}

int FindFloorInBoxBU(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c) {
    br_vector3 nor2;
    int i;
    int j;
    br_scalar dist;
    tFace_ref* face_ref;
    NOT_IMPLEMENTED();
}

int FindFloorInBoxBU2(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c) {
    br_vector3 nor2;
    br_vector3 tv;
    int i;
    int j;
    br_scalar dist;
    tFace_ref* face_ref;
    NOT_IMPLEMENTED();
}

int FindFloorInBoxM2(br_vector3* a, br_vector3* b, br_vector3* nor, br_scalar* d, tCollision_info* c) {
    br_vector3 aa;
    br_vector3 bb;
    NOT_IMPLEMENTED();
}

int BoxFaceIntersect(br_bounds* pB, br_matrix34* pM, br_matrix34* pMold, br_vector3* pPoint_list, br_vector3* pNorm_list, br_scalar* pDist_list, int pMax_pnts, tCollision_info* c) {
    br_vector3 p[3];
    br_vector3 tv;
    br_vector3 pos;
    br_bounds bnds;
    int i;
    int j;
    int n;
    int flag;
    int m;
    tFace_ref* f_ref;
    br_face* face;
    NOT_IMPLEMENTED();
}

int AddEdgeCollPoints(br_vector3* p1, br_vector3* p2, br_bounds* pB, br_matrix34* pMold, br_vector3* pPoint_list, br_vector3* pNorm_list, int n, int pMax_pnts, tCollision_info* c) {
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
    NOT_IMPLEMENTED();
}

void GetPlaneNormal(br_vector3* n, int p) {
    int d;
    NOT_IMPLEMENTED();
}

int GetBoundsEdge(br_vector3* pos, br_vector3* edge, br_bounds* pB, int plane1, int plane2, br_vector3* a, br_vector3* b, br_vector3* c, int flag) {
    int d1;
    int d2;
    int d3;
    br_vector3 n;
    br_vector3 p;
    br_vector3 q;
    NOT_IMPLEMENTED();
}

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
    br_model* below_model;
    br_model* above_model;
    NOT_IMPLEMENTED();
}

void ToggleCollisionDetection() {
    NOT_IMPLEMENTED();
}

void CancelPendingCunningStunt() {
    NOT_IMPLEMENTED();
}

float frac(float pN) {
    NOT_IMPLEMENTED();
}

void SetAmbientPratCam(tCar_spec* pCar) {
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
    static tU32 last_time_on_ground;
    NOT_IMPLEMENTED();
}

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
    tCar_spec* the_car;
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
    tSpecial_screen* the_special_screen;
    br_material* the_material;
    tU32 the_time;
    br_actor* oily_actor;
    NOT_IMPLEMENTED();
}

void ResetCarScreens() {
    int car_count;
    int i;
    tCar_spec* the_car;
    NOT_IMPLEMENTED();
}

tCar_spec* GetRaceLeader() {
    int i;
    int score;
    tCar_spec* car;
    NOT_IMPLEMENTED();
}

void AmIGettingBoredWatchingCameraSpin() {
    static tU32 time_of_death;
    static tU32 headup_timer;
    tCar_spec* car;
    char s[256];
    NOT_IMPLEMENTED();
}

void ViewNetPlayer() {
    NOT_IMPLEMENTED();
}

void ViewOpponent() {
    NOT_IMPLEMENTED();
}

void ToggleCarToCarCollisions() {
    NOT_IMPLEMENTED();
}

void SwapCar() {
    NOT_IMPLEMENTED();
}

void AdjustDownForce() {
    tCar_spec* c;
    NOT_IMPLEMENTED();
}

void FreezeMechanics() {
    NOT_IMPLEMENTED();
}

void PutOpponentsInNeutral() {
    NOT_IMPLEMENTED();
}

void SetPanningFieldOfView() {
    br_camera* camera_ptr;
    NOT_IMPLEMENTED();
}

void CheckDisablePlingMaterials(tCar_spec* pCar) {
    br_matrix34* mat;
    br_scalar height;
    int i;
    NOT_IMPLEMENTED();
}

void PositionExternalCamera(tCar_spec* c, tU32 pTime) {
    static int old_camera_mode;
    br_camera* camera_ptr;
    NOT_IMPLEMENTED();
}

void CameraBugFix(tCar_spec* c, tU32 pTime) {
    br_matrix34 mat;
    br_matrix34* m2;
    br_vector3 tv;
    NOT_IMPLEMENTED();
}

int PossibleRemoveNonCarFromWorld(br_actor* pActor) {
    tU8 cx;
    tU8 cz;
    tTrack_spec* track_spec;
    NOT_IMPLEMENTED();
}

void PutNonCarBackInWorld(br_actor* pActor) {
    tU8 cx;
    tU8 cz;
    tTrack_spec* track_spec;
    NOT_IMPLEMENTED();
}

int IncidentCam(tCar_spec* c, tU32 pTime) {
    br_matrix34* m2;
    br_matrix34 mat;
    br_vector3 tv;
    br_vector3 tv2;
    br_vector3 perp;
    br_vector3 vertical;
    br_vector3 murderer_pos;
    br_scalar ts;
    tCar_spec* car2;
    static tU32 next_incident_time;
    static tIncident_type type;
    static float severity;
    static tIncident_info info;
    static int random;
    static int count;
    br_scalar temp;
    br_vector3 old_cam_pos;
    int removed;
    NOT_IMPLEMENTED();
}

int MoveCamToIncident(tCar_spec* c, tIncident_type* type, float* severity, tIncident_info* info, tU32* next_incident_time) {
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
    NOT_IMPLEMENTED();
}

void PanningExternalCamera(tCar_spec* c, tU32 pTime) {
    br_matrix34* m2;
    br_matrix34* m1;
    br_vector3 tv;
    br_scalar ts;
    static int inside_camera_zone;
    NOT_IMPLEMENTED();
}

int CheckForWall(br_vector3* start, br_vector3* end) {
    br_vector3 dir;
    br_material* material;
    br_vector3 normal;
    br_scalar d;
    NOT_IMPLEMENTED();
}

void SetUpPanningCamera(tCar_spec* c) {
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
    br_matrix34* m2;
    br_matrix34* m1;
    br_vector3 left;
    br_vector3 right;
    br_vector3 car_centre;
    int left_score;
    int right_score;
    NOT_IMPLEMENTED();
}

void SaveCameraPosition(int i) {
    NOT_IMPLEMENTED();
}

void RestoreCameraPosition(int i) {
    NOT_IMPLEMENTED();
}

void NormalPositionExternalCamera(tCar_spec* c, tU32 pTime) {
    br_matrix34* m2;
    br_matrix34* m1;
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
    NOT_IMPLEMENTED();
}

void MoveWithWheels(tCar_spec* c, br_vector3* vn, int manual_swing) {
    br_angle yaw;
    br_angle theta;
    static int move_with_wheels;
    NOT_IMPLEMENTED();
}

void SwingCamera(tCar_spec* c, br_matrix34* m1, br_matrix34* m2, br_vector3* vn, tU32 pTime) {
    int i;
    br_scalar ts;
    br_angle yaw;
    br_angle theta;
    br_angle alpha;
    br_scalar sin_dtheta;
    br_scalar cos_dtheta;
    br_scalar sign;
    int manual_swing;
    static br_angle omega;
    static int elapsed_time;
    static br_vector3 old_vn;
    NOT_IMPLEMENTED();
}

void PointCameraAtCar(tCar_spec* c, br_matrix34* m1, br_matrix34* m2) {
    br_vector3 vn;
    br_vector3 tv;
    br_vector3 tv2;
    br_scalar dist;
    br_scalar frac;
    br_angle theta;
    br_vector3* pos;
    br_camera* camera_ptr;
    br_angle off_centre_angle;
    int swoop;
    NOT_IMPLEMENTED();
}

void PointCamera(br_vector3* pos, br_matrix34* m2) {
    br_vector3 vn;
    br_scalar dist;
    br_angle theta;
    br_camera* camera_ptr;
    NOT_IMPLEMENTED();
}

int CollideCamera2(br_vector3* car_pos, br_vector3* cam_pos, br_vector3* old_camera_pos, int manual_move) {
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
    br_material* material;
    br_scalar alpha;
    br_scalar sa;
    br_scalar sb;
    br_scalar sc;
    tFace_ref face_list[3];
    NOT_IMPLEMENTED();
}

int BoundsTest(br_bounds* bnds, br_vector3* p) {
    int j;
    NOT_IMPLEMENTED();
}

int CollideCameraWithOtherCars(br_vector3* car_pos, br_vector3* cam_pos) {
    int i;
    int plane;
    br_scalar ts;
    tCar_spec* c;
    br_vector3 tv;
    br_vector3 pos_car_space;
    br_vector3 dir;
    br_vector3 p;
    br_vector3 n;
    br_bounds bnds;
    NOT_IMPLEMENTED();
}

void InitialiseExternalCamera() {
    tCar_spec* c;
    br_vector3 r;
    br_angle yaw;
    NOT_IMPLEMENTED();
}

void FreezeCamera() {
    NOT_IMPLEMENTED();
}

void FlyCar(tCar_spec* c, br_scalar dt) {
    int accflag;
    int turnflag;
    br_vector3 step;
    br_matrix34* mat;
    br_angle theta;
    static br_scalar vel;
    tFace_ref faces[20];
    tBounds bnds;
    NOT_IMPLEMENTED();
}

void DrVector3RotateY(br_vector3* v, br_angle t) {
    br_scalar c;
    br_scalar s;
    br_scalar ts;
    NOT_IMPLEMENTED();
}

void CrashCarsTogether(br_scalar dt) {
    int pass;
    int k;
    int i;
    tCollison_data collide_list[32];
    NOT_IMPLEMENTED();
}

int CrashCarsTogetherSinglePass(br_scalar dt, int pPass, tCollison_data* collide_list) {
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
    tCollision_info* car_1;
    tCollision_info* car_2;
    tCollision_info* car_3;
    tCollision_info* car_in_middle;
    tCollision_info* car_on_wall;
    NOT_IMPLEMENTED();
}

void BringCarToAGrindingHalt(tCollision_info* car) {
    NOT_IMPLEMENTED();
}

int SimpleCarCarCollisionTest(tCollision_info* car1, tCollision_info* car2) {
    NOT_IMPLEMENTED();
}

int CollideTwoCarsWithWalls(tCollision_info* car1, tCollision_info* car2, br_scalar dt) {
    br_vector3 mom1;
    br_vector3 mom2;
    int l;
    int m;
    int n;
    int p;
    int im1;
    int im2;
    NOT_IMPLEMENTED();
}

int CollideTwoCarsRepeatedly(tCollision_info* car1, tCollision_info* car2, br_scalar dt) {
    int l;
    int collide;
    br_scalar ts;
    NOT_IMPLEMENTED();
}

int CollideTwoCars(tCollision_info* car1, tCollision_info* car2, int pPass) {
    int k;
    int old_k;
    int i;
    int j;
    br_scalar dist;
    br_scalar ts;
    br_bounds new_car1_bnds;
    br_bounds new_car2_bnds;
    br_bounds bnds;
    br_matrix34* mat1;
    br_matrix34* mat2;
    br_matrix34* oldmat1;
    br_matrix34* oldmat2;
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
    static br_vector3 oldr1;
    static br_vector3 oldr2;
    static br_vector3 oldn1;
    static br_vector3 oldn2;
    static int is_old_point_available;
    NOT_IMPLEMENTED();
}

int GetEdgeEdgeCollisions(br_bounds* pB1, br_bounds* pB2, br_matrix34* pM21, br_matrix34* pM12, br_matrix34* pMo21, br_matrix34* pMo12, br_matrix34* pM1o1, br_vector3* pPoint_list, br_vector3* pNorm_list, int pMax) {
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
    NOT_IMPLEMENTED();
}

int FacePointCarCarCollide(tCollision_info* car1, tCollision_info* car2, br_matrix34* pMms, br_matrix34* pMoms, br_matrix34* pMsos, br_vector3* pPoint_list, br_vector3* pNorm_list, int pMax, int order) {
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
    br_bounds* pStat_box;
    br_bounds* pMove_box;
    NOT_IMPLEMENTED();
}

void MungeCarsMass(tCollision_info* pCar, br_scalar pFactor) {
    NOT_IMPLEMENTED();
}

void ModifyCarsMass(tCollision_info* pCar_1, tCollision_info* pCar_2) {
    NOT_IMPLEMENTED();
}

void ResetCarsMass(tCollision_info* pCar_1, tCollision_info* pCar_2) {
    NOT_IMPLEMENTED();
}

int DoCollide(tCollision_info* car1, tCollision_info* car2, br_vector3* r, br_vector3* n, int k, int pPass, br_matrix34* mat1_to_mat2) {
    br_matrix34* mat1;
    br_matrix34* mat2;
    br_matrix34* oldmat1;
    br_matrix34* oldmat2;
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
    br_vector3* tau2;
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
    NOT_IMPLEMENTED();
}

br_scalar TwoPointCollB(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n) {
    br_scalar ts;
    NOT_IMPLEMENTED();
}

br_scalar ThreePointCollRecB(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n) {
    int i;
    int j;
    br_scalar ts;
    NOT_IMPLEMENTED();
}

br_scalar FourPointCollB(br_scalar* f, br_matrix4* m, br_scalar* d, br_vector3* tau, br_vector3* n) {
    int i;
    int j;
    int l;
    br_scalar ts;
    NOT_IMPLEMENTED();
}

int TestForNan(float* f) {
    tU32 i;
    NOT_IMPLEMENTED();
}

void CheckCameraHither() {
    static int old_hither;
    NOT_IMPLEMENTED();
}

void SetCarSuspGiveAndHeight(tCar_spec* pCar, br_scalar pFront_give_factor, br_scalar pRear_give_factor, br_scalar pDamping_factor, br_scalar pExtra_front_height, br_scalar pExtra_rear_height) {
    br_scalar front_give;
    br_scalar rear_give;
    br_scalar damping;
    br_scalar ratio;
    int i;
    NOT_IMPLEMENTED();
}

int TestForCarInSensiblePlace(tCar_spec* car) {
    br_bounds bnds;
    br_matrix34 mat;
    br_matrix34* mat1;
    br_matrix34* mat2;
    int i;
    int j;
    int k;
    tCollision_info* c2;
    tCollision_info* car_info;
    br_vector3 sep;
    br_vector3 tv;
    br_vector3 tv2;
    NOT_IMPLEMENTED();
}

int TestOldMats(tCollision_info* c1, tCollision_info* c2, int newmats) {
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
    br_bounds* b1;
    br_bounds* b2;
    br_matrix34 invmat1;
    br_matrix34 mat21;
    NOT_IMPLEMENTED();
}

int PullActorFromWorld(br_actor* pActor) {
    NOT_IMPLEMENTED();
}

int DoPullActorFromWorld(br_actor* pActor) {
    int num;
    int i;
    tCollision_info* c;
    tNon_car_spec* non_car;
    NOT_IMPLEMENTED();
}

void CheckForDeAttachmentOfNonCars(tU32 pTime) {
    static tU32 total_time;
    br_bounds bnds;
    int i;
    int j;
    int last_free_slot;
    int drop;
    tCollision_info* c;
    tCollision_info* c2;
    br_actor* actor;
    tU8 cx;
    tU8 cz;
    tTrack_spec* track_spec;
    br_matrix34 mat;
    NOT_IMPLEMENTED();
}

void AdjustNonCar(br_actor* pActor, br_matrix34* pMat) {
    tU8 cx;
    tU8 cz;
    tTrack_spec* track_spec;
    NOT_IMPLEMENTED();
}

void PipeSingleNonCar(tCollision_info* c) {
    NOT_IMPLEMENTED();
}

int GetPrecalculatedFacesUnderCar(tCar_spec* pCar, tFace_ref** pFace_refs) {
    NOT_IMPLEMENTED();
}

br_material* SomeNearbyMaterial() {
    NOT_IMPLEMENTED();
}
