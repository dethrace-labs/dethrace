#ifndef HARNESS_KEYBOARD_H
#define HARNESS_KEYBOARD_H

void Keyboard_Init();
void Keyboard_HandleEvent(SDL_KeyboardEvent* key);
int Keyboard_IsKeyDown(unsigned char scan_code);
//void Keyboard_SetKeyArray(int* pKeys, int pMark);

#endif