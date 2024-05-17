#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "dr_types.h"

extern int gIn_check_quit;
extern tU32 gLost_time;
extern tU32 gLong_key[4];
extern tU32 gOther_long_key[4];
extern int gEncryption_method;
extern char* gMisc_strings[250];
extern br_pixelmap* g16bit_palette;
extern br_pixelmap* gSource_for_16bit_palette;

typedef br_uint_32 recurse_with_mat_cbfn(br_actor*, br_material*, void*);

int CheckQuit(void);

double sqr(double pN);

void EncodeLine(char* pS);

int IRandomBetween(int pA, int pB);

int PercentageChance(int pC);

int IRandomPosNeg(int pN);

float FRandomBetween(float pA, float pB);

float FRandomPosNeg(float pN);

br_scalar SRandomBetween(br_scalar pA, br_scalar pB);

br_scalar SRandomPosNeg(br_scalar pN);

char* GetALineWithNoPossibleService(FILE* pF, unsigned char* pS);

char* GetALineAndDontArgue(FILE* pF, char* pS);

void PathCat(char* pDestn_str, char* pStr_1, char* pStr_2);

int Chance(float pChance_per_second, int pPeriod);

float tandeg(float pAngle);

tU32 GetFileLength(FILE* pF);

int BooleanTo1Or0(int pB);

br_pixelmap* DRPixelmapAllocate(br_uint_8 pType, br_uint_16 pW, br_uint_16 pH, void* pPixels, int pFlags);

br_pixelmap* DRPixelmapAllocateSub(br_pixelmap* pPm, br_uint_16 pX, br_uint_16 pY, br_uint_16 pW, br_uint_16 pH);

br_pixelmap* DRPixelmapMatchSized(br_pixelmap* pSrc, tU8 pMatch_type, tS32 pWidth, tS32 pHeight);

void CopyDoubled8BitTo16BitRectangle(br_pixelmap* pDst, br_pixelmap* pSrc, int pSrc_width, int pSrc_height, int pDst_x, int pDst_y, br_pixelmap* pPalette);

br_pixelmap* Scale8BitPixelmap(br_pixelmap* pSrc, int pWidth, int pHeight);

br_pixelmap* Tile8BitPixelmap(br_pixelmap* pSrc, int pN);

tException_list FindExceptionInList(char* pName, tException_list pList);

br_pixelmap* PurifiedPixelmap(br_pixelmap* pSrc);

br_pixelmap* DRPixelmapLoad(char* pFile_name);

br_uint_32 DRPixelmapLoadMany(char* pFile_name, br_pixelmap** pPixelmaps, br_uint_16 pNum);

void WaitFor(tU32 pDelay);

br_uintptr_t DRActorEnumRecurse(br_actor* pActor, br_actor_enum_cbfn* callback, void* arg);

br_uintptr_t CompareActorID(br_actor* pActor, void* pArg);

br_actor* DRActorFindRecurse(br_actor* pSearch_root, char* pName);

br_uint_32 DRActorEnumRecurseWithMat(br_actor* pActor, br_material* pMat, br_uint_32 (*pCall_back)(br_actor*, br_material*, void*), void* pArg);

br_uint_32 DRActorEnumRecurseWithTrans(br_actor* pActor, br_matrix34* pMatrix, br_uint_32 (*pCall_back)(br_actor*, br_matrix34*, void*), void* pArg);

int sign(int pNumber);

float fsign(float pNumber);

FILE* OpenUniqueFileB(char* pPrefix, char* pExtension);

void PrintScreenFile(FILE* pF);

void PrintScreenFile16(FILE* pF);

void PrintScreen(void);

tU32 GetTotalTime(void);

tU32 GetRaceTime(void);

void AddLostTime(tU32 pLost_time);

void TimerString(tU32 pTime, char* pStr, int pFudge_colon, int pForce_colon);

char* GetMiscString(int pIndex);

void GetCopyOfMiscString(int pIndex, char* pStr);

int Flash(tU32 pPeriod, tU32* pLast_change, int* pCurrent_state);

void MaterialCopy(br_material* pDst, br_material* pSrc);

double RGBDifferenceSqr(tRGB_colour* pColour_1, tRGB_colour* pColour_2);

int FindBestMatch(tRGB_colour* pRGB_colour, br_pixelmap* pPalette);

void BuildShadeTablePath(char* pThe_path, int pR, int pG, int pB);

br_pixelmap* LoadGeneratedShadeTable(int pR, int pG, int pB);

void SaveGeneratedShadeTable(br_pixelmap* pThe_table, int pR, int pG, int pB);

br_pixelmap* GenerateShadeTable(int pHeight, br_pixelmap* pPalette, int pRed_mix, int pGreen_mix, int pBlue_mix, float pQuarter, float pHalf, float pThree_quarter);

br_pixelmap* GenerateDarkenedShadeTable(int pHeight, br_pixelmap* pPalette, int pRed_mix, int pGreen_mix, int pBlue_mix, float pQuarter, float pHalf, float pThree_quarter, br_scalar pDarken);

void PossibleService(void);

void DRMatrix34TApplyP(br_vector3* pA, br_vector3* pB, br_matrix34* pC);

tU16 PaletteEntry16Bit(br_pixelmap* pPal, int pEntry);

br_pixelmap* PaletteOf16Bits(br_pixelmap* pSrc);

void Copy8BitTo16Bit(br_pixelmap* pDst, br_pixelmap* pSrc, br_pixelmap* pPalette);

void Copy8BitTo16BitRectangle(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette);

void Copy8BitTo16BitRectangleWithTransparency(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette);

void Copy8BitToOnscreen16BitRectangleWithTransparency(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, br_pixelmap* pPalette);

void Copy8BitRectangleTo16BitRhombusWithTransparency(br_pixelmap* pDst, tS16 pDst_x, tS16 pDst_y, br_pixelmap* pSrc, tS16 pSrc_x, tS16 pSrc_y, tS16 pWidth, tS16 pHeight, tX1616 pShear, br_pixelmap* pPalette);

void DRPixelmapRectangleCopy(br_pixelmap* dst, br_int_16 dx, br_int_16 dy, br_pixelmap* src, br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h);

void DRPixelmapCopy(br_pixelmap* dst, br_pixelmap* src);

void DRPixelmapRectangleFill(br_pixelmap* dst, br_int_16 x, br_int_16 y, br_uint_16 w, br_uint_16 h, br_uint_32 colour);

int NormalSideOfPlane(br_vector3* pPoint, br_vector3* pNormal, br_scalar pD);

br_material* DRMaterialClone(br_material* pMaterial);

void StripCR(char* s);

void SubsStringJob(char* pStr, ...);

void DecodeLine2(char* pS);

void EncodeLine2(char* pS);

void EncodeFile(char* pThe_path);

void EncodeFileWrapper(char* pThe_path);

void EncodeAllFilesInDirectory(char* pThe_path);

void SkipNLines(FILE* pF);

int DRStricmp(char* p1, char* p2);

void GlorifyMaterial(br_material** pArray, int pCount);

void WhitenVertexRGB(br_model** pArray, int pN);

void NobbleNonzeroBlacks(br_pixelmap* pPalette);

int PDCheckDriveExists(char* pThe_path);

int OpacityInPrims(br_token_value* pPrims);

int AlreadyBlended(br_material* pMaterial);

void BlendifyMaterialTablishly(br_material* pMaterial, int pPercent);

void BlendifyMaterialPrimitively(br_material* pMaterial, int pPercent);

void BlendifyMaterial(br_material* pMaterial, int pPercent);

#endif
