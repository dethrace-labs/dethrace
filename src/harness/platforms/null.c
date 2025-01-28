#include "null.h"

// todo: shouldnt depend on sdl...
#include <SDL.h>

static int null_set_window_pos(void* hWnd, int x, int y, int nWidth, int nHeight) {
    return 0;
}

static void null_destroy_window(void* hWnd) {
}

static int null_show_error_message(void* window, char* text, char* caption) {
    return 0;
}

static int null_get_and_handle_message(MSG_* msg) {
    return 0;
}

static void null_get_keyboard_state(unsigned int count, uint8_t* buffer) {
}

static int null_get_mouse_buttons(int* pButton1, int* pButton2) {
    return 0;
}

static int null_get_mouse_position(int* pX, int* pY) {
    return 0;
}

static int null_show_cursor(int show) {
    return 0;
}

static void null_set_palette(PALETTEENTRY_* palette) {
}

void Null_Platform_Init(tHarness_platform* platform) {
    platform->ProcessWindowMessages = null_get_and_handle_message;
    // todo: shouldnt depend on sdl...
    platform->Sleep = SDL_Delay;
    platform->GetTicks = SDL_GetTicks;
    platform->ShowCursor = null_show_cursor;
    platform->SetWindowPos = null_set_window_pos;
    platform->DestroyWindow = null_destroy_window;
    platform->GetKeyboardState = null_get_keyboard_state;
    platform->GetMousePosition = null_get_mouse_position;
    platform->GetMouseButtons = null_get_mouse_buttons;
    platform->ShowErrorMessage = null_show_error_message;

    platform->Renderer_SetPalette = null_set_palette;
}
