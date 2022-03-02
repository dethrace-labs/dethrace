#ifndef HARNESS_H
#define HARNESS_H

#include "brender/br_types.h"
#include "harness/trace.h"

typedef struct tPlatform {
    void (*Init)();
    void (*NewWindow)(char* title, int width, int height, int render_width, int render_height);
    void (*PollEvents)();
    int* (*GetKeyMap)();
    int (*IsKeyDown)(unsigned char pScan_code);
    void (*BeginScene)(br_actor* camera, br_pixelmap* colour_buffer);
    void (*EndScene)();
    void (*SetPalette)(uint8_t* palette);
    void (*RenderFullScreenQuad)(uint8_t* src, int width, int height);
    void (*RenderModel)(br_actor* actor, br_model* model, br_matrix34 model_matrix);
    void (*Swap)();
    void (*BufferTexture)(br_pixelmap* pm);
    void (*BufferMaterial)(br_material* mat);
    void (*FlushBuffers)(br_pixelmap* color_buffer, br_pixelmap* depth_buffer);

} tPlatform;

typedef struct tCamera {
    void (*update)();
    float* (*getProjection)();
    float* (*getView)();
    void (*setPosition)();
} tCamera;

#endif