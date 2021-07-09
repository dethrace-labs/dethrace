#ifndef HARNESS_H
#define HARNESS_H

#include "br_types.h"
#include <SDL.h>

#define COUNT_OF(array) (sizeof((array)) / sizeof((array)[0]))

typedef struct tRenderer {
    int (*get_window_flags)();
    void (*init)(SDL_Window* window);
    void (*renderFrameBegin)();
    void (*renderFrameEnd)();
    void (*renderScreenBuffer)(uint32_t* src, int transparent);
    void (*swap)(SDL_Window* window);
    void (*renderCube)(float x, float y, float z);
} tRenderer;

typedef struct tHarness_GameMode {
    char* name;
    char* intro_smk_file;
} tHarness_GameMode;

extern tHarness_GameMode harness_game_mode;
extern int harness_disable_cd_check;
extern int harness_debug_level;

void Harness_Init(char* name, tRenderer* renderer);

void Harness_Debug_PrintStack();

#endif