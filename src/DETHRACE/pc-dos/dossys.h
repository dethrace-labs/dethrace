#ifndef _DOSSYS_H_
#define _DOSSYS_H_

#include "dr_types.h"
#include "br_types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Offset: 0
// Size: 291
void KeyboardHandler();

// Offset: 292
// Size: 71
// EAX: pScan_code
int KeyDown(tU8 pScan_code);

// Offset: 364
// Size: 71
// EAX: pKey_index
// EDX: pScan_code_1
// EBX: pScan_code_2
void KeyTranslation(tU8 pKey_index, tU8 pScan_code_1, tU8 pScan_code_2);

// Offset: 436
// Size: 1897
void KeyBegin();

// Offset: 2336
// Size: 52
void KeyEnd();

// Offset: 2388
// Size: 79
// EAX: pKey_index
int KeyDown22(int pKey_index);

// Offset: 2468
// Size: 141
// EAX: pKeys
// EDX: pMark
void PDSetKeyArray(int *pKeys, int pMark);

// Offset: 2612
// Size: 73
// EAX: pKey
int PDGetASCIIFromKey(int pKey);

// Offset: 2688
// Size: 174
// EAX: pThe_str
void PDFatalError(char *pThe_str);

// Offset: 2864
// Size: 55
// EAX: pThe_str
void PDNonFatalError(char *pThe_str);

// Offset: 2920
// Size: 190
void PDInitialiseSystem();

// Offset: 3112
// Size: 48
void PDShutdownSystem();

// Offset: 3160
// Size: 24
void PDSaveOriginalPalette();

// Offset: 3184
// Size: 24
void PDRevertPalette();

// Offset: 3208
// Size: 90
// EAX: pArgc
// EDX: pArgv
int PDInitScreenVars(int pArgc, char **pArgv);

// Offset: 3300
// Size: 24
void PDInitScreen();

// Offset: 3324
// Size: 113
void PDLockRealBackScreen();

// Offset: 3440
// Size: 48
void PDUnlockRealBackScreen();

// Offset: 3488
// Size: 431
void PDAllocateScreenAndBack();

// Offset: 3920
// Size: 209
// EAX: pDst
// EDX: pSrc
// EBX: pPalette
void Copy8BitTo16BitPixelmap(br_pixelmap *pDst, br_pixelmap *pSrc, br_pixelmap *pPalette);

// Offset: 4132
// Size: 307
// EAX: pDst
// EDX: pSrc
// EBX: pPalette
// ECX: pOff
void Double8BitTo16BitPixelmap(br_pixelmap *pDst, br_pixelmap *pSrc, br_pixelmap *pPalette, tU16 pOff, tU16 pSrc_width, tU16 pSrc_height);

// Offset: 4440
// Size: 35
br_pixelmap* PDInterfacePixelmap();

// Offset: 4476
// Size: 154
void SwapBackScreen();

// Offset: 4632
// Size: 253
// EAX: pRendering_area_only
// EDX: pClear_top_and_bottom
void ReallyCopyBackScreen(int pRendering_area_only, int pClear_top_and_bottom);

// Offset: 4888
// Size: 40
// EAX: pRendering_area_only
void CopyBackScreen(int pRendering_area_only);

// Offset: 4928
// Size: 92
// EAX: pRendering_area_only
void PDScreenBufferSwap(int pRendering_area_only);

// Offset: 5020
// Size: 82
// EAX: dst
// EDX: dx
// EBX: dy
// ECX: src
void PDPixelmapToScreenRectangleCopy(br_pixelmap *dst, br_int_16 dx, br_int_16 dy, br_pixelmap *src, br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h);

// Offset: 5104
// Size: 68
// EAX: dst
// EDX: x1
// EBX: y1
// ECX: x2
void PDPixelmapHLineOnScreen(br_pixelmap *dst, br_int_16 x1, br_int_16 y1, br_int_16 x2, br_int_16 y2, br_uint_32 colour);

// Offset: 5172
// Size: 68
// EAX: dst
// EDX: x1
// EBX: y1
// ECX: x2
void PDPixelmapVLineOnScreen(br_pixelmap *dst, br_int_16 x1, br_int_16 y1, br_int_16 x2, br_int_16 y2, br_uint_32 colour);

// Offset: 5240
// Size: 24
void PDInstallErrorHandlers();

// Offset: 5264
// Size: 39
void PDSetFileVariables();

// Offset: 5304
// Size: 98
// EAX: pThe_path
void PDBuildAppPath(char *pThe_path);

// Offset: 5404
// Size: 133
// EAX: pThe_path
// EDX: pAction_routine
void PDForEveryFile(char *pThe_path, void (*pAction_routine)(char*));

// Offset: 5540
// Size: 39
// EAX: pThe_palette
void PDSetPalette(br_pixelmap *pThe_palette);

// Offset: 5580
// Size: 136
// EAX: pPalette
// EDX: pFirst_colour
// EBX: pCount
void PDSetPaletteEntries(br_pixelmap *pPalette, int pFirst_colour, int pCount);

// Offset: 5716
// Size: 44
void PDSwitchToRealResolution();

// Offset: 5760
// Size: 44
void PDSwitchToLoresMode();

// Offset: 5804
// Size: 86
// EAX: pButton_1
// EDX: pButton_2
void PDMouseButtons(int *pButton_1, int *pButton_2);

// Offset: 5892
// Size: 380
// EAX: pX_coord
// EDX: pY_coord
void PDGetMousePosition(int *pX_coord, int *pY_coord);

// Offset: 6272
// Size: 38
int PDGetTotalTime();

// Offset: 6312
// Size: 37
// EAX: pTime_since_last_call
int PDServiceSystem(tU32 pTime_since_last_call);

// Offset: 6352
// Size: 121
tU32 LargestBlockAvail();

// Offset: 6476
// Size: 111
// EAX: pMaximum_required
// EDX: pAmount_allocated
void* PDGrabLargestMammaryWeCanPlayWith(tU32 pMaximum_required, tU32 *pAmount_allocated);

// Offset: 6588
// Size: 200
// EAX: pBuffer
// EDX: pBuffer_size
void PDAllocateActionReplayBuffer(char **pBuffer, tU32 *pBuffer_size);

// Offset: 6788
// Size: 35
// EAX: pBuffer
void PDDisposeActionReplayBuffer(char *pBuffer);

// Offset: 6824
// Size: 146
// EAX: pProgpath
void Usage(char *pProgpath);

// Offset: 6972
// Size: 722
// EAX: pArgc
// EDX: pArgv
int main(int pArgc, char **pArgv);

// Offset: 7696
// Size: 62
int OurGetChar();

// Offset: 7760
// Size: 34
int PDGetGorePassword();

// Offset: 7796
// Size: 73
// EAX: pGory
void PDDisplayGoreworthiness(int pGory);

// Offset: 7872
// Size: 35
// EAX: pStr
void PDEnterDebugger(char *pStr);

// Offset: 7908
// Size: 24
void PDEndItAllAndReRunTheBastard();

// Offset: 7932
// Size: 57
// EAX: err
int matherr(struct exception_ *err);

// Offset: 7992
// Size: 127
// EAX: limit
int LoopLimitTooLow(tU32 limit);

// Offset: 8120
// Size: 131
tS32 UpperLoopLimit();

// Offset: 8252
// Size: 65
int InitJoysticks();

// Offset: 8320
// Size: 182
// EAX: pBit
tU32 ReadJoystickAxis(int pBit);

// Offset: 8504
// Size: 406
void PDReadJoySticks();

// Offset: 8912
// Size: 123
tS32 PDGetJoy1X();

// Offset: 9036
// Size: 123
tS32 PDGetJoy1Y();

// Offset: 9160
// Size: 123
tS32 PDGetJoy2X();

// Offset: 9284
// Size: 123
tS32 PDGetJoy2Y();

// Offset: 9408
// Size: 66
int PDGetJoy1Button1();

// Offset: 9476
// Size: 66
int PDGetJoy1Button2();

// Offset: 9544
// Size: 34
int PDGetJoy1Button3();

// Offset: 9580
// Size: 34
int PDGetJoy1Button4();

// Offset: 9616
// Size: 66
int PDGetJoy2Button1();

// Offset: 9684
// Size: 66
int PDGetJoy2Button2();

// Offset: 9752
// Size: 34
int PDGetJoy2Button3();

// Offset: 9788
// Size: 34
int PDGetJoy2Button4();

// Offset: 9824
// Size: 51
// EAX: pThe_path
int PDFileUnlock(char *pThe_path);

// Offset: 9876
// Size: 33
void CriticalISR(INTPACK pRegs);

// Offset: 9912
// Size: 269
// EAX: pThe_path
// EDX: pFile_name
// EBX: pMin_size
int PDCheckDriveExists2(char *pThe_path, char *pFile_name, tU32 pMin_size);

// Offset: 10184
// Size: 108
int PDDoWeLeadAnAustereExistance();

#endif
