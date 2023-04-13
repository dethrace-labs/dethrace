#ifndef HARNESS_H
#define HARNESS_H

#include "brender/br_types.h"
#include "harness/trace.h"

void Harness_ForceNullRenderer();

typedef struct tCamera {
    void (*update)();
    float* (*getProjection)();
    float* (*getView)();
    void (*setPosition)();
} tCamera;

void IOPlatform_missing_hook();

#endif
