#include "mainloop.h"

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

// Offset: 0
// Size: 161
void ToggleInfo() {
}

// Offset: 164
// Size: 220
void CalculateFrameRate() {
    tU32 new_time;
    static int last_rates[30];
    int new_rate;
    int i;
}

// Offset: 384
// Size: 104
void LoseOldestWastedMassage() {
}

// Offset: 488
// Size: 97
// EAX: pIndex
void QueueWastedMassage(int pIndex) {
}

// Offset: 588
// Size: 2796
void MungeHeadups() {
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
}

// Offset: 3384
// Size: 588
// EAX: pCamera_period
void UpdateFramePeriod(tU32* pCamera_period) {
    tU32 new_tick_count;
    tU32 new_camera_tick_count;
    int error;
    static int last_AR_mode;
}

// Offset: 3972
// Size: 45
tU32 GetLastTickCount() {
}

// Offset: 4020
// Size: 192
void CheckTimer() {
    tS32 time_left;
    static tU32 last_time_in_seconds;
    static tU32 last_demo_time_in_seconds;
}

// Offset: 4212
// Size: 319
int MungeRaceFinished() {
}

// Offset: 4532
// Size: 1868
tRace_result MainGameLoop() {
    tU32 camera_period;
    tU32 start_menu_time;
    tU32 frame_start_time;
    tRace_result result;
    int tried_to_allocate_AR;
    int i;
    int bonus;
}

// Offset: 6400
// Size: 61
tRace_result DoRace() {
}
