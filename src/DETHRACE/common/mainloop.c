#include "mainloop.h"
#include "brender/brender.h"
#include "car.h"
#include "controls.h"
#include "crush.h"
#include "depth.h"
#include "displays.h"
#include "drmem.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrkm.h"
#include "globvrpb.h"
#include "graphics.h"
#include "harness/trace.h"
#include "input.h"
#include "main.h"
#include "mainmenu.h"
#include "netgame.h"
#include "network.h"
#include "oil.h"
#include "opponent.h"
#include "pd/sys.h"
#include "pedestrn.h"
#include "piping.h"
#include "powerup.h"
#include "pratcam.h"
#include "replay.h"
#include "s3/s3sound.h"
#include "skidmark.h"
#include "sound.h"
#include "spark.h"
#include "structur.h"
#include "trig.h"
#include "utility.h"
#include "world.h"
#include <stdlib.h>

int gNasty_kludgey_cockpit_variable;
tInfo_mode gInfo_mode;
tU32 gLast_tick_count;
tU32 gActual_last_tick_count;
tU32 gAverage_frame_period;
tU32 gOld_camera_time;
tU32 gLast_wasted_massage_start;
float gMr_odo;
tU32 gWasted_last_flash;
tTime_bonus_state gTime_bonus_state;
int gQueued_wasted_massages_count;
int gTime_bonus;
int gRace_bonus_headup;
int gWasted_flash_state;
int gLast_time_headup;
int gTime_bonus_headup;
int gQueued_wasted_massages[5];
tU32 gTime_bonus_start;
int gLast_credit_headup__mainloop; // suffix added to avoid duplicate symbol

// IDA: void __cdecl ToggleInfo()
void ToggleInfo() {
    LOG_TRACE("()");

    if (gProgram_state.game_completed) {
        if (KeyIsDown(KEYMAP_LCTRL)) {
            gAR_fudge_headups = gAR_fudge_headups == 0;
        } else {
            gInfo_on = gInfo_on == 0;
            if (gInfo_on) {
                gInfo_mode = PDKeyDown(KEY_LSHIFT);
            }
        }
    }
}

// IDA: void __cdecl CalculateFrameRate()
void CalculateFrameRate() {
    static tU32 last_time;
    tU32 new_time;
    static int last_rates[30];
    int new_rate;
    int i;
    LOG_TRACE("()");

    new_time = PDGetTotalTime();
    if (new_time != last_time) {
        new_rate = 10000u / (new_time - last_time);
        gFrame_rate = new_rate;
        for (i = 0; i < 30; ++i) {
            gFrame_rate += last_rates[i];
        }
        gFrame_rate /= 31;
        for (i = 0; i < 29; i++) {
            last_rates[i] = last_rates[i + 1];
        }
        last_rates[29] = new_rate;
        last_time = new_time;
    }
}

// IDA: void __cdecl LoseOldestWastedMassage()
void LoseOldestWastedMassage() {
    int i;
    LOG_TRACE("()");

    for (i = 1; i < gQueued_wasted_massages_count; i++) {
        gQueued_wasted_massages[0] = gQueued_wasted_massages[i];
    }
    gQueued_wasted_massages_count--;
    gLast_wasted_massage_start = GetTotalTime();
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

    ClearHeadupSlot(3);
    gMr_odo = (double)gFrame_period * gProgram_state.current_car.speedo_speed * 6.9000001 / 1600.0 + gMr_odo;
    if (gInfo_on) {
        bearing = 360.0 - FastScalarArcTan2(gCamera_to_world.m[0][2], gCamera_to_world.m[2][2]);
        if (gInfo_mode) {
            sprintf(
                the_text,
                "P'cam: curr=%d, ambi=%d, pend=%d Car: c=%+.3f, a=%+.3f, b=%+.3f",
                PratcamGetCurrent(),
                PratcamGetAmbient(),
                PratcamGetPending(),
                gCar_to_view->curvature,
                gCar_to_view->acc_force,
                gCar_to_view->brake_force);
        } else {
            sprintf(
                the_text,
                "%d.%d (%.3f, %.3f, %.3f) %.0f, %.0f, MILES=%.2f",
                gFrame_rate / 10,
                gFrame_rate % 10,
                gSelf->t.t.translate.t.v[0],
                gSelf->t.t.translate.t.v[1],
                gSelf->t.t.translate.t.v[2],
                gCamera_to_horiz_angle,
                bearing,
                gMr_odo);
        }
        ChangeHeadupText(gProgram_state.frame_rate_headup, the_text);
    } else {
        ChangeHeadupText(gProgram_state.frame_rate_headup, "");
    }
    net_credits = gProgram_state.credits_earned - gProgram_state.credits_lost;
    if (fabs((double)(gProgram_state.credits_earned - gProgram_state.credits_lost) - (double)gLast_credit_headup__mainloop) / (double)gFrame_period > 1.2) {
        if (net_credits - gLast_credit_headup__mainloop <= 0) {
            net_credits = (double)gLast_credit_headup__mainloop
                - ((double)(gLast_credit_headup__mainloop - net_credits) + 1000.0)
                    * (double)gFrame_period
                    * 1.2
                    / 1000.0;
        } else {
            net_credits = (net_credits - gLast_credit_headup__mainloop) + 1000.0 * (double)gFrame_period * 1.2 / 1000.0
                + (double)gLast_credit_headup__mainloop;
        }
    }
    gLast_credit_headup__mainloop = net_credits;
    if (gCountdown) {
        new_countdown = 7.5 - (double)GetRaceTime() / 1000.0;
        if (new_countdown < 0) {
            new_countdown = 0;
        }
        if (gCountdown != new_countdown && new_countdown <= 5) {
            gCountdown = new_countdown;
            NewImageHeadupSlot(5, 0, 800, new_countdown + 4);
            DRS3StartSound(gIndexed_outlets[4], gCountdown + 8000);
            if (!new_countdown) {
                MakeFlagWavingBastardWaveHisFlagWhichIsTheProbablyTheLastThingHeWillEverDo();
            }
        }
    }
    if (fabs((double)gTimer - (double)gLast_time_headup) / (double)gFrame_period <= 10.0) {
        effective_timer = gTimer;
    } else if (gTimer - gLast_time_headup <= 0) {
        effective_timer = gTimer;
    } else {
        effective_timer = gFrame_period * 10.0 + gLast_time_headup;
    }
    gLast_time_headup = effective_timer;
    if (gNet_mode) {
        DoNetworkHeadups(net_credits);
    } else {
        if (net_credits < 0) {
            sprintf(the_text, "\xF8%d\xFA %s", -net_credits, GetMiscString(17));
        } else {
            sprintf(the_text, "\xF8%d\xFA %s", net_credits, GetMiscString(net_credits < 100000 ? 16 : 47));
        }
        ChangeHeadupText(gCredits_won_headup, the_text);
        if (gPedestrians_on) {
            sprintf(the_text, "\xF8%d\xFA/%d %s", gProgram_state.peds_killed, gTotal_peds, GetMiscString(18));
            ChangeHeadupText(gPed_kill_count_headup, the_text);
        } else {
            ChangeHeadupText(gPed_kill_count_headup, "");
        }
        if (gQueued_wasted_massages_count && GetTotalTime() > gLast_wasted_massage_start + 5000) {
            LoseOldestWastedMassage();
        }
        if (gQueued_wasted_massages_count) {
            if (Flash(150, &gWasted_last_flash, &gWasted_flash_state)) {
                sprintf(the_text, "\xF9%s %s", gOpponents[gQueued_wasted_massages[0]].abbrev_name, GetMiscString(46));
            } else {
                sprintf(the_text, " ");
            }
        } else {
            oppo_count = GetCarCount(eVehicle_opponent);
            sprintf(the_text, "%s \xF8%d\xFA/%d", GetMiscString(19), oppo_count - NumberOfOpponentsLeft(), oppo_count);
        }
        ChangeHeadupText(gCar_kill_count_headup, the_text);
        if (effective_timer > 1199000) {
            effective_timer = 1199000;
        }
        TimerString(effective_timer, the_text, 1, 0);
        ChangeHeadupText(gTimer_headup, the_text);
        sprintf(the_text, "%s \xF8%d\xFA/%d %s \xF8%d\xFA/%d", GetMiscString(21), gCheckpoint, gCheckpoint_count, GetMiscString(20), gLap, gTotal_laps);
        ChangeHeadupText(gLaps_headup, the_text);
        the_time = GetTotalTime() - gTime_bonus_start;
        switch (gTime_bonus_state) {
        case eTime_bonus_initial_pause:
            if (the_time >= 500) {
                bonus = gCurrent_race.bonus_score[gRace_over_reason][gProgram_state.skill_level];
                sprintf(the_text, "%s %d", GetMiscString(22), bonus);
                DRS3StartSound(gIndexed_outlets[4], 8015);
                ChangeHeadupText(gRace_bonus_headup, the_text);
                gProgram_state.credits_earned += bonus;
                gTime_bonus_state = eTime_bonus_race_bonus;
                gTime_bonus_start = GetTotalTime();
            }
            gRace_finished = 10000;
            break;
        case eTime_bonus_race_bonus:
            if (the_time >= 2000) {
                gTime_bonus_state = eTime_bonus_tb_up;
                gTime_bonus_start = GetTotalTime();
                last_rattle_time = 0;
            }
            gRace_finished = 10000;
            break;
        case eTime_bonus_tb_up:
            if (gTimer) {
                if (the_time - last_rattle_time > 15) {
                    previous_gtimer = gTimer;
                    gTimer -= 1000 * ((the_time - last_rattle_time) / 0xF);
                    if (gTimer < 0) {
                        gTimer = 0;
                    }
                    gTime_bonus += (previous_gtimer - gTimer) / 1000 * gPoints_per_second[gProgram_state.skill_level];
                    last_rattle_time += 15 * ((the_time - last_rattle_time) / 15);
                }
                sprintf(the_text, "%s %d", GetMiscString(23), gTime_bonus);
                ChangeHeadupText(gTime_bonus_headup, the_text);
            } else {
                gTime_bonus_state = eTime_bonus_tb_pause;
                gTime_bonus_start = GetTotalTime();
                last_rattle_time = 0;
            }
            gRace_finished = 10000;
            break;
        case eTime_bonus_tb_pause:
            if (the_time >= 1000) {
                gTime_bonus_state = eTime_bonus_tb_down;
                gTime_bonus_start = GetTotalTime();
                last_rattle_time = 0;
            }
            gRace_finished = 10000;
            break;
        case eTime_bonus_tb_down:
            if (gTime_bonus) {
                if (the_time - last_rattle_time > 15) {
                    bonus = gTime_bonus;
                    gTime_bonus -= (the_time - last_rattle_time) * gPoints_per_second[gProgram_state.skill_level] / 15;
                    if (gTime_bonus < 0) {
                        gTime_bonus = 0;
                    }
                    gProgram_state.credits_earned += bonus - gTime_bonus;
                    last_rattle_time += 15 * ((the_time - last_rattle_time) / 15);
                }
                sprintf(the_text, "%s %d", GetMiscString(23), gTime_bonus);
                ChangeHeadupText(gTime_bonus_headup, the_text);
            } else {
                gTime_bonus_state = eTime_bonus_end_pause;
                gTime_bonus_start = GetTotalTime();
            }
            gRace_finished = 10000;
            break;
        case eTime_bonus_end_pause:
            if (the_time >= 2000 && gRace_finished > 1) {
                gRace_finished = 1;
            }
            break;
        default:
            return;
        }
    }
}

// IDA: void __usercall UpdateFramePeriod(tU32 *pCamera_period@<EAX>)
void UpdateFramePeriod(tU32* pCamera_period) {
    tU32 new_tick_count;
    tU32 new_camera_tick_count;
    int error;
    static int last_AR_mode;
    LOG_TRACE("(%p)", pCamera_period);

    if (gAction_replay_mode != last_AR_mode) {
        if (gAction_replay_mode) {
            gLast_replay_frame_time = GetTotalTime();
        } else {
            gLast_tick_count = GetTotalTime();
        }
        last_AR_mode = gAction_replay_mode;
    }
    if (gAction_replay_mode) {
        // Was (removed abs to avoid compiler warning) gFrame_period = abs(gLast_replay_frame_time - gLast_tick_count);
        gFrame_period = gLast_replay_frame_time - gLast_tick_count;
        gLast_tick_count = gLast_replay_frame_time;
        new_camera_tick_count = PDGetTotalTime();
        new_tick_count = GetTotalTime();
        if (gOld_camera_time) {
            *pCamera_period = new_camera_tick_count - gOld_camera_time;
        } else {
            *pCamera_period = 0;
        }
        gOld_camera_time = new_camera_tick_count;
        if (gFrame_period) {
            *pCamera_period = gFrame_period;
        }
    } else {
        new_tick_count = GetTotalTime();
        gLast_tick_count += gFrame_period;
        error = new_tick_count - gLast_tick_count;
        gFrame_period = new_tick_count - gActual_last_tick_count;
        if ((new_tick_count - gActual_last_tick_count) > 500 && new_tick_count - gRace_start > 2000) {
            gFrame_period = gAverage_frame_period / 10;
            gLast_tick_count = new_tick_count;
            if (gNet_mode) {
                if (gNet_mode == eNet_mode_client) {
                    gProgram_state.current_car.last_car_car_collision = 0;
                }
            }
        }
        gAverage_frame_period = 9 * gAverage_frame_period / 10 + gFrame_period;
        if ((new_tick_count - gRace_start) > 2000) {
            gFrame_period = gAverage_frame_period / 10;
        }
        if ((int)(error + gFrame_period) > 0) {
            gFrame_period += error;
        } else {
            gLast_tick_count = new_tick_count;
        }
        *pCamera_period = gFrame_period;
        gActual_last_tick_count = new_tick_count;
    }
    if (gFrame_period >= 10) {
        if (gFrame_period > 1000) {
            gFrame_period = 1000;
            gLast_tick_count = new_tick_count;
        }
    } else {
        gFrame_period = 10;
        gLast_tick_count = new_tick_count;
    }
    if (*pCamera_period >= 10) {
        if (*pCamera_period > 1000) {
            *pCamera_period = 1000;
        }
    } else {
        *pCamera_period = 10;
    }
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
    static tU32 last_time_in_seconds = 0;
    static tU32 last_demo_time_in_seconds = 0;
    LOG_TRACE("()");

    if (!gFreeze_timer && !gCountdown && !gRace_finished) {
        if (gFrame_period < gTimer) {
            if (gNet_mode == eNet_mode_none) {
                gTimer -= gFrame_period;
            }
            time_left = gTimer + 500;
            time_in_seconds = (time_left) / 1000;
            if (time_in_seconds != last_time_in_seconds && time_in_seconds <= 10) {
                DRS3StartSound(gIndexed_outlets[4], 1001);
            }
            last_time_in_seconds = time_in_seconds;
        } else {
            gTimer = 0;
            RaceCompleted(eRace_over_out_of_time);
        }
    }
}

// IDA: int __cdecl MungeRaceFinished()
int MungeRaceFinished() {
    LOG_TRACE("()");

    if (!gRace_finished || gAction_replay_mode || (gNet_mode && gRace_over_reason == -1)) {
        return 0;
    }
    if (gRace_finished > gFrame_period) {
        gRace_finished -= gFrame_period;
    } else {
        if (!gTimer || gNet_mode) {
            gRace_finished = 0;
            return 1;
        }
        gRace_finished = 15 * gTimer + 4500;
        gRace_bonus_headup = NewTextHeadupSlot(9, 0, 0, -4, "");
        gTime_bonus_headup = NewTextHeadupSlot(10, 0, 0, -4, "");
        gTime_bonus = 0;
        gTime_bonus_start = GetTotalTime();
        gTime_bonus_state = eTime_bonus_initial_pause;
    }
    return PDKeyDown(KEY_RETURN) || PDKeyDown(KEY_KP_ENTER);
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

    tried_to_allocate_AR = 0;
    gCar_to_view = &gProgram_state.current_car;
    gOld_camera_time = 0;
    ClearEntireScreen();
    ResetPalette();
    gLast_credit_headup__mainloop = 0;
    gLast_time_headup = gTimer;
    gRace_bonus_headup = -1;
    gTime_bonus_headup = -1;
    gTime_bonus_start = 0;
    gTime_bonus_state = eTime_bonus_none;
    gLast_replay_frame_time = PDGetTotalTime();
    gLast_tick_count = GetTotalTime();
    gActual_last_tick_count = gLast_tick_count;
    gQueued_wasted_massages_count = 0;
    gWasted_flash_state = 0;
    gWasted_last_flash = 0;
    RevertPalette();
    gHost_abandon_game = 0;
    gNo_races_yet = 0;
    gRace_over_reason = -1;
    gMr_odo = 0.0;
    gReceived_game_scores = 0;
    ShowSpecialVolumesIfRequ();
    gProgram_state.racing = 1;
    if (gNet_mode == eNet_mode_host) {
        gCurrent_net_game->status.stage = eNet_game_playing;
    }
    NetPlayerStatusChanged(ePlayer_status_racing);
    GetAverageGridPosition(&gCurrent_race);
    ForceRebuildActiveCarList();
    PrintMemoryDump(0, "ABOUT TO ENTER MAINLOOP");

    double last_time = GetTotalTime() / 1000.0;

    do {
        frame_start_time = GetTotalTime();
        CyclePollKeys();
        CheckSystemKeys(1);
        NetReceiveAndProcessMessages();
        if (gHost_abandon_game || gProgram_state.prog_status == eProg_idling) {
            break;
        }
        if (gNet_mode && gMap_mode
            && ((gCurrent_net_game->type == eNet_game_type_foxy && gThis_net_player_index == gIt_or_fox)
                || (gCurrent_net_game->type == eNet_game_type_tag && gThis_net_player_index != gIt_or_fox))) {
            ToggleMap();
        }
        ResetGrooveFlags();
        MungeEngineNoise();
        ServiceGameInRace();
        EnterUserMessage();
        UpdateFramePeriod(&camera_period);
        if (!gAction_replay_mode) {
            DoPowerupPeriodics(gFrame_period);
        }
        ResetLollipopQueue();
        if (!gAction_replay_mode) {
            MungeOpponents(gFrame_period);
            PollCarControls(gFrame_period);
        }
        PollCameraControls(camera_period);
        if (gAction_replay_mode) {
            DoActionReplay(gFrame_period);
        } else {
            ControlOurCar(gFrame_period);
            ApplyPhysicsToCars(gLast_tick_count - gRace_start, gFrame_period);
            PipeCarPositions();
            NetSendMessageStacks();
            CheckRecoveryOfCars(gFrame_period + gLast_tick_count - gRace_start);
        }
        if (!gNasty_kludgey_cockpit_variable) {
            gNasty_kludgey_cockpit_variable = 1;
            ToggleCockpit();
        }
        gOur_pos = &gSelf->t.t.euler.t;
        PositionExternalCamera(&gProgram_state.current_car, camera_period);
        BrActorToActorMatrix34(&gCamera_to_world, gCamera, gUniverse_actor);
        BrActorToActorMatrix34(&gRearview_camera_to_world, gRearview_camera, gUniverse_actor);
        gCamera_to_horiz_angle = FastScalarArcTan2(gCamera_to_world.m[2][1], gCamera_to_world.m[1][1]);
        gYon_squared = ((br_camera*)gCamera->type_data)->yon_z * ((br_camera*)gCamera->type_data)->yon_z
            * gYon_multiplier
            * gYon_multiplier;
        if (!gAction_replay_mode) {
            CheckCheckpoints();
        }
        ChangingView();
        MungeCarGraphics(gFrame_period);
        FunkThoseTronics();
        GrooveThoseDelics();
        DoWheelDamage(gFrame_period);
        CalculateFrameRate();
        MungePedestrians(gFrame_period);
        CameraBugFix(&gProgram_state.current_car, camera_period);
        if (!gAction_replay_mode) {
            MungeHeadups();
            ProcessOilSpills(gFrame_period);
        }
        MungeShrapnel(gFrame_period);
        ChangeDepthEffect();
        ServiceGameInRace();
        EnterUserMessage();
        SkidsPerFrame();
        if (!gWait_for_it) {
            RenderAFrame(1);
        }
        CheckReplayTurnOn();
        if (!gRecover_car
            && gProgram_state.prog_status == eProg_game_ongoing
            && !gRecover_timer
            && (gNet_mode == eNet_mode_none
                || !gAction_replay_mode
                || gProgram_state.current_car.car_master_actor->t.t.mat.m[3][0] < 500.0)) {
            EnsureRenderPalette();
            EnsurePaletteUp();
        }
        DoNetGameManagement();
        if (KeyIsDown(0) && !gEntering_message) {
            WaitForNoKeys();
            if (gAction_replay_mode) {
                ToggleReplay();
            } else {
                start_menu_time = PDGetTotalTime();
                FadePaletteDown();
                ClearEntireScreen();
                GoingToInterfaceFromRace();
                DoMainMenuScreen(0, 0, 1);
                GoingBackToRaceFromInterface();
                AddLostTime(PDGetTotalTime() - start_menu_time);
            }
        }
        if (gAction_replay_mode) {
            PollActionReplayControls(gFrame_period);
        } else {
            CheckTimer();
        }
        if (!gAction_replay_mode && gKnobbled_frame_period) {
            while (GetTotalTime() - frame_start_time < gKnobbled_frame_period) {
                ;
            }
        }
        if (!tried_to_allocate_AR) {
            tried_to_allocate_AR = 1;
            PrintMemoryDump(0, "JUST RENDERED 1ST STUFF");
            InitialisePiping();
            PrintMemoryDump(0, "JUST ALLOCATED ACTION REPLAY BUFFER");
        }
        if (gNet_mode == eNet_mode_client && gAbandon_game) {
            gProgram_state.prog_status = eProg_idling;
            gAbandon_game = 0;
        }

        // Added to lock framerate to 30fps. Seems to help physics be less twitchy...
        double secs = GetTotalTime() / 1000.0;
        //LOG_DEBUG("timediff %f", secs - (last_time + (1.0 / 30.0)));
        while (secs < last_time + (1.0 / 30.0)) {
            //LOG_DEBUG("skipping time...");
            secs = GetTotalTime() / 1000.0;
        }
        last_time += 1.0 / 30.0;

    } while (gProgram_state.prog_status == eProg_game_ongoing
        && !MungeRaceFinished()
        && !gAbandon_game
        && !gHost_abandon_game);
    PrintMemoryDump(0, "JUST EXITED MAINLOOP");
    FadePaletteDown();
    ClearEntireScreen();
    SuspendPendingFlic();
    RevertPalette();
    if (gMap_mode) {
        ToggleMap();
    }
    EnsureSpecialVolumesHidden();
    FadePaletteDown();
    NetPlayerStatusChanged(ePlayer_status_loading);
    if (gProgram_state.prog_status == eProg_game_ongoing) {
        ResetCarScreens();
    }
    if (gAbandon_game && gNet_mode == eNet_mode_host && gRace_over_reason < eRace_over_network_victory) {
        NetFinishRace(gCurrent_net_game, eRace_over_abandoned);
    }
    if (gAction_replay_mode) {
        ToggleReplay();
    }

    if (gHost_abandon_game) {
        result = eRace_game_abandonned;
    } else if (gProgram_state.prog_status == eProg_game_ongoing) {
        if (gAbandon_game) {
            result = eRace_aborted;
        } else if (gRace_over_reason == eRace_over_out_of_time || gRace_over_reason == eRace_over_demo) {
            result = eRace_timed_out;
        } else {
            result = eRace_completed;
        }
    } else {
        result = eRace_game_abandonned;
    }
    if (result >= eRace_completed) {
        gProgram_state.redo_race_index = -1;
    } else {
        gProgram_state.redo_race_index = gProgram_state.current_race_index;
    }
    gAbandon_game = 0;
    gSynch_race_start = 0;
    gInitialised_grid = 0;
    gHost_abandon_game = 0;
    S3StopAllOutletSounds();
    if (gTime_bonus_state > eTime_bonus_none) {
        gTime_bonus += gTimer / 1000 * gPoints_per_second[gProgram_state.skill_level];
        gProgram_state.credits_earned += gTime_bonus;
    }
    if (gTime_bonus_state < eTime_bonus_race_bonus) {
        bonus = gCurrent_race.bonus_score[gRace_over_reason][gProgram_state.skill_level];
        gProgram_state.credits_earned += bonus;
    }
    if (gNet_mode) {
        for (i = 0; i < gNumber_of_net_players; i++) {
            StopCarBeingIt(gNet_players[i].car);
        }
    }
    gProgram_state.racing = 0;
    if (gNet_mode == eNet_mode_host) {
        gCurrent_net_game->status.stage = eNet_game_starting;
    }
    WaitForNoKeys();
    if (gNet_mode && gAbandon_game) {
        gProgram_state.prog_status = eProg_idling;
    }
    return result;
}

// IDA: tRace_result __cdecl DoRace()
tRace_result DoRace() {
    tRace_result result;
    LOG_TRACE("()");

    gRace_start = GetTotalTime();
    result = MainGameLoop();
    return result;
}
