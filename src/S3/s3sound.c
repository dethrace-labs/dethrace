#include "s3sound.h"
#include "audio.h"
#include "harness/config.h"
#include "miniaudio/miniaudio.h"
#include "resource.h"
#include "harness/trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void dr_dprintf(char* fmt_string, ...);

int gS3_sample_filter_funcs_registered;
tS3_sample_filter* gS3_sample_filter_func;
tS3_sample_filter* gS3_sample_filter_disable_func;

ma_engine engine;

int S3OpenSampleDevice() {
    ma_result result;

    ma_engine_config engineConfig;
    engineConfig = ma_engine_config_init();
    engineConfig.sampleRate = 22050;

    result = ma_engine_init(&engineConfig, &engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.");
        return 0;
    }

    ma_engine_set_volume(&engine, harness_game_config.volume_multiplier);

    S3Enable();
    return 1;
}

// Returns 0 if no error
int S3LoadSample(tS3_sound_id id) {
    // LPDIRECTSOUNDBUFFER WavFile; // eax
    char filename[MAX_PATH_LENGTH]; // [esp+10h] [ebp-5Ch] BYREF
    tS3_descriptor* descriptor;     // [esp+60h] [ebp-Ch]
    tS3_sample* sample;             // [esp+68h] [ebp-4h]

    if (!gS3_enabled) {
        return 0;
    }
    descriptor = S3GetDescriptorByID(id);
    if (!descriptor) {
        return eS3_error_bad_id;
    }
    if (descriptor->type != eS3_ST_sample) {
        return 0;
    }
    if (descriptor->sound_data) {
        return 0;
    }
    filename[0] = 0;
    strcpy(filename, descriptor->filename);
    sample = S3MemAllocate(sizeof(tS3_sample), kMem_S3_sound_header);
    if (!sample) {
        return eS3_error_memory;
    }

    memset(sample, 0, sizeof(tS3_sample));
    descriptor->sound_buffer = S3LoadWavFile(filename, sample);

    if (!descriptor->sound_buffer) {
        S3MemFree(sample);
        return gS3_last_error;
    }
    descriptor->special_fx = 0;
    descriptor->sound_data = (char*)sample;
    return eS3_error_none;
}

int S3ReadWavHeader(char* buf, tWAVEFORMATEX_** pWav_format, char** data_ptr, int* pData_size) {
    int riff_len;
    char* file_eof;         // [esp+10h] [ebp-14h]
    unsigned int chunk_len; // [esp+18h] [ebp-Ch]
    char* chunk_data;       // [esp+1Ch] [ebp-8h]
    // char* chunk_name;       // [esp+20h] [ebp-4h]
    char* chunk_ptr;

    if (pWav_format) {
        *pWav_format = 0;
    }
    if (data_ptr) {
        *data_ptr = 0;
    }
    if (pData_size) {
        *pData_size = 0;
    }
    chunk_ptr = buf + 12;
    if (buf[0] != 'R' || buf[1] != 'I' || buf[2] != 'F' || buf[3] != 'F') {
        return 0;
    }
    if (buf[8] != 'W' || buf[9] != 'A' || buf[10] != 'V' || buf[11] != 'E') {
        return 0;
    }
    memcpy(&riff_len, &buf[4], sizeof(riff_len)); // (int32_t)buf[4];
#if BR_ENDIAN_BIG
    riff_len = BrSwap32(riff_len);
#endif
    file_eof = &chunk_ptr[riff_len - 4];
    while (file_eof > chunk_ptr) {
        memcpy(&chunk_len, chunk_ptr + 4, sizeof(chunk_len));
#if BR_ENDIAN_BIG
        chunk_len = BrSwap32(chunk_len);
#endif
        chunk_data = chunk_ptr + 8;
        if (strncmp(chunk_ptr, "fmt ", 4) == 0) {
            if (pWav_format && *pWav_format == NULL) {
                if (chunk_len < 14) {
                    return 0;
                }
                *pWav_format = (tWAVEFORMATEX_*)chunk_data;
                if ((!data_ptr || *data_ptr) && (!pData_size || *pData_size)) {
                    return 1;
                }
            }
        } else if (strncmp(chunk_ptr, "data", 4) == 0 && ((data_ptr && !*data_ptr) || (pData_size && !*pData_size))) {
            if (data_ptr) {
                *data_ptr = chunk_data;
            }
            if (pData_size) {
                *pData_size = chunk_len;
            }
            if (!pWav_format || *pWav_format) {
                return 1;
            }
        }
        chunk_ptr = &chunk_data[(chunk_len + 1) & 0xFFFFFFFE];
    }
    return 0;
}

void* S3LoadWavFile(char* pFile_name, tS3_sample* pSample) {
    FILE* f;           // [esp+Ch] [ebp-C8h]
    size_t bytes_read; // [esp+14h] [ebp-C0h] BYREF
    //  unsigned int locked_buffer_data_len; // [esp+18h] [ebp-BCh] BYREF
    //   struct _OFSTRUCT ReOpenBuff;         // [esp+1Ch] [ebp-B8h] BYREF
    char* buf; // [esp+A4h] [ebp-30h]
    // LPDIRECTSOUNDBUFFER ds_buffer;       // [esp+A8h] [ebp-2Ch] BYREF
    // DSBUFFERDESC buffer_desc;   // [esp+ACh] [ebp-28h] BYREF
    int data_size;              // [esp+C0h] [ebp-14h] BYREF
    tWAVEFORMATEX_* wav_format; // [esp+C4h] [ebp-10h] BYREF
    char* data_ptr;             // [esp+C8h] [ebp-Ch] BYREF
    // char* locked_buffer_data;   // [esp+CCh] [ebp-8h] BYREF
    size_t file_len; // [esp+D0h] [ebp-4h]

    f = fopen(pFile_name, "r");
    if (f == NULL) {
        gS3_last_error = eS3_error_readfile;
        return 0;
    }
    fseek(f, 0, SEEK_END);
    file_len = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);

    buf = S3MemAllocate(file_len, kMem_S3_Windows_95_load_WAV_file);
    if (buf == NULL) {
        fclose(f);
        gS3_last_error = eS3_error_memory;
        return 0;
    }
    bytes_read = fread(buf, file_len, 1, f);
    fclose(f);

    data_size = 0;
    wav_format = 0;
    data_ptr = 0;
    if (S3ReadWavHeader(buf, &wav_format, &data_ptr, &data_size) == 0) {
        gS3_last_error = eS3_error_readfile;
        dr_dprintf("ERROR: .WAV file '%s' is crap", pFile_name);
        return 0;
    }
    pSample->freeptr = 0;
    pSample->dataptr = 0;
    pSample->size = data_size;
    pSample->rate = wav_format->nSamplesPerSec;
    pSample->resolution = wav_format->nAvgBytesPerSec;
    pSample->channels = wav_format->nChannels;
#if BR_ENDIAN_BIG
    pSample->rate = BrSwap32(pSample->rate);
    pSample->resolution = BrSwap32(pSample->resolution);
    pSample->channels = BrSwap32(pSample->channels);
#endif

    ma_sound* sound = malloc(sizeof(ma_sound));
    // TOOD: load from memory - we've already read the file data
    if (ma_sound_init_from_file(&engine, pFile_name, MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_SPATIALIZATION, NULL, NULL, sound) != MA_SUCCESS) {
        return NULL; // Failed to load sound.
    }
    S3MemFree(buf);
    return sound;

    // S3MemFree(buf);

    // buffer_desc.dwReserved = 0;
    // buffer_desc.dwSize = 20;
    // buffer_desc.dwFlags = 226;
    // buffer_desc.dwBufferBytes = data_size;
    // buffer_desc.lpwfxFormat = wav_format;
    // if (gS3_direct_sound_ptr->lpVtbl->CreateSoundBuffer(gS3_direct_sound_ptr, &buffer_desc, &ds_buffer, 0)) {
    //     return 0;
    // } else if (ds_buffer->lpVtbl->Lock(
    //                ds_buffer,
    //                0,
    //                data_size,
    //                (LPVOID*)&locked_buffer_data,
    //                &locked_buffer_data_len,
    //                0,
    //                0,
    //                0)) {
    //     return 0;
    // } else {
    //     qmemcpy(locked_buffer_data, data_ptr, locked_buffer_data_len);
    //     S3MemFree(buf);
    //     ds_buffer->lpVtbl->Unlock(ds_buffer, locked_buffer_data, locked_buffer_data_len, 0, 0);
    //     return ds_buffer;
    // }
    return NULL;
}

int S3StopSample(tS3_channel* chan) {
    if (chan->descriptor && chan->descriptor->type == chan->type) {
        ma_sound_stop(chan->descriptor->sound_buffer);
        ma_sound_seek_to_pcm_frame(chan->descriptor->sound_buffer, 0);

        // dsound_buffer = chan->descriptor->dsound_buffer;
        // if (dsound_buffer) {
        //     dsound_buffer->lpVtbl->Stop(dsound_buffer);
        //     dsound_buffer->lpVtbl->SetCurrentPosition(dsound_buffer, 0);
        // }
    }
    if (chan->active) {
        chan->needs_service = 1;
    }
    return 1;
}

int S3ExecuteSampleFilterFuncs(tS3_channel* chan) {
    if (((chan->descriptor->special_fx == 0) & gS3_sample_filter_funcs_registered) != 0) {
        gS3_sample_filter_func(1, chan->tag);
        chan->descriptor->special_fx = 1;
    } else if (((gS3_sample_filter_funcs_registered == 0) & chan->descriptor->special_fx) != 0) {
        gS3_sample_filter_disable_func(1, chan->tag);
        chan->descriptor->special_fx = 0;
    }
    return 0;
}

int S3PlaySample(tS3_channel* chan) {

    S3SyncSampleVolume(chan);
    S3SyncSampleRate(chan);
    if (chan->descriptor && chan->descriptor->type == chan->type) {
        ma_sound_seek_to_pcm_frame(chan->descriptor->sound_buffer, 0);
        ma_sound_set_looping(chan->descriptor->sound_buffer, chan->repetitions == 0);
        ma_sound_start(chan->descriptor->sound_buffer);
        // Mix_PlayChannel(chan->id, chan->descriptor->sound_buffer, 0);
    }
    // BOOL play_flags;                   // [esp+Ch] [ebp-Ch]
    // int status;                        // [esp+10h] [ebp-8h] BYREF
    // LPDIRECTSOUNDBUFFER dsound_buffer; // [esp+14h] [ebp-4h]

    // S3SampleSyncVolume(chan);
    // S3SampleSyncRate(chan);
    // if (chan->descriptor && chan->descriptor->type == chan->type) {
    //     dsound_buffer = chan->descriptor->dsound_buffer;
    //     if (dsound_buffer) {
    //         dsound_buffer->lpVtbl->SetCurrentPosition(dsound_buffer, 0);
    //         play_flags = chan->repetitions == 0; // 1 = DSBPLAY_LOOPING
    //         dsound_buffer->lpVtbl->Play(dsound_buffer, 0, 0, play_flags);
    //         if (!dsound_buffer->lpVtbl->GetStatus(dsound_buffer, (LPDWORD)&status)) {
    //             if ((status & 1) != 0) // DSBSTATUS_PLAYING
    //             {
    //                 dsound_buffer->lpVtbl->SetCurrentPosition(dsound_buffer, 0);
    //             } else {
    //                 dsound_buffer->lpVtbl->Play(dsound_buffer, 0, 0, play_flags);
    //             }
    //         }
    //     }
    // }
    return 1;
}

int sub_49D837(tS3_channel* chan) {
    return 1;
}

int sub_49D84C(tS3_channel* chan) {
    return 1;
}

int S3SyncSampleVolume(tS3_channel* chan) {

    float pan_ratio; // [esp+38h] [ebp-8h]
    float total_vol; // [esp+3Ch] [ebp-4h]

    int volume_db;
    int pan;
    float linear_volume;

    if (chan->type != eS3_ST_sample) {
        return 1;
    }
    total_vol = chan->left_volume + chan->right_volume;
    if (total_vol == 0.0f) {
        total_vol = 1.0f;
    }
    if (chan->descriptor && chan->descriptor->type == chan->type) {
        volume_db = 510.0f / total_vol * -5.0f - 350.0f;
        if (volume_db >= 0) {
            volume_db = 0;
        }

        // convert from directsound -10000-0 volume scale
        linear_volume = ma_volume_db_to_linear(volume_db / 100.0f);
        ma_sound_set_volume(chan->descriptor->sound_buffer, linear_volume);

        if (chan->spatial_sound) {
            if (chan->left_volume != 0 && chan->right_volume > chan->left_volume) {
                pan_ratio = chan->right_volume / (float)chan->left_volume;
            } else if (chan->right_volume != 0) {
                pan_ratio = chan->left_volume / (float)chan->right_volume;
            }
            if (chan->left_volume != 0 && chan->right_volume != 0) {
                pan = (chan->right_volume - chan->left_volume) * pan_ratio;
                pan = MAX(pan, -10000);
                pan = MIN(pan, 10000);
            } else if (chan->left_volume != 0) {
                pan = -10000;
            } else {
                pan = 10000;
            }
            ma_sound_set_pan(chan->descriptor->sound_buffer, pan / 10000.0f);
        }
    }
    return 1;
}

int S3SyncSampleRate(tS3_channel* chan) {
    if (chan->type != eS3_ST_sample) {
        return 1;
    }
    if (chan->descriptor && chan->descriptor->type == chan->type) {
        if (chan->descriptor->sound_buffer != NULL) {
            int rate = chan->rate;
            if (rate >= 100000) {
                rate = 100000;
            }
            //  sound_buffer->lpVtbl->SetFrequency(sound_buffer, rate);
            // miniaudio uses a linear pitch scale instead of sample rate, so scale it down
            ma_sound_set_pitch(chan->descriptor->sound_buffer, (rate / (float)((tS3_sample*)chan->descriptor->sound_data)->rate));
        }
    }
    return 1;
}

void S3SetEffects(tS3_sample_filter* filter1, tS3_sample_filter* filter2) {
    STUB_ONCE();
}
