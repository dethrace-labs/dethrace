#include "racesumm.h"

int gPlayer_lookup[6];
tMouse_area gOld_back_button;
tWreck_info gWreck_array[30];
br_actor* gWreck_root;
br_actor* gWreck_camera;
tU32 gWreck_start_zoom;
tU32 gWreck_gallery_start;
float gTemp_rank_increase;
float gRank_per_ms;
tU32 gLast_wreck_draw;
tS3_sound_tag gSumm_sound;
float gCredits_per_ms;
tMouse_area* gBack_button_ptr;
tU32 gSummary_start;
br_pixelmap* gWreck_z_buffer;
br_pixelmap* gWreck_render_area;
int gWreck_selected;
int gWreck_zoom_out;
br_pixelmap* gChrome_font;
int gWreck_zoom_in;
int gTemp_credits;
int gUser_interacted;
int gWreck_count;
int gRank_etc_munged;
int gRank_increase;
int gTemp_earned;
int gTemp_rank;
int gWreck_zoomed_in;
int gDone_initial;
int gTemp_lost;

// Offset: 0
// Size: 302
// EAX: pThe_state
void MungeRankEtc(tProgram_state* pThe_state) {
    int i;
    int not_done_yet;
}

// Offset: 304
// Size: 96
void CalcRankIncrease() {
}

// Offset: 400
// Size: 81
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int RaceSummaryDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
}

// Offset: 484
// Size: 166
// EAX: pBox_left
// EDX: pText_left
// EBX: pTop
// ECX: pRight
void DrawInBox(int pBox_left, int pText_left, int pTop, int pRight, int pBottom, int pColour, int pAmount) {
}

// Offset: 652
// Size: 220
// EAX: pLeft_1
// EDX: pLeft_2
// EBX: pPitch
// ECX: pTop
void DrawChromeNumber(int pLeft_1, int pLeft_2, int pPitch, int pTop, int pAmount) {
}

// Offset: 872
// Size: 604
void DrawSummaryItems() {
}

// Offset: 1476
// Size: 127
// EAX: pRate
// EDX: pTime
void RampUpRate(float* pRate, tU32 pTime) {
}

// Offset: 1604
// Size: 747
// EAX: pCurrent_choice
// EDX: pCurrent_mode
void DrawSummary(int pCurrent_choice, int pCurrent_mode) {
    tU32 the_time;
    static tU32 last_time;
    static tU32 last_change_time;
    int credit_delta;
    float old_temp_increase;
    float rank_delta;
}

// Offset: 2352
// Size: 49
void StartSummary() {
}

// Offset: 2404
// Size: 86
void SetUpTemps() {
}

// Offset: 2492
// Size: 78
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int Summ1GoAhead(int* pCurrent_choice, int* pCurrent_mode) {
}

// Offset: 2572
// Size: 324
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int SummCheckGameOver(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    tS3_sound_tag sound_tag;
}

// Offset: 2896
// Size: 288
tSO_result DoEndRaceSummary1() {
    static tFlicette flicker_off[1];
    static tFlicette push[1];
    static tMouse_area mouse_areas[1];
    static tInterface_spec interface_spec;
    int result;
    int completed_already;
}

// Offset: 3352
// Size: 1003
void BuildWrecks() {
    int i;
    int position;
    int car_count;
    br_actor* this_car;
    tCar_spec* the_car;
}

// Offset: 4356
// Size: 385
void DisposeWrecks() {
    int i;
    int position;
    int car_count;
    br_actor* this_car;
    tCar_spec* the_car;
}

// Offset: 4744
// Size: 185
// EAX: pMat
int MatrixIsIdentity(br_matrix34* pMat) {
}

// Offset: 4932
// Size: 403
// EAX: pFrame_period
void SpinWrecks(tU32 pFrame_period) {
    int i;
    br_vector3 translation;
    br_matrix34 old_mat;
}

// Offset: 5336
// Size: 143
// EAX: pIndex
// EDX: pCurrent_choice
// EBX: pCurrent_mode
void ZoomInTo(int pIndex, int* pCurrent_choice, int* pCurrent_mode) {
}

// Offset: 5480
// Size: 189
// EAX: pIndex
// EDX: pCurrent_choice
// EBX: pCurrent_mode
void ZoomOutTo(int pIndex, int* pCurrent_choice, int* pCurrent_mode) {
}

// Offset: 5672
// Size: 102
int WreckPick(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pNear, br_scalar pFar, void* pArg) {
    int i;
}

// Offset: 5776
// Size: 404
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int CastSelectionRay(int* pCurrent_choice, int* pCurrent_mode) {
    int mouse_x;
    int mouse_y;
    int i;
    int result;
    br_scalar inv_wreck_pick_scale_factor;
}

// Offset: 6180
// Size: 167
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int DamageScrnExit(int* pCurrent_choice, int* pCurrent_mode) {
}

// Offset: 6348
// Size: 2481
// EAX: pCurrent_choice
// EDX: pCurrent_mode
void DamageScrnDraw(int pCurrent_choice, int pCurrent_mode) {
    tU32 the_time;
    br_vector3 camera_movement;
    int finished;
    int h;
    int v;
    int rows;
    int columns;
    float spacing;
    br_actor* sel_actor;
    char* name;
}

// Offset: 8832
// Size: 268
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int DamageScrnLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
}

// Offset: 9100
// Size: 275
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int DamageScrnRight(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
}

// Offset: 9376
// Size: 428
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int DamageScrnUp(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    int difference;
    int new_difference;
    int new_selection;
}

// Offset: 9804
// Size: 470
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int DamageScrnDown(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    int difference;
    int new_difference;
    int new_selection;
}

// Offset: 10276
// Size: 176
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int DamageScrnGoHead(int* pCurrent_choice, int* pCurrent_mode) {
}

// Offset: 10452
// Size: 258
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int ClickDamage(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int mouse_x;
    int mouse_y;
    int old_mouse_x;
    int old_mouse_y;
}

// Offset: 10712
// Size: 66
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int DamageScrnDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
}

// Offset: 10780
// Size: 286
tSO_result DoEndRaceSummary2() {
    static tFlicette flicker_off[3];
    static tFlicette push[3];
    static tMouse_area mouse_areas[3];
    static tInterface_spec interface_spec;
    int result;
}

// Offset: 11256
// Size: 74
// EAX: pFirst_one
// EDX: pSecond_one
int SortScores(void* pFirst_one, void* pSecond_one) {
}

// Offset: 11332
// Size: 60
void SortGameScores() {
}

// Offset: 11392
// Size: 709
// EAX: pCurrent_choice
// EDX: pCurrent_mode
void NetSumDraw(int pCurrent_choice, int pCurrent_mode) {
    int i;
    char s[256];
    tNet_game_player_info* player;
}

// Offset: 12104
// Size: 188
void DoNetRaceSummary() {
    static tFlicette flicker_off[1];
    static tFlicette push[1];
    static tMouse_area mouse_areas[1];
    static tInterface_spec interface_spec;
    int i;
    int result;
    tS32 start_time;
}

// Offset: 12292
// Size: 227
// EAX: pFirst_summary_done
// EDX: pRace_result
tSO_result DoEndRaceSummary(int* pFirst_summary_done, tRace_result pRace_result) {
    tSO_result result;
}
