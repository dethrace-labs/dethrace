#include "gl_renderer.h"
#include "harness.h"

#ifdef _WIN32
#include <gl/gl.h>
#elif defined __unix__
#include <GL/gl.h>
#elif defined __APPLE__
#include <OpenGL/gl.h>
#endif
#include <SDL2/SDL_opengl.h>

renderer OpenGLRenderer = {
    Harness_GLRenderer_GetWindowFlags,
    Harness_GLRenderer_Init,
    Harness_GLRenderer_Activate,
    Harness_GLRenderer_DoubleBuffer,
};

SDL_GLContext context;

void Harness_GLRenderer_GetWindowFlags() {
    return SDL_WINDOW_OPENGL;
}

void Harness_GLRenderer_Init(SDL_Window* window) {

    context = SDL_GL_CreateContext(window);
    if (!context) {
        LOG_PANIC("Failed to create context");
    }
}

void Harness_GLRenderer_Activate(SDL_Window* window) {

    SDL_GL_MakeCurrent(window, context);

    //SDL_GL_SwapWindow(window);
}

void Harness_GLRenderer_DoubleBuffer(br_pixelmap* dst, br_pixelmap* src, SDL_Window* window) {
    LOG_TRACE("(%p, %p)", dst, src);
    SDL_GL_SwapWindow(window);
}