#ifndef HARNESS_H
#define HARNESS_H

#include "brender/br_types.h"
#include "harness/trace.h"

void Harness_ForceNullPlatform(void);

typedef struct tCamera {
    void (*update)(void);
    float* (*getProjection)(void);
    float* (*getView)(void);
    void (*setPosition)(void);
} tCamera;

#endif
