#include "opponent.h"
#include "common/globvars.h"
#include <stdlib.h>

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

void PointActorAlongThisBloodyVector(br_actor* pThe_actor, br_vector3* pThe_vector) {
    br_transform trans;
    NOT_IMPLEMENTED();
}

void ProcessCurrentObjective(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    NOT_IMPLEMENTED();
}

tS16 ReallocExtraPathNodes(int pHow_many_then) {
    tPath_node* new_nodes;
    tS16 first_new_node;
    NOT_IMPLEMENTED();
}

tS16 ReallocExtraPathSections(int pHow_many_then) {
    tPath_section* new_sections;
    tS16 first_new_section;
    NOT_IMPLEMENTED();
}

int PointVisibleFromHere(br_vector3* pFrom, br_vector3* pTo) {
    br_vector3 from;
    br_vector3 dir;
    br_vector3 norm;
    br_scalar t;
    br_material* material;
    NOT_IMPLEMENTED();
}

tS16 FindNearestPathNode(br_vector3* pActor_coords, br_scalar* pDistance) {
    int i;
    tS16 nearest_node;
    br_scalar distance;
    br_vector3 actor_to_node;
    NOT_IMPLEMENTED();
}

tS16 FindNearestPathSection(br_vector3* pActor_coords, br_vector3* pPath_direction, br_vector3* pIntersect, br_scalar* pDistance) {
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void DeadStopCar(tCar_spec* pCar_spec) {
    NOT_IMPLEMENTED();
}

void TurnOpponentPhysicsOn(tOpponent_spec* pOpponent_spec) {
    NOT_IMPLEMENTED();
}

void TurnOpponentPhysicsOff(tOpponent_spec* pOpponent_spec) {
    NOT_IMPLEMENTED();
}

void NewObjective(tOpponent_spec* pOpponent_spec, tOpponent_objective_type pObjective_type, ...) {
    va_list marker;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void TopUpRandomRoute(tOpponent_spec* pOpponent_spec, int pSections_to_add) {
    tS16 section_no;
    tS16 node_no;
    tS16 temp_section_array[8];
    int i;
    int target;
    int num_of_temp_sections;
    int direction;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void ClearOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec) {
    NOT_IMPLEMENTED();
}

int AddToOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec, tS16 pSection_no, int pDirection) {
    NOT_IMPLEMENTED();
}

int ShiftOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec, int pPlaces) {
    int i;
    NOT_IMPLEMENTED();
}

void StunTheBugger(tOpponent_spec* pOpponent_spec, int pMilliseconds) {
    NOT_IMPLEMENTED();
}

void UnStunTheBugger(tOpponent_spec* pOpponent_spec) {
    NOT_IMPLEMENTED();
}

void ProcessCompleteRace(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    br_vector3* initial_pos;
    br_actor* car_actor;
    tComplete_race_data* data;
    int res;
    char str[256];
    NOT_IMPLEMENTED();
}

void StartRecordingTrail(tCar_spec* pPursuee) {
    int i;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

tS16 FindNearestTrailSection(tOpponent_spec* pOpponent_spec, tCar_spec* pPursuee, br_vector3* pSection_v, br_vector3* pIntersect, br_scalar* pDistance) {
    NOT_IMPLEMENTED();
}

tS16 CalcNextTrailSection(tOpponent_spec* pOpponent_spec, int pSection) {
    int section_no;
    tPursuee_trail* trail;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void ProcessRunAway(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    int res;
    tS16 section_no;
    br_scalar distance;
    br_vector3 intersect;
    br_vector3 direction_v;
    char str[256];
    NOT_IMPLEMENTED();
}

void ProcessWaitForSomeHaplessSod(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    NOT_IMPLEMENTED();
}

void ProcessReturnToStart(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    br_vector3 section_v;
    br_vector3 our_pos_xz;
    br_vector3 cop_to_start;
    br_scalar distance;
    int res;
    NOT_IMPLEMENTED();
}

void ProcessLevitate(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    float t;
    float terminal_time;
    float y;
    NOT_IMPLEMENTED();
}

void ProcessGetNearPlayer(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    br_vector3* initial_pos;
    br_actor* car_actor;
    int res;
    char str[256];
    NOT_IMPLEMENTED();
}

void ProcessFrozen(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand) {
    NOT_IMPLEMENTED();
}

int HeadOnWithPlayerPossible(tOpponent_spec* pOpponent_spec) {
    br_vector3 oppo_to_player_norm;
    NOT_IMPLEMENTED();
}

int AlreadyPursuingCar(tOpponent_spec* pOpponent_spec, tCar_spec* pPursuee) {
    NOT_IMPLEMENTED();
}

int LastTwatteeAPlayer(tOpponent_spec* pOpponent_spec) {
    NOT_IMPLEMENTED();
}

int LastTwatterAPlayer(tOpponent_spec* pOpponent_spec) {
    NOT_IMPLEMENTED();
}

void ObjectiveComplete(tOpponent_spec* pOpponent_spec) {
    NOT_IMPLEMENTED();
}

void TeleportOpponentToNearestSafeLocation(tOpponent_spec* pOpponent_spec) {
    tS16 section_no;
    tU8 section_direction;
    br_scalar distance;
    br_vector3 direction_v;
    br_vector3 intersect;
    int section_counter;
    int found_safe_place;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void ProcessThisOpponent(tOpponent_spec* pOpponent_spec) {
    int i;
    NOT_IMPLEMENTED();
}

int IsNetCarActive(br_vector3* pPoint) {
    br_vector3 tv;
    NOT_IMPLEMENTED();
}

void RebuildActiveCarList() {
    tCar_spec* car_spec;
    NOT_IMPLEMENTED();
}

void ForceRebuildActiveCarList() {
    NOT_IMPLEMENTED();
}

void StartToCheat(tOpponent_spec* pOpponent_spec) {
    NOT_IMPLEMENTED();
}

void OiStopCheating(tOpponent_spec* pOpponent_spec) {
    NOT_IMPLEMENTED();
}

int TeleportCopToStart(tOpponent_spec* pOpponent_spec) {
    br_vector3 wank;
    NOT_IMPLEMENTED();
}

void CalcDistanceFromHome(tOpponent_spec* pOpponent_spec) {
    br_vector3 wank;
    NOT_IMPLEMENTED();
}

int MassageOpponentPosition(tOpponent_spec* pOpponent_spec, int pMassage_count) {
    br_matrix34* mat;
    br_vector3* car_trans;
    br_vector3 displacement;
    br_vector3 positive_y_vector;
    br_vector3 direction_v;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

int RematerialiseOpponentOnNearestSection(tOpponent_spec* pOpponent_spec, br_scalar pSpeed) {
    br_vector3 intersect;
    br_vector3 direction_v;
    br_vector3 car_to_end;
    br_vector3* start;
    br_vector3* finish;
    br_scalar distance;
    br_scalar distance_to_end;
    tS16 section_no;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void CalcPlayerConspicuousness(tOpponent_spec* pOpponent_spec) {
    br_vector3 pos_in_cop_space;
    br_matrix34 inverse_transform;
    NOT_IMPLEMENTED();
}

void CalcOpponentConspicuousnessWithAViewToCheatingLikeFuck(tOpponent_spec* pOpponent_spec) {
    NOT_IMPLEMENTED();
}

void ChallengeOccurred(int pChallenger_index, int pAccepted) {
    NOT_IMPLEMENTED();
}

void LoadCopCars() {
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void DisposeOpponentPaths() {
    NOT_IMPLEMENTED();
}

void MungeOpponents(tU32 pFrame_period) {
    int i;
    int un_stun_flag;
    NOT_IMPLEMENTED();
}

void SetInitialCopPositions() {
    NOT_IMPLEMENTED();
}

void InitOpponents(tRace_info* pRace_info) {
    int i;
    int opponent_number;
    int rank_dependent_difficulty;
    int skill_dependent_difficulty;
    br_bounds bounds;
    NOT_IMPLEMENTED();
}

void DisposeOpponents() {
    NOT_IMPLEMENTED();
}

void WakeUpOpponentsToTheFactThatTheStartHasBeenJumped(int pWhat_the_countdown_was) {
    int i;
    NOT_IMPLEMENTED();
}

void ReportMurderToPoliceDepartment(tCar_spec* pCar_spec) {
    int i;
    NOT_IMPLEMENTED();
}

int GetCarCount(tVehicle_type pCategory) {
    NOT_IMPLEMENTED();
}

tCar_spec* GetCarSpec(tVehicle_type pCategory, int pIndex) {
    NOT_IMPLEMENTED();
}

char* GetDriverName(tVehicle_type pCategory, int pIndex) {
    NOT_IMPLEMENTED();
}

tOpponent_spec* GetOpponentSpecFromCarSpec(tCar_spec* pCar_spec) {
    int i;
    NOT_IMPLEMENTED();
}

tCar_spec* GetCarSpecFromGlobalOppoIndex(int pIndex) {
    int i;
    NOT_IMPLEMENTED();
}

int GetOpponentsRealSection(tOpponent_spec* pOpponent_spec, int pSection_no) {
    NOT_IMPLEMENTED();
}

int GetOpponentsFirstSection(tOpponent_spec* pOpponent_spec) {
    NOT_IMPLEMENTED();
}

int GetOpponentsNextSection(tOpponent_spec* pOpponent_spec, tS16 pCurrent_section) {
    NOT_IMPLEMENTED();
}

tS16 GetOpponentsSectionStartNode(tOpponent_spec* pOpponent_spec, tS16 pSection) {
    tS16 section_no;
    int node_index_index;
    NOT_IMPLEMENTED();
}

tS16 GetOpponentsSectionFinishNode(tOpponent_spec* pOpponent_spec, tS16 pSection) {
    tS16 section_no;
    int node_index_index;
    NOT_IMPLEMENTED();
}

br_vector3* GetOpponentsSectionStartNodePoint(tOpponent_spec* pOpponent_spec, tS16 pSection) {
    tS16 section_no;
    tS16 node_no;
    int node_index_index;
    NOT_IMPLEMENTED();
}

br_vector3* GetOpponentsSectionFinishNodePoint(tOpponent_spec* pOpponent_spec, tS16 pSection) {
    tS16 section_no;
    tS16 node_no;
    int node_index_index;
    NOT_IMPLEMENTED();
}

br_scalar GetOpponentsSectionWidth(tOpponent_spec* pOpponent_spec, tS16 pSection) {
    NOT_IMPLEMENTED();
}

int GetOpponentsSectionMinSpeed(tOpponent_spec* pOpponent_spec, tS16 pSection, int pTowards_finish) {
    tS16 section_no;
    int direction;
    NOT_IMPLEMENTED();
}

int GetOpponentsSectionMaxSpeed(tOpponent_spec* pOpponent_spec, tS16 pSection, int pTowards_finish) {
    tS16 section_no;
    int direction;
    NOT_IMPLEMENTED();
}

void InitOpponentPsyche(int pOpponent_index) {
    gOpponents[pOpponent_index].psyche.grudge_against_player = 0;
}

void ClearTwattageOccurrenceVariables(tOpponent_spec* pOpponent_spec) {
    NOT_IMPLEMENTED();
}

void TwoCarsHitEachOther(tCar_spec* pA_car, tCar_spec* pAnother_car) {
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void ToggleOpponentTest() {
    NOT_IMPLEMENTED();
}

void ToggleOpponentProcessing() {
    NOT_IMPLEMENTED();
}

void ToggleMellowOpponents() {
    NOT_IMPLEMENTED();
}

void RepairOpponentsSystems() {
    NOT_IMPLEMENTED();
}

void DeleteSection(tS16 pSection_to_delete) {
    tS16 section_no;
    tS16 section_no_index;
    tS16 node_no;
    tS16 node_no_index;
    tS16 found_it;
    NOT_IMPLEMENTED();
}

void DeleteNode(tS16 pNode_to_delete, int pAnd_sections) {
    tS16 node_no;
    tS16 section_no;
    tS16 section1;
    tS16 section2;
    NOT_IMPLEMENTED();
}

void DeleteOrphanNodes() {
    NOT_IMPLEMENTED();
}

void InsertThisNodeInThisSectionHere(tS16 pInserted_node, tS16 pSection_no, br_vector3* pWhere) {
    tS16 new_section;
    tS16 section_no_index;
    tS16 node1;
    tS16 node2;
    tS16 node3;
    NOT_IMPLEMENTED();
}

void TrackElasticateyPath() {
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void RecalcNearestPathSectionWidth(br_scalar pAdjustment) {
    tS16 section_no;
    br_vector3 direction_v;
    br_vector3 intersect;
    br_scalar distance;
    char str[128];
    NOT_IMPLEMENTED();
}

void CalcNegativeXVector(br_vector3* pNegative_x_vector, br_vector3* pStart, br_vector3* pFinish, br_scalar pLength) {
    br_vector3 positive_y_vector;
    br_vector3 path_vector;
    NOT_IMPLEMENTED();
}

void MakeVertexAndOffsetIt(br_model* pModel, int pVertex_num, br_scalar pX, br_scalar pY, br_scalar pZ, br_vector3* pOffset) {
    NOT_IMPLEMENTED();
}

void MakeFaceAndTextureIt(br_model* pModel, int pFace_num, int pV0, int pV1, int pV2, br_material* pMaterial) {
    NOT_IMPLEMENTED();
}

void MakeSection(br_uint_16 pFirst_vertex, br_uint_16 pFirst_face, br_vector3* pStart, br_vector3* pFinish, br_scalar pWidth, br_material* pMaterial_centre_lt, br_material* pMaterial_centre_dk, br_material* pMaterial_edges_start_lt, br_material* pMaterial_edges_start_dk, br_material* pMaterial_edges_finish_lt, br_material* pMaterial_edges_finish_dk) {
    int i;
    br_vector3 offset_v;
    br_vector3 centre_length_v;
    br_material* the_material_start_lt;
    br_material* the_material_start_dk;
    br_material* the_material_finish_lt;
    br_material* the_material_finish_dk;
    br_scalar height;
    NOT_IMPLEMENTED();
}

void MakeCube(br_uint_16 pFirst_vertex, br_uint_16 pFirst_face, br_vector3* pPoint, br_material* pMaterial_1, br_material* pMaterial_2, br_material* pMaterial_3) {
    br_vector3 offset_v;
    br_vector3 point;
    NOT_IMPLEMENTED();
}

void CalcNumberOfFacesAndVerticesForOppoPathModel(br_uint_16* pFace_index_ptr, br_uint_16* pVertex_index_ptr) {
    NOT_IMPLEMENTED();
}

void ReallocModelFacesAndVertices(br_model* pModel, int pNum_faces, int pNum_vertices) {
    br_vertex* new_vertices;
    br_face* new_faces;
    int i;
    NOT_IMPLEMENTED();
}

br_material* CreateSimpleMaterial(int pColour_index) {
    br_material* return_me;
    NOT_IMPLEMENTED();
}

void AllocateMatsForOppoPathModel() {
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void ShowOppoPaths() {
    NOT_IMPLEMENTED();
}

void WriteOutOppoPaths() {
    char str[13];
    FILE* f;
    int i;
    NOT_IMPLEMENTED();
}

int NewNodeOKHere() {
    NOT_IMPLEMENTED();
}

void ShowHideOppoPaths() {
    NOT_IMPLEMENTED();
}

void DropElasticateyNode() {
    tS16 old_node;
    tS16 new_node;
    tS16 section_no_index;
    br_scalar distance;
    int all_the_same_type;
    int one_wayness;
    tPath_section_type_enum section_type;
    tPath_section_type_enum original_type;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void InsertAndDontElasticate() {
    tS16 section_no;
    br_vector3 direction_v;
    br_vector3 intersect;
    br_vector3 wank;
    br_scalar distance;
    int not_perp;
    char str[256];
    NOT_IMPLEMENTED();
}

void DropDeadEndNode() {
    NOT_IMPLEMENTED();
}

void DropNodeOnNodeAndStopElasticating() {
    char str[256];
    br_scalar distance;
    NOT_IMPLEMENTED();
}

void WidenOppoPathSection() {
    NOT_IMPLEMENTED();
}

void NarrowOppoPathSection() {
    NOT_IMPLEMENTED();
}

void IncreaseSectionMinSpeed() {
    NOT_IMPLEMENTED();
}

void DecreaseSectionMinSpeed() {
    NOT_IMPLEMENTED();
}

void IncreaseSectionMaxSpeed() {
    NOT_IMPLEMENTED();
}

void DecreaseSectionMaxSpeed() {
    NOT_IMPLEMENTED();
}

void PullOppoPoint() {
    br_scalar distance;
    NOT_IMPLEMENTED();
}

void ShowNodeInfo() {
    char str[256];
    br_scalar distance;
    NOT_IMPLEMENTED();
}

void ShowSectionInfo1() {
    char str[256];
    br_scalar distance;
    br_vector3 direction_v;
    br_vector3 intersect;
    NOT_IMPLEMENTED();
}

void ShowSectionInfo2() {
    char str[256];
    br_scalar distance;
    br_vector3 direction_v;
    br_vector3 intersect;
    NOT_IMPLEMENTED();
}

void DeleteOppoPathSection() {
    br_vector3 intersect;
    br_vector3 direction_v;
    tS16 section_no;
    NOT_IMPLEMENTED();
}

void DeleteOppoPathNodeAndSections() {
    tS16 node_no;
    NOT_IMPLEMENTED();
}

void DeleteOppoPathNodeAndJoin() {
    tS16 node_no;
    NOT_IMPLEMENTED();
}

void ReverseSectionDirection() {
    tU8 speed_temp;
    br_scalar distance;
    br_vector3 intersect;
    br_vector3 direction_v;
    tS16 section_no;
    NOT_IMPLEMENTED();
}

void CycleSectionType() {
    br_vector3 intersect;
    br_vector3 direction_v;
    tS16 section_no;
    char str[256];
    NOT_IMPLEMENTED();
}

void ToggleOneWayNess() {
    br_vector3 intersect;
    br_vector3 direction_v;
    tS16 section_no;
    NOT_IMPLEMENTED();
}

void CopStartPointInfo() {
    int i;
    int closest;
    br_scalar closest_distance;
    br_scalar distance;
    br_vector3 car_to_point;
    NOT_IMPLEMENTED();
}

void DropCopStartPoint() {
    NOT_IMPLEMENTED();
}

void DeleteCopStartPoint() {
    int i;
    int closest;
    br_scalar closest_distance;
    br_scalar distance;
    br_vector3 car_to_point;
    NOT_IMPLEMENTED();
}

void CycleCopStartPointType() {
    NOT_IMPLEMENTED();
}
