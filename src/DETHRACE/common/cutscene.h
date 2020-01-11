#ifndef _CUTSCENE_H_
#define _CUTSCENE_H_

#include "br_types.h"
#include "dr_types.h"

// Offset: 0
// Size: 118
void* radmalloc(unsigned long numbytes);

// Offset: 120
// Size: 59
void radfree(void* ptr);

// Offset: 180
// Size: 179
// EAX: pIndex
// EDX: pWait_end
// EBX: pSound_ID
void ShowCutScene(int pIndex, int pWait_end, int pSound_ID, br_scalar pDelay);

// Offset: 360
// Size: 34
void DoSCILogo();

// Offset: 396
// Size: 34
void DoStainlessLogo();

// Offset: 432
// Size: 844
// EAX: pSmack_name
void PlaySmackerFile(char* pSmack_name);

// Offset: 1276
// Size: 59
void DoOpeningAnimation();

// Offset: 1336
// Size: 79
void DoNewGameAnimation();

// Offset: 1416
// Size: 34
void DoGoToRaceAnimation();

// Offset: 1452
// Size: 215
void DoEndRaceAnimation();

// Offset: 1668
// Size: 54
void DoGameOverAnimation();

// Offset: 1724
// Size: 54
void DoGameCompletedAnimation();

// Offset: 1780
// Size: 70
void StartLoadingScreen();

#endif
