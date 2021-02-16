#ifndef HARNESS_GL_RENDERER
#define HARNESS_GL_RENDERER

#include "harness.h"
#include <SDL.h>

extern tRenderer OpenGLRenderer;

int Harness_GLRenderer_GetWindowFlags();
void Harness_GLRenderer_Init(SDL_Window* window);
void Harness_GLRenderer_Activate(SDL_Window* window);
void Harness_GLRenderer_DoubleBuffer(uint32_t* dst, SDL_Window* window);

#endif