#ifndef DEBUGUI_H
#define DEBUGUI_H

#if defined(DETHRACE_DEBUGUI)

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(PLATFORM_SDL2)

typedef struct tSDLGLWindowState tSDLGLWindowState;
typedef union SDL_Event SDL_Event;

int DebugUI_OnEvent(SDL_Event* event);

#endif

extern int gEnableDebugUi;

void DebugUi_Start(void* windowState);

void DebugUI_StartFrame(void);

void DebugUI_FinishFrame(void);

int DebugUI_MouseCaptured(void);

#if defined(__cplusplus)
}
#endif

#else

#define DebugUI_OnEvent(EVENT) (0)

#define DebugUi_Start(STATE)

#define DebugUI_StartFrame()

#define DebugUI_FinishFrame()

#define DebugUI_MouseCaptured() (0)

#endif

#endif
