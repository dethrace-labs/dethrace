#include "smackw32/smackw32.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "harness/hooks.h"
#include "harness/os.h"
#include "harness/trace.h"

// lib/libsmacker
#include "smacker.h"

static unsigned int smack_last_frame_time = 0;

static void copy_palette(Smack* smack) {
    const unsigned char* pal = smk_get_palette(smack->smk_handle);
    memcpy(smack->Palette, pal, 256 * 3);
}

Smack* SmackOpen(const char* name, unsigned int flags, unsigned int extrabuf) {
    unsigned char track_mask_smk;
    unsigned char channels_smk[7];
    unsigned char bitdepth_smk[7];
    unsigned long sample_rate_smk[7];
    double microsecs_per_frame;
    Smack* smack;
    double fps;
    FILE* f = NULL;
    smk smk_handle;

    f = OS_fopen(name, "rb");
    if (f == NULL) {
        return NULL;
    }
    smk_handle = smk_open_filepointer(f, SMK_MODE_MEMORY);
    if (smk_handle == NULL) {
        fclose(f);
        return NULL;
    }

    smack = malloc(sizeof(Smack));

    // libsmacker doesn't tell us whether the palette is new on each frame or not, so just assume it always is new
    smack->NewPalette = 1;

    // smk_handle is added to hold a pointer to the underlying libsmacker instance
    smack->smk_handle = smk_handle;

    smack->f = f;

    smk_info_all(smk_handle, NULL, &smack->Frames, &microsecs_per_frame);
    fps = 1000000.0 / microsecs_per_frame;
    smack->MSPerFrame = (unsigned long)((1 / fps) * 1000);
    smk_info_video(smk_handle, &smack->Width, &smack->Height, NULL);
    smk_enable_video(smk_handle, 1);

    // get info about the audio tracks in this video
    smk_info_audio(smk_handle, &track_mask_smk, channels_smk, bitdepth_smk, sample_rate_smk);

    smack->audio_stream = NULL;
    if ((track_mask_smk & SMK_AUDIO_TRACK_0)) {
        smack->audio_stream = AudioBackend_StreamOpen(bitdepth_smk[0], channels_smk[0], sample_rate_smk[0]);
        if (smack->audio_stream != NULL) {
            // tell libsmacker we can process audio now
            smk_enable_audio(smk_handle, 0, 1);
        }
    }

    // load the first frame and return a handle to the Smack file
    if (smk_first(smk_handle) == SMK_ERROR) {
        smk_close(smk_handle);
        free(smack);
        return NULL;
    }
    copy_palette(smack);
    return smack;
}

int SmackSoundUseDirectSound(void* dd) {
    return 0;
}

void SmackToBuffer(Smack* smack, unsigned int left, unsigned int top, unsigned int pitch, unsigned int destheight, void* buf, unsigned int flags) {
    unsigned long i; // Pierre-Marie Baty -- fixed type
    char* char_buf = buf;
    const unsigned char* frame;

    // minimal implementation
    assert(left == 0);
    assert(top == 0);
    assert(flags == 0);

    frame = smk_get_video(smack->smk_handle);
    for (i = 0; i < smack->Height; i++) {
        memcpy(&char_buf[(i * pitch)], &frame[i * smack->Width], smack->Width);
    }
}

int SmackDoFrame(Smack* smack) {
    const unsigned char* audio_data;
    unsigned long audio_data_size;

    // process audio if we have some
    if (smack->audio_stream != NULL) {
        audio_data = smk_get_audio(smack->smk_handle, 0);
        audio_data_size = smk_get_audio_size(smack->smk_handle, 0);
        if ((audio_data == NULL) || (audio_data_size == 0)) {
            return 0;
        }

        AudioBackend_StreamWrite(smack->audio_stream, audio_data, audio_data_size);
    }

    return 0;
}

void SmackNextFrame(Smack* smack) {
    smk_next(smack->smk_handle);
    copy_palette(smack);
}

int SmackWait(Smack* smack) {
    unsigned int now = gHarness_platform.GetTicks();
    if (now < smack_last_frame_time + smack->MSPerFrame) {
        gHarness_platform.Sleep(1);
        return 1;
    }
    smack_last_frame_time = now;
    return 0;
}

void SmackClose(Smack* smack) {
    if (smack->audio_stream != NULL) {
        AudioBackend_StreamClose(smack->audio_stream);
    }

    smk_close(smack->smk_handle);
    // libsmacker closes file, no need to do `fclose(smack->f)`
    free(smack);
}
