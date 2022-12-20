#ifndef HARNESS_PRIVATE_SDL_GL_H
#define HARNESS_PRIVATE_SDL_GL_H

typedef struct SDL_Window SDL_Window;
#if defined(RENDERER_OPENGL3)
typedef void *SDL_GLContext;
#endif

typedef struct tPlatformState {
    SDL_Window* window;
#if defined(RENDERER_OPENGL3)
    SDL_GLContext glContext;
#endif
} tPlatformState;

#endif
