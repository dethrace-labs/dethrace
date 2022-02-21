#ifndef SDL_GL_PLATFORM_H
#define SDL_GL_PLATFORM_H

#include "harness.h"
#include "sdl/common.h"
#include "sdl/gl_renderer.h"

tPlatform sdl_gl_platform = {
    SDLPlatform_Init,
    GLRenderer_CreateWindow,
    SDLPlatform_PollEvents,
    SDLPlatform_GetKeyMap,
    SDLPlatform_IsKeyDown,
    GLRenderer_BeginScene,
    GLRenderer_EndScene,
    GLRenderer_SetPalette,
    GLRenderer_RenderFullScreenQuad,
    GLRenderer_RenderModel,
    GLRenderer_Swap,
    GLRenderer_BufferTexture,
    GLRenderer_BufferMaterial
};

#endif