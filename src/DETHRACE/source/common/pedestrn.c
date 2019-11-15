#include "pedestrn.h"

int gPed_gib_counts[4][5];
int gPed_gib_maxes[4][5];
tPedestrian_instruction gPed_instrucs[100];
char *gPed_geb_names[4][5];
char *gInstruc_commands[10];
float gMin_ped_gib_speeds[4];
tPed_gib gPed_gibs[30];
float gPed_gib_speeds[4];
char *gPed_gib_names[4][5];
float gPed_gib_distrib[4];
tPed_gib_materials gPed_gib_materials[4];
int gPed_size_counts[4];
tProximity_ray gProximity_rays[20];
int gVesuvian_corpses;
int gPed_colliding;
float gZombie_factor;
int gRespawn_variance;
br_scalar gPed_scale_factor;
int gTotal_peds;
int gPedestrian_harvest;
br_actor *gPath_actor;
br_vector3 gPed_pos_camera;
int gMin_respawn_time;
br_material *gPath_mat_calc;
br_actor *gCurrent_ped_path_actor;
float gPedestrian_speed_factor;
int gExploding_pedestrians;
int gBlind_pedestrians;
br_material *gPed_material;
br_material *gPath_mat_normal;
int gPedestrians_on;
br_material *gInit_pos_mat_calc;
int gPed_other;
int gAttracted_pedestrians;
int gPed_count;
int gPed_ref_num;
br_scalar gMax_distance_squared;
br_model *gPed_model;
float gDanger_level;
br_scalar gExploding_ped_scale[3];
br_vector3 gDanger_direction;
int gPed_instruc_count;
int gInit_ped_instruc;
br_vector3 gZero_v;
char *gRate_commands[3];
char *gCollide_commands[1];
int gCurrent_lollipop_index;
int gDetect_peds;
int gVesuvians_this_time;
int gNumber_of_ped_gibs;
tU32 gLast_ped_message_send;
tPedestrian_data *gFlag_waving_bastard;
int gReally_stupid_ped_bug_enable;
int gNumber_of_pedestrians;
br_pixelmap *gProx_ray_shade_table;
tPedestrian_data *gPedestrian_array;
int gVesuvians_last_time;
tU32 gLast_ped_splat_time;
int gCurrent_ped_multiplier;
int gPed_sound_disable;
tPedestrian_instruction *gInitial_instruction;
int gSend_peds;

// Offset: 0
// Size: 152
// EAX: pModel
void PedModelUpdate(br_model *pModel, br_scalar x0, br_scalar y0, br_scalar x1, br_scalar y1, br_scalar x2, br_scalar y2, br_scalar x3, br_scalar y3) {
}

// Offset: 152
// Size: 98
// EAX: pActor
int ActorIsPedestrian(br_actor *pActor) {
}

// Offset: 252
// Size: 79
// EAX: pActor
br_scalar PedHeightFromActor(br_actor *pActor) {
}

// Offset: 332
// Size: 52
// EAX: pActor
int GetPedestrianValue(br_actor *pActor) {
}

// Offset: 384
// Size: 88
// EAX: pActor
int PedestrianActorIsPerson(br_actor *pActor) {
}

// Offset: 472
// Size: 87
// EAX: pIndex
br_actor* GetPedestrianActor(int pIndex) {
}

// Offset: 560
// Size: 97
// EAX: pActor
// EDX: pFlipped
br_pixelmap* GetPedestrianTexture(br_actor *pActor, int *pFlipped) {
}

// Offset: 660
// Size: 153
void TogglePedestrians() {
}

// Offset: 816
// Size: 918
void InitPedGibs() {
    int j;
    br_model *the_model;
    br_pixelmap *the_pix;
    br_material *the_material;
}

// Offset: 1736
// Size: 240
// EAX: pActor
void SetPedMaterialForRender(br_actor *pActor) {
    tPedestrian_data *ped;
    int changed;
}

// Offset: 1976
// Size: 158
void PedCallBack(br_actor *pActor, br_model *pModel, br_material *pMaterial, void *pRender_data, br_uint_8 pStyle, int pOn_screen) {
    tPedestrian_data *ped;
}

// Offset: 2136
// Size: 516
void InitPeds() {
}

// Offset: 2652
// Size: 275
// EAX: pActor
void MungeModelSize(br_actor *pActor, br_scalar pScaling_factor) {
    br_pixelmap *the_pix;
    br_scalar half_width;
    br_scalar half_height;
}

// Offset: 2928
// Size: 1468
// EAX: pPedestrian
int BurstPedestrian(tPedestrian_data *pPedestrian, float pSplattitudinalitude, int pAllow_explosion) {
    int i;
    int j;
    int gib_index;
    int next_gib_index;
    int size_threshold;
    int current_size;
    int max_size;
    int gib_count;
    int exploded;
    tPed_gib *the_ped_gib;
    br_scalar min_speed;
    br_scalar max_speed;
    tU32 the_time;
}

// Offset: 4396
// Size: 137
void ResetAllPedGibs() {
    tPed_gib *the_ped_gib;
}

// Offset: 4536
// Size: 200
// EAX: pIndex
// EDX: pSize
// EBX: pGib_index
// ECX: pPed_index
void AdjustPedGib(int pIndex, int pSize, int pGib_index, int pPed_index, br_matrix34 *pTrans) {
    tPed_gib *the_ped_gib;
}

// Offset: 4736
// Size: 810
// EAX: pFrame_period
void MungePedGibs(tU32 pFrame_period) {
    int i;
    int frame;
    tPed_gib *the_ped_gib;
    br_scalar s_frame_period;
    tU32 the_time;
    tPedestrian_data *pedestrian;
}

// Offset: 5548
// Size: 267
// EAX: pPedestrian
void KillPedestrian(tPedestrian_data *pPedestrian) {
}

// Offset: 5816
// Size: 302
// EAX: pPedestrian
// EDX: pPixelmap
// EBX: pHeight
// ECX: pWidth
void CalcPedWidthNHeight(tPedestrian_data *pPedestrian, br_pixelmap *pPixelmap, br_scalar *pHeight, br_scalar *pWidth) {
    br_pixelmap *reference_pixel_bastard;
    br_scalar scale_to_use;
}

// Offset: 6120
// Size: 1942
// EAX: pPedestrian
int PedestrianNextInstruction(tPedestrian_data *pPedestrian, float pDanger_level, int pPosition_explicitly, int pMove_pc) {
    tPedestrian_instruction *instruction;
    float most_dangerous;
    float chance_value;
    int result;
    int start_index;
    int end_index;
    int i;
    int j;
    int choice;
    int the_marker_ref;
}

// Offset: 8064
// Size: 464
// EAX: pPedestrian
// EDX: pAction_changed
void MungePedestrianSequence(tPedestrian_data *pPedestrian, int pAction_changed) {
    tPedestrian_action *the_action;
    int i;
    int the_sequence;
    int current_looping;
    br_scalar ped_movement_angle;
    float heading_difference;
    tPedestrian_sequence *sequence_ptr;
}

// Offset: 8528
// Size: 166
// EAX: pPedestrian
void DetachPedFromCar(tPedestrian_data *pPedestrian) {
}

// Offset: 8696
// Size: 117
// EAX: pPedestrian
void SetPedPos(tPedestrian_data *pPedestrian) {
    br_vector3 temp_v;
}

// Offset: 8816
// Size: 59
// EAX: pActor
void DetachPedActorFromCar(br_actor *pActor) {
}

// Offset: 8876
// Size: 1317
// EAX: pPedestrian
void MungePedestrianFrames(tPedestrian_data *pPedestrian) {
    tPedestrian_sequence *the_sequence;
    float f_the_time;
    float frame_period;
    int frame_offset;
    int number_of_frames;
    int new_frame;
    tU32 the_time;
}

// Offset: 10196
// Size: 1288
// EAX: pPedestrian
void MungePedModel(tPedestrian_data *pPedestrian) {
    br_vector3 old_pos;
    br_vector3 delta_pos;
    br_scalar height_over2;
    br_matrix34 mat;
    br_scalar x_offset;
    br_scalar y_offset;
    br_scalar temp_scalar;
    br_scalar *component_address;
    br_pixelmap *current_pixel_bastard;
    br_pixelmap *reference_pixel_bastard;
    br_vertex *vertices;
    int model_changed;
    tPed_frame_info *the_frame;
    tCar_spec *murderer;
    br_actor *old_parent;
}

// Offset: 11484
// Size: 408
// EAX: pPedestrian
// EDX: pAction_index
// EBX: pRedo_frames_etc
void ChangeActionTo(tPedestrian_data *pPedestrian, int pAction_index, int pRedo_frames_etc) {
    tPedestrian_action *the_action;
    int the_sound;
    tU32 the_pitch;
}

// Offset: 11892
// Size: 589
// EAX: pPedestrian
int MungePedestrianAction(tPedestrian_data *pPedestrian, float pDanger_level) {
    tU32 time_diff;
    int i;
    int choice;
    int start_index;
    int end_index;
    float chance_value;
    float most_dangerous;
}

// Offset: 12484
// Size: 105
void MakeFlagWavingBastardWaveHisFlagWhichIsTheProbablyTheLastThingHeWillEverDo() {
}

// Offset: 12592
// Size: 2334
// EAX: pPedestrian
void MungePedestrianPath(tPedestrian_data *pPedestrian, float pDanger_level, br_vector3 *pDanger_direction) {
    tPedestrian_action *the_action;
    float terminal_speed;
    float new_falling_speed;
    float grav_times_period;
    float ped_gravity;
    br_vector3 movement_vector;
    br_vector3 over_shoot;
    br_vector3 cast_point;
    br_scalar old_y;
    br_scalar new_y;
    int damage;
    tU32 the_pitch;
}

// Offset: 14928
// Size: 764
// EAX: pPedestrian
// EDX: pDanger_direction
float CalcPedestrianDangerLevel(tPedestrian_data *pPedestrian, br_vector3 *pDanger_direction) {
    br_vector3 *ped_pos;
    int i;
    float most_dangerous;
    float this_danger;
    br_scalar distance_squared;
    br_scalar car_movement_angle;
    br_scalar car_to_pedestrian_angle;
    br_scalar heading_difference;
    br_scalar camera_view_angle;
    tCar_spec *car;
}

// Offset: 15692
// Size: 972
// EAX: pPedestrian
// EDX: pCar
// EBX: pCar_actor
tPed_hit_position MoveToEdgeOfCar(tPedestrian_data *pPedestrian, tCollision_info *pCar, br_actor *pCar_actor, br_scalar pPed_x, br_scalar pPed_z, br_scalar pCar_bounds_min_x, br_scalar pCar_bounds_max_x, br_scalar pCar_bounds_min_z, br_scalar pCar_bounds_max_z, br_vector3 *pMin_ped_bounds_car, br_vector3 *pMax_ped_bounds_car) {
    br_vector3 car_plus_ped;
    br_vector3 delta_vector;
    br_vector3 ped_move_in_car;
    br_vector3 ped_move_in_global;
    br_vector3 scaled_car_direction;
    br_vector3 scaled_ped_direction;
    br_matrix34 global_to_car;
    br_scalar z_to_use;
    br_scalar x_to_use;
    br_scalar t;
    br_scalar x;
    br_scalar z;
    tPed_hit_position result;
}

// Offset: 16664
// Size: 101
void CheckLastPed() {
}

// Offset: 16768
// Size: 319
// EAX: pCar
// EDX: pPed_car
int BloodyWheels(tCar_spec *pCar, br_vector3 *pPed_car, br_scalar pSize, br_vector3 *pPed_glob) {
    int wheel;
    int squish;
    br_scalar ped_m_z;
    br_scalar ped_m_x;
    br_scalar dist_sqr;
    br_scalar size_sqr;
}

// Offset: 17088
// Size: 142
// EAX: pPedestrian
// EDX: pCar
int FancyATossOffMate(tPedestrian_data *pPedestrian, tCollision_info *pCar, float pImpact_speed) {
}

// Offset: 17232
// Size: 5870
// EAX: pPedestrian
void CheckPedestrianDeathScenario(tPedestrian_data *pPedestrian) {
    int norman;
    int i;
    int tossing;
    int orig_gib_flag;
    int exploded;
    int proximity_rayed;
    int fated;
    int billiards_shot;
    int credits_value;
    br_vector3 *car_pos;
    br_vector3 *ped_pos;
    br_vector3 min_ped_bounds;
    br_vector3 max_ped_bounds;
    br_vector3 min_ped_bounds_car;
    br_vector3 max_ped_bounds_car;
    br_vector3 ped_to_car_vector;
    br_vector3 up;
    br_vector3 zero_v;
    br_scalar attitude;
    br_scalar distance_squared;
    br_scalar ped_centre_x;
    br_scalar ped_centre_y;
    br_scalar car_bounds_min_x;
    br_scalar car_bounds_max_x;
    br_scalar car_bounds_min_z;
    br_scalar car_bounds_max_z;
    br_scalar prev_car_bounds_min_x;
    br_scalar prev_car_bounds_max_x;
    br_scalar prev_car_bounds_min_z;
    br_scalar prev_car_bounds_max_z;
    br_scalar scalar_frame_time;
    br_scalar gross_dismiss;
    br_actor *car_actor;
    br_actor *incident_actor;
    br_matrix34 ped_to_car;
    tCollision_info *the_car;
    float impact_speed;
    float volume_damage;
    tU32 the_time;
    tPed_hit_position hit_pos;
}

// Offset: 23104
// Size: 769
// EAX: pPedestrian
// EDX: pIndex
void SendPedestrian(tPedestrian_data *pPedestrian, int pIndex) {
    tNet_contents *the_contents;
    tNet_message *the_message;
    int size_decider;
}

// Offset: 23876
// Size: 1162
// EAX: pPedestrian
// EDX: pIndex
void DoPedestrian(tPedestrian_data *pPedestrian, int pIndex) {
    float danger_level;
    float start_speed;
    int action_changed;
    int alive;
    int old_frame;
    int start_ins;
    int start_act;
    int start_hp;
    int start_ins_dir;
    br_vector3 danger_direction;
    br_vector3 old_pos;
}

// Offset: 25040
// Size: 767
// EAX: pIndex
// EDX: pAction_index
// EBX: pFrame_index
// ECX: pHit_points
void AdjustPedestrian(int pIndex, int pAction_index, int pFrame_index, int pHit_points, int pDone_initial, tU16 pParent, br_actor *pParent_actor, float pSpin_period, br_scalar pJump_magnitude, br_vector3 *pOffset, br_vector3 *pTrans) {
    tPedestrian_data *pedestrian;
    br_actor *parent;
    br_vector3 old_pos;
}

// Offset: 25808
// Size: 194
// EAX: pFirst_vertex
// EDX: pPoint
void SquirtPathVertex(br_vertex *pFirst_vertex, br_vector3 *pPoint) {
}

// Offset: 26004
// Size: 92
void ResetAllPedestrians() {
    tPedestrian_data *the_pedestrian;
}

// Offset: 26096
// Size: 476
// EAX: pPedestrian
void GroundPedestrian(tPedestrian_data *pPedestrian) {
    br_scalar new_y;
    br_vector3 cast_point;
    tPedestrian_sequence *sequence;
}

// Offset: 26572
// Size: 483
// EAX: pPedestrian
// EDX: pAnimate
void RevivePedestrian(tPedestrian_data *pPedestrian, int pAnimate) {
}

// Offset: 27056
// Size: 1078
// EAX: pFrame_period
void MungePedestrians(tU32 pFrame_period) {
    int i;
    int mirror_on;
    tPedestrian_data *the_pedestrian;
    br_vector3 ped_pos;
    br_vector3 ped_pos_car;
    br_vector3 ped_pos_temp;
    br_matrix34 ped_pos_matrix;
    br_camera *camera_ptr;
    br_scalar max_distance;
    br_scalar distance_squared;
    br_scalar x_delta;
    br_scalar y_delta;
    br_scalar z_delta;
    tS32 diff;
}

// Offset: 28136
// Size: 201
void RespawnPedestrians() {
    tPedestrian_data *the_pedestrian;
}

// Offset: 28340
// Size: 45
int GetPedCount() {
}

// Offset: 28388
// Size: 260
// EAX: pIndex
// EDX: pPos
int GetPedPosition(int pIndex, br_vector3 *pPos) {
    tPedestrian_data *pedestrian;
}

// Offset: 28648
// Size: 2930
// EAX: pG
// EDX: pInstructions
// EBX: pInstruc_count
// ECX: pInit_instruc
void CreatePedestrian(FILE *pG, tPedestrian_instruction *pInstructions, int pInstruc_count, int pInit_instruc, int pRef_num, int pForce_read) {
    tPath_name the_path;
    char s[256];
    char s2[256];
    char *str;
    char *str2;
    tPedestrian_data *the_pedestrian;
    tPedestrian_sequence *the_sequence;
    tPedestrian_action *the_action;
    br_model *the_model;
    int the_ref;
    int i;
    int j;
    int k;
    int l;
    int temp_int;
    int new_frames;
    tPed_collide_type collide_frame;
    float temp_float1;
    float temp_float2;
    br_scalar height;
    br_scalar width_over_2;
    br_scalar minnest_min;
    br_scalar maxest_min;
    br_scalar minnest_max;
    br_scalar maxest_max;
}

// Offset: 31580
// Size: 73
void ResetProxRay() {
}

// Offset: 31656
// Size: 34
void PedMaterialFromHell() {
}

// Offset: 31692
// Size: 34
void ResetPedMaterial() {
    int j;
}

// Offset: 31728
// Size: 1879
// EAX: pF
// EDX: pSubs_count
// EBX: pSubs_array
void LoadInPedestrians(FILE *pF, int pSubs_count, tPed_subs *pSubs_array) {
    tPath_name the_path;
    char s[256];
    char s2[256];
    char *str;
    char *str2;
    FILE *g;
    int the_ref;
    int i;
    int j;
    int k;
    int l;
    int knock_out;
    int check_for_duplicates;
    int duplicates_found;
    int temp_int;
    int ref_number;
    int instruc_count;
    int ped_count;
    int init_instruc;
    float temp_float1;
    float temp_float2;
    tPedestrian_instruction *instructions;
    tPedestrian_instruction *the_instruction;
    tPed_choice *the_choice;
}

// Offset: 33608
// Size: 1313
// EAX: pInstructions
// EDX: pInstruc_count
// EBX: pInit_instruc
br_actor* BuildPedPaths(tPedestrian_instruction *pInstructions, int pInstruc_count, int pInit_instruc) {
    int vertex_count;
    int last_vertex_count;
    int face_count;
    int j;
    int point_count;
    br_vector3 the_point;
    br_material *the_mat;
    br_model *the_model;
    br_actor *the_actor;
}

// Offset: 34924
// Size: 1161
void WriteOutPeds() {
    int j;
    int k;
    int point_count;
    int min_ped;
    int min_point;
    br_vector3 the_point;
    br_vector3 last_point;
    br_scalar min_distance;
    br_scalar the_distance;
    char s[255];
    tPedestrian_data *the_pedestrian;
    tPedestrian_instruction *the_instruction;
    tPed_choice *the_choice;
    FILE *f;
    tPath_name the_path;
}

// Offset: 36088
// Size: 213
void AddPed() {
    FILE *g;
    tPath_name the_path;
}

// Offset: 36304
// Size: 296
// EAX: pRef_num
void NewPed(int pRef_num) {
    char s[255];
}

// Offset: 36600
// Size: 113
void RemoveCurrentPedPath() {
}

// Offset: 36716
// Size: 89
void ScrubPedestrian() {
}

// Offset: 36808
// Size: 134
void TogglePedDetect() {
}

// Offset: 36944
// Size: 41
void NewPed0() {
}

// Offset: 36988
// Size: 44
void NewPed1() {
}

// Offset: 37032
// Size: 44
void NewPed2() {
}

// Offset: 37076
// Size: 44
void NewPed3() {
}

// Offset: 37120
// Size: 44
void NewPed4() {
}

// Offset: 37164
// Size: 44
void NewPed5() {
}

// Offset: 37208
// Size: 44
void NewPed6() {
}

// Offset: 37252
// Size: 44
void NewPed7() {
}

// Offset: 37296
// Size: 44
void NewPed8() {
}

// Offset: 37340
// Size: 44
void NewPed9() {
}

// Offset: 37384
// Size: 44
void NewPed0B() {
}

// Offset: 37428
// Size: 44
void NewPed1B() {
}

// Offset: 37472
// Size: 44
void NewPed2B() {
}

// Offset: 37516
// Size: 44
void NewPed3B() {
}

// Offset: 37560
// Size: 44
void NewPed4B() {
}

// Offset: 37604
// Size: 44
void NewPed5B() {
}

// Offset: 37648
// Size: 44
void NewPed6B() {
}

// Offset: 37692
// Size: 44
void NewPed7B() {
}

// Offset: 37736
// Size: 44
void NewPed8B() {
}

// Offset: 37780
// Size: 44
void NewPed9B() {
}

// Offset: 37824
// Size: 100
void MungeShowPedPath() {
}

// Offset: 37924
// Size: 101
void DropPedPoint2() {
}

// Offset: 38028
// Size: 67
void DropPedPoint() {
}

// Offset: 38096
// Size: 77
void DropInitPedPoint() {
}

// Offset: 38176
// Size: 126
void DropPedPointAir2() {
}

// Offset: 38304
// Size: 67
void DropPedPointAir() {
}

// Offset: 38372
// Size: 77
void DropInitPedPointAir() {
}

// Offset: 38452
// Size: 77
br_uint_32 KillActorsModel(br_actor *pActor, void *pArg) {
}

// Offset: 38532
// Size: 154
void DisposePedPaths() {
}

// Offset: 38688
// Size: 417
// EAX: pPed_index
// EDX: pPoint_index
void GetPedPos(int *pPed_index, int *pPoint_index) {
    int i;
    int j;
    int point_count;
    br_vector3 the_point;
    br_vector3 last_point;
    br_scalar min_distance;
    br_scalar the_distance;
}

// Offset: 39108
// Size: 105
void ShowPedPos() {
    int min_point;
    char s[255];
}

// Offset: 39216
// Size: 381
void ShowPedPaths() {
    br_model *the_model;
}

// Offset: 39600
// Size: 104
void PullPedPoint() {
    int the_point;
}

// Offset: 39704
// Size: 141
void PullPedPointAir() {
    int the_point;
}

// Offset: 39848
// Size: 135
void DeletePedPath() {
    int the_point;
    int i;
}

// Offset: 39984
// Size: 89
void DeletePedPoint() {
}

// Offset: 40076
// Size: 589
void DisposePedestrians() {
    int j;
    tPedestrian_data *the_pedestrian;
}

// Offset: 40668
// Size: 364
void DoPedReport() {
    tPath_name the_path;
    time_t the_bloody_time;
    int i;
    int j;
    int count;
    int last_ref_num;
}

// Offset: 41032
// Size: 1223
// EAX: pRender_screen
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pCamera_to_world
void RenderProximityRays(br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_actor *pCamera, br_matrix34 *pCamera_to_world, tU32 pTime) {
    int i;
    float seed;
    tU32 the_time;
    br_vector3 car_pos;
    br_vector3 ped_pos;
    br_vector3 car_pos_cam;
    br_vector3 ped_pos_cam;
    br_vector3 car_add;
    br_vector3 car_add_c;
    br_vector3 ped_add;
    br_vector3 ray;
    br_vector3 r1;
    br_vector3 from_pos;
    br_vector3 to_pos;
    br_model *car_model;
    br_scalar distance;
    br_scalar t;
}

// Offset: 42256
// Size: 145
// EAX: pRay_index
// EDX: pCar_ID
// EBX: pPed_index
// ECX: pTime
void AdjustProxRay(int pRay_index, tU16 pCar_ID, tU16 pPed_index, tU32 pTime) {
}

// Offset: 42404
// Size: 1200
// EAX: pContents
// EDX: pMessage
// EBX: pReceive_time
void ReceivedPedestrian(tNet_contents *pContents, tNet_message *pMessage, tU32 pReceive_time) {
    tPedestrian_data *the_pedestrian;
    tNet_game_player_info *sender;
    tNet_game_player_info *murderer;
    int modified_action;
    int modified_action_2;
    int action;
    int instruction;
    int dead;
    tPedestrian_sequence *the_sequence;
}

// Offset: 43604
// Size: 115
// EAX: pPlayer
void SendAllPedestrianPositions(tPlayer_ID pPlayer) {
    int i;
    tPedestrian_data *the_pedestrian;
    tNet_contents *the_contents;
}

