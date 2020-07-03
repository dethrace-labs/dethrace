#include "null_renderer.h"

int Harness_NullRenderer_GetWindowFlags() {
    return 0;
}
void Harness_NullRenderer_Init(SDL_Window* window) {}
void Harness_NullRenderer_Activate(SDL_Window* window) {}
void Harness_NullRenderer_DoubleBuffer(br_pixelmap* dst, br_pixelmap* src, SDL_Window* window) {}

renderer NullRenderer = {
    Harness_NullRenderer_GetWindowFlags,
    Harness_NullRenderer_Init,
    Harness_NullRenderer_Activate,
    Harness_NullRenderer_DoubleBuffer
};