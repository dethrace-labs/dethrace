#ifndef sdl2_syms_h
#define sdl2_syms_h

#include <SDL.h>

#define FOREACH_SDL2_SYM(X)                                                             \
    X(Init, int, (Uint32))                                                              \
    X(Quit, void, (void))                                                               \
    X(Delay, void, (Uint32))                                                            \
    X(GetTicks, Uint32, (void))                                                         \
    X(GetError, const char*, (void))                                                    \
    X(PollEvent, int, (SDL_Event*))                                                     \
    X(ShowSimpleMessageBox, int, (Uint32, const char*, const char*, SDL_Window*))       \
    X(CreateWindow, SDL_Window*, (const char*, int, int, int, int, Uint32))             \
    X(DestroyWindow, void, (SDL_Window*))                                               \
    X(GetWindowFlags, Uint32, (SDL_Window*))                                            \
    X(GetWindowID, Uint32, (SDL_Window*))                                               \
    X(GetWindowSize, void, (SDL_Window*, int*, int*))                                   \
    X(SetWindowFullscreen, int, (SDL_Window*, Uint32))                                  \
    X(SetWindowSize, void, (SDL_Window*, int, int))                                     \
    X(CreateRenderer, SDL_Renderer*, (SDL_Window*, int, Uint32))                        \
    X(RenderClear, int, (SDL_Renderer*))                                                \
    X(RenderCopy, int, (SDL_Renderer*, SDL_Texture*, const SDL_Rect*, const SDL_Rect*)) \
    X(RenderPresent, void, (SDL_Renderer*))                                             \
    X(RenderWindowToLogical, void, (SDL_Renderer*, int, int, float*, float*))           \
    X(GetRendererInfo, int, (SDL_Renderer*, SDL_RendererInfo*))                         \
    X(RenderSetLogicalSize, int, (SDL_Renderer*, int, int))                             \
    X(SetRenderDrawBlendMode, int, (SDL_Renderer*, SDL_BlendMode))                      \
    X(CreateTexture, SDL_Texture*, (SDL_Renderer*, Uint32, int, int, int))              \
    X(LockTexture, int, (SDL_Texture*, const SDL_Rect*, void**, int*))                  \
    X(UnlockTexture, void, (SDL_Texture*))                                              \
    X(GetMouseFocus, SDL_Window*, (void))                                               \
    X(GetMouseState, Uint32, (int*, int*))                                              \
    X(ShowCursor, int, (int))                                                           \
    X(GetPixelFormatName, const char*, (Uint32))                                        \
    X(GetScancodeName, const char*, (SDL_Scancode))                                     \
    X(GL_CreateContext, SDL_GLContext, (SDL_Window*))                                   \
    X(GL_GetProcAddress, void*, (const char*))                                          \
    X(GL_SetAttribute, int, (SDL_GLattr, int))                                          \
    X(GL_SetSwapInterval, int, (int))                                                   \
    X(GL_SwapWindow, void, (SDL_Window*))                                               \
    X(GetPrefPath, char*, (const char* org, const char* app))                           \
    X(free, void, (void*))

#undef SDL2_SYM

#endif /* sdl2_syms_h */
