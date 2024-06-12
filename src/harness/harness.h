#ifndef HARNESS_H
#define HARNESS_H

#include "harness/trace.h"

void Harness_ForceNullPlatform(void);
int Harness_CalculateFrameDelay(int last_frame_time);

typedef struct tCamera {
    void (*update)(void);
    float* (*getProjection)(void);
    float* (*getView)(void);
    void (*setPosition)(void);
} tCamera;

#endif
