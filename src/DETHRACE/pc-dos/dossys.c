#include "brender.h"
#include "car.h"
#include "errors.h"
#include "globvars.h"
#include "globvrbm.h"
#include "grafdata.h"
#include "graphics.h"
#include "harness/config.h"
#include "harness/hooks.h"
#include "harness/os.h"
#include "harness/trace.h"
#include "init.h"
#include "input.h"
#include "loadsave.h"
#include "main.h"
#include "pc-dos/scancodes.h"
#include "pd/sys.h"
#include "sound.h"
#include "utility.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

// #ifdef __DOS__

// This code comes from DOS, so small changes need to be made to run correctly on windowed systems.
// Generally the pc-win95 does the same thing
#define PLAY_NICE_WITH_GUI 1

int gDOSGfx_initialized;
int gExtra_mem;
int gReplay_override;
tGraf_spec gGraf_specs[2] = {
    { 8, 1, 0, 320, 200, 0, 0, "32X20X8", "MCGA,W:320,H:200,B:8", 320, 320, 200, NULL },
    { 8, 1, 0, 640, 480, 0, 0, "64X48X8", "VESA,W:640,H:480,B:8", 640, 640, 480, NULL }
    // { 8, 1, 0, 1920, 1080, 0, 0, "64X48X8", "VESA,W:640,H:480,B:8", 640, 1920, 1080, NULL }
};
int gASCII_table[128];
tU32 gKeyboard_bits[8];
int gASCII_shift_table[128];
char gNetwork_profile_fname[256];
tS32 gJoystick_min1y;
tS32 gJoystick_min2y;
tS32 gJoystick_min2x;
tS32 gRaw_joystick2y;
tS32 gRaw_joystick2x;
tS32 gRaw_joystick1y;
tS32 gRaw_joystick1x;
tS32 gJoystick_range2y;
tS32 gJoystick_range2x;
tS32 gJoystick_range1y;
tS32 gJoystick_range1x;
int gNo_voodoo;
int gSwitched_resolution;
br_pixelmap* gReal_back_screen;
tS32 gJoystick_min1x;
br_pixelmap* gTemp_screen;
tU32 gUpper_loop_limit;
int gReal_back_screen_locked;
void (*gPrev_keyboard_handler)(void);
tU8 gScan_code[123][2];

// Added from retail executable
int gForce_voodoo_rush_mode;
int gForce_voodoo_mode;

br_device_gl_callback_procs gl_callbacks;
br_device_virtualfb_callback_procs virtualfb_callbacks;

// forward declare for `PDInitialiseSystem`
int InitJoysticks(void);

// IDA: void __cdecl KeyboardHandler()
void KeyboardHandler(void) {
    tU8 scan_code;
    tU8 up;
    static tU8 extended;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall KeyDown@<EAX>(tU8 pScan_code@<EAX>)
int KeyDown(tU8 pScan_code) {
    return (gKeyboard_bits[pScan_code >> 5] >> (pScan_code & 0x1F)) & 1;
}

// IDA: void __usercall KeyTranslation(tU8 pKey_index@<EAX>, tU8 pScan_code_1@<EDX>, tU8 pScan_code_2@<EBX>)
void KeyTranslation(tU8 pKey_index, tU8 pScan_code_1, tU8 pScan_code_2) {
    LOG_TRACE("(%d, %d, %d)", pKey_index, pScan_code_1, pScan_code_2);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl KeyBegin()
void KeyBegin(void) {

    gScan_code[KEY_0][0] = SCANCODE_0;
    gScan_code[KEY_2][0] = SCANCODE_2;
    gScan_code[KEY_3][0] = SCANCODE_3;
    gScan_code[KEY_1][0] = SCANCODE_1;
    gScan_code[KEY_4][0] = SCANCODE_4;
    gScan_code[KEY_6][0] = SCANCODE_6;
    gScan_code[KEY_7][0] = SCANCODE_7;
    gScan_code[KEY_5][0] = SCANCODE_5;
    gScan_code[KEY_8][0] = SCANCODE_8;
    gScan_code[KEY_A][0] = SCANCODE_A;
    gScan_code[KEY_B][0] = SCANCODE_B;
    gScan_code[KEY_9][0] = SCANCODE_9;
    gScan_code[KEY_C][0] = SCANCODE_C;
    gScan_code[KEY_E][0] = SCANCODE_E;
    gScan_code[KEY_F][0] = SCANCODE_F;
    gScan_code[KEY_D][0] = SCANCODE_D;
    gScan_code[KEY_G][0] = SCANCODE_G;
    gScan_code[KEY_I][0] = SCANCODE_I;
    gScan_code[KEY_J][0] = SCANCODE_J;
    gScan_code[KEY_H][0] = SCANCODE_H;
    gScan_code[KEY_K][0] = SCANCODE_K;
    gScan_code[KEY_M][0] = SCANCODE_M;
    gScan_code[KEY_N][0] = SCANCODE_N;
    gScan_code[KEY_L][0] = SCANCODE_L;
    gScan_code[KEY_O][0] = SCANCODE_O;
    gScan_code[KEY_Q][0] = SCANCODE_Q;
    gScan_code[KEY_R][0] = SCANCODE_R;
    gScan_code[KEY_P][0] = SCANCODE_P;
    gScan_code[KEY_S][0] = SCANCODE_S;
    gScan_code[KEY_U][0] = SCANCODE_U;
    gScan_code[KEY_V][0] = SCANCODE_V;
    gScan_code[KEY_T][0] = SCANCODE_T;
    gScan_code[KEY_W][0] = SCANCODE_W;
    gScan_code[KEY_X][0] = SCANCODE_X;
    gScan_code[KEY_Y][0] = SCANCODE_Y;
    gScan_code[KEY_Z][0] = SCANCODE_Z;
    gScan_code[KEY_GRAVE][0] = SCANCODE_GRAVE;
    gScan_code[KEY_MINUS][0] = SCANCODE_MINUS;
    gScan_code[KEY_EQUALS][0] = SCANCODE_EQUALS;
    gScan_code[KEY_BACKSPACE][0] = SCANCODE_BACK;
    gScan_code[KEY_RETURN][0] = SCANCODE_RETURN;
    gScan_code[KEY_KP_ENTER][0] = SCANCODE_NUMPADENTER;

    gScan_code[KEY_SHIFT_ANY][0] = SCANCODE_LSHIFT;
    gScan_code[KEY_SHIFT_ANY][1] = SCANCODE_RSHIFT;
    gScan_code[KEY_ALT_ANY][0] = SCANCODE_LALT;
    gScan_code[KEY_ALT_ANY][1] = SCANCODE_RALT;
    gScan_code[KEY_CTRL_ANY][0] = SCANCODE_LCONTROL;
    gScan_code[KEY_CTRL_ANY][1] = SCANCODE_RCONTROL;
    gScan_code[KEY_CTRL_ANY_2][0] = SCANCODE_LCONTROL;
    gScan_code[KEY_CTRL_ANY_2][1] = SCANCODE_RCONTROL;

    gScan_code[KEY_CAPSLOCK][0] = SCANCODE_CAPITAL;
    gScan_code[KEY_UNKNOWN_55][0] = SCANCODE_OEM_102;
    gScan_code[KEY_SLASH][0] = SCANCODE_SLASH;
    gScan_code[KEY_SEMICOLON][0] = SCANCODE_SEMICOLON;
    gScan_code[KEY_COMMA][0] = SCANCODE_COMMA;
    gScan_code[KEY_TAB][0] = SCANCODE_TAB;
    gScan_code[KEY_PERIOD][0] = SCANCODE_PERIOD;
    gScan_code[KEY_LBRACKET][0] = SCANCODE_LBRACKET;
    gScan_code[KEY_ESCAPE][0] = SCANCODE_ESCAPE;
    gScan_code[KEY_APOSTROPHE][0] = SCANCODE_APOSTROPHE;
    gScan_code[KEY_BACKSLASH][0] = SCANCODE_BACKSLASH;
    gScan_code[KEY_INSERT][0] = SCANCODE_INSERT;
    gScan_code[KEY_END][0] = SCANCODE_END;
    gScan_code[KEY_RBRACKET][0] = SCANCODE_RBRACKET;
    gScan_code[KEY_HOME][0] = SCANCODE_HOME;
    gScan_code[KEY_PAGEUP][0] = SCANCODE_PGUP;
    gScan_code[KEY_RIGHT][0] = SCANCODE_RIGHT;
    gScan_code[KEY_DELETE][0] = SCANCODE_DELETE;
    gScan_code[KEY_LEFT][0] = SCANCODE_LEFT;
    gScan_code[KEY_UP][0] = SCANCODE_UP;
    gScan_code[KEY_PAGEDOWN][0] = SCANCODE_PGDN;
    gScan_code[KEY_KP_NUMLOCK][0] = SCANCODE_NUMLOCK;
    gScan_code[KEY_DOWN][0] = SCANCODE_DOWN;
    gScan_code[KEY_KP_DIVIDE][0] = SCANCODE_DIVIDE;
    gScan_code[KEY_KP_MULTIPLY][0] = SCANCODE_MULTIPLY;
    gScan_code[KEY_KP_PLUS][0] = SCANCODE_ADD;
    gScan_code[KEY_KP_MINUS][0] = SCANCODE_SUBTRACT;
    gScan_code[KEY_KP_EQUALS][0] = 0;
    gScan_code[KEY_KP_PERIOD][0] = SCANCODE_DECIMAL;
    gScan_code[KEY_KP_1][0] = SCANCODE_NUMPAD1;
    gScan_code[KEY_KP_3][0] = SCANCODE_NUMPAD3;
    gScan_code[KEY_KP_0][0] = SCANCODE_NUMPAD0;
    gScan_code[KEY_KP_2][0] = SCANCODE_NUMPAD2;
    gScan_code[KEY_KP_5][0] = SCANCODE_NUMPAD5;
    gScan_code[KEY_KP_7][0] = SCANCODE_NUMPAD7;
    gScan_code[KEY_KP_4][0] = SCANCODE_NUMPAD4;
    gScan_code[KEY_KP_6][0] = SCANCODE_NUMPAD6;
    gScan_code[KEY_KP_9][0] = SCANCODE_NUMPAD9;
    gScan_code[KEY_F2][0] = SCANCODE_F2;
    gScan_code[KEY_KP_8][0] = SCANCODE_NUMPAD8;
    gScan_code[KEY_F1][0] = SCANCODE_F1;
    gScan_code[KEY_F4][0] = SCANCODE_F4;
    gScan_code[KEY_F6][0] = SCANCODE_F6;
    gScan_code[KEY_F3][0] = SCANCODE_F3;
    gScan_code[KEY_F5][0] = SCANCODE_F5;
    gScan_code[KEY_F8][0] = SCANCODE_F8;
    gScan_code[KEY_F10][0] = SCANCODE_F10;
    gScan_code[KEY_F7][0] = SCANCODE_F7;
    gScan_code[KEY_F9][0] = SCANCODE_F9;
    gScan_code[KEY_F12][0] = SCANCODE_F12;
    gScan_code[KEY_SCRLK][0] = SCANCODE_SCROLL;
    gScan_code[KEY_F11][0] = SCANCODE_F11;
    gScan_code[KEY_PRTSCN][0] = 0;
    gScan_code[KEY_PAUSE][0] = 0;
    gScan_code[KEY_SPACE][0] = SCANCODE_SPACE;
    gScan_code[KEY_RSHIFT][0] = SCANCODE_RSHIFT;
    gScan_code[KEY_RALT][0] = SCANCODE_RALT;
    gScan_code[KEY_RCTRL][0] = SCANCODE_RCONTROL;
    gScan_code[KEY_LSHIFT][0] = SCANCODE_LSHIFT;
    gScan_code[KEY_LALT][0] = SCANCODE_LALT;
    gScan_code[KEY_LCTRL][0] = SCANCODE_LCONTROL;

    // gPrev_keyboard_handler = dos_getvect(9);
    // dos_setvect(9, KeyboardHandler);
}

// IDA: void __cdecl KeyEnd()
void KeyEnd(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();

    // dos_setvect(9, gPrev_keyboard_handler);
}

// IDA: int __usercall KeyDown22@<EAX>(int pKey_index@<EAX>)
int KeyDown22(int pKey_index) {
    LOG_TRACE("(%d)", pKey_index);

    return KeyDown(gScan_code[pKey_index][0]) || KeyDown(gScan_code[pKey_index][1]);
}

// IDA: void __usercall PDSetKeyArray(int *pKeys@<EAX>, int pMark@<EDX>)
void PDSetKeyArray(int* pKeys, int pMark) {
    int i;
    tS32 joyX;
    tS32 joyY;
    LOG_TRACE10("(%p, %d)", pKeys, pMark);

#ifdef PLAY_NICE_WITH_GUI
    // Required in some cases like a tight loop waiting for a keypress
    gHarness_platform.ProcessWindowMessages(NULL);
#endif

    gKeys_pressed = 0;
    for (i = 0; i < COUNT_OF(gScan_code); i++) {
        if (KeyDown(gScan_code[i][0]) || KeyDown(gScan_code[i][1])) {
            gKeys_pressed = i + (gKeys_pressed << 8) + 1;
            pKeys[i] = pMark;
        } else if (pMark == pKeys[i]) {
            pKeys[i] = 0;
        }
    }
}

int PDGetASCIIFromKey(int pKey) {
    if (PDKeyDown3(KEY_SHIFT_ANY))
        return gASCII_shift_table[pKey];
    else
        return gASCII_table[pKey];
}

// IDA: void __usercall PDFatalError(char *pThe_str@<EAX>)
void PDFatalError(char* pThe_str) {
    static int been_here = 0;
    LOG_TRACE("(\"%s\")", pThe_str);

    if (been_here) {
        exit(1);
    }
    been_here = 1;
    if (gDOSGfx_initialized) {
        gDOSGfx_initialized = 0;
        BrDevEndOld();
    }
    printf("FATAL ERROR: %s\n", pThe_str);
    dr_dprintf("FATAL ERROR: %s\n", pThe_str);
#ifdef PLAY_NICE_WITH_GUI
    gHarness_platform.ShowErrorMessage(NULL, "Carmageddon Fatal Error", pThe_str);
#endif
    if (gBrZb_initialized) {
        gBrZb_initialized = 0;
        BrZbEnd();
    }
    if (gBr_initialized) {
        gBr_initialized = 0;
    }
#ifndef PLAY_NICE_WITH_GUI
    // There is no window to receive keyboard events from
    while (PDAnyKeyDown() == -1) {
    }
#endif
    QuitGame();
}

// IDA: void __usercall PDNonFatalError(char *pThe_str@<EAX>)
void PDNonFatalError(char* pThe_str) {
    LOG_TRACE("(\"%s\")", pThe_str);

    printf("ERROR: %s", pThe_str);
    while (PDAnyKeyDown() == -1) {
    }
}

// IDA: void __cdecl PDInitialiseSystem()
void PDInitialiseSystem(void) {
    tPath_name the_path;
    FILE* f;
    int len;

    KeyBegin();

    // DOSMouseBegin();
    InitJoysticks();

    // Demo's do not ship with KEYBOARD.COK file
    if (harness_game_info.defines.ascii_table == NULL) {
        PathCat(the_path, gApplication_path, "KEYBOARD.COK");
        f = fopen(the_path, "rb");
        if (f == NULL) {
            PDFatalError("This .exe must have KEYBOARD.COK in the DATA folder.");
        }

        fseek(f, 0, SEEK_END);
        len = ftell(f);
        rewind(f);
        fread(gASCII_table, len / 2, 1, f);
        fread(gASCII_shift_table, len / 2, 1, f);
        fclose(f);
#if BR_ENDIAN_BIG
        for (int i = 0; i < 128; i++) {
            gASCII_table[i] = BrSwap32(gASCII_table[i]);
            gASCII_shift_table[i] = BrSwap32(gASCII_shift_table[i]);
        }
#endif
    } else {
        memcpy(gASCII_table, harness_game_info.defines.ascii_table, sizeof(gASCII_table));
        memcpy(gASCII_shift_table, harness_game_info.defines.ascii_shift_table, sizeof(gASCII_shift_table));
    }
}

// IDA: void __cdecl PDShutdownSystem()
void PDShutdownSystem(void) {
    LOG_TRACE("()");

    // dos_setvect(9, gPrev_keyboard_handler);
    if (gDOSGfx_initialized) {
        BrDevEndOld();
    }
    // DOSMouseEnd();
    PDRevertPalette();
}

// IDA: void __cdecl PDSaveOriginalPalette()
void PDSaveOriginalPalette(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PDRevertPalette()
void PDRevertPalette(void) {
    LOG_TRACE("()");

    // empty function
}

// IDA: int __usercall PDInitScreenVars@<EAX>(int pArgc@<EAX>, char **pArgv@<EDX>)
int PDInitScreenVars(int pArgc, char** pArgv) {
    gGraf_specs[gGraf_spec_index].phys_width = gGraf_specs[gGraf_spec_index].total_width;
    gGraf_specs[gGraf_spec_index].phys_height = gGraf_specs[gGraf_spec_index].total_height;
    return 1;
}

// IDA: void __cdecl PDInitScreen()
void PDInitScreen(void) {
    LOG_TRACE("()");
}

// IDA: void __cdecl sub_B4DB4()
void sub_B4DB4(void) {
    // if (!gReal_back_screen->pixels_qualifier) {
    //     gReal_back_screen->pixels_qualifier = (unsigned __int16)__DS__;
    // }
}

// IDA: void __cdecl PDLockRealBackScreen()
// In all retail 3dfx executables, it is void __usercall PDLockRealBackScreen(lock@<EAX>)
void PDLockRealBackScreen(int lock) {
    LOG_TRACE("()");

    if (!gReal_back_screen_locked && !gReal_back_screen->pixels && lock <= gVoodoo_rush_mode) {
        sub_B4DB4();
        BrPixelmapDirectLock(gReal_back_screen, 1);
        if (!gReal_back_screen->pixels)
            FatalError(117, "gReal_back_screen");
        gReal_back_screen_locked = 1;
    }
}

// IDA: void __cdecl PDUnlockRealBackScreen()
// In all retail 3dfx executables, it is void __usercall PDUnlockRealBackScreen(lock@<EAX>)
void PDUnlockRealBackScreen(int lock) {
    LOG_TRACE("()");

    if (gReal_back_screen_locked && gReal_back_screen->pixels && lock <= gVoodoo_rush_mode) {
        BrPixelmapDirectUnlock(gReal_back_screen);
        gReal_back_screen_locked = 0;
    }
}

// IDA: void __cdecl PDAllocateScreenAndBack()
void PDAllocateScreenAndBack(void) {

    gScreen = NULL;

    // added by dethrace. We default to software mode unless we explicitly ask for 3dfx opengl mode
    if (harness_game_config.opengl_3dfx_mode) {

        if (gGraf_spec_index != 0 && !gNo_voodoo) {

#ifdef PLAY_NICE_WITH_GUI
            gl_callbacks.get_proc_address = gHarness_platform.GL_GetProcAddress;
            gl_callbacks.swap_buffers = gHarness_platform.Swap;
            gl_callbacks.get_viewport = gHarness_platform.GetViewport;
            gHarness_platform.CreateWindow_("Carmageddon", gGraf_specs[gGraf_spec_index].phys_width, gGraf_specs[gGraf_spec_index].phys_height, eWindow_type_opengl);

            BrDevBeginVar(&gScreen, "glrend",
                BRT_WIDTH_I32, gGraf_specs[gGraf_spec_index].phys_width,
                BRT_HEIGHT_I32, gGraf_specs[gGraf_spec_index].phys_height,
                BRT_OPENGL_CALLBACKS_P, &gl_callbacks,
                BRT_PIXEL_TYPE_U8, BR_PMT_RGB_565,
                BR_NULL_TOKEN);
#else
            BrDevBegin(&gScreen, "3dfx_dos,w:640,h:480,b:16");
#endif
        }
    }

    if (gScreen != NULL) {
        if ((strcmp(gScreen->identifier, "Voodoo Graphics") == 0 && !gForce_voodoo_rush_mode) || gForce_voodoo_mode) {
            dr_dprintf("Voodoo Graphics mode");
        } else {
            dr_dprintf("Voodoo Rush mode");
            gVoodoo_rush_mode = 1;
        }
        gInterpolate_textures = 1;
        gUse_mip_maps = 1;
        gTextures_need_powers_of_2 = 1;
        gMax_texture_aspect_ratio = 8;
        gMax_texture_side = 256;
        gBlitting_is_slow = 1;
        gMaterial_fogging = 1;
        gExceptions_general_file = "VOODOO";
        gExceptions_file_suffix = ".TXT";
        gSmall_frames_are_slow = 1;
        gNo_2d_effects = 1;
        gPerspective_is_fast = 1;
        gNo_transients = 1;
        gDevious_2d = 1;
        gShade_tables_do_not_work = 1;
    } else {
        gExceptions_file_suffix = ".TXT";
        gInterpolate_textures = 1;
        gExceptions_general_file = "SOFTWARE";

#ifdef PLAY_NICE_WITH_GUI
        // Render framebuffer to memory and call hooks when swapping or palette changing
        virtualfb_callbacks.palette_changed = gHarness_platform.PaletteChanged;
        virtualfb_callbacks.swap_buffers = gHarness_platform.Swap;
        gHarness_platform.CreateWindow_("Carmageddon", gGraf_specs[gGraf_spec_index].phys_width, gGraf_specs[gGraf_spec_index].phys_height, eWindow_type_software);
        BrDevBeginVar(&gScreen, "virtualframebuffer",
            BRT_WIDTH_I32, gGraf_specs[gGraf_spec_index].phys_width,
            BRT_HEIGHT_I32, gGraf_specs[gGraf_spec_index].phys_height,
            BRT_VIRTUALFB_CALLBACKS_P, &virtualfb_callbacks,
            BR_NULL_TOKEN);
#else
        gScreen = BrDevBeginOld(gGraf_specs[gGraf_spec_index].gfx_init_string);
#endif
        gDOSGfx_initialized = 1;
    }
    gScreen->origin_x = 0;
    gScreen->origin_y = 0;
    gBack_screen = BrPixelmapMatch(gScreen, BR_PMMATCH_OFFSCREEN);
    gReal_back_screen = gBack_screen;
    PDLockRealBackScreen(0);
    gBack_screen->origin_x = 0;
    gBack_screen->origin_y = 0;
    gTemp_screen = BrPixelmapAllocate(BR_PMT_INDEX_8, gScreen->width, gScreen->height, 0, 0);
    gTemp_screen->origin_x = 0;
    gTemp_screen->origin_y = 0;
}

// IDA: void __usercall Copy8BitTo16BitPixelmap(br_pixelmap *pDst@<EAX>, br_pixelmap *pSrc@<EDX>, br_pixelmap *pPalette@<EBX>)
void Copy8BitTo16BitPixelmap(br_pixelmap* pDst, br_pixelmap* pSrc, br_pixelmap* pPalette) {
    int x;
    int y;
    tU8* src;
    tU8 value;
    tU8 red;
    tU8 green;
    tU8 blue;
    tU16* dst;
    tU16* palette_entry;
    LOG_TRACE("(%p, %p, %p)", pDst, pSrc, pPalette);

    palette_entry = PaletteOf16Bits(pPalette)->pixels;
    for (y = 0; pSrc->height > y; y++) {
        src = (tU8*)pSrc->pixels + pSrc->row_bytes * y;
        dst = pDst->pixels + pDst->row_bytes * y;
        for (x = 0; x < pSrc->width; x++) {
            value = *src;
            *dst = palette_entry[value];
            src++;
            dst++;
        }
    }
}

// IDA: void __usercall Double8BitTo16BitPixelmap(br_pixelmap *pDst@<EAX>, br_pixelmap *pSrc@<EDX>, br_pixelmap *pPalette@<EBX>, tU16 pOff@<ECX>, tU16 pSrc_width, tU16 pSrc_height)
void Double8BitTo16BitPixelmap(br_pixelmap* pDst, br_pixelmap* pSrc, br_pixelmap* pPalette, tU16 pOff, tU16 pSrc_width, tU16 pSrc_height) {
    int x;
    int y;
    tU8* src;
    tU8 value;
    tU8 red;
    tU8 green;
    tU8 blue;
    tU16* dst0;
    tU16* dst1;
    tU16 sixteen;
    tU16* palette_entry;
    LOG_TRACE("(%p, %p, %p, %d, %d, %d)", pDst, pSrc, pPalette, pOff, pSrc_width, pSrc_height);

    // added by dethrace. Some local symbols seem to be missing
    int dst_y = 0;
    int line_buff_x = 0;
    static tU16 line_buff[640];

    palette_entry = PaletteOf16Bits(pPalette)->pixels;
    if (pSrc_width > 640) {
        FatalError(94, "Double8BitTo16BitPixelmap");
    }
    dst_y = 0;
    for (y = 0; y < pSrc_height; y++) {
        src = (tU8*)pSrc->pixels + pSrc->row_bytes * y;
        dst0 = (tU16*)((tU8*)pDst->pixels + pDst->row_bytes * (dst_y + pOff));
        dst1 = (tU16*)((tU8*)pDst->pixels + pDst->row_bytes * (dst_y + pOff + 1));
        line_buff_x = 0;

        for (x = 0; x < pSrc_width; x++) {
            sixteen = palette_entry[*src];
            line_buff[line_buff_x] = sixteen;
            line_buff[line_buff_x + 1] = sixteen;
            src++;
            line_buff_x += 2;
        }

        // copy 2 full lines into destination
        memcpy(dst0, line_buff, pSrc_width * 2 * sizeof(tU16));
        memcpy(dst1, line_buff, pSrc_width * 2 * sizeof(tU16));
        dst_y += 2;
    }
}

// IDA: br_pixelmap* __cdecl PDInterfacePixelmap()
br_pixelmap* PDInterfacePixelmap(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SwapBackScreen()
void SwapBackScreen(void) {
    LOG_TRACE("()");

    PDUnlockRealBackScreen(1);
    BrPixelmapDoubleBuffer(gScreen, gReal_back_screen);
    PDLockRealBackScreen(1);
}

// IDA: void __usercall ReallyCopyBackScreen(int pRendering_area_only@<EAX>, int pClear_top_and_bottom@<EDX>)
void ReallyCopyBackScreen(int pRendering_area_only, int pClear_top_and_bottom) {
    LOG_TRACE("(%d, %d)", pRendering_area_only, pClear_top_and_bottom);

    gAlready_copied = 1;
    if (pRendering_area_only) {
        BrPixelmapRectangleCopy(gScreen, gX_offset, gY_offset, gRender_screen, 0, 0, gWidth, gHeight);
    } else if (gReal_graf_data_index != gGraf_data_index) {
        BrPixelmapRectangleFill(gReal_back_screen, 0, 0, 640, 40, 0);
        BrPixelmapRectangleFill(gReal_back_screen, 0, 440, 640, 40, 0);
        if (gReal_back_screen->type == BR_PMT_RGB_565) {
            Double8BitTo16BitPixelmap(gReal_back_screen, gBack_screen, gCurrent_palette, 40, 320, 200);
        } else {
            DRPixelmapDoubledCopy(gReal_back_screen, gBack_screen, 320, 200, 0, 40);
        }
    }
}

// IDA: void __usercall CopyBackScreen(int pRendering_area_only@<EAX>)
void CopyBackScreen(int pRendering_area_only) {
    LOG_TRACE("(%d)", pRendering_area_only);

    ReallyCopyBackScreen(pRendering_area_only, 1);
}

// IDA: void __usercall PDScreenBufferSwap(int pRendering_area_only@<EAX>)
void PDScreenBufferSwap(int pRendering_area_only) {
    LOG_TRACE10("(%d)", pRendering_area_only);

    if (gSwitched_resolution) {
        BrPixelmapFill(gTemp_screen, 0);
    }
    if (!gAlready_copied) {
        ReallyCopyBackScreen(pRendering_area_only, 0);
    }
    gAlready_copied = 0;
    if (!pRendering_area_only) {
        SwapBackScreen();
    }
}

// IDA: void __usercall PDPixelmapToScreenRectangleCopy(br_pixelmap *dst@<EAX>, br_int_16 dx@<EDX>, br_int_16 dy@<EBX>, br_pixelmap *src@<ECX>, br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h)
void PDPixelmapToScreenRectangleCopy(br_pixelmap* dst, br_int_16 dx, br_int_16 dy, br_pixelmap* src, br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h) {
    LOG_TRACE("(%p, %d, %d, %p, %d, %d, %d, %d)", dst, dx, dy, src, sx, sy, w, h);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDPixelmapHLineOnScreen(br_pixelmap *dst@<EAX>, br_int_16 x1@<EDX>, br_int_16 y1@<EBX>, br_int_16 x2@<ECX>, br_int_16 y2, br_uint_32 colour)
void PDPixelmapHLineOnScreen(br_pixelmap* dst, br_int_16 x1, br_int_16 y1, br_int_16 x2, br_int_16 y2, br_uint_32 colour) {
    LOG_TRACE("(%p, %d, %d, %d, %d, %d)", dst, x1, y1, x2, y2, colour);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDPixelmapVLineOnScreen(br_pixelmap *dst@<EAX>, br_int_16 x1@<EDX>, br_int_16 y1@<EBX>, br_int_16 x2@<ECX>, br_int_16 y2, br_uint_32 colour)
void PDPixelmapVLineOnScreen(br_pixelmap* dst, br_int_16 x1, br_int_16 y1, br_int_16 x2, br_int_16 y2, br_uint_32 colour) {
    LOG_TRACE("(%p, %d, %d, %d, %d, %d)", dst, x1, y1, x2, y2, colour);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PDInstallErrorHandlers()
void PDInstallErrorHandlers(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PDSetFileVariables()
void PDSetFileVariables(void) {

    // gDir_separator[0] = '\\';
    gDir_separator[0] = '/';
}

// IDA: void __usercall PDBuildAppPath(char *pThe_path@<EAX>)
void PDBuildAppPath(char* pThe_path) {
    int pos;

    getcwd(pThe_path, 256);
    // strcat(pThe_path, "\\");
    strcat(pThe_path, "/");
    strcpy(gNetwork_profile_fname, pThe_path);
    strcat(gNetwork_profile_fname, "NETWORK.INI");
}

// IDA: void __usercall PDForEveryFile(char *pThe_path@<EAX>, void (*pAction_routine)(char*)@<EDX>)
void PDForEveryFile(char* pThe_path, void (*pAction_routine)(char*)) {
    char find_path[256];
    char found_path[256];

    char* found = OS_GetFirstFileInDirectory(pThe_path);
    while (found != NULL) {
        PathCat(found_path, pThe_path, found);
        pAction_routine(found_path);
        found = OS_GetNextFileInDirectory();
    }
}

// IDA: void __usercall PDSetPalette(br_pixelmap *pThe_palette@<EAX>)
void PDSetPalette(br_pixelmap* pThe_palette) {
    BrDevPaletteSetOld(pThe_palette);
}

// IDA: void __usercall PDSetPaletteEntries(br_pixelmap *pPalette@<EAX>, int pFirst_colour@<EDX>, int pCount@<EBX>)
void PDSetPaletteEntries(br_pixelmap* pPalette, int pFirst_colour, int pCount) {
    int i;
    tU8* p;

    p = pPalette->pixels;
    p += pFirst_colour * 4;
    for (i = pFirst_colour; i < pFirst_colour + pCount; i++) {
#if BR_ENDIAN_BIG
        BrDevPaletteSetEntryOld(i, (p[1] << 16) | (p[2] << 8) | p[3]);
#else
        BrDevPaletteSetEntryOld(i, (p[2] << 16) | (p[1] << 8) | *p);
#endif
        p += 4;
    }
}

// IDA: void __cdecl PDSwitchToRealResolution()
void PDSwitchToRealResolution(void) {
    LOG_TRACE("()");

    gBack_screen = gReal_back_screen;
    gSwitched_resolution = 1;
}

// IDA: void __cdecl PDSwitchToLoresMode()
void PDSwitchToLoresMode(void) {
    LOG_TRACE("()");

    gBack_screen = gTemp_screen;
    gSwitched_resolution = 0;
}

// IDA: void __usercall PDMouseButtons(int *pButton_1@<EAX>, int *pButton_2@<EDX>)
void PDMouseButtons(int* pButton_1, int* pButton_2) {
    br_uint_32 mouse_buttons;
    br_int_32 mouse_x;
    br_int_32 mouse_y;
    LOG_TRACE("(%p, %p)", pButton_1, pButton_2);

    // DOSMouseRead(...)
    gHarness_platform.GetMouseButtons(pButton_1, pButton_2);
}

// IDA: void __usercall PDGetMousePosition(int *pX_coord@<EAX>, int *pY_coord@<EDX>)
void PDGetMousePosition(int* pX_coord, int* pY_coord) {
    br_uint_32 mouse_buttons;
    br_int_32 mouse_x2;
    br_int_32 mouse_y2;
    int delta_x;
    int delta_y;
    static br_int_32 mouse_x;
    static br_int_32 mouse_y;
    LOG_TRACE("(%p, %p)", pX_coord, pY_coord);

    // DOSMouseRead(...)
    gHarness_platform.GetMousePosition(pX_coord, pY_coord);
}

// IDA: int __cdecl PDGetTotalTime()
int PDGetTotalTime(void) {
    return gHarness_platform.GetTicks();
}

// IDA: int __usercall PDServiceSystem@<EAX>(tU32 pTime_since_last_call@<EAX>)
int PDServiceSystem(tU32 pTime_since_last_call) {

#ifdef PLAY_NICE_WITH_GUI
    // Added by dethrace. Win95 code does the same
    gHarness_platform.ProcessWindowMessages(NULL);
#endif
    return 0;
}

// IDA: tU32 __cdecl LargestBlockAvail()
tU32 LargestBlockAvail(void) {
    // SREGS sregs;
    tMem_info mem_info;
    size_t memmax;

    // Added >>
    return 15000000;
    // <<
}

// IDA: void* __usercall PDGrabLargestMammaryWeCanPlayWith@<EAX>(tU32 pMaximum_required@<EAX>, tU32 *pAmount_allocated@<EDX>)
void* PDGrabLargestMammaryWeCanPlayWith(tU32 pMaximum_required, tU32* pAmount_allocated) {
    void* result;
    LOG_TRACE("(%d, %p)", pMaximum_required, pAmount_allocated);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDAllocateActionReplayBuffer(char **pBuffer@<EAX>, tU32 *pBuffer_size@<EDX>)
void PDAllocateActionReplayBuffer(char** pBuffer, tU32* pBuffer_size) {
    tU32 lba;
    tU32 required;
    LOG_TRACE("(%p, %p)", pBuffer, pBuffer_size);

    lba = LargestBlockAvail();
    if (gReplay_override) {
        *pBuffer = 0;
        *pBuffer_size = 0;
    } else {
        if (gGraf_spec_index) {
            required = 600000;
        } else {
            required = 300000;
        }
        required += gExtra_mem;
        if (lba >= required + 65536) {
            required = lba - required;
        } else {
            required = 65536;
        }
        dr_dprintf("Allocated %u bytes to the action replay buffer for %s-res", required, gGraf_spec_index != 0 ? "high" : "low");
        *pBuffer = malloc(required);
        *pBuffer_size = required;
    }
}

// IDA: void __usercall PDDisposeActionReplayBuffer(char *pBuffer@<EAX>)
void PDDisposeActionReplayBuffer(char* pBuffer) {
    LOG_TRACE("(\"%s\")", pBuffer);

    free(pBuffer);
}

// IDA: void __usercall Usage(char *pProgpath@<EAX>)
void Usage(char* pProgpath) {
    // char basename[9];
    char* basename; // changed to support longer names

    basename = OS_Basename(pProgpath);

#ifdef DETHRACE_3DFX_PATCH
    fprintf(stderr, "Usage: %s options\n", basename);
    fprintf(stderr, "E.G. %s %s 0.5 %s 0 %s 2 %s\n", basename, "-yon", "-simple", "-sound", "-spamfritter");
    fprintf(stderr, "Valid options are:\n");
    fprintf(stderr, "%s: force 640x480\n", "-hires");
    fprintf(stderr, "%s number: yon factor (between 0 and 1)\n", "-yon");
    fprintf(stderr, "%s number: car simplification level (integer between 0 and %d)\n", "-simple", 4);
    fprintf(stderr, "%s number: sound detail level (integer between 0 and 2)\n", "-sound");
    fprintf(stderr, "%s\n", "-robots");
    fprintf(stderr, "%s: force low memory mode\n", "-lomem");
    fprintf(stderr, "%s\n", "-nosound");
    fprintf(stderr, "%s: optimal spam frittering\n", "-spamfritter");
    fprintf(stderr, "%s\n", "-nocutscenes");
    fprintf(stderr, "%s\n", "-noreplay");
    fprintf(stderr, "%s\n", "-novoodoo");
    fprintf(stderr, "%s: force Voodoo Graphics mode\n", "-vgraphics");
    fprintf(stderr, "%s: force Voodoo Rush (or Voodoo 2) mode\n", "-vrush");
#else
    fprintf(stderr,
        "Usage: %s [%s] [%s YonFactor] [%s CarSimplificationLevel] [%s SoundDetailLevel] [%s] [%s] [%s] [%s] [%s] [%s]\nWhere YonFactor is between 0 and 1,\nCarSimplificationLevel is a whole number between 0 and %d,\nand SoundDetailLevel is a whole number.\n",
        basename,
        "-hires",
        "-yon",
        "-simple",
        "-sound",
        "-robots",
        "-lomem",
        "-nosound",
        "-spamfritter",
        "-nocutscenes",
        "-noreplay",
        CAR_MAX_SIMPLIFICATION_LEVEL);
#endif
    exit(1);
}

// Renamed from "main" to "original_main" to allow for harness + unit testing
int original_main(int pArgc, char** pArgv) {
    int arg;
    int i;
    float f;

    for (i = 1; i < pArgc; i++) {
        if (strcasecmp(pArgv[i], "-hires") == 0) {
            gGraf_spec_index = 1;
        } else if (strcasecmp(pArgv[i], "-yon") == 0 && i < pArgc - 1) {
            i++;
            sscanf(pArgv[i], "%f", &f);
            if (f >= 0.0 && f <= 1.0f) {
                gYon_multiplier = f;
            }
        } else if (strcasecmp(pArgv[i], "-simple") == 0 && i < pArgc - 1) {
            i++;
            sscanf(pArgv[i], "%d", &arg);
            if (arg >= 0 && arg < 5) {
                gCar_simplification_level = arg;
            }
        } else if (strcasecmp(pArgv[i], "-sound") == 0 && i < pArgc - 1) {
            i++;
            sscanf(pArgv[i], "%d", &arg);
            gSound_detail_level = arg;

        } else if (strcasecmp(pArgv[i], "-robots") == 0 || strcasecmp(pArgv[i], "-german") == 0) {
            gSausage_override = 1;
        } else if (strcasecmp(pArgv[i], "-lomem") == 0) {
            gAustere_override = 1;
        } else if (strcasecmp(pArgv[i], "-nosound") == 0) {
            gSound_override = 1;
        } else if (strcasecmp(pArgv[i], "-spamfritter") == 0) {
            gExtra_mem = 2000000;
        } else if (strcasecmp(pArgv[i], "-nocutscenes") == 0) {
            gCut_scene_override = 1;
        } else if (strcasecmp(pArgv[i], "-noreplay") == 0) {
            gReplay_override = 1;
        } else if (strcasecmp(pArgv[i], "-novoodoo") == 0) {
            gNo_voodoo = 1;
        } else if (strcasecmp(pArgv[i], "-vrush") == 0) {
            gForce_voodoo_mode = 0;
            gForce_voodoo_rush_mode = 1;
        } else if (strcasecmp(pArgv[i], "-vgraphics") == 0) {
            gForce_voodoo_rush_mode = 0;
            gForce_voodoo_mode = 1;
        } else {
            Usage(pArgv[0]);
        }
    }

    if (!gNo_voodoo) {
        gGraf_spec_index = 1;
    }

    GameMain(pArgc, pArgv);
    return 0;
}

// IDA: int __cdecl OurGetChar()
int OurGetChar(void) {
    int key;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDEnterDebugger(char *pStr@<EAX>)
void PDEnterDebugger(char* pStr) {
    static unsigned char* save_it;
    LOG_TRACE("(\"%s\")", pStr);

    save_it = (unsigned char*)pStr;
}

// IDA: void __cdecl PDEndItAllAndReRunTheBastard()
void PDEndItAllAndReRunTheBastard(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall LoopLimitTooLow@<EAX>(tU32 limit@<EAX>)
int LoopLimitTooLow(tU32 limit) {
    clock_t start;
    tU32 count;
    tU32 val;
    LOG_TRACE("(%d)", limit);

    // v2 = j___clock(limit);
    // v3 = v2;
    // for (count = 0; count < limit; gRaw_joystick1x += v2) {
    //     v5 = __inbyte(0x201u);
    //     v2 = v5 & 1;
    //     ++count;
    // }
    // return j___clock(v2) < (unsigned int)(v3 + 3);

    return 0;
}

// IDA: tS32 __cdecl UpperLoopLimit()
tS32 UpperLoopLimit(void) {
    tU32 limit;
    LOG_TRACE("()");

    for (limit = 1024; 2 * limit && LoopLimitTooLow(limit); limit *= 2)
        ;
    while (2 * limit && LoopLimitTooLow(limit))
        limit *= 2;
    while (2 * limit && LoopLimitTooLow(limit))
        limit *= 2;
    return limit - 1;
}

// IDA: int __cdecl InitJoysticks()
int InitJoysticks(void) {
    LOG_TRACE("()");

    gJoystick_deadzone = 8000;
    gUpper_loop_limit = UpperLoopLimit() / 2;
    return 0;
}

// IDA: tU32 __usercall ReadJoystickAxis@<EAX>(int pBit@<EAX>)
tU32 ReadJoystickAxis(int pBit) {
    tU32 val;
    tU32 count;
    LOG_TRACE("(%d)", pBit);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PDReadJoySticks()
void PDReadJoySticks(void) {
    tU32 temp1x;
    tU32 temp1y;
    tU32 temp2x;
    tU32 temp2y;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tS32 __cdecl PDGetJoy1X()
tS32 PDGetJoy1X(void) {
    tS32 joy;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tS32 __cdecl PDGetJoy1Y()
tS32 PDGetJoy1Y(void) {
    tS32 joy;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tS32 __cdecl PDGetJoy2X()
tS32 PDGetJoy2X(void) {
    tS32 joy;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tS32 __cdecl PDGetJoy2Y()
tS32 PDGetJoy2Y(void) {
    tS32 joy;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetJoy1Button1()
int PDGetJoy1Button1(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetJoy1Button2()
int PDGetJoy1Button2(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetJoy1Button3()
int PDGetJoy1Button3(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetJoy1Button4()
int PDGetJoy1Button4(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetJoy2Button1()
int PDGetJoy2Button1(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetJoy2Button2()
int PDGetJoy2Button2(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetJoy2Button3()
int PDGetJoy2Button3(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetJoy2Button4()
int PDGetJoy2Button4(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PDFileUnlock@<EAX>(char *pThe_path@<EAX>)
int PDFileUnlock(char* pThe_path) {
    unsigned int attr;
    LOG_TRACE("(\"%s\")", pThe_path);
    // _dos_setfileattr_(pThe_path, 0);
    return 0;
}

// IDA: void __cdecl CriticalISR(INTPACK pRegs)
typedef void* INTPACK;
void CriticalISR(INTPACK pRegs) {
    LOG_TRACE("(%d)", pRegs);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PDCheckDriveExists2@<EAX>(char *pThe_path@<EAX>, char *pFile_name@<EDX>, tU32 pMin_size@<EBX>)
int PDCheckDriveExists2(char* pThe_path, char* pFile_name, tU32 pMin_size) {
    struct stat buf;
    void (*old_critical_isr)(void);
    int stat_failed;
    char slasher[4];
    char the_path[256];
    LOG_TRACE("(\"%s\", \"%s\", %d)", pThe_path, pFile_name, pMin_size);

    strcpy(slasher, "?:\\");
    if (pFile_name) {
        PathCat(the_path, pThe_path, pFile_name);
    } else {
        strcpy(the_path, pThe_path);
    }

    // Jeff: force unix dir separator >>
    char* rep = the_path;
    while ((rep = strchr(rep, '\\')) != NULL) {
        *rep++ = '/';
    }
    // <<

    stat_failed = stat(the_path, &buf);
    return stat_failed == 0 && buf.st_size >= pMin_size;
}

// IDA: int __cdecl PDDoWeLeadAnAustereExistance()
int PDDoWeLeadAnAustereExistance(void) {
    tU32 block;

    block = LargestBlockAvail();

    dr_dprintf("PDDoWeLeadAnAustereExistance (sic): LargestBlockAvail=%d\n", block);

    if (gReal_graf_data_index == 0) {
        return block < 13000000;
    }
    return block < 15000000;
}

int CheckGorePasswordFile(char* pPassword) {
    tPath_name path;
    FILE* f;
    char line[10];

    PathCat(path, "DATA", "PASS.TXT");
    f = fopen(path, "rb");
    if (f == NULL) {
        return 0;
    }
    if (fgets(line, sizeof(line), f) == NULL) {
        fclose(f);
        return 0;
    }
    fclose(f);
#if defined(DETHRACE_FIX_BUGS)
    // Allow a final newline in DATA/PASS.TXT
    while (strlen(line) > 0 && (line[strlen(line) - 1] == '\n' || line[strlen(line) - 1] == '\r')) {
        line[strlen(line) - 1] = '\0';
    }
#endif
#ifdef _WIN32
    return stricmp(line, pPassword) == 0;
#else
    return strcasecmp(line, pPassword) == 0;
#endif
}

// IDA: int __cdecl PDGetGorePassword()
int PDGetGorePassword(void) {
    int ch;
    int len;
    int chances;
    char password[17];
    LOG_TRACE("()");

    for (chances = 0; chances < 3; chances++) {
        printf(chances == 0 ? "\n\n\n\n\nEnter password for uncut version...\n" : "\nIncorrect password, please try again...\n");
        OS_ConsoleReadPassword(password, sizeof(password));
        dr_dprintf("Password entered is '%s'", password);
        if (CheckGorePasswordFile(password)) {
            return 1;
        }
    }
    return 0;
}

// IDA: void __usercall PDDisplayGoreworthiness(int pGory@<EAX>)
void PDDisplayGoreworthiness(int pGory) {
    tU32 delay_start;
    LOG_TRACE("(%d)", pGory);

    printf(pGory ? "\nPlaying full version...\n" : "\nPlaying zombie version...\n");
    delay_start = 2;
    gHarness_platform.Sleep(delay_start * 1000);
}
