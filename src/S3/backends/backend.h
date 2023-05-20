
#ifndef BACKEND_H
#define BACKEND_H

#include "s3_defs.h"
int AudioBackend_Init(void);
int AudioBackend_UnInit(void);
int AudioBackend_SoundIsPlaying(tS3_channel* chan);
int AudioBackend_StopSample(tS3_channel* chan);
int AudioBackend_PlaySample(tS3_channel* chan);

int AudioBackend_InitChannel(tS3_channel* chan);
int AudioBackend_SetVolume(tS3_channel* chan, int volume);
int AudioBackend_SetPan(tS3_channel* chan, int pan);
int AudioBackend_SetFrequency(tS3_channel* chan, int rate);

#endif
