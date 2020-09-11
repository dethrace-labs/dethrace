#include "mainloop.h"
#include <stdlib.h>

tU32 gOld_camera_time;
tU32 gActual_last_tick_count;
tU32 gLast_wasted_massage_start;
float gMr_odo;
tU32 gWasted_last_flash;
tU32 gLast_tick_count;
tU32 gAverage_frame_period;
tTime_bonus_state gTime_bonus_state;
int gQueued_wasted_massages_count;
int gTime_bonus;
int gRace_bonus_headup;
int gWasted_flash_state;
int gLast_time_headup;
int gNasty_kludgey_cockpit_variable;
tInfo_mode gInfo_mode;
int gTime_bonus_headup;
int gQueued_wasted_massages[5];
tU32 gTime_bonus_start;
int gLast_credit_headup;

// IDA: void __cdecl ToggleInfo()
void ToggleInfo() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CalculateFrameRate()
void CalculateFrameRate() {
    static tU32 last_time;
    tU32 new_time;
    static int last_rates[30];
    int new_rate;
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl LoseOldestWastedMassage()
void LoseOldestWastedMassage() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall QueueWastedMassage(int pIndex@<EAX>)
void QueueWastedMassage(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl MungeHeadups()
void MungeHeadups() {
    char the_text[256];
    int flash_rate;
    int new_countdown;
    int net_credits;
    int previous_gtimer;
    int previous_time_bonus;
    int effective_timer;
    int bonus;
    int oppo_count;
    tU32 the_time;
    float bearing;
    br_material* nearby;
    tPixelmap_user_data* user;
    static tU32 last_rattle_time;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall UpdateFramePeriod(tU32 *pCamera_period@<EAX>)
void UpdateFramePeriod(tU32* pCamera_period) {
    tU32 new_tick_count;
    tU32 new_camera_tick_count;
    int error;
    static int last_AR_mode;
    LOG_TRACE("(%p)", pCamera_period);
    NOT_IMPLEMENTED();
}

// IDA: tU32 __cdecl GetLastTickCount()
tU32 GetLastTickCount() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CheckTimer()
void CheckTimer() {
    tS32 time_in_seconds;
    tS32 time_left;
    static tU32 last_time_in_seconds;
    static tU32 last_demo_time_in_seconds;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl MungeRaceFinished()
int MungeRaceFinished() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tRace_result __cdecl MainGameLoop()
tRace_result MainGameLoop() {
    tU32 camera_period;
    tU32 start_menu_time;
    tU32 frame_start_time;
    tRace_result result;
    int tried_to_allocate_AR;
    int i;
    int bonus;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tRace_result __cdecl DoRace()
tRace_result DoRace() {
    tRace_result result;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
