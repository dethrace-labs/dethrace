#ifndef HARNESS_H
#define HARNESS_H

#include "brender/br_types.h"
#include "harness/trace.h"

typedef struct tPlatform {
    void (*Init)();
    void (*NewWindow)(char* title, int width, int height);
    void (*PollEvents)();
    int* (*GetKeyMap)();
    int (*IsKeyDown)(unsigned char pScan_code);
    void (*BeginFrame)(br_actor* camera, br_pixelmap* colour_buffer);
    void (*EndFrame)();
    void (*RenderFullScreenQuad)(uint32_t* src, int transparent);
    void (*RenderModel)(br_model* model, br_matrix34 model_matrix);
    void (*Swap)();

} tPlatform;

typedef struct tCamera {
    void (*update)();
    float* (*getProjection)();
    float* (*getView)();
    void (*setPosition)();
} tCamera;

#endif