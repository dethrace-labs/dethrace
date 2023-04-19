#include "include/smackw32/smackw32.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "harness/hooks.h"
// lib/libsmacker
#include "smacker.h"

uint32_t smack_last_frame_time;

void copy_palette(Smack* smack) {
    const unsigned char* pal = smk_get_palette(smack->smk_handle);
    memcpy(smack->Palette, pal, 256 * 3);
}

Smack* SmackOpen(const char* name, uint32_t flags, uint32_t extrabuf) {
    double usf;
    Smack* smack;
    double fps;

    smk smk_handle = smk_open_file(name, SMK_MODE_MEMORY);
    if (smk_handle == NULL) {
        return NULL;
    }

    smack = malloc(sizeof(Smack));

    // libsmacker doesn't tell us whether the palette is new on each frame or not, so just assume it always is new
    smack->NewPalette = 1;

    // smk_handle is added to hold a pointer to the underlying libsmacker instance
    smack->smk_handle = smk_handle;

    smk_info_all(smk_handle, NULL, &smack->Frames, &usf);
    fps = 1000000.0 / usf;
    smack->MSPerFrame = (1 / fps) * 1000;
    smk_info_video(smk_handle, &smack->Width, &smack->Height, NULL);
    smk_enable_video(smk_handle, 1);

    if (smk_first(smk_handle) == SMK_ERROR) {
        smk_close(smk_handle);
        free(smack);
        return NULL;
    }
    copy_palette(smack);
    return smack;
}

int SmackSoundUseDirectSound(void* dd) {
    // TODO: do some miniaudio init

    return 0;
}

void SmackToBuffer(Smack* smack, uint32_t left, uint32_t top, uint32_t pitch, uint32_t destheight, void* buf, uint32_t flags) {
    int i, j;

    // minimal implementation
    assert(left == 0);
    assert(top == 0);
    assert(flags == 0);

    char* char_buf = buf;

    const unsigned char* frame = smk_get_video(smack->smk_handle);
    for (i = 0; i < destheight; i++) {
        memcpy(&char_buf[(i * pitch)], &frame[i * pitch], pitch);
    }
}

uint32_t SmackDoFrame(Smack* smack) {
    smack_last_frame_time = gHarness_platform.GetTicks();

    // TODO: audio processing
    return 0;
}

void SmackNextFrame(Smack* smack) {
    smk_next(smack->smk_handle);
    copy_palette(smack);
}

uint32_t SmackWait(Smack* smack) {
    uint32_t now = gHarness_platform.GetTicks();
    if (now < smack_last_frame_time + smack->MSPerFrame) {
        gHarness_platform.Sleep(1);
        return 1;
    }
    return 0;
}

void SmackClose(Smack* smack) {
    smk_close(smack->smk_handle);
    free(smack);
}
