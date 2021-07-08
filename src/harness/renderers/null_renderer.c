#include "null_renderer.h"

int Harness_NullRenderer_GetWindowFlags() {
    return 0;
}
void Harness_NullRenderer_Init(SDL_Window* window) {}
void Harness_NullRenderer_Activate(SDL_Window* window) {}
void Harness_NullRenderer_RenderScreenBuffer(uint32_t* src, int transparent) {}
void Harness_NullRenderer_Swap(SDL_Window* window) {}

void Harness_NullRenderer_RenderFrameBegin() {
}

void Harness_NullRenderer_RenderFrameEnd() {
}

tRenderer NullRenderer = {
    Harness_NullRenderer_GetWindowFlags,
    Harness_NullRenderer_Init,
    Harness_NullRenderer_RenderFrameBegin,
    Harness_NullRenderer_RenderFrameEnd,
    Harness_NullRenderer_RenderScreenBuffer,
    Harness_NullRenderer_Swap
};