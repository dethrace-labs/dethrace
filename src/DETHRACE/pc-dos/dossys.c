#include "dossys.h"

#include <time.h>
#include <sys/stat.h>

int gASCII_table[128];
tU32 gKeyboard_bits[8];
int gASCII_shift_table[128];
tGraf_spec gGraf_specs[2];
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
br_pixelmap *gReal_back_screen;
tS32 gJoystick_min1x;
br_pixelmap *gTemp_screen;
int gDOSGfx_initialized;
tU32 gUpper_loop_limit;
int gExtra_mem;
int gReal_back_screen_locked;
void (*gPrev_keyboard_handler)();
tU8 gScan_code[123][2];

int _unittest_do_not_exit = 0;

// Offset: 0
// Size: 291
void KeyboardHandler() {
    tU8 scan_code;
    tU8 up;
    tU8 extended;
}

// Offset: 292
// Size: 71
// EAX: pScan_code
int KeyDown(tU8 pScan_code) {
}

// Offset: 364
// Size: 71
// EAX: pKey_index
// EDX: pScan_code_1
// EBX: pScan_code_2
void KeyTranslation(tU8 pKey_index, tU8 pScan_code_1, tU8 pScan_code_2) {
}

// Offset: 436
// Size: 1897
void KeyBegin() {
}

// Offset: 2336
// Size: 52
void KeyEnd() {
}

// Offset: 2388
// Size: 79
// EAX: pKey_index
int KeyDown22(int pKey_index) {
}

// Offset: 2468
// Size: 141
// EAX: pKeys
// EDX: pMark
void PDSetKeyArray(int *pKeys, int pMark) {
    int i;
    tS32 joyX;
    tS32 joyY;
}

// Offset: 2612
// Size: 73
// EAX: pKey
int PDGetASCIIFromKey(int pKey) {
}

// Offset: 2688
// Size: 174
// EAX: pThe_str
void PDFatalError(char *pThe_str) {
    int been_here;

    if (been_here) {
        exit(1);
    }
    been_here = 1;

    fprintf(stderr, "FATAL ERROR: %s\n", pThe_str);
    // wait for keypress
    // DoSaveGame() -> exit
    if (_unittest_do_not_exit == 0) {
       exit(1);
    }
}

// Offset: 2864
// Size: 55
// EAX: pThe_str
void PDNonFatalError(char *pThe_str) {
}

// Offset: 2920
// Size: 190
void PDInitialiseSystem() {
    tPath_name the_path;
    FILE *f;
    int len;
}

// Offset: 3112
// Size: 48
void PDShutdownSystem() {
}

// Offset: 3160
// Size: 24
void PDSaveOriginalPalette() {
}

// Offset: 3184
// Size: 24
void PDRevertPalette() {
}

// Offset: 3208
// Size: 90
// EAX: pArgc
// EDX: pArgv
int PDInitScreenVars(int pArgc, char **pArgv) {
}

// Offset: 3300
// Size: 24
void PDInitScreen() {
}

// Offset: 3324
// Size: 113
void PDLockRealBackScreen() {
}

// Offset: 3440
// Size: 48
void PDUnlockRealBackScreen() {
}

// Offset: 3488
// Size: 431
void PDAllocateScreenAndBack() {
}

// Offset: 3920
// Size: 209
// EAX: pDst
// EDX: pSrc
// EBX: pPalette
void Copy8BitTo16BitPixelmap(br_pixelmap *pDst, br_pixelmap *pSrc, br_pixelmap *pPalette) {
    int x;
    int y;
    tU8 *src;
    tU8 value;
    tU8 red;
    tU8 green;
    tU8 blue;
    tU16 *dst;
    tU16 *palette_entry;
}

// Offset: 4132
// Size: 307
// EAX: pDst
// EDX: pSrc
// EBX: pPalette
// ECX: pOff
void Double8BitTo16BitPixelmap(br_pixelmap *pDst, br_pixelmap *pSrc, br_pixelmap *pPalette, tU16 pOff, tU16 pSrc_width, tU16 pSrc_height) {
    int x;
    int y;
    tU8 *src;
    tU8 value;
    tU8 red;
    tU8 green;
    tU8 blue;
    tU16 *dst0;
    tU16 *dst1;
    tU16 sixteen;
    tU16 *palette_entry;
}

// Offset: 4440
// Size: 35
br_pixelmap* PDInterfacePixelmap() {
}

// Offset: 4476
// Size: 154
void SwapBackScreen() {
}

// Offset: 4632
// Size: 253
// EAX: pRendering_area_only
// EDX: pClear_top_and_bottom
void ReallyCopyBackScreen(int pRendering_area_only, int pClear_top_and_bottom) {
}

// Offset: 4888
// Size: 40
// EAX: pRendering_area_only
void CopyBackScreen(int pRendering_area_only) {
}

// Offset: 4928
// Size: 92
// EAX: pRendering_area_only
void PDScreenBufferSwap(int pRendering_area_only) {
}

// Offset: 5020
// Size: 82
// EAX: dst
// EDX: dx
// EBX: dy
// ECX: src
void PDPixelmapToScreenRectangleCopy(br_pixelmap *dst, br_int_16 dx, br_int_16 dy, br_pixelmap *src, br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h) {
}

// Offset: 5104
// Size: 68
// EAX: dst
// EDX: x1
// EBX: y1
// ECX: x2
void PDPixelmapHLineOnScreen(br_pixelmap *dst, br_int_16 x1, br_int_16 y1, br_int_16 x2, br_int_16 y2, br_uint_32 colour) {
}

// Offset: 5172
// Size: 68
// EAX: dst
// EDX: x1
// EBX: y1
// ECX: x2
void PDPixelmapVLineOnScreen(br_pixelmap *dst, br_int_16 x1, br_int_16 y1, br_int_16 x2, br_int_16 y2, br_uint_32 colour) {
}

// Offset: 5240
// Size: 24
void PDInstallErrorHandlers() {
}

// Offset: 5264
// Size: 39
void PDSetFileVariables() {
}

// Offset: 5304
// Size: 98
// EAX: pThe_path
void PDBuildAppPath(char *pThe_path) {
    int pos;
}

// Offset: 5404
// Size: 133
// EAX: pThe_path
// EDX: pAction_routine
void PDForEveryFile(char *pThe_path, void (*pAction_routine)(char*)) {
    char find_path[256];
    char found_path[256];
    find_t the_find_buffer;
}

// Offset: 5540
// Size: 39
// EAX: pThe_palette
void PDSetPalette(br_pixelmap *pThe_palette) {
}

// Offset: 5580
// Size: 136
// EAX: pPalette
// EDX: pFirst_colour
// EBX: pCount
void PDSetPaletteEntries(br_pixelmap *pPalette, int pFirst_colour, int pCount) {
    int i;
    tU8 *p;
}

// Offset: 5716
// Size: 44
void PDSwitchToRealResolution() {
}

// Offset: 5760
// Size: 44
void PDSwitchToLoresMode() {
}

// Offset: 5804
// Size: 86
// EAX: pButton_1
// EDX: pButton_2
void PDMouseButtons(int *pButton_1, int *pButton_2) {
    br_uint_32 mouse_buttons;
    br_int_32 mouse_x;
    br_int_32 mouse_y;
}

// Offset: 5892
// Size: 380
// EAX: pX_coord
// EDX: pY_coord
void PDGetMousePosition(int *pX_coord, int *pY_coord) {
    br_uint_32 mouse_buttons;
    br_int_32 mouse_x2;
    br_int_32 mouse_y2;
    int delta_x;
    int delta_y;
    br_int_32 mouse_x;
    br_int_32 mouse_y;
}

// Offset: 6272
// Size: 38
int PDGetTotalTime() {
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    return spec.tv_sec * 1000 + spec.tv_nsec / 1000;
}

// Offset: 6312
// Size: 37
// EAX: pTime_since_last_call
int PDServiceSystem(tU32 pTime_since_last_call) {
}

// Offset: 6352
// Size: 121
tU32 LargestBlockAvail() {
    SREGS sregs;
    tMem_info mem_info;
    size_t memmax;
}

// Offset: 6476
// Size: 111
// EAX: pMaximum_required
// EDX: pAmount_allocated
void* PDGrabLargestMammaryWeCanPlayWith(tU32 pMaximum_required, tU32 *pAmount_allocated) {
    void *result;
}

// Offset: 6588
// Size: 200
// EAX: pBuffer
// EDX: pBuffer_size
void PDAllocateActionReplayBuffer(char **pBuffer, tU32 *pBuffer_size) {
    tU32 lba;
    tU32 required;
}

// Offset: 6788
// Size: 35
// EAX: pBuffer
void PDDisposeActionReplayBuffer(char *pBuffer) {
}

// Offset: 6824
// Size: 146
// EAX: pProgpath
void Usage(char *pProgpath) {
    char basename[9];
}

// // Offset: 6972
// // Size: 722
// // EAX: pArgc
// // EDX: pArgv
// int main(int pArgc, char **pArgv) {
//     int arg;
//     int i;
//     float f;
// }

// Offset: 7696
// Size: 62
int OurGetChar() {
}

// Offset: 7760
// Size: 34
int PDGetGorePassword() {
    int len;
    int chances;
    char password[17];
}

// Offset: 7796
// Size: 73
// EAX: pGory
void PDDisplayGoreworthiness(int pGory) {
    tU32 delay_start;
}

// Offset: 7872
// Size: 35
// EAX: pStr
void PDEnterDebugger(char *pStr) {
    unsigned char *save_it;
}

// Offset: 7908
// Size: 24
void PDEndItAllAndReRunTheBastard() {
}

// Offset: 7932
// Size: 57
// EAX: err
int matherr(struct exception_ *err) {
}

// Offset: 7992
// Size: 127
// EAX: limit
int LoopLimitTooLow(tU32 limit) {
    clock_t start;
    tU32 count;
    tU32 val;
}

// Offset: 8120
// Size: 131
tS32 UpperLoopLimit() {
}

// Offset: 8252
// Size: 65
int InitJoysticks() {
}

// Offset: 8320
// Size: 182
// EAX: pBit
tU32 ReadJoystickAxis(int pBit) {
    tU32 val;
    tU32 count;
}

// Offset: 8504
// Size: 406
void PDReadJoySticks() {
    tU32 temp1y;
    tU32 temp2x;
    tU32 temp2y;
}

// Offset: 8912
// Size: 123
tS32 PDGetJoy1X() {
}

// Offset: 9036
// Size: 123
tS32 PDGetJoy1Y() {
}

// Offset: 9160
// Size: 123
tS32 PDGetJoy2X() {
}

// Offset: 9284
// Size: 123
tS32 PDGetJoy2Y() {
}

// Offset: 9408
// Size: 66
int PDGetJoy1Button1() {
}

// Offset: 9476
// Size: 66
int PDGetJoy1Button2() {
}

// Offset: 9544
// Size: 34
int PDGetJoy1Button3() {
}

// Offset: 9580
// Size: 34
int PDGetJoy1Button4() {
}

// Offset: 9616
// Size: 66
int PDGetJoy2Button1() {
}

// Offset: 9684
// Size: 66
int PDGetJoy2Button2() {
}

// Offset: 9752
// Size: 34
int PDGetJoy2Button3() {
}

// Offset: 9788
// Size: 34
int PDGetJoy2Button4() {
}

// Offset: 9824
// Size: 51
// EAX: pThe_path
int PDFileUnlock(char *pThe_path) {
    unsigned int attr;
}

// Offset: 9876
// Size: 33
void CriticalISR(INTPACK pRegs) {
}

// Offset: 9912
// Size: 269
// EAX: pThe_path
// EDX: pFile_name
// EBX: pMin_size
int PDCheckDriveExists2(char *pThe_path, char *pFile_name, tU32 pMin_size) {
    struct stat buf;
    void (*old_critical_isr)();
    int stat_failed;
    char slasher[4];
    char the_path[256];
}

// Offset: 10184
// Size: 108
int PDDoWeLeadAnAustereExistance() {
}

