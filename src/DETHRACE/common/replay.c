#include "replay.h"
#include <stdlib.h>

#include "common/loading.h"

int gProgress_line_top[2];
int gProgress_line_left[2];
int gProgress_line_right[2];
br_pixelmap* gReplay_pixies[10];
char* gReplay_pixie_names[10] = {
    "REPLAY.PIX",
    "RBUTTONS.PIX",
    "REWSTART.PIX",
    "REW.PIX",
    "REVPLAY.PIX",
    "PAUSE.PIX",
    "PLAY.PIX",
    "FFWD.PIX",
    "FWDEND.PIX",
    "CAMERA.PIX"
};
int gKey_down;
int gNo_cursor;
int gSingle_frame_mode;
int gSave_frame_number;
int gCam_change_button_down;
tU32 gAction_replay_start_time;
tU32 gLast_replay_zappy_screen;
tS32 gStopped_time;
tU32 gCam_change_time;
float gPending_replay_rate;
tU32 gAction_replay_end_time;
float gReplay_rate;
int gSave_file;
int gSave_bunch_ID;
int gPlay_direction;
int gPaused;
tAction_replay_camera_type gAction_replay_camera_mode;

// Offset: 0
// Size: 71
int ReplayIsPaused() {
    NOT_IMPLEMENTED();
}

// Offset: 72
// Size: 47
float GetReplayRate() {
    NOT_IMPLEMENTED();
}

// Offset: 120
// Size: 45
int GetReplayDirection() {
    NOT_IMPLEMENTED();
}

// Offset: 168
// Size: 54
void StopSaving() {
    NOT_IMPLEMENTED();
}

// Offset: 224
// Size: 1597
void ActualActionReplayHeadups(int pSpecial_zappy_bastard) {
    tU32 the_time;
    int x;
    tU16 played_col1;
    tU16 played_col2;
    tU16 to_play_col1;
    tU16 to_play_col2;
    NOT_IMPLEMENTED();
}

// Offset: 1824
// Size: 44
void DoActionReplayPostSwap() {
    NOT_IMPLEMENTED();
}

// Offset: 1868
// Size: 109
void DoZappyActionReplayHeadups(int pSpecial_zappy_bastard) {
    tU32 the_time;
    NOT_IMPLEMENTED();
}

// Offset: 1980
// Size: 41
void DoActionReplayHeadups() {
    NOT_IMPLEMENTED();
}

// Offset: 2024
// Size: 318
void MoveReplayBuffer(tS32 pMove_amount) {
    tU8* play_ptr;
    tU8* old_play_ptr;
    tU8* old_play_ptr2;
    int i;
    int a;
    tU32 old_time;
    NOT_IMPLEMENTED();
}

// Offset: 2344
// Size: 118
void MoveToEndOfReplay() {
    NOT_IMPLEMENTED();
}

// Offset: 2464
// Size: 118
void MoveToStartOfReplay() {
    NOT_IMPLEMENTED();
}

// Offset: 2584
// Size: 395
void ToggleReplay() {
    NOT_IMPLEMENTED();
}

// Offset: 2980
// Size: 49
void ReverseSound(tS3_effect_tag pEffect_index, tS3_sound_tag pSound_tag) {
    NOT_IMPLEMENTED();
}

// Offset: 3032
// Size: 223
int FindUniqueFile() {
    FILE* f;
    tPath_name the_path;
    NOT_IMPLEMENTED();
}

// Offset: 3256
// Size: 1576
void PollActionReplayControls(tU32 pFrame_period) {
    float old_replay_rate;
    int old_key_down;
    int x_coord;
    int y_coord;
    int i;
    tU32 real_time;
    static tU32 last_real_time;
    static int psuedo_mouse_keys[8];
    static tRectangle mouse_areas[2][8];
    NOT_IMPLEMENTED();
}

// Offset: 4832
// Size: 94
void CheckReplayTurnOn() {
    NOT_IMPLEMENTED();
}

// Offset: 4928
// Size: 98
void InitializeActionReplay() {
    int i;
    LOG_TRACE("()");
    for (int i = 0; i < 10; i++) {
        gReplay_pixies[i] = LoadPixelmap(gReplay_pixie_names[i]);
    }
    gAction_replay_camera_mode = eAction_replay_action;
}

// Offset: 5028
// Size: 71
void DoActionReplay(tU32 pFrame_period) {
    NOT_IMPLEMENTED();
}

// Offset: 5100
// Size: 330
void SynchronizeActionReplay() {
    tPath_name the_path;
    static tU32 gLast_synch_time;
    NOT_IMPLEMENTED();
}
