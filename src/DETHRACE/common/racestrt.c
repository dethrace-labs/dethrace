#include "racestrt.h"
#include <stdlib.h>

int gGrid_number_colour[4];
br_pixelmap* gDead_car;
int gFade_away_parts_shop;
tU32 gDare_start_time;
int gRefund_rate;
int gSwap_grid_2;
int gSwap_grid_1;
int gChange_race_net_mode;
tParts_category gPart_category;
tU32 gNet_synch_start;
tNet_game_details* gChoose_car_net_game;
int gPart_index;
int gChallenger_index;
tGrid_draw gDraw_grid_status;
tNet_sequence_type gNet_race_sequence;
tU32 gLast_host_query;
br_pixelmap* gTaken_image;
int gGrid_number_x_coords[31];
int gGrid_transition_stage;
int gJust_bought_part;
int gGrid_y_adjust;
br_pixelmap* gBullet_image;
br_pixelmap* gDeceased_image;
int gBest_pos_available;
int gChallenger_position;
int gOpponent_index;
int gChallenge_time;
int gOriginal_position;
int gCurrent_race_index;
tInterface_spec* gStart_interface_spec;
int gCurrent_car_index;
int gOur_starting_position;

void DrawRaceList(int pOffset) {
    int i;
    int font_height;
    int pitch;
    int y;
    int left_most;
    int right_most;
    int rank_colour;
    int text_colour;
    int box_bot;
    int text_width;
    int text_x;
    char rank_str[256];
    NOT_IMPLEMENTED();
}

void MoveRaceList(int pFrom, int pTo, tS32 pTime_to_move) {
    tS32 start_time;
    tS32 the_time;
    int move_distance;
    int pitch;
    NOT_IMPLEMENTED();
}

int UpRace(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int DownRace(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int ClickOnRace(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int x_coord;
    int y_coord;
    int race_delta;
    NOT_IMPLEMENTED();
}

int UpClickRace(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

int DownClickRace(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

void StartChangeRace() {
    NOT_IMPLEMENTED();
}

int ChangeRace(int* pRace_index, int pNet_mode, tNet_sequence_type pNet_race_sequence) {
    static tFlicette flicker_on[4];
    static tFlicette flicker_off[4];
    static tFlicette push[4];
    static tMouse_area mouse_areas[5];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

void DoChangeRace() {
    NOT_IMPLEMENTED();
}

void DrawCar(int pCurrent_choice, int pCurrent_mode) {
    char s[64];
    int text_x;
    int text_width;
    NOT_IMPLEMENTED();
}

void SetCarFlic() {
    NOT_IMPLEMENTED();
}

int UpCar(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int DownCar(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int UpClickCar(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

int DownClickCar(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

int ChangeCarGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int ChangeCar(int pNet_mode, int* pCar_index, tNet_game_details* pNet_game) {
    static tFlicette flicker_on[4];
    static tFlicette flicker_off[4];
    static tFlicette push[4];
    static tMouse_area mouse_areas[4];
    static tInterface_spec interface_spec;
    int i;
    int result;
    int power_up_levels[3];
    NOT_IMPLEMENTED();
}

void DoChangeCar() {
    NOT_IMPLEMENTED();
}

int PartsShopRecommended() {
    int i;
    int current_index;
    int counter;
    NOT_IMPLEMENTED();
}

void CalcPartPrice(int pCategory, int pIndex, int* pPrice, int* pCost) {
    int current_value;
    NOT_IMPLEMENTED();
}

int BuyPart(int pCategory, int pIndex) {
    int price;
    int cost;
    NOT_IMPLEMENTED();
}

void DoAutoParts() {
    int lowest_yet;
    int lowest_one;
    int price;
    int cost;
    int current_level;
    NOT_IMPLEMENTED();
}

void DrawPartsLabel() {
    NOT_IMPLEMENTED();
}

void ErasePartsText(int pTotal_as_well) {
    NOT_IMPLEMENTED();
}

void DrawPartsText() {
    int cost;
    NOT_IMPLEMENTED();
}

void SetPartsImage() {
    NOT_IMPLEMENTED();
}

int GetPartsMax() {
    NOT_IMPLEMENTED();
}

void CalcPartsIndex() {
    NOT_IMPLEMENTED();
}

void DoExchangePart() {
    int cost;
    NOT_IMPLEMENTED();
}

int PartsShopGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    int flic_index;
    NOT_IMPLEMENTED();
}

int UpPart(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int DownPart(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int UpClickPart(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

int DownClickPart(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

int PartsArrowsOn(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int PartsArrowsOff(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

void StartPartsShop() {
    NOT_IMPLEMENTED();
}

int DonePartsShop(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

void DrawPartsShop(int pCurrent_choice, int pCurrent_mode) {
    NOT_IMPLEMENTED();
}

void DoPartsShop(int pFade_away) {
    static tFlicette flicker_on[7];
    static tFlicette flicker_off[7];
    static tFlicette push[7];
    static tMouse_area mouse_areas[7];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

int AutoPartsDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

tSO_result DoAutoPartsShop() {
    static tFlicette flicker_off[3];
    static tFlicette push[3];
    static tMouse_area mouse_areas[3];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

void SetOpponentFlic() {
    NOT_IMPLEMENTED();
}

void DrawSceneyMappyInfoVieweyThing() {
    NOT_IMPLEMENTED();
}

void DismissSceneyMappyInfoVieweyThing() {
    NOT_IMPLEMENTED();
}

int SelectRaceDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

int StartRaceGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int TryToMoveToArrows(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int UpOpponent(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int DownOpponent(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int UpClickOpp(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

int DownClickOpp(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

void SelectRaceStart() {
    NOT_IMPLEMENTED();
}

int SuggestRace() {
    int least_done;
    int suggested_so_far;
    int suggested_race;
    int new_suggestion;
    int number_of_visits;
    NOT_IMPLEMENTED();
}

void SelectRaceDraw(int pCurrent_choice, int pCurrent_mode) {
    tOpponent* the_opponent;
    tText_chunk* the_chunk;
    int j;
    int k;
    int y_coord;
    char s[256];
    char temp_str[256];
    char* sub_pt;
    char sub_str[16];
    tU32* test;
    static tU32 test2;
    NOT_IMPLEMENTED();
}

tSO_result DoSelectRace(int* pSecond_time_around) {
    static tFlicette flicker_on[7];
    static tFlicette flicker_off[7];
    static tFlicette push[7];
    static tMouse_area mouse_areas[7];
    static tInterface_spec interface_spec;
    int result;
    int default_choice;
    int suggested;
    int old_current_race;
    NOT_IMPLEMENTED();
}

void DrawGridCar(int pX, int pY, br_pixelmap* pImage) {
    NOT_IMPLEMENTED();
}

void DrawGrid(int pOffset, int pDraw_it) {
    int i;
    int j;
    int y;
    int x;
    int str_x;
    int width_job;
    int done_highest;
    int str_index;
    int swap_1_x;
    int swap_1_y;
    int swap_2_x;
    int swap_2_y;
    br_pixelmap* the_image;
    br_pixelmap* swap_1_image;
    br_pixelmap* swap_2_image;
    char numbers_str[4][100];
    char total_str[256];
    tU32 the_time;
    NOT_IMPLEMENTED();
}

void MoveGrid(int pFrom, int pTo, tS32 pTime_to_move) {
    tS32 start_time;
    tS32 the_time;
    int move_distance;
    int pitch;
    NOT_IMPLEMENTED();
}

int CalcGridOffset(int pPosition) {
    NOT_IMPLEMENTED();
}

void GridDraw(int pCurrent_choice, int pCurrent_mode) {
    NOT_IMPLEMENTED();
}

void ActuallySwapOrder(int pFirst_index, int pSecond_index) {
    tOpp_spec temp_opp;
    NOT_IMPLEMENTED();
}

void DoGridTransition(int pFirst_index, int pSecond_index) {
    tU32 start_time;
    tU32 the_time;
    NOT_IMPLEMENTED();
}

void ChallengeStart() {
    int i;
    int j;
    int line_count;
    int dare_index;
    FILE* f;
    tPath_name the_path;
    char s[256];
    NOT_IMPLEMENTED();
}

int CheckNextStage(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int ChallengeDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

void DoChallengeScreen() {
    static tFlicette flicker_off[2];
    static tFlicette push[2];
    static tMouse_area mouse_areas[2];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

int GridDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

void GridStart() {
    NOT_IMPLEMENTED();
}

int GridMoveLeft(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int GridMoveRight(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int GridClickCar(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int rel_pos;
    int new_pos;
    int base_pos;
    int x_coord;
    NOT_IMPLEMENTED();
}

int GridClickNumbers(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int new_pos;
    int i;
    NOT_IMPLEMENTED();
}

int GridClickLeft(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

int GridClickRight(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

int CheckChallenge(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int FindBestPos(int pOur_rank) {
    int i;
    NOT_IMPLEMENTED();
}

int SortGridFunction(void* pFirst_one, void* pSecond_one) {
    NOT_IMPLEMENTED();
}

void SortOpponents() {
    NOT_IMPLEMENTED();
}

tSO_result DoGridPosition() {
    static tFlicette flicker_off[3];
    static tFlicette push[3];
    static tMouse_area mouse_areas[5];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

void CheckPlayersAreResponding() {
    tU32 time;
    tNet_message* message;
    NOT_IMPLEMENTED();
}

void NetSynchStartStart() {
    NOT_IMPLEMENTED();
}

void NetSynchStartDraw(int pCurrent_choice, int pCurrent_mode) {
    int i;
    int number_ready;
    char s[256];
    NOT_IMPLEMENTED();
}

int NetSynchStartDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

int NetSynchStartGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int ExitWhenReady(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

tSO_result NetSynchRaceStart2(tNet_synch_mode pMode) {
    static tFlicette flicker_on_hf[2];
    static tFlicette flicker_off_hf[2];
    static tFlicette push_hf[2];
    static tMouse_area mouse_areas_hf[2];
    static tInterface_spec interface_spec_hf;
    static tFlicette flicker_on_hs[1];
    static tFlicette flicker_off_hs[1];
    static tFlicette push_hs[1];
    static tMouse_area mouse_areas_hs[1];
    static tInterface_spec interface_spec_hs;
    static tFlicette flicker_on_c[1];
    static tFlicette flicker_off_c[1];
    static tFlicette push_c[1];
    static tMouse_area mouse_areas_c[1];
    static tInterface_spec interface_spec_c;
    int result;
    NOT_IMPLEMENTED();
}

tSO_result NetSynchRaceStart() {
    NOT_IMPLEMENTED();
}
