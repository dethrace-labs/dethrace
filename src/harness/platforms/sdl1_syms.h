#ifndef sdl1_syms_h
#define sdl1_syms_h

#include <SDL.h>

#define FOREACH_SDL1_SYM(X)                                                 \
    X(Init, int, (Uint32))                                                  \
    X(Quit, void, (void))                                                   \
    X(Delay, void, (Uint32))                                                \
    X(GetTicks, Uint32, (void))                                             \
    X(GetError, char*, (void))                                              \
    X(PollEvent, int, (SDL_Event*))                                         \
    X(SetVideoMode, SDL_Surface*, (int, int, int, Uint32))                  \
    X(FreeSurface, void, (SDL_Surface*))                                    \
    X(LockSurface, int, (SDL_Surface*))                                     \
    X(UnlockSurface, void, (SDL_Surface*))                                  \
    X(UpdateRect, void, (SDL_Surface*, Sint32, Sint32, Sint32, Sint32))     \
    X(WM_SetCaption, void, (const char*, const char*))                      \
    X(WM_ToggleFullScreen, int, (SDL_Surface*))                             \
    X(Flip, int, (SDL_Surface*))                                            \
    X(ShowCursor, int, (int))                                               \
    X(GetMouseState, Uint8, (int*, int*))                                   \
    X(GetKeyName, char*, (SDLKey))                                          \
    X(GL_GetProcAddress, void*, (const char*))                              \
    X(GL_SwapBuffers, void, (void))

#endif /* sdl1_syms_h */
