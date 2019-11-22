#ifndef _DISPLAYS_H_
#define _DISPLAYS_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 0
// Size: 56
// EAX: pStr
// EDX: pFudge_colon
void GetTimerString(char *pStr, int pFudge_colon);

// Offset: 56
// Size: 116
void InitHeadups();

// Offset: 172
// Size: 54
// EAX: pIndex
void ClearHeadup(int pIndex);

// Offset: 228
// Size: 110
// EAX: pSlot_index
void ClearHeadupSlot(int pSlot_index);

// Offset: 340
// Size: 204
void ClearHeadups();

// Offset: 544
// Size: 78
// EAX: pIndex
int HeadupActive(int pIndex);

// Offset: 624
// Size: 406
// EAX: pPixelmap
// EDX: pX
// EBX: pY
// ECX: pFont
void DRPixelmapText(br_pixelmap *pPixelmap, int pX, int pY, tDR_font *pFont, char *pText, int pRight_edge);

// Offset: 1032
// Size: 550
// EAX: pPixelmap
// EDX: pX
// EBX: pY
// ECX: pFont
void DRPixelmapCleverText2(br_pixelmap *pPixelmap, int pX, int pY, tDR_font *pFont, signed char *pText, int pRight_edge);

// Offset: 1584
// Size: 248
// EAX: pPixelmap
// EDX: pLeft
// EBX: pTop
// ECX: pRight
void DeviouslyDimRectangle(br_pixelmap *pPixelmap, int pLeft, int pTop, int pRight, int pBottom, int pKnock_out_corners);

// Offset: 1832
// Size: 461
void DimRectangle(br_pixelmap *pPixelmap, int pLeft, int pTop, int pRight, int pBottom, int pKnock_out_corners);

// Offset: 2296
// Size: 352
void DimAFewBits();

// Offset: 2648
// Size: 65
void KillOldestQueuedHeadup();

// Offset: 2716
// Size: 120
// EAX: pX_index
// EDX: pY
// EBX: pColour
void DubreyBar(int pX_index, int pY, int pColour);

// Offset: 2836
// Size: 329
// EAX: pY
// EDX: pLevel
// EBX: pName
// ECX: pBar_colour
void DoPSPowerHeadup(int pY, int pLevel, char *pName, int pBar_colour);

// Offset: 3168
// Size: 163
void DoPSPowerupHeadups();

// Offset: 3332
// Size: 2340
// EAX: pThe_time
void DoHeadups(tU32 pThe_time);

// Offset: 5672
// Size: 136
// EAX: pSlot_index
int FindAHeadupHoleWoofBarkSoundsABitRude(int pSlot_index);

// Offset: 5808
// Size: 144
// EAX: pFont
// EDX: pText
int DRTextWidth(tDR_font *pFont, char *pText);

// Offset: 5952
// Size: 206
// EAX: pFont
// EDX: pText
int DRTextCleverWidth(tDR_font *pFont, signed char *pText);

// Offset: 6160
// Size: 98
// EAX: pPixelmap
// EDX: pX
// EBX: pY
// ECX: pFont
void DRPixelmapCentredText(br_pixelmap *pPixelmap, int pX, int pY, tDR_font *pFont, char *pText);

// Offset: 6260
// Size: 77
// EAX: pText
int IsHeadupTextClever(signed char *pText);

// Offset: 6340
// Size: 562
// EAX: pHeadup
int MungeHeadupWidth(tHeadup *pHeadup);

// Offset: 6904
// Size: 605
// EAX: pSlot_index
// EDX: pFlash_rate
// EBX: pLifetime
// ECX: pFont_index
int NewTextHeadupSlot2(int pSlot_index, int pFlash_rate, int pLifetime, int pFont_index, char *pText, int pQueue_it);

// Offset: 7512
// Size: 71
// EAX: pSlot_index
// EDX: pFlash_rate
// EBX: pLifetime
// ECX: pFont_index
int NewTextHeadupSlot(int pSlot_index, int pFlash_rate, int pLifetime, int pFont_index, char *pText);

// Offset: 7584
// Size: 496
// EAX: pSlot_index
// EDX: pFlash_rate
// EBX: pLifetime
// ECX: pImage_index
int NewImageHeadupSlot(int pSlot_index, int pFlash_rate, int pLifetime, int pImage_index);

// Offset: 8080
// Size: 284
// EAX: pIndex
void DoFancyHeadup(int pIndex);

// Offset: 8364
// Size: 407
void AdjustHeadups();

// Offset: 8772
// Size: 107
// EAX: pHeadup_index
// EDX: pNew_x
// EBX: pNew_y
void MoveHeadupTo(int pHeadup_index, int pNew_x, int pNew_y);

// Offset: 8880
// Size: 83
// EAX: pHeadup_index
// EDX: pNew_text
void ChangeHeadupText(int pHeadup_index, char *pNew_text);

// Offset: 8964
// Size: 199
// EAX: pHeadup_index
// EDX: pNew_image
void ChangeHeadupImage(int pHeadup_index, int pNew_image);

// Offset: 9164
// Size: 69
// EAX: pHeadup_index
// EDX: pNew_colour
void ChangeHeadupColour(int pHeadup_index, int pNew_colour);

// Offset: 9236
// Size: 427
// EAX: pThe_time
void DoDamageScreen(tU32 pThe_time);

// Offset: 9664
// Size: 404
void PoshDrawLine(float pAngle, br_pixelmap *pDestn, int pX1, int pY1, int pX2, int pY2, int pColour);

// Offset: 10068
// Size: 3133
// EAX: pThe_time
void DoInstruments(tU32 pThe_time);

// Offset: 13204
// Size: 386
// EAX: pThe_time
void DoSteeringWheel(tU32 pThe_time);

// Offset: 13592
// Size: 557
void ChangingView();

// Offset: 14152
// Size: 445
// EAX: pAmount
// EDX: pPrefix_text
void EarnCredits2(int pAmount, char *pPrefix_text);

// Offset: 14600
// Size: 50
// EAX: pAmount
void EarnCredits(int pAmount);

// Offset: 14652
// Size: 103
// EAX: pAmount
int SpendCredits(int pAmount);

// Offset: 14756
// Size: 279
// EAX: pTime
void AwardTime(tU32 pTime);

// Offset: 15036
// Size: 170
// EAX: pPixelmap
// EDX: pLeft
// EBX: pTop
// ECX: pRight
void DrawRectangle(br_pixelmap *pPixelmap, int pLeft, int pTop, int pRight, int pBottom, int pColour);

// Offset: 15208
// Size: 178
// EAX: pPixelmap
// EDX: pLeft
// EBX: pTop
// ECX: pRight
void DrawRRectangle(br_pixelmap *pPixelmap, int pLeft, int pTop, int pRight, int pBottom, int pColour);

// Offset: 15388
// Size: 620
// EAX: pFont_index
// EDX: pText
// EBX: pPixelmap
// ECX: pLeft
void OoerrIveGotTextInMeBoxMissus(int pFont_index, char *pText, br_pixelmap *pPixelmap, int pLeft, int pTop, int pRight, int pBottom, int pCentred);

// Offset: 16008
// Size: 117
// EAX: pPixelmap
// EDX: pX
// EBX: pY
// ECX: pColour
void TransBrPixelmapText(br_pixelmap *pPixelmap, int pX, int pY, br_uint_32 pColour, br_font *pFont, signed char *pText);

// Offset: 16128
// Size: 250
// EAX: pPixelmap
// EDX: pX
// EBX: pY
// ECX: pFont
void TransDRPixelmapText(br_pixelmap *pPixelmap, int pX, int pY, tDR_font *pFont, char *pText, int pRight_edge);

// Offset: 16380
// Size: 250
// EAX: pPixelmap
// EDX: pX
// EBX: pY
// ECX: pFont
void TransDRPixelmapCleverText(br_pixelmap *pPixelmap, int pX, int pY, tDR_font *pFont, char *pText, int pRight_edge);

#endif
