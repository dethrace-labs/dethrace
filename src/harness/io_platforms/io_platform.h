#ifndef PLATFORM_H
#define PLATFORM_H

#include "../renderers/renderer.h"

tRenderer* Window_Create(char* title, int width, int height, int pRender_width, int pRender_height);
void Window_PollEvents(void);
void Window_Swap(int delay_ms_after_swap);
void Window_GetSize(int* pW, int* pH);
int* Input_GetKeyMap(void);
int Input_IsKeyDown(unsigned char scan_code);
void Input_GetMousePosition(int* pX, int* pY);
void Input_GetMouseButtons(int* pButton1, int* pButton2);

void Time_Delay_ms(int ms);

#endif
