#include "structur.h"
#include "common/cutscene.h"
#include "common/globvars.h"
#include "common/graphics.h"
#include "common/init.h"
#include "common/loadsave.h"
#include "common/mainmenu.h"
#include "common/sound.h"
#include <stdlib.h>

int gOpponent_mix[10][5];
int gPratcam_on = 1;
tRace_over_reason gRace_over_reason;
int gCockpit_on = 1;
tU32 gLast_checkpoint_time;
int gLast_wrong_checkpoint;
int gMirror_on = 1;

// Offset: 0
// Size: 124
int NumberOfOpponentsLeft() {
    int car_count;
    int result;
    tCar_spec* the_car;
    NOT_IMPLEMENTED();
}

// Offset: 124
// Size: 419
// EAX: pReason
void RaceCompleted(tRace_over_reason pReason) {
    NOT_IMPLEMENTED();
}

// Offset: 544
// Size: 79
// EAX: pCheckpoint_index
// EDX: pDo_sound
void Checkpoint(int pCheckpoint_index, int pDo_sound) {
    NOT_IMPLEMENTED();
}

// Offset: 624
// Size: 305
void IncrementCheckpoint() {
    NOT_IMPLEMENTED();
}

// Offset: 932
// Size: 70
void IncrementLap() {
    NOT_IMPLEMENTED();
}

// Offset: 1004
// Size: 152
// EAX: pV0
// EDX: pV1
// EBX: pV2
// ECX: pNormal
int RayHitFace(br_vector3* pV0, br_vector3* pV1, br_vector3* pV2, br_vector3* pNormal, br_vector3* pStart, br_vector3* pDir) {
    tFace_ref the_face;
    br_scalar rt;
    NOT_IMPLEMENTED();
}

// Offset: 1156
// Size: 241
// EAX: pCheckpoint_index
void WrongCheckpoint(int pCheckpoint_index) {
    NOT_IMPLEMENTED();
}

// Offset: 1400
// Size: 870
void CheckCheckpoints() {
    tCar_spec* car;
    br_vector3 orig;
    br_vector3 dir;
    int i;
    int j;
    int cat;
    int car_count;
    int car_index;
    tNet_game_player_info* net_player;
    NOT_IMPLEMENTED();
}

// Offset: 2272
// Size: 72
void TotalRepair() {
    NOT_IMPLEMENTED();
}

// Offset: 2344
// Size: 64
void DoLogos() {
    NOT_IMPLEMENTED();
}

// Offset: 2408
// Size: 54
void DoProgOpeningAnimation() {
    NOT_IMPLEMENTED();
}

// Offset: 2464
// Size: 59
void DoProgramDemo() {
    NOT_IMPLEMENTED();
}

// Offset: 2524
// Size: 308
// EAX: pNastiness
// EDX: pHad_scum
int ChooseOpponent(int pNastiness, int* pHad_scum) {
    int i;
    int count;
    int temp_array[40];
    NOT_IMPLEMENTED();
}

// Offset: 2832
// Size: 294
// EAX: pRace_info
void SelectOpponents(tRace_info* pRace_info) {
    int i;
    int rank_band;
    int nastiness;
    int had_scum;
    NOT_IMPLEMENTED();
}

// Offset: 3128
// Size: 269
// EAX: pCurrent_race
// EDX: pNet_race_sequence
int PickNetRace(int pCurrent_race, tNet_sequence_type pNet_race_sequence) {
    int i;
    int new_index;
    int races_count;
    int most_seldom_seen;
    int races_to_pick_from[50];
    NOT_IMPLEMENTED();
}

// Offset: 3400
// Size: 288
void SwapNetCarsLoad() {
    int switched_res;
    NOT_IMPLEMENTED();
}

// Offset: 3688
// Size: 165
void SwapNetCarsDispose() {
    NOT_IMPLEMENTED();
}

// Offset: 3856
// Size: 1161
void DoGame() {
    tRace_result race_result;
    int second_select_race;
    int first_summary_done;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 5020
// Size: 242
void InitialiseProgramState() {
    gProgram_state.loaded = 0;
    gProgram_state.last_slot = 0;
    gProgram_state.frank_or_anniness = eFrankie;
    gProgram_state.skill_level = 1;
    gProgram_state.view_type = eVT_Scene;
    gProgram_state.auto_parts_reply = eAP_auto;
    gProgram_state.racing = 0;
    gProgram_state.cut_scene = 0;
    gProgram_state.saving = 0;
    gProgram_state.loading = 0;
    gProgram_state.dont_save_or_load = 0;
    gProgram_state.dont_load = 0;
    gProgram_state.mirror_on = gMirror_on;
    gProgram_state.prog_status = eProg_intro;
    if (gAusterity_mode) {
        gProgram_state.prat_cam_on = 0;
        gPratcam_on = 0;
    } else {
        gProgram_state.prat_cam_on = gPratcam_on;
    }
    gProgram_state.cockpit_on = gCockpit_on;
    gProgram_state.car_name[0] = 0;
    SetSoundVolumes();
    AllocateRearviewPixelmap();
}

// Offset: 5264
// Size: 171
void DoProgram() {
    InitialiseProgramState();
    while (1) {
        switch (gProgram_state.prog_status) {
        case eProg_intro:
            DisposeGameIfNecessary();
            ClearEntireScreen();
            DoSCILogo();
            DoOpeningAnimation();
            DoSCILogo();
            gProgram_state.prog_status = eProg_opening;
            continue;
        case eProg_opening:
            DisposeGameIfNecessary();
            gProgram_state.prog_status = eProg_idling;
            DRS3StopOutletSound(gIndexed_outlets[0]);
            if (gProgram_state.prog_status == eProg_quit) {
                return;
            }
            continue;
        case eProg_idling:
            DisposeGameIfNecessary();
            if (gGame_to_load >= 0) {
                DoLoadGame(0);
                if (gProgram_state.prog_status == eProg_quit) {
                    return;
                }
                continue;
            }
            DoMainMenuScreen(30000, 0, 0);
            if (gProgram_state.prog_status != eProg_quit) {
                continue;
            }
            return;
        case eProg_demo:
            DoLogos();
            gProgram_state.prog_status = eProg_idling;
            DRS3StopOutletSound(gIndexed_outlets[0]);
            if (gProgram_state.prog_status == eProg_quit) {
                return;
            }
            continue;
        case eProg_game_starting:
            DoGame();
            if (gProgram_state.prog_status == eProg_quit) {
                return;
            }
            continue;
        case eProg_game_ongoing:
        case eProg_quit:
            // JeffH added to avoid compiler warnings about missing case handling
            LOG_PANIC("We do not expect to get here");
        }
    }
}

// Offset: 5436
// Size: 270
void JumpTheStart() {
    NOT_IMPLEMENTED();
}

// Offset: 5708
// Size: 81
void GoingToInterfaceFromRace() {
    NOT_IMPLEMENTED();
}

// Offset: 5792
// Size: 81
void GoingBackToRaceFromInterface() {
    NOT_IMPLEMENTED();
}
