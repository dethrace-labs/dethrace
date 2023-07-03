#ifndef HARNESS_RENDERER_H
#define HARNESS_RENDERER_H

#include "brender/br_types.h"
#include "dr_types.h"

typedef struct tRenderer {
    void (*Init)(int width, int height, int pRender_width, int pRender_height);
    void (*Shutdown)(void);
    void (*BeginScene)(br_actor* camera, br_pixelmap* colour_buffer);
    void (*EndScene)();
    void (*SetPalette)(uint8_t* palette);
    void (*FullScreenQuad)(uint8_t* src);
    void (*Model)(br_actor* actor, br_model* model, br_matrix34 model_matrix);
    void (*ClearBuffers)();
    void (*BufferTexture)(br_pixelmap* pm);
    void (*BufferMaterial)(br_material* mat);
    void (*BufferModel)(br_model* model);
    void (*FlushBuffers)(br_pixelmap* color_buffer, br_pixelmap* depth_buffer);
    void (*GetRenderSize)(int* width, int* height);
    void (*GetWindowSize)(int* width, int* height);
    void (*SetWindowSize)(int width, int height);
    void (*GetViewport)(int* x, int* y, int* width, int* height);
} tRenderer;

#endif
