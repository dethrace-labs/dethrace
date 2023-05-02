#include "brender/brender.h"
#include "car.h"
#include "dinput.h"
#include "errors.h"
#include "globvars.h"
#include "grafdata.h"
#include "graphics.h"
#include "harness/config.h"
#include "harness/hooks.h"
#include "harness/os.h"
#include "harness/trace.h"
#include "input.h"
#include "loadsave.h"
#include "main.h"
#include "pd/sys.h"
#include "sound.h"
#include "ssdx.h"
#include "utility.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "harness/win95_polyfill.h"

#define GFX_INIT_STRING_32X20X8 "320x200 init string"
#define GFX_INIT_STRING_64X48X8 "640x480 init string"

#define KEYDOWN(var, key) (var[key] & 0x80)

int gExtra_mem;
int gReplay_override;
tGraf_spec gGraf_specs[2] = {
    { 8, 1, 0, 320, 200, 0, 0, "32X20X8", GFX_INIT_STRING_32X20X8, 320, 320, 200, NULL },
    { 8, 1, 0, 640, 480, 0, 0, "64X48X8", GFX_INIT_STRING_64X48X8, 640, 640, 480, NULL }
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
int gGfx_initialized; // maybe renamed here
tU32 gUpper_loop_limit;
int gReal_back_screen_locked;
tU32 gScan_code[123]; // was tU8 [123][2] in symbol dump

// Added by dethrace. Windows-specific. Original variable names unknown.
int gWin32_fatal_error_exit_code;
int gWin32_show_fatal_error_message;
char gWin32_fatal_error_message[512];
int gWin32_action_replay_buffer_allocated;
void* gWin32_action_replay_buffer;
int gWin32_action_replay_buffer_size;
void* gWin32_hwnd;
int gWin32_lbutton_down;
int gWin32_rbutton_down;
PALETTEENTRY_ gWin32_palette[256];
br_diaghandler gWin32_br_diaghandler;
int gWin32_window_has_focus = 1;
int gNetwork_profile_file_exists = 0;

char* _unittest_last_fatal_error;

int original_main(int pArgc, char** pArgv);

/*static*/ void KeyboardHandler(void);
/*static*/ int KeyDown(tU8 pScan_code);
/*static*/ void KeyTranslation(tU8 pKey_index, tU8 pScan_code_1, tU8 pScan_code_2);
/*static*/ void KeyBegin(void);
/*static*/ void KeyEnd(void);
/*static*/ int KeyDown22(int pKey_index);
static void Win32ReleaseInputDevice(void);
static void Win32PumpMessages(void);
static HARNESS_NORETURN void Win32FatalError(char* pStr_1, char* pStr_2);
static void Win32CreateWindow(void);
static void Win32InitScreen(void);
/*static*/ void Copy8BitTo16BitPixelmap(br_pixelmap* pDst, br_pixelmap* pSrc, br_pixelmap* pPalette);
/*static*/ void SwapBackScreen(void);
/*static*/ void ReallyCopyBackScreen(int pRendering_area_only, int pClear_top_and_bottom);
/*static*/ void CopyBackScreen(int pRendering_area_only);
static void Win32SetPaletteEntries(uint8_t* entries, int pFirst_colour, int pCount);
static void Win32InitInputDevice(void);
static void Win32AllocateActionReplayBuffer(void);
static void Usage(char* pProgpath);
/*static*/ int OurGetChar(void);
/*static*/ int InitJoysticks(void);
/*static*/ tU32 ReadJoystickAxis(int pBit);
static void Win32BRenderWarningFunc(char* msg);
static void Win32BRenderFailureFunc(char* msg);

extern void QuitGame(void);

void KeyboardHandler(void) {
    tU8 scan_code;
    tU8 up;
    static tU8 extended;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

int KeyDown(tU8 pScan_code) {
    NOT_IMPLEMENTED();
}

void KeyTranslation(tU8 pKey_index, tU8 pScan_code_1, tU8 pScan_code_2) {
    LOG_TRACE("(%d, %d, %d)", pKey_index, pScan_code_1, pScan_code_2);
    NOT_IMPLEMENTED();
}

void KeyBegin(void) {
    gScan_code[KEY_0] = DIK_0;
    gScan_code[KEY_1] = DIK_1;
    gScan_code[KEY_2] = DIK_2;
    gScan_code[KEY_3] = DIK_3;
    gScan_code[KEY_4] = DIK_4;
    gScan_code[KEY_5] = DIK_5;
    gScan_code[KEY_6] = DIK_6;
    gScan_code[KEY_7] = DIK_7;
    gScan_code[KEY_8] = DIK_8;
    gScan_code[KEY_9] = DIK_9;
    gScan_code[KEY_A] = DIK_A;
    gScan_code[KEY_B] = DIK_B;
    gScan_code[KEY_C] = DIK_C;
    gScan_code[KEY_D] = DIK_D;
    gScan_code[KEY_E] = DIK_E;
    gScan_code[KEY_F] = DIK_F;
    gScan_code[KEY_G] = DIK_G;
    gScan_code[KEY_H] = DIK_H;
    gScan_code[KEY_I] = DIK_I;
    gScan_code[KEY_J] = DIK_J;
    gScan_code[KEY_K] = DIK_K;
    gScan_code[KEY_L] = DIK_L;
    gScan_code[KEY_M] = DIK_M;
    gScan_code[KEY_N] = DIK_N;
    gScan_code[KEY_O] = DIK_O;
    gScan_code[KEY_P] = DIK_P;
    gScan_code[KEY_Q] = DIK_Q;
    gScan_code[KEY_R] = DIK_R;
    gScan_code[KEY_S] = DIK_S;
    gScan_code[KEY_T] = DIK_T;
    gScan_code[KEY_U] = DIK_U;
    gScan_code[KEY_V] = DIK_V;
    gScan_code[KEY_W] = DIK_W;
    gScan_code[KEY_X] = DIK_X;
    gScan_code[KEY_Y] = DIK_Y;
    gScan_code[KEY_Z] = DIK_Z;
    gScan_code[KEY_GRAVE] = DIK_GRAVE;
    gScan_code[KEY_MINUS] = DIK_MINUS;
    gScan_code[KEY_EQUALS] = DIK_EQUALS;
    gScan_code[KEY_BACKSPACE] = DIK_BACK;
    gScan_code[KEY_RETURN] = DIK_RETURN;
    gScan_code[KEY_KP_ENTER] = DIK_NUMPADENTER;
    gScan_code[KEY_CAPSLOCK] = DIK_CAPSLOCK;
    gScan_code[KEY_TAB] = DIK_TAB;
    gScan_code[KEY_SLASH] = DIK_SLASH;
    gScan_code[KEY_UNKNOWN_55] = 0;
    gScan_code[KEY_SEMICOLON] = DIK_SEMICOLON;
    gScan_code[KEY_APOSTROPHE] = DIK_APOSTROPHE;
    gScan_code[KEY_PERIOD] = DIK_PERIOD;
    gScan_code[KEY_COMMA] = DIK_COMMA;
    gScan_code[KEY_LBRACKET] = DIK_LBRACKET;
    gScan_code[KEY_RBRACKET] = DIK_RBRACKET;
    gScan_code[KEY_ESCAPE] = DIK_ESCAPE;
    gScan_code[KEY_INSERT] = DIK_INSERT;
    gScan_code[KEY_DELETE] = DIK_DELETE;
    gScan_code[KEY_HOME] = DIK_HOME;
    gScan_code[KEY_END] = DIK_END;
    gScan_code[KEY_PAGEUP] = DIK_PGUP;
    gScan_code[KEY_PAGEDOWN] = DIK_PGDN;
    gScan_code[KEY_LEFT] = DIK_LEFT;
    gScan_code[KEY_RIGHT] = DIK_RIGHT;
    gScan_code[KEY_UP] = DIK_UP;
    gScan_code[KEY_DOWN] = DIK_DOWN;
    gScan_code[KEY_KP_NUMLOCK] = DIK_NUMLOCK;
    gScan_code[KEY_KP_DIVIDE] = DIK_DIVIDE;
    gScan_code[KEY_KP_MULTIPLY] = DIK_MULTIPLY;
    gScan_code[KEY_KP_MINUS] = DIK_SUBTRACT;
    gScan_code[KEY_KP_PLUS] = DIK_ADD;
    gScan_code[KEY_KP_PERIOD] = DIK_DECIMAL;
    gScan_code[KEY_KP_0] = DIK_NUMPAD0;
    gScan_code[KEY_KP_1] = DIK_NUMPAD1;
    gScan_code[KEY_KP_2] = DIK_NUMPAD2;
    gScan_code[KEY_KP_3] = DIK_NUMPAD3;
    gScan_code[KEY_KP_4] = DIK_NUMPAD4;
    gScan_code[KEY_KP_5] = DIK_NUMPAD5;
    gScan_code[KEY_KP_6] = DIK_NUMPAD6;
    gScan_code[KEY_KP_7] = DIK_NUMPAD7;
    gScan_code[KEY_KP_8] = DIK_NUMPAD8;
    gScan_code[KEY_KP_9] = DIK_NUMPAD9;
    gScan_code[KEY_F1] = DIK_F1;
    gScan_code[KEY_F2] = DIK_F2;
    gScan_code[KEY_F3] = DIK_F3;
    gScan_code[KEY_F4] = DIK_F4;
    gScan_code[KEY_F5] = DIK_F5;
    gScan_code[KEY_F6] = DIK_F6;
    gScan_code[KEY_F7] = DIK_F7;
    gScan_code[KEY_F8] = DIK_F8;
    gScan_code[KEY_F9] = DIK_F9;
    gScan_code[KEY_F10] = DIK_F10;
    gScan_code[KEY_F11] = DIK_F11;
    gScan_code[KEY_F12] = DIK_F12;
    gScan_code[KEY_PRTSCN] = DIK_SYSRQ;
    gScan_code[KEY_SCRLK] = DIK_SCROLL;
    gScan_code[KEY_SPACE] = DIK_SPACE;
    gScan_code[KEY_RSHIFT] = DIK_RSHIFT;
    gScan_code[KEY_RALT] = DIK_RALT;
    gScan_code[KEY_RCTRL] = DIK_RCONTROL;
    gScan_code[KEY_LSHIFT] = DIK_LSHIFT;
    gScan_code[KEY_LALT] = DIK_LALT;
    gScan_code[KEY_LCTRL] = DIK_LCONTROL;
    gScan_code[KEY_KP_EQUALS] = 0;
    gScan_code[KEY_BACKSLASH] = DIK_BACKSLASH;
    gScan_code[KEY_CTRL_ANY_2] = 0xff;
    gScan_code[KEY_PAUSE] = 0xff;
    gScan_code[KEY_SHIFT_ANY] = 0xff;
    gScan_code[KEY_ALT_ANY] = 0xff;
    gScan_code[KEY_CTRL_ANY] = 0xff;
}

void KeyEnd(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

int KeyDown22(int pKey_index) {
    LOG_TRACE("(%d)", pKey_index);
    NOT_IMPLEMENTED();
}

void PDSetKeyArray(int* pKeys, int pMark) {
    int i;
    uint8_t keystate[256];
    LOG_TRACE10("(%p, %d)", pKeys, pMark);

    gKeys_pressed = 0;
    Win32PumpMessages();
    DirectInputDevice_GetDeviceState(COUNT_OF(keystate), keystate);

    for (i = 0; i < COUNT_OF(gScan_code); i++) {
        if (gScan_code[i] == 0) {
            continue;
        }
        if (gScan_code[i] == 0xff) {
            switch (i) {
            case KEY_SHIFT_ANY:
                if (KEYDOWN(keystate, gScan_code[KEY_LSHIFT]) || KEYDOWN(keystate, gScan_code[KEY_RSHIFT])) {
                    pKeys[KEY_SHIFT_ANY] = pMark;
                } else {
                    if (pKeys[KEY_SHIFT_ANY] == pMark) {
                        pKeys[KEY_SHIFT_ANY] = 0;
                    }
                }
                break;
            case KEY_ALT_ANY:
                if (KEYDOWN(keystate, gScan_code[KEY_LALT]) || KEYDOWN(keystate, gScan_code[KEY_RALT])) {
                    pKeys[KEY_ALT_ANY] = pMark;

                } else {
                    if (pKeys[KEY_ALT_ANY] == pMark) {
                        pKeys[KEY_ALT_ANY] = 0;
                    }
                }
                break;
            case KEY_CTRL_ANY:
            case KEY_CTRL_ANY_2:
                if (KEYDOWN(keystate, gScan_code[KEY_LCTRL]) || KEYDOWN(keystate, gScan_code[KEY_RCTRL])) {
                    pKeys[KEY_CTRL_ANY] = pMark;
                    pKeys[KEY_CTRL_ANY_2] = pMark;
                } else {
                    if (pKeys[KEY_CTRL_ANY] == pMark || pKeys[KEY_CTRL_ANY_2] == pMark) {
                        pKeys[KEY_CTRL_ANY] = 0;
                        pKeys[KEY_CTRL_ANY_2] = 0;
                    }
                }
                break;
            default:
                continue;
            }
        } else if (KEYDOWN(keystate, gScan_code[i])) {
            pKeys[i] = pMark;
            gKeys_pressed = i + (gKeys_pressed << 8) + 1;
        } else {
            if (pKeys[i] == pMark) {
                pKeys[i] = 0;
            }
        }
    }
}

void Win32ReleaseInputDevice(void) {
}

int PDGetASCIIFromKey(int pKey) {
    LOG_TRACE("(%d)", pKey);

    if (PDKeyDown(KEY_SHIFT_ANY)) {
        return gASCII_shift_table[pKey];
    } else {
        return gASCII_table[pKey];
    }
}

void Win32PumpMessages(void) {
    MSG_ msg; // [esp+Ch] [ebp-20h] BYREF

    PDUnlockRealBackScreen();
    while ((!gWin32_window_has_focus || PeekMessageA_(&msg, 0, 0, 0, 1u)) && (gWin32_window_has_focus || GetMessageA_(&msg, 0, 0, 0) != -1)) {
        if (msg.message == WM_QUIT) {
            dr_dprintf("WM_QUIT received.");
            if (gWin32_window_has_focus) {
                dr_dprintf("Active, so lock the surface");
                PDLockRealBackScreen();
                dr_dprintf("QuitGame being called...");
                QuitGame();
            }
            PDShutdownSystem();
        }
        TranslateMessage_(&msg);
        DispatchMessageA_(&msg);
    }
    PDLockRealBackScreen();
}

void PDFatalError(char* pThe_str) {
    LOG_TRACE("(\"%s\")", pThe_str);

    dr_dprintf("FATAL ERROR: %s", pThe_str);
    Win32FatalError(pThe_str, "");
}

void Win32FatalError(char* pStr_1, char* pStr_2) {
    gWin32_show_fatal_error_message = 1;
    sprintf(gWin32_fatal_error_message, "%s\n%s", pStr_1, pStr_2);
    gWin32_fatal_error_exit_code = 15;
    PDShutdownSystem();
}

void PDNonFatalError(char* pThe_str) {
    LOG_TRACE("(\"%s\")", pThe_str);
    NOT_IMPLEMENTED();
}

void PDInitialiseSystem(void) {
    tPath_name the_path;
    FILE* f;
    int len;

    Win32AllocateActionReplayBuffer();
    gBack_screen = NULL;
    gScreen = NULL;
    Win32CreateWindow();
    ShowCursor_(0);
    KeyBegin();

    PathCat(the_path, gApplication_path, "KEYBOARD.COK");
    f = fopen(the_path, "rb");
    if (f == NULL) {
        if (harness_game_info.defines.requires_ascii_table) {
#if !defined(DETHRACE_FIX_BUGS)
            PDFatalError("This .exe must have KEYBOARD.COK in the DATA folder.");
#endif
        }
        // dethrace: demos do not ship with KEYBOARD.COK file
        memcpy(gASCII_table, harness_game_info.defines.ascii_table, sizeof(gASCII_table));
        memcpy(gASCII_shift_table, harness_game_info.defines.ascii_shift_table, sizeof(gASCII_shift_table));
    } else {
        PathCat(the_path, gApplication_path, "KEYBOARD.COK");
        f = fopen(the_path, "rb");
        if (f == NULL) {
            PDFatalError("This .exe must have KEYBOARD.COK in the DATA folder.");
        }
        fseek(f, 0, 2);
        len = ftell(f) / 2;
        rewind(f);
        fread(gASCII_table, len, 1u, f);
        fread(gASCII_shift_table, len, 1u, f);
        fclose(f);
    }
    Win32InitInputDevice();
}

void Win32CreateWindow(void) {
    // wnd_class.style = 3;
    // wnd_class.lpfnWndProc = window_proc;
    // wnd_class.cbClsExtra = 0;
    // wnd_class.cbWndExtra = 0;
    // wnd_class.hInstance = gWin32_hinst;
    // wnd_class.hIcon = LoadIconA(0, (LPCSTR)0x7F00);
    // wnd_class.hCursor = LoadCursorA(0, (LPCSTR)0x7F00);
    // wnd_class.hbrBackground = (HBRUSH)GetStockObject(4);
    // wnd_class.lpszMenuName = 0;
    // wnd_class.lpszClassName = "CarmageddonClass";
    // LOWORD(v5) = RegisterClassA(&wnd_class);
    // v5 = (unsigned __int16)v5;
    // v2 = gWin32_hinst;

    // int height = GetSystemMetrics(SM_CYSCREEN);
    // int width = GetSystemMetrics(SM_CXSCREEN);

    int width = gGraf_specs[gGraf_spec_index].total_width;
    int height = gGraf_specs[gGraf_spec_index].total_height;
    // WS_VISIBLE | WS_POPUP
    gWin32_hwnd = CreateWindowExA_(0, "CarmageddonClass", "Carmageddon", 0x90000000, 0, 0, width, height, 0, NULL, NULL, NULL);
    SSDXGetWindowRect(gWin32_hwnd);
    // hdc = GetDC(gWin32_hwnd);
    // GetSystemPaletteEntries(hdc, 0, 256u, &gWin32_palette);
    // ReleaseDC(gWin32_hwnd, hdc);
    // UpdateWindow(gWin32_hwnd);
    // SetFocus(gWin32_hwnd);
}

void PDShutdownSystem(void) {
    static int been_here = 0;
    LOG_TRACE("()");

    if (!been_here) {
        been_here = 1;
        dr_dprintf("PDShutdownSystem()...");
        SSDXRelease();
        Win32ReleaseInputDevice();
        ShowCursor_(1);
        if (gWin32_hwnd) {
            dr_dprintf("Resizing main window...");
            SetWindowPos_(gWin32_hwnd, 0, -100, -100, 64, 64, 0x414u);
        }
        dr_dprintf("Servicing messages...");
        Win32PumpMessages();
        dr_dprintf("Sending WM_SHOWWINDOW broadcast message...");
        SendMessageA_(HWND_BROADCAST, 0x18u, 1u, 0);
        if (gWin32_show_fatal_error_message) {
            dr_dprintf("Displaying fatal error...");
            MessageBoxA_(0, gWin32_fatal_error_message, "Carmageddon Fatal Error", MB_ICONERROR);
        }
        if (gWin32_hwnd) {
            dr_dprintf("Destroying window...");
            DestroyWindow_(gWin32_hwnd);
            gWin32_hwnd = 0;
        }
        dr_dprintf("End of PDShutdownSystem().");
        CloseDiagnostics();
        ExitProcess_(gWin32_fatal_error_exit_code);
    }
    ExitProcess_(8u);
}

void PDSaveOriginalPalette(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

void PDRevertPalette(void) {
    LOG_TRACE("()");

    // empty function
}

int PDInitScreenVars(int pArgc, char** pArgv) {
    // this codes from dossys
    gGraf_specs[gGraf_spec_index].phys_width = gGraf_specs[gGraf_spec_index].total_width;
    gGraf_specs[gGraf_spec_index].phys_height = gGraf_specs[gGraf_spec_index].total_height;
    return 1;
}

void PDInitScreen(void) {
    Win32InitScreen();
}

void Win32InitScreen(void) {
    // SWP_NOSENDCHANGING | SWP_NOACTIVATE | SWP_NOZORDER
    SetWindowPos_(gWin32_hwnd, 0, 0, 0, gGraf_specs[gReal_graf_data_index].total_width, gGraf_specs[gReal_graf_data_index].total_height, 0x414u);
    // get_window_rect(gWin32_hwnd);
    Win32PumpMessages();
}

void PDLockRealBackScreen(void) {
    LOG_TRACE("()");

    // no-op
}

void PDUnlockRealBackScreen(void) {
    LOG_TRACE("()");

    // no-op
}

void PDAllocateScreenAndBack(void) {
    // this is a mix of windows and dos code

    dr_dprintf("PDAllocateScreenAndBack() - START...");
    BrMaterialFindHook(PDMissingMaterial);
    BrTableFindHook(PDMissingTable);
    BrModelFindHook(PDMissingModel);
    BrMapFindHook(PDMissingMap);

    int row_bytes;
    SSDXInitDirectDraw(gGraf_specs[gGraf_spec_index].total_width, gGraf_specs[gGraf_spec_index].total_height, &row_bytes);
    gScreen = BrPixelmapAllocate(BR_PMT_INDEX_8, gGraf_specs[gGraf_spec_index].total_width, gGraf_specs[gGraf_spec_index].total_height, NULL, BR_PMAF_NORMAL);

    gScreen->origin_x = 0;
    gGfx_initialized = 1;
    gScreen->origin_y = 0;
    gBack_screen = BrPixelmapMatch(gScreen, BR_PMMATCH_OFFSCREEN);
    gBack_screen->origin_x = 0;
    gBack_screen->origin_y = 0;
    gTemp_screen = BrPixelmapMatch(gScreen, BR_PMMATCH_OFFSCREEN);
    gTemp_screen->origin_x = 0;
    gTemp_screen->origin_y = 0;
    dr_dprintf("PDAllocateScreenAndBack() - END.");

    // dr_dprintf("PDAllocateScreenAndBack() - START...");
    // BrMaterialFindHook(PDMissingMaterial);
    // BrTableFindHook(PDMissingTable);
    // BrModelFindHook(PDMissingModel);
    // BrMapFindHook(PDMissingMap);
    // dr_dprintf("Setting up DirectDraw stuff...");
    // SSDXStart(gWin32_hwnd, 0, 1);
    // dr_dprintf("Setting up DirectSound stuff...");
    // int result = SSDXStart(gWin32_hwnd, 0, 2);
    // int row_bytes;
    // if (result == 0) {
    //     result = SSDXInitDirectDraw(gGraf_specs[gGraf_spec_index].total_width, gGraf_specs[gGraf_spec_index].total_height, &row_bytes);
    // }
    // if (result) {
    //     SSDXHandleError(result);
    //     PDFatalError("Unable to setup DirectDraw - please check that DirectX is installed");
    // }
    // gGraf_specs[gGraf_spec_index].row_bytes = row_bytes;
    // gGraf_specs[gGraf_spec_index].phys_width = gGraf_specs[gGraf_spec_index].row_bytes;
    // gGraf_specs[gGraf_spec_index].phys_height = gGraf_specs[gGraf_spec_index].total_height;
    // dr_dprintf(
    //     "Graf spec info after screen allocation:\n"
    //     "                 Total width:   %d\n"
    //     "                 Total height:  %d\n"
    //     "                 Phys width:    %d\n"
    //     "                 Phys height:   %d\n"
    //     "                 Row bytes:     %d\n",
    //     gGraf_specs[gGraf_spec_index].total_width,
    //     gGraf_specs[gGraf_spec_index].total_height,
    //     gGraf_specs[gGraf_spec_index].phys_width,
    //     gGraf_specs[gGraf_spec_index].phys_height,
    //     gGraf_specs[gGraf_spec_index].row_bytes);
    // dr_dprintf("First attempt at locking surface...");
    // PDLockRealBackScreen();
    // dr_dprintf("Done first surface lock; gOffscreen_pixels 0x%p", gOffscreen_pixels);
    // gBack_screen = BrPixelmapAllocate(
    //     3u,
    //     gGraf_specs[gGraf_spec_index].phys_width,
    //     gGraf_specs[gGraf_spec_index].phys_height,
    //     gOffscreen_pixels,
    //     0);
    // gScreen = gBack_screen;
    // gBack_screen->origin_x = 0;
    // gBack_screen->origin_y = 0;
    // gBack_screen->base_x = 0;
    // gBack_screen->base_y = 0;
    // gBack_screen->row_bytes = gGraf_specs[gGraf_spec_index].row_bytes;
    // gTemp_screen = (br_pixelmap*)BrPixelmapMatch(gBack_screen, 0);
    // gGfx_initialized = 1;
    // memset(gBack_screen->pixels, 0, gBack_screen->row_bytes * gBack_screen->height);
    // PDScreenBufferSwap(0);
    // dr_dprintf("PDAllocateScreenAndBack() - END.");
}

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
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

br_pixelmap* PDInterfacePixelmap(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

void SwapBackScreen(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

void ReallyCopyBackScreen(int pRendering_area_only, int pClear_top_and_bottom) {
    LOG_TRACE("(%d, %d)", pRendering_area_only, pClear_top_and_bottom);
    NOT_IMPLEMENTED();
}

void CopyBackScreen(int pRendering_area_only) {
    LOG_TRACE("(%d)", pRendering_area_only);
    NOT_IMPLEMENTED();
}

void PDScreenBufferSwap(int pRendering_area_only) {
    // taken from dossys

    if (pRendering_area_only) {
        BrPixelmapRectangleCopy(gScreen, gY_offset, gX_offset, gRender_screen, 0, 0, gWidth, gHeight);
    } else {
        if (gReal_graf_data_index == gGraf_data_index) {
            BrPixelmapDoubleBuffer(gScreen, gBack_screen);
        } else {
            DRPixelmapDoubledCopy(gTemp_screen, gBack_screen, 320, 200, 0, 40);
            BrPixelmapDoubleBuffer(gScreen, gTemp_screen);
        }
    }
}

void PDPixelmapToScreenRectangleCopy(br_pixelmap* dst, br_int_16 dx, br_int_16 dy, br_pixelmap* src, br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h) {
    LOG_TRACE("(%p, %d, %d, %p, %d, %d, %d, %d)", dst, dx, dy, src, sx, sy, w, h);
    NOT_IMPLEMENTED();
}

void PDPixelmapHLineOnScreen(br_pixelmap* dst, br_int_16 x1, br_int_16 y1, br_int_16 x2, br_int_16 y2, br_uint_32 colour) {
    LOG_TRACE("(%p, %d, %d, %d, %d, %d)", dst, x1, y1, x2, y2, colour);
    NOT_IMPLEMENTED();
}

void PDPixelmapVLineOnScreen(br_pixelmap* dst, br_int_16 x1, br_int_16 y1, br_int_16 x2, br_int_16 y2, br_uint_32 colour) {
    LOG_TRACE("(%p, %d, %d, %d, %d, %d)", dst, x1, y1, x2, y2, colour);
    NOT_IMPLEMENTED();
}

void PDInstallErrorHandlers(void) {
    LOG_TRACE("()");

    gWin32_br_diaghandler.identifier = "LlantisilioBlahBlahBlahOgOgOch";
    gWin32_br_diaghandler.warning = Win32BRenderWarningFunc;
    gWin32_br_diaghandler.failure = Win32BRenderFailureFunc;
    BrDiagHandlerSet(&gWin32_br_diaghandler);
}

void PDSetFileVariables(void) {

    // Changed by dethrace for cross-platform
    // strcpy(gDir_separator, "\\");
    strcpy(gDir_separator, "/");
}

void PDBuildAppPath(char* pThe_path) {
    GetCurrentDirectoryA_(253, pThe_path);
    // GetShortPathNameA(pThe_path, pThe_path, 253);
    strcat(pThe_path, "/"); // original: strcat(pThe_path, "\\")
    dr_dprintf("Application path '%s'", pThe_path);
}

void PDForEveryFile(char* pThe_path, void (*pAction_routine)(char*)) {
    char found_path[256];        // [esp+Ch] [ebp-448h] BYREF
    WIN32_FIND_DATAA_ find_data; // [esp+10Ch] [ebp-348h] BYREF
    HANDLE_ hFindFile;           // [esp+24Ch] [ebp-208h]
    char file_filter[256];       // [esp+250h] [ebp-204h] BYREF
    char current_dir[260];       // [esp+350h] [ebp-104h] BYREF

    GetCurrentDirectoryA_(sizeof(current_dir), current_dir);
    if (SetCurrentDirectoryA_(pThe_path)) {
        strcpy(file_filter, "*.???");
        hFindFile = FindFirstFileA_(file_filter, &find_data);
        if (hFindFile != INVALID_HANDLE_VALUE) {
            do {
                PathCat(found_path, pThe_path, find_data.cFileName);
                pAction_routine(found_path);
            } while (FindNextFileA_(hFindFile, &find_data));
            FindClose_(hFindFile);
        }
        SetCurrentDirectoryA_(current_dir);
    }
}

void PDSetPalette(br_pixelmap* pThe_palette) {
    PDSetPaletteEntries(pThe_palette, 0, 256);
}

void Win32SetPaletteEntries(uint8_t* entries, int pFirst_colour, int pCount) {
    int i;           // [esp+Ch] [ebp-Ch]
    int last_colour; // [esp+14h] [ebp-4h]

    last_colour = pFirst_colour + pCount - 1;
    if (last_colour > 255) {
        last_colour = 255;
    }
    for (i = pFirst_colour; i <= last_colour; i++) {
        gWin32_palette[i].peFlags = 0;
        gWin32_palette[i].peRed = entries[i * 4 + 2];
        gWin32_palette[i].peGreen = entries[i * 4 + 1];
        gWin32_palette[i].peBlue = entries[i * 4];
    }
    SSDXSetPaleeteEntries(gWin32_palette, 0, 256);
}

void PDSetPaletteEntries(br_pixelmap* pPalette, int pFirst_colour, int pCount) {
    Win32SetPaletteEntries(pPalette->pixels, pFirst_colour, pCount);
}

void PDSwitchToRealResolution(void) {
    LOG_TRACE("()");
}

void PDSwitchToLoresMode(void) {
    LOG_TRACE("()");
}

void PDMouseButtons(int* pButton_1, int* pButton_2) {
    br_uint_32 mouse_buttons;
    br_int_32 mouse_x;
    br_int_32 mouse_y;
    LOG_TRACE("(%p, %p)", pButton_1, pButton_2);

    // added by dethrace
    gHarness_platform.GetMouseButtons(pButton_1, pButton_2);
}

void PDGetMousePosition(int* pX_coord, int* pY_coord) {
    POINT_ p;
    LOG_TRACE("(%p, %p)", pX_coord, pY_coord);

    GetCursorPos_(&p);
    ScreenToClient_(gWin32_hwnd, &p);
    *pX_coord = p.x;
    *pY_coord = p.y;
}

int PDGetTotalTime(void) {
    return timeGetTime_();
}

int PDServiceSystem(tU32 pTime_since_last_call) {
    Win32PumpMessages();
    return 0;
}

void Win32InitInputDevice(void) {
    // do directinput stuff
    gJoystick_deadzone = 8000;
}

void Win32AllocateActionReplayBuffer(void) {
    MEMORYSTATUS_ mem_status; // [esp+Ch] [ebp-28h] BYREF
    size_t buf_size;          // [esp+2Ch] [ebp-8h]
    void* buf;                // [esp+30h] [ebp-4h]

    buf = 0;
    if (gWin32_action_replay_buffer_allocated) {
        return;
    }
    gWin32_action_replay_buffer_allocated = 1;
    mem_status.dwLength = sizeof(mem_status);
    GlobalMemoryStatus_(&mem_status);
    dr_dprintf(
        "Win32AllocateActionReplayBuffer(): Memory Status BEFORE Action Replay Allocation:\n"
        "             dwLength        %u\n"
        "             dwMemoryLoad    %u\n"
        "             dwTotalPhys     %u\n"
        "             dwAvailPhys     %u\n"
        "             dwTotalPageFile %u\n"
        "             dwAvailPageFile %u\n"
        "             dwTotalVirtual  %u\n"
        "             dwAvailVirtual  %u",
        mem_status.dwLength,
        mem_status.dwMemoryLoad,
        mem_status.dwTotalPhys,
        mem_status.dwAvailPhys,
        mem_status.dwTotalPageFile,
        mem_status.dwAvailPageFile,
        mem_status.dwTotalVirtual,
        mem_status.dwAvailVirtual);

    buf_size = 20000000;

    if (mem_status.dwTotalPhys < 16000000) {
        buf_size = 500000;
    }
    if (mem_status.dwTotalPhys < 24000000) {
        buf_size = 4000000;
    }
    if (mem_status.dwTotalPhys < 32000000) {
        buf_size = 6000000;
    }
    if (mem_status.dwTotalPhys < 48000000) {
        buf_size = 12000000;
    }

    dr_dprintf("Win32AllocateActionReplayBuffer(): We want %d bytes...", buf_size);
    if (mem_status.dwAvailPhys + mem_status.dwAvailPageFile < buf_size) {
        buf_size = mem_status.dwAvailPhys + mem_status.dwAvailPageFile - 1048576;
        dr_dprintf("Win32AllocateActionReplayBuffer(): ...but there's only %d bytes available...", buf_size);
    }
    if (buf_size < 65536) {
        buf_size = 65536;
        dr_dprintf("Win32AllocateActionReplayBuffer(): ...but we have to have a minimum size of %d bytes...", 65536);
    }
    while (buf_size >= 65536) {
        buf = malloc(buf_size);
        if (buf) {
            break;
        }
        buf_size -= 65536;
    }
    gWin32_action_replay_buffer = buf;
    if (buf) {
        gWin32_action_replay_buffer_size = buf_size;
        Sleep_(1000u);
    } else {
        gWin32_action_replay_buffer_size = 0;
    }
    dr_dprintf("Win32AllocateActionReplayBuffer(): Actually allocated %d bytes.", buf_size);
    GlobalMemoryStatus_(&mem_status);
    dr_dprintf(
        "Win32AllocateActionReplayBuffer(): Memory Status AFTER Action Replay Allocation:\n"
        "             dwLength        %u\n"
        "             dwMemoryLoad    %u\n"
        "             dwTotalPhys     %u\n"
        "             dwAvailPhys     %u\n"
        "             dwTotalPageFile %u\n"
        "             dwAvailPageFile %u\n"
        "             dwTotalVirtual  %u\n"
        "             dwAvailVirtual  %u",
        mem_status.dwLength,
        mem_status.dwMemoryLoad,
        mem_status.dwTotalPhys,
        mem_status.dwAvailPhys,
        mem_status.dwTotalPageFile,
        mem_status.dwAvailPageFile,
        mem_status.dwTotalVirtual,
        mem_status.dwAvailVirtual);
}

void PDAllocateActionReplayBuffer(char** pBuffer, tU32* pBuffer_size) {
    LOG_TRACE("(%p, %p)", pBuffer, pBuffer_size);

    Win32AllocateActionReplayBuffer();
    *pBuffer = gWin32_action_replay_buffer;
    *pBuffer_size = gWin32_action_replay_buffer_size;
}

void PDDisposeActionReplayBuffer(char* pBuffer) {
    LOG_TRACE("(\"%s\")", pBuffer);
}

// this function is taken from dossys
void Usage(char* pProgpath) {
    char base_name[256]; // fix: changed from 9 to avoid overflow on longer filenames

    _splitpath_(pProgpath, NULL, NULL, base_name, NULL);

    fprintf(stderr,
        "Usage: %s [%s] [%s YonFactor] [%s CarSimplificationLevel] [%s SoundDetailLevel] [%s] [%s] [%s] [%s] [%s] [%s]\nWhere YonFactor is between 0 and 1,\nCarSimplificationLevel is a whole number between 0 and %d,\nand SoundDetailLevel is a whole number.\n",
        base_name,
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
        } else {
            Usage(pArgv[0]);
        }
    }

    gNetwork_profile_fname[0] = 0;
    uint32_t len = GetCurrentDirectoryA_(240, gNetwork_profile_fname);
    if (len > 0 && len == strlen(gNetwork_profile_fname)) {
        gNetwork_profile_file_exists = 1;
        strcat(gNetwork_profile_fname, "/");
        strcat(gNetwork_profile_fname, "NETWORK.INI");
    }

    GameMain(pArgc, pArgv);
    return 0;
}

int OurGetChar(void) {
    int key;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

void PDEnterDebugger(char* pStr) {
    static unsigned char* save_it;
    LOG_TRACE("(\"%s\")", pStr);

    dr_dprintf("PDEnterDebugger(): %s", pStr);
    ShowCursor_(1);
#ifdef DETHRACE_FIX_BUGS
    if (strcmp(pStr, "Bet you weren't expecting this") != 0
#else
    if (pStr != "Bet you weren't expecting this"
#endif
        && _CrtDbgReport_(_CRT_ASSERT, "C:\\Msdev\\Projects\\DethRace\\Win95sys.c", 437, 0, 0) == 1) {

        abort(); // original: __debugbreak();
    }
    ShowCursor_(0);
}

// Added function
br_material* PDMissingMaterial(char* name) {
    LOG_TRACE("(\"%s\")", name);

    // FIXME: call functiont in harness
    dr_dprintf("SOMETHING MISSING WARNING - Can't find material '%s'", name);
    return NULL;
}

// Added function
br_pixelmap* PDMissingTable(char* name) {
    LOG_TRACE("(\"%s\")", name);

    // FIXME: call function in harness
    dr_dprintf("SOMETHING MISSING WARNING - Can't find shade table '%s'", name);
    return NULL;
}

// Added function
br_model* PDMissingModel(char* name) {
    LOG_TRACE("(\"%s\")", name);

    // FIXME: call function in harness
    dr_dprintf("SOMETHING MISSING WARNING - Can't find model '%s'", name);
    return NULL;
}

// Added function
br_pixelmap* PDMissingMap(char* name) {
    LOG_TRACE("(\"%s\")", name);

    // FIXME: call function in harness
    dr_dprintf("SOMETHING MISSING WARNING - Can't find pixelmap '%s'", name);
    return NULL;
}

void PDEndItAllAndReRunTheBastard(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

int InitJoysticks(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

tU32 ReadJoystickAxis(int pBit) {
    tU32 val;
    tU32 count;
    LOG_TRACE("(%d)", pBit);
    NOT_IMPLEMENTED();
}

void PDReadJoySticks(void) {
    tU32 temp1x;
    tU32 temp1y;
    tU32 temp2x;
    tU32 temp2y;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

tS32 PDGetJoy1X(void) {
    tS32 joy;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

tS32 PDGetJoy1Y(void) {
    tS32 joy;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

tS32 PDGetJoy2X(void) {
    tS32 joy;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

tS32 PDGetJoy2Y(void) {
    tS32 joy;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

int PDGetJoy1Button1(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

int PDGetJoy1Button2(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

int PDGetJoy1Button3(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

int PDGetJoy1Button4(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

int PDGetJoy2Button1(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

int PDGetJoy2Button2(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

int PDGetJoy2Button3(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

int PDGetJoy2Button4(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

int PDFileUnlock(char* pThe_path) {
    uint32_t dwFileAttributes; // [esp+Ch] [ebp-4h]
    LOG_TRACE("(\"%s\")", pThe_path);

    dwFileAttributes = GetFileAttributesA_(pThe_path);
    return dwFileAttributes != INVALID_FILE_ATTRIBUTES && SetFileAttributesA_(pThe_path, dwFileAttributes & ~FILE_ATTRIBUTE_READONLY);
}

int PDCheckDriveExists2(char* pThe_path, char* pFile_name, tU32 pMin_size) {

    char the_path[256]; // [esp+Ch] [ebp-108h] BYREF
    tU32 file_size;     // [esp+10Ch] [ebp-8h]
    HANDLE_ hFile;      // [esp+110h] [ebp-4h]
    LOG_TRACE("(\"%s\", \"%s\", %d)", pThe_path, pFile_name, pMin_size);

    file_size = 0;
    if (pFile_name) {
        PathCat(the_path, pThe_path, pFile_name);
    } else {
        strcpy(the_path, pThe_path);
    }
    if (the_path[0] && the_path[1] == ':' && !the_path[2]) {
        strcat(the_path, gDir_separator);
    }
    if (GetFileAttributesA_(pThe_path) == INVALID_FILE_ATTRIBUTES) {
        return 0;
    }
    hFile = CreateFileA_(the_path, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile != INVALID_HANDLE_VALUE) {
        file_size = GetFileSize_(hFile, 0);
        CloseHandle_(hFile);
    }
    return file_size >= pMin_size;
}

int PDDoWeLeadAnAustereExistance(void) {
    return 0;
}

// Windows-specific functions. The only name we know for sure is "Win32AllocateReplayBuffer".

void Win32BRenderWarningFunc(char* msg) {
    dr_dprintf("*******************************************************************************");
    dr_dprintf("BRender WARNING: '%s'", msg);
    dr_dprintf("*******************************************************************************");
}

void Win32BRenderFailureFunc(char* msg) {
    dr_dprintf("*******************************************************************************");
    dr_dprintf("BRender FAILURE: '%s'", msg);
    dr_dprintf("*******************************************************************************");
    Win32FatalError("BRender error detected:", msg);
}
