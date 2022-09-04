#ifndef DEBUGUI_H
#define DEBUGUI_H

typedef struct tSDLGLWindowState tSDLGLWindowState;
typedef union SDL_Event SDL_Event;

#if defined(__cplusplus)
extern "C" {
#endif

extern int gEnableDebugUi;

typedef struct tWindowState tWindowState;

void DebugUi_Start(tSDLGLWindowState* windowState);

int DebugUI_OnEvent(SDL_Event* event);

void DebugUI_StartFrame(void);

void DebugUI_FinishFrame(void);

int DebugUI_MouseCaptured(void);

#if defined(__cplusplus)
}
#endif

#endif
