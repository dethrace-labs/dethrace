#include "harness.h"

void Null_Init() {}
void Null_CreateWindow(char* title, int width, int height, int render_width, int render_height) {}
void Null_PollEvents() {}
int* Null_GetKeyMap() { return NULL; }
int Null_IsKeyDown(unsigned char pScan_code) { return 0; }
void Null_BeginFrame(br_actor* camera, br_pixelmap* colour_buffer) {}
void Null_EndFrame() {}
void Null_SetPalette(uint8_t* palette) {}
void Null_RenderFullScreenQuad(uint8_t* src, int width, int height) {}
void Null_RenderModel(br_model* model, br_matrix34 model_matrix) {}
void Null_RenderFrameBuffer() {}
void Null_Swap() {}
void Null_BufferTexture(br_pixelmap* pm) {}
void Null_BufferMaterial(br_material* mat) {}
void Null_FlushBuffers(br_pixelmap* color_buffer, br_pixelmap* depth_buffer) {}

tPlatform null_platform = {
    Null_Init,
    Null_CreateWindow,
    Null_PollEvents,
    Null_GetKeyMap,
    Null_IsKeyDown,
    Null_BeginFrame,
    Null_EndFrame,
    Null_SetPalette,
    Null_RenderFullScreenQuad,
    Null_RenderModel,
    Null_Swap,
    Null_BufferTexture,
    Null_BufferMaterial,
    Null_FlushBuffers
};