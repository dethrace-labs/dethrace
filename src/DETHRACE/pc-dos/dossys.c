#include "brender/brender.h"
#include "car.h"
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
#include "utility.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#ifdef __DOS__
#define GFX_INIT_STRING_32X20X8 "MCGA,W:320,H:200,B:8"
#define GFX_INIT_STRING_64X48X8 "VESA,W:640,H:480,B:8"

int gDOSGfx_initialized;

tU32 gUpper_loop_limit;
int gReal_back_screen_locked;
void (*gPrev_keyboard_handler)();

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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall KeyTranslation(tU8 pKey_index@<EAX>, tU8 pScan_code_1@<EDX>, tU8 pScan_code_2@<EBX>)
void KeyTranslation(tU8 pKey_index, tU8 pScan_code_1, tU8 pScan_code_2) {
    LOG_TRACE("(%d, %d, %d)", pKey_index, pScan_code_1, pScan_code_2);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl KeyBegin()
void KeyBegin() {
    NOT_IMPLEMENTED();
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
}

// IDA: void __usercall PDFatalError(char *pThe_str@<EAX>)
void PDFatalError(char* pThe_str) {
    static int been_here = 0;
    LOG_TRACE("(\"%s\")", pThe_str);

    if (been_here) {
        exit(1);
    }
    been_here = 1;

    dr_dprintf("FATAL ERROR: %s", pThe_str);

    _unittest_last_fatal_error = pThe_str;
    fprintf(stderr, "FATAL ERROR: %s\n", pThe_str);

    // wait for keypress

    abort();
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

    // v4 = DOSMouseBegin();
    gJoystick_deadzone = 8000;
    // gUpper_loop_limit = sub_A1940(v4, v5, v3, v6) / 2;

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
    } else {
        memcpy(gASCII_table, harness_game_info.defines.ascii_table, sizeof(gASCII_table));
        memcpy(gASCII_shift_table, harness_game_info.defines.ascii_shift_table, sizeof(gASCII_shift_table));
    }
}

// IDA: void __cdecl PDShutdownSystem()
void PDShutdownSystem() {
    LOG_TRACE("()");

    Harness_Hook_PDShutdownSystem();

    CloseDiagnostics();
    exit(0);
}

// IDA: void __cdecl PDSaveOriginalPalette()
void PDSaveOriginalPalette() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PDRevertPalette()
void PDRevertPalette() {
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

    dr_dprintf("PDAllocateScreenAndBack() - START...");
    BrMaterialFindHook(PDMissingMaterial);
    BrTableFindHook(PDMissingTable);
    BrModelFindHook(PDMissingModel);
    BrMapFindHook(PDMissingMap);

    // This is a bit of a mix between the original DOS code and windows code...
    // DOS:
    // gScreen = DOSGfxBegin(gGraf_specs[gGraf_spec_index].gfx_init_string);
    //
    // Windows:
    // SSDXInit(gGraf_specs[gGraf_spec_index].total_width, gGraf_specs[gGraf_spec_index].total_height)
    // gScreen = BrPixelmapAllocate(BR_PMT_INDEX_8, ...)
    //

    Harness_Hook_GraphicsInit(gGraf_specs[gGraf_spec_index].total_width, gGraf_specs[gGraf_spec_index].total_height);
    gScreen = BrPixelmapAllocate(BR_PMT_INDEX_8, gGraf_specs[gGraf_spec_index].total_width, gGraf_specs[gGraf_spec_index].total_height, NULL, BR_PMAF_NORMAL);

    gScreen->origin_x = 0;
    gDOSGfx_initialized = 1;
    gScreen->origin_y = 0;
    gBack_screen = BrPixelmapMatch(gScreen, BR_PMMATCH_OFFSCREEN);
    gBack_screen->origin_x = 0;
    gBack_screen->origin_y = 0;
    gTemp_screen = BrPixelmapMatch(gScreen, BR_PMMATCH_OFFSCREEN);
    gTemp_screen->origin_x = 0;
    gTemp_screen->origin_y = 0;
    dr_dprintf("PDAllocateScreenAndBack() - END.");
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
    p = (tU8*)pPalette->pixels + 4 * pFirst_colour;
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
void PDSwitchToRealResolution() {
    LOG_TRACE("()");
}

// IDA: void __cdecl PDSwitchToLoresMode()
void PDSwitchToLoresMode() {
    LOG_TRACE("()");
}

// IDA: void __usercall PDMouseButtons(int *pButton_1@<EAX>, int *pButton_2@<EDX>)
void PDMouseButtons(int* pButton_1, int* pButton_2) {
    br_uint_32 mouse_buttons;
    br_int_32 mouse_x;
    br_int_32 mouse_y;
    LOG_TRACE("(%p, %p)", pButton_1, pButton_2);

    Harness_Hook_GetMouseButtons(pButton_1, pButton_2);
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

    Harness_Hook_GetMousePosition(pX_coord, pY_coord);
}

// IDA: int __cdecl PDGetTotalTime()
int PDGetTotalTime() {
    return OS_GetTime();
}

// IDA: int __usercall PDServiceSystem@<EAX>(tU32 pTime_since_last_call@<EAX>)
int PDServiceSystem(tU32 pTime_since_last_call) {
    Harness_Hook_PDServiceSystem();
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

    OS_AllocateActionReplayBuffer(pBuffer, pBuffer_size);
}

// IDA: void __usercall PDDisposeActionReplayBuffer(char *pBuffer@<EAX>)
void PDDisposeActionReplayBuffer(char* pBuffer) {
    LOG_TRACE("(\"%s\")", pBuffer);
}

// IDA: void __usercall Usage(char *pProgpath@<EAX>)
void Usage(char* pProgpath) {
    // char basename[9];
    char basename[256]; // fix: changed from 9 to avoid overflow on longer filenames

    OS_Basename(pProgpath, basename);

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

// IDA: void __usercall PDEnterDebugger(char *pStr@<EAX>)
void PDEnterDebugger(char* pStr) {
    static unsigned char* save_it;
    LOG_TRACE("(\"%s\")", pStr);

    // FIXME: uses __CrtDbgReport when using MSVC runtime
    STUB_ONCE();

    dr_dprintf("PDEnterDebugger(): %s", pStr);
    // ShowCursor(1);
    abort();
    // ShowCursor(0);
}

// IDA: void __cdecl PDEndItAllAndReRunTheBastard()
void PDEndItAllAndReRunTheBastard() {
    LOG_TRACE("()");
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
    return stat_failed == 0 && buf.st_size >= pMin_size;
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

#endif

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
int PDGetGorePassword() {
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
