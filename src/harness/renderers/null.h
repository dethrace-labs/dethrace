#include "renderer.h"

void Null_Init() {}
void Null_BeginFrame(br_actor* camera, br_pixelmap* colour_buffer) {}
void Null_EndFrame() {}
void Null_SetPalette(uint8_t* palette) {}
void Null_RenderFullScreenQuad(uint8_t* src, int width, int height) {}
void Null_RenderModel(br_actor* actor, br_model* model, br_matrix34 model_matrix) {}
void Null_RenderFrameBuffer() {}
void Null_ClearBuffers() {}
void Null_BufferTexture(br_pixelmap* pm) {}
void Null_BufferMaterial(br_material* mat) {}
void Null_BufferModel(br_model* model) {}
void Null_FlushBuffers(br_pixelmap* color_buffer, br_pixelmap* depth_buffer) {}

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
    Null_FlushBuffers
};