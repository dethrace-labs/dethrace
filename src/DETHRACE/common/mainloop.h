#ifndef _MAINLOOP_H_
#define _MAINLOOP_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 0
// Size: 161
void ToggleInfo();

// Offset: 164
// Size: 220
void CalculateFrameRate();

// Offset: 384
// Size: 104
void LoseOldestWastedMassage();

// Offset: 488
// Size: 97
// EAX: pIndex
void QueueWastedMassage(int pIndex);

// Offset: 588
// Size: 2796
void MungeHeadups();

// Offset: 3384
// Size: 588
// EAX: pCamera_period
void UpdateFramePeriod(tU32 *pCamera_period);

// Offset: 3972
// Size: 45
tU32 GetLastTickCount();

// Offset: 4020
// Size: 192
void CheckTimer();

// Offset: 4212
// Size: 319
int MungeRaceFinished();

// Offset: 4532
// Size: 1868
tRace_result MainGameLoop();

// Offset: 6400
// Size: 61
tRace_result DoRace();

#endif
