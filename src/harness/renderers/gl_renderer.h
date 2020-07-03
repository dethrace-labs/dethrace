#ifndef HARNESS_GL_RENDERER
#define HARNESS_GL_RENDERER

#include "harness.h"
#include <SDL2/SDL.h>

extern renderer OpenGLRenderer;

void Harness_GLRenderer_GetWindowFlags();
void Harness_GLRenderer_Init(SDL_Window* window);
void Harness_GLRenderer_Activate(SDL_Window* window);
void Harness_GLRenderer_DoubleBuffer(br_pixelmap* dst, br_pixelmap* src, SDL_Window* window);

#endif