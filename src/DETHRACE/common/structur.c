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

// IDA: int __cdecl NumberOfOpponentsLeft()
int NumberOfOpponentsLeft() {
    int i;
    int car_count;
    int result;
    tCar_spec *the_car;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RaceCompleted(tRace_over_reason pReason@<EAX>)
void RaceCompleted(tRace_over_reason pReason) {
    LOG_TRACE("(%d)", pReason);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall Checkpoint(int pCheckpoint_index@<EAX>, int pDo_sound@<EDX>)
void Checkpoint(int pCheckpoint_index, int pDo_sound) {
    LOG_TRACE("(%d, %d)", pCheckpoint_index, pDo_sound);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl IncrementCheckpoint()
void IncrementCheckpoint() {
    int done_voice;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl IncrementLap()
void IncrementLap() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall RayHitFace@<EAX>(br_vector3 *pV0@<EAX>, br_vector3 *pV1@<EDX>, br_vector3 *pV2@<EBX>, br_vector3 *pNormal@<ECX>, br_vector3 *pStart, br_vector3 *pDir)
int RayHitFace(br_vector3* pV0, br_vector3* pV1, br_vector3* pV2, br_vector3* pNormal, br_vector3* pStart, br_vector3* pDir) {
    tFace_ref the_face;
    br_scalar rt;
    LOG_TRACE("(%p, %p, %p, %p, %p, %p)", pV0, pV1, pV2, pNormal, pStart, pDir);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall WrongCheckpoint(int pCheckpoint_index@<EAX>)
void WrongCheckpoint(int pCheckpoint_index) {
    LOG_TRACE("(%d)", pCheckpoint_index);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CheckCheckpoints()
void CheckCheckpoints() {
    tCar_spec *car;
    br_vector3 orig;
    br_vector3 dir;
    int i;
    int j;
    int cat;
    int car_count;
    int car_index;
    tNet_game_player_info *net_player;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TotalRepair()
void TotalRepair() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoLogos()
void DoLogos() {
    ClearEntireScreen();
    DoSCILogo();
    DoOpeningAnimation();
    DoSCILogo();
    gProgram_state.prog_status = eProg_opening;
}

// IDA: void __cdecl DoProgOpeningAnimation()
void DoProgOpeningAnimation() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoProgramDemo()
void DoProgramDemo() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ChooseOpponent@<EAX>(int pNastiness@<EAX>, int *pHad_scum@<EDX>)
int ChooseOpponent(int pNastiness, int* pHad_scum) {
    int i;
    int count;
    int temp_array[40];
    LOG_TRACE("(%d, %p)", pNastiness, pHad_scum);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SelectOpponents(tRace_info *pRace_info@<EAX>)
void SelectOpponents(tRace_info* pRace_info) {
    int i;
    int rank_band;
    int nastiness;
    int had_scum;
    LOG_TRACE("(%p)", pRace_info);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PickNetRace@<EAX>(int pCurrent_race@<EAX>, tNet_sequence_type pNet_race_sequence@<EDX>)
int PickNetRace(int pCurrent_race, tNet_sequence_type pNet_race_sequence) {
    int i;
    int new_index;
    int races_count;
    int most_seldom_seen;
    int races_to_pick_from[50];
    LOG_TRACE("(%d, %d)", pCurrent_race, pNet_race_sequence);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SwapNetCarsLoad()
void SwapNetCarsLoad() {
    int i;
    int switched_res;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SwapNetCarsDispose()
void SwapNetCarsDispose() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoGame()
void DoGame() {
    tSO_result options_result;
    tRace_result race_result;
    int second_select_race;
    int first_summary_done;
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitialiseProgramState()
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

// IDA: void __cdecl DoProgram()
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

// IDA: void __cdecl JumpTheStart()
void JumpTheStart() {
    char s[256];
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GoingToInterfaceFromRace()
void GoingToInterfaceFromRace() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl GoingBackToRaceFromInterface()
void GoingBackToRaceFromInterface() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
