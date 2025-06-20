#ifndef HARNESS_HOOKS_H
#define HARNESS_HOOKS_H

#include "brender.h"
#include <stdio.h>

typedef enum tHarness_window_type {
    eWindow_type_software = 0,
    eWindow_type_opengl = 1,
} tHarness_window_type;

// Platform implementation functions
typedef struct tHarness_platform {
    // Render a fullscreen quad using the specified pixel data
    void (*Renderer_Present)(br_pixelmap* src);
    // Set the 256 color palette to use (BGRA format)
    void (*Renderer_SetPalette)(br_colour* palette);
    // Get mouse button state
    int (*GetMouseButtons)(int* button_1, int* button_2);
    // Get mouse position
    int (*GetMousePosition)(int* pX, int* pY);
    // Close specified window
    void (*DestroyWindow)(void);
    // Process window messages, return any WM_QUIT message
    void (*ProcessWindowMessages)(void);
    // Set position of a window
    int (*SetWindowPos)(void* hWnd, int x, int y, int nWidth, int nHeight);
    // Show/hide the cursor
    int (*ShowCursor)(int show);
    // invoked when key is set up or down
    void (*SetKeyHandler)(void (*handler_func)(void));
    // Get keyboard state. Argument expected to point to 32 byte buffer - 1 bit per key
    void (*GetKeyboardState)(br_uint_32* buffer);

    // Sleep
    void (*Sleep)(br_uint_32 dwMilliseconds);
    // Get ticks
    br_uint_32 (*GetTicks)(void);
    // Show error message
    int (*ShowErrorMessage)(char* title, char* message);

    // Create a window. Uses an underscore to avoid name collisions with windows.h `CreateWindow` macro
    void (*CreateWindow_)(const char* title, int nWidth, int nHeight, tHarness_window_type window_type);
    void (*Swap)(br_pixelmap* back_buffer);
    void (*PaletteChanged)(br_colour entries[256]);
    // If this platform supports OpenGL
    void* (*GL_GetProcAddress)(const char* name);
    void (*GetViewport)(int* x, int* y, float* width_multiplier, float* height_multiplier);

} tHarness_platform;

enum {
    ePlatform_cap_software = 0x1,
    ePlatform_cap_opengl = 0x2,
    ePlatform_cap_video_mask = ePlatform_cap_software | ePlatform_cap_opengl,
};

typedef struct tPlatform_bootstrap {
    const char* name;
    const char* description;
    int capabilities;
    int (*init)(tHarness_platform* platform);
} tPlatform_bootstrap;

extern tHarness_platform gHarness_platform;

int Harness_Init(int* argc, char* argv[]);

// Hooks are called from original game code.

// Filesystem hooks
FILE* Harness_Hook_fopen(const char* pathname, const char* mode);

// Localization
int Harness_Hook_isalnum(int c);

#endif
