#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "br_types.h"
#include "dr_types.h"

extern char* gMisc_strings[250];
extern int gEncryption_method;

// Offset: 0
// Size: 144
int CheckQuit();

// Offset: 196
// Size: 542
// EAX: pS
void EncodeLine(char* pS);

// Offset: 740
// Size: 73
// EAX: pA
// EDX: pB
int IRandomBetween(int pA, int pB);

// Offset: 816
// Size: 79
// EAX: pC
int PercentageChance(int pC);

// Offset: 896
// Size: 56
// EAX: pN
int IRandomPosNeg(int pN);

// Offset: 952
// Size: 72
float FRandomBetween(float pA, float pB);

// Offset: 1024
// Size: 69
float FRandomPosNeg(float pN);

// Offset: 1096
// Size: 67
br_scalar SRandomBetween(br_scalar pA, br_scalar pB);

// Offset: 1164
// Size: 69
br_scalar SRandomPosNeg(br_scalar pN);

// Offset: 1236
// Size: 506
// EAX: pF
// EDX: pS
char* GetALineWithNoPossibleService(FILE* pF, /*unsigned*/ char* pS);

// Offset: 1744
// Size: 60
// EAX: pF
// EDX: pS
char* GetALineAndDontArgue(FILE* pF, char* pS);

// Offset: 1804
// Size: 86
// EAX: pDestn_str
// EDX: pStr_1
// EBX: pStr_2
void PathCat(char* pDestn_str, char* pStr_1, char* pStr_2);

// Offset: 1892
// Size: 96
int Chance(float pChance_per_second, int pPeriod);

// Offset: 1988
// Size: 96
float tandeg(float pAngle);

// Offset: 2084
// Size: 80
// EAX: pF
tU32 GetFileLength(FILE* pF);

// Offset: 2164
// Size: 68
// EAX: pB
int BooleanTo1Or0(int pB);

// Offset: 2232
// Size: 114
// EAX: pType
// EDX: pW
// EBX: pH
// ECX: pPixels
br_pixelmap* DRPixelmapAllocate(br_uint_8 pType, br_uint_16 pW, br_uint_16 pH, void* pPixels, int pFlags);

// Offset: 2348
// Size: 118
// EAX: pPm
// EDX: pX
// EBX: pY
// ECX: pW
br_pixelmap* DRPixelmapAllocateSub(br_pixelmap* pPm, br_uint_16 pX, br_uint_16 pY, br_uint_16 pW, br_uint_16 pH);

// Offset: 2468
// Size: 195
// EAX: pSrc
// EDX: pMatch_type
// EBX: pWidth
// ECX: pHeight
br_pixelmap* DRPixelmapMatchSized(br_pixelmap* pSrc, tU8 pMatch_type, tS32 pWidth, tS32 pHeight);

// Offset: 2664
// Size: 357
// EAX: pDst
// EDX: pSrc
// EBX: pSrc_width
// ECX: pSrc_height
void CopyDoubled8BitTo16BitRectangle(br_pixelmap* pDst, br_pixelmap* pSrc, int pSrc_width, int pSrc_height, int pDst_x, int pDst_y, br_pixelmap* pPalette);

// Offset: 3024
// Size: 262
// EAX: pSrc
// EDX: pWidth
// EBX: pHeight
br_pixelmap* Scale8BitPixelmap(br_pixelmap* pSrc, int pWidth, int pHeight);

// Offset: 3288
// Size: 390
// EAX: pSrc
// EDX: pN
br_pixelmap* Tile8BitPixelmap(br_pixelmap* pSrc, int pN);

// Offset: 3680
// Size: 83
// EAX: pName
// EDX: pList
tException_list FindExceptionInList(char* pName, tException_list pList);

// Offset: 3764
// Size: 492
// EAX: pSrc
br_pixelmap* PurifiedPixelmap(br_pixelmap* pSrc);

// Offset: 4256
// Size: 105
// EAX: pFile_name
br_pixelmap* DRPixelmapLoad(char* pFile_name);

// Offset: 4364
// Size: 174
// EAX: pFile_name
// EDX: pPixelmaps
// EBX: pNum
br_uint_32 DRPixelmapLoadMany(char* pFile_name, br_pixelmap** pPixelmaps, br_uint_16 pNum);

// Offset: 4540
// Size: 67
// EAX: pDelay
void WaitFor(tU32 pDelay);

// Offset: 4608
// Size: 136
// EAX: pActor
// EDX: callback
// EBX: arg
br_uint_32 DRActorEnumRecurse(br_actor* pActor, br_actor_enum_cbfn* callback, void* arg);

// Offset: 4744
// Size: 77
br_uint_32 CompareActorID(br_actor* pActor, void* pArg);

// Offset: 4824
// Size: 60
// EAX: pSearch_root
// EDX: pName
br_actor* DRActorFindRecurse(br_actor* pSearch_root, char* pName);

// Offset: 4884
// Size: 156
// EAX: pActor
// EDX: pMat
// EBX: pCall_back
// ECX: pArg
br_uint_32 DRActorEnumRecurseWithMat(br_actor* pActor, br_material* pMat, br_uint_32 (*pCall_back)(br_actor*, br_material*, void*), void* pArg);

// Offset: 5040
// Size: 188
// EAX: pActor
// EDX: pMatrix
// EBX: pCall_back
// ECX: pArg
br_uint_32 DRActorEnumRecurseWithTrans(br_actor* pActor, br_matrix34* pMatrix, br_uint_32 (*pCall_back)(br_actor*, br_matrix34*, void*), void* pArg);

// Offset: 5228
// Size: 89
// EAX: pNumber
int sign(int pNumber);

// Offset: 5320
// Size: 98
float fsign(float pNumber);

// Offset: 5420
// Size: 210
// EAX: pPrefix
// EDX: pExtension
FILE* OpenUniqueFileB(char* pPrefix, char* pExtension);

// Offset: 5632
// Size: 572
// EAX: pF
void PrintScreenFile(FILE* pF);

// Offset: 6204
// Size: 554
// EAX: pF
void PrintScreenFile16(FILE* pF);

// Offset: 6760
// Size: 95
void PrintScreen();

// Offset: 6856
// Size: 89
tU32 GetTotalTime();

// Offset: 6948
// Size: 51
tU32 GetRaceTime();

// Offset: 7000
// Size: 46
// EAX: pLost_time
void AddLostTime(tU32 pLost_time);

// Offset: 7048
// Size: 208
// EAX: pTime
// EDX: pStr
// EBX: pFudge_colon
// ECX: pForce_colon
void TimerString(tU32 pTime, char* pStr, int pFudge_colon, int pForce_colon);

// Offset: 7256
// Size: 55
// EAX: pIndex
char* GetMiscString(int pIndex);

// Offset: 7312
// Size: 58
// EAX: pIndex
// EDX: pStr
void GetCopyOfMiscString(int pIndex, char* pStr);

// Offset: 7372
// Size: 113
// EAX: pPeriod
// EDX: pLast_change
// EBX: pCurrent_state
int Flash(tU32 pPeriod, tU32* pLast_change, int* pCurrent_state);

// Offset: 7488
// Size: 225
// EAX: pDst
// EDX: pSrc
void MaterialCopy(br_material* pDst, br_material* pSrc);

// Offset: 7716
// Size: 139
// EAX: pColour_1
// EDX: pColour_2
double RGBDifferenceSqr(tRGB_colour* pColour_1, tRGB_colour* pColour_2);

// Offset: 7856
// Size: 200
// EAX: pRGB_colour
// EDX: pPalette
int FindBestMatch(tRGB_colour* pRGB_colour, br_pixelmap* pPalette);

// Offset: 8056
// Size: 232
// EAX: pThe_path
// EDX: pR
// EBX: pG
// ECX: pB
void BuildShadeTablePath(char* pThe_path, int pR, int pG, int pB);

// Offset: 8288
// Size: 80
// EAX: pR
// EDX: pG
// EBX: pB
br_pixelmap* LoadGeneratedShadeTable(int pR, int pG, int pB);

// Offset: 8368
// Size: 79
// EAX: pThe_table
// EDX: pR
// EBX: pG
// ECX: pB
void SaveGeneratedShadeTable(br_pixelmap* pThe_table, int pR, int pG, int pB);

// Offset: 8448
// Size: 106
// EAX: pHeight
// EDX: pPalette
// EBX: pRed_mix
// ECX: pGreen_mix
br_pixelmap* GenerateShadeTable(int pHeight, br_pixelmap* pPalette, int pRed_mix, int pGreen_mix, int pBlue_mix, float pQuarter, float pHalf, float pThree_quarter);

// Offset: 8556
// Size: 761
// EAX: pHeight
// EDX: pPalette
// EBX: pRed_mix
// ECX: pGreen_mix
br_pixelmap* GenerateDarkenedShadeTable(int pHeight, br_pixelmap* pPalette, int pRed_mix, int pGreen_mix, int pBlue_mix, float pQuarter, float pHalf, float pThree_quarter, br_scalar pDarken);

// Offset: 9320
// Size: 92
void PossibleService();

// Offset: 9412
// Size: 192
// EAX: pA
// EDX: pB
// EBX: pC
void DRMatrix34TApplyP(br_vector3* pA, br_vector3* pB, br_matrix34* pC);

// Offset: 9604
// Size: 132
// EAX: pPal
// EDX: pEntry
tU16 PaletteEntry16Bit(br_pixelmap* pPal, int pEntry);

// Offset: 9736
// Size: 210
// EAX: pSrc
br_pixelmap* PaletteOf16Bits(br_pixelmap* pSrc);

// Offset: 9948
// Size: 219
// EAX: pDst
// EDX: pSrc
// EBX: pPalette
void Copy8BitTo16Bit(br_pixelmap* pDst, br_pixelmap* pSrc, br_pixelmap* pPalette);

// Offset: 10168
// Size: 558
// EAX: pDst
// EDX: pDst_x
// EBX: pDst_y
// ECX: pSrc
void Copy8BitTo16BitRectangle(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette);

// Offset: 10728
// Size: 518
// EAX: pDst
// EDX: pDst_x
// EBX: pDst_y
// ECX: pSrc
void Copy8BitTo16BitRectangleWithTransparency(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette);

// Offset: 11248
// Size: 279
// EAX: pDst
// EDX: pDst_x
// EBX: pDst_y
// ECX: pSrc
void Copy8BitToOnscreen16BitRectangleWithTransparency(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette);

// Offset: 11528
// Size: 601
// EAX: pDst
// EDX: pDst_x
// EBX: pDst_y
// ECX: pSrc
void Copy8BitRectangleTo16BitRhombusWithTransparency(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, tX1616 pShear, br_pixelmap* pPalette);

// Offset: 12132
// Size: 173
// EAX: dst
// EDX: dx
// EBX: dy
// ECX: src
void DRPixelmapRectangleCopy(br_pixelmap* dst, br_int_16 dx, br_int_16 dy, br_pixelmap* src, br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h);

// Offset: 12308
// Size: 107
// EAX: dst
// EDX: src
void DRPixelmapCopy(br_pixelmap* dst, br_pixelmap* src);

// Offset: 12416
// Size: 82
// EAX: dst
// EDX: x
// EBX: y
// ECX: w
void DRPixelmapRectangleFill(br_pixelmap* dst, br_int_16 x, br_int_16 y, br_uint_16 w, br_uint_16 h, br_uint_32 colour);

// Offset: 12500
// Size: 156
// EAX: pPoint
// EDX: pNormal
int NormalSideOfPlane(br_vector3* pPoint, br_vector3* pNormal, br_scalar pD);

// Offset: 12656
// Size: 351
// EAX: pMaterial
br_material* DRMaterialClone(br_material* pMaterial);

// Offset: 13008
// Size: 93
// EAX: s
void StripCR(char* s);

// Offset: 13104
// Size: 132
void SubsStringJob(char* pStr, ...);

// Offset: 13236
// Size: 454
// EAX: pS
void DecodeLine2(char* pS);

// Offset: 13692
// Size: 304
// EAX: pS
void EncodeLine2(char* pS);

// Offset: 13996
// Size: 553
// EAX: pThe_path
void EncodeFile(char* pThe_path);

// Offset: 14552
// Size: 513
// EAX: pThe_path
void EncodeFileWrapper(char* pThe_path);

// Offset: 15068
// Size: 72
// EAX: pThe_path
void EncodeAllFilesInDirectory(char* pThe_path);

// Offset: 15140
// Size: 89
// EAX: pF
void SkipNLines(FILE* pF);

// Offset: 15232
// Size: 118
// EAX: p1
// EDX: p2
int DRStricmp(char* p1, char* p2);

// Offset: 15352
// Size: 559
// EAX: pArray
// EDX: pCount
void GlorifyMaterial(br_material** pArray, int pCount);

// Offset: 15912
// Size: 178
// EAX: pArray
// EDX: pN
void WhitenVertexRGB(br_model** pArray, int pN);

// Offset: 16092
// Size: 239
// EAX: pPalette
void NobbleNonzeroBlacks(br_pixelmap* pPalette);

// Offset: 16332
// Size: 55
// EAX: pThe_path
int PDCheckDriveExists(char* pThe_path);

// Offset: 16388
// Size: 102
// EAX: pPrims
int OpacityInPrims(br_token_value* pPrims);

// Offset: 16492
// Size: 97
// EAX: pMaterial
int AlreadyBlended(br_material* pMaterial);

// Offset: 16592
// Size: 148
// EAX: pMaterial
// EDX: pPercent
void BlendifyMaterialTablishly(br_material* pMaterial, int pPercent);

// Offset: 16740
// Size: 110
// EAX: pMaterial
// EDX: pPercent
void BlendifyMaterialPrimitively(br_material* pMaterial, int pPercent);

// Offset: 16852
// Size: 73
// EAX: pMaterial
// EDX: pPercent
void BlendifyMaterial(br_material* pMaterial, int pPercent);

#endif
