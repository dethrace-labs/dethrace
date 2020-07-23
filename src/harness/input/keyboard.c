#include "dr_types.h"
#include <SDL2/SDL.h>

#include "keyboard.h"

extern tU8 gScan_code[123][2];

uint8_t sdl_key_state[123];

int keymap[123] = {
    SDL_SCANCODE_LSHIFT,
    SDL_SCANCODE_LALT,
    SDL_SCANCODE_LCTRL,
    -1,
    SDL_SCANCODE_CAPSLOCK,

    SDL_SCANCODE_RSHIFT,
    SDL_SCANCODE_RALT,
    SDL_SCANCODE_RCTRL,
    SDL_SCANCODE_LSHIFT,
    SDL_SCANCODE_LALT,
    SDL_SCANCODE_LCTRL,

    SDL_SCANCODE_0,
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_4,
    SDL_SCANCODE_5,
    SDL_SCANCODE_6,
    SDL_SCANCODE_7,
    SDL_SCANCODE_8,
    SDL_SCANCODE_9,
    SDL_SCANCODE_A,
    SDL_SCANCODE_B,
    SDL_SCANCODE_C,
    SDL_SCANCODE_D,
    SDL_SCANCODE_E,
    SDL_SCANCODE_F,
    SDL_SCANCODE_G,
    SDL_SCANCODE_H,
    SDL_SCANCODE_I,
    SDL_SCANCODE_J,
    SDL_SCANCODE_K,
    SDL_SCANCODE_L,
    SDL_SCANCODE_M,
    SDL_SCANCODE_N,
    SDL_SCANCODE_O,
    SDL_SCANCODE_P,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_R,
    SDL_SCANCODE_S,
    SDL_SCANCODE_T,
    SDL_SCANCODE_U,
    SDL_SCANCODE_V,
    SDL_SCANCODE_W,
    SDL_SCANCODE_X,
    SDL_SCANCODE_Y,
    SDL_SCANCODE_Z,

    SDL_SCANCODE_GRAVE,
    SDL_SCANCODE_MINUS,
    SDL_SCANCODE_EQUALS,
    SDL_SCANCODE_BACKSPACE,
    SDL_SCANCODE_RETURN,
    SDL_SCANCODE_KP_ENTER,
    SDL_SCANCODE_TAB,
    SDL_SCANCODE_KP_DIVIDE,
    SDL_SCANCODE_SLASH,
    SDL_SCANCODE_SEMICOLON,
    SDL_SCANCODE_APOSTROPHE,
    SDL_SCANCODE_PERIOD,
    SDL_SCANCODE_COMMA,
    SDL_SCANCODE_LEFTBRACKET,
    SDL_SCANCODE_RIGHTBRACKET,
    -1,
    SDL_SCANCODE_ESCAPE,
    SDL_SCANCODE_INSERT,
    SDL_SCANCODE_DELETE,
    SDL_SCANCODE_HOME,
    SDL_SCANCODE_END,
    SDL_SCANCODE_PAGEUP,
    SDL_SCANCODE_PAGEDOWN,
    SDL_SCANCODE_LEFT,
    SDL_SCANCODE_RIGHT,
    SDL_SCANCODE_UP,
    SDL_SCANCODE_DOWN,
    -1,
    SDL_SCANCODE_KP_DIVIDE,
    SDL_SCANCODE_KP_MULTIPLY,
    SDL_SCANCODE_KP_MINUS,
    SDL_SCANCODE_KP_PLUS,
    SDL_SCANCODE_KP_PERIOD,
    SDL_SCANCODE_KP_EQUALS,
    SDL_SCANCODE_KP_0,
    SDL_SCANCODE_KP_1,
    SDL_SCANCODE_KP_2,
    SDL_SCANCODE_KP_3,
    SDL_SCANCODE_KP_4,
    SDL_SCANCODE_KP_5,
    SDL_SCANCODE_KP_6,
    SDL_SCANCODE_KP_7,
    SDL_SCANCODE_KP_8,
    SDL_SCANCODE_KP_9,
    SDL_SCANCODE_F1,
    SDL_SCANCODE_F2,
    SDL_SCANCODE_F3,
    SDL_SCANCODE_F4,
    SDL_SCANCODE_F5,
    SDL_SCANCODE_F6,
    SDL_SCANCODE_F7,
    SDL_SCANCODE_F8,
    SDL_SCANCODE_F9,
    SDL_SCANCODE_F10,
    SDL_SCANCODE_F11,
    SDL_SCANCODE_F12,
    SDL_SCANCODE_PRINTSCREEN,
    -1,
    SDL_SCANCODE_PAUSE,
    SDL_SCANCODE_SPACE
};

void Keyboard_Init() {
    int i;
    for (i = 0; i < 123; i++) {
        gScan_code[i][0] = keymap[i];
        //gScan_code[i][1] = keymap[i];
    }
}

int Keyboard_IsKeyDown(unsigned char scan_code) {
    return sdl_key_state[scan_code];
}

void Keyboard_HandleEvent(SDL_KeyboardEvent* key) {
    if (key->keysym.scancode < 0 || key->keysym.scancode > 122) {
        LOG_WARN("unexpected scan code %d", key->keysym.scancode);
        return;
    }
    if (key->type == SDL_KEYDOWN) {
        sdl_key_state[key->keysym.scancode] = 1;
    } else {
        sdl_key_state[key->keysym.scancode] = 0;
    }
}

// void Keyboard_SetKeyArray(int* pKeys, int pMark) {
//     int i;
//     const uint8_t* state = SDL_GetKeyboardState(NULL);

//     for (int i = 0; i < 123; i++) {
//         if (keymap[i] <= 0) {
//             continue;
//         }
//         if (state[keymap[i]] > 0) {
//             printf("key down %d, %d, %d\n", i, keymap[i], gScan_code[i][0]);
//         }
//         if (Keyboard_IsKeyDown(106)) {
//             printf("space is down %d\n", i);
//         }
//         //pKeys[i] = keymap[i] > 0 ? state[keymap[i]] : 0;
//     }
// }