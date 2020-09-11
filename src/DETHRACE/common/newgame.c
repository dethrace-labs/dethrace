#include "newgame.h"
#include <stdlib.h>

char x[] = "xxxxxxxx.TXT";
char y[] = "yyyyyyyy.TXT";
char* gBasic_car_names[2] = { x, y };
tNet_game_options gNet_settings[8];
tRadio_bastards gRadio_bastards[11];
tJoinable_game gGames_to_join[6];
tNet_game_options* gOptions;
int gNet_target[7];
int gLast_graph_sel;
tInterface_spec* gThe_interface_spec;
tNet_sequence_type gNet_race_sequence;
tNet_game_type gLast_game_type;
int gCurrent_net_game_count;
tU32 gAnne_flic_data_length;
int gShifted_default_yet;
int gNet_storage_allocated;
tU8* gFrank_flic_data;
char* gNet_name;
tU32 gFrank_flic_data_length;
tU8* gAnne_flic_data;
int gLast_net_choose_box;
int gCurrent_game_selection;
int gRace_index;
int gRadio_selected;

// Offset: 0
// Size: 149
void StartRollingPlayerNamesIn() {
    NOT_IMPLEMENTED();
}

// Offset: 152
// Size: 314
void FrankAnneStart1() {
    NOT_IMPLEMENTED();
}

// Offset: 468
// Size: 150
void FrankAnneStart2() {
    NOT_IMPLEMENTED();
}

// Offset: 620
// Size: 68
void GetPlayerName(int pStarting_to_type, int pCurrent_choice, char* pString, int* pMax_length) {
    NOT_IMPLEMENTED();
}

// Offset: 688
// Size: 203
int FrankAnneDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

// Offset: 892
// Size: 139
void FrankAnneDraw(int pCurrent_choice, int pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 1032
// Size: 123
int FrankieOrAnnie() {
    static tFlicette flicker_off[3];
    static tFlicette push[3];
    static tMouse_area mouse_areas[3];
    static tRectile recopy_areas[2];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

// Offset: 1156
// Size: 88
int SelectSkillLevel() {
    static tFlicette flicker_off[4];
    static tFlicette push[4];
    static tMouse_area mouse_areas[4];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

// Offset: 1244
// Size: 323
int DoOnePlayerStart() {
    tProgram_state saved_state;
    NOT_IMPLEMENTED();
}

// Offset: 1568
// Size: 263
int NewNetGameUp(int* pCurrent_choice, int* pCurrent_mode) {
    int new_sel;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 1832
// Size: 263
int NewNetGameDown(int* pCurrent_choice, int* pCurrent_mode) {
    int new_sel;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 2096
// Size: 70
void DisposeJoinableGame(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 2168
// Size: 123
void DrawAnItem(int pX, int pY_index, int pFont_index, char* pText) {
    NOT_IMPLEMENTED();
}

// Offset: 2292
// Size: 119
void DrawColumnHeading(int pStr_index, int pX) {
    NOT_IMPLEMENTED();
}

// Offset: 2412
// Size: 1821
void DrawGames(int pCurrent_choice, int pCurrent_mode) {
    int i;
    int font_index;
    int current_index;
    int x_coord;
    int y_coord;
    char s[256];
    char* s2;
    char* s3;
    NOT_IMPLEMENTED();
}

// Offset: 4236
// Size: 85
void InitGamesToJoin() {
    NOT_IMPLEMENTED();
}

// Offset: 4324
// Size: 95
void DisposeJoinList(int pExemption) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 4420
// Size: 227
void AddToJoinList(tNet_game_details* pGame) {
    int i;
    int slot_to_use;
    tU32 this_game_ID;
    NOT_IMPLEMENTED();
}

// Offset: 4648
// Size: 111
void NewNetStart1() {
    NOT_IMPLEMENTED();
}

// Offset: 4760
// Size: 61
void NewNetStart2() {
    NOT_IMPLEMENTED();
}

// Offset: 4824
// Size: 62
void NewNetGetName(int pStarting_to_type, int pCurrent_choice, char* pString, int* pMax_length) {
    NOT_IMPLEMENTED();
}

// Offset: 4888
// Size: 72
int NewNetDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

// Offset: 4960
// Size: 72
int NewNetGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 5032
// Size: 287
tJoin_or_host_result JoinOrHostGame(tNet_game_details** pGame_to_join) {
    static tFlicette flicker_on[2];
    static tFlicette flicker_off[2];
    static tFlicette push[2];
    static tMouse_area mouse_areas[3];
    static tRectile recopy_areas[1];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

// Offset: 5320
// Size: 223
void GetNetOptions(tNet_game_options* pGame_options) {
    NOT_IMPLEMENTED();
}

// Offset: 5544
// Size: 234
void SetNetOptions(tNet_game_options* pGame_options) {
    NOT_IMPLEMENTED();
}

// Offset: 5780
// Size: 72
void NetPlayCheckboxOn2(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 5852
// Size: 72
void NetPlayCheckboxOff2(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 5924
// Size: 75
void NetPlayCheckboxOn(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 6000
// Size: 75
void NetPlayCheckboxOff(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 6076
// Size: 81
void NetPlayRadioOn2(int pIndex, int pValue) {
    NOT_IMPLEMENTED();
}

// Offset: 6160
// Size: 81
void NetPlayRadioOff2(int pIndex, int pValue) {
    NOT_IMPLEMENTED();
}

// Offset: 6244
// Size: 79
void NetPlayRadioOn(int pIndex, int pValue) {
    NOT_IMPLEMENTED();
}

// Offset: 6324
// Size: 79
void NetPlayRadioOff(int pIndex, int pValue) {
    NOT_IMPLEMENTED();
}

// Offset: 6404
// Size: 229
void DrawNOptInitialRadios() {
    int j;
    NOT_IMPLEMENTED();
}

// Offset: 6636
// Size: 84
void NetRadioChanged(int pIndex, int pNew_value) {
    NOT_IMPLEMENTED();
}

// Offset: 6720
// Size: 116
void NetCheckboxChanged(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 6836
// Size: 153
int NetOptLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int new_value;
    NOT_IMPLEMENTED();
}

// Offset: 6992
// Size: 157
int NetOptRight(int* pCurrent_choice, int* pCurrent_mode) {
    int new_value;
    NOT_IMPLEMENTED();
}

// Offset: 7152
// Size: 148
int NetOptUp(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 7300
// Size: 148
int NetOptDown(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 7448
// Size: 213
int NetRadioClick(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 7664
// Size: 142
void RevertToDefaults() {
    FILE* f;
    tNet_game_options net_options;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 7808
// Size: 156
void DefaultNetSettings() {
    FILE* f;
    int i;
    STUB();
}

// Offset: 7964
// Size: 98
int NetOptGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 8064
// Size: 202
void NetPlotAGraphBox(int pIndex, int pColour_value) {
    NOT_IMPLEMENTED();
}

// Offset: 8268
// Size: 50
void NetDrawAGraphBox(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 8320
// Size: 47
void NetEraseAGraphBox(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 8368
// Size: 80
void DrawNetOptBox(int pCurrent_choice, int pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 8448
// Size: 111
void DoNetOptions(tNet_game_options* pGame_options) {
    static tFlicette flicker_on[14];
    static tFlicette flicker_off[14];
    static tFlicette push[14];
    static tMouse_area mouse_areas[14];
    static tInterface_spec interface_spec;
    NOT_IMPLEMENTED();
}

// Offset: 8560
// Size: 190
void SetOptions(tNet_game_type pGame_type, tNet_game_options* pGame_options) {
    NOT_IMPLEMENTED();
}

// Offset: 8992
// Size: 44
void DrawNetChooseInitial() {
    NOT_IMPLEMENTED();
}

// Offset: 9036
// Size: 250
int NetChooseGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 9288
// Size: 182
void PlotAGraphBox(int pIndex, int pColour_value) {
    NOT_IMPLEMENTED();
}

// Offset: 9472
// Size: 50
void DrawAGraphBox(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 9524
// Size: 47
void EraseAGraphBox(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 9572
// Size: 719
void DrawNetChoose(int pCurrent_choice, int pCurrent_mode) {
    char s[256];
    tU32* k;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 10292
// Size: 61
int NetChooseLR(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

// Offset: 10356
// Size: 190
void SetGameTarget(tNet_game_type* pGame_type, tNet_game_options* pGame_options) {
    NOT_IMPLEMENTED();
}

// Offset: 10548
// Size: 383
int NetGameChoices(tNet_game_type* pGame_type, tNet_game_options* pGame_options, int* pRace_index) {
    static tFlicette flicker_on[11];
    static tFlicette flicker_off[11];
    static tFlicette push[11];
    static tMouse_area mouse_areas[11];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

// Offset: 10932
// Size: 95
void ReadNetGameChoices(tNet_game_type* pGame_type, tNet_game_options* pGame_options, int* pRace_index) {
    NOT_IMPLEMENTED();
}

// Offset: 11028
// Size: 56
int ChooseStartRace(int* pRank) {
    NOT_IMPLEMENTED();
}

// Offset: 11084
// Size: 63
void SetUpOtherNetThings(tNet_game_details* pNet_game) {
    NOT_IMPLEMENTED();
}

// Offset: 11148
// Size: 97
void RequestCarDetails(tNet_game_details* pNet_game) {
    tNet_message* message;
    NOT_IMPLEMENTED();
}

// Offset: 11248
// Size: 181
int PickARandomCar() {
    int cars[120];
    int array_size;
    NOT_IMPLEMENTED();
}

// Offset: 11432
// Size: 54
void PollCarDetails(tNet_game_details* pNet_game) {
    NOT_IMPLEMENTED();
}

// Offset: 11488
// Size: 278
void SetNetAvailability(tNet_game_options* pOptions) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 11768
// Size: 396
int ChooseNetCar(tNet_game_details* pNet_game, tNet_game_options* pOptions, int* pCar_index, int pIm_the_host_so_fuck_off) {
    tS32 start_time;
    int i;
    int result;
    int car_index;
    int the_car_index;
    NOT_IMPLEMENTED();
}

// Offset: 12164
// Size: 115
void InitNetStorageSpace() {
    NOT_IMPLEMENTED();
}

// Offset: 12280
// Size: 63
void DisposeNetStorageSpace() {
    NOT_IMPLEMENTED();
}

// Offset: 12344
// Size: 793
int DoMultiPlayerStart() {
    tNet_game_details* game_to_join;
    char s[256];
    char* s2;
    tNet_game_type new_game_type;
    tNet_game_options new_game_options;
    int start_rank;
    int car_index;
    NOT_IMPLEMENTED();
}
