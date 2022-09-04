#ifndef HARNESS_PRIVATE_SDL_GL_H
#define HARNESS_PRIVATE_SDL_GL_H

typedef struct SDL_Window SDL_Window;
typedef void *SDL_GLContext;

typedef struct tSDLGLWindowState {
    SDL_Window* window;
    SDL_GLContext glContext;
} tSDLGLWindowState;

#endif
