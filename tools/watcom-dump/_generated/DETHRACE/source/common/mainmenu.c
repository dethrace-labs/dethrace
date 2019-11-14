#include "mainmenu.h"

char *gPalette_copy;
int gPixel_buffer_size;
tInterface_spec *gMain_menu_spec;
int gMouse_was_started;
int gReplace_background;
char *gPixels_copy;

// Offset: 0
// Size: 194
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int MainMenuDone1(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
}

// Offset: 196
// Size: 163
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int MainMenuDone2(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
}

// Offset: 360
// Size: 251
void StartMainMenu() {
}

// Offset: 612
// Size: 455
// EAX: pTime_out
// EDX: pContinue_allowed
int DoMainMenuInterface(tU32 pTime_out, int pContinue_allowed) {
    tFlicette flicker_on1[8];
    tFlicette flicker_off1[8];
    tFlicette push1[8];
    tMouse_area mouse_areas1[8];
    tInterface_spec interface_spec1;
    tFlicette flicker_on2[5];
    tFlicette flicker_off2[5];
    tFlicette push2[5];
    tMouse_area mouse_areas2[5];
    tInterface_spec interface_spec2;
    int result;
}

// Offset: 1068
// Size: 256
// EAX: pTime_out
// EDX: pContinue_allowed
tMM_result GetMainMenuOption(tU32 pTime_out, int pContinue_allowed) {
    int result;
}

// Offset: 1324
// Size: 169
void QuitVerifyStart() {
}

// Offset: 1496
// Size: 220
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int QuitVerifyDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out) {
}

// Offset: 1716
// Size: 333
// EAX: pReplace_background
int DoVerifyQuit(int pReplace_background) {
    tFlicette flicker_on[2];
    tFlicette flicker_off[2];
    tFlicette push[2];
    tMouse_area mouse_areas[2];
    tInterface_spec interface_spec;
    int result;
    int switched_res;
    int woz_in_race;
}

// Offset: 2052
// Size: 366
// EAX: pTime_out
// EDX: pSave_allowed
// EBX: pContinue_allowed
tMM_result DoMainMenu(tU32 pTime_out, int pSave_allowed, int pContinue_allowed) {
    tMM_result the_result;
}

// Offset: 2420
// Size: 304
// EAX: pTime_out
// EDX: pSave_allowed
// EBX: pContinue_allowed
void DoMainMenuScreen(tU32 pTime_out, int pSave_allowed, int pContinue_allowed) {
    tPlayer_status old_status;
}

