#include "null.h"

// todo: shouldnt depend on sdl...
#include <SDL.h>

static void* null_create_window_and_renderer(char* title, int x, int y, int width, int height) {
    return 0;
}

static int null_set_window_pos(void* hWnd, int x, int y, int nWidth, int nHeight) {
    return 0;
}

static void null_destroy_window(void* hWnd) {
}

static int null_get_and_handle_message(MSG_* msg) {
    return 0;
}

static void null_swap_window(void) {
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

static void NullRenderer_BeginScene(br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer) {
}

static void NullRenderer_EndScene(void) {
}

static void NullRenderer_SetPalette(PALETTEENTRY_* palette) {
}

static void NullRenderer_FullScreenQuad(uint8_t* src) {
}

static void NullRenderer_Model(br_actor* actor, br_model* model, br_material* material, br_token render_type, br_matrix34 model_matrix) {
}

static void NullRenderer_ClearBuffers(void) {
}

static void NullRenderer_BufferTexture(br_pixelmap* pm) {
}

static void NullRenderer_BufferMaterial(br_material* mat) {
}

static void NullRenderer_BufferModel(br_model* model) {
}

static void NullRenderer_FlushBuffers(void) {
}

static void NullRenderer_SetViewport(int x, int y, int width, int height) {
}

void Null_Platform_Init(tHarness_platform* platform) {
    platform->ProcessWindowMessages = null_get_and_handle_message;
    // todo: shouldnt depend on sdl...
    platform->Sleep = SDL_Delay;
    platform->GetTicks = SDL_GetTicks;
    platform->CreateWindowAndRenderer = null_create_window_and_renderer;
    platform->ShowCursor = null_show_cursor;
    platform->SetWindowPos = null_set_window_pos;
    platform->SwapWindow = null_swap_window;
    platform->DestroyWindow = null_destroy_window;
    platform->GetKeyboardState = null_get_keyboard_state;
    platform->GetMousePosition = null_get_mouse_position;
    platform->GetMouseButtons = null_get_mouse_buttons;
    platform->DestroyWindow = null_destroy_window;

    platform->Renderer_SetPalette = NullRenderer_SetPalette;
    // platform->Renderer_FullScreenQuad = NullRenderer_FullScreenQuad;
}
