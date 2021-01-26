#ifndef HARNESS_H
#define HARNESS_H

#include "br_types.h"
#include "debug.h"
#include <SDL2/SDL.h>

#define COUNT_OF(array) (sizeof((array)) / sizeof((array)[0]))

typedef struct tRenderer {
    int (*get_window_flags)();
    void (*init)(SDL_Window* window);
    void (*doubleBuffer)(uint32_t* src, SDL_Window* window);
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

// Hooks are called from original game code.
int Harness_Hook_HandleCommandLineArg(char* arg);
void Harness_Hook_DOSGfxBegin();
void Harness_Hook_BrDevPaletteSetOld(br_pixelmap* pm);
void Harness_Hook_BrDevPaletteSetEntryOld(int i, br_colour colour);
void Harness_Hook_BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src);

void Harness_Hook_KeyBegin();
int Harness_Hook_KeyDown(unsigned char pScan_code);

void Harness_Hook_PDServiceSystem(int pTime_since_last_call);

#endif