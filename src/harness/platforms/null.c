#include "null.h"
#include <string.h>

static br_uint_32 null_time;

static int null_set_window_pos(void* hWnd, int x, int y, int nWidth, int nHeight) {
    null_time += 1;
    return 0;
}

static void null_destroy_window(void) {
    null_time += 1;
}

static int null_show_error_message(char* title, char* text) {
    null_time += 1;
    return 0;
}

static void null_get_and_handle_message(void) {
    null_time += 1;
}

static void null_get_keyboard_state(br_uint_32* buffer) {
    null_time += 1;
}

static int null_get_mouse_buttons(int* pButton1, int* pButton2) {
    null_time += 1;
    return 0;
}

static int null_get_mouse_position(int* pX, int* pY) {
    null_time += 1;
    return 0;
}

static int null_show_cursor(int show) {
    null_time += 1;
    return 0;
}

static void null_set_palette(br_colour* palette) {
    null_time += 1;
}

static void null_sleep(br_uint_32 milliseconds) {
    null_time += 1;
    null_time += milliseconds;
}

static br_uint_32 null_getticks(void) {
    null_time += 1;
    return null_time;
}

static void null_get_pref_path(char* path, char* app_name) {
    strcpy(path, ".");
}

void Null_Platform_Init(tHarness_platform* platform) {
    null_time = 0;
    platform->ProcessWindowMessages = null_get_and_handle_message;

    platform->Sleep = null_sleep;
    platform->GetTicks = null_getticks;
    platform->ShowCursor = null_show_cursor;
    platform->SetWindowPos = null_set_window_pos;
    platform->DestroyWindow = null_destroy_window;
    platform->GetKeyboardState = null_get_keyboard_state;
    platform->GetMousePosition = null_get_mouse_position;
    platform->GetMouseButtons = null_get_mouse_buttons;
    platform->ShowErrorMessage = null_show_error_message;
    platform->Renderer_SetPalette = null_set_palette;
}
