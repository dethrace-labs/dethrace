#include "include/smackw32/smackw32.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "harness/audio.h"
#include "harness/hooks.h"
#include "harness/trace.h"

// lib/libsmacker
#include "smacker.h"

static uint32_t smack_last_frame_time = 0;

static void copy_palette(Smack* smack) {
    const unsigned char* pal = smk_get_palette(smack->smk_handle);
    memcpy(smack->Palette, pal, 256 * 3);
}

Smack* SmackOpen(const char* name, uint32_t flags, uint32_t extrabuf) {
    unsigned char track_mask_smk;
    unsigned char channels_smk[7];
    unsigned char bitdepth_smk[7];
    unsigned long sample_rate_smk[7];
    double microsecs_per_frame;
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

    smk_info_all(smk_handle, NULL, &smack->Frames, &microsecs_per_frame);
    fps = 1000000.0 / microsecs_per_frame;
    smack->MSPerFrame = (unsigned long)((1 / fps) * 1000);
    smk_info_video(smk_handle, &smack->Width, &smack->Height, NULL);
    smk_enable_video(smk_handle, 1);

    // get info about the audio tracks in this video
    smk_info_audio(smk_handle, &track_mask_smk, channels_smk, bitdepth_smk, sample_rate_smk);
    // if (!(track_mask_smk & SMK_AUDIO_TRACK_0)) {
    //     LOG_INFO("Smacker file does not contain audio");
    //     goto no_audio;
    // }
    // smack->audio_sample_rate = sample_rate_smk[0];
    // switch (bitdepth_smk[0]) {
    // case 8:
    //     audioformat_ma = ma_format_u8;
    //     smack->audio_frame_size_in_bytes = 1 * channels_smk[0];
    //     break;
    // case 16:
    //     audioformat_ma = ma_format_s16;
    //     smack->audio_frame_size_in_bytes = 2 * channels_smk[0];
    //     break;
    // case 24:
    //     audioformat_ma = ma_format_s24;
    //     smack->audio_frame_size_in_bytes = 3 * channels_smk[0];
    //     break;
    // case 32:
    //     audioformat_ma = ma_format_s32;
    //     smack->audio_frame_size_in_bytes = 4 * channels_smk[0];
    //     break;
    // default:
    //     LOG_WARN("Smacker audio stream has invalid bit depth: %d", bitdepth_smk[0]);
    //     goto no_audio;
    // }

    // // allocate and initialize paged buffer data
    // smack->audio_paged_buffer_data = malloc(sizeof(ma_paged_audio_buffer_data));

    // if ((smack->audio_frame_size_in_bytes == 0) || (ma_paged_audio_buffer_data_init(audioformat_ma, channels_smk[0], smack->audio_paged_buffer_data) != MA_SUCCESS)) {
    //     LOG_WARN("Failed to create paged audio buffer data");
    //     goto no_audio;
    // }

    // // allocate and initialize paged buffer
    // smack->audio_paged_buffer = malloc(sizeof(ma_paged_audio_buffer));

    // paged_audio_buffer_config = ma_paged_audio_buffer_config_init(smack->audio_paged_buffer_data);
    // if (ma_paged_audio_buffer_init(&paged_audio_buffer_config, smack->audio_paged_buffer) != MA_SUCCESS) {
    //     LOG_WARN("Failed to create paged audio buffer for smacker audio stream");
    //     goto no_audio;
    // }

    // // allocate and initialize sound
    // smack->audio_track = malloc(sizeof(ma_sound));

    // if (ma_sound_init_from_data_source(&miniaudio_engine, smack->audio_paged_buffer, MA_SOUND_FLAG_NO_PITCH | MA_SOUND_FLAG_NO_SPATIALIZATION, NULL, smack->audio_track) != MA_SUCCESS) {
    //     LOG_WARN("Failed to create sound from data source");
    //     goto no_audio;
    // }

    // // allocate and initialize data converter if miniaudio engine and Smack file soundtrack sample rates differ
    // if (ma_engine_get_sample_rate(&miniaudio_engine) != sample_rate_smk[0]) {
    //     smack->audio_converter = malloc(sizeof(ma_data_converter));

    //     data_converter_config = ma_data_converter_config_init(audioformat_ma, audioformat_ma, channels_smk[0], channels_smk[0], smack->audio_sample_rate, ma_engine_get_sample_rate(&miniaudio_engine));
    //     if (ma_data_converter_init(&data_converter_config, NULL, smack->audio_converter) != MA_SUCCESS) {
    //         LOG_WARN("Failed to create sound data converter");
    //         goto no_audio;
    //     }
    // }
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
    // ma_result result;
    // ma_engine_config engineConfig;
    // engineConfig = ma_engine_config_init();
    // // engineConfig.sampleRate = 22050;
    // result = ma_engine_init(&engineConfig, &miniaudio_engine);
    // if (result != MA_SUCCESS) {
    //     LOG_PANIC("Failed to initialize miniaudio");
    // }
    return 0;
}

void SmackToBuffer(Smack* smack, uint32_t left, uint32_t top, uint32_t pitch, uint32_t destheight, void* buf, uint32_t flags) {
    unsigned long i; // Pierre-Marie Baty -- fixed type

    // minimal implementation
    assert(left == 0);
    assert(top == 0);
    assert(flags == 0);

    char* char_buf = buf;

    const unsigned char* frame = smk_get_video(smack->smk_handle);
    for (i = 0; i < smack->Height; i++) {
        memcpy(&char_buf[(i * pitch)], &frame[i * smack->Width], smack->Width);
    }
}

uint32_t SmackDoFrame(Smack* smack) {
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

uint32_t SmackWait(Smack* smack) {
    uint32_t now = gHarness_platform.GetTicks();
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
    free(smack);
}
