
#ifndef BACKEND_H
#define BACKEND_H

#include "s3_defs.h"

typedef enum tAudioBackend_error_code {
    eAB_success = 0,
    eAB_error = 1
} tAudioBackend_error_code;

tAudioBackend_error_code AudioBackend_Init(void);
tAudioBackend_error_code AudioBackend_InitCDA(void);
void AudioBackend_UnInit(void);
void AudioBackend_UnInitCDA(void);
void* AudioBackend_AllocateSampleTypeStruct(void);

// returns 1 if playing, otherwise 0
int AudioBackend_SoundIsPlaying(tS3_channel* chan);
tAudioBackend_error_code AudioBackend_StopSample(tS3_channel* chan);
tAudioBackend_error_code AudioBackend_PlaySample(tS3_channel* chan);
tAudioBackend_error_code AudioBackend_SetVolume(tS3_channel* chan, int volume);
tAudioBackend_error_code AudioBackend_SetPan(tS3_channel* chan, int pan);
tAudioBackend_error_code AudioBackend_SetFrequency(tS3_channel* chan, int rate);

#endif
