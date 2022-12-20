#ifndef DEBUGUI_H
#define DEBUGUI_H

#if defined(DETHRACE_DEBUGUI)

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct tPlatformState tPlatformState;

extern int gEnableDebugUi;

void DebugUi_Start(tPlatformState* windowState);

void DebugUI_StartFrame(void);

void DebugUI_FinishFrame(void);

int DebugUI_MouseCaptured(void);

int DebugUI_OnEvent(void* event);

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
