#include "brender.h"
#include "common/car.h"
#include "common/errors.h"
#include "common/globvars.h"
#include "common/grafdata.h"
#include "common/graphics.h"
#include "common/loadsave.h"
#include "common/main.h"
#include "common/sound.h"
#include "common/utility.h"
#include "harness.h"
#include "harness_hooks.h"
#include "input.h"
#include "pd/sys.h"
#include "watcom_functions.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#ifdef _WIN32
#define NS_PER_SEC (1000ULL * 1000ULL * 1000ULL)
#define CLOCK_MONOTONIC 1

int clock_gettime(int dummy, struct timespec* tv) {
    static LARGE_INTEGER ticksPerSec;
    LARGE_INTEGER ticks;
    double seconds;

    if (!ticksPerSec.QuadPart) {
        QueryPerformanceFrequency(&ticksPerSec);
        if (!ticksPerSec.QuadPart) {
            errno = ENOTSUP;
            return -1;
        }
    }

    QueryPerformanceCounter(&ticks);

    seconds = (double)ticks.QuadPart / (double)ticksPerSec.QuadPart;
    tv->tv_sec = (time_t)seconds;
    tv->tv_nsec = (long)((ULONGLONG)(seconds * NS_PER_SEC) % NS_PER_SEC);

    return 0;
}
#endif

int gASCII_table[128];
tU32 gKeyboard_bits[8];
int gASCII_shift_table[128];
tGraf_spec gGraf_specs[2] = {
    { 8, 1, 0, 320, 200, 0, 0, "32X20X8", "MCGA,W:320,H:200,B:8", 0, 0, 0, NULL },
    { 8, 1, 0, 640, 480, 0, 0, "64X48X8", "VESA,W:640,H:480,B:8", 0, 0, 0, NULL }
};

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
int gReplay_override;
br_pixelmap* gReal_back_screen;
tS32 gJoystick_min1x;
br_pixelmap* gTemp_screen;
int gDOSGfx_initialized;
tU32 gUpper_loop_limit;
int gExtra_mem;
int gReal_back_screen_locked;
void (*gPrev_keyboard_handler)();
tU8 gScan_code[123][2];

const double NANOSECONDS_TO_MILLISECONDS = 1.0 / 1000000.0;

int _unittest_do_not_exit = 0;
char* _unittest_last_fatal_error;

// IDA: void __cdecl KeyboardHandler()
void KeyboardHandler() {
    tU8 scan_code;
    tU8 up;
    static tU8 extended;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall KeyDown@<EAX>(tU8 pScan_code@<EAX>)
int KeyDown(tU8 pScan_code) {
    return Harness_Hook_KeyDown(pScan_code);
}

// IDA: void __usercall KeyTranslation(tU8 pKey_index@<EAX>, tU8 pScan_code_1@<EDX>, tU8 pScan_code_2@<EBX>)
void KeyTranslation(tU8 pKey_index, tU8 pScan_code_1, tU8 pScan_code_2) {
    LOG_TRACE("(%d, %d, %d)", pKey_index, pScan_code_1, pScan_code_2);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl KeyBegin()
void KeyBegin() {

    Harness_Hook_KeyBegin();
    // int v0; // edx@0
    // int v1; // ST00_4@1
    // __int16 v2; // dx@1

    // *(_WORD *)gScan_code[11] = 11;
    // *(_WORD *)gScan_code[13] = 3;
    // *(_WORD *)gScan_code[14] = 4;
    // *(_WORD *)gScan_code[12] = 2;
    // *(_WORD *)gScan_code[15] = 5;
    // *(_WORD *)gScan_code[17] = 7;
    // *(_WORD *)gScan_code[18] = 8;
    // *(_WORD *)gScan_code[16] = 6;
    // *(_WORD *)gScan_code[19] = 9;
    // *(_WORD *)gScan_code[21] = 30;
    // *(_WORD *)gScan_code[22] = 48;
    // *(_WORD *)gScan_code[20] = 10;
    // *(_WORD *)gScan_code[23] = 46;
    // *(_WORD *)gScan_code[25] = 18;
    // *(_WORD *)gScan_code[26] = 33;
    // *(_WORD *)gScan_code[24] = 32;
    // *(_WORD *)gScan_code[27] = 34;
    // *(_WORD *)gScan_code[29] = 23;
    // *(_WORD *)gScan_code[30] = 36;
    // *(_WORD *)gScan_code[28] = 35;
    // *(_WORD *)gScan_code[31] = 37;
    // *(_WORD *)gScan_code[33] = 50;
    // *(_WORD *)gScan_code[34] = 49;
    // *(_WORD *)gScan_code[32] = 38;
    // *(_WORD *)gScan_code[35] = 24;
    // *(_WORD *)gScan_code[37] = 16;
    // *(_WORD *)gScan_code[38] = 19;
    // *(_WORD *)gScan_code[36] = 25;
    // *(_WORD *)gScan_code[39] = 31;
    // *(_WORD *)gScan_code[41] = 22;
    // *(_WORD *)gScan_code[42] = 47;
    // *(_WORD *)gScan_code[40] = 20;
    // *(_WORD *)gScan_code[43] = 17;
    // *(_WORD *)gScan_code[44] = 45;
    // *(_WORD *)gScan_code[45] = 21;
    // *(_WORD *)gScan_code[46] = 44;
    // *(_WORD *)gScan_code[47] = 41;
    // *(_WORD *)gScan_code[48] = 12;
    // *(_WORD *)gScan_code[49] = 13;
    // *(_WORD *)gScan_code[50] = 14;
    // *(_WORD *)gScan_code[51] = 28;
    // *(_WORD *)gScan_code[52] = 156;

    // *(_WORD *)gScan_code[0] = 13866;
    // *(_WORD *)gScan_code[1] = -18376;
    // *(_WORD *)gScan_code[2] = -25315;
    // *(_WORD *)gScan_code[3] = -25315;
    // *(_WORD *)gScan_code[4] = 58;
    // *(_WORD *)gScan_code[55] = 86;
    // *(_WORD *)gScan_code[54] = 53;
    // *(_WORD *)gScan_code[56] = 39;
    // *(_WORD *)gScan_code[59] = 51;
    // *(_WORD *)gScan_code[53] = 15;
    // *(_WORD *)gScan_code[58] = 52;
    // *(_WORD *)gScan_code[60] = 26;
    // *(_WORD *)gScan_code[63] = 1;
    // *(_WORD *)gScan_code[57] = 40;
    // *(_WORD *)gScan_code[62] = 43;
    // *(_WORD *)gScan_code[64] = 210;
    // *(_WORD *)gScan_code[67] = 207;
    // *(_WORD *)gScan_code[61] = 27;
    // *(_WORD *)gScan_code[66] = 199;
    // *(_WORD *)gScan_code[68] = 201;
    // *(_WORD *)gScan_code[71] = 205;
    // *(_WORD *)gScan_code[65] = 211;
    // *(_WORD *)gScan_code[70] = 203;
    // *(_WORD *)gScan_code[72] = 200;
    // *(_WORD *)gScan_code[69] = 209;
    // *(_WORD *)gScan_code[74] = 69;
    // *(_WORD *)gScan_code[73] = 208;
    // *(_WORD *)gScan_code[75] = 181;
    // *(_WORD *)gScan_code[76] = 55;
    // *(_WORD *)gScan_code[78] = 78;
    // *(_WORD *)gScan_code[77] = 74;
    // *(_WORD *)gScan_code[80] = 0;
    // *(_WORD *)gScan_code[79] = 83;
    // *(_WORD *)gScan_code[82] = 79;
    // *(_WORD *)gScan_code[84] = 81;
    // *(_WORD *)gScan_code[81] = 82;
    // *(_WORD *)gScan_code[83] = 80;
    // *(_WORD *)gScan_code[86] = 76;
    // *(_WORD *)gScan_code[88] = 71;
    // *(_WORD *)gScan_code[85] = 75;
    // *(_WORD *)gScan_code[87] = 77;
    // *(_WORD *)gScan_code[90] = 73;
    // *(_WORD *)gScan_code[92] = 60;
    // *(_WORD *)gScan_code[89] = 72;
    // *(_WORD *)gScan_code[91] = 59;
    // *(_WORD *)gScan_code[94] = 62;
    // *(_WORD *)gScan_code[96] = 64;
    // *(_WORD *)gScan_code[93] = 61;
    // *(_WORD *)gScan_code[95] = 63;
    // *(_WORD *)gScan_code[98] = 66;
    // *(_WORD *)gScan_code[100] = 68;
    // *(_WORD *)gScan_code[97] = 65;
    // *(_WORD *)gScan_code[99] = 67;
    // *(_WORD *)gScan_code[102] = 88;
    // *(_WORD *)gScan_code[104] = 70;
    // *(_WORD *)gScan_code[101] = 87;
    // *(_WORD *)gScan_code[103] = 0;
    // *(_WORD *)gScan_code[105] = 0;
    // *(_WORD *)gScan_code[106] = 57;
    // *(_WORD *)gScan_code[5] = 54;
    // *(_WORD *)gScan_code[6] = 184;
    // *(_WORD *)gScan_code[7] = 157;
    // *(_WORD *)gScan_code[8] = 42;
    // *(_WORD *)gScan_code[9] = 56;
    // *(_WORD *)gScan_code[10] = 29;

    //gPrev_keyboard_handler = (void (__fastcall *)())dos_getvect(9);
    //unk_142E6C = v2;
    //dos_setvect(9, KeyboardHandler);
}

// IDA: void __cdecl KeyEnd()
void KeyEnd() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall KeyDown22@<EAX>(int pKey_index@<EAX>)
int KeyDown22(int pKey_index) {
    LOG_TRACE("(%d)", pKey_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDSetKeyArray(int *pKeys@<EAX>, int pMark@<EDX>)
void PDSetKeyArray(int* pKeys, int pMark) {
    int i;
    tS32 joyX;
    tS32 joyY;
    LOG_TRACE10("(%p, %d)", pKeys, pMark);

    gKeys_pressed = 0;
    for (i = 0; i < 123; i++) {
        if (KeyDown(gScan_code[i][0]) || KeyDown(gScan_code[i][1])) {
            gKeys_pressed = (gKeys_pressed << 8) + i + 1;
            pKeys[i] = pMark;
        } else if (pKeys[i] == pMark) {
            pKeys[i] = 0;
        }
    }
}

// IDA: int __usercall PDGetASCIIFromKey@<EAX>(int pKey@<EAX>)
int PDGetASCIIFromKey(int pKey) {
    LOG_TRACE("(%d)", pKey);

    if (PDKeyDown3(KEY_LSHIFT)) {
        return gASCII_shift_table[pKey];
    } else {
        return gASCII_table[pKey];
    }
}

// IDA: void __usercall PDFatalError(char *pThe_str@<EAX>)
void PDFatalError(char* pThe_str) {
    static int been_here = 0;
    LOG_TRACE("(\"%s\")", pThe_str);

    if (been_here) {
        if (!_unittest_do_not_exit) {
            exit(1);
        }
    }
    been_here = 1;

    _unittest_last_fatal_error = pThe_str;
    fprintf(stderr, "FATAL ERROR: %s\n", pThe_str);

    // wait for keypress

    Harness_Debug_PrintStack();

    DoSaveGame(1);
    if (!_unittest_do_not_exit) {
        exit(1);
    }
}

// IDA: void __usercall PDNonFatalError(char *pThe_str@<EAX>)
void PDNonFatalError(char* pThe_str) {
    LOG_TRACE("(\"%s\")", pThe_str);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PDInitialiseSystem()
void PDInitialiseSystem() {
    tPath_name the_path;
    FILE* f;
    int len;

    KeyBegin();

    //v4 = DOSMouseBegin();
    gJoystick_deadzone = 8000;
    //gUpper_loop_limit = sub_A1940(v4, v5, v3, v6) / 2;
    PathCat(the_path, gApplication_path, "KEYBOARD.COK");
    f = fopen(the_path, "rb");
    if (!f) {
        PDFatalError("This .exe must have KEYBOARD.COK in the DATA folder.");
    }

    fseek(f, 0, SEEK_END);
    len = ftell(f);
    rewind(f);
    fread(gASCII_table, len / 2, 1, f);
    fread(gASCII_shift_table, len / 2, 1, f);
    fclose(f);
}

// IDA: void __cdecl PDShutdownSystem()
void PDShutdownSystem() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PDSaveOriginalPalette()
void PDSaveOriginalPalette() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PDRevertPalette()
void PDRevertPalette() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PDInitScreenVars@<EAX>(int pArgc@<EAX>, char **pArgv@<EDX>)
int PDInitScreenVars(int pArgc, char** pArgv) {
    gGraf_specs[gGraf_spec_index].phys_width = gGraf_specs[gGraf_spec_index].total_width;
    gGraf_specs[gGraf_spec_index].phys_height = gGraf_specs[gGraf_spec_index].total_height;
    return 1;
}

// IDA: void __cdecl PDInitScreen()
void PDInitScreen() {
}

// IDA: void __cdecl PDLockRealBackScreen()
void PDLockRealBackScreen() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PDUnlockRealBackScreen()
void PDUnlockRealBackScreen() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PDAllocateScreenAndBack()
void PDAllocateScreenAndBack() {
    gScreen = DOSGfxBegin(gGraf_specs[gGraf_spec_index].gfx_init_string);
    gScreen->origin_x = 0;
    gDOSGfx_initialized = 1;
    gScreen->origin_y = 0;
    gBack_screen = BrPixelmapMatch(gScreen, BR_PMMATCH_OFFSCREEN);
    gBack_screen->origin_x = 0;
    gBack_screen->origin_y = 0;
    gTemp_screen = BrPixelmapMatch(gScreen, BR_PMMATCH_OFFSCREEN);
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __cdecl PDInterfacePixelmap()
br_pixelmap* PDInterfacePixelmap() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SwapBackScreen()
void SwapBackScreen() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReallyCopyBackScreen(int pRendering_area_only@<EAX>, int pClear_top_and_bottom@<EDX>)
void ReallyCopyBackScreen(int pRendering_area_only, int pClear_top_and_bottom) {
    LOG_TRACE("(%d, %d)", pRendering_area_only, pClear_top_and_bottom);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CopyBackScreen(int pRendering_area_only@<EAX>)
void CopyBackScreen(int pRendering_area_only) {
    LOG_TRACE("(%d)", pRendering_area_only);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDScreenBufferSwap(int pRendering_area_only@<EAX>)
void PDScreenBufferSwap(int pRendering_area_only) {
    LOG_TRACE10("(%d)", pRendering_area_only);
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
void PDInstallErrorHandlers() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PDSetFileVariables()
void PDSetFileVariables() {
    gDir_separator[0] = '\\';

    // Added >>
    gDir_separator[0] = '/';
    gDir_separator[1] = '\0';
    // <<
}

// IDA: void __usercall PDBuildAppPath(char *pThe_path@<EAX>)
void PDBuildAppPath(char* pThe_path) {
    int pos;

    getcwd(pThe_path, 256);
    strcat(pThe_path, "/"); // original: pThe_path[pos] = '\\';
    strcpy(gNetwork_profile_fname, pThe_path);
    strcat(gNetwork_profile_fname, "NETWORK.INI");
}

// IDA: void __usercall PDForEveryFile(char *pThe_path@<EAX>, void (*pAction_routine)(char*)@<EDX>)
void PDForEveryFile(char* pThe_path, void (*pAction_routine)(char*)) {
    char find_path[256];
    char found_path[256];
    //find_t the_find_buffer;
    DIR* d;
    struct dirent* entry;

    d = opendir(pThe_path);
    if (d) {
        while ((entry = readdir(d)) != NULL) {
            // only files, and only files that dont start with '.'
            if (entry->d_type == DT_REG && entry->d_name[0] != '.') {
                PathCat(found_path, pThe_path, entry->d_name);
                pAction_routine(found_path);
            }
        }
        closedir(d);
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
    p = (tU8*)pPalette->pixels + 4 * pFirst_colour;
    for (i = pFirst_colour; i < pFirst_colour + pCount; i++) {
        BrDevPaletteSetEntryOld(i, (p[2] << 16) | (p[1] << 8) | *p);
        p += 4;
    }
}

// IDA: void __cdecl PDSwitchToRealResolution()
void PDSwitchToRealResolution() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PDSwitchToLoresMode()
void PDSwitchToLoresMode() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDMouseButtons(int *pButton_1@<EAX>, int *pButton_2@<EDX>)
void PDMouseButtons(int* pButton_1, int* pButton_2) {
    br_uint_32 mouse_buttons;
    br_int_32 mouse_x;
    br_int_32 mouse_y;
    LOG_TRACE("(%p, %p)", pButton_1, pButton_2);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetTotalTime()
int PDGetTotalTime() {
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    return spec.tv_sec * 1000 + spec.tv_nsec / 1000000;
}

// IDA: int __usercall PDServiceSystem@<EAX>(tU32 pTime_since_last_call@<EAX>)
int PDServiceSystem(tU32 pTime_since_last_call) {
    Harness_Hook_PDServiceSystem(pTime_since_last_call);
    return 0;
}

// IDA: tU32 __cdecl LargestBlockAvail()
tU32 LargestBlockAvail() {
    SREGS sregs;
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDDisposeActionReplayBuffer(char *pBuffer@<EAX>)
void PDDisposeActionReplayBuffer(char* pBuffer) {
    LOG_TRACE("(\"%s\")", pBuffer);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall Usage(char *pProgpath@<EAX>)
void Usage(char* pProgpath) {
    char basename[9];

    splitpath(pProgpath, NULL, NULL, basename, NULL);

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
    exit(1);
}

// Renamed from "main" to "original_main" to allow for harness + unit testing
int original_main(int pArgc, char** pArgv) {
    int arg;
    int i;
    float f;

    for (i = 1; i < pArgc; i++) {

        if (Harness_Hook_HandleCommandLineArg(pArgv[i])) {
            continue;
        }

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

        } else if (strcasecmp(pArgv[i], "-robots") == 0) {
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

    GameMain(pArgc, pArgv);
    return 0;
}

// IDA: int __cdecl OurGetChar()
int OurGetChar() {
    int key;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetGorePassword()
int PDGetGorePassword() {
    int ch;
    int len;
    int chances;
    char password[17];
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDDisplayGoreworthiness(int pGory@<EAX>)
void PDDisplayGoreworthiness(int pGory) {
    tU32 delay_start;
    LOG_TRACE("(%d)", pGory);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PDEnterDebugger(char *pStr@<EAX>)
void PDEnterDebugger(char* pStr) {
    static unsigned char* save_it;
    LOG_TRACE("(\"%s\")", pStr);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PDEndItAllAndReRunTheBastard()
void PDEndItAllAndReRunTheBastard() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __usercall matherr@<EAX>(exception *err@<EAX>)
int matherr(struct exception_* err) {
    LOG_TRACE("(%p)", err);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall LoopLimitTooLow@<EAX>(tU32 limit@<EAX>)
int LoopLimitTooLow(tU32 limit) {
    clock_t start;
    tU32 count;
    tU32 val;
    LOG_TRACE("(%d)", limit);
    NOT_IMPLEMENTED();
}

// IDA: tS32 __cdecl UpperLoopLimit()
tS32 UpperLoopLimit() {
    tU32 limit;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl InitJoysticks()
int InitJoysticks() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tU32 __usercall ReadJoystickAxis@<EAX>(int pBit@<EAX>)
tU32 ReadJoystickAxis(int pBit) {
    tU32 val;
    tU32 count;
    LOG_TRACE("(%d)", pBit);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PDReadJoySticks()
void PDReadJoySticks() {
    tU32 temp1x;
    tU32 temp1y;
    tU32 temp2x;
    tU32 temp2y;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tS32 __cdecl PDGetJoy1X()
tS32 PDGetJoy1X() {
    tS32 joy;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tS32 __cdecl PDGetJoy1Y()
tS32 PDGetJoy1Y() {
    tS32 joy;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tS32 __cdecl PDGetJoy2X()
tS32 PDGetJoy2X() {
    tS32 joy;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tS32 __cdecl PDGetJoy2Y()
tS32 PDGetJoy2Y() {
    tS32 joy;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetJoy1Button1()
int PDGetJoy1Button1() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetJoy1Button2()
int PDGetJoy1Button2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetJoy1Button3()
int PDGetJoy1Button3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetJoy1Button4()
int PDGetJoy1Button4() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetJoy2Button1()
int PDGetJoy2Button1() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetJoy2Button2()
int PDGetJoy2Button2() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetJoy2Button3()
int PDGetJoy2Button3() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl PDGetJoy2Button4()
int PDGetJoy2Button4() {
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
void CriticalISR(INTPACK pRegs) {
    LOG_TRACE("(%d)", pRegs);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PDCheckDriveExists2@<EAX>(char *pThe_path@<EAX>, char *pFile_name@<EDX>, tU32 pMin_size@<EBX>)
int PDCheckDriveExists2(char* pThe_path, char* pFile_name, tU32 pMin_size) {
    struct stat buf;
    void (*old_critical_isr)();
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
    return !stat_failed && buf.st_size >= pMin_size;
}

// IDA: int __cdecl PDDoWeLeadAnAustereExistance()
int PDDoWeLeadAnAustereExistance() {
    tU32 block;

    block = LargestBlockAvail();

    dr_dprintf("PDDoWeLeadAnAustereExistance (sic): LargestBlockAvail=%d\n", block);

    if (gReal_graf_data_index == 0) {
        return block < 13000000;
    }
    return block < 15000000;
}
