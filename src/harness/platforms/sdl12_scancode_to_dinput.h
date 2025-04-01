/*
 * Maps SDL2 scancodes to directinput keynums/scancodes.
 * Useful if you're porting a game that uses dinput scancodes internally
 * (for key bindings etc) or any other lib (like CEGUI) that uses them.
 *
 * (C) 2015 Daniel Gibson
 *
 * Homepage: https://github.com/DanielGibson/Snippets/
 *
 * License:
 *   This software is dual-licensed to the public domain and under the following
 *   license: you are granted a perpetual, irrevocable license to copy, modify,
 *   publish, and distribute this file as you see fit.
 *   No warranty implied; use at your own risk.
 *
 * So you can do whatever you want with this code, including copying it
 * (or parts of it) into your own source.
 * No need to mention me or this "license" in your code or docs, even though
 * it would be appreciated, of course.
 *
 */

#include <SDL.h>

#if 0 // Usage Example:
#include "sdl2_scancode_to_dinput.h"
 static int SDLScanCodeToKeyNum(SDL_Scancode sc)
 {
   int idx = (int)sc;
   assert(idx >= 0 && idx < SDL_NUM_SCANCODES);
   return scanCodeToKeyNum[idx];
 }

 static SDL_Scancode KeyNumToSDLScanCode( int keyNum )
 {
   if( keyNum >= 0 && keyNum < 0xEF )
   {
     for(int i = 0; i < SDL_NUM_SCANCODES; ++i)
     {
       if(scanCodeToKeyNum[i] == keyNum) return (SDL_Scancode)i;
     }
   }
   return SDL_SCANCODE_UNKNOWN;
 }
#endif // 0

#ifndef _SDL12_SCANCODE_TO_DINPUT_H_
#define _SDL12_SCANCODE_TO_DINPUT_H_

// TODO: map the following keys, if possible:
// #define DIK_UNDERLINE       0x93    /*                     (NEC PC98) */
// #define DIK_KANJI           0x94    /* (Japanese keyboard)            */
// #define DIK_AX              0x96    /*                     (Japan AX) */
// #define DIK_UNLABELED       0x97    /*                        (J3100) */
//
// #define DIK_WAKE            0xE3    /* System Wake */
//
// (#define DIK_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */ - system should map this to KP_COMMA or something,
//                                         according to USB doc, so probably it doesn't need mapping here)

// maps SDL12 keysyms to directinput keynums/scancodes - dinput_key = sdlScanCodeToDirectInputKeyNum[(int)your_sdl2_scancode];
static int sdl12KeyToDirectInputKeyNum[SDLK_LAST];

static void initializeSDL12KeyNums(void) {
    memset(sdl12KeyToDirectInputKeyNum, 0, sizeof(sdl12KeyToDirectInputKeyNum));
    sdl12KeyToDirectInputKeyNum[SDLK_a] = 0x1E;             /* DIK_A */
    sdl12KeyToDirectInputKeyNum[SDLK_b] = 0x30;             /* DIK_B */
    sdl12KeyToDirectInputKeyNum[SDLK_c] = 0x2E;             /* DIK_C */
    sdl12KeyToDirectInputKeyNum[SDLK_d] = 0x20;             /* DIK_D */
    sdl12KeyToDirectInputKeyNum[SDLK_e] = 0x12;             /* DIK_E */
    sdl12KeyToDirectInputKeyNum[SDLK_f] = 0x21;             /* DIK_F */
    sdl12KeyToDirectInputKeyNum[SDLK_g] = 0x22;             /* DIK_G */
    sdl12KeyToDirectInputKeyNum[SDLK_h] = 0x23;             /* DIK_H */
    sdl12KeyToDirectInputKeyNum[SDLK_i] = 0x17;             /* DIK_I */
    sdl12KeyToDirectInputKeyNum[SDLK_j] = 0x24;             /* DIK_J */
    sdl12KeyToDirectInputKeyNum[SDLK_k] = 0x25;             /* DIK_K */
    sdl12KeyToDirectInputKeyNum[SDLK_l] = 0x26;             /* DIK_L */
    sdl12KeyToDirectInputKeyNum[SDLK_m] = 0x32;             /* DIK_M */
    sdl12KeyToDirectInputKeyNum[SDLK_n] = 0x31;             /* DIK_N */
    sdl12KeyToDirectInputKeyNum[SDLK_o] = 0x18;             /* DIK_O */
    sdl12KeyToDirectInputKeyNum[SDLK_p] = 0x19;             /* DIK_P */
    sdl12KeyToDirectInputKeyNum[SDLK_q] = 0x10;             /* DIK_Q */
    sdl12KeyToDirectInputKeyNum[SDLK_r] = 0x13;             /* DIK_R */
    sdl12KeyToDirectInputKeyNum[SDLK_s] = 0x1F;             /* DIK_S */
    sdl12KeyToDirectInputKeyNum[SDLK_t] = 0x14;             /* DIK_T */
    sdl12KeyToDirectInputKeyNum[SDLK_u] = 0x16;             /* DIK_U */
    sdl12KeyToDirectInputKeyNum[SDLK_v] = 0x2F;             /* DIK_V */
    sdl12KeyToDirectInputKeyNum[SDLK_w] = 0x11;             /* DIK_W */
    sdl12KeyToDirectInputKeyNum[SDLK_x] = 0x2D;             /* DIK_X */
    sdl12KeyToDirectInputKeyNum[SDLK_y] = 0x15;             /* DIK_Y */
    sdl12KeyToDirectInputKeyNum[SDLK_z] = 0x2C;             /* DIK_Z */
    sdl12KeyToDirectInputKeyNum[SDLK_0] = 0x2C;             /* DIK_Z */
    sdl12KeyToDirectInputKeyNum[SDLK_1] = 0x02;             /* DIK_1 */
    sdl12KeyToDirectInputKeyNum[SDLK_2] = 0x03;             /* DIK_2 */
    sdl12KeyToDirectInputKeyNum[SDLK_3] = 0x04;             /* DIK_3 */
    sdl12KeyToDirectInputKeyNum[SDLK_4] = 0x05;             /* DIK_4 */
    sdl12KeyToDirectInputKeyNum[SDLK_5] = 0x06;             /* DIK_5 */
    sdl12KeyToDirectInputKeyNum[SDLK_6] = 0x07;             /* DIK_6 */
    sdl12KeyToDirectInputKeyNum[SDLK_7] = 0x08;             /* DIK_7 */
    sdl12KeyToDirectInputKeyNum[SDLK_8] = 0x09;             /* DIK_8 */
    sdl12KeyToDirectInputKeyNum[SDLK_9] = 0x0A;             /* DIK_9 */
    sdl12KeyToDirectInputKeyNum[SDLK_0] = 0x0B;             /* DIK_0 */

    sdl12KeyToDirectInputKeyNum[SDLK_KP1] = 0x4F;           /* DIK_NUMPAD1 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP2] = 0x50;           /* DIK_NUMPAD2 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP3] = 0x51;           /* DIK_NUMPAD3 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP4] = 0x4B;           /* DIK_NUMPAD4 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP5] = 0x4C;           /* DIK_NUMPAD5 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP6] = 0x4D;           /* DIK_NUMPAD6 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP7] = 0x47;           /* DIK_NUMPAD7 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP8] = 0x48;           /* DIK_NUMPAD8 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP9] = 0x49;           /* DIK_NUMPAD9 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP0] = 0x52;           /* DIK_NUMPAD0 */

    sdl12KeyToDirectInputKeyNum[SDLK_RETURN] = 0x1C;        /* DIK_RETURN */
    sdl12KeyToDirectInputKeyNum[SDLK_ESCAPE] = 0x01;        /* DIK_ESCAPE */
    sdl12KeyToDirectInputKeyNum[SDLK_BACKSPACE] = 0x0E;     /* DIK_BACK */
    sdl12KeyToDirectInputKeyNum[SDLK_TAB] = 0x0F;           /* DIK_TAB */
    sdl12KeyToDirectInputKeyNum[SDLK_SPACE] = 0x39;         /* DIK_SPACE */

    sdl12KeyToDirectInputKeyNum[SDLK_MINUS] = 0x0C;         /* DIK_MINUS */
    sdl12KeyToDirectInputKeyNum[SDLK_EQUALS] = 0x0D;        /* DIK_EQUALS */
    sdl12KeyToDirectInputKeyNum[SDLK_LEFTBRACKET] = 0x1A;   /* DIK_LBRACKET */
    sdl12KeyToDirectInputKeyNum[SDLK_RIGHTBRACKET] = 0x1B;  /* DIK_RBRACKET */
    sdl12KeyToDirectInputKeyNum[SDLK_BACKSLASH] = 0x2B;     /* DIK_BACKSLASH */
    sdl12KeyToDirectInputKeyNum[SDLK_HASH] = 0x2B;          /* DIK_BACKSLASH */
    sdl12KeyToDirectInputKeyNum[SDLK_SEMICOLON] = 0x27;     /* DIK_SEMICOLON */
    sdl12KeyToDirectInputKeyNum[SDLK_QUOTE] = 0x28;         /* DIK_APOSTROPHE */
    sdl12KeyToDirectInputKeyNum[SDLK_BACKQUOTE] = 0x29;     /* DIK_GRAVE */
    sdl12KeyToDirectInputKeyNum[SDLK_COMMA] = 0x33;         /* DIK_COMMA */
    sdl12KeyToDirectInputKeyNum[SDLK_PERIOD] = 0x34;        /* DIK_PERIOD */
    sdl12KeyToDirectInputKeyNum[SDLK_SLASH] = 0x35;         /* DIK_SLASH */

    sdl12KeyToDirectInputKeyNum[SDLK_CAPSLOCK] = 0x3A;      /* DIK_CAPITAL */

    sdl12KeyToDirectInputKeyNum[SDLK_F1] = 0x3A;      /* DIK_CAPITAL */

    sdl12KeyToDirectInputKeyNum[SDLK_F1] = 0x3B;            /* DIK_F1 */
    sdl12KeyToDirectInputKeyNum[SDLK_F2] = 0x3C;            /* DIK_F2 */
    sdl12KeyToDirectInputKeyNum[SDLK_F3] = 0x3D;            /* DIK_F3 */
    sdl12KeyToDirectInputKeyNum[SDLK_F4] = 0x3E;            /* DIK_F4 */
    sdl12KeyToDirectInputKeyNum[SDLK_F5] = 0x3F;            /* DIK_F5 */
    sdl12KeyToDirectInputKeyNum[SDLK_F6] = 0x40;            /* DIK_F6 */
    sdl12KeyToDirectInputKeyNum[SDLK_F7] = 0x41;            /* DIK_F7 */
    sdl12KeyToDirectInputKeyNum[SDLK_F8] = 0x42;            /* DIK_F8 */
    sdl12KeyToDirectInputKeyNum[SDLK_F9] = 0x43;            /* DIK_F9 */
    sdl12KeyToDirectInputKeyNum[SDLK_F10] = 0x44;           /* DIK_F10 */
    sdl12KeyToDirectInputKeyNum[SDLK_F11] = 0x57;           /* DIK_F11 */
    sdl12KeyToDirectInputKeyNum[SDLK_F12] = 0x58;           /* DIK_F12 */

    sdl12KeyToDirectInputKeyNum[SDLK_PRINT] = 0xB7;         /* DIK_F12 */

    sdl12KeyToDirectInputKeyNum[SDLK_SCROLLOCK] = 0x46;     /* DIK_SCROLL */
    sdl12KeyToDirectInputKeyNum[SDLK_PAUSE] =  0xC5;        /* DIK_PAUSE */
    sdl12KeyToDirectInputKeyNum[SDLK_INSERT] = 0xD2;        /* DIK_INSERT */
    sdl12KeyToDirectInputKeyNum[SDLK_HOME] = 0xC7;          /* DIK_HOME */
    sdl12KeyToDirectInputKeyNum[SDLK_PAGEUP] = 0xC9;        /* DIK_PRIOR */
    sdl12KeyToDirectInputKeyNum[SDLK_DELETE] = 0xD3;        /* DIK_DELETE */
    sdl12KeyToDirectInputKeyNum[SDLK_END] = 0xCF;           /* DIK_END */
    sdl12KeyToDirectInputKeyNum[SDLK_PAGEDOWN] = 0xD1;      /* DIK_NEXT */
    sdl12KeyToDirectInputKeyNum[SDLK_RIGHT] = 0xCD;         /* DIK_RIGHT */
    sdl12KeyToDirectInputKeyNum[SDLK_LEFT] = 0xCB;          /* DIK_LEFT */
    sdl12KeyToDirectInputKeyNum[SDLK_DOWN] = 0xD0;          /* DIK_DOWN */
    sdl12KeyToDirectInputKeyNum[SDLK_UP] = 0xC8;            /* DIK_UP */

    sdl12KeyToDirectInputKeyNum[SDLK_NUMLOCK] = 0x45;       /* DIK_NUMLOCK */

    sdl12KeyToDirectInputKeyNum[SDLK_KP_DIVIDE] = 0xB5;     /* DIK_DIVIDE */
    sdl12KeyToDirectInputKeyNum[SDLK_KP_MULTIPLY] = 0x37;   /* DIK_MULTIPLY */
    sdl12KeyToDirectInputKeyNum[SDLK_KP_MINUS] = 0x4A;      /* DIK_SUBTRACT */
    sdl12KeyToDirectInputKeyNum[SDLK_KP_PLUS] = 0x4E;       /* DIK_ADD */
    sdl12KeyToDirectInputKeyNum[SDLK_KP_ENTER] = 0x9C;      /* DIK_NUMPADENTER */
    sdl12KeyToDirectInputKeyNum[SDLK_KP1] = 0x4F;           /* DIK_NUMPAD1 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP2] = 0x50;           /* DIK_NUMPAD2 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP3] = 0x51;           /* DIK_NUMPAD3 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP4] = 0x4B;           /* DIK_NUMPAD4 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP5] = 0x4C;           /* DIK_NUMPAD5 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP6] = 0x4D;           /* DIK_NUMPAD6 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP7] = 0x47;           /* DIK_NUMPAD7 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP8] = 0x48;           /* DIK_NUMPAD8 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP9] = 0x49;           /* DIK_NUMPAD9 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP0] = 0x52;           /* DIK_NUMPAD0 */
    sdl12KeyToDirectInputKeyNum[SDLK_KP_PERIOD] = 0x53;     /* DIK_DECIMAL */

    /* 0x56 - DIK_OEM_102 */
    /* 0xDD - DIK_APPS */
    sdl12KeyToDirectInputKeyNum[SDLK_POWER] = 0xDE;         /* DIK_POWER */

    sdl12KeyToDirectInputKeyNum[SDLK_KP_EQUALS] = 0x8D;     /* DIK_NUMPADEQUALS */

    sdl12KeyToDirectInputKeyNum[SDLK_F13] = 0x8D;           /* DIK_F13 */
    sdl12KeyToDirectInputKeyNum[SDLK_F14] = 0x64;           /* DIK_F14 */
    sdl12KeyToDirectInputKeyNum[SDLK_F15] = 0x65;           /* DIK_F15 */

    sdl12KeyToDirectInputKeyNum[SDLK_SYSREQ] = 0xB7;        /* SDL_SCANCODE_SYSREQ */
}

#endif /* _SDL12_SCANCODE_TO_DINPUT_H_ */
