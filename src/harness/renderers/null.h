#include "renderer.h"

void Null_Init() {}
void Null_BeginFrame(br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer) {}
void Null_EndFrame() {}
void Null_SetPalette(uint8_t* palette) {}
void Null_RenderFullScreenQuad(uint8_t* src) {}
void Null_RenderModel(br_actor* actor, br_model* model, br_material* material, br_token render_type, br_matrix34 model_matrix) {}
void Null_RenderFrameBuffer() {}
void Null_ClearBuffers() {}
void Null_BufferTexture(br_pixelmap* pm) {}
void Null_BufferMaterial(br_material* mat) {}
void Null_BufferModel(br_model* model) {}
void Null_FlushBuffers() {}
void Null_GetRenderSize(int* width, int* height) {
    *width = 640;
    *height = 480;
}
void Null_GetWindowSize(int* width, int* height) {
    *width = 640;
    *height = 480;
}
void Null_SetWindowSize(int width, int height) {}
void Null_GetViewportSize(int* x, int* y, int* width, int* height) {
    *x = 0;
    *y = 0;
    *width = 640;
    *height = 480;
}

tRenderer null_renderer = {
    Null_Init,
    Null_BeginFrame,
    Null_EndFrame,
    Null_SetPalette,
    Null_RenderFullScreenQuad,
    Null_RenderModel,
    Null_ClearBuffers,
    Null_BufferTexture,
    Null_BufferMaterial,
    Null_BufferModel,
    Null_FlushBuffers,
    Null_GetRenderSize,
    Null_GetWindowSize,
    Null_SetWindowSize,
    Null_GetViewportSize
};
