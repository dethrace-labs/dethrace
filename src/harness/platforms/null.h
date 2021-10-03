#include "harness.h"

void Init() {}
void CreateWindow(char* title, int width, int height) {}
void PollEvents() {}
int* GetKeyMap() { return NULL; }
int IsKeyDown(unsigned char pScan_code) { return 0; }
void BeginFrame(br_actor* camera, br_pixelmap* colour_buffer) {}
void EndFrame() {}
void RenderFullScreenQuad(uint32_t* src, int transparent) {}
void RenderModel(br_model* model, br_matrix34 model_matrix) {}
void Swap() {}

tPlatform null_platform = {
    Init,
    CreateWindow,
    PollEvents,
    GetKeyMap,
    IsKeyDown,
    BeginFrame,
    EndFrame,
    RenderFullScreenQuad,
    RenderModel,
    Swap
};