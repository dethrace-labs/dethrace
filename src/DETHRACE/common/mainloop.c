#include "mainloop.h"
#include "brender.h"
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
#include "harness/config.h"
#include "harness/hooks.h"
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
#include "s3/s3.h"
#include "skidmark.h"
#include "sound.h"
#include "spark.h"
#include "structur.h"
#include "trig.h"
#include "utility.h"
#include "world.h"
#include <stdlib.h>

// GLOBAL: CARM95 0x00514afc
int gNasty_kludgey_cockpit_variable;

// GLOBAL: CARM95 0x00514b00
tInfo_mode gInfo_mode;

// GLOBAL: CARM95 0x00514b04
tU32 gLast_tick_count;

// GLOBAL: CARM95 0x00514b08
tU32 gActual_last_tick_count;

// GLOBAL: CARM95 0x00514b0c
tU32 gAverage_frame_period;

// GLOBAL: CARM95 0x0053a1a8
tU32 gOld_camera_time;

// GLOBAL: CARM95 0x0053a118
tU32 gLast_wasted_massage_start;

// GLOBAL: CARM95 0x0053a114
float gMr_odo;

// GLOBAL: CARM95 0x0053a0e8
tU32 gWasted_last_flash;

// GLOBAL: CARM95 0x0053a120
tTime_bonus_state gTime_bonus_state;

// GLOBAL: CARM95 0x0053a104
int gQueued_wasted_massages_count;

// GLOBAL: CARM95 0x0053a11c
int gTime_bonus;

// GLOBAL: CARM95 0x0053a1a4
int gRace_bonus_headup;

// GLOBAL: CARM95 0x0053a10c
int gWasted_flash_state;

// GLOBAL: CARM95 0x0053a110
int gLast_time_headup;

// GLOBAL: CARM95 0x0053a1a0
int gTime_bonus_headup;

// GLOBAL: CARM95 0x0053a0f0
int gQueued_wasted_massages[5];

// GLOBAL: CARM95 0x0053a124
tU32 gTime_bonus_start;

// GLOBAL: CARM95 0x0053a108
int gLast_credit_headup__mainloop; // suffix added to avoid duplicate symbol

// IDA: void __cdecl ToggleInfo()
// FUNCTION: CARM95 0x0046fd00
void ToggleInfo(void) {

    if (gProgram_state.game_completed) {
        if (KeyIsDown(KEYMAP_CONTROL_ANY)) {
            gAR_fudge_headups = !gAR_fudge_headups;
        } else {
            gInfo_on = !gInfo_on;
            if (gInfo_on) {
#ifdef DETHRACE_3DFX_PATCH
                if (PDKeyDown(KEY_SHIFT_ANY)) {
                    gInfo_mode = (gInfo_mode + 1) % 3;
                }
#else
                gInfo_mode = PDKeyDown(KEY_SHIFT_ANY);
#endif
            }
        }
    }
}

// IDA: void __cdecl CalculateFrameRate()
// FUNCTION: CARM95 0x004706fa
void CalculateFrameRate(void) {
    // GLOBAL: CARM95 0x53a0ec
    static tU32 last_time;
    tU32 new_time;
    static int last_rates[30];
    int new_rate;
    int i;

    new_time = PDGetTotalTime();
    if (new_time != last_time) {
        new_rate = 10000u / (new_time - last_time);
        gFrame_rate = new_rate;
        for (i = 0; i < COUNT_OF(last_rates); ++i) {
            gFrame_rate += last_rates[i];
        }
        gFrame_rate /= COUNT_OF(last_rates) + 1;
        for (i = 0; i < COUNT_OF(last_rates) - 1; i++) {
            last_rates[i] = last_rates[i + 1];
        }
        last_rates[29] = new_rate;
        last_time = new_time;
    }
}

// IDA: void __cdecl LoseOldestWastedMassage()
// FUNCTION: CARM95 0x0046fde6
void LoseOldestWastedMassage(void) {
    int i;

    for (i = 1; i < gQueued_wasted_massages_count; i++) {
        gQueued_wasted_massages[i - 1] = gQueued_wasted_massages[i];
    }
    gQueued_wasted_massages_count--;
    gLast_wasted_massage_start = GetTotalTime();
}

// IDA: void __usercall QueueWastedMassage(int pIndex@<EAX>)
// FUNCTION: CARM95 0x0046fd9c
void QueueWastedMassage(int pIndex) {

    if (gQueued_wasted_massages_count == COUNT_OF(gQueued_wasted_massages)) {
        LoseOldestWastedMassage();
    }
    if (gQueued_wasted_massages_count == 0) {
        gLast_wasted_massage_start = GetTotalTime();
    }
    gQueued_wasted_massages[gQueued_wasted_massages_count] = pIndex;
    gQueued_wasted_massages_count++;
}

// IDA: void __cdecl MungeHeadups()
// FUNCTION: CARM95 0x004707bd
void MungeHeadups(void) {
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
    // GLOBAL: CARM95 0x53a1ac
    static tU32 last_rattle_time;

    ClearHeadupSlot(3);
    gMr_odo = (double)gFrame_period * gProgram_state.current_car.speedo_speed * WORLD_SCALE / 1600.0 + gMr_odo;
    if (gInfo_on) {
        bearing = 360.0 - FastScalarArcTan2(gCamera_to_world.m[0][2], gCamera_to_world.m[2][2]);
        switch (gInfo_mode) {
        case 0:
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
            break;
        case 1:
            sprintf(
                the_text,
                "P'cam: curr=%d, ambi=%d, pend=%d Car: c=%+.3f, a=%+.3f, b=%+.3f",
                PratcamGetCurrent(),
                PratcamGetAmbient(),
                PratcamGetPending(),
                gCar_to_view->curvature,
                gCar_to_view->acc_force,
                gCar_to_view->brake_force);
            break;
#ifdef DETHRACE_3DFX_PATCH
        case 2:
            nearby = SomeNearbyMaterial();
            strcpy(the_text, nearby->identifier);
            if (nearby->colour_map != NULL) {
                sprintf(&the_text[strlen(the_text)], " %s", nearby->colour_map->identifier);
                user = (tPixelmap_user_data*)nearby->colour_map->user;
                if (user != NULL) {
                    sprintf(&the_text[strlen(the_text)], " %dx%d to", user->orig_width, user->orig_height);
                } else {
                    sprintf(&the_text[strlen(the_text)], " %dx%d", nearby->colour_map->width, nearby->colour_map->height);
                }
                if ((nearby->flags & BR_MATF_MAP_INTERPOLATION)) {
                    strcat(the_text, " bilinear");
                } else {
                    strcat(the_text, " nobilinear");
                }
                if ((nearby->flags & BR_MATF_MAP_ANTIALIASING) && nearby->colour_map != NULL && nearby->colour_map->mip_offset != 0) {
                    strcat(the_text, " mipmap");
                } else {
                    strcat(the_text, " nomipmap");
                }
                break;
            }
#endif
        default:
            break;
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
            NewImageHeadupSlot(eHeadupSlot_countdown, 0, 800, new_countdown + 4);
            DRS3StartSound(gPedestrians_outlet, gCountdown + 8000);
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
    if (gNet_mode != eNet_mode_none) {
        DoNetworkHeadups(net_credits);
    } else {
        if (net_credits < 0) {
            sprintf(the_text, "\xF8%d\xFA %s", -net_credits, GetMiscString(kMiscString_LOSS));
        } else {
            sprintf(the_text, "\xF8%d\xFA %s", net_credits, GetMiscString(net_credits < 100000 ? kMiscString_PROFIT : kMiscString_PRFT));
        }
        ChangeHeadupText(gCredits_won_headup, the_text);
        if (gPedestrians_on) {
            sprintf(the_text, "\xF8%d\xFA/%d %s", gProgram_state.peds_killed, gTotal_peds, GetMiscString(kMiscString_KILLS));
            ChangeHeadupText(gPed_kill_count_headup, the_text);
        } else {
            ChangeHeadupText(gPed_kill_count_headup, "");
        }
        if (gQueued_wasted_massages_count && GetTotalTime() > gLast_wasted_massage_start + 5000) {
            LoseOldestWastedMassage();
        }
        if (gQueued_wasted_massages_count) {
            if (Flash(150, &gWasted_last_flash, &gWasted_flash_state)) {
                sprintf(the_text, "\xF9%s %s", gOpponents[gQueued_wasted_massages[0]].abbrev_name, GetMiscString(kMiscString_WASTED_46));
            } else {
                sprintf(the_text, " ");
            }
        } else {
            oppo_count = GetCarCount(eVehicle_opponent);
            sprintf(the_text, "%s \xF8%d\xFA/%d", GetMiscString(kMiscString_WASTED_19), oppo_count - NumberOfOpponentsLeft(), oppo_count);
        }
        ChangeHeadupText(gCar_kill_count_headup, the_text);
        if (effective_timer > 1199000) {
            effective_timer = 1199000;
        }
        TimerString(effective_timer, the_text, 1, 0);
        ChangeHeadupText(gTimer_headup, the_text);
        sprintf(the_text, "%s \xF8%d\xFA/%d %s \xF8%d\xFA/%d", GetMiscString(kMiscString_CP), gCheckpoint, gCheckpoint_count, GetMiscString(kMiscString_LAP), gLap, gTotal_laps);
        ChangeHeadupText(gLaps_headup, the_text);
        the_time = GetTotalTime() - gTime_bonus_start;
        switch (gTime_bonus_state) {
        case eTime_bonus_initial_pause:
            if (the_time >= 500) {
                bonus = gCurrent_race.bonus_score[gRace_over_reason][gProgram_state.skill_level];
                sprintf(the_text, "%s %d", GetMiscString(kMiscString_CompletionBonusColon), bonus);
                DRS3StartSound(gPedestrians_outlet, 8015);
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
                    gTimer -= 1000 * ((the_time - last_rattle_time) / 15);
                    if (gTimer < 0) {
                        gTimer = 0;
                    }
                    gTime_bonus += (previous_gtimer - gTimer) / 1000 * gPoints_per_second[gProgram_state.skill_level];
                    last_rattle_time += 15 * ((the_time - last_rattle_time) / 15);
                }
                sprintf(the_text, "%s %d", GetMiscString(kMiscString_TimeBonus), gTime_bonus);
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
                sprintf(the_text, "%s %d", GetMiscString(kMiscString_TimeBonus), gTime_bonus);
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
// FUNCTION: CARM95 0x004712bd
void UpdateFramePeriod(tU32* pCamera_period) {
    tU32 new_tick_count;
    tU32 new_camera_tick_count;
    int error;
    // GLOBAL: CARM95 0x514b10
    static int last_AR_mode;

    if (gAction_replay_mode != last_AR_mode) {
        if (gAction_replay_mode) {
            gLast_replay_frame_time = GetTotalTime();
        } else {
            gLast_tick_count = GetTotalTime();
        }
        last_AR_mode = gAction_replay_mode;
    }
    if (gAction_replay_mode) {
        gFrame_period = abs((int)(gLast_replay_frame_time - gLast_tick_count));
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
        // The following makes the timer go too fast when the real frame rate is high (=low frame period)
#ifndef DETHRACE_FIX_BUGS
        gFrame_period = 10;
#endif
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
tU32 GetLastTickCount(void) {

    return gLast_tick_count;
}

// IDA: void __cdecl CheckTimer()
// FUNCTION: CARM95 0x00471543
void CheckTimer(void) {
    tS32 time_in_seconds;
    tS32 time_left;
    // GLOBAL: CARM95 0x514b14
    static tU32 last_time_in_seconds = 0;
    static tU32 last_demo_time_in_seconds = 0;

    if (harness_game_config.freeze_timer) {
        return;
    }

    if (!gFreeze_timer && !gCountdown && !gRace_finished) {
        if (gFrame_period < gTimer) {
            if (gNet_mode == eNet_mode_none) {
                gTimer -= gFrame_period;
            }
            time_left = gTimer + 500;
            time_in_seconds = (time_left) / 1000;
            if (time_in_seconds != last_time_in_seconds && time_in_seconds <= 10) {
                DRS3StartSound(gPedestrians_outlet, 1001);
            }
            last_time_in_seconds = time_in_seconds;
        } else {
            gTimer = 0;
            RaceCompleted(eRace_over_out_of_time);
        }

        if (harness_game_info.mode == eGame_carmageddon_demo || harness_game_info.mode == eGame_splatpack_demo || harness_game_info.mode == eGame_splatpack_xmas_demo) {
            if (harness_game_config.demo_timeout != 0) {
                time_left = harness_game_config.demo_timeout - GetRaceTime();
                time_in_seconds = (time_left + 500) / 1000;
                if (time_in_seconds != last_demo_time_in_seconds && time_in_seconds <= 10)
                    DRS3StartSound(gPedestrians_outlet, 1001);
                last_demo_time_in_seconds = time_in_seconds;
                if (time_left <= 0) {
                    gTimer = 0;
                    RaceCompleted(eRace_over_demo);
                }
            }
        }
    }
}

// IDA: int __cdecl MungeRaceFinished()
// FUNCTION: CARM95 0x00471607
int MungeRaceFinished(void) {

    if (!gRace_finished || gAction_replay_mode || (gNet_mode != eNet_mode_none && gRace_over_reason == eRace_not_over_yet)) {
        return 0;
    }
    if (gRace_finished > gFrame_period) {
        gRace_finished -= gFrame_period;
    } else {
        if (!gTimer || gNet_mode != eNet_mode_none) {
            gRace_finished = 0;
            return 1;
        }
        gRace_finished = 15 * gTimer + 4500;
        gRace_bonus_headup = NewTextHeadupSlot(eHeadupSlot_race_bonus, 0, 0, -4, "");
        gTime_bonus_headup = NewTextHeadupSlot(eHeadupSlot_time_bonus, 0, 0, -4, "");
        gTime_bonus = 0;
        gTime_bonus_start = GetTotalTime();
        gTime_bonus_state = eTime_bonus_initial_pause;
    }
    return PDKeyDown(KEY_RETURN) || PDKeyDown(KEY_KP_ENTER);
}

// IDA: tRace_result __cdecl MainGameLoop()
// FUNCTION: CARM95 0x0046fe77
tRace_result MainGameLoop(void) {
    tU32 camera_period;
    tU32 start_menu_time;
    tU32 frame_start_time;
    tRace_result result;
    int tried_to_allocate_AR;
    int i;
    int bonus;

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
    gRace_over_reason = eRace_not_over_yet;
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
        gOur_pos = &gSelf->t.t.translate.t;
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
#if defined(DETHRACE_FIX_BUGS)
            // Fixes issue where returning to race mode from the UI shows 2d elements in the wrong colors for half a second.
            // In 3dfx mode, 2d elements are rendered using `Copy8BitTo16BitRectangleWithTransparency` which uses
            // `gCurrent_palette` to convert 8 bit to 16 bit pixels. `gCurrent_palette` is still set to the interface palette here
            // I couldn't confirm why this does not happen in the original 3dfx executable (or does it?)
            if (harness_game_config.opengl_3dfx_mode) {
                EnsureRenderPalette();
                EnsurePaletteUp();
            }
#endif
            RenderAFrame(1);
        }
        CheckReplayTurnOn();
        if (!gRecover_car
            && gProgram_state.prog_status == eProg_game_ongoing
            && !gPalette_fade_time
            && (gNet_mode == eNet_mode_none
                || !gAction_replay_mode
                || gProgram_state.current_car.car_master_actor->t.t.mat.m[3][0] < 500.0)) {

            EnsureRenderPalette();
            EnsurePaletteUp();
        }
        DoNetGameManagement();
        if (KeyIsDown(KEYMAP_ESCAPE) && !gEntering_message) {
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
    // From splatpack x-mas demo
    if (gArrow_mode) {
        ToggleArrow();
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
        // FIXME: gRace_over_reason can be -1 eRace_not_over_yet (=-1) when aborting a race
        bonus = gCurrent_race.bonus_score[gRace_over_reason][gProgram_state.skill_level];
        gProgram_state.credits_earned += bonus;
    }
    if (gNet_mode != eNet_mode_none) {
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
// FUNCTION: CARM95 0x0046fe4f
tRace_result DoRace(void) {
    tRace_result result;

    gRace_start = GetTotalTime();
    result = MainGameLoop();
    return result;
}
