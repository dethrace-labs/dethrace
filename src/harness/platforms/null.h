#ifndef PLATFORM_NULL
#define PLATFORM_NULL

#include "harness/hooks.h"
// todo: shouldnt depend on sdl...
#include <SDL.h>

void* null_create_window_and_renderer(char* title, int x, int y, int width, int height) {
    return 0;
}

void null_set_window_pos(HWND hWnd, int x, int y, int nWidth, int nHeight) {
}

void null_destroy_window(HWND hWnd) {
}

int null_get_and_handle_message(MSG* msg) {

    return 0;
}

void null_swap_window() {
}

void null_get_keyboard_state(unsigned int count, uint8_t* buffer) {
}

void null_get_mouse_buttons(int* pButton1, int* pButton2) {
}

void null_get_mouse_position(int* pX, int* pY) {
}

void null_show_cursor(int show) {
}

void NullRenderer_Init() {}
void NullRenderer_BeginScene(br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer) {}
void NullRenderer_EndScene() {}
void NullRenderer_SetPalette(uint8_t* palette) {}
void NullRenderer_FullScreenQuad(uint8_t* src) {}
void NullRenderer_Model(br_actor* actor, br_model* model, br_material* material, br_matrix34 model_matrix, br_token render_type) {}
void NullRenderer_RenderFrameBuffer() {}
void NullRenderer_ClearBuffers() {}
void NullRenderer_BufferTexture(br_pixelmap* pm) {}
void NullRenderer_BufferMaterial(br_material* mat) {}
void NullRenderer_BufferModel(br_model* model) {}
void NullRenderer_FlushBuffers() {}
void NullRenderer_SetViewport(int x, int y, int width, int height) {}

void Null_Platform_Init(tHarness_platform* platform) {
    platform->GetMessage = null_get_and_handle_message;
    // todo: shouldnt depend on sdl...
    platform->Sleep = SDL_Delay;
    platform->GetTicks = SDL_GetTicks;
    platform->CreateWindow = null_create_window_and_renderer;
    platform->ShowCursor = null_show_cursor;
    platform->SetWindowPos = null_set_window_pos;
    platform->SwapWindow = null_swap_window;
    platform->DestroyWindow = null_destroy_window;
    platform->GetKeyboardState = null_get_keyboard_state;
    platform->GetMousePosition = null_get_mouse_position;
    platform->GetMouseButtons = null_get_mouse_buttons;
    platform->DestroyWindow = null_destroy_window;

    platform->Renderer_BufferModel = NullRenderer_BufferModel;
    platform->Renderer_BufferMaterial = NullRenderer_BufferMaterial;
    platform->Renderer_BufferTexture = NullRenderer_BufferTexture;
    platform->Renderer_SetPalette = NullRenderer_SetPalette;
    platform->Renderer_FullScreenQuad = NullRenderer_FullScreenQuad;
    platform->Renderer_Model = NullRenderer_Model;
    platform->Renderer_ClearBuffers = NullRenderer_ClearBuffers;
    platform->Renderer_FlushBuffers = NullRenderer_FlushBuffers;
    platform->Renderer_BeginScene = NullRenderer_BeginScene;
    platform->Renderer_EndScene = NullRenderer_EndScene;
    platform->Renderer_SetViewport = NullRenderer_SetViewport;
}

#endif
