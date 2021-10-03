#include "harness.h"

void Null_Init() {}
void Null_CreateWindow(char* title, int width, int height) {}
void Null_PollEvents() {}
int* Null_GetKeyMap() { return NULL; }
int Null_IsKeyDown(unsigned char pScan_code) { return 0; }
void Null_BeginFrame(br_actor* camera, br_pixelmap* colour_buffer) {}
void Null_EndFrame() {}
void Null_RenderFullScreenQuad(uint32_t* src, int transparent) {}
void Null_RenderModel(br_model* model, br_matrix34 model_matrix) {}
void Null_Swap() {}

tPlatform null_platform = {
    Null_Init,
    Null_CreateWindow,
    Null_PollEvents,
    Null_GetKeyMap,
    Null_IsKeyDown,
    Null_BeginFrame,
    Null_EndFrame,
    Null_RenderFullScreenQuad,
    Null_RenderModel,
    Null_Swap
};