#ifndef HARNESS_HOOKS_H
#define HARNESS_HOOKS_H

#include "brender/br_types.h"
#include "harness/win95_polyfill_defs.h"
#include <stdio.h>

// Platform implementation functions
typedef struct tHarness_platform {
    // Initialize the renderer
    void (*Renderer_Init)(int width, int height, int pRender_width, int pRender_height);
    // Called when beginning a 3D scene
    void (*Renderer_BeginScene)(br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer);
    // Called at the end of a 3D scene
    void (*Renderer_EndScene)();
    // Render a fullscreen quad using the specified pixel data
    void (*Renderer_FullScreenQuad)(uint8_t* src);
    // Render a model
    void (*Renderer_Model)(br_actor* actor, br_model* model, br_matrix34 model_matrix, br_token render_type);
    // Clear frame and depth buffers
    void (*Renderer_ClearBuffers)();
    // Load pixelmap into video memory
    void (*Renderer_BufferTexture)(br_pixelmap* pm);
    // Load material
    void (*Renderer_BufferMaterial)(br_material* mat);
    // Load model into video memory
    void (*Renderer_BufferModel)(br_model* model);
    // Pull contents of frame and depth buffers from video into main memory for software effects
    void (*Renderer_FlushBuffers)();
    // Set the 256 color palette to use (BGRA format)
    void (*Renderer_SetPalette)(PALETTEENTRY* palette);
    // Set the viewport for 3d rendering
    void (*Renderer_SetViewport)(int x, int y, int width, int height);
    // Create a window. Return a handle to the window
    void* (*CreateWindow)(char* title, int x, int y, int nWidth, int nHeight);
    // Get mouse button state
    int (*GetMouseButtons)(int* button_1, int* button_2);
    // Get mouse position
    int (*GetMousePosition)(int* pX, int* pY);
    // Close specified window
    void (*DestroyWindow)(HWND window);
    // Pull a message from the window event queue
    int (*GetMessage)(LPMSG msg);
    // Set position of a window
    int (*SetWindowPos)(HWND hWnd, int x, int y, int nWidth, int nHeight);
    // Show/hide the cursor
    int (*ShowCursor)(int show);
    // Get keyboard state. Expected to be in DirectInput key codes
    void (*GetKeyboardState)(unsigned int count, uint8_t* buffer);
    // Sleep
    void (*Sleep)(uint32_t dwMilliseconds);
    // Get ticks
    uint32_t (*GetTicks)();
    // Swap window
    void (*SwapWindow)();

} tHarness_platform;

extern tHarness_platform gHarness_platform;

void Harness_Init(int* argc, char* argv[]);

// Hooks are called from original game code.

// BRender hooks
void Harness_Hook_BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src);
void Harness_Hook_BrV1dbRendererBegin(br_v1db_state* v1db);
void Harness_Hook_renderActor(br_actor* actor, br_model* model, br_material* material, br_token type);

// Sound hooks
void Harness_Hook_S3Service(int unk1, int unk2);
void Harness_Hook_S3StopAllOutletSounds();

// Filesystem hooks
FILE* Harness_Hook_fopen(const char* pathname, const char* mode);

void Harness_RenderLastScreen();

#endif
