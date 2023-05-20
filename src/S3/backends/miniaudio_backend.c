
#include "harness/config.h"
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

// dethrace
ma_engine miniaudio_engine;

int AudioBackend_Init(void) {
    ma_result result;

    ma_engine_config engineConfig;
    engineConfig = ma_engine_config_init();

    result = ma_engine_init(&engineConfig, &miniaudio_engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.");
        return 0;
    }

    ma_engine_set_volume(&miniaudio_engine, harness_game_config.volume_multiplier);
    return 1;
}

int AudioBackend_UnInit(void) {
    ma_engine_uninit(&miniaudio_engine);
    return 1;
}

int AudioBackend_InitChannel(tS3_channel* chan) {
    tS3_sample_struct_miniaudio* sample_struct;
    sample_struct = S3MemAllocate(sizeof(tS3_sample_struct_miniaudio), kMem_S3_DOS_SOS_channel);
    if (sample_struct == NULL) {
        return 0;
    }
    memset(sample_struct, 0, sizeof(tS3_sample_struct_miniaudio));
    chan->type_struct_midi = NULL;
    chan->type_struct_cda = NULL;
    chan->type_struct_sample = (char*)sample_struct;
    return 1;
}

int AudioBackend_PlaySample(tS3_channel* chan) {
    tS3_sample_struct_miniaudio* miniaudio;
    tS3_sample* sample_data;
    ma_result result;

    miniaudio = (tS3_sample_struct_miniaudio*)chan->type_struct_sample;
    assert(miniaudio != NULL);
    sample_data = (tS3_sample*)chan->descriptor->sound_data;
    assert(sample_data != NULL);

    result = ma_audio_buffer_ref_init(ma_format_u8, sample_data->channels, sample_data->dataptr, sample_data->size / sample_data->channels, &miniaudio->buffer_ref);
    miniaudio->buffer_ref.sampleRate = sample_data->rate;
    if (result != MA_SUCCESS) {
        return 0;
    }
    result = ma_sound_init_from_data_source(&miniaudio_engine, &miniaudio->buffer_ref, MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_SPATIALIZATION, NULL, &miniaudio->sound);
    if (result != MA_SUCCESS) {
        return 0;
    }
    miniaudio->initialized = 1;

    ma_sound_set_looping(&miniaudio->sound, chan->repetitions == 0);
    ma_sound_start(&miniaudio->sound);
    return 1;
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

int AudioBackend_SetVolume(tS3_channel* chan, int volume_db) {
    tS3_sample_struct_miniaudio* miniaudio;
    float linear_volume;

    miniaudio = (tS3_sample_struct_miniaudio*)chan->type_struct_sample;
    assert(miniaudio != NULL);

    // convert from directsound -10000 - 0 decibel volume scale
    linear_volume = ma_volume_db_to_linear(volume_db / 100.0f);
    ma_sound_set_volume(&miniaudio->sound, linear_volume);
    return 1;
}

int AudioBackend_SetPan(tS3_channel* chan, int pan) {
    tS3_sample_struct_miniaudio* miniaudio;

    miniaudio = (tS3_sample_struct_miniaudio*)chan->type_struct_sample;
    assert(miniaudio != NULL);

    // convert from directsound -10000 - 10000 pan scale
    ma_sound_set_pan(&miniaudio->sound, pan / 10000.0f);
    return 1;
}

int AudioBackend_SetFrequency(tS3_channel* chan, int rate) {
    tS3_sample_struct_miniaudio* miniaudio;
    tS3_sample* sample_data;

    miniaudio = (tS3_sample_struct_miniaudio*)chan->type_struct_sample;
    assert(miniaudio != NULL);

    sample_data = (tS3_sample*)chan->descriptor->sound_data;

    // convert from directsound frequency to linear pitch scale
    ma_sound_set_pitch(&miniaudio->sound, (rate / (float)sample_data->rate));
    return 1;
}

int AudioBackend_StopSample(tS3_channel* chan) {
    tS3_sample_struct_miniaudio* miniaudio = (tS3_sample_struct_miniaudio*)chan->type_struct_sample;
    if (miniaudio == NULL) {
        return 0;
    }
    if (miniaudio->initialized) {
        ma_sound_stop(&miniaudio->sound);
        ma_sound_uninit(&miniaudio->sound);
        ma_audio_buffer_ref_uninit(&miniaudio->buffer_ref);
        miniaudio->initialized = 0;
    }
    return 1;
}
