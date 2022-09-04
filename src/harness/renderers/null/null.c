#include "renderer.h"

static void Null_Init() {}
static void Null_BeginScene(br_actor* camera, br_pixelmap* colour_buffer) {}
static void Null_EndScene() {}
static void Null_SetPalette(uint8_t* palette) {}
static void Null_RenderFullScreenQuad(uint8_t* src) {}
static void Null_RenderModel(br_actor* actor, br_model* model, br_matrix34 model_matrix) {}
static void Null_ClearBuffers() {}
static void Null_BufferTexture(br_pixelmap* pm) {}
static void Null_BufferMaterial(br_material* mat) {}
static void Null_BufferModel(br_model* model) {}
static void Null_FlushBuffers(br_pixelmap* color_buffer, br_pixelmap* depth_buffer) {}
static void Null_GetRenderSize(int* width, int* height) {
    *width = 640;
    *height = 480;
}
static void Null_GetWindowSize(int* width, int* height) {
    *width = 640;
    *height = 480;
}
static void Null_SetWindowSize(int width, int height) {}
static void Null_GetViewportSize(int* x, int* y, int* width, int* height) {
    *x = 0;
    *y = 0;
    *width = 640;
    *height = 480;
}
static void NULL_RenderDebugUI(void) {}

tRenderer null_renderer = {
    Null_Init,
    Null_BeginScene,
    Null_EndScene,
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
    Null_GetViewportSize,
    NULL_RenderDebugUI
};
