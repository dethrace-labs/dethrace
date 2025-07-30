#include "s3.h"
#include "resource.h"

#include "3d.h"
#include "harness/audio.h"
#include "harness/config.h"
#include "harness/os.h"
#include "harness/trace.h"
#include "s3/s3.h"
#include "s3cda.h"
#include "s3music.h"
#include "s3sound.h"
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int PDGetTotalTime(void);

int gS3_enabled;
int gS3_noutlets;
int gS3_nsound_sources;
int gS3_next_outlet_id;
tS3_outlet* gS3_outlets;
tS3_sound_source* gS3_sound_sources;
tS3_descriptor* gS3_descriptors;
tS3_descriptor* gS3_root_descriptor;
int gS3_opened_output_devices;
int gS3_last_service_time;
int gS3_service_time_delta;
tS3_channel* gS3_unbound_channels;
tS3_channel* gS3_last_unbound_channel;
int gS3_last_error;
int gS3_default_pitch;
tS3_hardware_info gS3_hardware_info;
char* gS3_soundbank_buffer;
int gS3_soundbank_buffer_len;
int gS3_inside_cockpit;

tS3_channel gS3_channel_template;
int gS3_tag_seed;

char gS3_directory_separator[4];
char gS3_directory_name[8];
int gS3_have_current_dir;
char gS3_current_dir[260];

int dword_5216C0;

int S3Init(char* pPath, int pLow_memory_mode) {
    tS3_descriptor* root_descriptor;

    gS3_noutlets = 0;
    gS3_nsound_sources = 0;
    gS3_next_outlet_id = 0;
    gS3_outlets = NULL;
    gS3_sound_sources = NULL;
    gS3_descriptors = NULL;
    gS3_root_descriptor = NULL;

    S3Disable();
    S3DisableMIDI();
    S3DisableCDA();
    gS3_sample_filter_funcs_registered = 0;
    gS3_sample_filter_func = NULL;
    gS3_sample_filter_disable_func = NULL;
    if (S3OpenOutputDevices() == 0) {
        return 1;
    }
    gS3_opened_output_devices = 1;
    root_descriptor = S3MemAllocate(sizeof(tS3_descriptor), kMem_S3_sentinel);
    if (!root_descriptor) {
        return 3;
    }
    memset(root_descriptor, 0, sizeof(tS3_descriptor));
    root_descriptor->id = 2495081;
    gS3_root_descriptor = root_descriptor;
    gS3_descriptors = root_descriptor;
    if (S3LoadSoundbank(pPath, pLow_memory_mode)) {
        return 5;
    }
    gS3_last_service_time = PDGetTotalTime();
    gS3_unbound_channels = 0;
    gS3_last_unbound_channel = 0;
    return 0;
}

void S3Shutdown(void) {
    tS3_outlet* outlet;              // [esp+10h] [ebp-10h]
    tS3_outlet* next_outlet;         // [esp+14h] [ebp-Ch]
    tS3_descriptor* next_descriptor; // [esp+18h] [ebp-8h]
    tS3_descriptor* descriptor;      // [esp+1Ch] [ebp-4h]

    S3StopAllOutletSounds();
    S3DisableMIDI();
    S3DisableCDA();
    if (gS3_enabled) {
        S3Disable();
        for (descriptor = gS3_descriptors; descriptor != NULL; descriptor = next_descriptor) {
            next_descriptor = descriptor->next;
            S3ReleaseSound(descriptor->id);
            S3MemFree(descriptor);
        }
        for (outlet = gS3_outlets; outlet != NULL; outlet = next_outlet) {
            next_outlet = outlet->next;
            S3ReleaseOutlet(outlet);
        }
        S3ReleaseUnboundChannels();
    }
    if (gS3_opened_output_devices) {
        S3CloseDevices();
    }
}

void S3Enable(void) {
    gS3_enabled = 1;
}

void S3Disable(void) {
    S3StopAllOutletSounds();
    gS3_enabled = 0;
}

int S3OpenOutputDevices(void) {

    // strcpy(gS3_directory_separator, "\\");
    strcpy(gS3_directory_separator, "/");
    strcpy(gS3_directory_name, "SOUND");
    memset(&gS3_hardware_info, 0, sizeof(gS3_hardware_info));
    if (S3OpenSampleDevice() == 0) {
        return 0;
    }
    S3OpenCDADevice();
    gS3_hardware_info.timer_installed = 0;
    gS3_hardware_info.device_installed = 1;
    gS3_hardware_info.independent_pitch = 0;
    return 1;
}

int S3OpenSampleDevice(void) {

    // if (DirectSoundCreate(0, &gS3_direct_sound_ptr, 0)) {
    //     return 0;
    // }
    // if (gS3_direct_sound_ptr->lpVtbl->SetCooperativeLevel(gS3_direct_sound_ptr, gWin32_hwnd, 3)) {
    //     return 0;
    // }

    if (AudioBackend_Init() != eAB_success) {
        return 0;
    }
    S3Enable();
    return 1;
}

// returns 0 on failure, 1 on success
int S3OpenCDADevice(void) {
    // gS3_cda_device.lpstrDeviceType = (LPCSTR)516;
    // if (mciSendCommandA(0, 0x803u, 0x3000u, (DWORD_PTR)&gS3_cda_device)
    //     && mciSendCommandA(0, 0x803u, 0x3100u, (DWORD_PTR)&gS3_cda_device)) {
    //     return 0;
    // }
    // stru_550560.dwTimeFormat = 10; // MCI_FORMAT_TMSF
    // mciSendCommandA(gS3_cda_device.wDeviceID, 0x80Du, 0x400u, (DWORD_PTR)&stru_550560);

    if (AudioBackend_InitCDA() != eAB_success) {
        return 0;
    }

    S3EnableCDA();
    return 1;
}

void S3CloseDevices(void) {
    if (gS3_hardware_info.device_installed) {
        // gS3_direct_sound_ptr->lpVtbl->Release(gS3_direct_sound_ptr);
        // gS3_direct_sound_ptr = NULL;

        AudioBackend_UnInit();
    }
    // if (gS3_cda_device.wDeviceID) {
    //     mciSendCommandA(gS3_cda_device.wDeviceID, 0x804u, 0, 0); // MCI_CLOSE
    // }

    AudioBackend_UnInitCDA();
}

int S3ReleaseSound(tS3_sound_id id) {
    tS3_channel* c;       // [esp+Ch] [ebp-10h]
    tS3_outlet* o;        // [esp+10h] [ebp-Ch]
    tS3_descriptor* desc; // [esp+14h] [ebp-8h]
    tS3_sample* sample_ptr;

    if (!gS3_enabled) {
        return 0;
    }

    desc = S3GetDescriptorByID(id);
    if (desc == NULL) {
        return eS3_error_bad_id;
    }
    if (desc->type == eS3_ST_midi) {
        for (o = gS3_outlets; o; o = o->next) {
            for (c = o->channel_list; c; c = c->next) {
                if (c->descriptor && c->descriptor->id == id) {
                    S3ReleaseMIDI(c->tag);
                }
            }
        }
    } else if (desc->type == eS3_ST_sample) {
        sample_ptr = (tS3_sample*)desc->sound_data;
        if (sample_ptr == NULL) {
            return 0;
        }
        S3MemFree(sample_ptr->freeptr);
        S3MemFree(sample_ptr);
        desc->sound_data = NULL;
    }
    return 0;
}

int S3LoadSoundbank(const char* pSoundbank_filename, int pLow_memory_mode) {
    char soundbank_filename[256];    // [esp+Ch] [ebp-218h] BYREF
    void* buffer;                    // [esp+10Ch] [ebp-118h]
    tS3_soundbank_read_ctx read_ctx; // [esp+110h] [ebp-114h] BYREF
    char* cur_dir;                   // [esp+120h] [ebp-104h]
    char dir_name[256];              // [esp+124h] [ebp-100h] BYREF

    if (gS3_enabled) {
        dir_name[0] = 0;
        soundbank_filename[0] = 0;
        cur_dir = S3GetCurrentDir();
        strcpy(dir_name, cur_dir);
        strcat(dir_name, gS3_directory_separator);
        strcat(dir_name, "DATA");
        strcat(dir_name, gS3_directory_separator);
        strcat(dir_name, gS3_directory_name);
        strcat(dir_name, gS3_directory_separator);
        strcpy(soundbank_filename, pSoundbank_filename);
        buffer = S3LoadSoundBankFile(soundbank_filename);
        if (!buffer) {
            return gS3_last_error;
        }
        read_ctx.data = (char*)buffer;
        read_ctx.data_len = gS3_soundbank_buffer_len;
        read_ctx.nlines = 0;
        S3SoundBankReaderSkipWhitespace(&read_ctx);
        while (S3SoundBankReadEntry(&read_ctx, dir_name, pLow_memory_mode)) {
            ;
        }
        S3MemFree(buffer);
    }
    return 0;
}

char* S3LoadSoundBankFile(char* pThe_path) {
    size_t bytes_read;
    // int fd;                   // [esp+Ch] [ebp-2Ch]
    char* buffer;
    // struct _stat stat_result;

    FILE* fd;
    size_t file_size;

    // fd = _open(pThe_path, 0x8000);
    fd = OS_fopen(pThe_path, "rb");
    if (!fd) {
        gS3_last_error = eS3_error_readfile;
        return 0;
    }

    // if (_fstat(fd, &stat_result)) {
    //     gS3_last_error = eS3_error_readfile;
    //     return 0;
    // }

    if (fseek(fd, 0, SEEK_END) != 0) {
        gS3_last_error = eS3_error_readfile;
        return 0;
    }
    file_size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    buffer = S3MemAllocate(file_size + 1, kMem_S3_sample);
    if (buffer == NULL) {
        fclose(fd);
        gS3_last_error = eS3_error_memory;
        return 0;
    }
    buffer[file_size] = 0;
    bytes_read = fread(buffer, 1, file_size, fd);
    if (bytes_read == file_size) {
        gS3_soundbank_buffer = buffer;
        gS3_soundbank_buffer_len = file_size;
        fclose(fd);
        return buffer;
    }
    gS3_last_error = eS3_error_readfile;
    return 0;
}

void S3SoundBankReaderNextLine(tS3_soundbank_read_ctx* ctx) {
    S3SoundBankReaderSkipToNewline(ctx);
    S3SoundBankReaderSkipWhitespace(ctx);
}

void S3SoundBankReaderSkipWhitespace(tS3_soundbank_read_ctx* ctx) {
    while (ctx->data_len) {
        if (isalnum(*ctx->data) || *ctx->data == '-') {
            break;
        }
        S3SoundBankReaderSkipToNewline(ctx);
    }
}

void S3SoundBankReaderSkipToNewline(tS3_soundbank_read_ctx* ctx) {
    char* newline_ptr; // [esp+Ch] [ebp-4h]

    newline_ptr = memchr(ctx->data, '\n', ctx->data_len);
    if (newline_ptr) {
        S3SoundBankReaderAdvance(ctx, newline_ptr + 1 - ctx->data);
        ctx->nlines++;
    } else {
        ctx->data_len = 0;
    }
}

void S3SoundBankReaderAdvance(tS3_soundbank_read_ctx* buffer, int bytes_read) {
    buffer->data += bytes_read;
    buffer->data_len -= bytes_read;
}

int S3SoundBankReadEntry(tS3_soundbank_read_ctx* ctx, char* dir_name, int low_memory_mode) {
    int nmemory_proxies;  // [esp+Ch] [ebp-24h] BYREF
    int i;                // [esp+10h] [ebp-20h]
    int proxy_id;         // [esp+14h] [ebp-1Ch] BYREF
    tS3_descriptor* desc; // [esp+18h] [ebp-18h]
    double tmp1;          // [esp+1Ch] [ebp-14h] BYREF
    double tmp2;          // [esp+24h] [ebp-Ch] BYREF
    int char_count;       // [esp+2Ch] [ebp-4h] BYREF
    char cda_dir_name[4];

    desc = S3CreateDescriptor();
    if (!desc) {
        return gS3_last_error;
    }
    if (sscanf(ctx->data, "%i%n", &desc->id, &char_count) != 1) {
        return 0;
    }
    S3SoundBankReaderAdvance(ctx, char_count);
    S3SoundBankReaderNextLine(ctx);
    if (sscanf(ctx->data, "%i,%i%n", &desc->type, &desc->flags, &char_count) != 2) {
        return 0;
    }
    S3SoundBankReaderAdvance(ctx, char_count);
    S3SoundBankReaderNextLine(ctx);
    if (desc->type == eS3_ST_cda) {
        dir_name = cda_dir_name;
        cda_dir_name[0] = '\0';
    }
    if (!S3SoundBankReaderReadFilename(&desc->filename, ctx, dir_name)) {
        return 0;
    }
    S3SoundBankReaderNextLine(ctx);
    if (sscanf(ctx->data, "%i%n", (int*)&desc->priority, &char_count) != 1) {
        return 0;
    }
    S3SoundBankReaderAdvance(ctx, char_count);
    S3SoundBankReaderNextLine(ctx);
    if (sscanf(ctx->data, "%i%n", &desc->repeats, &char_count) != 1) {
        return 0;
    }
    S3SoundBankReaderAdvance(ctx, char_count);
    S3SoundBankReaderNextLine(ctx);
    if (sscanf(ctx->data, "%i,%i%n", &desc->min_volume, &desc->max_volume, &char_count) != 2) {
        return 0;
    }
    S3SoundBankReaderAdvance(ctx, char_count);
    S3SoundBankReaderNextLine(ctx);
    if (sscanf(ctx->data, "%lf,%lf%n", &tmp1, &tmp2, &char_count) != 2) {
        return 0;
    }
    S3SoundBankReaderAdvance(ctx, char_count);
    S3SoundBankReaderNextLine(ctx);
    if (tmp1 == 0.0f) {
        tmp1 = 1.0f;
    }
    if (tmp2 == 0.0f) {
        tmp2 = 1.0f;
    }
    desc->min_pitch = ldexp(tmp1, 16);
    desc->max_pitch = ldexp(tmp2, 16);
    if (sscanf(ctx->data, "%lf,%lf%n", &tmp1, &tmp2, &char_count) != 2) {
        return 0;
    }
    S3SoundBankReaderAdvance(ctx, char_count);
    S3SoundBankReaderNextLine(ctx);
    if (tmp1 == 0.0) {
        tmp1 = 1.0;
    }
    if (tmp2 == 0.0) {
        tmp2 = 1.0;
    }
    desc->min_speed = ldexp(tmp1, 16);
    desc->max_speed = ldexp(tmp2, 16);
    if (sscanf(ctx->data, "%i%n", &desc->special_fx, &char_count) != 1) {
        return 0;
    }
    S3SoundBankReaderAdvance(ctx, char_count);
    S3SoundBankReaderNextLine(ctx);
    if (sscanf(ctx->data, "%d%n", &nmemory_proxies, &char_count) != 1) {
        return 0;
    }
    S3SoundBankReaderAdvance(ctx, char_count);
    S3SoundBankReaderNextLine(ctx);
    desc->memory_proxy = -1;
    for (i = 0; i < nmemory_proxies; i++) {
        if (sscanf(ctx->data, "%d%n", &proxy_id, &char_count) != 1) {
            return 0;
        }
        if (low_memory_mode == i + 1) {
            desc->memory_proxy = proxy_id;
        }
        S3SoundBankReaderAdvance(ctx, char_count);
        S3SoundBankReaderNextLine(ctx);
    }
    if ((desc->flags & 1) != 0 && desc->memory_proxy == -1) {
        if (desc->type == eS3_ST_midi) {
            desc->sound_data = NULL;
        } else if (S3LoadSample(desc->id) != 0) {
            printf("\nSound bank file: couldn't load '%s'\n", desc->filename);
            ctx->data_len = 1;
            return 0;
        }
    }
    return 1;
}

tS3_descriptor* S3CreateDescriptor(void) {
    tS3_descriptor* root;
    tS3_descriptor* d;

    d = S3MemAllocate(sizeof(tS3_descriptor), kMem_S3_descriptor);
    if (!d) {
        gS3_last_error = eS3_error_memory;
        return NULL;
    }
    memset(d, 0, sizeof(tS3_descriptor));
    root = gS3_root_descriptor;
    gS3_root_descriptor->next = d;
    d->prev = root;
    gS3_root_descriptor = d;
    return d;
}

int S3SoundBankReaderReadFilename(char** filename, tS3_soundbank_read_ctx* ctx, const char* dir_name) {
    char* data_start;          // [esp+10h] [ebp-Ch]
    unsigned int bytes_read;   // [esp+14h] [ebp-8h]
    unsigned int dir_name_len; // [esp+18h] [ebp-4h]

    data_start = ctx->data;
    dir_name_len = strlen(dir_name);
    while (ctx->data_len) {
        if (isspace(*ctx->data)) {
            break;
        }
        S3SoundBankReaderAdvance(ctx, 1);
    }
    bytes_read = ctx->data - data_start;
    if (!bytes_read) {
        return 0;
    }
    *filename = S3MemAllocate(bytes_read + dir_name_len + 1, kMem_S3_scan_name);
    if (!*filename) {
        return 0;
    }
    strcpy(*filename, dir_name);
    memcpy(&(*filename)[dir_name_len], data_start, bytes_read);
    (*filename)[bytes_read + dir_name_len] = '\0';
    return 1;
}

tS3_outlet* S3CreateOutlet(int unk1, int pChannel_count) {
    tS3_outlet* o;
    int nchannels;
    tS3_outlet* outlet;
    int channels_remaining;

    // nchannels = (int)operator new(unk1, (void*)pChannel_count);
    nchannels = pChannel_count;

    if (nchannels == 0) {
        gS3_last_error = eS3_error_channel_alloc;
        return NULL;
    }
    outlet = S3MemAllocate(sizeof(tS3_outlet), kMem_S3_outlet);
    if (outlet == NULL) {
        gS3_last_error = eS3_error_memory;
        return 0;
    }
    memset(outlet, 0, sizeof(tS3_outlet));
    channels_remaining = S3CreateOutletChannels(outlet, nchannels);
    if (channels_remaining == nchannels) {
        S3MemFree(outlet);
        return NULL;
    }
    o = gS3_outlets;
    if (gS3_outlets) {
        while (o->next) {
            o = o->next;
        }
        o->next = outlet;
        outlet->prev = o;
    } else {
        gS3_outlets = outlet;
    }
    outlet->max_channels = nchannels - channels_remaining;
    outlet->id = gS3_next_outlet_id;
    gS3_next_outlet_id++;
    outlet->independent_pitch = gS3_hardware_info.independent_pitch;
    gS3_noutlets++;
    return outlet;
}

int S3CreateOutletChannels(tS3_outlet* outlet, int pChannel_count) {
    tS3_channel* chan;      // [esp+Ch] [ebp-8h]
    tS3_channel* last_chan; // [esp+10h] [ebp-4h]

    last_chan = NULL;
    while (pChannel_count) {
        chan = (tS3_channel*)S3MemAllocate(sizeof(tS3_channel), kMem_S3_channel);
        if (!chan) {
            return pChannel_count;
        }
        memset(chan, 0, sizeof(tS3_channel));
        chan->owner_outlet = outlet;

        if (S3CreateTypeStructs(chan) == 0) {
            S3MemFree(chan);
            return pChannel_count;
        }
        chan->volume_multiplier = 1.0f;
        if (last_chan) {
            last_chan->next = chan;
        } else {
            outlet->channel_list = chan;
        }
        last_chan = chan;
        pChannel_count--;
    }
    return 0;
}

void S3ReleaseOutlet(tS3_outlet* outlet) {
    tS3_outlet* next;
    tS3_outlet* prev;

    if (outlet) {
        S3UnbindChannels(outlet);
        prev = outlet->prev;
        next = outlet->next;
        if (prev != NULL) {
            prev->next = next;
        } else {
            gS3_outlets = outlet->next;
        }
        if (next != NULL) {
            next->prev = prev;
        }
        if (gS3_noutlets) {
            gS3_noutlets--;
            if (gS3_noutlets == 0) {
                gS3_outlets = NULL;
            }
        }
        S3MemFree(outlet);
    }
}

int S3UnbindChannels(tS3_outlet* outlet) {
    tS3_channel* chan;
    tS3_channel* next;

    for (chan = outlet->channel_list; chan; chan = next) {
        next = chan->next;
        S3ReleaseTypeStructs(chan);
        if (gS3_unbound_channels) {
            gS3_last_unbound_channel->next = chan;
        } else {
            gS3_unbound_channels = chan;
        }
        gS3_last_unbound_channel = chan;
        memset(chan, 0, sizeof(tS3_channel));
    }
    outlet->channel_list = NULL;
    return 1;
}

void S3ReleaseUnboundChannels(void) {
    tS3_channel* channel;      // [esp+Ch] [ebp-8h]
    tS3_channel* next_channel; // [esp+10h] [ebp-4h]

    for (channel = gS3_unbound_channels; channel != NULL; channel = next_channel) {
        next_channel = channel->next;
        S3MemFree(channel);
    }
}

tS3_channel* S3AllocateChannel(tS3_outlet* outlet, int priority) {
    tS3_channel* c;                    // [esp+Ch] [ebp-10h]
    int lowest_priority;               // [esp+10h] [ebp-Ch] MAPDST
    int this_priority;                 // [esp+14h] [ebp-8h]
    tS3_channel* lowest_priority_chan; // [esp+18h] [ebp-4h]

    lowest_priority_chan = outlet->channel_list;
    c = outlet->channel_list;
    if (lowest_priority_chan == NULL) {
        return NULL;
    }
    while (c) {
        if (!c->active || c->needs_service) {
            if (!c->needs_service) {
                c->active = 1;
                return c;
            }
        } else {
            if (lowest_priority_chan->descriptor) {
                lowest_priority = lowest_priority_chan->descriptor->priority
                    * (lowest_priority_chan->right_volume + lowest_priority_chan->left_volume + 1);
            } else {
                lowest_priority = 0;
            }
            if (c->descriptor) {
                this_priority = c->descriptor->priority * (c->left_volume + c->right_volume + 1);
            } else {
                this_priority = 0;
            }
            if (this_priority <= lowest_priority) {
                lowest_priority_chan = c;
            }
        }
        c = c->next;
    }
    if (!lowest_priority_chan->descriptor || lowest_priority_chan->needs_service) {
        lowest_priority = 0;
    } else {
        lowest_priority = lowest_priority_chan->descriptor->priority * (lowest_priority_chan->right_volume + lowest_priority_chan->left_volume + 1);
    }
    if (priority > lowest_priority && !lowest_priority_chan->needs_service) {
        lowest_priority_chan->termination_reason = 2;
        S3StopChannel(lowest_priority_chan);
        lowest_priority_chan->active = 1;
    }

    return NULL;
}

int S3StopChannel(tS3_channel* chan) {
    if (!chan->tag) {
        return eS3_error_bad_stag;
    }
    chan->termination_reason = eS3_tr_stopped;
    if (chan->active) {
        chan->needs_service = 1;
    }
    if (chan->type == eS3_ST_sample) {
        if (chan->sound_source_ptr) {
            chan->sound_source_ptr->tag = 0;
            chan->sound_source_ptr->channel = 0;
            chan->sound_source_ptr->volume = 0;
        }
        if (S3StopSample(chan) == 0) {
            return eS3_error_function_failed;
        }
    } else if (chan->type == eS3_ST_midi) {
        if (S3StopMIDI(chan) != 0) {
            return eS3_error_function_failed;
        }
    } else if (chan->type == eS3_ST_cda) {
        if (S3StopCDA(chan) != 0) {
            return eS3_error_function_failed;
        }
    }

    if ((chan->descriptor->flags & 2) != 0) {
        S3ReleaseSound(chan->descriptor->id);
    }
    chan->repetitions = 1;
    return 0;
}

tS3_sound_source* S3CreateSoundSourceBR(br_vector3* pPosition, br_vector3* pVelocity, tS3_outlet* pBound_outlet) {
    tS3_sound_source* source; // [esp+Ch] [ebp-4h]

    if (!gS3_enabled) {
        return 0;
    }
    source = S3CreateSoundSource(pPosition, pVelocity, pBound_outlet);
    if (source != NULL) {
        source->brender_vector = 1;
    }
    return source;
}

tS3_sound_source* S3CreateSoundSource(void* pPosition, void* pVelocity, tS3_outlet* pBound_outlet) {
    tS3_sound_source* src; // [esp+Ch] [ebp-8h]
    tS3_sound_source* s;   // [esp+10h] [ebp-4h]

    src = S3MemAllocate(sizeof(tS3_sound_source), kMem_S3_source);
    if (src == NULL) {
        gS3_last_error = eS3_error_memory;
        return 0;
    }
    memset(src, 0, sizeof(tS3_sound_source));
    src->bound_outlet = pBound_outlet;
    src->position_ptr = pPosition;
    src->velocity_ptr = pVelocity;
    s = gS3_sound_sources;
    if (gS3_sound_sources) {
        while (s->next) {
            s = s->next;
        }
        s->next = src;
        src->prev = s;
    } else {
        gS3_sound_sources = src;
    }
    gS3_nsound_sources++;
    return src;
}

int S3ReleaseSoundSource(tS3_sound_source* src) {
    tS3_sound_source* prev; // [esp+Ch] [ebp-8h]
    tS3_sound_source* next; // [esp+10h] [ebp-4h]

    if (!gS3_enabled) {
        return 0;
    }

    if (src) {
        prev = src->prev;
        next = src->next;
        if (prev) {
            prev->next = next;
        } else {
            gS3_sound_sources = src->next;
        }
        if (next) {
            next->prev = prev;
        }
        if (gS3_nsound_sources) {
            gS3_nsound_sources--;
            if (gS3_nsound_sources == 0) {
                gS3_sound_sources = NULL;
            }
        }
        S3StopSoundSource(src);
        S3MemFree(src);
    }
    return 0;
}

void S3Service(int inside_cockpit, int unk1) {

    int now;        // [esp+Ch] [ebp-10h]
    tS3_channel* c; // [esp+10h] [ebp-Ch]
    tS3_outlet* o;  // [esp+14h] [ebp-8h]
    int v5;         // [esp+18h] [ebp-4h]

    v5 = 0;
    gS3_inside_cockpit = inside_cockpit;
    if (!gS3_enabled) {
        return;
    }
    now = PDGetTotalTime();
    gS3_service_time_delta = now - gS3_last_service_time;
    gS3_last_service_time = now;
    S3ServiceOutlets();
    if (unk1 == 1) {
        S3UpdateListenerVectors();
        S3ServiceAmbientSoundSources();
    }
    for (o = gS3_outlets; o; o = o->next) {
        for (c = o->channel_list; c; c = c->next) {
            if (c->needs_service) {
                c->needs_service = 0;
                if (c->descriptor && c->descriptor->flags == 2) {
                    S3ReleaseSound(c->descriptor->id);
                }
                c->active = 0;
                if (c->type != eS3_ST_midi) {
                    c->tag = 0;
                }
            } else if (c->spatial_sound && c->active) {
                if (S3UpdateSpatialSound(c)) {
                    if (c->sound_source_ptr && c->sound_source_ptr->ambient && !S3SoundStillPlaying(c->tag)) {
                        S3UpdateSoundSource(NULL, -1, c->sound_source_ptr, -1.0f, -1, -1, 0, -1, -1);
                    }
                } else if (c->sound_source_ptr) {
                    if (c->sound_source_ptr->ambient) {
                        S3UpdateSoundSource(NULL, -1, c->sound_source_ptr, -1.0f, -1, -1, 0, -1, -1);
                    }
                } else {
                    S3StopChannel(c);
                }
            } else if (c->type == eS3_ST_midi && c->active) {
                // sub_4124BE(c);
            }
            if (unk1 < 2 && gS3_last_service_time > dword_5216C0) {
                v5 = 1;
                if (!c->active && c->spatial_sound == 2) {
                    // null_unknown_libname_8();  /* no-op */
                }
            }
        }
    }
    if (v5) {
        dword_5216C0 = gS3_last_service_time;
    }
}

void S3ServiceOutlets(void) {
    tS3_channel* c; // [esp+Ch] [ebp-8h]
    tS3_outlet* o;  // [esp+10h] [ebp-4h]

    for (o = gS3_outlets; o; o = o->next) {
        for (c = o->channel_list; c; c = c->next) {
            S3ServiceChannel(c);
        }
    }
}

int S3ServiceChannel(tS3_channel* chan) {
    if (chan->type == eS3_ST_sample) {
        if (AudioBackend_SoundIsPlaying(chan->type_struct_sample)) {
            return 1;
        }
        S3StopSample(chan);
        return 0;
    } else if (chan->type == eS3_ST_midi) {
        return !S3IsMIDIStopped(chan);
    } else if (chan->type == eS3_ST_cda) {
        return S3IsCDAPlaying();
    } else {
        return 0;
    }
}

void S3StopAllOutletSounds(void) {
    tS3_outlet* o; // [esp+Ch] [ebp-4h]

    if (!gS3_enabled) {
        return;
    }

    for (o = gS3_outlets; o; o = o->next) {
        S3StopOutletSound(o);
    }
}

tS3_sound_tag S3StartSound(tS3_outlet* pOutlet, tS3_sound_id pSound) {
    int repetitions;      // eax
    tS3_channel* chan;    // [esp+14h] [ebp-Ch]
    tS3_descriptor* desc; // [esp+1Ch] [ebp-4h]

    if (!gS3_enabled) {
        return 0;
    }
    if (!pOutlet) {
        gS3_last_error = eS3_error_bad_id;
        return 0;
    }
    desc = S3GetDescriptorByID(pSound);
    if (!desc) {
        gS3_last_error = eS3_error_bad_id;
        return 0;
    }
    memset(&gS3_channel_template, 0, sizeof(gS3_channel_template));
    S3CalculateRandomizedFields(&gS3_channel_template, desc);
    chan = S3AllocateChannel(pOutlet, desc->priority * (gS3_channel_template.right_volume + gS3_channel_template.left_volume + 1));
    if (!chan) {
        gS3_last_error = eS3_error_channel_alloc;
        return 0;
    }
    chan->left_volume = gS3_channel_template.left_volume * chan->volume_multiplier;
    chan->right_volume = gS3_channel_template.right_volume * chan->volume_multiplier;
    chan->rate = gS3_channel_template.rate;
    if (desc->type == eS3_ST_sample && (!desc->sound_data || desc->flags == 2)) {
        if (!S3LoadSample(pSound)) {
            chan->needs_service = 1;
            gS3_last_error = eS3_error_load_sound;
            return 0;
        }
    }
    if (chan->descriptor && chan->descriptor->type == 1 && chan->descriptor->id != pSound) {
        S3ReleaseMIDI(chan->tag);
    }
    chan->spatial_sound = 0;
    chan->sound_source_ptr = 0;
    chan->descriptor = desc;
    chan->type = desc->type;
    repetitions = desc->repeats;
    if (repetitions <= 0) {
        repetitions = 0;
    }
    chan->repetitions = repetitions;
    chan->needs_service = 0;
    chan->termination_reason = eS3_tr_natural;
    chan->tag = S3GenerateTag(pOutlet);
    if (desc->type == eS3_ST_midi && !desc->sound_data) {
        if (S3MIDILoadSong(chan)) {
            chan->needs_service = 1;
            return 0;
        }
    }
    if (chan->type == eS3_ST_sample) {
        S3ExecuteSampleFilterFuncs(chan);
        if (S3PlaySample(chan) == 0) {
            gS3_last_error = eS3_error_start_sound;
            chan->needs_service = 1;
            return 0;
        }
    } else if (chan->type == eS3_ST_midi) {
        if (S3PlayMIDI(chan)) {
            chan->needs_service = 1;
            gS3_last_error = eS3_error_start_song;
            return 0;
        }
        S3SetMIDIVolume(chan);
    } else if (chan->type == eS3_ST_cda) {
        if (S3PlayCDA(chan)) {
            chan->needs_service = 1;
            gS3_last_error = eS3_error_start_cda;
            return 0;
        }
    }

    return chan->tag;
}

tS3_sound_tag S3StartSound2(tS3_outlet* pOutlet, tS3_sound_id pSound, tS3_repeats pRepeats, tS3_volume pLVolume, tS3_volume pRVolume, tS3_pitch pPitch, tS3_speed pSpeed) {
    tS3_channel* chan;    // [esp+30h] [ebp-Ch]
    tS3_descriptor* desc; // [esp+38h] [ebp-4h]

    if (!gS3_enabled) {
        return 0;
    }
    desc = S3GetDescriptorByID(pSound);
    if (!desc) {
        gS3_last_error = eS3_error_bad_id;
        return 0;
    }
    if (pLVolume < 0) {
        pLVolume = 128;
    }
    if (pRVolume < 0) {
        pRVolume = 128;
    }
    if (pLVolume > 255) {
        pLVolume = 255;
    }
    if (pRVolume > 255) {
        pRVolume = 255;
    }
    chan = S3AllocateChannel(pOutlet, desc->priority * (pLVolume + pRVolume + 1));
    if (chan == NULL) {
        gS3_last_error = eS3_error_channel_alloc;
        return 0;
    }
    if (desc->type == eS3_ST_sample) {
        if (desc->sound_data == NULL && (!S3LoadSample(pSound) || (desc->flags & 2) != 0)) {
            chan->needs_service = 1;
            gS3_last_error = eS3_error_load_sound;
            return 0;
        }
    }

    if (chan->descriptor && chan->descriptor->type == eS3_ST_midi) {
        if (chan->descriptor->id != pSound) {
            S3ReleaseMIDI(chan->tag);
        }
    }
    chan->spatial_sound = 0;
    chan->descriptor = desc;
    chan->needs_service = 0;
    chan->termination_reason = eS3_tr_natural;
    chan->type = desc->type;
    chan->sound_source_ptr = NULL;
    chan->repetitions = MAX(pRepeats, 0);
    S3CalculateRandomizedFields(chan, desc);
    chan->left_volume = pLVolume * chan->volume_multiplier;
    chan->right_volume = pRVolume * chan->volume_multiplier;
    chan->tag = S3GenerateTag(pOutlet);
    if (pPitch == -1) {
        pPitch = 0x10000;
    }
    if (pSpeed == -1) {
        pSpeed = 0x10000;
    }
    chan->rate = ldexp(pPitch, -16) * chan->rate;
    if (!pOutlet->independent_pitch) {
        chan->rate = ldexp(pSpeed, -16) * chan->rate;
    }
    if (desc->type == eS3_ST_midi && desc->sound_data == NULL && S3MIDILoadSong(chan)) {
        chan->needs_service = 1;
        return 0;
    }

    switch (chan->type) {
    case eS3_ST_sample:
        S3ExecuteSampleFilterFuncs(chan);
        if (!S3PlaySample(chan)) {
            chan->needs_service = 1;
            gS3_last_error = eS3_error_start_sound;
            return 0;
        }
        break;

    case eS3_ST_midi:
        if (S3PlayMIDI(chan)) {
            chan->needs_service = 1;
            gS3_last_error = eS3_error_start_song;
            return 0;
        }
        S3SetMIDIVolume(chan);
        break;

    case eS3_ST_cda:
        if (S3PlayCDA(chan)) {
            chan->needs_service = 1;
            gS3_last_error = eS3_error_start_cda;
            return 0;
        }
        break;
    }

    return chan->tag;
}

void S3CalculateRandomizedFields(tS3_channel* chan, tS3_descriptor* desc) {
    int vol; // eax

    vol = S3IRandomBetween(desc->min_volume, desc->max_volume, 128);
    chan->left_volume = vol;
    chan->right_volume = vol;
    if (desc->type == eS3_ST_sample) {
#if defined(DETHRACE_FIX_BUGS)
        /* Avoid a possible NULL pointer dereference. */
        if (desc->sound_data == NULL) {
            chan->rate = desc->min_pitch;
            return;
        }
#endif
        chan->rate = S3IRandomBetweenLog(desc->min_pitch, desc->max_pitch, ((tS3_sample*)desc->sound_data)->rate);
    }
}

// duplicate of S3IRandomBetween2
int S3IRandomBetween(int pMin, int pMax, int pDefault) {
    if (pMin == -1) {
        return pDefault;
    }
    if (pMax <= pMin) {
        return pMin;
    }
    return rand() % (pMax - pMin) + pMin;
}

// duplicate of S3IRandomBetweenLog2
int S3IRandomBetweenLog(int pMin, int pMax, int pDefault) {
    float v4; // st7

    if (pMin == -1 || pMin >= pMax) {
        return pDefault;
    }
    v4 = S3FRandomBetween(log(pMin), log(pMax));
    return ldexp(exp(v4), -16) * pDefault;
}

// duplicate of S3FRandomBetween2
double S3FRandomBetween(double pMin, double pMax) {
    return (double)rand() * (pMax - pMin) / (double)RAND_MAX + pMin;
}

int S3GenerateTag(tS3_outlet* outlet) {
    gS3_tag_seed += 256;
    return gS3_tag_seed | outlet->id;
}

int S3SoundStillPlaying(tS3_sound_tag pTag) {
    tS3_channel* chan;

    if (!gS3_enabled) {
        return 0;
    }
    if (!pTag) {
        return 0;
    }
    chan = S3GetChannelForTag(pTag);
    if (!chan) {
        return 0;
    }
    return S3ServiceChannel(chan) != 0;
}

int S3ChangePitchSpeed(tS3_sound_tag pTag, tS3_pitch pNew_pitch) {
    tS3_channel* chan;

    if (!gS3_enabled) {
        return 0;
    }
    if (pNew_pitch == -1) {
        pNew_pitch = 0x10000;
    }
    chan = S3GetChannelForTag(pTag);
    if (chan == NULL) {
        return eS3_error_bad_stag;
    }
    if (chan->type != eS3_ST_sample) {
        return 0;
    }
    chan->rate = ldexp(pNew_pitch, -16) * ((tS3_sample*)chan->descriptor->sound_data)->rate;
    if (S3SyncSampleRate(chan)) {
        return 0;
    } else {
        return eS3_error_function_failed;
    }
}

int S3StopSound(tS3_sound_tag pTag) {
    tS3_channel* chan; // [esp+Ch] [ebp-4h]

    if (!gS3_enabled) {
        return 0;
    }
    if (!pTag) {
        return eS3_error_bad_stag;
    }
    chan = S3GetChannelForTag(pTag);
    if (!chan) {
        return eS3_error_bad_stag;
    }
    chan->termination_reason = eS3_tr_stopped;
    chan->initial_volume = 0;
    if (chan->active) {
        chan->needs_service = 1;
    }
    if (chan->type == eS3_ST_sample) {
        if (chan->sound_source_ptr) {
            chan->sound_source_ptr->tag = 0;
            chan->sound_source_ptr->channel = NULL;
            chan->sound_source_ptr->volume = 0;
        }
        if (!S3StopSample(chan)) {
            return eS3_error_function_failed;
        }
    } else if (chan->type == eS3_ST_midi) {
        if (S3StopMIDI(chan)) {
            return eS3_error_function_failed;
        }
    } else if (chan->type == eS3_ST_cda) {
        if (S3StopCDA(chan)) {
            return eS3_error_function_failed;
        }
    }

    if ((chan->descriptor->flags & 2) != 0) {
        S3ReleaseSound(chan->descriptor->id);
    }
    chan->repetitions = 1;
    return 0;
}

int S3StopOutletSound(tS3_outlet* pOutlet) {
    tS3_channel* c; // [esp+Ch] [ebp-4h]

    if (!gS3_enabled) {
        return 0;
    }
    for (c = pOutlet->channel_list; c; c = c->next) {
        // null_unknown_libname_8();
        if (c->active) {
            c->spatial_sound = 0;
            S3StopChannel(c);
            c->needs_service = 1;
        }
    }
    return 0;
}

char* S3GetCurrentDir(void) {
    if (!gS3_have_current_dir) {
        if (getcwd(gS3_current_dir, 260) == NULL) {
            LOG_PANIC("failed to call getcwd"); // added by dethrace
        };
        gS3_have_current_dir = 1;
    }
    return gS3_current_dir;
}

tS3_descriptor* S3GetDescriptorByID(tS3_sound_tag id) {
    tS3_descriptor* d; // [esp+Ch] [ebp-4h]

    assert(id != 0);

    for (d = gS3_descriptors;; d = d->next) {
        if (!d) {
            return 0;
        }
        if (d->id == id) {
            break;
        }
    }
    if (d->memory_proxy < 0) {
        return d;
    } else {
        return S3GetDescriptorByID(d->memory_proxy);
    }
}

int S3SetOutletVolume(tS3_outlet* pOutlet, tS3_volume pVolume) {
    tS3_channel* c; // [esp+10h] [ebp-4h]

    if (!gS3_enabled) {
        return 0;
    }
    if (pVolume > 255) {
        pVolume = 255;
    }
    if (pVolume < 10) {
        pVolume = 10;
    }
    if (!pOutlet) {
        return 0;
    }
    for (c = pOutlet->channel_list; c; c = c->next) {
        c->volume_multiplier = pVolume / 150.0f;
        if (c->active) {
            S3ChangeVolume(c->tag, pVolume);
        }
    }
    return 0;
}

tS3_channel* S3GetChannelForTag(tS3_sound_tag tag) {
    tS3_channel* c; // [esp+Ch] [ebp-Ch]
    tS3_outlet* o;  // [esp+14h] [ebp-4h]

    if (!tag) {
        return 0;
    }
    for (o = gS3_outlets; o && o->id != tag; o = o->next) {
        ;
    }
    if (!o) {
        return 0;
    }
    for (c = o->channel_list; c; c = c->next) {
        if (c->tag == tag) {
            return c;
        }
    }
    return 0;
}

int S3ChangeVolume(tS3_sound_tag pTag, tS3_volume pVolume) {
    tS3_channel* chan; // [esp+14h] [ebp-4h]

    if (!gS3_enabled) {
        return 0;
    }
    chan = S3GetChannelForTag(pTag);
    if (!chan) {
        return eS3_error_bad_stag;
    }
    if (pVolume < 0) {
        pVolume = 128;
    }
    if (pVolume > 255) {
        pVolume = 255;
    }
    if (chan->type == eS3_ST_sample) {
        chan->left_volume = pVolume * chan->volume_multiplier;
        chan->right_volume = pVolume * chan->volume_multiplier;
        if (!S3SyncSampleVolumeAndPan(chan)) {
            return eS3_error_function_failed;
        }
    } else if (chan->type == eS3_ST_midi) {
        S3SetMIDIVolume(chan);
    } else if (chan->type == eS3_ST_cda) {
        S3SetCDAVolume(chan, pVolume);
    }

    return 0;
}
