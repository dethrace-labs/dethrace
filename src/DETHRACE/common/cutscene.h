#ifndef _CUTSCENE_H_
#define _CUTSCENE_H_

#include "brender/br_types.h"
#include "brender/brender.h"

#include "dr_types.h"

extern tS32 gLast_demo_end_anim;

void ShowCutScene(int pIndex, int pWait_end, int pSound_ID, br_scalar pDelay);

void DoSCILogo(void);

void DoStainlessLogo(void);

void PlaySmackerFile(char* pSmack_name);

void DoOpeningAnimation(void);

void DoNewGameAnimation(void);

void DoGoToRaceAnimation(void);

void DoEndRaceAnimation(void);

void DoGameOverAnimation(void);

void DoGameCompletedAnimation(void);

void DoFeatureUnavailableInDemo(void);

void DoFullVersionPowerpoint(void);

void DoDemoGoodbye(void);

void StartLoadingScreen(void);

#endif
