#ifndef S3_TYPES_H
#define S3_TYPES_H

#include <stdint.h>

// extern int PDGetTotalTime();
// extern void dr_dprintf(char* fmt_string, ...);

#define MIN(a, b) ((a) < (b) ? a : b)
#define MAX(a, b) ((a) > (b) ? a : b)

// Internal typedefs

typedef float tF32;
typedef char* tS3_sound_source_ptr;
typedef int tS3_sound_tag;
typedef int tS3_sound_id;
typedef int tS3_type;
typedef int tS3_repeats;
typedef int tS3_volume;
typedef int tS3_effect_tag;
typedef long tS3_priority;
typedef long tS3_pitch;
typedef long tS3_speed;
typedef char* tS3_outlet_ptr;
typedef struct tS3_outlet tS3_outlet;
typedef struct tS3_descriptor tS3_descriptor;
typedef struct tS3_channel tS3_channel;
typedef struct tS3_sound_source tS3_sound_source;

typedef enum tS3_error_codes {
    eS3_error_none = 0,
    eS3_error_digi_init = 1,
    eS3_error_midi_init = 2,
    eS3_error_memory = 3,
    eS3_error_readfile = 4,
    eS3_error_soundbank = 5,
    eS3_error_bad_id = 6,
    eS3_error_bad_stag = 7,
    eS3_error_load_sound = 8,
    eS3_error_start_sound = 9,
    eS3_error_channel_alloc = 10,
    eS3_error_not_spatial = 11,
    eS3_error_function_failed = 12,
    eS3_error_load_song = 13,
    eS3_error_song_not_loaded = 14,
    eS3_error_start_song = 15,
    eS3_error_start_cda = 16,
    eS3_error_cda_not_init = 17,
    eS3_error_already_started_cda = 18,
    eS3_error_nonexistant_source = 19
} tS3_error_codes;

typedef enum tS3_termination_reason {
    eS3_tr_natural = 0,
    eS3_tr_stopped = 1,
    eS3_tr_overridden = 2
} tS3_termination_reason;

typedef enum tS3_filter_type {
    eS3_filter_none = 0,
    eS3_filter_reversed = 1,
    eS3_filter_echo = 2
} tS3_filter_type;

typedef enum tS3_sound_type {
    eS3_ST_sample = 0,
    eS3_ST_midi = 1,
    eS3_ST_cda = 2
} tS3_sound_type;

typedef void tS3_outlet_callback(tS3_outlet*, tS3_sound_tag, tS3_termination_reason);
typedef void tS3_sample_filter(tS3_effect_tag, tS3_sound_tag);

typedef struct tS3_vector3 {
    tF32 x;
    tF32 y;
    tF32 z;
} tS3_vector3;

typedef struct tS3_channel {
    int active;
    int termination_reason;
    int needs_service;
    int repetitions;
    int rate;
    int spatial_sound;
    unsigned int song_handle;
    tS3_sound_tag tag;
    tS3_type type;
    tS3_volume left_volume;
    tS3_volume right_volume;
    float volume_multiplier;
    tS3_vector3 position;
    tS3_vector3 velocity;
    tS3_vector3 lastpos;
    tS3_volume initial_volume;
    tS3_pitch initial_pitch;
    float pMax_distance_squared;
    tS3_outlet* owner_outlet;
    tS3_descriptor* descriptor;
    tS3_channel* next;
    char* type_struct_sample;
    char* type_struct_midi;
    char* type_struct_cda;
    tS3_sound_source* sound_source_ptr;
} tS3_channel;

typedef struct tS3_outlet {
    int id;
    int max_channels;
    int independent_pitch;
    tS3_channel* channel_list;
    tS3_outlet* prev;
    tS3_outlet* next;
    tS3_outlet_callback* callback;
} tS3_outlet;

typedef struct tS3_descriptor {
    tS3_sound_id id;
    tS3_type type;
    tS3_priority priority;
    tS3_repeats repeats;
    tS3_volume min_volume;
    tS3_volume max_volume;
    tS3_pitch min_pitch;
    tS3_pitch max_pitch;
    tS3_speed min_speed;
    tS3_speed max_speed;
    tS3_descriptor* prev;
    tS3_descriptor* next;
    int flags;
    int midi_song_handle;
    int special_fx;
    int memory_proxy;
    char* sound_data;
    char* filename;
    // LPDIRECTSOUNDBUFFER for example
    void* sound_buffer;
} tS3_descriptor;

typedef struct tS3_sound_source {
    tS3_channel* channel;
    int brender_vector;
    void* position_ptr;
    void* velocity_ptr;
    tS3_outlet* bound_outlet;
    tS3_sound_source* prev;
    tS3_sound_source* next;
    int ambient;
    tS3_repeats ambient_repeats;
    int time_since_last_played;
    tS3_sound_id sound_id;
    float max_distance_sq;
    int period;
    tS3_volume volume;
    tS3_pitch pitch;
    tS3_speed speed;
    tS3_sound_tag tag;
} tS3_sound_source;

typedef struct tS3_sample {
    int size;
    int rate;
    int resolution;
    int channels;
    char* dataptr;
    void* freeptr;
} tS3_sample;

typedef struct tS3_hardware_info {
    int device_installed;
    int timer_installed;
    int num_channels;
    int max_resolution;
    int stereo_flag;
    int max_sample_rate;
    int min_sample_rate;
    int independent_pitch;
} tS3_hardware_info;

// Added by dethrace
typedef struct tS3_soundbank_read_ctx {
    int data_len;
    int unk1;
    int nlines;
    char* data;
} tS3_soundbank_read_ctx;

// Added by dethrace
#pragma pack(push, 1)
typedef struct tWAVEFORMATEX_ {
    int16_t wFormatTag;
    int16_t nChannels;
    int32_t nSamplesPerSec;
    int32_t nAvgBytesPerSec;
    int16_t nBlockAlign;
    int16_t wBitsPerSample;
    int16_t cbSize;
} tWAVEFORMATEX_;
#pragma pack(pop)

#endif