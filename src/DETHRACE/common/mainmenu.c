#include "mainmenu.h"
#include <stdlib.h>

char* gPalette_copy;
int gPixel_buffer_size;
tInterface_spec* gMain_menu_spec;
int gMouse_was_started;
int gReplace_background;
char* gPixels_copy;

// Offset: 0
// Size: 194
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int MainMenuDone1(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

// Offset: 196
// Size: 163
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int MainMenuDone2(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

// Offset: 360
// Size: 251
void StartMainMenu() {
    NOT_IMPLEMENTED();
}

// Offset: 612
// Size: 455
// EAX: pTime_out
// EDX: pContinue_allowed
int DoMainMenuInterface(tU32 pTime_out, int pContinue_allowed) {
    static tFlicette flicker_on1[8];
    static tFlicette flicker_off1[8];
    static tFlicette push1[8];
    static tMouse_area mouse_areas1[8];
    static tInterface_spec interface_spec1;
    static tFlicette flicker_on2[5];
    static tFlicette flicker_off2[5];
    static tFlicette push2[5];
    static tMouse_area mouse_areas2[5];
    static tInterface_spec interface_spec2;
    int result;
    NOT_IMPLEMENTED();
}

// Offset: 1068
// Size: 256
// EAX: pTime_out
// EDX: pContinue_allowed
tMM_result GetMainMenuOption(tU32 pTime_out, int pContinue_allowed) {
    int result;
    NOT_IMPLEMENTED();
}

// Offset: 1324
// Size: 169
void QuitVerifyStart() {
    NOT_IMPLEMENTED();
}

// Offset: 1496
// Size: 220
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int QuitVerifyDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
    NOT_IMPLEMENTED();
}

// Offset: 1716
// Size: 333
// EAX: pReplace_background
int DoVerifyQuit(int pReplace_background) {
    static tFlicette flicker_on[2];
    static tFlicette flicker_off[2];
    static tFlicette push[2];
    static tMouse_area mouse_areas[2];
    static tInterface_spec interface_spec;
    int result;
    int switched_res;
    int woz_in_race;
    NOT_IMPLEMENTED();
}

// Offset: 2052
// Size: 366
// EAX: pTime_out
// EDX: pSave_allowed
// EBX: pContinue_allowed
tMM_result DoMainMenu(tU32 pTime_out, int pSave_allowed, int pContinue_allowed) {
    tMM_result the_result;
    NOT_IMPLEMENTED();
}

// Offset: 2420
// Size: 304
// EAX: pTime_out
// EDX: pSave_allowed
// EBX: pContinue_allowed
void DoMainMenuScreen(tU32 pTime_out, int pSave_allowed, int pContinue_allowed) {
    tPlayer_status old_status;
    NOT_IMPLEMENTED();
}
