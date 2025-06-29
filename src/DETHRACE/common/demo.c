#include "demo.h"
#include "globvars.h"
#include "graphics.h"
#include "harness/trace.h"
#include "input.h"
#include "loading.h"
#include "pd/sys.h"
#include "s3/s3.h"
#include "sound.h"
#include "utility.h"
#include <stdlib.h>

// GLOBAL: CARM95 0x00512080
int gLast_demo;

// IDA: void __cdecl DoDemo()
// FUNCTION: CARM95 0x00461110
void DoDemo(void) {
    tS32 start_time;
    tS32 frame_time;
    FILE* f;
    tPath_name the_path;
    int i;
    int count;
    char s[256];
    char* str;
    tS3_sound_tag song_tag;

    PathCat(the_path, gApplication_path, "DEMOFILE.TXT");
    f = DRfopen(the_path, "rt");
    if (f == NULL) {
        return;
    }
    count = GetAnInt(f);
    gLast_demo++;
    if (gLast_demo >= count) {
        gLast_demo = 0;
    }
    for (i = 0; i <= gLast_demo; i++) {
        GetALineAndDontArgue(f, s);
    }
    fclose(f);
    PathCat(the_path, gApplication_path, s);
    f = DRfopen(the_path, "rb");
    if (f == NULL) {
        return;
    }

    ClearEntireScreen();
    song_tag = S3StartSound(gEffects_outlet, 10000);
    DRSetPalette(gRender_palette);
    FadePaletteUp();

    while (1) {
        SoundService();
        start_time = PDGetTotalTime();
        frame_time = ReadS32(f);
        fread(gBack_screen->pixels, gBack_screen->height * gBack_screen->width, 1, f);
        PDScreenBufferSwap(0);
        while (frame_time > PDGetTotalTime() - start_time && !AnyKeyDown() && !EitherMouseButtonDown()) {
            // FIXME: sleep? SoundService?
        }
        if (!S3SoundStillPlaying(song_tag)) {
            song_tag = S3StartSound(gEffects_outlet, 10000);
        }
        if (AnyKeyDown() || EitherMouseButtonDown() || feof(f)) {
            break;
        }
    }
    S3StopSound(song_tag);
    S3StopOutletSound(gEffects_outlet);
    S3StopAllOutletSounds();
    fclose(f);
    FadePaletteDown();
    WaitForNoKeys();
}
