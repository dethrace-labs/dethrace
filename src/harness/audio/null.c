#include "harness/audio.h"
#include <string.h>

tAudioBackend_error_code AudioBackend_Init(void) {
    return eAB_error;
}

void AudioBackend_UnInit(void) {
}

tAudioBackend_error_code AudioBackend_InitCDA(void) {
    return eAB_error;
}

void AudioBackend_UnInitCDA(void) {
}

void* AudioBackend_AllocateSampleTypeStruct(void) {
}

tAudioBackend_error_code AudioBackend_PlaySample(void* type_struct_sample, int channels, void* data, int size, int rate, int loop) {
    return eAB_error;
}

int AudioBackend_SoundIsPlaying(void* type_struct_sample) {
}

tAudioBackend_error_code AudioBackend_StopSample(void* type_struct_sample) {
    return eAB_error;
}

tAudioBackend_error_code AudioBackend_SetVolume(void* type_struct_sample, int volume) {
    return eAB_error;
}

tAudioBackend_error_code AudioBackend_SetPan(void* type_struct_sample, int pan) {
    return eAB_error;
}

tAudioBackend_error_code AudioBackend_SetFrequency(void* type_struct_sample, int original_rate, int new_rate) {
    return eAB_error;
}

tAudioBackend_error_code AudioBackend_SetVolumeSeparate(void* type_struct_sample, int left_volume, int right_volume) {
    return eAB_error;
}

tAudioBackend_error_code AudioBackend_PlayCDA(int track) {
    return eAB_error;
}

tAudioBackend_error_code AudioBackend_StopCDA(void) {
    return eAB_error;
}

int AudioBackend_CDAIsPlaying(void) {
}

tAudioBackend_error_code AudioBackend_SetCDAVolume(int volume) {
    return eAB_error;
}

// Used by smackw32
tAudioBackend_stream* AudioBackend_StreamOpen(int bitdepth, int channels, unsigned int sample_rate) {
    return NULL;
}

tAudioBackend_error_code AudioBackend_StreamWrite(tAudioBackend_stream* stream_handle, const unsigned char* data, unsigned long size) {
    return eAB_error;
}

tAudioBackend_error_code AudioBackend_StreamClose(tAudioBackend_stream* stream_handle) {
    return eAB_error;
}
