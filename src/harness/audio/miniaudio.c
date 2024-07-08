// Disable miniaudio's 'null' device fallback. A proper device must be found to enable playback
#define MA_NO_NULL

#include "miniaudio/miniaudio.h"
#include "harness/audio.h"
#include "harness/config.h"
#include "harness/trace.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static int kMem_S3_DOS_SOS_channel = 234;

typedef struct tS3_sample_struct_miniaudio {
    ma_audio_buffer_ref buffer_ref;
    ma_sound sound;
    int initialized;
} tS3_sample_struct_miniaudio;

typedef struct tAudioBackend_stream_miniaudio {
    int frame_size_in_bytes;
    int sample_rate;
    int needs_converting;
    ma_paged_audio_buffer_data paged_audio_buffer_data;
    ma_paged_audio_buffer paged_audio_buffer;
    ma_data_converter data_converter;
    ma_sound sound;

} tAudioBackend_stream_miniaudio;

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
    sample_struct = BrMemAllocate(sizeof(tS3_sample_struct_miniaudio), kMem_S3_DOS_SOS_channel);
    if (sample_struct == NULL) {
        return 0;
    }
    memset(sample_struct, 0, sizeof(tS3_sample_struct_miniaudio));
    return sample_struct;
}

tAudioBackend_error_code AudioBackend_PlaySample(void* type_struct_sample, int channels, void* data, int size, int rate, int loop) {
    tS3_sample_struct_miniaudio* miniaudio;
    // tS3_sample* sample_data;
    ma_result result;
    ma_int32 flags;

    miniaudio = (tS3_sample_struct_miniaudio*)type_struct_sample;
    assert(miniaudio != NULL);
    // sample_data = (tS3_sample*)chan->descriptor->sound_data;
    // assert(sample_data != NULL);

    result = ma_audio_buffer_ref_init(ma_format_u8, channels, data, size / channels, &miniaudio->buffer_ref);
    miniaudio->buffer_ref.sampleRate = rate;
    if (result != MA_SUCCESS) {
        return eAB_error;
    }

    flags = MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_SPATIALIZATION;
    result = ma_sound_init_from_data_source(&miniaudio_engine, &miniaudio->buffer_ref, flags, NULL, &miniaudio->sound);
    if (result != MA_SUCCESS) {
        return eAB_error;
    }
    miniaudio->initialized = 1;

    ma_sound_set_looping(&miniaudio->sound, loop);
    ma_sound_start(&miniaudio->sound);
    return eAB_success;
}

int AudioBackend_SoundIsPlaying(void* type_struct_sample) {
    tS3_sample_struct_miniaudio* miniaudio;

    miniaudio = (tS3_sample_struct_miniaudio*)type_struct_sample;
    assert(miniaudio != NULL);

    if (ma_sound_is_playing(&miniaudio->sound)) {
        return 1;
    }
    return 0;
}

tAudioBackend_error_code AudioBackend_SetVolume(void* type_struct_sample, int volume_db) {
    tS3_sample_struct_miniaudio* miniaudio;
    float linear_volume;

    miniaudio = (tS3_sample_struct_miniaudio*)type_struct_sample;
    assert(miniaudio != NULL);

    // convert from directsound -10000 - 0 decibel volume scale
    linear_volume = ma_volume_db_to_linear(volume_db / 100.0f);
    ma_sound_set_volume(&miniaudio->sound, linear_volume);
    return eAB_success;
}

tAudioBackend_error_code AudioBackend_SetPan(void* type_struct_sample, int pan) {
    tS3_sample_struct_miniaudio* miniaudio;

    miniaudio = (tS3_sample_struct_miniaudio*)type_struct_sample;
    assert(miniaudio != NULL);

    // convert from directsound -10000 - 10000 pan scale
    ma_sound_set_pan(&miniaudio->sound, pan / 10000.0f);
    return eAB_success;
}

tAudioBackend_error_code AudioBackend_SetFrequency(void* type_struct_sample, int original_rate, int new_rate) {
    tS3_sample_struct_miniaudio* miniaudio;

    miniaudio = (tS3_sample_struct_miniaudio*)type_struct_sample;
    assert(miniaudio != NULL);

    // convert from directsound frequency to linear pitch scale
    ma_sound_set_pitch(&miniaudio->sound, (new_rate / (float)original_rate));
    return eAB_success;
}

tAudioBackend_error_code AudioBackend_StopSample(void* type_struct_sample) {
    tS3_sample_struct_miniaudio* miniaudio;

    miniaudio = (tS3_sample_struct_miniaudio*)type_struct_sample;
    assert(miniaudio != NULL);

    if (miniaudio->initialized) {
        ma_sound_stop(&miniaudio->sound);
        ma_sound_uninit(&miniaudio->sound);
        ma_audio_buffer_ref_uninit(&miniaudio->buffer_ref);
        miniaudio->initialized = 0;
    }
    return eAB_success;
}

tAudioBackend_stream* AudioBackend_StreamOpen(int bit_depth, int channels, int sample_rate) {
    tAudioBackend_stream_miniaudio* new;
    ma_data_converter_config data_converter_config;

    new = malloc(sizeof(tAudioBackend_stream_miniaudio));
    new->sample_rate = sample_rate;
    ma_format format;
    switch (bit_depth) {
    case 8:
        format = ma_format_u8;
        new->frame_size_in_bytes = 1 * channels;
        break;
    case 16:
        format = ma_format_s16;
        new->frame_size_in_bytes = 2 * channels;
        break;
    case 24:
        format = ma_format_s24;
        new->frame_size_in_bytes = 3 * channels;
        break;
    case 32:
        format = ma_format_s32;
        new->frame_size_in_bytes = 4 * channels;
        break;
    default:
        goto failed;
    }

    if ((new->frame_size_in_bytes == 0) || (ma_paged_audio_buffer_data_init(format, channels, &new->paged_audio_buffer_data) != MA_SUCCESS)) {
        LOG_WARN("Failed to create paged audio buffer data");
        goto failed;
    }

    ma_paged_audio_buffer_config paged_audio_buffer_config = ma_paged_audio_buffer_config_init(&new->paged_audio_buffer_data);
    if (ma_paged_audio_buffer_init(&paged_audio_buffer_config, &new->paged_audio_buffer) != MA_SUCCESS) {
        LOG_WARN("Failed to create paged audio buffer for smacker audio stream");
        goto failed;
    }

    if (ma_sound_init_from_data_source(&miniaudio_engine, &new->paged_audio_buffer, MA_SOUND_FLAG_NO_PITCH | MA_SOUND_FLAG_NO_SPATIALIZATION, NULL, &new->sound) != MA_SUCCESS) {
        LOG_WARN("Failed to create sound from data source");
        goto failed;
    }

    // allocate and initialize data converter if miniaudio engine and Smack file soundtrack sample rates differ
    if (ma_engine_get_sample_rate(&miniaudio_engine) != sample_rate) {
        new->needs_converting = 1;
        data_converter_config = ma_data_converter_config_init(format, format, channels, channels, sample_rate, ma_engine_get_sample_rate(&miniaudio_engine));
        if (ma_data_converter_init(&data_converter_config, NULL, &new->data_converter) != MA_SUCCESS) {
            LOG_WARN("Failed to create sound data converter");
            goto failed;
        }
    }
    return new;

failed:
    free(new);
    return NULL;
}

tAudioBackend_error_code AudioBackend_StreamWrite(void* stream_handle, unsigned char* data, unsigned long size) {
    tAudioBackend_stream_miniaudio* stream = stream_handle;
    ma_uint64 nb_frames_in;
    ma_uint64 nb_frames_out;
    ma_uint64 current_pos;
    ma_paged_audio_buffer_page* new_page;

    if (ma_paged_audio_buffer_get_length_in_pcm_frames(&stream->paged_audio_buffer, &current_pos) != MA_SUCCESS) {
        LOG_WARN("ma_paged_audio_buffer_get_length_in_pcm_frames failed");
        return eAB_error;
    }

    // do we need to convert the sample frequency?
    if (stream->needs_converting) {
        nb_frames_in = size / stream->frame_size_in_bytes;
        nb_frames_out = nb_frames_in * ma_engine_get_sample_rate(&miniaudio_engine) / stream->sample_rate;

        if (ma_paged_audio_buffer_data_allocate_page(&stream->paged_audio_buffer_data, nb_frames_out, NULL, NULL, &new_page) != MA_SUCCESS) {
            LOG_WARN("ma_paged_audio_buffer_data_allocate_page failed");
            return eAB_error;
        }
        if (ma_data_converter_process_pcm_frames(&stream->data_converter, data, &nb_frames_in, new_page->pAudioData, &nb_frames_out) != MA_SUCCESS) {
            LOG_WARN("ma_data_converter_process_pcm_frames failed");
            return eAB_error;
        }
        if (ma_paged_audio_buffer_data_append_page(&stream->paged_audio_buffer_data, new_page) != MA_SUCCESS) {
            LOG_WARN("ma_paged_audio_buffer_data_append_page failed");
            return eAB_error;
        }
    } else { // no sampling frequency conversion needed
        if (ma_paged_audio_buffer_data_allocate_and_append_page(&stream->paged_audio_buffer_data, (ma_uint32)(size / (ma_uint64)stream->frame_size_in_bytes), data, NULL) != MA_SUCCESS) {
            LOG_WARN("ma_paged_audio_buffer_data_allocate_and_append_page failed");
            return eAB_error;
        }
    }

    if (!ma_sound_is_playing(&stream->sound)) {
        // seek either at start, or where the accumulated value hasn't played yet
        if (ma_sound_seek_to_pcm_frame(&stream->sound, current_pos) != MA_SUCCESS) {
            LOG_WARN("ma_sound_seek_to_pcm_frame failed");
        }
        if (ma_sound_start(&stream->sound) != MA_SUCCESS) {
            LOG_WARN("ma_sound_start failed");
        }
    }
    if (ma_sound_at_end(&stream->sound)) {
        LOG_WARN("video not playing fast enough: sound starved!");
    }
    return eAB_success;
}

tAudioBackend_error_code AudioBackend_StreamClose(tAudioBackend_stream* stream_handle) {
    tAudioBackend_stream_miniaudio* stream = stream_handle;
    if (stream->needs_converting) {
        ma_data_converter_uninit(&stream->data_converter, NULL);
    }
    ma_sound_stop(&stream->sound);
    ma_sound_uninit(&stream->sound);
    ma_paged_audio_buffer_uninit(&stream->paged_audio_buffer);
    ma_paged_audio_buffer_data_uninit(&stream->paged_audio_buffer_data, NULL);

    free(stream);
    return eAB_success;
}
