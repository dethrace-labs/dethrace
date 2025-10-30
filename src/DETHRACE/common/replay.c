#include "replay.h"
#include "brender.h"
#include "brhton.h"
#include "car.h"
#include "controls.h"
#include "displays.h"
#include "globvars.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "graphics.h"
#include "input.h"
#include "loading.h"
#include "main.h"
#include "netgame.h"
#include "oil.h"
#include "opponent.h"
#include "piping.h"
#include "s3/s3.h"
#include "sys.h"
#include "utility.h"
#include "world.h"

#include "harness/config.h"
#include "harness/os.h"
#include "harness/trace.h"

#include <stdio.h>
#include <stdlib.h>

// GLOBAL: CARM95 0x0050a1b0
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
    "CAMERA.PIX",
};

// GLOBAL: CARM95 0x0050a1d8
int gSingle_frame_mode = 0;

// GLOBAL: CARM95 0x0050a1dc
tU32 gCam_change_time = 0;

// GLOBAL: CARM95 0x0050a1e0
int gSave_file = 0;

// GLOBAL: CARM95 0x0050a1e8
int gProgress_line_left[2] = { 70, 141 };

// GLOBAL: CARM95 0x0050a1f0
int gProgress_line_right[2] = { 279, 558 };

// GLOBAL: CARM95 0x0050a1f8
int gProgress_line_top[2] = { 178, 429 };

// GLOBAL: CARM95 0x00531dd0
br_pixelmap* gReplay_pixies[10];

// GLOBAL: CARM95 0x00531db8
int gKey_down;
int gNo_cursor;

// GLOBAL: CARM95 0x00531db4
int gSave_frame_number;

// GLOBAL: CARM95 0x00531da8
int gCam_change_button_down;

// GLOBAL: CARM95 0x00531da0
tU32 gAction_replay_start_time;

// GLOBAL: CARM95 0x00531dc4
tU32 gLast_replay_zappy_screen;

// GLOBAL: CARM95 0x00531dc0
tS32 gStopped_time;

// GLOBAL: CARM95 0x00531dbc
float gPending_replay_rate;

// GLOBAL: CARM95 0x00531da4
tU32 gAction_replay_end_time;

// GLOBAL: CARM95 0x00531dcc
float gReplay_rate;

// GLOBAL: CARM95 0x00531db0
int gSave_bunch_ID;

// GLOBAL: CARM95 0x00531dac
int gPlay_direction;

// GLOBAL: CARM95 0x00531dc8
int gPaused;

// GLOBAL: CARM95 0x00551db4
tAction_replay_camera_type gAction_replay_camera_mode;

// IDA: int __cdecl ReplayIsPaused()
// FUNCTION: CARM95 0x0041adc0
int ReplayIsPaused(void) {

    return gReplay_rate == 0.f;
}

// IDA: float __cdecl GetReplayRate()
// FUNCTION: CARM95 0x0041adf3
float GetReplayRate(void) {

    return gReplay_rate;
}

// IDA: int __cdecl GetReplayDirection()
// FUNCTION: CARM95 0x0041ae09
int GetReplayDirection(void) {

    return gPlay_direction;
}

// IDA: void __cdecl StopSaving()
// FUNCTION: CARM95 0x0041b5a0
void StopSaving(void) {

    gSave_file = 0;
    gPaused = 1;
}

// IDA: void __usercall ActualActionReplayHeadups(int pSpecial_zappy_bastard@<EAX>)
// FUNCTION: CARM95 0x0041ae48
void ActualActionReplayHeadups(int pSpecial_zappy_bastard) {
    tU32 the_time;
    int x;
    // dethrace: these are unused, present in DOS build but removed in the windows build
    // tU16 played_col1;
    // tU16 played_col2;
    // tU16 to_play_col1;
    // tU16 to_play_col2;

    the_time = PDGetTotalTime();
    if (!gSave_file && !PDKeyDown(KEY_SHIFT_ANY)) {

        if ((the_time / 400) % 2) {
            DRMaskedStamp(gCurrent_graf_data->action_replay_R_x,
                gCurrent_graf_data->action_replay_R_y,
                gReplay_pixies[0]);
        }
        DRMaskedStamp(gCurrent_graf_data->action_replay_controls_x,
            gCurrent_graf_data->action_replay_controls_y,
            gReplay_pixies[1]);
        if (pSpecial_zappy_bastard < 0) {
            DRMaskedStamp(gCurrent_graf_data->action_replay_rew_start_x,
                gCurrent_graf_data->action_replay_hilite_y,
                gReplay_pixies[2]);
        } else if (pSpecial_zappy_bastard > 0) {
            DRMaskedStamp(gCurrent_graf_data->action_replay_fwd_end_x,
                gCurrent_graf_data->action_replay_hilite_y,
                gReplay_pixies[8]);
        } else {
            if (gReplay_rate < -1.f) {
                DRMaskedStamp(gCurrent_graf_data->action_replay_rew_x,
                    gCurrent_graf_data->action_replay_hilite_y,
                    gReplay_pixies[3]);
            } else if (gReplay_rate > 1.f) {
                DRMaskedStamp(gCurrent_graf_data->action_replay_ffwd_x,
                    gCurrent_graf_data->action_replay_hilite_y,
                    gReplay_pixies[7]);
            } else if (gReplay_rate == 1.f) {
                DRMaskedStamp(gCurrent_graf_data->action_replay_play_x,
                    gCurrent_graf_data->action_replay_hilite_y,
                    gReplay_pixies[6]);
            } else if (gReplay_rate == -1.f) {
                DRMaskedStamp(gCurrent_graf_data->action_replay_rev_play_x,
                    gCurrent_graf_data->action_replay_hilite_y,
                    gReplay_pixies[4]);
            } else {
                DRMaskedStamp(gCurrent_graf_data->action_replay_pause_x,
                    gCurrent_graf_data->action_replay_hilite_y,
                    gReplay_pixies[5]);
            }
        }

        x = (int)((gLast_replay_frame_time - gAction_replay_start_time)
                / (double)(gAction_replay_end_time - gAction_replay_start_time)
                * (gProgress_line_right[gGraf_data_index] - gProgress_line_left[gGraf_data_index]))
            + gProgress_line_left[gGraf_data_index];

        if (x > gProgress_line_left[gGraf_data_index]) {
            BrPixelmapLine(gBack_screen,
                gProgress_line_left[gGraf_data_index], gProgress_line_top[gGraf_data_index],
                x - 1, gProgress_line_top[gGraf_data_index],
                2);
            BrPixelmapLine(gBack_screen,
                gProgress_line_left[gGraf_data_index], gProgress_line_top[gGraf_data_index] + 1,
                x - 1, gProgress_line_top[gGraf_data_index] + 1,
                4);
            BrPixelmapLine(gBack_screen,
                gProgress_line_left[gGraf_data_index], gProgress_line_top[gGraf_data_index] + 2,
                x - 1, gProgress_line_top[gGraf_data_index] + 2,
                2);
        }
        if (x < gProgress_line_right[gGraf_data_index]) {
            BrPixelmapLine(gBack_screen,
                x, gProgress_line_top[gGraf_data_index],
                gProgress_line_right[gGraf_data_index] - 1, gProgress_line_top[gGraf_data_index],
                81);
            BrPixelmapLine(gBack_screen,
                x, gProgress_line_top[gGraf_data_index] + 1,
                gProgress_line_right[gGraf_data_index] - 1, gProgress_line_top[gGraf_data_index] + 1,
                82);
            BrPixelmapLine(gBack_screen,
                x, gProgress_line_top[gGraf_data_index] + 2,
                gProgress_line_right[gGraf_data_index] - 1, gProgress_line_top[gGraf_data_index] + 2,
                81);
        }
        BrPixelmapLine(gBack_screen,
            gProgress_line_left[gGraf_data_index] - 1, gProgress_line_top[gGraf_data_index],
            gProgress_line_left[gGraf_data_index] - 1, gProgress_line_top[gGraf_data_index] + 2,
            2);
        BrPixelmapLine(gBack_screen,
            gProgress_line_right[gGraf_data_index], gProgress_line_top[gGraf_data_index],
            gProgress_line_right[gGraf_data_index], gProgress_line_top[gGraf_data_index] + 2,
            81);
        if (gCam_change_button_down) {
            DRMaskedStamp(gCurrent_graf_data->action_replay_camera_x,
                gCurrent_graf_data->action_replay_hilite_y,
                gReplay_pixies[9]);
        }
        if (the_time - gCam_change_time < 2000) {
            TransDRPixelmapText(gBack_screen,
                gCurrent_graf_data->action_replay_cam_text_x - DRTextWidth(&gFonts[kFont_ORANGHED], GetMiscString(kMiscString_StandardCamera + gAction_replay_camera_mode)),
                gCurrent_graf_data->action_replay_cam_text_y,
                &gFonts[kFont_ORANGHED],
                GetMiscString(kMiscString_StandardCamera + gAction_replay_camera_mode),
                2 * gCurrent_graf_data->action_replay_cam_text_x);
        }
        TurnOnPaletteConversion();
        DoMouseCursor();
        TurnOffPaletteConversion();
    }
}

// IDA: void __cdecl DoActionReplayPostSwap()
// FUNCTION: CARM95 0x0041ae1e
void DoActionReplayPostSwap(void) {

    RemoveTransientBitmaps(1);
}

// IDA: void __usercall DoZappyActionReplayHeadups(int pSpecial_zappy_bastard@<EAX>)
// FUNCTION: CARM95 0x0041b5bf
void DoZappyActionReplayHeadups(int pSpecial_zappy_bastard) {
    tU32 the_time;

    the_time = PDGetTotalTime();
    // Draw screen every 50ms (when we are going fast)
    if (abs(pSpecial_zappy_bastard) > 10000 && the_time - gLast_replay_zappy_screen > 50) {
        ActualActionReplayHeadups(pSpecial_zappy_bastard);
        gLast_replay_zappy_screen = the_time;
        PDScreenBufferSwap(0);
        RemoveTransientBitmaps(1);
    }
}

// IDA: void __cdecl DoActionReplayHeadups()
// FUNCTION: CARM95 0x0041ae33
void DoActionReplayHeadups(void) {

    ActualActionReplayHeadups(0);
}

// IDA: void __usercall MoveReplayBuffer(tS32 pMove_amount@<EAX>)
// FUNCTION: CARM95 0x0041b41e
void MoveReplayBuffer(tS32 pMove_amount) {
    tU8* play_ptr;
    tU8* old_play_ptr;
    tU8* old_play_ptr2;
    int i;
    int a;
    tU32 old_time;

    old_play_ptr = NULL;
    gLast_replay_zappy_screen = 0;
    play_ptr = GetPipePlayPtr();
    old_play_ptr2 = play_ptr;
    old_time = GetTotalTime();
    for (i = 0; i < abs(pMove_amount) && play_ptr != old_play_ptr && !KeyIsDown(KEYMAP_ESCAPE); i++) {
        if (SomeReplayLeft()) {
            PipingFrameReset();
        }
        old_play_ptr = play_ptr;
        if (pMove_amount > 0) {
            while (!ApplyPipedSession(&play_ptr)) {
                DoZappyActionReplayHeadups(pMove_amount);
            }
            SetPipePlayPtr(play_ptr);
        } else if (pMove_amount < 0) {
            while (!UndoPipedSession(&play_ptr)) {
                DoZappyActionReplayHeadups(pMove_amount);
            }
            SetPipePlayPtr(play_ptr);
        }
        ProcessOilSpills(gFrame_period);
    }
    if (gReplay_rate < 0.f) {
        ScanAndPlaySoundsToBe(old_play_ptr2, old_time, GetTotalTime());
    }
    if (old_play_ptr == play_ptr) {
        gReplay_rate = 0.f;
        gPaused = 1;
        StopSaving();
    }
    if (KeyIsDown(KEYMAP_ESCAPE)) {
        WaitForNoKeys();
    }
}

// IDA: void __cdecl MoveToEndOfReplay()
// FUNCTION: CARM95 0x0041b3df
void MoveToEndOfReplay(void) {
    float old_replay_rate;

    DisablePipedSounds();
    old_replay_rate = gReplay_rate;
    gReplay_rate = 100.f;
#define INT32_MAX 2147483647
    MoveReplayBuffer(INT32_MAX);
    gReplay_rate = old_replay_rate;
    EnablePipedSounds();
}

// IDA: void __cdecl MoveToStartOfReplay()
// FUNCTION: CARM95 0x0041b622
void MoveToStartOfReplay(void) {
    float old_replay_rate;

    DisablePipedSounds();
    old_replay_rate = gReplay_rate;
    gReplay_rate = -100.f;
    MoveReplayBuffer(-INT32_MAX);
    gReplay_rate = old_replay_rate;
    EnablePipedSounds();
}

// IDA: void __cdecl ToggleReplay()
// FUNCTION: CARM95 0x0041b661
void ToggleReplay(void) {

    if (IsActionReplayAvailable()) {
        if (!gAction_replay_mode) {
            if (gMap_mode) {
                ToggleMap();
            }
            if (gNet_mode == eNet_mode_host) {
                SendGameplayToAllPlayers(eNet_gameplay_host_paused, 0, 0, 0, 0);
            }
            gReplay_rate = 1.f;
            gPaused = 1;
            gStopped_time = PDGetTotalTime();
            gPlay_direction = 1;
            gAction_replay_end_time = GetTotalTime();
            gLast_replay_frame_time = gAction_replay_end_time;
            gAction_replay_start_time = GetARStartTime();
            ResetPipePlayToEnd();
            LoadInterfaceStuff(1);
            StartMouseCursor();
            gKey_down = KEY_KP_ENTER;
            gPending_replay_rate = 0;
            gCam_change_time = PDGetTotalTime();
            if (!gRace_finished) {
                SaveCameraPosition(0);
            }
        } else {
            MoveToEndOfReplay();
            EndMouseCursor();
            S3RegisterSampleFilters(NULL, NULL);
            UnlockInterfaceStuff();
            AddLostTime(PDGetTotalTime() - gStopped_time);
            if (!gRace_finished) {
                RestoreCameraPosition(0);
            }
            if (gNet_mode == eNet_mode_host) {
                SendGameplayToAllPlayers(eNet_gameplay_host_unpaused, 0, 0, 0, 0);
            }
        }
        gAction_replay_mode = !gAction_replay_mode;
        ForceRebuildActiveCarList();
    } else {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_ACTION_REPLAY_UNAVAILABLE));
    }
}

// IDA: void __usercall ReverseSound(tS3_effect_tag pEffect_index@<EAX>, tS3_sound_tag pSound_tag@<EDX>)
// FUNCTION: CARM95 0x0041b7fe
void ReverseSound(tS3_effect_tag pEffect_index, tS3_sound_tag pSound_tag) {
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl FindUniqueFile()
// FUNCTION: CARM95 0x0041b819
int FindUniqueFile(void) {
    int index = 0;
    FILE* f;
    tPath_name the_path;

    for (index = 0; index < 1000; index++) {
        PathCat(the_path, gApplication_path, "BMPFILES");
        PathCat(the_path, the_path, "");
        sprintf(&the_path[strlen(the_path)], "%03d", index);
        strcat(the_path, "_0000.BMP");
        f = DRfopen(the_path, "rt");
        if (f == NULL) {
            return index;
        }
        fclose(f);
    }
    return 0;
}

// IDA: void __usercall PollActionReplayControls(tU32 pFrame_period@<EAX>)
// FUNCTION: CARM95 0x0041b925
void PollActionReplayControls(tU32 pFrame_period) {
    float old_replay_rate;
    int old_key_down;
    int x_coord;
    int y_coord;
    int i;
    tU32 real_time;

    // GLOBAL: CARM95 0x50a200
    static tU32 last_real_time = 0;

    // GLOBAL: CARM95 0x0050A208
    static int psuedo_mouse_keys[8] = {
        KEY_KP_7,
        KEY_KP_4,
        KEY_COMMA,
        KEY_SPACE,
        KEY_PERIOD,
        KEY_KP_6,
        KEY_KP_9,
        KEY_KP_MULTIPLY,
    };

    /* clang-format off */
    // GLOBAL: CARM95 0x0050A228
    static tRectangle mouse_areas[2][8] = {
        {
            {  63, 182,  92, 198, },
            {  93, 182, 118, 198, },
            { 119, 182, 144, 198, },
            { 145, 182, 166, 198, },
            { 167, 182, 192, 198, },
            { 193, 182, 218, 198, },
            { 219, 182, 244, 198, },
            { 245, 182, 272, 198, },
        },
        {
            { 126, 436, 184, 475, },
            { 186, 436, 236, 475, },
            { 238, 436, 288, 475, },
            { 290, 436, 332, 475, },
            { 334, 436, 384, 475, },
            { 386, 436, 436, 475, },
            { 438, 436, 488, 475, },
            { 490, 436, 544, 475, },
        },
    };
    /* clang-format on */

    real_time = PDGetTotalTime();
    old_replay_rate = gReplay_rate;
    old_key_down = gKey_down == KEY_CAPSLOCK ? -1 : gKey_down;
    gKey_down = PDAnyKeyDown();
    if (old_key_down == -1 && KeyIsDown(KEYMAP_REPLAYMODE)) {
        ToggleReplay();
        return;
    }

    if (gKey_down != -1) {
        gMouse_in_use = 0;
    } else {
        if ((old_key_down == -1 || old_key_down == KEY_KP_4 || old_key_down == KEY_KP_6 || old_key_down == KEY_KP_MULTIPLY) && EitherMouseButtonDown()) {
            GetMousePosition(&x_coord, &y_coord);
            for (i = 0; i < COUNT_OF(mouse_areas[0]); i++) {
                if (mouse_areas[gGraf_data_index][i].left <= x_coord
                    && mouse_areas[gGraf_data_index][i].top <= y_coord
                    && mouse_areas[gGraf_data_index][i].right > x_coord
                    && mouse_areas[gGraf_data_index][i].bottom > y_coord) {
                    gKey_down = psuedo_mouse_keys[i];
                    break;
                }
            }
        }
    }

    if (gKey_down == KEY_KP_DIVIDE && old_key_down != KEY_KP_DIVIDE) {
        if (gSave_file) {
            StopSaving();
        } else {
            gSave_bunch_ID = FindUniqueFile();
            gSave_frame_number = 0;
            gSave_file = 1;
            gPlay_direction = 1;
            gPaused = 0;
        }
    }

    if (gKey_down == KEY_KP_MULTIPLY) {
        gCam_change_button_down = 1;
        if (old_key_down != KEY_KP_MULTIPLY) {
            gCam_change_time = PDGetTotalTime();
            if (gAction_replay_camera_mode++ == eAction_replay_action) {
                gAction_replay_camera_mode = eAction_replay_standard;
            }
        }
    } else {
        gCam_change_button_down = 0;
    }

    if ((gKey_down == KEY_KP_5 || gKey_down == KEY_SPACE) && old_key_down == -1) {
        gPaused = !gPaused;
    } else if ((gKey_down == KEY_KP_0 || gKey_down == KEY_BACKSPACE) && old_key_down == -1) {
        gPlay_direction = -gPlay_direction;
        if (gPaused) {
            gPaused = 0;
        }
    }

    if (gKey_down == KEY_KP_1 && old_key_down == -1) {
        gReplay_rate = -1.f;
        gPlay_direction = -1;
        gSingle_frame_mode = 1;
    } else if (gKey_down == KEY_KP_3 && old_key_down == -1) {
        gReplay_rate = 1.f;
        gPlay_direction = 1;
        gSingle_frame_mode = 1;
    } else if (gKey_down == KEY_KP_4 || gKey_down == KEY_PAGEUP) {
        if (gReplay_rate > -1.2) {
            gReplay_rate = -1.2f;
        }
        if (last_real_time != 0) {
            gReplay_rate -= 0.002 * (real_time - last_real_time);
        }
        if (gReplay_rate < -8.f) {
            gReplay_rate = -8.f;
        }
    } else if (gKey_down == KEY_KP_6 || gKey_down == KEY_PAGEDOWN) {
        if (gReplay_rate < 1.2) {
            gReplay_rate = 1.2f;
        }
        if (last_real_time != 0) {
            gReplay_rate += 0.002 * (real_time - last_real_time);
        }
        if (gReplay_rate > 8.f) {
            gReplay_rate = 8.f;
        }
    } else if (gKey_down == KEY_COMMA) {
        gReplay_rate = -1.f;
        gPlay_direction = -1;
        gPaused = 0;
    } else if (gKey_down == KEY_PERIOD) {
        gReplay_rate = 1.f;
        gPlay_direction = 1;
        gPaused = 0;
    } else if (gPaused) {
        gReplay_rate = 0.f;
    } else {
        gReplay_rate = (float)gPlay_direction;
    }

    if ((gKey_down == KEY_KP_7 || gKey_down == KEY_HOME) && old_key_down == -1) {
        MoveToStartOfReplay();
        gReplay_rate = 1.f;
        MungeCarGraphics(gFrame_period);
        GrooveThoseDelics();
        gReplay_rate = 0.f;
        gPlay_direction = 1;
        gPaused = 1;
    } else if ((gKey_down == KEY_KP_9 || gKey_down == KEY_END) && old_key_down == -1) {
        MoveToEndOfReplay();
        gReplay_rate = -1.f;
        MungeCarGraphics(gFrame_period);
        GrooveThoseDelics();
        gReplay_rate = 0.f;
        gPlay_direction = -1;
        gPaused = 1;
    }

    if (gPending_replay_rate != 0.f) {
        gReplay_rate = gPending_replay_rate;
        gPending_replay_rate = 0;
    }
    if (old_replay_rate * gReplay_rate < 0.f) {
        gPending_replay_rate = gReplay_rate;
        gReplay_rate = 0.f;
    } else {
        gPending_replay_rate = 0.f;
    }

    if (old_replay_rate != 0.f) {
        gFrame_period = gFrame_period * (gReplay_rate / old_replay_rate);
    }
    // last_real_time = fabs(gReplay_rate) < 1.2 ? 0 : real_time;
    if (fabs(gReplay_rate) < 1.2) {
        last_real_time = 0;
    } else {
        last_real_time = real_time;
    }

    if (old_replay_rate <= 0.f && gReplay_rate > 0.f) {
        S3RegisterSampleFilters(NULL, NULL);
    } else if (old_replay_rate >= 0.f && gReplay_rate < 0.f) {
        S3RegisterSampleFilters(ReverseSound, ReverseSound);
    }
}

// IDA: void __cdecl CheckReplayTurnOn()
// FUNCTION: CARM95 0x0041c03b
void CheckReplayTurnOn(void) {

    if (!gAction_replay_mode) {
        if (KeyIsDown(KEYMAP_REPLAYMODE) && !gEntering_message) {
            if (gKey_down == -1) {
                ToggleReplay();
            }
        } else {
            gKey_down = -1;
        }
    }
}

// IDA: void __cdecl InitializeActionReplay()
// FUNCTION: CARM95 0x0041c093
void InitializeActionReplay(void) {
    int i;

    for (i = 0; i < COUNT_OF(gReplay_pixie_names); i++) {
        gReplay_pixies[i] = LoadPixelmap(gReplay_pixie_names[i]);
    }
    gAction_replay_camera_mode = eAction_replay_action;
}

// IDA: void __usercall DoActionReplay(tU32 pFrame_period@<EAX>)
// FUNCTION: CARM95 0x0041c0e6
void DoActionReplay(tU32 pFrame_period) {

    if (gReplay_rate != 0.f) {
        MoveReplayBuffer((tS32)gReplay_rate);
    }
}

// IDA: void __cdecl SynchronizeActionReplay()
// FUNCTION: CARM95 0x0041c11c
void SynchronizeActionReplay(void) {
    FILE* f;
    tPath_name the_path;
    // GLOBAL: CARM95 0x50a328
    static tU32 gLast_synch_time;

    while (gReplay_rate != 0.f) {
        if (PDGetTotalTime() - gLast_synch_time >= gFrame_period / fabs(gReplay_rate)) {
            break;
        }
        ServiceGameInRace();
    }
    gLast_synch_time = PDGetTotalTime();
    if (gSingle_frame_mode) {
        gReplay_rate = 0.f;
        gSingle_frame_mode = 0;
    }

    if (gSave_file) {
        PathCat(the_path, gApplication_path, "BMPFILES");
        strcat(the_path, gDir_separator);
        sprintf(&the_path[strlen(the_path)], "%03d_%04d.BMP", gSave_bunch_ID, gSave_frame_number);
        f = DRfopen(the_path, "wb");
        if (f != NULL) {
            PrintScreenFile(f);
            fclose(f);
        }
        gSave_frame_number++;
    }
}
