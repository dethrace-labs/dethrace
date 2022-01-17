#ifndef HARNESS_GL_RENDERER
#define HARNESS_GL_RENDERER

#include "harness.h"
#include <SDL.h>

void GLRenderer_CreateWindow(char* title, int width, int height);
void GLRenderer_BeginFrame(br_actor* camera, br_pixelmap* colour_buffer);
void GLRenderer_EndFrame();
void GLRenderer_RenderFullScreenQuad(uint32_t* screen_buffer, int transparent);
void GLRenderer_Swap();
void GLRenderer_RenderModel(br_model* model, br_matrix34 model_matrix);
void GLRenderer_BufferTexture(br_pixelmap* pm);
void GLRenderer_BufferMaterial(br_material* mat);

#endif