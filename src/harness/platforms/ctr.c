#include <3ds.h>

#include "platforms/ctr/ctr_gfx.h"

#include "harness.h"
#include "harness/config.h"
#include "harness/hooks.h"
#include "harness/trace.h"

uint32_t* rgba_buffer = NULL;
uint32_t converted_palette[256];
br_pixelmap* last_screen_src;

u32 last_frame_time;

uint8_t directinput_key_state[256] = {0};
float pos_x, pos_y;
bool button_1, button_2;

static void* ctr_create_renderer(char* title, int x, int y, int width, int height) {
	ctr_gfx_init();

    return NULL;
}

static int ctr_showcursor(int toggle) {
    return 1;
}

static int ctr_set_window_pos(void* hWnd, int x, int y, int nWidth, int nHeight) {
    return 0;
}

static void ctr_destroy_renderer(void* hWnd) {
    ctr_gfx_exit();

    if (rgba_buffer) {
        free(rgba_buffer);
        rgba_buffer = NULL;
    }
}

static int map_3ds_key_to_direct_input(u32 key) {
    switch (key) {
        case KEY_A:      return 0x39; // DIK_SPACE;
        case KEY_B:      return 0x30; // DIK_B;
        case KEY_X:      return 0x2D; // DIK_X;
        case KEY_Y:      return 0x15; // DIK_Y;
        case KEY_L:      return 0x26; // DIK_L;
//        case KEY_R:      return 0x13; // DIK_R // used for mouse button instead
        case KEY_ZL:     return 0x0C; // DIK_MINUS
        case KEY_ZR:     return 0x0D; // DIK_EQUALS
        case KEY_START:  return 0x1C; // DIK_RETURN
        case KEY_SELECT: return 0x01; // DIK_ESCAPE
        case KEY_DUP:    return 0x48; // DIK_NUMPAD8
        case KEY_DDOWN:  return 0x50; // DIK_NUMPAD2
        case KEY_DLEFT:  return 0x4B; // DIK_NUMPAD4
        case KEY_DRIGHT: return 0x4D; // DIK_NUMPAD6
        default: return -1;
    }
}

static int ctr_get_and_handle_message(MSG_* msg) {
    hidScanInput();
    u32 kHeld = hidKeysHeld();
    int dinput_key;

    circlePosition circlePad;
    hidCircleRead(&circlePad);

    for (u32 key = KEY_A; key <= KEY_ZR; key <<= 1) {
        dinput_key = map_3ds_key_to_direct_input(key);
        if (dinput_key >= 0) {
            if (kHeld & key) {
                directinput_key_state[dinput_key] = 0x80;
            } else {
                directinput_key_state[dinput_key] = 0x00;
            }
        }
    }

    if (circlePad.dy > 20) {
        directinput_key_state[0xC8] = 0x80; // DIK_UP
    } else if (circlePad.dy < -20) {
        directinput_key_state[0xD0] = 0x80; // DIK_DOWN
    } else {
        directinput_key_state[0xC8] = 0x00;
        directinput_key_state[0xD0] = 0x00;
    }

    if (circlePad.dx > 20) {
        directinput_key_state[0xCD] = 0x80; // DIK_RIGHT
    } else if (circlePad.dx < -20) {
        directinput_key_state[0xCB] = 0x80; // DIK_LEFT
    } else {
        directinput_key_state[0xCD] = 0x00;
        directinput_key_state[0xCB] = 0x00;
    }

    return 0;
}

static void ctr_get_keyboard_state(unsigned int count, uint8_t* buffer) {
    memcpy(buffer, directinput_key_state, count);
}

static int ctr_get_mouse_buttons(int* pButton1, int* pButton2) {
    *pButton1 = false; 
    *pButton2 = false;

    return 0;
}

static int ctr_get_mouse_position(int* pX, int* pY) {
    float lX = 0;
    float lY = 0;

    touchPosition touch;
    hidTouchRead(&touch);

    if ((touch.px > 0) && (touch.py > 0))
    {
        pos_x = touch.px;
        pos_y = touch.py;
    }

    *pX = pos_x;
    *pY = pos_y;

    *pX = (int)lX;
    *pY = (int)lY;

    return 0;
}

static void limit_fps(void) {
    u32 now = gHarness_platform.GetTicks();
    if (last_frame_time != 0) {
        unsigned int frame_time = now - last_frame_time;
        last_frame_time = now;
        if (frame_time < 100) {
            int sleep_time = (1000 / harness_game_config.fps) - frame_time;
            if (sleep_time > 5) {
                gHarness_platform.Sleep(sleep_time);
            }
        }
    }
    last_frame_time = gHarness_platform.GetTicks();
}

static void ctr_present_screen(br_pixelmap* src) {
    static int buffer_size = 0;

    int pixel_count = src->width * src->height;
    if (buffer_size < pixel_count) {
         if (rgba_buffer) {
            free(rgba_buffer);
        }

        rgba_buffer = (uint32_t*)malloc(pixel_count * sizeof(uint32_t));
        if (!rgba_buffer) {
            fprintf(stderr, "Failed to allocate RGBA buffer\n");
            return;
        }
        buffer_size = pixel_count;
    }

    uint8_t* src_pixels = src->pixels;

    for (int i = 0; i < pixel_count; i++) {
        rgba_buffer[i] = converted_palette[src_pixels[i]];
    }

    ctr_gfx_draw(rgba_buffer, src->width, src->height);

    last_screen_src = src;

    if (harness_game_config.fps != 0) {
        limit_fps();
    }
}

static void ctr_set_palette(PALETTEENTRY_* pal) {
    for (int i = 0; i < 256; i++) {
        converted_palette[i] = (pal[i].peRed << 24) | (pal[i].peGreen << 16) | (pal[i].peBlue << 8) |  0xFF;
    }

    if (last_screen_src != NULL) {
        ctr_present_screen(last_screen_src);
    }
}

static int ctr_show_error_message(void* window, char* text, char* caption) {
    fprintf(stderr, "%s", text);

    bool gfxInited = gspHasGpuRight();

    if (!gfxInited)
        gfxInitDefault();

    errorConf msg;
    errorInit(&msg, ERROR_TEXT, CFG_LANGUAGE_EN);
    errorText(&msg, text);
    errorDisp(&msg);

    if (!gfxInited)
        gfxExit();

    return 0;
}

static u32 ctr_getTicks(void) {
    u64 ticks = svcGetSystemTick();
    return (u32)(ticks * 1000 / SYSCLOCK_ARM11);
}

static void ctr_sleep(u32 ms)
{
    u64 ns = (u64)ms * 1000000;
    svcSleepThread(ns);
}

void Harness_Platform_Init(tHarness_platform* platform) {
    platform->ProcessWindowMessages = ctr_get_and_handle_message;
    platform->Sleep = ctr_sleep;
    platform->GetTicks = ctr_getTicks;
    platform->CreateWindowAndRenderer = ctr_create_renderer;
    platform->ShowCursor = ctr_showcursor;
    platform->SetWindowPos = ctr_set_window_pos;
    platform->DestroyWindow = ctr_destroy_renderer;
    platform->GetKeyboardState = ctr_get_keyboard_state;
    platform->GetMousePosition = ctr_get_mouse_position;
    platform->GetMouseButtons = ctr_get_mouse_buttons;
    platform->ShowErrorMessage = ctr_show_error_message;
    platform->Renderer_SetPalette = ctr_set_palette;
    platform->Renderer_Present = ctr_present_screen;
}
