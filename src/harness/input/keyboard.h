#ifndef HARNESS_KEYBOARD_H
#define HARNESS_KEYBOARD_H

#include <SDL.h>

void Keyboard_Init();
void Keyboard_HandleEvent(SDL_KeyboardEvent* key);
int Keyboard_IsKeyDown(unsigned char scan_code);

#endif