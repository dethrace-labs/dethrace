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
    void (*doubleBuffer)(br_pixelmap* dst, br_pixelmap* src, SDL_Window* window);
} renderer;

void Harness_Init(char* name, renderer* renderer);
void Harness_RunWindowLoop(harness_game_func* game_func, void* arg);

// Hooks are called from original game code.
void Harness_Hook_ActivateRenderer();
void Harness_Hook_SetPalette(br_pixelmap* pm);
void Harness_Hook_DoubleBuffer(br_pixelmap* dst, br_pixelmap* src);

#endif