#include "loadsave.h"
#include <stdlib.h>

tSave_game* gSaved_games[8];
int gStarted_typing;
int gSave_allowed;

void CorrectLoadByteOrdering(int pIndex) {
    int i;
    NOT_IMPLEMENTED();
}

tU32 CalcLSChecksum(tSave_game* pSaved_game) {
    tU32 checksum;
    tU32 checksum2;
    int i;
    tU8* ptr;
    NOT_IMPLEMENTED();
}

void LoadSavedGames() {
    int i;
    FILE* f;
    tU32 the_size;
    NOT_IMPLEMENTED();
}

void DisposeSavedGames() {
    NOT_IMPLEMENTED();
}

void LoadTheGame(int pSlot_index) {
    int i;
    char the_car_name[14];
    NOT_IMPLEMENTED();
}

void StartRollingSaveNamesIn() {
    int save_slot_height;
    NOT_IMPLEMENTED();
}

void LoadStart() {
    NOT_IMPLEMENTED();
}

int DoLoadGame() {
    static tFlicette flicker_on[9];
    static tFlicette flicker_off[9];
    static tFlicette push[9];
    static tMouse_area mouse_areas[9];
    static tRectile recopy_areas[24];
    static tInterface_spec interface_spec;
    int result;
    NOT_IMPLEMENTED();
}

void CorrectSaveByteOrdering(int pIndex) {
    int i;
    NOT_IMPLEMENTED();
}

void SaveTheGame(int pSlot_number) {
    tPath_name the_path;
    FILE* f;
    NOT_IMPLEMENTED();
}

int ConfirmMidGameSave() {
    static tFlicette flicker_off[2];
    static tFlicette push[2];
    static tMouse_area mouse_areas[2];
    static tInterface_spec interface_spec;
    NOT_IMPLEMENTED();
}

void MakeSavedGame(tSave_game** pSave_record) {
    int i;
    NOT_IMPLEMENTED();
}

void SaveStart() {
    NOT_IMPLEMENTED();
}

void GetSaveName(int pStarting_to_type, int pCurrent_choice, char* pString, int* pMax_length) {
    NOT_IMPLEMENTED();
}

int SaveDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

int SaveGoAhead(int* pCurrent_choice, int* pCurrent_mode) {
    char s1[256];
    char s2[256];
    NOT_IMPLEMENTED();
}

int SaveEscape(int* pCurrent_choice, int* pCurrent_mode) {
    char s1[256];
    char s2[256];
    NOT_IMPLEMENTED();
}

int SaveGameInterface(int pDefault_choice) {
    static tFlicette flicker_on[9];
    static tFlicette flicker_off[9];
    static tFlicette push[9];
    static tMouse_area mouse_areas[9];
    static tRectile recopy_areas[24];
    static tInterface_spec interface_spec;
    NOT_IMPLEMENTED();
}

void DoSaveGame(int pSave_allowed) {
    STUB();
}
