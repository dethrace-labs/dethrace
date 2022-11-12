#include "cutscene.h"
#include "drmem.h"
#include "errors.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrpb.h"
#include "graphics.h"
#include "harness/config.h"
#include "harness/os.h"
#include "harness/trace.h"
#include "input.h"
#include "loading.h"
#include "pd/sys.h"
#include "smacker.h"
#include "sound.h"
#include "utility.h"
#include <stdlib.h>
#include <time.h>

tS32 gLast_demo_end_anim = -90000;

// IDA: void __usercall ShowCutScene(int pIndex@<EAX>, int pWait_end@<EDX>, int pSound_ID@<EBX>, br_scalar pDelay)
void ShowCutScene(int pIndex, int pWait_end, int pSound_ID, br_scalar pDelay) {
    LOG_TRACE("(%d, %d, %d, %f)", pIndex, pWait_end, pSound_ID, pDelay);

    gProgram_state.cut_scene = 1;
    if (pSound_ID >= 0) {
        DRS3LoadSound(pSound_ID);
        SetFlicSound(pSound_ID, PDGetTotalTime() + 1000.f * pDelay);
    }
    SetNonFatalAllocationErrors();
    RunFlic(pIndex);
    ResetNonFatalAllocationErrors();
    if (pWait_end) {
        WaitForAKey();
    } else {
        WaitForNoKeys();
    }
    FadePaletteDown();
    ClearEntireScreen();
    if (pSound_ID >= 0) {
        DRS3ReleaseSound(pSound_ID);
        SetFlicSound(0, 0);
    }
    gProgram_state.cut_scene = 0;
}

// IDA: void __cdecl DoSCILogo()
void DoSCILogo() {
}

// IDA: void __cdecl DoStainlessLogo()
void DoStainlessLogo() {
    LOG_TRACE("()");
}

// IDA: void __usercall PlaySmackerFile(char *pSmack_name@<EAX>)
void PlaySmackerFile(char* pSmack_name) {
    tPath_name the_path;
    br_colour* br_colours_ptr;
    tU8* smack_colours_ptr;
    // Smack* smk;
    int i;
    int j;
    int len;
    int fuck_off;

    LOG_TRACE("(\"%s\")", pSmack_name);

    smk s;
    br_uint_8* dest_pix = (br_uint_8*)gBack_screen->pixels;
    unsigned long w, h, f;
    unsigned char r, g, b;
    double usf;
    struct timespec ts;

    if (!gSound_override && !gCut_scene_override) {
        StopMusic();
        FadePaletteDown();
        ClearEntireScreen();
        br_colours_ptr = gCurrent_palette->pixels;
        PathCat(the_path, gApplication_path, "CUTSCENE");
        PathCat(the_path, the_path, pSmack_name);

        dr_dprintf("Trying to open smack file '%s'", the_path);
        s = smk_open_file(the_path, SMK_MODE_MEMORY);
        if (s == NULL) {
            dr_dprintf("Unable to open smack file - attempt to load smack from CD...");
            if (GetCDPathFromPathsTxtFile(the_path)) {
                strcat(the_path, gDir_separator);
                strcat(the_path, "DATA");
                PathCat(the_path, the_path, "CUTSCENE");
                PathCat(the_path, the_path, pSmack_name);
                if (PDCheckDriveExists(the_path)) {
                    s = smk_open_file(the_path, SMK_MODE_MEMORY);
                }
            } else {
                dr_dprintf("Can't get CD directory name");
            }
        }
        if (s != NULL) {
            dr_dprintf("Smack file opened OK");
            smk_info_all(s, NULL, &f, &usf);
            smk_info_video(s, &w, &h, NULL);
            double fps = 1000000.0 / usf;
            int delay_ms = (1 / fps) * 1000;

            smk_enable_video(s, 1);

            smk_first(s);
            do {
                const unsigned char* pal = smk_get_palette(s);
                for (i = 0; i < 256; i++) {
                    r = pal[(i * 3)];
                    g = pal[(i * 3) + 1];
                    b = pal[(i * 3) + 2];
                    br_colours_ptr[i] = b | (g << 8) | (r << 16);
                }
                DRSetPalette(gCurrent_palette);
                EnsurePaletteUp();

                const unsigned char* frame = smk_get_video(s);
                for (i = 0; i < h; i++) {
                    for (j = 0; j < w; j++) {
                        dest_pix[(i * gBack_screen->row_bytes) + j] = frame[i * w + j];
                    }
                }
                PDScreenBufferSwap(0);

                if (AnyKeyDown() || EitherMouseButtonDown()) {
                    break;
                }
                // wait until its time for the next frame
                OS_Sleep(delay_ms);
            } while (smk_next(s) == SMK_MORE);

            smk_close(s);

            FadePaletteDown();
            ClearEntireScreen();
            StartMusic();
        } else {
            dr_dprintf("Smack file '%s' failed to open", pSmack_name);
            StartMusic();
        }
    }
}

// IDA: void __cdecl DoOpeningAnimation()
void DoOpeningAnimation() {
    LOG_TRACE("()");

    PlaySmackerFile("LOGO.SMK");
    PlaySmackerFile(harness_game_info.defines.INTRO_SMK_FILE);
    WaitForNoKeys();
}

// IDA: void __cdecl DoNewGameAnimation()
void DoNewGameAnimation() {
    LOG_TRACE("()");
}

// IDA: void __cdecl DoGoToRaceAnimation()
void DoGoToRaceAnimation() {
    LOG_TRACE("()");

    if (!gNet_mode) {
        if (PercentageChance(50)) {
            PlaySmackerFile("GARAGE2.SMK");
        } else {
            PlaySmackerFile("GARAGE1.SMK");
        }
    }
}

// IDA: void __cdecl DoEndRaceAnimation()
void DoEndRaceAnimation() {
    int made_a_profit;
    int went_up_a_rank;
    LOG_TRACE("()");

    made_a_profit = gProgram_state.credits_earned >= gProgram_state.credits_lost;
    went_up_a_rank = gProgram_state.credits_earned >= gProgram_state.credits_per_rank;

    FadePaletteDown();

    if (gAusterity_mode || gNet_mode != eNet_mode_none) {
        return;
    }
    if (gProgram_state.credits + gProgram_state.credits_earned - gProgram_state.credits_lost >= 0) {
        if (made_a_profit && went_up_a_rank) {
            PlaySmackerFile("SUCCESS.SMK");
        } else if (made_a_profit || went_up_a_rank) {
            PlaySmackerFile("MUNDANE.SMK");
        } else {
            PlaySmackerFile("UNSUCSES.SMK");
        }
    }
}

// IDA: void __cdecl DoGameOverAnimation()
void DoGameOverAnimation() {
    LOG_TRACE("()");

    StopMusic();
    PlaySmackerFile("CRASH.SMK");
    StartMusic();
}

// IDA: void __cdecl DoGameCompletedAnimation()
void DoGameCompletedAnimation() {
    LOG_TRACE("()");

    StopMusic();
    PlaySmackerFile("TOPRANK.SMK");
    StartMusic();
}

void DoFeatureUnavailableInDemo() {
    LOG_TRACE("()");

    PrintMemoryDump(0, "BEFORE DEMO-ONLY SCREEN");

    SuspendPendingFlic();
    FadePaletteDown();
    ShowCutScene(7, 1, 8502, gCut_delay_3);
    FadePaletteDown();

    PrintMemoryDump(0, "AFTER DEMO-ONLY SCREEN");
}

void DoFullVersionPowerpoint() {
    LOG_TRACE("()");

    FadePaletteDown();
    DRSetPalette(gRender_palette);
    if (harness_game_info.mode == eGame_splatpack_demo) {
        PlaySmackerFile("DEMOEND.SMK");
    } else {
        ShowCutScene(9, 0, 8503, gCut_delay_4);
    }
    FadePaletteDown();

    gLast_demo_end_anim = PDGetTotalTime();
}

void DoDemoGoodbye() {
    if (PDGetTotalTime() - gLast_demo_end_anim > 90000) {
        DoFullVersionPowerpoint();
    }
}

// IDA: void __cdecl StartLoadingScreen()
void StartLoadingScreen() {
    LOG_TRACE("()");

    PossibleService();
    if (gProgram_state.sausage_eater_mode) {
        SplashScreenWith(harness_game_info.defines.GERMAN_LOADSCRN);
    } else {
        SplashScreenWith("LOADSCRN.PIX");
    }
}
