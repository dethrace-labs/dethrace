#ifndef HARNESS_H
#define HARNESS_H

#include "br_types.h"
#include "debug.h"
#include <SDL2/SDL.h>

typedef int harness_game_func(void*);

typedef struct renderer {
    int (*get_window_flags)();
    void (*init)(SDL_Window* window);
    void (*activate)(SDL_Window* window);
    void (*doubleBuffer)(uint32_t* src, SDL_Window* window);
} renderer;

void Harness_Init(char* name, renderer* renderer);
void Harness_RunWindowLoop(harness_game_func* game_func, void* arg);

// Hooks are called from original game code.
void Harness_Hook_DOSGfxBegin();
void Harness_Hook_BrDevPaletteSetOld(br_pixelmap* pm);
void Harness_Hook_BrDevPaletteSetEntryOld(int i, br_colour colour);
void Harness_Hook_BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src);

void Harness_Hook_KeyBegin();
int Harness_Hook_KeyDown(unsigned char pScan_code);

#endif