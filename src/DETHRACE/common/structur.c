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
#include <stdlib.h>

int gLast_wrong_checkpoint;
int gMirror_on__structur = 1; // suffix added to avoid duplicate symbol
int gPratcam_on = 1;
int gCockpit_on = 1;
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
tU32 gLast_checkpoint_time;
tRace_over_reason gRace_over_reason;

// IDA: int __cdecl NumberOfOpponentsLeft()
int NumberOfOpponentsLeft() {
    int i;
    int car_count;
    int result;
    tCar_spec* the_car;
    LOG_TRACE("()");

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
void RaceCompleted(tRace_over_reason pReason) {
    LOG_TRACE("(%d)", pReason);

    if (!gRace_finished) {
        if (gNet_mode == eNet_mode_host && pReason < eRace_over_network_victory) {
            NetFinishRace(gCurrent_net_game, pReason);
        }
        if (pReason == eRace_over_out_of_time || pReason == eRace_over_demo) {
            ChangeAmbientPratcam(35);
        } else if (pReason < eRace_over_abandoned) {
            ChangeAmbientPratcam(34);
        }
        gRace_over_reason = pReason;
        if (gMap_mode) {
            ToggleMap();
        }
        switch (gRace_over_reason) {
        case eRace_over_laps:
        case eRace_over_peds:
        case eRace_over_opponents:
            ChangeAmbientPratcam(34);
            DoFancyHeadup(14);
            DRS3StartSound(gIndexed_outlets[4], 8011);
            break;
        case eRace_over_abandoned:
            if (gNet_mode == eNet_mode_client) {
                gHost_abandon_game = 1;
                NetFullScreenMessage(87, 0);
            }
            break;
        case eRace_over_out_of_time:
            ChangeAmbientPratcam(35);
            DoFancyHeadup(13);
            DRS3StartSound(gIndexed_outlets[4], 8010);
            break;
        case eRace_over_demo:
            ChangeAmbientPratcam(35);
            DoFancyHeadup(21);
            break;
        case eRace_over_network_victory:
            ChangeAmbientPratcam(34);
            DoFancyHeadup(20);
            break;
        case eRace_over_network_loss:
            ChangeAmbientPratcam(36);
            DoFancyHeadup(17);
            break;
        default:
            break;
        }
        if (gNet_mode) {
            gRace_finished = 8000;
        } else {
            gRace_finished = 4000;
        }
    }
}

// IDA: void __usercall Checkpoint(int pCheckpoint_index@<EAX>, int pDo_sound@<EDX>)
void Checkpoint(int pCheckpoint_index, int pDo_sound) {
    LOG_TRACE("(%d, %d)", pCheckpoint_index, pDo_sound);

    PratcamEvent(33);  // FIXME: or PratcamEventNow
    DoFancyHeadup(12);
    if (pDo_sound) {
        DRS3StartSound(gIndexed_outlets[4], 8012);
    }
}

// IDA: void __cdecl IncrementCheckpoint()
void IncrementCheckpoint() {
    int done_voice;
    LOG_TRACE("()");

    done_voice = 0;
    if (gRace_finished) {
        return;
    }
    gLast_checkpoint_time = GetTotalTime();
    if (gCheckpoint < gCheckpoint_count) {
        gCheckpoint++;
    } else {
        gCheckpoint = 1;
        gLap++;
        if (gLap == gTotal_laps) {
            PratcamEvent(33);  // FIXME: or PratcamEventNow
            NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(42));
            DRS3StartSound(gIndexed_outlets[4], 8014);
            done_voice = 1;
        } else if (gLap > gTotal_laps) {
            gLap = gTotal_laps;
            gCheckpoint = gCheckpoint_count;
            RaceCompleted(eRace_over_laps);
        }
    }
    if (!gRace_finished) {
        Checkpoint(gCheckpoint, !done_voice);
        if (gCheck_point_cash[gProgram_state.skill_level] != 0) {
            EarnCredits(gCheck_point_cash[gProgram_state.skill_level]);
        }
    }
}

// IDA: void __cdecl IncrementLap()
void IncrementLap() {
    int i;
    LOG_TRACE("()");

    for (i = gCheckpoint; i <= gCheckpoint_count; i++) {
        IncrementCheckpoint();
    }
}

// IDA: int __usercall RayHitFace@<EAX>(br_vector3 *pV0@<EAX>, br_vector3 *pV1@<EDX>, br_vector3 *pV2@<EBX>, br_vector3 *pNormal@<ECX>, br_vector3 *pStart, br_vector3 *pDir)
int RayHitFace(br_vector3* pV0, br_vector3* pV1, br_vector3* pV2, br_vector3* pNormal, br_vector3* pStart, br_vector3* pDir) {
    tFace_ref the_face;
    br_scalar rt;
    LOG_TRACE("(%p, %p, %p, %p, %p, %p)", pV0, pV1, pV2, pNormal, pStart, pDir);

    the_face.material = NULL;
    BrVector3Copy(&the_face.v[0], pV0);
    BrVector3Copy(&the_face.v[1], pV1);
    BrVector3Copy(&the_face.v[2], pV2);
    BrVector3Copy(&the_face.normal, pNormal);
    CheckSingleFace(&the_face, pStart, pDir, &the_face.normal, &rt);
    return rt >= 0.f && rt <= 1.f;
}

// IDA: void __usercall WrongCheckpoint(int pCheckpoint_index@<EAX>)
void WrongCheckpoint(int pCheckpoint_index) {
    LOG_TRACE("(%d)", pCheckpoint_index);

    if ((pCheckpoint_index == gLast_wrong_checkpoint && GetTotalTime() - gLast_checkpoint_time > 20000) ||
            (pCheckpoint_index != gLast_wrong_checkpoint && GetTotalTime() - gLast_checkpoint_time > 2000)) {
        if (gNet_mode == eNet_mode_none) {
            if (gCheckpoint == ((gCurrent_race.check_point_count < pCheckpoint_index + 2) ? ((gLap == 1) ? -1 : 1) : (pCheckpoint_index + 2))) {
                return;
            }
        }
        NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(43));
        DRS3StartSound(gIndexed_outlets[4], 8013);
        gLast_checkpoint_time = GetTotalTime();
        gLast_wrong_checkpoint = pCheckpoint_index;
    }
}

// IDA: void __cdecl CheckCheckpoints()
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
    LOG_TRACE("()");

    if (gNet_mode == eNet_mode_client) {
        return;
    }
    if (gNet_mode == eNet_mode_host && gCurrent_net_game->type != eNet_game_type_checkpoint && gCurrent_net_game->type != eNet_game_type_sudden_death) {
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
            BrVector3Copy(&orig, (br_vector3*)car->old_frame_mat.m[3]);
            BrVector3Sub(&dir, &car->car_master_actor->t.t.translate.t, &orig);
            for (i = 0; i < gCurrent_race.check_point_count; i++) {
                for (j = 0; j < gCurrent_race.checkpoints[i].quad_count; j++) {
                    if (
                            RayHitFace(&gCurrent_race.checkpoints[i].vertices[j][0],
                                &gCurrent_race.checkpoints[i].vertices[j][1],
                                &gCurrent_race.checkpoints[i].vertices[j][2],
                                &gCurrent_race.checkpoints[i].normal[j],
                                &orig, &dir) ||
                            RayHitFace(&gCurrent_race.checkpoints[i].vertices[j][0],
                                &gCurrent_race.checkpoints[i].vertices[j][2],
                                &gCurrent_race.checkpoints[i].vertices[j][3],
                                &gCurrent_race.checkpoints[i].normal[j],
                                &orig,
                                &dir)) {
                        if (gNet_mode == eNet_mode_none) {
                            if (i + 1 == gCheckpoint) {
                                IncrementCheckpoint();
                            } else {
                                WrongCheckpoint(i);
                            }
                        } else {
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
                        }
                        break;
                    }

                }
            }
        }
        car->old_frame_mat = car->car_master_actor->t.t.mat;
    }
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
    DoStainlessLogo();
    gProgram_state.prog_status = eProg_opening;
}

// IDA: void __cdecl DoProgOpeningAnimation()
void DoProgOpeningAnimation() {
    LOG_TRACE("()");

    gProgram_state.prog_status = eProg_idling;
    DRS3StopOutletSound(gIndexed_outlets[0]);
}

// IDA: void __cdecl DoProgramDemo()
void DoProgramDemo() {
    LOG_TRACE("()");

    DoLogos();
    gProgram_state.prog_status = eProg_idling;
    DRS3StopOutletSound(gIndexed_outlets[0]);
}

// IDA: int __usercall ChooseOpponent@<EAX>(int pNastiness@<EAX>, int *pHad_scum@<EDX>)
int ChooseOpponent(int pNastiness, int* pHad_scum) {
    int i;
    int count;
    int temp_array[40];
    LOG_TRACE("(%d, %p)", pNastiness, pHad_scum);

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
void SelectOpponents(tRace_info* pRace_info) {
    int i;
    int rank_band;
    int nastiness;
    int had_scum;
    LOG_TRACE("(%p)", pRace_info);

    if (harness_game_info.mode == eGame_carmageddon_demo || harness_game_info.mode == eGame_splatpack_demo) {
        pRace_info->number_of_racers = OPPONENT_COUNT;
        for (i = 0; i < OPPONENT_COUNT; i++) {
            pRace_info->opponent_list[i].index = gDemo_opponents[i];
            pRace_info->opponent_list[i].ranking = IRandomBetween(gProgram_state.rank - 10, gProgram_state.rank + 10);
        }
        return;
    }

    had_scum = 0;
    if (gNet_mode == eNet_mode_none) {
        pRace_info->number_of_racers = OPPONENT_COUNT;
        for (i = 0; i < gNumber_of_racers; ++i) {
            gOpponents[i].picked = 0;
        }
        if (gRace_list[gProgram_state.current_race_index].suggested_rank < 0) {
            rank_band = 0;
        } else {
            rank_band = gRace_list[gProgram_state.current_race_index].suggested_rank / 10;
        }
        for (i = 0; i < OPPONENT_COUNT; i++) {
            nastiness = gOpponent_mix[rank_band][i];
            pRace_info->opponent_list[i].index = ChooseOpponent(nastiness, &had_scum);
            pRace_info->opponent_list[i].ranking = IRandomBetween(gProgram_state.rank - 10, gProgram_state.rank + 10);
        }
    }
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

    DisableNetService();
    AboutToLoadFirstCar();
    switched_res = SwitchToRealResolution();
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].next_car_index >= 0) {
            gNet_players[i].car_index = gNet_players[i].next_car_index;
        }
        gNet_players[i].next_car_index = -1;
        LoadCar(gOpponents[gNet_players[i].car_index].car_file_name,
            (gThis_net_player_index == i) ? eDriver_local_human : eDriver_net_human,
            gNet_players[i].car, gNet_players[i].car_index, gNet_players[i].player_name,
            &gNet_cars_storage_space);
    }
    if (switched_res) {
        SwitchToLoresMode();
    }
    ReenableNetService();
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

    gAbandon_game = 0;
    gDisallow_abandon_race = 0;
    gCar_to_view = &gProgram_state.current_car;
    StartLoadingScreen();
    gProgram_state.prog_status = eProg_game_ongoing;
    second_select_race = 0;
    if (gNet_mode_of_last_game == gNet_mode) {
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
            if (gNet_mode != eNet_mode_none) {
                if (gCurrent_net_game->options.random_car_choice
                    && (gCurrent_net_game->options.car_choice == eNet_car_all || gCurrent_net_game->options.car_choice == eNet_car_both)
                    && !gNo_races_yet) {
                    SwapNetCarsLoad();
                }
            } else {
                LoadOpponentsCars(&gCurrent_race);
            }
            PrintMemoryDump(0, "AFTER LOADING OPPONENTS IN");
            InitRace();
            if (gNet_mode_of_last_game == gNet_mode) {
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
                        if (gNet_mode) {
                            gProgram_state.current_race_index = gPending_race;
                            gCurrent_net_game->start_race = gPending_race;
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
                        if (gNet_mode) {
                            if (gCurrent_net_game->options.random_car_choice
                                && (gCurrent_net_game->options.car_choice == eNet_car_all
                                    || gCurrent_net_game->options.car_choice == eNet_car_both)
                                && !gNo_races_yet) {
                                SwapNetCarsDispose();
                            }
                        } else {
                            DisposeOpponentsCars(&gCurrent_race);
                        }
                        DisposeTrack();
                        if (harness_game_info.mode == eGame_carmageddon_demo) {
                            DoFullVersionPowerpoint();
                        }
                        gProgram_state.loaded = 0;
                        if (gProgram_state.prog_status == eProg_game_ongoing) {
                            gProgram_state.prog_status = eProg_game_starting;
                        }
                    }
                }
            } else {
                gProgram_state.prog_status = eProg_idling;
            }
        }
    } else {
        gProgram_state.prog_status = eProg_idling;
    }
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
    gProgram_state.mirror_on = gMirror_on__structur;
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
    while (gProgram_state.prog_status != eProg_quit) {
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
            if (gGame_to_load < 0) {
                DoMainMenuScreen(30000u, 0, 0);
            } else {
                DoLoadGame();
            }
            break;
        case eProg_demo:
            DoProgramDemo();
            break;
        case eProg_game_starting:
            DoGame();
            break;
        default:
            break;
        }
    }
}

// IDA: void __cdecl JumpTheStart()
void JumpTheStart() {
    char s[256];
    LOG_TRACE("()");

    if (gNet_mode == eNet_mode_none
        || gProgram_state.credits_earned - gProgram_state.credits_lost >= gJump_start_fine[gProgram_state.skill_level]) {
        WakeUpOpponentsToTheFactThatTheStartHasBeenJumped(gCountdown);
        gCountdown = 0;
        DRS3StopOutletSound(gIndexed_outlets[4]);
        DRS3StartSound(gIndexed_outlets[4], 8016);
        SpendCredits(gJump_start_fine[gProgram_state.skill_level]);
        sprintf(s, "%s %d %s", GetMiscString(gProgram_state.frank_or_anniness == eFrankie ? 44 : 49), gJump_start_fine[gProgram_state.skill_level], GetMiscString(45));
        NewTextHeadupSlot(4, 0, 1000, -4, s);
    }
}

// IDA: void __cdecl GoingToInterfaceFromRace()
void GoingToInterfaceFromRace() {
    LOG_TRACE("()");

    gInterface_within_race_mode = 1;
    PlayFlicsFromDisk();
    SwitchToLoresMode();
    if (gNet_mode == eNet_mode_host) {
        SendGameplayToAllPlayers(eNet_gameplay_host_paused, 0, 0, 0, 0);
    }
}

// IDA: void __cdecl GoingBackToRaceFromInterface()
void GoingBackToRaceFromInterface() {
    LOG_TRACE("()");

    gInterface_within_race_mode = 0;
    PlayFlicsFromMemory();
    SwitchToRealResolution();
    if (gNet_mode == eNet_mode_host) {
        SendGameplayToAllPlayers(eNet_gameplay_host_unpaused, 0, 0, 0, 0);
    }
}
