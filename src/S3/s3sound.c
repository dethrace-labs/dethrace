#include "s3sound.h"
#include "audio.h"
#include "resource.h"
#include <stdio.h>

int gS3_sample_filter_funcs_registered;
tS3_sample_filter* gS3_sample_filter_func;
tS3_sample_filter* gS3_sample_filter_disable_func;

#include <SDL_mixer.h>

int S3OpenSampleDevice() {

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        return 0;
    }

    Mix_AllocateChannels(20);
    // if (DirectSoundCreate(0, &gS3_direct_sound_ptr, 0)) {
    //     return 0;
    // }
    // if (gS3_direct_sound_ptr->lpVtbl->SetCooperativeLevel(gS3_direct_sound_ptr, hWnd, 3)) {
    //     return 0;
    // }
    S3Enable();
    return 1;
}

int S3LoadSample(tS3_sound_id id) {
    // LPDIRECTSOUNDBUFFER WavFile; // eax
    char filename[80];          // [esp+10h] [ebp-5Ch] BYREF
    tS3_descriptor* descriptor; // [esp+60h] [ebp-Ch]
    tS3_sample* sample;         // [esp+68h] [ebp-4h]

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
    return 0;
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
    file_eof = &chunk_ptr[riff_len - 4];
    while (file_eof > chunk_ptr) {
        memcpy(&chunk_len, chunk_ptr + 4, sizeof(chunk_len));
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
    FILE* f;                             // [esp+Ch] [ebp-C8h]
    size_t bytes_read;                   // [esp+14h] [ebp-C0h] BYREF
    unsigned int locked_buffer_data_len; // [esp+18h] [ebp-BCh] BYREF
    // struct _OFSTRUCT ReOpenBuff;         // [esp+1Ch] [ebp-B8h] BYREF
    char* buf; // [esp+A4h] [ebp-30h]
    // LPDIRECTSOUNDBUFFER ds_buffer;       // [esp+A8h] [ebp-2Ch] BYREF
    // DSBUFFERDESC buffer_desc;   // [esp+ACh] [ebp-28h] BYREF
    int data_size;              // [esp+C0h] [ebp-14h] BYREF
    tWAVEFORMATEX_* wav_format; // [esp+C4h] [ebp-10h] BYREF
    char* data_ptr;             // [esp+C8h] [ebp-Ch] BYREF
    char* locked_buffer_data;   // [esp+CCh] [ebp-8h] BYREF
    size_t file_len;            // [esp+D0h] [ebp-4h]

    locked_buffer_data = 0;
    locked_buffer_data_len = 0;
    // f = OpenFile(lpFileName, &ReOpenBuff, 0);
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

    Mix_Chunk* raw_chunk = Mix_LoadWAV(pFile_name); // Mix_QuickLoad_RAW((Uint8*)data_ptr, data_size);
    if (raw_chunk == NULL) {
        return NULL;
    }
    // S3MemFree(buf);
    return raw_chunk;
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
    // LPDIRECTSOUNDBUFFER dsound_buffer; // [esp+Ch] [ebp-4h]

    if (chan->descriptor && chan->descriptor->type == chan->type) {
        if (Mix_Playing(chan->id)) {
            printf("stopping channel %d\n", chan->id);
        }
        Mix_HaltChannel(chan->id);
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
    printf("playing on %d\n", chan->id);

    if (Mix_Playing(chan->id)) {
        printf("WARN already playing\n");
    }
    int left = 0;
    Mix_SetPanning(chan->id, left, 255 - left);
    Mix_PlayChannel(chan->id, chan->descriptor->sound_buffer, 0);
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

int S3SampleSyncVolume(tS3_channel* chan) {

    return 0;
}
