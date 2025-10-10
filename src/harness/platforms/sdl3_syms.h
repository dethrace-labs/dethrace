#ifndef sdl3_syms_h
#define sdl3_syms_h

#include <SDL3/SDL.h>

#define FOREACH_SDL3_SYM(X)                                                                             \
    X(free, void, (void *))                                                                             \
    X(Init, bool, (Uint32))                                                                             \
    X(Quit, void, (void))                                                                               \
    X(Delay, void, (Uint32))                                                                            \
    X(GetTicks, Uint64, (void))                                                                         \
    X(GetError, const char*, (void))                                                                    \
    X(GetPointerProperty, void*, (SDL_PropertiesID, const char*, void*))                                \
    X(PollEvent, bool, (SDL_Event*))                                                                    \
    X(ShowSimpleMessageBox, bool, (SDL_MessageBoxFlags flags, const char*, const char *, SDL_Window*))  \
    X(CreateWindow, SDL_Window*, (const char*, int, int, SDL_WindowFlags))                              \
    X(DestroyWindow, void, (SDL_Window*))                                                               \
    X(GetWindowFlags, SDL_WindowFlags, (SDL_Window*))                                                   \
    X(GetPrefPath, char*, (const char *, const char *))                                                 \
    X(GetWindowID, SDL_WindowID, (SDL_Window*))                                                         \
    X(GetWindowSize, bool, (SDL_Window*, int*, int*))                                                   \
    X(HideCursor, bool, (void))                                                                         \
    X(SetWindowFullscreen, bool, (SDL_Window*, bool))                                                   \
    X(SetWindowSize, bool, (SDL_Window*, int, int))                                                     \
    X(CreateRenderer, SDL_Renderer*, (SDL_Window*, const char*))                                        \
    X(RenderClear, bool, (SDL_Renderer*))                                                               \
    X(RenderTexture, bool, (SDL_Renderer*, SDL_Texture*, const SDL_FRect*, const SDL_FRect*))           \
    X(RenderPresent, bool, (SDL_Renderer*))                                                             \
    X(RenderCoordinatesFromWindow, bool, (SDL_Renderer*, float, float, float*, float*))                 \
    X(GetRendererName, const char*, (SDL_Renderer*))                                                    \
    X(GetRendererProperties, SDL_PropertiesID, (SDL_Renderer*))                                         \
    X(SetRenderLogicalPresentation, bool, (SDL_Renderer*, int, int, SDL_RendererLogicalPresentation))   \
    X(SetRenderDrawBlendMode, bool, (SDL_Renderer*, SDL_BlendMode))                                     \
    X(SetRenderVSync, bool, (SDL_Renderer*, int))                                                       \
    X(CreateTexture, SDL_Texture*, (SDL_Renderer*, SDL_PixelFormat, SDL_TextureAccess, int, int))       \
    X(LockTexture, bool, (SDL_Texture*, const SDL_Rect*, void**, int*))                                 \
    X(UnlockTexture, void, (SDL_Texture*))                                                              \
    X(GetMouseFocus, SDL_Window*, (void))                                                               \
    X(GetMouseState, SDL_MouseButtonFlags, (float*, float*))                                            \
    X(ShowCursor, bool, (void))                                                                         \
    X(GetPixelFormatName, const char*, (SDL_PixelFormat))                                               \
    X(GetScancodeName, const char *, (SDL_Scancode))                                                    \
    X(GL_CreateContext, SDL_GLContext, (SDL_Window*))                                                   \
    X(GL_GetProcAddress, SDL_FunctionPointer, (const char*))                                            \
    X(GL_SetAttribute, bool, (SDL_GLAttr, int))                                                         \
    X(GL_SetSwapInterval, bool, (int))                                                                  \
    X(GL_SwapWindow, bool, (SDL_Window*))

#endif /* sdl3_syms_h */
