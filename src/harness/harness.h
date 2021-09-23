#ifndef HARNESS_H
#define HARNESS_H

#include "brender/br_types.h"
#include "harness_trace.h"
#include <SDL.h>

typedef struct tRenderer {
    int (*get_window_flags)();
    void (*init)(SDL_Window* window);
    void (*renderFrameBegin)(br_actor* camera);
    void (*renderFrameEnd)();
    void (*renderScreenBuffer)(uint32_t* src, int transparent);
    void (*swap)(SDL_Window* window);
    void (*renderModel)(br_model* model, br_matrix34 model_matrix);
    void (*renderCube)(float color, float x, float y, float z);
} tRenderer;

typedef struct tCamera {
    void (*update)();
    float* (*getProjection)();
    float* (*getView)();
    void (*setPosition)();
} tCamera;

typedef struct tHarness_GameMode {
    char* name;
    char* intro_smk_file;
} tHarness_GameMode;

extern tHarness_GameMode harness_game_mode;
extern int harness_disable_cd_check;

void Harness_Init(char* name, tRenderer* renderer);

void Harness_Debug_PrintStack();

#endif