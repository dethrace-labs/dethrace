#include "cutscene.h"
#include "common/errors.h"
#include "common/globvars.h"
#include "common/graphics.h"
#include "common/input.h"
#include "common/loading.h"
#include "common/utility.h"
#include "libsmacker/smacker.h"
#include "pc-dos/dossys.h"
#include "sound.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

tS32 gLast_demo_end_anim;

void ShowCutScene(int pIndex, int pWait_end, int pSound_ID, br_scalar pDelay) {
    NOT_IMPLEMENTED();
}

void DoSCILogo() {
}

void DoStainlessLogo() {
    NOT_IMPLEMENTED();
}

void PlaySmackerFile(char* pSmack_name) {
    tPath_name the_path;
    br_colour* br_colours_ptr;
    tU8* smack_colours_ptr;
    //Smack* smk;
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
        if (!s) {
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
        if (s) {
            dr_dprintf("Smack file opened OK");
            smk_info_all(s, NULL, &f, &usf);
            smk_info_video(s, &w, &h, NULL);
            double fps = 1000000.0 / usf;
            int delay_ms = (1 / fps) * 1000;
            ts.tv_sec = delay_ms / 1000;
            ts.tv_nsec = (delay_ms % 1000) * 1000000;
            smk_enable_video(s, 1);

            smk_first(s);
            do {
                const unsigned char* pal = smk_get_palette(s);
                for (i = 1; i < 256; i++) {
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
                nanosleep(&ts, &ts);
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

void DoOpeningAnimation() {
    LOG_TRACE("()");
    PlaySmackerFile("LOGO.SMK");
    PlaySmackerFile("MIX_INTR.SMK");
    return WaitForNoKeys();
}

void DoNewGameAnimation() {
    NOT_IMPLEMENTED();
}

void DoGoToRaceAnimation() {
    NOT_IMPLEMENTED();
}

void DoEndRaceAnimation() {
    int went_up_a_rank;
    NOT_IMPLEMENTED();
}

void DoGameOverAnimation() {
    NOT_IMPLEMENTED();
}

void DoGameCompletedAnimation() {
    NOT_IMPLEMENTED();
}

void StartLoadingScreen() {
    NOT_IMPLEMENTED();
}
