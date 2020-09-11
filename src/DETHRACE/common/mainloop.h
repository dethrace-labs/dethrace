#ifndef _MAINLOOP_H_
#define _MAINLOOP_H_

#include "br_types.h"
#include "dr_types.h"

void ToggleInfo();

void CalculateFrameRate();

void LoseOldestWastedMassage();

void QueueWastedMassage(int pIndex);

void MungeHeadups();

void UpdateFramePeriod(tU32* pCamera_period);

tU32 GetLastTickCount();

void CheckTimer();

int MungeRaceFinished();

tRace_result MainGameLoop();

tRace_result DoRace();

#endif
