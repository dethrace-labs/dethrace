#ifndef HARNESS_H
#define HARNESS_H

#include "brender/br_types.h"
#include "harness/trace.h"
#include "renderers/renderer.h"

void Harness_ForceNullRenderer();

typedef struct tCamera {
    void (*update)();
    float* (*getProjection)();
    float* (*getView)();
    void (*setPosition)();
} tCamera;

// platform stuff. See cmake variable RENDERER_PLATFORM
tRenderer* Window_Create(char* title, int width, int height, int pRender_width, int pRender_height);
void Window_PollEvents(void);
void Window_Swap(int delay_ms_after_swap);
int* Input_GetKeyMap(void);
int Input_IsKeyDown(unsigned char scan_code);

#endif