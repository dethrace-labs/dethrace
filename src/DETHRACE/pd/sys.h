#ifndef _PD_SYS_H_
#define _PD_SYS_H_

// Jeff:
// Header files are generated from information in the original symbol dump, but we don't exactly know what this used to look like or be called.
// Each platform build included a platform-dependant `sys` and `net` files. From the symbol dump and debug/error messages in the binaries,
// we know of at least `pc-dos/dossys.c`, `pc-dos/dosnet.c`, `Win95sys.c`.
// Functions contained within these files are prefixed with `PD` - we assume that is short for something like `Platform Dependant`.

#include "dr_types.h"

#include "harness/compiler.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int gDOSGfx_initialized;

// extern int gExtra_mem;
// extern int gReplay_override;
extern tGraf_spec gGraf_specs[2];
// extern int gASCII_table[128];
// extern tU32 gKeyboard_bits[8];
// extern int gASCII_shift_table[128];
// extern char gNetwork_profile_fname[256];
extern tS32 gJoystick_min1y;
extern tS32 gJoystick_min2y;
extern tS32 gJoystick_min2x;
extern tS32 gRaw_joystick2y;
extern tS32 gRaw_joystick2x;
extern tS32 gRaw_joystick1y;
extern tS32 gRaw_joystick1x;
extern tS32 gJoystick_range2y;
extern tS32 gJoystick_range2x;
extern tS32 gJoystick_range1y;
extern tS32 gJoystick_range1x;
// extern int gNo_voodoo;
// extern int gSwitched_resolution;
extern br_pixelmap* gReal_back_screen;
extern tS32 gJoystick_min1x;
// extern br_pixelmap* gTemp_screen;
// extern tU32 gUpper_loop_limit;
// extern int gReal_back_screen_locked;
// extern void (*gPrev_keyboard_handler)(void);

// #ifdef __DOS__
// extern tU8 gScan_code[123][2];
// #else
// extern tU32 gScan_code[123];
// #endif

extern char* _unittest_last_fatal_error;

// void KeyboardHandler(void);

// int KeyDown(tU8 pScan_code);

// void KeyTranslation(tU8 pKey_index, tU8 pScan_code_1, tU8 pScan_code_2);

// void KeyBegin(void);

// void KeyEnd(void);

// int KeyDown22(int pKey_index);

void PDSetKeyArray(int* pKeys, int pMark);

int PDGetASCIIFromKey(int pKey);

void PDFatalError(char* pThe_str);

void PDNonFatalError(char* pThe_str);

void PDInitialiseSystem(void);

void PDShutdownSystem(void);

void PDSaveOriginalPalette(void);

void PDRevertPalette(void);

int PDInitScreenVars(int pArgc, char** pArgv);

void PDInitScreen(void);

void PDLockRealBackScreen(int lock);

void PDUnlockRealBackScreen(int lock);

void PDAllocateScreenAndBack(void);

// void Copy8BitTo16BitPixelmap(br_pixelmap* pDst, br_pixelmap* pSrc, br_pixelmap* pPalette);

// void Double8BitTo16BitPixelmap(br_pixelmap* pDst, br_pixelmap* pSrc, br_pixelmap* pPalette, tU16 pOff, tU16 pSrc_width, tU16 pSrc_height);

br_pixelmap* PDInterfacePixelmap(void);

// void SwapBackScreen(void);

// void ReallyCopyBackScreen(int pRendering_area_only, int pClear_top_and_bottom);

void CopyBackScreen(int pRendering_area_only);

void PDScreenBufferSwap(int pRendering_area_only);

void PDPixelmapToScreenRectangleCopy(br_pixelmap* dst, br_int_16 dx, br_int_16 dy, br_pixelmap* src, br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h);

void PDPixelmapHLineOnScreen(br_pixelmap* dst, br_int_16 x1, br_int_16 y1, br_int_16 x2, br_int_16 y2, br_uint_32 colour);

void PDPixelmapVLineOnScreen(br_pixelmap* dst, br_int_16 x1, br_int_16 y1, br_int_16 x2, br_int_16 y2, br_uint_32 colour);

void PDInstallErrorHandlers(void);

void PDSetFileVariables(void);

void PDBuildAppPath(char* pThe_path);

void PDForEveryFile(char* pThe_path, void (*pAction_routine)(char*));

void PDSetPalette(br_pixelmap* pThe_palette);

void PDSetPaletteEntries(br_pixelmap* pPalette, int pFirst_colour, int pCount);

void PDSwitchToRealResolution(void);

void PDSwitchToLoresMode(void);

void PDMouseButtons(int* pButton_1, int* pButton_2);

void PDGetMousePosition(int* pX_coord, int* pY_coord);

int PDGetTotalTime(void);

int PDServiceSystem(tU32 pTime_since_last_call);

// tU32 LargestBlockAvail(void);

void* PDGrabLargestMammaryWeCanPlayWith(tU32 pMaximum_required, tU32* pAmount_allocated);

void PDAllocateActionReplayBuffer(char** pBuffer, tU32* pBuffer_size);

void PDDisposeActionReplayBuffer(char* pBuffer);

// void Usage(char* pProgpath);

// int OurGetChar(void);

int PDGetGorePassword(void);

void PDDisplayGoreworthiness(int pGory);

void PDEnterDebugger(char* pStr);

// Added function
br_material* PDMissingMaterial(char* name);

// Added function
br_pixelmap* PDMissingTable(char* name);

// Added function
br_model* PDMissingModel(char* name);

// Added function
br_pixelmap* PDMissingMap(char* name);

void PDEndItAllAndReRunTheBastard(void);

// int matherr(struct exception_* err);

// int LoopLimitTooLow(tU32 limit);

// tS32 UpperLoopLimit(void);

// int InitJoysticks(void);

// tU32 ReadJoystickAxis(int pBit);

void PDReadJoySticks(void);

tS32 PDGetJoy1X(void);

tS32 PDGetJoy1Y(void);

tS32 PDGetJoy2X(void);

tS32 PDGetJoy2Y(void);

int PDGetJoy1Button1(void);

int PDGetJoy1Button2(void);

int PDGetJoy1Button3(void);

int PDGetJoy1Button4(void);

int PDGetJoy2Button1(void);

int PDGetJoy2Button2(void);

int PDGetJoy2Button3(void);

int PDGetJoy2Button4(void);

int PDFileUnlock(char* pThe_path);

// void CriticalISR(INTPACK pRegs);

int PDCheckDriveExists2(char* pThe_path, char* pFile_name, tU32 pMin_size);

int PDDoWeLeadAnAustereExistance(void);

#endif
