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

typedef enum tHarness_game_type {
    eGame_carmageddon,
    eGame_splatpack,
    eGame_carmageddon_demo
} tHarness_game_type;

typedef struct tHarness_game_info {
    tHarness_game_type mode;
    char* intro_smk_file;
} tHarness_game_info;

extern tHarness_game_info harness_game_info;
extern int harness_disable_cd_check;

void Harness_Init(char* name, tRenderer* renderer);

void Harness_Debug_PrintStack();

#endif