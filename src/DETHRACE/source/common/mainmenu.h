#include "dr_types.h"
#include "br_types.h"
// Offset: 0
// Size: 194
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int MainMenuDone1(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

// Offset: 196
// Size: 163
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int MainMenuDone2(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

// Offset: 360
// Size: 251
void StartMainMenu();

// Offset: 612
// Size: 455
// EAX: pTime_out
// EDX: pContinue_allowed
int DoMainMenuInterface(tU32 pTime_out);

// Offset: 1068
// Size: 256
// EAX: pTime_out
// EDX: pContinue_allowed
tMM_result GetMainMenuOption(tU32 pTime_out);

// Offset: 1324
// Size: 169
void QuitVerifyStart();

// Offset: 1496
// Size: 220
// EAX: pCurrent_choice
// EDX: pCurrent_mode
// EBX: pGo_ahead
// ECX: pEscaped
int QuitVerifyDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

// Offset: 1716
// Size: 333
// EAX: pReplace_background
int DoVerifyQuit(int pReplace_background);

// Offset: 2052
// Size: 366
// EAX: pTime_out
// EDX: pSave_allowed
// EBX: pContinue_allowed
tMM_result DoMainMenu(tU32 pTime_out, int pSave_allowed);

// Offset: 2420
// Size: 304
// EAX: pTime_out
// EDX: pSave_allowed
// EBX: pContinue_allowed
void DoMainMenuScreen(tU32 pTime_out, int pSave_allowed);

