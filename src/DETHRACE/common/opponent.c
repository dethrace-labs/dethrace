#include "opponent.h"

char gOppo_path_filename[256];
br_scalar gIn_view_distance;
tU8* gBit_per_node;
int gGrudge_reduction_per_period;
int gSFS_max_cycles;
int gChallenger_index;
int gSFS_cycles_this_time;
br_scalar gMinimum_yness_before_knackerisation;
int gWanky_arse_tit_fuck;
br_scalar gHead_on_cos_value;
int gSFS_count;
int gSFS_total_cycles;
tU32 gNext_grudge_reduction;
br_scalar gCop_pursuit_speed_percentage_multiplier;
br_scalar gDefinite_cop_pursuit_speed;
int gAcknowledged_start;
int gMin_bangness;
int gStart_jumped;
int gNum_of_opponents_getting_near;
tU32 gNext_elastication;
int gNumber_of_cops_before_faffage;
int gFirst_frame;
tU32 gAcme_frame_count;
char* gDrone_name;
br_scalar gDefinite_no_cop_pursuit_speed;
tU32 gNext_write_during_elastication;
int gNum_of_opponents_completing_race;
int gNum_of_opponents_pursuing;
int gMax_bangness;
int gActive_car_list_rebuild_required;
char* gCop_name;
br_scalar gFrame_period_for_this_munging_in_secs;
int gBig_bang;
char* gPath_section_type_names[3];
br_material* gMat_lt_blu;
int gFaces_used_in_non_edit_paths;
br_material* gMat_dk_blu;
int gMats_allocated;
int gBIG_APC_index;
br_material* gMat_lt_red;
int gTest_toggle;
int gVertices_used_in_non_edit_paths;
br_material* gMat_lt_turq;
br_material* gMat_lt_grn;
int gAlready_elasticating;
br_material* gMat_dk_red;
int gOppo_paths_shown;
br_material* gMat_dk_turq;
br_material* gMat_lt_gry;
br_material* gMat_md_gry;
br_material* gMat_dk_grn;
int gMellow_opponents;
int gMade_path_filename;
int gProcessing_opponents;
br_material* gMat_dk_gry;
br_material* gMat_dk_yel;
br_material* gMat_lt_yel;
br_material* gMat_md_yel;
br_model* gOppo_path_model;
br_actor* gOppo_path_actor;
tU32 gFrame_period_for_this_munging;
tU32 gTime_stamp_for_this_munging;
float gOpponent_nastyness_frigger;
tS16 gMobile_section;

// Offset: 0
// Size: 137
// EAX: pThe_actor
// EDX: pThe_vector
void PointActorAlongThisBloodyVector(br_actor* pThe_actor, br_vector3* pThe_vector) {
    br_transform trans;
}

// Offset: 140
// Size: 301
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessCurrentObjective(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
}

// Offset: 444
// Size: 190
// EAX: pHow_many_then
tS16 ReallocExtraPathNodes(int pHow_many_then) {
    tPath_node* new_nodes;
    tS16 first_new_node;
}

// Offset: 636
// Size: 188
// EAX: pHow_many_then
tS16 ReallocExtraPathSections(int pHow_many_then) {
    tPath_section* new_sections;
    tS16 first_new_section;
}

// Offset: 824
// Size: 186
// EAX: pFrom
// EDX: pTo
int PointVisibleFromHere(br_vector3* pFrom, br_vector3* pTo) {
    br_vector3 from;
    br_vector3 dir;
    br_vector3 norm;
    br_scalar t;
    br_material* material;
}

// Offset: 1012
// Size: 276
// EAX: pActor_coords
// EDX: pDistance
tS16 FindNearestPathNode(br_vector3* pActor_coords, br_scalar* pDistance) {
    int i;
    tS16 nearest_node;
    br_scalar distance;
    br_vector3 actor_to_node;
}

// Offset: 1288
// Size: 66
// EAX: pActor_coords
// EDX: pPath_direction
// EBX: pIntersect
// ECX: pDistance
tS16 FindNearestPathSection(br_vector3* pActor_coords, br_vector3* pPath_direction, br_vector3* pIntersect, br_scalar* pDistance) {
}

// Offset: 1356
// Size: 1187
// EAX: pPursuee
// EDX: pActor_coords
// EBX: pPath_direction
// ECX: pIntersect
tS16 FindNearestGeneralSection(tCar_spec* pPursuee, br_vector3* pActor_coords, br_vector3* pPath_direction, br_vector3* pIntersect, br_scalar* pDistance) {
    int section_no;
    int no_sections;
    tS16 nearest_node_section_no;
    tS16 nearest_section;
    br_scalar nearest_node_distance_squared;
    br_scalar closest_distance_squared;
    br_scalar the_distance_squared;
    br_scalar t;
    br_scalar length_squared_a;
    br_vector3 a;
    br_vector3 p;
    br_vector3 wank;
    br_vector3 intersect;
    br_vector3* start;
    br_vector3* finish;
    br_vector3* nearest_node_v;
}

// Offset: 2544
// Size: 171
// EAX: pCar_spec
void DeadStopCar(tCar_spec* pCar_spec) {
}

// Offset: 2716
// Size: 69
// EAX: pOpponent_spec
void TurnOpponentPhysicsOn(tOpponent_spec* pOpponent_spec) {
}

// Offset: 2788
// Size: 80
// EAX: pOpponent_spec
void TurnOpponentPhysicsOff(tOpponent_spec* pOpponent_spec) {
}

// Offset: 2868
// Size: 247
void NewObjective(tOpponent_spec* pOpponent_spec, tOpponent_objective_type pObjective_type, ...) {
    va_list marker;
}

// Offset: 3116
// Size: 929
// EAX: pOpponent_spec
void CalcRaceRoute(tOpponent_spec* pOpponent_spec) {
    tS16 section_no;
    tS16 section_no_index;
    tS16 node_no;
    tS16 race_section_count;
    tS16 normal_section_ok_direction_count;
    tS16 normal_section_wrong_direction_count;
    tS16 temp_section_array[8];
    br_scalar distance;
    br_vector3 direction_v;
    br_vector3 intersect;
    char str[256];
    char work_str[32];
    int i;
}

// Offset: 4048
// Size: 790
// EAX: pOpponent_spec
// EDX: pSections_to_add
void TopUpRandomRoute(tOpponent_spec* pOpponent_spec, int pSections_to_add) {
    tS16 section_no;
    tS16 node_no;
    tS16 temp_section_array[8];
    int i;
    int target;
    int num_of_temp_sections;
    int direction;
}

// Offset: 4840
// Size: 942
// EAX: pTemp_store
// EDX: pPerm_store
// EBX: pNum_of_perm_store_sections
// ECX: pTarget_section
int SearchForSection(tRoute_section* pTemp_store, tRoute_section* pPerm_store, int* pNum_of_perm_store_sections, tS16 pTarget_section, int pDepth, br_scalar pDistance_so_far, tOpponent_spec* pOpponent_spec) {
    static br_scalar shortest_dist;
    static int routes_found;
    char depth_indent[32];
    int direction;
    tPath_node* node_ptr;
    tS16 node_no;
    tS16 section_no;
    tS16 section_no_index;
    br_scalar distance_so_far;
}

// Offset: 5784
// Size: 796
// EAX: pOpponent_spec
// EDX: pPlayer
void CalcGetNearPlayerRoute(tOpponent_spec* pOpponent_spec, tCar_spec* pPlayer) {
    int i;
    int pass_2_depth;
    int sections_away;
    int num_of_perm_store_sections;
    int sections_to_copy;
    int fuck_it;
    tS16 section_no;
    tS16 players_section;
    br_vector3 section_v;
    br_vector3 intersect;
    br_scalar distance;
    tRoute_section temp_store[10];
    tRoute_section perm_store[10];
    char work_str[32];
    char str[256];
}

// Offset: 6580
// Size: 575
// EAX: pOpponent_spec
void CalcReturnToStartPointRoute(tOpponent_spec* pOpponent_spec) {
    int i;
    int pass_2_depth;
    int sections_away;
    int num_of_perm_store_sections;
    int sections_to_copy;
    tS16 section_no;
    br_vector3 intersect;
    br_vector3 section_v;
    br_scalar distance;
    tRoute_section temp_store[10];
    tRoute_section perm_store[10];
}

// Offset: 7156
// Size: 47
// EAX: pOpponent_spec
void ClearOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec) {
}

// Offset: 7204
// Size: 114
// EAX: pOpponent_spec
// EDX: pSection_no
// EBX: pDirection
int AddToOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec, tS16 pSection_no, int pDirection) {
}

// Offset: 7320
// Size: 177
// EAX: pOpponent_spec
// EDX: pPlaces
int ShiftOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec, int pPlaces) {
    int i;
}

// Offset: 7500
// Size: 134
// EAX: pOpponent_spec
// EDX: pMilliseconds
void StunTheBugger(tOpponent_spec* pOpponent_spec, int pMilliseconds) {
}

// Offset: 7636
// Size: 47
// EAX: pOpponent_spec
void UnStunTheBugger(tOpponent_spec* pOpponent_spec) {
}

// Offset: 7684
// Size: 436
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessCompleteRace(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    br_vector3* initial_pos;
    br_actor* car_actor;
    tComplete_race_data* data;
    int res;
    char str[256];
}

// Offset: 8120
// Size: 337
// EAX: pPursuee
void StartRecordingTrail(tCar_spec* pPursuee) {
    int i;
}

// Offset: 8460
// Size: 1039
// EAX: pPursuee
void RecordNextTrailNode(tCar_spec* pPursuee) {
    tPursuee_trail* trail;
    br_vector3 start1;
    br_vector3 finish1;
    br_vector3 start2;
    br_vector3 finish2;
    br_vector3 offset_v;
    br_vector3 car_to_last_point_v;
    br_scalar length;
    int visible;
}

// Offset: 9500
// Size: 78
// EAX: pOpponent_spec
// EDX: pPursuee
// EBX: pSection_v
// ECX: pIntersect
tS16 FindNearestTrailSection(tOpponent_spec* pOpponent_spec, tCar_spec* pPursuee, br_vector3* pSection_v, br_vector3* pIntersect, br_scalar* pDistance) {
}

// Offset: 9580
// Size: 111
// EAX: pOpponent_spec
// EDX: pSection
tS16 CalcNextTrailSection(tOpponent_spec* pOpponent_spec, int pSection) {
    int section_no;
    tPursuee_trail* trail;
}

// Offset: 9692
// Size: 2515
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessPursueAndTwat(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    tPursue_car_data* data;
    br_vector3 wank;
    br_vector3 section_v;
    br_vector3 intersect;
    br_scalar d;
    br_scalar s;
    br_scalar t;
    br_scalar distance;
    tFollow_path_result res;
    char str[256];
    tS16 section_no;
}

// Offset: 12208
// Size: 638
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessRunAway(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    int res;
    tS16 section_no;
    br_scalar distance;
    br_vector3 intersect;
    br_vector3 direction_v;
    char str[256];
}

// Offset: 12848
// Size: 126
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessWaitForSomeHaplessSod(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
}

// Offset: 12976
// Size: 629
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessReturnToStart(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    br_vector3 section_v;
    br_vector3 our_pos_xz;
    br_vector3 cop_to_start;
    br_scalar distance;
    int res;
}

// Offset: 13608
// Size: 921
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessLevitate(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    float t;
    float terminal_time;
    float y;
}

// Offset: 14532
// Size: 543
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessGetNearPlayer(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    br_vector3* initial_pos;
    br_actor* car_actor;
    int res;
    char str[256];
}

// Offset: 15076
// Size: 230
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessFrozen(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
}

// Offset: 15308
// Size: 355
// EAX: pOpponent_spec
int HeadOnWithPlayerPossible(tOpponent_spec* pOpponent_spec) {
    br_vector3 oppo_to_player_norm;
}

// Offset: 15664
// Size: 88
// EAX: pOpponent_spec
// EDX: pPursuee
int AlreadyPursuingCar(tOpponent_spec* pOpponent_spec, tCar_spec* pPursuee) {
}

// Offset: 15752
// Size: 97
// EAX: pOpponent_spec
int LastTwatteeAPlayer(tOpponent_spec* pOpponent_spec) {
}

// Offset: 15852
// Size: 97
// EAX: pOpponent_spec
int LastTwatterAPlayer(tOpponent_spec* pOpponent_spec) {
}

// Offset: 15952
// Size: 130
// EAX: pOpponent_spec
void ObjectiveComplete(tOpponent_spec* pOpponent_spec) {
}

// Offset: 16084
// Size: 542
// EAX: pOpponent_spec
void TeleportOpponentToNearestSafeLocation(tOpponent_spec* pOpponent_spec) {
    tS16 section_no;
    tU8 section_direction;
    br_scalar distance;
    br_vector3 direction_v;
    br_vector3 intersect;
    int section_counter;
    int found_safe_place;
}

// Offset: 16628
// Size: 4057
// EAX: pOpponent_spec
// EDX: pMust_choose_one
void ChooseNewObjective(tOpponent_spec* pOpponent_spec, int pMust_choose_one) {
    char str[255];
    tS16 players_section;
    br_vector3 wank;
    br_vector3 player_to_oppo_v;
    br_vector3 section_v;
    br_vector3 intersect;
    br_scalar dot;
    br_scalar distance;
    int do_it;
    int i;
    int j;
    int pursuit_percentage;
    int percentage;
    int general_grudge_increase;
}

// Offset: 20688
// Size: 365
// EAX: pOpponent_spec
void ProcessThisOpponent(tOpponent_spec* pOpponent_spec) {
    int i;
}

// Offset: 21056
// Size: 256
// EAX: pPoint
int IsNetCarActive(br_vector3* pPoint) {
    br_vector3 tv;
}

// Offset: 21312
// Size: 692
void RebuildActiveCarList() {
    tCar_spec* car_spec;
}

// Offset: 22004
// Size: 58
void ForceRebuildActiveCarList() {
}

// Offset: 22064
// Size: 138
// EAX: pOpponent_spec
void StartToCheat(tOpponent_spec* pOpponent_spec) {
}

// Offset: 22204
// Size: 138
// EAX: pOpponent_spec
void OiStopCheating(tOpponent_spec* pOpponent_spec) {
}

// Offset: 22344
// Size: 326
// EAX: pOpponent_spec
int TeleportCopToStart(tOpponent_spec* pOpponent_spec) {
    br_vector3 wank;
}

// Offset: 22672
// Size: 139
// EAX: pOpponent_spec
void CalcDistanceFromHome(tOpponent_spec* pOpponent_spec) {
    br_vector3 wank;
}

// Offset: 22812
// Size: 767
// EAX: pOpponent_spec
// EDX: pMassage_count
int MassageOpponentPosition(tOpponent_spec* pOpponent_spec, int pMassage_count) {
    br_matrix34* mat;
    br_vector3* car_trans;
    br_vector3 displacement;
    br_vector3 positive_y_vector;
    br_vector3 direction_v;
}

// Offset: 23580
// Size: 821
// EAX: pOpponent_spec
int RematerialiseOpponentOnThisSection(tOpponent_spec* pOpponent_spec, br_scalar pSpeed, tS16 pSection_no) {
    br_vector3* start;
    br_vector3* finish;
    br_vector3 a;
    br_vector3 p;
    br_vector3 section_v;
    br_vector3 car_to_end;
    br_vector3 intersect;
    br_scalar t;
    br_scalar distance_to_end;
    br_scalar length;
}

// Offset: 24404
// Size: 482
// EAX: pOpponent_spec
int RematerialiseOpponentOnNearestSection(tOpponent_spec* pOpponent_spec, br_scalar pSpeed) {
    br_vector3 intersect;
    br_vector3 direction_v;
    br_vector3 car_to_end;
    br_vector3* start;
    br_vector3* finish;
    br_scalar distance;
    br_scalar distance_to_end;
    tS16 section_no;
}

// Offset: 24888
// Size: 2365
// EAX: pOpponent_spec
int RematerialiseOpponent(tOpponent_spec* pOpponent_spec, br_scalar pSpeed) {
    static int count;
    static int total;
    static int highest;
    int this_total;
    br_matrix34* mat;
    br_matrix34 original_mat;
    br_vector3 a;
    br_vector3 b;
    br_vector3 norm;
    br_vector3 norm2;
    br_scalar dist;
    br_scalar dist2;
    br_scalar ts;
    br_angle phi;
    int i;
    int j;
    int massage_count;
    br_angle theta;
    int sensible_place;
}

// Offset: 27256
// Size: 434
// EAX: pOpponent_spec
void CalcPlayerConspicuousness(tOpponent_spec* pOpponent_spec) {
    br_vector3 pos_in_cop_space;
    br_matrix34 inverse_transform;
}

// Offset: 27692
// Size: 192
// EAX: pOpponent_spec
void CalcOpponentConspicuousnessWithAViewToCheatingLikeFuck(tOpponent_spec* pOpponent_spec) {
}

// Offset: 27884
// Size: 52
// EAX: pChallenger_index
// EDX: pAccepted
void ChallengeOccurred(int pChallenger_index, int pAccepted) {
}

// Offset: 27936
// Size: 232
void LoadCopCars() {
}

// Offset: 28168
// Size: 2460
// EAX: pF
void LoadInOppoPaths(FILE* pF) {
    char s[256];
    char* res;
    int data_errors;
    int section_no;
    int node_no;
    int i;
    br_scalar x;
    br_scalar y;
    br_scalar z;
    br_scalar scalars[8];
    br_scalar distance;
    tPath_node* node_ptr;
    br_vector3 section_v;
    br_vector3 positive_y_vector;
    br_vector3 intersect;
    br_vector3 cop_to_section;
    int j;
    int sections_to_delete;
    int delete_these[1024];
}

// Offset: 30628
// Size: 153
void DisposeOpponentPaths() {
}

// Offset: 30784
// Size: 950
// EAX: pFrame_period
void MungeOpponents(tU32 pFrame_period) {
    int i;
    int un_stun_flag;
}

// Offset: 31736
// Size: 251
void SetInitialCopPositions() {
}

// Offset: 31988
// Size: 2545
// EAX: pRace_info
void InitOpponents(tRace_info* pRace_info) {
    int i;
    int opponent_number;
    int rank_dependent_difficulty;
    int skill_dependent_difficulty;
    br_bounds bounds;
}

// Offset: 34536
// Size: 142
void DisposeOpponents() {
}

// Offset: 34680
// Size: 311
// EAX: pWhat_the_countdown_was
void WakeUpOpponentsToTheFactThatTheStartHasBeenJumped(int pWhat_the_countdown_was) {
    int i;
}

// Offset: 34992
// Size: 91
// EAX: pCar_spec
void ReportMurderToPoliceDepartment(tCar_spec* pCar_spec) {
    int i;
}

// Offset: 35084
// Size: 178
// EAX: pCategory
int GetCarCount(tVehicle_type pCategory) {
}

// Offset: 35264
// Size: 238
// EAX: pCategory
// EDX: pIndex
tCar_spec* GetCarSpec(tVehicle_type pCategory, int pIndex) {
}

// Offset: 35504
// Size: 172
// EAX: pCategory
// EDX: pIndex
char* GetDriverName(tVehicle_type pCategory, int pIndex) {
}

// Offset: 35676
// Size: 240
// EAX: pCar_spec
tOpponent_spec* GetOpponentSpecFromCarSpec(tCar_spec* pCar_spec) {
    int i;
}

// Offset: 35916
// Size: 113
// EAX: pIndex
tCar_spec* GetCarSpecFromGlobalOppoIndex(int pIndex) {
    int i;
}

// Offset: 36032
// Size: 95
// EAX: pOpponent_spec
// EDX: pSection_no
int GetOpponentsRealSection(tOpponent_spec* pOpponent_spec, int pSection_no) {
}

// Offset: 36128
// Size: 106
// EAX: pOpponent_spec
int GetOpponentsFirstSection(tOpponent_spec* pOpponent_spec) {
}

// Offset: 36236
// Size: 185
// EAX: pOpponent_spec
// EDX: pCurrent_section
int GetOpponentsNextSection(tOpponent_spec* pOpponent_spec, tS16 pCurrent_section) {
}

// Offset: 36424
// Size: 239
// EAX: pOpponent_spec
// EDX: pSection
tS16 GetOpponentsSectionStartNode(tOpponent_spec* pOpponent_spec, tS16 pSection) {
    tS16 section_no;
    int node_index_index;
}

// Offset: 36664
// Size: 185
// EAX: pOpponent_spec
// EDX: pSection
tS16 GetOpponentsSectionFinishNode(tOpponent_spec* pOpponent_spec, tS16 pSection) {
    tS16 section_no;
    int node_index_index;
}

// Offset: 36852
// Size: 284
// EAX: pOpponent_spec
// EDX: pSection
br_vector3* GetOpponentsSectionStartNodePoint(tOpponent_spec* pOpponent_spec, tS16 pSection) {
    tS16 section_no;
    tS16 node_no;
    int node_index_index;
}

// Offset: 37136
// Size: 266
// EAX: pOpponent_spec
// EDX: pSection
br_vector3* GetOpponentsSectionFinishNodePoint(tOpponent_spec* pOpponent_spec, tS16 pSection) {
    tS16 section_no;
    tS16 node_no;
    int node_index_index;
}

// Offset: 37404
// Size: 165
// EAX: pOpponent_spec
// EDX: pSection
br_scalar GetOpponentsSectionWidth(tOpponent_spec* pOpponent_spec, tS16 pSection) {
}

// Offset: 37572
// Size: 262
// EAX: pOpponent_spec
// EDX: pSection
// EBX: pTowards_finish
int GetOpponentsSectionMinSpeed(tOpponent_spec* pOpponent_spec, tS16 pSection, int pTowards_finish) {
    tS16 section_no;
    int direction;
}

// Offset: 37836
// Size: 262
// EAX: pOpponent_spec
// EDX: pSection
// EBX: pTowards_finish
int GetOpponentsSectionMaxSpeed(tOpponent_spec* pOpponent_spec, tS16 pSection, int pTowards_finish) {
    tS16 section_no;
    int direction;
}

// Offset: 38100
// Size: 59
// EAX: pOpponent_index
void InitOpponentPsyche(int pOpponent_index) {
    int i;
}

// Offset: 38160
// Size: 108
// EAX: pOpponent_spec
void ClearTwattageOccurrenceVariables(tOpponent_spec* pOpponent_spec) {
}

// Offset: 38268
// Size: 86
// EAX: pA_car
// EDX: pAnother_car
void TwoCarsHitEachOther(tCar_spec* pA_car, tCar_spec* pAnother_car) {
}

// Offset: 38356
// Size: 1166
// EAX: pTwatter
// EDX: pTwattee
void RecordOpponentTwattageOccurrence(tCar_spec* pTwatter, tCar_spec* pTwattee) {
    int bangness;
    int twatter_index;
    int twattee_index;
    int grudginess_caused_by_damage;
    int new_grudge_value;
    float damage;
    char str[256];
    tOpponent_spec* twattee_opponent_spec;
    tOpponent_spec* twatter_opponent_spec;
}

// Offset: 39524
// Size: 67
void ToggleOpponentTest() {
}

// Offset: 39592
// Size: 342
void ToggleOpponentProcessing() {
}

// Offset: 39936
// Size: 183
void ToggleMellowOpponents() {
}

// Offset: 40120
// Size: 184
void RepairOpponentsSystems() {
}

// Offset: 40564
// Size: 530
// EAX: pSection_to_delete
void DeleteSection(tS16 pSection_to_delete) {
    tS16 section_no;
    tS16 section_no_index;
    tS16 node_no;
    tS16 node_no_index;
    tS16 found_it;
}

// Offset: 41096
// Size: 824
// EAX: pNode_to_delete
// EDX: pAnd_sections
void DeleteNode(tS16 pNode_to_delete, int pAnd_sections) {
    tS16 node_no;
    tS16 section_no;
    tS16 section1;
    tS16 section2;
}

// Offset: 41920
// Size: 116
void DeleteOrphanNodes() {
}

// Offset: 42036
// Size: 716
// EAX: pInserted_node
// EDX: pSection_no
// EBX: pWhere
void InsertThisNodeInThisSectionHere(tS16 pInserted_node, tS16 pSection_no, br_vector3* pWhere) {
    tS16 new_section;
    tS16 section_no_index;
    tS16 node1;
    tS16 node2;
    tS16 node3;
}

// Offset: 42752
// Size: 254
void TrackElasticateyPath() {
}

// Offset: 43008
// Size: 1043
// EAX: pMax_not_min
// EDX: pAdjustment
void RecalcNearestPathSectionSpeed(int pMax_not_min, int pAdjustment) {
    tS16 section_no;
    br_vector3 direction_v;
    br_vector3 intersect;
    br_vector3 wank;
    br_scalar distance;
    br_scalar dist_to_start;
    br_scalar dist_to_finish;
    char str[128];
    int new_speed;
    int nearest_end;
}

// Offset: 44052
// Size: 320
void RecalcNearestPathSectionWidth(br_scalar pAdjustment) {
    tS16 section_no;
    br_vector3 direction_v;
    br_vector3 intersect;
    br_scalar distance;
    char str[128];
}

// Offset: 44372
// Size: 325
// EAX: pNegative_x_vector
// EDX: pStart
// EBX: pFinish
void CalcNegativeXVector(br_vector3* pNegative_x_vector, br_vector3* pStart, br_vector3* pFinish, br_scalar pLength) {
    br_vector3 positive_y_vector;
    br_vector3 path_vector;
}

// Offset: 44700
// Size: 162
// EAX: pModel
// EDX: pVertex_num
void MakeVertexAndOffsetIt(br_model* pModel, int pVertex_num, br_scalar pX, br_scalar pY, br_scalar pZ, br_vector3* pOffset) {
}

// Offset: 44864
// Size: 134
// EAX: pModel
// EDX: pFace_num
// EBX: pV0
// ECX: pV1
void MakeFaceAndTextureIt(br_model* pModel, int pFace_num, int pV0, int pV1, int pV2, br_material* pMaterial) {
}

// Offset: 45000
// Size: 942
// EAX: pFirst_vertex
// EDX: pFirst_face
// EBX: pStart
// ECX: pFinish
void MakeSection(br_uint_16 pFirst_vertex, br_uint_16 pFirst_face, br_vector3* pStart, br_vector3* pFinish, br_scalar pWidth, br_material* pMaterial_centre_lt, br_material* pMaterial_centre_dk, br_material* pMaterial_edges_start_lt, br_material* pMaterial_edges_start_dk, br_material* pMaterial_edges_finish_lt, br_material* pMaterial_edges_finish_dk) {
    int i;
    br_vector3 offset_v;
    br_vector3 centre_length_v;
    br_material* the_material_start_lt;
    br_material* the_material_start_dk;
    br_material* the_material_finish_lt;
    br_material* the_material_finish_dk;
    br_scalar height;
}

// Offset: 45944
// Size: 1235
// EAX: pFirst_vertex
// EDX: pFirst_face
// EBX: pPoint
// ECX: pMaterial_1
void MakeCube(br_uint_16 pFirst_vertex, br_uint_16 pFirst_face, br_vector3* pPoint, br_material* pMaterial_1, br_material* pMaterial_2, br_material* pMaterial_3) {
    br_vector3 offset_v;
    br_vector3 point;
}

// Offset: 47180
// Size: 88
// EAX: pFace_index_ptr
// EDX: pVertex_index_ptr
void CalcNumberOfFacesAndVerticesForOppoPathModel(br_uint_16* pFace_index_ptr, br_uint_16* pVertex_index_ptr) {
}

// Offset: 47268
// Size: 433
// EAX: pModel
// EDX: pNum_faces
// EBX: pNum_vertices
void ReallocModelFacesAndVertices(br_model* pModel, int pNum_faces, int pNum_vertices) {
    br_vertex* new_vertices;
    br_face* new_faces;
    int i;
}

// Offset: 47704
// Size: 127
// EAX: pColour_index
br_material* CreateSimpleMaterial(int pColour_index) {
    br_material* return_me;
}

// Offset: 47832
// Size: 254
void AllocateMatsForOppoPathModel() {
}

// Offset: 48088
// Size: 1093
void RebuildOppoPathModel() {
    static int nfaces_last_time;
    int i;
    int at_least_one;
    br_uint_16 nfaces;
    br_uint_16 nvertices;
    br_uint_16 first_face;
    br_uint_16 first_vertex;
    br_material* centre_mat_lt;
    br_material* centre_mat_dk;
    br_material* edge_mat_start_lt;
    br_material* edge_mat_start_dk;
    br_material* edge_mat_finish_lt;
    br_material* edge_mat_finish_dk;
}

// Offset: 49184
// Size: 1319
int ConsistencyCheck() {
    tS16 section_no;
    tS16 start_node;
    tS16 finish_node;
    tS16 section_no_index;
    tS16 section_no_index1;
    int found_how_many;
    int failed;
    tU8* nodes_referenced_by_sections_array;
    tU8* sections_referenced_by_nodes_array;
}

// Offset: 50504
// Size: 172
void ShowOppoPaths() {
}

// Offset: 50676
// Size: 832
void WriteOutOppoPaths() {
    char str[13];
    FILE* f;
    int i;
}

// Offset: 51508
// Size: 338
int NewNodeOKHere() {
}

// Offset: 51848
// Size: 81
void ShowHideOppoPaths() {
}

// Offset: 51932
// Size: 1148
void DropElasticateyNode() {
    tS16 old_node;
    tS16 new_node;
    tS16 section_no_index;
    br_scalar distance;
    int all_the_same_type;
    int one_wayness;
    tPath_section_type_enum section_type;
    tPath_section_type_enum original_type;
}

// Offset: 53080
// Size: 1216
void InsertAndElasticate() {
    tS16 elasticatey_node;
    tS16 section_no;
    tS16 new_section;
    br_vector3 direction_v;
    br_vector3 intersect;
    br_vector3 wank;
    br_scalar distance;
    int not_perp;
    int one_wayness;
    char str[256];
    tPath_section_type_enum section_type;
}

// Offset: 54296
// Size: 623
void InsertAndDontElasticate() {
    tS16 section_no;
    br_vector3 direction_v;
    br_vector3 intersect;
    br_vector3 wank;
    br_scalar distance;
    int not_perp;
    char str[256];
}

// Offset: 54920
// Size: 300
void DropDeadEndNode() {
}

// Offset: 55220
// Size: 404
void DropNodeOnNodeAndStopElasticating() {
    char str[256];
    br_scalar distance;
}

// Offset: 55624
// Size: 53
void WidenOppoPathSection() {
}

// Offset: 55680
// Size: 53
void NarrowOppoPathSection() {
}

// Offset: 55736
// Size: 55
void IncreaseSectionMinSpeed() {
}

// Offset: 55792
// Size: 55
void DecreaseSectionMinSpeed() {
}

// Offset: 55848
// Size: 58
void IncreaseSectionMaxSpeed() {
}

// Offset: 55908
// Size: 58
void DecreaseSectionMaxSpeed() {
}

// Offset: 55968
// Size: 260
void PullOppoPoint() {
    br_scalar distance;
}

// Offset: 56228
// Size: 301
void ShowNodeInfo() {
    char str[256];
    br_scalar distance;
}

// Offset: 56532
// Size: 364
void ShowSectionInfo1() {
    char str[256];
    br_scalar distance;
    br_vector3 direction_v;
    br_vector3 intersect;
}

// Offset: 56896
// Size: 717
void ShowSectionInfo2() {
    char str[256];
    br_scalar distance;
    br_vector3 direction_v;
    br_vector3 intersect;
}

// Offset: 57616
// Size: 226
void DeleteOppoPathSection() {
    br_vector3 intersect;
    br_vector3 direction_v;
    tS16 section_no;
}

// Offset: 57844
// Size: 225
void DeleteOppoPathNodeAndSections() {
    tS16 node_no;
}

// Offset: 58072
// Size: 535
void DeleteOppoPathNodeAndJoin() {
    tS16 node_no;
}

// Offset: 58608
// Size: 424
void ReverseSectionDirection() {
    tU8 speed_temp;
    br_scalar distance;
    br_vector3 intersect;
    br_vector3 direction_v;
    tS16 section_no;
}

// Offset: 59032
// Size: 325
void CycleSectionType() {
    br_vector3 intersect;
    br_vector3 direction_v;
    tS16 section_no;
    char str[256];
}

// Offset: 59360
// Size: 321
void ToggleOneWayNess() {
    br_vector3 intersect;
    br_vector3 direction_v;
    tS16 section_no;
}

// Offset: 59684
// Size: 350
void CopStartPointInfo() {
    int i;
    int closest;
    br_scalar closest_distance;
    br_scalar distance;
    br_vector3 car_to_point;
}

// Offset: 60036
// Size: 318
void DropCopStartPoint() {
}

// Offset: 60356
// Size: 504
void DeleteCopStartPoint() {
    int i;
    int closest;
    br_scalar closest_distance;
    br_scalar distance;
    br_vector3 car_to_point;
}

// Offset: 60860
// Size: 34
void CycleCopStartPointType() {
}
