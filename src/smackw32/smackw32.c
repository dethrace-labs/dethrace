#include "include/smackw32/smackw32.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "harness/hooks.h"
// lib/libsmacker
#include "smacker.h"
// lib/miniaudio
#include <miniaudio/miniaudio.h>

extern ma_engine miniaudio_engine;

typedef struct {
    uint32_t smack_last_frame_time;
    // libsmacker data
    smk smk_handle;

    struct {
        int audio_valid;
        ma_paged_audio_buffer paged_audio_buffer;
        ma_paged_audio_buffer_data paged_audio_buffer_data;
        ma_sound sound;
        ma_uint32 sample_rate;
        ma_data_converter data_converter;
        int frame_size_in_bytes;
    } miniaudio;
} SmackUserData;

static int audio_init(Smack* smack) {
    SmackUserData* const user = smack->user;

    unsigned char track_mask_smk;
    unsigned char channels_smk[7];
    unsigned char bitdepth_smk[7];
    unsigned long sample_rate_smk[7];

    ma_result result_ma;
    ma_format audioformat_ma;
    ma_paged_audio_buffer_config paged_audio_buffer_config;
    ma_data_converter_config dataConverterConfig;

    smk_info_audio(user->smk_handle, &track_mask_smk, channels_smk, bitdepth_smk, sample_rate_smk);
    if (!(track_mask_smk & SMK_AUDIO_TRACK_0)) {
        goto error_base;
    }
    user->miniaudio.sample_rate = sample_rate_smk[0];

    switch (bitdepth_smk[0]) {
    case 8:
        audioformat_ma = ma_format_u8;
        user->miniaudio.frame_size_in_bytes = 1 * channels_smk[0];
        break;
    case 16:
        audioformat_ma = ma_format_s16;
        user->miniaudio.frame_size_in_bytes = 2 * channels_smk[0];
        break;
    case 24:
        audioformat_ma = ma_format_s24;
        user->miniaudio.frame_size_in_bytes = 3 * channels_smk[0];
        break;
    case 32:
        audioformat_ma = ma_format_s32;
        user->miniaudio.frame_size_in_bytes = 4 * channels_smk[0];
        break;
    default:
        fprintf(stderr, "Smacker audio stream has invalid bit depth: %d\n", bitdepth_smk[0]);
        goto error_base;
    }

    result_ma = ma_paged_audio_buffer_data_init(audioformat_ma, channels_smk[0], &user->miniaudio.paged_audio_buffer_data);
    if (result_ma != MA_SUCCESS) {
        fprintf(stderr, "Failed to create paged audio buffer data\n");
        goto error_base;
    }

    paged_audio_buffer_config = ma_paged_audio_buffer_config_init(&user->miniaudio.paged_audio_buffer_data);
    result_ma = ma_paged_audio_buffer_init(&paged_audio_buffer_config, &user->miniaudio.paged_audio_buffer);
    if (result_ma != MA_SUCCESS) {
        fprintf(stderr, "Failed to create paged audio buffer for smacker audio stream\n");
        goto error_paged_audio_buffer_data;
    }

    result_ma = ma_sound_init_from_data_source(&miniaudio_engine, &user->miniaudio.paged_audio_buffer, MA_SOUND_FLAG_NO_PITCH | MA_SOUND_FLAG_NO_SPATIALIZATION, NULL, &user->miniaudio.sound);
    if (result_ma != MA_SUCCESS) {
        fprintf(stderr, "Failed to create sound from data source\n");
        goto error_paged_audio_buffer;
    }

    dataConverterConfig = ma_data_converter_config_init(audioformat_ma, audioformat_ma, channels_smk[0], ma_engine_get_channels(&miniaudio_engine), (ma_uint32)sample_rate_smk[0], ma_engine_get_sample_rate(&miniaudio_engine));
    result_ma = ma_data_converter_init(&dataConverterConfig, NULL, &user->miniaudio.data_converter);
    if (result_ma != MA_SUCCESS) {
        fprintf(stderr, "Failed to create data converter\n");
        goto error_sound;
    }
    user->miniaudio.audio_valid = 1;
    smk_enable_audio(user->smk_handle, 0, 1);
    return 0;
error_sound:
    ma_sound_uninit(&user->miniaudio.sound);
error_paged_audio_buffer:
    ma_paged_audio_buffer_uninit(&user->miniaudio.paged_audio_buffer);
error_paged_audio_buffer_data:
    ma_paged_audio_buffer_data_uninit(&user->miniaudio.paged_audio_buffer_data, NULL);
error_base:
    user->miniaudio.audio_valid = 0;
    return 1;
}

static void audio_exit(Smack *smack) {
    SmackUserData *const user = smack->user;

    if (user->miniaudio.audio_valid) {

        while (ma_sound_is_playing(&user->miniaudio.sound)) {
            // FIXME: sleep
            //        PossibleService();
            //        OS_Sleep(1);
        }
        ma_sound_stop(&user->miniaudio.sound);
        ma_data_converter_uninit(&user->miniaudio.data_converter, NULL);
        ma_sound_uninit(&user->miniaudio.sound);
        ma_paged_audio_buffer_uninit(&user->miniaudio.paged_audio_buffer);
        ma_paged_audio_buffer_data_uninit(&user->miniaudio.paged_audio_buffer_data, NULL);
    }
}

static void audio_step(Smack *smack) {
    ma_result result_ma;
    SmackUserData *const user = smack->user;

    const unsigned char *audio_data = smk_get_audio(user->smk_handle, 0);
    unsigned long audio_data_size = smk_get_audio_size(user->smk_handle, 0);

    if (audio_data == NULL) {
        return;
    }

    ma_uint64 frame_length_at_start;
    result_ma = ma_paged_audio_buffer_get_length_in_pcm_frames(&user->miniaudio.paged_audio_buffer, &frame_length_at_start);
    if (result_ma != MA_SUCCESS) {
        fprintf(stderr, "ma_paged_audio_buffer_get_length_in_pcm_frames failed");
        return;
    }

    ma_uint64 framesIn = audio_data_size / (ma_uint64)user->miniaudio.frame_size_in_bytes;
    ma_uint64 framesOut = framesIn * ma_engine_get_sample_rate(&miniaudio_engine) / user->miniaudio.sample_rate;

    ma_paged_audio_buffer_page *newPage;
    result_ma = ma_paged_audio_buffer_data_allocate_page(&user->miniaudio.paged_audio_buffer_data, framesOut, NULL, NULL, &newPage);
    if (result_ma != MA_SUCCESS) {
        fprintf(stderr, "ma_paged_audio_buffer_data_allocate_page failed");
        return;
    }

    ma_data_converter_process_pcm_frames(&user->miniaudio.data_converter,
        audio_data,
        &framesIn,
        newPage->pAudioData,
        &framesOut);
    result_ma = ma_paged_audio_buffer_data_append_page(&user->miniaudio.paged_audio_buffer_data, newPage);
    if (result_ma != MA_SUCCESS) {
        fprintf(stderr, "ma_paged_audio_buffer_data_append_page failed");
        ma_paged_audio_buffer_data_free_page(&user->miniaudio.paged_audio_buffer_data, newPage, NULL);
    }
    if (!ma_sound_is_playing(&user->miniaudio.sound)) {
        ma_sound_seek_to_pcm_frame(&user->miniaudio.sound, frame_length_at_start);

        ma_sound_start(&user->miniaudio.sound);
    }
}

static void copy_palette(Smack* smack) {
    SmackUserData *const user = smack->user;

    const unsigned char* pal = smk_get_palette(user->smk_handle);
    memcpy(smack->Palette, pal, 256 * 3);
}

Smack* SmackOpen(const char* name, uint32_t flags, uint32_t extrabuf) {
    double usf;
    Smack* smack = NULL;
    SmackUserData *user = NULL;

    smk smk_handle = smk_open_file(name, SMK_MODE_MEMORY);
    if (smk_handle == NULL) {
        goto error;
    }

    smack = malloc(sizeof(Smack));
    if (smack == NULL) {
        goto error;
    }
    memset(smack, 0, sizeof(*smack));

    user = malloc(sizeof(SmackUserData));
    if (user == NULL) {
        goto error;
    }
    memset(user, 0, sizeof(*user));
    smack->user = user;

    // libsmacker doesn't tell us whether the palette is new on each frame or not, so just assume it always is new
    smack->NewPalette = 1;

    // smk_handle is added to hold a pointer to the underlying libsmacker instance
    user->smk_handle = smk_handle;

    smk_info_all(smk_handle, NULL, &smack->Frames, &usf);
    smack->MSPerFrame = (unsigned long)(usf / 1000);
    smk_info_video(smk_handle, &smack->Width, &smack->Height, NULL);
    smk_enable_video(smk_handle, 1);

    audio_init(smack);

    if (smk_first(smk_handle) == SMK_ERROR) {
        goto error;
    }
    copy_palette(smack);
    return smack;
error:
    if (smk_handle) {
        smk_close(smk_handle);
    }
    free(user);
    free(smack);
    return NULL;
}

int SmackSoundUseDirectSound(void* dd) {
}

void SmackToBuffer(Smack* smack, uint32_t left, uint32_t top, uint32_t pitch, uint32_t destheight, void* buf, uint32_t flags) {
    int i, j;
    SmackUserData *const user = smack->user;

    // minimal implementation
    assert(left == 0);
    assert(top == 0);
    assert(flags == 0);

    char* char_buf = buf;

    const unsigned char* frame = smk_get_video(user->smk_handle);
    for (i = 0; i < smack->Height; i++) {
        memcpy(&char_buf[(i * pitch)], &frame[i * smack->Width], smack->Width);
    }
}

uint32_t SmackDoFrame(Smack* smack) {
    SmackUserData *const user = smack->user;

    user->smack_last_frame_time = gHarness_platform.GetTicks();

    audio_step(smack);

    return 0;
}

void SmackNextFrame(Smack* smack) {
    SmackUserData *const user = smack->user;

    smk_next(user->smk_handle);
    copy_palette(smack);
}

uint32_t SmackWait(Smack* smack) {
    SmackUserData *const user = smack->user;

    uint32_t now = gHarness_platform.GetTicks();
    if (now < user->smack_last_frame_time + smack->MSPerFrame - 5) {
        gHarness_platform.Sleep(1);
        return 1;
    }
    return 0;
}

void SmackClose(Smack* smack) {
    SmackUserData *const user = smack->user;

    audio_exit(smack);
    smk_close(user->smk_handle);
    free(user);
    free(smack);
}
