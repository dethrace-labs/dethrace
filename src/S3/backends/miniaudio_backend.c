// Disable miniaudio's 'null' device fallback. A proper device must be found to enable playback
#define MA_NO_NULL

#include "backend.h"
#include "harness/config.h"
#include "harness/trace.h"
#include "miniaudio/miniaudio.h"
#include "resource.h"
#include "s3_defs.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct tS3_sample_struct_miniaudio {
    ma_audio_buffer_ref buffer_ref;
    ma_sound sound;
    int initialized;
} tS3_sample_struct_miniaudio;

ma_engine miniaudio_engine;

tAudioBackend_error_code AudioBackend_Init(void) {
    ma_result result;

    ma_engine_config engineConfig;
    engineConfig = ma_engine_config_init();
    result = ma_engine_init(&engineConfig, &miniaudio_engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.");
        return eAB_error;
    }
    LOG_INFO("Playback device: '%s'", miniaudio_engine.pDevice->playback.name);
    ma_engine_set_volume(&miniaudio_engine, harness_game_config.volume_multiplier);
    return eAB_success;
}

tAudioBackend_error_code AudioBackend_InitCDA(void) {
    return eAB_error;
}

void AudioBackend_UnInit(void) {
    ma_engine_uninit(&miniaudio_engine);
}

void AudioBackend_UnInitCDA(void) {
}

void* AudioBackend_AllocateSampleTypeStruct(void) {
    tS3_sample_struct_miniaudio* sample_struct;
    sample_struct = S3MemAllocate(sizeof(tS3_sample_struct_miniaudio), kMem_S3_DOS_SOS_channel);
    if (sample_struct == NULL) {
        return 0;
    }
    memset(sample_struct, 0, sizeof(tS3_sample_struct_miniaudio));
    return sample_struct;
}

tAudioBackend_error_code AudioBackend_PlaySample(tS3_channel* chan) {
    tS3_sample_struct_miniaudio* miniaudio;
    tS3_sample* sample_data;
    ma_result result;
    ma_int32 flags;

    miniaudio = (tS3_sample_struct_miniaudio*)chan->type_struct_sample;
    assert(miniaudio != NULL);
    sample_data = (tS3_sample*)chan->descriptor->sound_data;
    assert(sample_data != NULL);

    result = ma_audio_buffer_ref_init(ma_format_u8, sample_data->channels, sample_data->dataptr, sample_data->size / sample_data->channels, &miniaudio->buffer_ref);
    miniaudio->buffer_ref.sampleRate = sample_data->rate;
    if (result != MA_SUCCESS) {
        return eAB_error;
    }

    flags = MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_SPATIALIZATION;
    result = ma_sound_init_from_data_source(&miniaudio_engine, &miniaudio->buffer_ref, flags, NULL, &miniaudio->sound);
    if (result != MA_SUCCESS) {
        return eAB_error;
    }
    miniaudio->initialized = 1;

    ma_sound_set_looping(&miniaudio->sound, chan->repetitions == 0);
    ma_sound_start(&miniaudio->sound);
    return eAB_success;
}

int AudioBackend_SoundIsPlaying(tS3_channel* chan) {
    tS3_sample_struct_miniaudio* miniaudio;

    miniaudio = (tS3_sample_struct_miniaudio*)chan->type_struct_sample;
    assert(miniaudio != NULL);

    if (ma_sound_is_playing(&miniaudio->sound)) {
        return 1;
    }
    return 0;
}

tAudioBackend_error_code AudioBackend_SetVolume(tS3_channel* chan, int volume_db) {
    tS3_sample_struct_miniaudio* miniaudio;
    float linear_volume;

    miniaudio = (tS3_sample_struct_miniaudio*)chan->type_struct_sample;
    assert(miniaudio != NULL);

    // convert from directsound -10000 - 0 decibel volume scale
    linear_volume = ma_volume_db_to_linear(volume_db / 100.0f);
    ma_sound_set_volume(&miniaudio->sound, linear_volume);
    return eAB_success;
}

tAudioBackend_error_code AudioBackend_SetPan(tS3_channel* chan, int pan) {
    tS3_sample_struct_miniaudio* miniaudio;

    miniaudio = (tS3_sample_struct_miniaudio*)chan->type_struct_sample;
    assert(miniaudio != NULL);

    // convert from directsound -10000 - 10000 pan scale
    ma_sound_set_pan(&miniaudio->sound, pan / 10000.0f);
    return eAB_success;
}

tAudioBackend_error_code AudioBackend_SetFrequency(tS3_channel* chan, int rate) {
    tS3_sample_struct_miniaudio* miniaudio;
    tS3_sample* sample_data;

    miniaudio = (tS3_sample_struct_miniaudio*)chan->type_struct_sample;
    assert(miniaudio != NULL);

    sample_data = (tS3_sample*)chan->descriptor->sound_data;

    // convert from directsound frequency to linear pitch scale
    ma_sound_set_pitch(&miniaudio->sound, (rate / (float)sample_data->rate));
    return eAB_success;
}

tAudioBackend_error_code AudioBackend_StopSample(tS3_channel* chan) {
    tS3_sample_struct_miniaudio* miniaudio;

    miniaudio = (tS3_sample_struct_miniaudio*)chan->type_struct_sample;
    assert(miniaudio != NULL);

    if (miniaudio->initialized) {
        ma_sound_stop(&miniaudio->sound);
        ma_sound_uninit(&miniaudio->sound);
        ma_audio_buffer_ref_uninit(&miniaudio->buffer_ref);
        miniaudio->initialized = 0;
    }
    return eAB_success;
}
