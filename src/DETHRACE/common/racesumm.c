#include "racesumm.h"
#include <stdlib.h>

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

void MungeRankEtc(tProgram_state* pThe_state) {
    int i;
    int not_done_yet;
    NOT_IMPLEMENTED();
}

void CalcRankIncrease() {
    NOT_IMPLEMENTED();
}

int RaceSummaryDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

void DrawInBox(int pBox_left, int pText_left, int pTop, int pRight, int pBottom, int pColour, int pAmount) {
    NOT_IMPLEMENTED();
}

void DrawChromeNumber(int pLeft_1, int pLeft_2, int pPitch, int pTop, int pAmount) {
    NOT_IMPLEMENTED();
}

void DrawSummaryItems() {
    NOT_IMPLEMENTED();
}

void RampUpRate(float* pRate, tU32 pTime) {
    NOT_IMPLEMENTED();
}

void DrawSummary(int pCurrent_choice, int pCurrent_mode) {
    tU32 the_time;
    static tU32 last_time;
    static tU32 last_change_time;
    int credit_delta;
    float old_temp_increase;
    float rank_delta;
    NOT_IMPLEMENTED();
}

void StartSummary() {
    NOT_IMPLEMENTED();
}

void SetUpTemps() {
    NOT_IMPLEMENTED();
}

int Summ1GoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int SummCheckGameOver(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    tS3_sound_tag sound_tag;
    NOT_IMPLEMENTED();
}

tSO_result DoEndRaceSummary1() {
    static tFlicette flicker_off[1];
    static tFlicette push[1];
    static tMouse_area mouse_areas[1];
    static tInterface_spec interface_spec;
    int result;
    int completed_already;
    NOT_IMPLEMENTED();
}

void BuildWrecks() {
    int i;
    int position;
    int car_count;
    br_actor* this_car;
    tCar_spec* the_car;
    NOT_IMPLEMENTED();
}

void DisposeWrecks() {
    int i;
    int position;
    int car_count;
    br_actor* this_car;
    tCar_spec* the_car;
    NOT_IMPLEMENTED();
}

int MatrixIsIdentity(br_matrix34* pMat) {
    NOT_IMPLEMENTED();
}

void SpinWrecks(tU32 pFrame_period) {
    int i;
    br_vector3 translation;
    br_matrix34 old_mat;
    NOT_IMPLEMENTED();
}

void ZoomInTo(int pIndex, int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

void ZoomOutTo(int pIndex, int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int WreckPick(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pNear, br_scalar pFar, void* pArg) {
    int i;
    NOT_IMPLEMENTED();
}

int CastSelectionRay(int* pCurrent_choice, int* pCurrent_mode) {
    int mouse_x;
    int mouse_y;
    int i;
    int result;
    br_scalar inv_wreck_pick_scale_factor;
    NOT_IMPLEMENTED();
}

int DamageScrnExit(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

int DamageScrnLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    NOT_IMPLEMENTED();
}

int DamageScrnRight(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    NOT_IMPLEMENTED();
}

int DamageScrnUp(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    int difference;
    int new_difference;
    int new_selection;
    NOT_IMPLEMENTED();
}

int DamageScrnDown(int* pCurrent_choice, int* pCurrent_mode) {
    int i;
    int difference;
    int new_difference;
    int new_selection;
    NOT_IMPLEMENTED();
}

int DamageScrnGoHead(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int ClickDamage(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int mouse_x;
    int mouse_y;
    int old_mouse_x;
    int old_mouse_y;
    NOT_IMPLEMENTED();
}

int DamageScrnDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

tSO_result DoEndRaceSummary2() {
    static tFlicette flicker_off[3];
    static tFlicette push[3];
    static tMouse_area mouse_areas[3];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

int SortScores(void* pFirst_one, void* pSecond_one) {
    NOT_IMPLEMENTED();
}

void SortGameScores() {
    NOT_IMPLEMENTED();
}

void NetSumDraw(int pCurrent_choice, int pCurrent_mode) {
    int i;
    char s[256];
    tNet_game_player_info* player;
    NOT_IMPLEMENTED();
}

void DoNetRaceSummary() {
    static tFlicette flicker_off[1];
    static tFlicette push[1];
    static tMouse_area mouse_areas[1];
    static tInterface_spec interface_spec;
    int i;
    int result;
    tS32 start_time;
    NOT_IMPLEMENTED();
}

tSO_result DoEndRaceSummary(int* pFirst_summary_done, tRace_result pRace_result) {
    tSO_result result;
    NOT_IMPLEMENTED();
}
