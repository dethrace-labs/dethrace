#ifndef PLATFORM_H
#define PLATFORM_H

#include "../renderers/renderer.h"

void IOPlatform_Init();
tRenderer* IOPlatform_CreateWindow(char* title, int width, int height, int pRender_width, int pRender_height);
void IOPlatform_PollEvents(void);
void IOPlatform_SwapWindow(int delay_ms_after_swap);
uint8_t* IOPlatform_GetKeyboardState();
void IOPlatform_GetMousePosition(int* pX, int* pY);
void IOPlatform_GetMouseButtons(int* pButton1, int* pButton2);
void IOPlatform_Shutdown();

#endif
