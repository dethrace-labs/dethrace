#include "harness/hooks.h"

#include "harness/os.h"
#include "harness/trace.h"

#include <miniaudio/miniaudio.h>
#include <smacker.h>

#include <stdlib.h>

extern ma_engine engine;
extern void PossibleService(void);

typedef struct smacker_audio {
    ma_paged_audio_buffer paged_audio_buffer;
    ma_paged_audio_buffer_data paged_audio_buffer_data;
    ma_sound sound;
    ma_uint32 sample_rate;
    ma_data_converter data_converter;
    int frame_size_in_bytes;
    smk smack_handle;
} smacker_audio;

int Harness_Hook_smacker_audio_init(smacker_audio** ppSmackerAudio, void* smack_handle) {
    unsigned char track_mask_smk;
    unsigned char channels_smk[7];
    unsigned char bitdepth_smk[7];
    unsigned long sample_rate_smk[7];

    ma_result result_ma;
    ma_format audioformat_ma;
    ma_paged_audio_buffer_config paged_audio_buffer_config;
    ma_data_converter_config dataConverterConfig;

    *ppSmackerAudio = malloc(sizeof(smacker_audio));
    
    smk_info_audio(smack_handle, &track_mask_smk, channels_smk, bitdepth_smk, sample_rate_smk);
    if (!(track_mask_smk & SMK_AUDIO_TRACK_0)) {
        goto error_base;
    }
    (*ppSmackerAudio)->smack_handle = smack_handle;
    (*ppSmackerAudio)->sample_rate = sample_rate_smk[0];

    switch (bitdepth_smk[0]) {
    case 8:
        audioformat_ma = ma_format_u8;
        (*ppSmackerAudio)->frame_size_in_bytes = 1 * channels_smk[0];
        break;
    case 16:
        audioformat_ma = ma_format_s16;
        (*ppSmackerAudio)->frame_size_in_bytes = 2 * channels_smk[0];
        break;
    case 24:
        audioformat_ma = ma_format_s24;
        (*ppSmackerAudio)->frame_size_in_bytes = 3 * channels_smk[0];
        break;
    case 32:
        audioformat_ma = ma_format_s32;
        (*ppSmackerAudio)->frame_size_in_bytes = 4 * channels_smk[0];
        break;
    default:
        LOG_WARN("Smacker audio stream has invalid bit depth: %d", bitdepth_smk[0]);
        goto error_base;
    }

    result_ma = ma_paged_audio_buffer_data_init(audioformat_ma, channels_smk[0], &(*ppSmackerAudio)->paged_audio_buffer_data);
    if (result_ma != MA_SUCCESS) {
        fprintf(stderr, "Failed to create paged audio buffer data");
        goto error_base;
    }

    paged_audio_buffer_config = ma_paged_audio_buffer_config_init(&(*ppSmackerAudio)->paged_audio_buffer_data);
    result_ma = ma_paged_audio_buffer_init(&paged_audio_buffer_config, &(*ppSmackerAudio)->paged_audio_buffer);
    if (result_ma != MA_SUCCESS) {
        fprintf(stderr, "Failed to create paged audio buffer for smacker audio stream");
        goto error_paged_audio_buffer_data;
    }

    result_ma = ma_sound_init_from_data_source(&engine, &(*ppSmackerAudio)->paged_audio_buffer, MA_SOUND_FLAG_NO_PITCH | MA_SOUND_FLAG_NO_SPATIALIZATION, NULL, &(*ppSmackerAudio)->sound);
    if (result_ma != MA_SUCCESS) {
        fprintf(stderr, "Failed to create sound from data source\n");
        goto error_paged_audio_buffer;
    }

    dataConverterConfig = ma_data_converter_config_init(audioformat_ma, audioformat_ma, channels_smk[0], channels_smk[0], (ma_uint32)sample_rate_smk[0], ma_engine_get_sample_rate(&engine));
    result_ma = ma_data_converter_init(&dataConverterConfig, NULL, &(*ppSmackerAudio)->data_converter);
    if (result_ma != MA_SUCCESS) {
        fprintf(stderr, "Failed to create data converter\n");
        goto error_sound;
    }
    return 0;
error_sound:
    ma_sound_uninit(&(*ppSmackerAudio)->sound);
error_paged_audio_buffer:
    ma_paged_audio_buffer_uninit(&(*ppSmackerAudio)->paged_audio_buffer);
error_paged_audio_buffer_data:
    ma_paged_audio_buffer_data_uninit(&(*ppSmackerAudio)->paged_audio_buffer_data, NULL);
error_base:
    free(*ppSmackerAudio);
    return 1;
}

void Harness_Hook_smacker_audio_uninit(smacker_audio* pSmackerAudio) {
    while (ma_sound_is_playing(&pSmackerAudio->sound)) {
        PossibleService();
        OS_Sleep(1);
    }
    ma_sound_stop(&pSmackerAudio->sound);
    ma_data_converter_uninit(&pSmackerAudio->data_converter, NULL);
    ma_sound_uninit(&pSmackerAudio->sound);
    ma_paged_audio_buffer_uninit(&pSmackerAudio->paged_audio_buffer);
    ma_paged_audio_buffer_data_uninit(&pSmackerAudio->paged_audio_buffer_data, NULL);
}

void Harness_Hook_smacker_audio_step(smacker_audio* pSmackerAudio) {
    ma_result result_ma;
    const unsigned char *audio_data = smk_get_audio(pSmackerAudio->smack_handle, 0);
    unsigned long audio_data_size = smk_get_audio_size(pSmackerAudio->smack_handle, 0);

    if (audio_data == NULL) {
        return;
    }

    ma_uint64 frame_length_at_start;
    result_ma = ma_paged_audio_buffer_get_length_in_pcm_frames(&pSmackerAudio->paged_audio_buffer, &frame_length_at_start);

    ma_uint64 framesIn = audio_data_size / (ma_uint64)pSmackerAudio->frame_size_in_bytes;
    ma_uint64 framesOut = framesIn * ma_engine_get_sample_rate(&engine) / pSmackerAudio->sample_rate;

    ma_paged_audio_buffer_page *newPage;
    result_ma = ma_paged_audio_buffer_data_allocate_page(&pSmackerAudio->paged_audio_buffer_data, framesOut, NULL, NULL, &newPage);
    if (result_ma != MA_SUCCESS) {
        LOG_WARN("ma_paged_audio_buffer_data_allocate_page failed");
        return;
    }

    ma_data_converter_process_pcm_frames(&pSmackerAudio->data_converter,
        audio_data,
        &framesIn,
        newPage->pAudioData,
        &framesOut);
    result_ma = ma_paged_audio_buffer_data_append_page(&pSmackerAudio->paged_audio_buffer_data, newPage);
    if (result_ma != MA_SUCCESS) {
        LOG_WARN("ma_paged_audio_buffer_data_append_page failed");
        ma_paged_audio_buffer_data_free_page(&pSmackerAudio->paged_audio_buffer_data, newPage, NULL);
    }
    if (!ma_sound_is_playing(&pSmackerAudio->sound)) {
        ma_sound_seek_to_pcm_frame(&pSmackerAudio->sound, frame_length_at_start);

        ma_sound_start(&pSmackerAudio->sound);
    }
}
