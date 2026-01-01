#include "structur.h"
#include "car.h"
#include "controls.h"
#include "crush.h"
#include "cutscene.h"
#include "displays.h"
#include "drmem.h"
#include "finteray.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrpb.h"
#include "graphics.h"
#include "harness/config.h"
#include "harness/trace.h"
#include "init.h"
#include "loading.h"
#include "loadsave.h"
#include "mainloop.h"
#include "mainmenu.h"
#include "netgame.h"
#include "network.h"
#include "opponent.h"
#include "piping.h"
#include "pratcam.h"
#include "racestrt.h"
#include "racesumm.h"
#include "sound.h"
#include "utility.h"
#include "world.h"
#include <stdlib.h>

// GLOBAL: CARM95 0x00509a60
int gLast_wrong_checkpoint;

// GLOBAL: CARM95 0x00509a64
int gMirror_on__structur = 1; // suffix added to avoid duplicate symbol

// GLOBAL: CARM95 0x00509a68
int gPratcam_on = 1;

// GLOBAL: CARM95 0x00509a6c
int gCockpit_on = 1;

// GLOBAL: CARM95 0x00509a70
int gOpponent_mix[10][5] = {
    { 3, 4, 4, 5, 5 },
    { 2, 3, 4, 5, 5 },
    { 2, 3, 4, 4, 5 },
    { 2, 2, 4, 4, 5 },
    { 2, 2, 3, 4, 5 },
    { 1, 2, 3, 4, 4 },
    { 1, 2, 3, 3, 4 },
    { 1, 2, 2, 3, 4 },
    { 1, 1, 2, 3, 3 },
    { 1, 1, 2, 2, 3 }
};

// GLOBAL: CARM95 0x00509b38
tU32 gLast_checkpoint_time;

// GLOBAL: CARM95 0x00551dbc
tRace_over_reason gRace_over_reason;

// IDA: int __cdecl NumberOfOpponentsLeft()
// FUNCTION: CARM95 0x00413f90
int NumberOfOpponentsLeft(void) {
    int i;
    int car_count;
    int result;
    tCar_spec* the_car;

    result = 0;
    car_count = GetCarCount(eVehicle_opponent);
    for (i = 0; car_count > i; ++i) {
        the_car = GetCarSpec(eVehicle_opponent, i);
        if (!the_car->knackered) {
            result++;
        }
    }
    return result;
}

// IDA: void __usercall RaceCompleted(tRace_over_reason pReason@<EAX>)
// FUNCTION: CARM95 0x00413ffe
void RaceCompleted(tRace_over_reason pReason) {

    if (!gRace_finished) {
        if (gNet_mode == eNet_mode_host && pReason < eRace_over_network_victory) {
            NetFinishRace(gCurrent_net_game, pReason);
        }
        if (pReason == eRace_over_out_of_time || pReason == eRace_over_demo) {
            ChangeAmbientPratcam(kPratcam_out_of_time);
        } else if (pReason < eRace_over_abandoned) {
            ChangeAmbientPratcam(kPratcam_race_complete);
        }
        gRace_over_reason = pReason;
        if (gMap_mode) {
            ToggleMap();
        }
        switch (gRace_over_reason) {
        case eRace_over_network_victory:
            ChangeAmbientPratcam(kPratcam_race_complete);
            DoFancyHeadup(kFancyHeadupNetworkVictory);
            break;

        case eRace_over_network_loss:
            ChangeAmbientPratcam(kPratcam_network_timeout);
            DoFancyHeadup(kFancyHeadupNetworkRaceOverNetworkLoss);
            break;

        case eRace_over_out_of_time:
            ChangeAmbientPratcam(kPratcam_out_of_time);
            DoFancyHeadup(kFancyHeadupOutOfTime);
            DRS3StartSound(gPedestrians_outlet, 8010);
            break;

        case eRace_over_demo:
            ChangeAmbientPratcam(kPratcam_out_of_time);
            DoFancyHeadup(kFancyHeadupDemoTimeout);
            break;

        case eRace_over_laps:
        case eRace_over_peds:
        case eRace_over_opponents:
            ChangeAmbientPratcam(kPratcam_race_complete);
            DoFancyHeadup(kFancyHeadupRaceCompleted);
            DRS3StartSound(gPedestrians_outlet, 8011);
            break;

        case eRace_over_abandoned:
            if (gNet_mode == eNet_mode_client) {
                gHost_abandon_game = 1;
                NetFullScreenMessage(kMiscString_HOST_ABANDONED_RACE, 0);
            }
            // break;

        default:
            break;
        }
        if (gNet_mode != eNet_mode_none) {
            gRace_finished = 8000;
        } else {
            gRace_finished = 4000;
        }
    }
}

// IDA: void __usercall Checkpoint(int pCheckpoint_index@<EAX>, int pDo_sound@<EDX>)
// FUNCTION: CARM95 0x004141ca
void Checkpoint(int pCheckpoint_index, int pDo_sound) {

    PratcamEvent(kPratcam_checkpoint);
    DoFancyHeadup(kFancyHeadupCheckpoint);
    if (pDo_sound) {
        DRS3StartSound(gPedestrians_outlet, 8012);
    }
}

// IDA: void __cdecl IncrementCheckpoint()
// FUNCTION: CARM95 0x00414206
void IncrementCheckpoint(void) {
    int done_voice;

    done_voice = 0;
    if (gRace_finished) {
        return;
    }
    gLast_checkpoint_time = GetTotalTime();
    if (gCheckpoint >= gCheckpoint_count) {
        gCheckpoint = 1;
        if (gLap++ >= gTotal_laps) {
            gLap = gTotal_laps;
            gCheckpoint = gCheckpoint_count;
            RaceCompleted(eRace_over_laps);
        } else if (gLap == gTotal_laps) {
            PratcamEvent(kPratcam_checkpoint);
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_FinalLap));
            DRS3StartSound(gPedestrians_outlet, 8014);
            done_voice = 1;
        }
    } else {
        gCheckpoint++;
    }
    if (!gRace_finished) {
        Checkpoint(gCheckpoint, !done_voice);
        if (gCheck_point_cash[gProgram_state.skill_level] != 0) {
            EarnCredits(gCheck_point_cash[gProgram_state.skill_level]);
        }
    }
}

// IDA: void __cdecl IncrementLap()
// FUNCTION: CARM95 0x0041433c
void IncrementLap(void) {
    int i;

    for (i = gCheckpoint; i <= gCheckpoint_count; i++) {
        IncrementCheckpoint();
    }
}

// IDA: int __usercall RayHitFace@<EAX>(br_vector3 *pV0@<EAX>, br_vector3 *pV1@<EDX>, br_vector3 *pV2@<EBX>, br_vector3 *pNormal@<ECX>, br_vector3 *pStart, br_vector3 *pDir)
// FUNCTION: CARM95 0x00414835
int RayHitFace(br_vector3* pV0, br_vector3* pV1, br_vector3* pV2, br_vector3* pNormal, br_vector3* pStart, br_vector3* pDir) {
    tFace_ref the_face;
    br_scalar rt;

    the_face.material = NULL;
    memcpy(&the_face.v[0], pV0, sizeof(br_vector3));
    memcpy(&the_face.v[1], pV1, sizeof(br_vector3));
    memcpy(&the_face.v[2], pV2, sizeof(br_vector3));
    memcpy(&the_face.normal, pNormal, sizeof(br_vector3));
    CheckSingleFace(&the_face, pStart, pDir, &the_face.normal, &rt);
    return rt >= 0.f && rt <= 1.f;
}

// IDA: void __usercall WrongCheckpoint(int pCheckpoint_index@<EAX>)
// FUNCTION: CARM95 0x00414373
void WrongCheckpoint(int pCheckpoint_index) {

    if ((pCheckpoint_index == gLast_wrong_checkpoint && GetTotalTime() - gLast_checkpoint_time > 20000) || (pCheckpoint_index != gLast_wrong_checkpoint && GetTotalTime() - gLast_checkpoint_time > 2000)) {

        if (gNet_mode != eNet_mode_none || (gCheckpoint != ((pCheckpoint_index + 2 > gCurrent_race.check_point_count) ? ((gLap == 1) ? -1 : 1) : (pCheckpoint_index + 2)))) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_WrongCheckpoint));
            DRS3StartSound(gPedestrians_outlet, 8013);
            gLast_checkpoint_time = GetTotalTime();
            gLast_wrong_checkpoint = pCheckpoint_index;
        }
        // }
    }
}

// IDA: void __cdecl CheckCheckpoints()
// FUNCTION: CARM95 0x00414468
void CheckCheckpoints(void) {
    tCar_spec* car;
    br_vector3 orig;
    br_vector3 dir;
    int i;
    int j;
    int cat;
    int car_count;
    int car_index;
    tNet_game_player_info* net_player;

    if (gNet_mode == eNet_mode_client || (gNet_mode == eNet_mode_host && gCurrent_net_game->type != eNet_game_type_checkpoint && gCurrent_net_game->type != eNet_game_type_sudden_death)) {
        return;
    }
    // in single-player mode (=eNet_mode_none), only the player will be checked,
    // in multi-player, the host + clients will be tested, there are no drone opponents there
    for (cat = 0; cat <= gNet_mode; cat++) {
        if (cat == eVehicle_self) {
            car_count = 1;
        } else {
            car_count = GetCarCount(cat);
        }
        for (car_index = 0; car_index < car_count; car_index++) {
            if (cat == eVehicle_self) {
                car = &gProgram_state.current_car;
            } else {
                car = GetCarSpec(cat, car_index);
            }

            memcpy(&orig, car->old_frame_mat.m[3], sizeof(br_vector3));
            BrVector3Sub(&dir, &car->car_master_actor->t.t.translate.t, &orig);
            for (i = 0; i < gCurrent_race.check_point_count; i++) {
                for (j = 0; j < gCurrent_race.checkpoints[i].quad_count; j++) {
                    if (
                        RayHitFace(&gCurrent_race.checkpoints[i].vertices[j][0],
                            &gCurrent_race.checkpoints[i].vertices[j][1],
                            &gCurrent_race.checkpoints[i].vertices[j][2],
                            &gCurrent_race.checkpoints[i].normal[j],
                            &orig, &dir)
                        || RayHitFace(&gCurrent_race.checkpoints[i].vertices[j][0],
                            &gCurrent_race.checkpoints[i].vertices[j][2],
                            &gCurrent_race.checkpoints[i].vertices[j][3],
                            &gCurrent_race.checkpoints[i].normal[j],
                            &orig,
                            &dir)) {
                        if (gNet_mode != eNet_mode_none) {
                            net_player = NetPlayerFromCar(car);
                            if (gCurrent_net_game->type == eNet_game_type_checkpoint) {
                                if (net_player->score & (1 << i)) {
                                    net_player->score &= ~(1 << i);
                                    SendGameplay(net_player->ID, eNet_gameplay_checkpoint, i, 0, 0, 0);
                                } else {
                                    SendGameplay(net_player->ID, eNet_gameplay_wrong_checkpoint, i, 0, 0, 0);
                                }
                            } else if (net_player->score % gCurrent_race.check_point_count == i) {
                                net_player->score++;
                                SendGameplay(net_player->ID, eNet_gameplay_checkpoint, i, 0, 0, 0);
                            } else {
                                SendGameplay(net_player->ID, eNet_gameplay_wrong_checkpoint, i, 0, 0, 0);
                            }
                        } else {
                            if (i + 1 == gCheckpoint) {
                                IncrementCheckpoint();
                            } else {
                                WrongCheckpoint(i);
                            }
                        }
                        break;
                    }
                }
            }
            car->old_frame_mat = car->car_master_actor->t.t.mat;
        }
    }
}

// IDA: void __cdecl TotalRepair()
// FUNCTION: CARM95 0x004148f7
void TotalRepair(void) {

    TotallyRepairCar();
    NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_InstantRepair));
}

// IDA: void __cdecl DoLogos()
// FUNCTION: CARM95 0x00414e48
void DoLogos(void) {
    ClearEntireScreen();
    DoSCILogo();
    DoOpeningAnimation();
    DoStainlessLogo();
#ifdef DETHRACE_FIX_BUGS
    /* StartMusic is only called in PlaySmackerFile when sound and cutscenes are enabled */
    if (!gSound_override && gCut_scene_override) {
        StartMusic();
    }
#endif
    gProgram_state.prog_status = eProg_opening;
}

// IDA: void __cdecl DoProgOpeningAnimation()
// FUNCTION: CARM95 0x00414e71
void DoProgOpeningAnimation(void) {

    gProgram_state.prog_status = eProg_idling;
    DRS3StopOutletSound(gEffects_outlet);
}

// IDA: void __cdecl DoProgramDemo()
// FUNCTION: CARM95 0x00414e94
void DoProgramDemo(void) {

    DoLogos();
    gProgram_state.prog_status = eProg_idling;
    DRS3StopOutletSound(gEffects_outlet);
}

// IDA: int __usercall ChooseOpponent@<EAX>(int pNastiness@<EAX>, int *pHad_scum@<EDX>)
// FUNCTION: CARM95 0x00414a47
int ChooseOpponent(int pNastiness, int* pHad_scum) {
    int i;
    int count;
    int temp_array[40];

    count = 0;
    for (i = 0; i < gNumber_of_racers; ++i) {
        if (gOpponents[i].strength_rating == pNastiness
            && gProgram_state.current_car.index != i
            && !gOpponents[i].picked
            && (gOpponents[i].car_number >= 0 || !*pHad_scum)) {
            temp_array[count++] = i;
        }
    }
    i = temp_array[IRandomBetween(0, count - 1)];
    gOpponents[i].picked = 1;
    if (gOpponents[i].car_number < 0) {
        *pHad_scum = 1;
    }
    return i;
}

// IDA: void __usercall SelectOpponents(tRace_info *pRace_info@<EAX>)
// FUNCTION: CARM95 0x00414925
void SelectOpponents(tRace_info* pRace_info) {
    int i;
    int rank_band;
    int nastiness;
    int had_scum;

#ifdef DETHRACE_FIX_BUGS
    // Adds support for the demo behavior
    if (harness_game_info.mode == eGame_carmageddon_demo || harness_game_info.mode == eGame_splatpack_demo || harness_game_info.mode == eGame_splatpack_xmas_demo) {
        pRace_info->number_of_racers = OPPONENT_COUNT;
        for (i = 0; i < OPPONENT_COUNT; i++) {
            pRace_info->opponent_list[i].index = gDemo_opponents[i];
            pRace_info->opponent_list[i].ranking = IRandomBetween(gProgram_state.rank - 10, gProgram_state.rank + 10);
        }
        return;
    }
#endif

    had_scum = 0;
    if (gNet_mode == eNet_mode_none) {
        pRace_info->number_of_racers = OPPONENT_COUNT;
        for (i = 0; i < gNumber_of_racers; ++i) {
            gOpponents[i].picked = 0;
        }
        if (gRace_list[gProgram_state.current_race_index].suggested_rank >= 0) {
            rank_band = gRace_list[gProgram_state.current_race_index].suggested_rank / 10;
        } else {
            rank_band = 0;
        }
        for (i = 0; i < OPPONENT_COUNT; i++) {
            nastiness = gOpponent_mix[rank_band][i];
            pRace_info->opponent_list[i].index = ChooseOpponent(nastiness, &had_scum);
            pRace_info->opponent_list[i].ranking = IRandomBetween(gProgram_state.rank - 10, gProgram_state.rank + 10);
        }
    }
}

// IDA: int __usercall PickNetRace@<EAX>(int pCurrent_race@<EAX>, tNet_sequence_type pNet_race_sequence@<EDX>)
// FUNCTION: CARM95 0x00414b57
int PickNetRace(int pCurrent_race, tNet_sequence_type pNet_race_sequence) {
    int i;
    int new_index;
    int races_count;
    int most_seldom_seen;
    int races_to_pick_from[50];

    if (pNet_race_sequence == eNet_sequence_sequential) {
        pCurrent_race++;
        if (pCurrent_race >= gNumber_of_races) {
            pCurrent_race = 0;
        }
        return pCurrent_race;

    } else {
        most_seldom_seen = 10000;
        for (i = 0; i < gNumber_of_races; i++) {
            if (gRace_list[i].been_there_done_that < most_seldom_seen) {
                most_seldom_seen = gRace_list[i].been_there_done_that;
            }
        }
        races_count = 0;
        for (i = 0; i < gNumber_of_races; i++) {
            if (gRace_list[i].been_there_done_that == most_seldom_seen && (i != pCurrent_race)) {
                races_to_pick_from[races_count] = i;
                races_count++;
            }
        }
        new_index = races_to_pick_from[IRandomBetween(0, races_count - 1)];
        gRace_list[new_index].been_there_done_that++;

        return new_index;
    }
}

// IDA: void __cdecl SwapNetCarsLoad()
// FUNCTION: CARM95 0x0041542b
void SwapNetCarsLoad(void) {
    int i;
    int switched_res;

    DisableNetService();
    AboutToLoadFirstCar();
    switched_res = SwitchToRealResolution();
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].next_car_index >= 0) {
            gNet_players[i].car_index = gNet_players[i].next_car_index;
        }
        gNet_players[i].next_car_index = -1;
        LoadCar(gOpponents[gNet_players[i].car_index].car_file_name,
            (gThis_net_player_index - i == 0) ? eDriver_local_human : eDriver_net_human,
            gNet_players[i].car, gNet_players[i].car_index, gNet_players[i].player_name,
            &gNet_cars_storage_space);
    }
    if (switched_res) {
        SwitchToLoresMode();
    }
    ReenableNetService();
}

// IDA: void __cdecl SwapNetCarsDispose()
// FUNCTION: CARM95 0x0041552d
void SwapNetCarsDispose(void) {
    int i;

    DisableNetService();
    for (i = 0; i < gNumber_of_net_players; i++) {
        DisposeCar(gNet_players[i].car, gNet_players[i].car_index);
        if (gNet_players[i].car_index >= 0) {
            gCar_details[gNet_players[i].car_index].ownership = eCar_owner_none;
        }
    }
    ClearOutStorageSpace(&gOur_car_storage_space);
    ClearOutStorageSpace(&gNet_cars_storage_space);
    ReenableNetService();
}

// IDA: void __cdecl DoGame()
// FUNCTION: CARM95 0x00414ebc
void DoGame(void) {
    tSO_result options_result;
    tRace_result race_result;
    int second_select_race;
    int first_summary_done;
    int i;

    gAbandon_game = 0;
    gDisallow_abandon_race = 0;
    gCar_to_view = &gProgram_state.current_car;
    StartLoadingScreen();
    gProgram_state.prog_status = eProg_game_ongoing;
    second_select_race = 0;
    if (gNet_mode != gNet_mode_of_last_game) {
        gProgram_state.prog_status = eProg_idling;
    } else {
        PrintMemoryDump(0, "BEFORE START RACE SCREEN");
        SelectOpponents(&gCurrent_race);
        if (gNet_mode != eNet_mode_none) {
            LoadRaceInfo(gProgram_state.current_race_index, &gCurrent_race);
            FillInRaceInfo(&gCurrent_race);
            DisposeRaceInfo(&gCurrent_race);
        } else {
            do {
                options_result = DoSelectRace(&second_select_race);
                if (options_result == eSO_main_menu_invoked) {
                    DoMainMenuScreen(0, 1, 1);
                }
                gDisallow_abandon_race = 0;
            } while (options_result == eSO_main_menu_invoked && gProgram_state.prog_status == eProg_game_ongoing && !gAbandon_game);
        }
        if (gProgram_state.prog_status == eProg_game_starting
            || gProgram_state.prog_status == eProg_quit
            || gProgram_state.prog_status == eProg_idling
            || gAbandon_game) {
            PrintMemoryDump(0, "AFTER ABANDONING START RACE SCREEN");
            if (gProgram_state.prog_status == eProg_game_ongoing) {
                gProgram_state.prog_status = eProg_game_starting;
            }
        } else {
            PrintMemoryDump(0, "AFTER START RACE SCREEN");
            DoNewGameAnimation();
            StartLoadingScreen();
            if (gNet_mode == eNet_mode_none) {
                LoadOpponentsCars(&gCurrent_race);
            } else {
                if (gCurrent_net_game->options.random_car_choice
                    && (gCurrent_net_game->options.car_choice == eNet_car_all || gCurrent_net_game->options.car_choice == eNet_car_both)
                    && !gNo_races_yet) {
                    SwapNetCarsLoad();
                }
            }
            PrintMemoryDump(0, "AFTER LOADING OPPONENTS IN");
            InitRace();
            if (gNet_mode_of_last_game != gNet_mode) {
                gProgram_state.prog_status = eProg_idling;
            } else {
                if (gProgram_state.prog_status == eProg_game_starting
                    || gProgram_state.prog_status == eProg_quit
                    || gProgram_state.prog_status == eProg_idling
                    || gAbandon_game) {
                    DisposeRace();
                    if (gNet_mode == eNet_mode_none && gNet_mode_of_last_game == eNet_mode_none) {
                        DisposeOpponentsCars(&gCurrent_race);
                    }
                    DisposeTrack();
                    if (gProgram_state.prog_status == eProg_game_ongoing) {
                        gProgram_state.prog_status = eProg_game_starting;
                    }
                } else {
                    if (gNet_mode != eNet_mode_none) {
                        do {
                            options_result = NetSynchRaceStart();
                            if (options_result == eSO_main_menu_invoked) {
                                DoMainMenuScreen(0, 1, 1);
                            }
                        } while (options_result == eSO_main_menu_invoked
                            && gProgram_state.prog_status == eProg_game_ongoing
                            && !gAbandon_game);
                    } else {
                        do {
                            options_result = DoGridPosition();
                            if (options_result == eSO_main_menu_invoked) {
                                DoMainMenuScreen(0, 1, 1);
                            }
                        } while (options_result == eSO_main_menu_invoked
                            && gProgram_state.prog_status == eProg_game_ongoing
                            && !gAbandon_game);
                        SetInitialPositions(&gCurrent_race);
                    }
                    if (gProgram_state.prog_status == eProg_game_starting
                        || gProgram_state.prog_status == eProg_quit
                        || gProgram_state.prog_status == eProg_idling
                        || gAbandon_game) {
                        DisposeRace();
                        if (!gNet_mode && !gNet_mode_of_last_game) {
                            DisposeOpponentsCars(&gCurrent_race);
                        }
                        DisposeTrack();
                        if (gProgram_state.prog_status == eProg_game_ongoing) {
                            gProgram_state.prog_status = eProg_game_starting;
                        }
                    } else {
                        SwitchToRealResolution();
                        InitOpponents(&gCurrent_race);
                        InitialiseCarsEtc(&gCurrent_race);
                        SetInitialCopPositions();
                        InitSoundSources();
                        InitLastDamageArrayEtc();
                        race_result = DoRace();
                        SwitchToLoresMode();
                        DisposeRace();
                        if (gNet_mode != eNet_mode_none) {
                            gProgram_state.current_race_index = gPending_race;
                            gCurrent_net_game->start_race = gProgram_state.current_race_index;
                            gPending_race = -1;
                        }
                        if (race_result == eRace_completed || race_result == eRace_timed_out) {
                            DoEndRaceAnimation();
                            first_summary_done = 0;
                            do {
                                options_result = DoEndRaceSummary(&first_summary_done, race_result);
                                if (options_result == eSO_main_menu_invoked) {
                                    DoMainMenuScreen(0, 1, 1);
                                }
                            } while (options_result == eSO_main_menu_invoked && gProgram_state.prog_status == eProg_game_ongoing);
                        }
                        if (gNet_mode) {
                            for (i = 0; i < gNumber_of_net_players; i++) {
                                TotallyRepairACar(gNet_players[i].car);
                            }
                        } else {
                            TotallyRepairCar();
                        }
                        if (gNet_mode == eNet_mode_none) {
                            DisposeOpponentsCars(&gCurrent_race);
                        } else {
                            if (gCurrent_net_game->options.random_car_choice
                                && (gCurrent_net_game->options.car_choice == eNet_car_all
                                    || gCurrent_net_game->options.car_choice == eNet_car_both)
                                && !gNo_races_yet) {
                                SwapNetCarsDispose();
                            }
                        }
                        DisposeTrack();
#ifdef DETHRACE_FIX_BUGS
                        // demo behavior
                        if (harness_game_info.mode == eGame_carmageddon_demo || harness_game_info.mode == eGame_splatpack_demo || harness_game_info.mode == eGame_splatpack_xmas_demo) {
                            DoFullVersionPowerpoint();
                        }
#endif
                        gProgram_state.loaded = 0;
                        if (gProgram_state.prog_status == eProg_game_ongoing) {
                            gProgram_state.prog_status = eProg_game_starting;
                        }
                    }
                }
            }
        }
    }
}

// IDA: void __cdecl InitialiseProgramState()
// FUNCTION: CARM95 0x00414ca8
void InitialiseProgramState(void) {
    gProgram_state.prog_status = eProg_intro;
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
    gProgram_state.mirror_on = gMirror_on__structur;
    if (gAusterity_mode) {
        gPratcam_on = 0;
        gProgram_state.prat_cam_on = gPratcam_on;
    } else {
        gProgram_state.prat_cam_on = gPratcam_on;
    }
    gProgram_state.cockpit_on = gCockpit_on;
    gProgram_state.car_name[0] = 0;
    SetSoundVolumes();
#if !defined(DETHRACE_3DFX_PATCH)
    AllocateRearviewPixelmap();
#endif
}

// IDA: void __cdecl DoProgram()
// FUNCTION: CARM95 0x00414d8a
void DoProgram(void) {
    InitialiseProgramState();
    do {
        switch (gProgram_state.prog_status) {
        case eProg_intro:
            DisposeGameIfNecessary();
            DoLogos();
            break;
        case eProg_opening:
            DisposeGameIfNecessary();
            DoProgOpeningAnimation();
            break;
        case eProg_idling:
            DisposeGameIfNecessary();
            if (gGame_to_load >= 0) {
                DoLoadGame();
            } else {
                DoMainMenuScreen(30000u, 0, 0);
            }
            break;
        case eProg_demo:
            DoProgramDemo();
            break;
        case eProg_game_starting:
            DoGame();
            // fallthrough
        default:
            break;
        }
    } while (gProgram_state.prog_status != eProg_quit);
}

// IDA: void __cdecl JumpTheStart()
// FUNCTION: CARM95 0x004155d4
void JumpTheStart(void) {
    char s[256];

    if (gNet_mode != eNet_mode_none
        && gProgram_state.credits_earned - gProgram_state.credits_lost < gJump_start_fine[gProgram_state.skill_level]) {
        return;
    }
    WakeUpOpponentsToTheFactThatTheStartHasBeenJumped(gCountdown);
    gCountdown = 0;
    DRS3StopOutletSound(gPedestrians_outlet);
    DRS3StartSound(gPedestrians_outlet, 8016);
    SpendCredits(gJump_start_fine[gProgram_state.skill_level]);
    sprintf(s, "%s %d %s", GetMiscString(gProgram_state.frank_or_anniness == eFrankie ? kMiscString_BadBoy : kMiscString_BadGirl), gJump_start_fine[gProgram_state.skill_level], GetMiscString(kMiscString_CreditFine));
    NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, s);
}

// IDA: void __cdecl GoingToInterfaceFromRace()
// FUNCTION: CARM95 0x004156c2
void GoingToInterfaceFromRace(void) {

    gInterface_within_race_mode = 1;
    PlayFlicsFromDisk();
    SwitchToLoresMode();
    if (gNet_mode == eNet_mode_host) {
        SendGameplayToAllPlayers(eNet_gameplay_host_paused, 0, 0, 0, 0);
    }
}

// IDA: void __cdecl GoingBackToRaceFromInterface()
// FUNCTION: CARM95 0x00415700
void GoingBackToRaceFromInterface(void) {

    gInterface_within_race_mode = 0;
    PlayFlicsFromMemory();
    SwitchToRealResolution();
    if (gNet_mode == eNet_mode_host) {
        SendGameplayToAllPlayers(eNet_gameplay_host_unpaused, 0, 0, 0, 0);
    }
}
