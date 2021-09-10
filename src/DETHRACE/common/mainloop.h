#ifndef _MAINLOOP_H_
#define _MAINLOOP_H_

#include "br_types.h"
#include "dr_types.h"

extern int gNasty_kludgey_cockpit_variable;
extern tInfo_mode gInfo_mode;
extern tU32 gLast_tick_count;
extern tU32 gActual_last_tick_count;
extern tU32 gAverage_frame_period;
extern tU32 gOld_camera_time;
extern tU32 gLast_wasted_massage_start;
extern float gMr_odo;
extern tU32 gWasted_last_flash;
extern tTime_bonus_state gTime_bonus_state;
extern int gQueued_wasted_massages_count;
extern int gTime_bonus;
extern int gRace_bonus_headup;
extern int gWasted_flash_state;
extern int gLast_time_headup;
extern int gTime_bonus_headup;
extern int gQueued_wasted_massages[5];
extern tU32 gTime_bonus_start;
extern int gLast_credit_headup__mainloop; // suffix added to avoid duplicate symbol

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
