
#ifndef HARNESS_AUDIO_H
#define HARNESS_AUDIO_H

typedef enum tAudioBackend_error_code {
    eAB_success = 0,
    eAB_error = 1
} tAudioBackend_error_code;

typedef void tAudioBackend_stream;

// Used by S3
tAudioBackend_error_code AudioBackend_Init(void);
void AudioBackend_UnInit(void);
tAudioBackend_error_code AudioBackend_InitCDA(void);
void AudioBackend_UnInitCDA(void);
void* AudioBackend_AllocateSampleTypeStruct(void);
tAudioBackend_error_code AudioBackend_PlaySample(void* type_struct_sample, int channels, void* data, int size, int rate, int loop);
int AudioBackend_SoundIsPlaying(void* type_struct_sample);
tAudioBackend_error_code AudioBackend_StopSample(void* type_struct_sample);
tAudioBackend_error_code AudioBackend_SetVolume(void* type_struct_sample, int volume);
tAudioBackend_error_code AudioBackend_SetPan(void* type_struct_sample, int pan);
tAudioBackend_error_code AudioBackend_SetFrequency(void* type_struct_sample, int original_rate, int new_rate);
tAudioBackend_error_code AudioBackend_SetVolumeSeparate(void* type_struct_sample, int left_volume, int right_volume);

tAudioBackend_error_code AudioBackend_PlayCDA(int track);
tAudioBackend_error_code AudioBackend_StopCDA(void);
int AudioBackend_CDAIsPlaying(void);
tAudioBackend_error_code AudioBackend_SetCDAVolume(int volume);

// Used by smackw32
tAudioBackend_stream* AudioBackend_StreamOpen(int bitdepth, int channels, unsigned int sample_rate);
tAudioBackend_error_code AudioBackend_StreamWrite(tAudioBackend_stream* stream_handle, const unsigned char* data, unsigned long size);
tAudioBackend_error_code AudioBackend_StreamClose(tAudioBackend_stream* stream_handle);

#endif
