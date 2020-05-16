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

// Offset: 0
// Size: 1371
// EAX: pOffset
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

// Offset: 1372
// Size: 141
// EAX: pFrom
// EDX: pTo
// EBX: pTime_to_move
void MoveRaceList(int pFrom, int pTo, tS32 pTime_to_move) {
    tS32 start_time;
    tS32 the_time;
    int move_distance;
    int pitch;
    NOT_IMPLEMENTED();
}

// Offset: 1516
// Size: 227
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int UpRace(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 1744
// Size: 232
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int DownRace(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 1976
// Size: 229
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int ClickOnRace(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int x_coord;
    int y_coord;
    int race_delta;
    NOT_IMPLEMENTED();
}

// Offset: 2208
// Size: 61
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int UpClickRace(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

// Offset: 2272
// Size: 61
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int DownClickRace(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

// Offset: 2336
// Size: 55
void StartChangeRace() {
    NOT_IMPLEMENTED();
}

// Offset: 2392
// Size: 227
// EAX: pRace_index
// EDX: pNet_mode
// EBX: pNet_race_sequence
int ChangeRace(int* pRace_index, int pNet_mode, tNet_sequence_type pNet_race_sequence) {
    static tFlicette flicker_on[4];
    static tFlicette flicker_off[4];
    static tFlicette push[4];
    static tMouse_area mouse_areas[5];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

// Offset: 2620
// Size: 62
void DoChangeRace() {
    NOT_IMPLEMENTED();
}

// Offset: 2684
// Size: 738
// EAX: pCurrent_choice
// EDX: pCurrent_mode
void DrawCar(int pCurrent_choice, int pCurrent_mode) {
    char s[64];
    int text_x;
    int text_width;
    NOT_IMPLEMENTED();
}

// Offset: 3424
// Size: 97
void SetCarFlic() {
    NOT_IMPLEMENTED();
}

// Offset: 3524
// Size: 287
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int UpCar(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 3812
// Size: 292
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int DownCar(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 4104
// Size: 61
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int UpClickCar(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

// Offset: 4168
// Size: 61
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int DownClickCar(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

// Offset: 4232
// Size: 107
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int ChangeCarGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 4340
// Size: 918
// EAX: pNet_mode
// EDX: pCar_index
// EBX: pNet_game
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

// Offset: 5260
// Size: 48
void DoChangeCar() {
    NOT_IMPLEMENTED();
}

// Offset: 5308
// Size: 264
int PartsShopRecommended() {
    int i;
    int current_index;
    int counter;
    NOT_IMPLEMENTED();
}

// Offset: 5572
// Size: 180
// EAX: pCategory
// EDX: pIndex
// EBX: pPrice
// ECX: pCost
void CalcPartPrice(int pCategory, int pIndex, int* pPrice, int* pCost) {
    int current_value;
    NOT_IMPLEMENTED();
}

// Offset: 5752
// Size: 145
// EAX: pCategory
// EDX: pIndex
int BuyPart(int pCategory, int pIndex) {
    int price;
    int cost;
    NOT_IMPLEMENTED();
}

// Offset: 5900
// Size: 257
void DoAutoParts() {
    int lowest_yet;
    int lowest_one;
    int price;
    int cost;
    int current_level;
    NOT_IMPLEMENTED();
}

// Offset: 6160
// Size: 174
void DrawPartsLabel() {
    NOT_IMPLEMENTED();
}

// Offset: 6336
// Size: 254
// EAX: pTotal_as_well
void ErasePartsText(int pTotal_as_well) {
    NOT_IMPLEMENTED();
}

// Offset: 6592
// Size: 788
void DrawPartsText() {
    int cost;
    NOT_IMPLEMENTED();
}

// Offset: 7380
// Size: 132
void SetPartsImage() {
    NOT_IMPLEMENTED();
}

// Offset: 7512
// Size: 129
int GetPartsMax() {
    NOT_IMPLEMENTED();
}

// Offset: 7644
// Size: 138
void CalcPartsIndex() {
    NOT_IMPLEMENTED();
}

// Offset: 7784
// Size: 148
void DoExchangePart() {
    int cost;
    NOT_IMPLEMENTED();
}

// Offset: 7932
// Size: 516
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int PartsShopGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    int flic_index;
    NOT_IMPLEMENTED();
}

// Offset: 8448
// Size: 367
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int UpPart(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 8816
// Size: 376
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int DownPart(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 9192
// Size: 61
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int UpClickPart(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

// Offset: 9256
// Size: 61
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int DownClickPart(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

// Offset: 9320
// Size: 197
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int PartsArrowsOn(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 9520
// Size: 197
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int PartsArrowsOff(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 9720
// Size: 44
void StartPartsShop() {
    NOT_IMPLEMENTED();
}

// Offset: 9764
// Size: 77
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int DonePartsShop(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

// Offset: 9844
// Size: 43
// EAX: pCurrent_choice
// EDX: pCurrent_mode
void DrawPartsShop(int pCurrent_choice, int pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 9888
// Size: 195
// EAX: pFade_away
void DoPartsShop(int pFade_away) {
    static tFlicette flicker_on[7];
    static tFlicette flicker_off[7];
    static tFlicette push[7];
    static tMouse_area mouse_areas[7];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

// Offset: 10084
// Size: 66
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int AutoPartsDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

// Offset: 10152
// Size: 166
tSO_result DoAutoPartsShop() {
    static tFlicette flicker_off[3];
    static tFlicette push[3];
    static tMouse_area mouse_areas[3];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

// Offset: 10320
// Size: 97
void SetOpponentFlic() {
    NOT_IMPLEMENTED();
}

// Offset: 10420
// Size: 161
void DrawSceneyMappyInfoVieweyThing() {
    NOT_IMPLEMENTED();
}

// Offset: 10584
// Size: 80
void DismissSceneyMappyInfoVieweyThing() {
    NOT_IMPLEMENTED();
}

// Offset: 10664
// Size: 71
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int SelectRaceDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

// Offset: 10736
// Size: 191
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int StartRaceGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 10928
// Size: 99
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int TryToMoveToArrows(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 11028
// Size: 296
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int UpOpponent(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 11324
// Size: 302
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int DownOpponent(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 11628
// Size: 61
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int UpClickOpp(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

// Offset: 11692
// Size: 61
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int DownClickOpp(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

// Offset: 11756
// Size: 51
void SelectRaceStart() {
    NOT_IMPLEMENTED();
}

// Offset: 11808
// Size: 480
int SuggestRace() {
    int least_done;
    int suggested_so_far;
    int suggested_race;
    int new_suggestion;
    int number_of_visits;
    NOT_IMPLEMENTED();
}

// Offset: 12288
// Size: 1493
// EAX: pCurrent_choice
// EDX: pCurrent_mode
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

// Offset: 13784
// Size: 754
// EAX: pSecond_time_around
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

// Offset: 14540
// Size: 135
// EAX: pX
// EDX: pY
// EBX: pImage
void DrawGridCar(int pX, int pY, br_pixelmap* pImage) {
    NOT_IMPLEMENTED();
}

// Offset: 14676
// Size: 2113
// EAX: pOffset
// EDX: pDraw_it
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

// Offset: 16792
// Size: 153
// EAX: pFrom
// EDX: pTo
// EBX: pTime_to_move
void MoveGrid(int pFrom, int pTo, tS32 pTime_to_move) {
    tS32 start_time;
    tS32 the_time;
    int move_distance;
    int pitch;
    NOT_IMPLEMENTED();
}

// Offset: 16948
// Size: 57
// EAX: pPosition
int CalcGridOffset(int pPosition) {
    NOT_IMPLEMENTED();
}

// Offset: 17008
// Size: 77
// EAX: pCurrent_choice
// EDX: pCurrent_mode
void GridDraw(int pCurrent_choice, int pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 17088
// Size: 112
// EAX: pFirst_index
// EDX: pSecond_index
void ActuallySwapOrder(int pFirst_index, int pSecond_index) {
    tOpp_spec temp_opp;
    NOT_IMPLEMENTED();
}

// Offset: 17200
// Size: 241
// EAX: pFirst_index
// EDX: pSecond_index
void DoGridTransition(int pFirst_index, int pSecond_index) {
    tU32 start_time;
    tU32 the_time;
    NOT_IMPLEMENTED();
}

// Offset: 17444
// Size: 895
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

// Offset: 18340
// Size: 186
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int CheckNextStage(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 18528
// Size: 212
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int ChallengeDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

// Offset: 18740
// Size: 172
void DoChallengeScreen() {
    static tFlicette flicker_off[2];
    static tFlicette push[2];
    static tMouse_area mouse_areas[2];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

// Offset: 18912
// Size: 81
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int GridDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

// Offset: 18996
// Size: 75
void GridStart() {
    NOT_IMPLEMENTED();
}

// Offset: 19072
// Size: 185
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int GridMoveLeft(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 19260
// Size: 166
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int GridMoveRight(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 19428
// Size: 253
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int GridClickCar(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int rel_pos;
    int new_pos;
    int base_pos;
    int x_coord;
    NOT_IMPLEMENTED();
}

// Offset: 19684
// Size: 213
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int GridClickNumbers(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int new_pos;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 19900
// Size: 61
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int GridClickLeft(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

// Offset: 19964
// Size: 61
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pX_offset
// ECX: pY_offset
int GridClickRight(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    NOT_IMPLEMENTED();
}

// Offset: 20028
// Size: 90
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int CheckChallenge(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 20120
// Size: 100
// EAX: pOur_rank
int FindBestPos(int pOur_rank) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 20220
// Size: 56
// EAX: pFirst_one
// EDX: pSecond_one
int SortGridFunction(void* pFirst_one, void* pSecond_one) {
    NOT_IMPLEMENTED();
}

// Offset: 20276
// Size: 286
void SortOpponents() {
    NOT_IMPLEMENTED();
}

// Offset: 20564
// Size: 326
tSO_result DoGridPosition() {
    static tFlicette flicker_off[3];
    static tFlicette push[3];
    static tMouse_area mouse_areas[5];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

// Offset: 20892
// Size: 185
void CheckPlayersAreResponding() {
    tU32 time;
    tNet_message* message;
    NOT_IMPLEMENTED();
}

// Offset: 21080
// Size: 39
void NetSynchStartStart() {
    NOT_IMPLEMENTED();
}

// Offset: 21212
// Size: 759
// EAX: pCurrent_choice
// EDX: pCurrent_mode
void NetSynchStartDraw(int pCurrent_choice, int pCurrent_mode) {
    int i;
    int number_ready;
    char s[256];
    NOT_IMPLEMENTED();
}

// Offset: 21972
// Size: 82
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int NetSynchStartDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

// Offset: 22056
// Size: 217
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int NetSynchStartGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 22276
// Size: 111
// EAX: pCurrent_choice
// EDX: pCurrent_mode
int ExitWhenReady(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 22388
// Size: 246
// EAX: pMode
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

// Offset: 22636
// Size: 100
tSO_result NetSynchRaceStart() {
    NOT_IMPLEMENTED();
}
