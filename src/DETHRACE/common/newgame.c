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

void StartRollingPlayerNamesIn() {
    NOT_IMPLEMENTED();
}

void FrankAnneStart1() {
    NOT_IMPLEMENTED();
}

void FrankAnneStart2() {
    NOT_IMPLEMENTED();
}

void GetPlayerName(int pStarting_to_type, int pCurrent_choice, char* pString, int* pMax_length) {
    NOT_IMPLEMENTED();
}

int FrankAnneDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

void FrankAnneDraw(int pCurrent_choice, int pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int FrankieOrAnnie() {
    static tFlicette flicker_off[3];
    static tFlicette push[3];
    static tMouse_area mouse_areas[3];
    static tRectile recopy_areas[2];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

int SelectSkillLevel() {
    static tFlicette flicker_off[4];
    static tFlicette push[4];
    static tMouse_area mouse_areas[4];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

int DoOnePlayerStart() {
    tProgram_state saved_state;
    NOT_IMPLEMENTED();
}

int NewNetGameUp(int* pCurrent_choice, int* pCurrent_mode) {
    int new_sel;
    int i;
    NOT_IMPLEMENTED();
}

int NewNetGameDown(int* pCurrent_choice, int* pCurrent_mode) {
    int new_sel;
    int i;
    NOT_IMPLEMENTED();
}

void DisposeJoinableGame(int pIndex) {
    NOT_IMPLEMENTED();
}

void DrawAnItem(int pX, int pY_index, int pFont_index, char* pText) {
    NOT_IMPLEMENTED();
}

void DrawColumnHeading(int pStr_index, int pX) {
    NOT_IMPLEMENTED();
}

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

void InitGamesToJoin() {
    NOT_IMPLEMENTED();
}

void DisposeJoinList(int pExemption) {
    int i;
    NOT_IMPLEMENTED();
}

void AddToJoinList(tNet_game_details* pGame) {
    int i;
    int slot_to_use;
    tU32 this_game_ID;
    NOT_IMPLEMENTED();
}

void NewNetStart1() {
    NOT_IMPLEMENTED();
}

void NewNetStart2() {
    NOT_IMPLEMENTED();
}

void NewNetGetName(int pStarting_to_type, int pCurrent_choice, char* pString, int* pMax_length) {
    NOT_IMPLEMENTED();
}

int NewNetDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

int NewNetGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

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

void GetNetOptions(tNet_game_options* pGame_options) {
    NOT_IMPLEMENTED();
}

void SetNetOptions(tNet_game_options* pGame_options) {
    NOT_IMPLEMENTED();
}

void NetPlayCheckboxOn2(int pIndex) {
    NOT_IMPLEMENTED();
}

void NetPlayCheckboxOff2(int pIndex) {
    NOT_IMPLEMENTED();
}

void NetPlayCheckboxOn(int pIndex) {
    NOT_IMPLEMENTED();
}

void NetPlayCheckboxOff(int pIndex) {
    NOT_IMPLEMENTED();
}

void NetPlayRadioOn2(int pIndex, int pValue) {
    NOT_IMPLEMENTED();
}

void NetPlayRadioOff2(int pIndex, int pValue) {
    NOT_IMPLEMENTED();
}

void NetPlayRadioOn(int pIndex, int pValue) {
    NOT_IMPLEMENTED();
}

void NetPlayRadioOff(int pIndex, int pValue) {
    NOT_IMPLEMENTED();
}

void DrawNOptInitialRadios() {
    int j;
    NOT_IMPLEMENTED();
}

void NetRadioChanged(int pIndex, int pNew_value) {
    NOT_IMPLEMENTED();
}

void NetCheckboxChanged(int pIndex) {
    NOT_IMPLEMENTED();
}

int NetOptLeft(int* pCurrent_choice, int* pCurrent_mode) {
    int new_value;
    NOT_IMPLEMENTED();
}

int NetOptRight(int* pCurrent_choice, int* pCurrent_mode) {
    int new_value;
    NOT_IMPLEMENTED();
}

int NetOptUp(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int NetOptDown(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

int NetRadioClick(int* pCurrent_choice, int* pCurrent_mode, int pX_offset, int pY_offset) {
    int i;
    NOT_IMPLEMENTED();
}

void RevertToDefaults() {
    FILE* f;
    tNet_game_options net_options;
    int i;
    NOT_IMPLEMENTED();
}

void DefaultNetSettings() {
    FILE* f;
    int i;
    STUB();
}

int NetOptGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

void NetPlotAGraphBox(int pIndex, int pColour_value) {
    NOT_IMPLEMENTED();
}

void NetDrawAGraphBox(int pIndex) {
    NOT_IMPLEMENTED();
}

void NetEraseAGraphBox(int pIndex) {
    NOT_IMPLEMENTED();
}

void DrawNetOptBox(int pCurrent_choice, int pCurrent_mode) {
    NOT_IMPLEMENTED();
}

void DoNetOptions(tNet_game_options* pGame_options) {
    static tFlicette flicker_on[14];
    static tFlicette flicker_off[14];
    static tFlicette push[14];
    static tMouse_area mouse_areas[14];
    static tInterface_spec interface_spec;
    NOT_IMPLEMENTED();
}

void SetOptions(tNet_game_type pGame_type, tNet_game_options* pGame_options) {
    NOT_IMPLEMENTED();
}

void DrawNetChooseInitial() {
    NOT_IMPLEMENTED();
}

int NetChooseGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

void PlotAGraphBox(int pIndex, int pColour_value) {
    NOT_IMPLEMENTED();
}

void DrawAGraphBox(int pIndex) {
    NOT_IMPLEMENTED();
}

void EraseAGraphBox(int pIndex) {
    NOT_IMPLEMENTED();
}

void DrawNetChoose(int pCurrent_choice, int pCurrent_mode) {
    char s[256];
    tU32* k;
    int i;
    NOT_IMPLEMENTED();
}

int NetChooseLR(int* pCurrent_choice, int* pCurrent_mode) {
    NOT_IMPLEMENTED();
}

void SetGameTarget(tNet_game_type* pGame_type, tNet_game_options* pGame_options) {
    NOT_IMPLEMENTED();
}

int NetGameChoices(tNet_game_type* pGame_type, tNet_game_options* pGame_options, int* pRace_index) {
    static tFlicette flicker_on[11];
    static tFlicette flicker_off[11];
    static tFlicette push[11];
    static tMouse_area mouse_areas[11];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

void ReadNetGameChoices(tNet_game_type* pGame_type, tNet_game_options* pGame_options, int* pRace_index) {
    NOT_IMPLEMENTED();
}

int ChooseStartRace(int* pRank) {
    NOT_IMPLEMENTED();
}

void SetUpOtherNetThings(tNet_game_details* pNet_game) {
    NOT_IMPLEMENTED();
}

void RequestCarDetails(tNet_game_details* pNet_game) {
    tNet_message* message;
    NOT_IMPLEMENTED();
}

int PickARandomCar() {
    int cars[120];
    int array_size;
    NOT_IMPLEMENTED();
}

void PollCarDetails(tNet_game_details* pNet_game) {
    NOT_IMPLEMENTED();
}

void SetNetAvailability(tNet_game_options* pOptions) {
    int i;
    NOT_IMPLEMENTED();
}

int ChooseNetCar(tNet_game_details* pNet_game, tNet_game_options* pOptions, int* pCar_index, int pIm_the_host_so_fuck_off) {
    tS32 start_time;
    int i;
    int result;
    int car_index;
    int the_car_index;
    NOT_IMPLEMENTED();
}

void InitNetStorageSpace() {
    NOT_IMPLEMENTED();
}

void DisposeNetStorageSpace() {
    NOT_IMPLEMENTED();
}

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
