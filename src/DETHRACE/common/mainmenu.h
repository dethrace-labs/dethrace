#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "br_types.h"
#include "dr_types.h"

int MainMenuDone1(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

int MainMenuDone2(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

void StartMainMenu();

int DoMainMenuInterface(tU32 pTime_out, int pContinue_allowed);

tMM_result GetMainMenuOption(tU32 pTime_out, int pContinue_allowed);

void QuitVerifyStart();

int QuitVerifyDone(int pCurrent_choice, int pCurrent_mode, int pGo_ahead, int pEscaped, int pTimed_out);

int DoVerifyQuit(int pReplace_background);

tMM_result DoMainMenu(tU32 pTime_out, int pSave_allowed, int pContinue_allowed);

void DoMainMenuScreen(tU32 pTime_out, int pSave_allowed, int pContinue_allowed);

#endif
