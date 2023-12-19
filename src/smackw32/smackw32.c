#include "include/smackw32/smackw32.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "harness/hooks.h"
#include "harness/trace.h"
// lib/libsmacker
#include "smacker.h"

#include "../miniaudio/include/miniaudio/miniaudio.h"
extern ma_engine miniaudio_engine; // defined in miniaudio_backend.c
extern int gSound_enabled; // defined in globvars.h

static uint32_t smack_last_frame_time = 0;

static void copy_palette(Smack* smack) {
    const unsigned char* pal = smk_get_palette(smack->smk_handle);
    memcpy(smack->Palette, pal, 256 * 3);
}

Smack* SmackOpen(const char* name, uint32_t flags, uint32_t extrabuf) {
    ma_format audioformat_ma;
    ma_paged_audio_buffer_config paged_audio_buffer_config;
    ma_data_converter_config data_converter_config;
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
    smack->MSPerFrame = (1 / fps) * 1000;
    smk_info_video(smk_handle, &smack->Width, &smack->Height, NULL);
    smk_enable_video(smk_handle, 1);

    if (smk_first(smk_handle) == SMK_ERROR) {
        smk_close(smk_handle);
        free(smack);
        return NULL;
    }
    copy_palette(smack);

    // initialize video sound if required and present
    smack->audio_sample_rate = 0;
    smack->audio_frame_size_in_bytes = 0;
    smack->audio_paged_buffer = NULL;
    smack->audio_pages_accumulated_cnt = 0;
    smack->audio_pages_accumulated_max = 0;
    smack->audio_paged_buffer_data = NULL;
    smack->audio_converter = NULL;
    smack->audio_track = NULL;
    if (!gSound_enabled) {
        return smack;
    }

    // get info about the audio tracks in this video
    smk_info_audio(smk_handle, &track_mask_smk, channels_smk, bitdepth_smk, sample_rate_smk);
    if (!(track_mask_smk & SMK_AUDIO_TRACK_0)) {
        LOG_INFO("Smacker file does not contain audio");
        return smack;
    }
    smack->audio_sample_rate = sample_rate_smk[0];
    switch (bitdepth_smk[0]) {
    case 8:
        audioformat_ma = ma_format_u8;
        smack->audio_frame_size_in_bytes = 1 * channels_smk[0];
        break;
    case 16:
        audioformat_ma = ma_format_s16;
        smack->audio_frame_size_in_bytes = 2 * channels_smk[0];
        break;
    case 24:
        audioformat_ma = ma_format_s24;
        smack->audio_frame_size_in_bytes = 3 * channels_smk[0];
        break;
    case 32:
        audioformat_ma = ma_format_s32;
        smack->audio_frame_size_in_bytes = 4 * channels_smk[0];
        break;
    default:
        LOG_WARN("Smacker audio stream has invalid bit depth: %d", bitdepth_smk[0]);
        goto audio_fail;
    }

    smack->audio_pages_accumulated_max = smack->Frames / 100; // divider discovered by trial and error. Syncs audio to video in all the intro and intermission videos without starving.
    if (smack->audio_pages_accumulated_max < 1)
        smack->audio_pages_accumulated_max = 1;
    //LOG_INFO("Smacker audio: will accumulate %d pages", smack->audio_pages_accumulated_max);

    // allocate and initialize paged buffer data
    smack->audio_paged_buffer_data = malloc(sizeof(ma_paged_audio_buffer_data));

    if ((smack->audio_frame_size_in_bytes == 0) || (ma_paged_audio_buffer_data_init(audioformat_ma, channels_smk[0], smack->audio_paged_buffer_data) != MA_SUCCESS)) {
        LOG_WARN("Failed to create paged audio buffer data");
        goto audio_fail;
    }

    // allocate and initialize paged buffer
    smack->audio_paged_buffer = malloc(sizeof(ma_paged_audio_buffer));

    paged_audio_buffer_config = ma_paged_audio_buffer_config_init(smack->audio_paged_buffer_data);
    if (ma_paged_audio_buffer_init (&paged_audio_buffer_config, smack->audio_paged_buffer) != MA_SUCCESS) {
        LOG_WARN("Failed to create paged audio buffer for smacker audio stream");
        goto audio_fail;
    }

    // allocate and initialize sound
    smack->audio_track = malloc(sizeof(ma_sound));

    if (ma_sound_init_from_data_source(&miniaudio_engine, smack->audio_paged_buffer, MA_SOUND_FLAG_NO_PITCH | MA_SOUND_FLAG_NO_SPATIALIZATION, NULL, smack->audio_track) != MA_SUCCESS) {
        LOG_WARN("Failed to create sound from data source");
        goto audio_fail;
    }

    // allocate and initialize data converter
    smack->audio_converter = malloc(sizeof(ma_data_converter));

    data_converter_config = ma_data_converter_config_init(audioformat_ma, audioformat_ma, channels_smk[0], channels_smk[0], smack->audio_sample_rate, ma_engine_get_sample_rate (&miniaudio_engine));
    if (ma_data_converter_init(&data_converter_config, NULL, smack->audio_converter) != MA_SUCCESS) {
        LOG_WARN("Failed to create sound data converter");
        goto audio_fail;
    }

    // tell libsmacker we can process audio now
    smk_enable_audio(smk_handle, 0, 1);
    return smack;

audio_fail:
    if (smack->audio_converter != NULL) {
        free(smack->audio_converter);
        smack->audio_converter = NULL;
    }
    if (smack->audio_track != NULL) {
        ma_sound_uninit(smack->audio_track);
        free(smack->audio_track);
        smack->audio_track = NULL;
    }
    if (smack->audio_paged_buffer != NULL) {
        ma_paged_audio_buffer_uninit(smack->audio_paged_buffer);
        free(smack->audio_paged_buffer);
        smack->audio_paged_buffer = NULL;
    }
    if (smack->audio_paged_buffer_data != NULL) {
        ma_paged_audio_buffer_data_uninit (smack->audio_paged_buffer_data, NULL);
        free(smack->audio_paged_buffer_data);
        smack->audio_paged_buffer_data = NULL;
    }
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
    for (i = 0; i < smack->Height; i++) {
        memcpy(&char_buf[(i * pitch)], &frame[i * smack->Width], smack->Width);
    }
}

uint32_t SmackDoFrame(Smack* smack) {
    const unsigned char *audio_data;
    unsigned long audio_data_size;
    ma_paged_audio_buffer_page *newPage;
    ma_uint64 current_pos;
    ma_uint64 nb_frames_in;
    ma_uint64 nb_frames_out;
    ma_uint64 seek_pos;

    // process audio if we have some
    if (smack->audio_converter != NULL) {
        audio_data = smk_get_audio (smack->smk_handle, 0);
        audio_data_size = smk_get_audio_size (smack->smk_handle, 0);
        if ((audio_data == NULL) || (audio_data_size == 0)) {
            return 0;
        }

        if (ma_paged_audio_buffer_get_length_in_pcm_frames (smack->audio_paged_buffer, &current_pos) != MA_SUCCESS) {
            LOG_WARN("ma_paged_audio_buffer_get_length_in_pcm_frames failed");
            return 0;
        }

        nb_frames_in = audio_data_size / smack->audio_frame_size_in_bytes;
        nb_frames_out = nb_frames_in * ma_engine_get_sample_rate (&miniaudio_engine) / smack->audio_sample_rate;

        if (ma_paged_audio_buffer_data_allocate_page (smack->audio_paged_buffer_data, nb_frames_out, NULL, NULL, &newPage) != MA_SUCCESS) {
            LOG_WARN("ma_paged_audio_buffer_data_allocate_page failed");
            return 0;
        }
        else if (ma_data_converter_process_pcm_frames (smack->audio_converter, audio_data, &nb_frames_in, newPage->pAudioData, &nb_frames_out) != MA_SUCCESS) {
            LOG_WARN("ma_data_converter_process_pcm_frames failed");
            return 0;
        }
        else if (ma_paged_audio_buffer_data_append_page (smack->audio_paged_buffer_data, newPage) != MA_SUCCESS) {
            LOG_WARN("ma_paged_audio_buffer_data_append_page failed");
            return 0;
        }
        smack->audio_pages_accumulated_cnt++;

        if (smack->audio_pages_accumulated_cnt > smack->audio_pages_accumulated_max)
        {
            if (!ma_sound_is_playing (smack->audio_track))
            {
                // seek either at start, or where the accumulated value hasn't played yet
                seek_pos = ((smack->audio_pages_accumulated_max - 1) * nb_frames_out < current_pos ? current_pos - (smack->audio_pages_accumulated_max - 1) * nb_frames_out : 0);
                if (ma_sound_seek_to_pcm_frame (smack->audio_track, seek_pos) != MA_SUCCESS) {
                    LOG_WARN("ma_sound_seek_to_pcm_frame failed");
                }
                if (ma_sound_start (smack->audio_track) != MA_SUCCESS) {
                    LOG_WARN("ma_sound_start failed");
                }
            }
            if (ma_sound_at_end (smack->audio_track)) {
                LOG_WARN ("video not playing fast enough: sound starved!");
            }
        }
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
    if (smack->audio_converter != NULL) {
        ma_data_converter_uninit(smack->audio_converter, NULL);
        free(smack->audio_converter);
    }
    if (smack->audio_track != NULL) {
        ma_sound_stop(smack->audio_track);
        ma_sound_uninit(smack->audio_track);
        free(smack->audio_track);
    }
    if (smack->audio_paged_buffer != NULL) {
        ma_paged_audio_buffer_uninit(smack->audio_paged_buffer);
        free(smack->audio_paged_buffer);
    }
    if (smack->audio_paged_buffer_data != NULL) {
        ma_paged_audio_buffer_data_uninit(smack->audio_paged_buffer_data, NULL);
        free(smack->audio_paged_buffer_data);
    }
    smk_close(smack->smk_handle);
    free(smack);
}
