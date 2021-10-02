#ifndef SDL_PLATFORM_H
#define SDL_PLATFORM_H

#include "harness.h"

void SDLPlatform_Init();
void SDLPlatform_PollEvents();
int* SDLPlatform_GetKeyMap();
int SDLPlatform_IsKeyDown(unsigned char scan_code);

#endif