#ifndef _DISPLAYS_H_
#define _DISPLAYS_H_

#include "dr_types.h"

extern int gLast_fancy_index;
extern int gLast_credit_headup__displays; // suffix added to avoid duplicate symbol
extern int gLast_time_credit_headup;
extern tDR_font* gCached_font;
extern br_font* gBR_fonts[4];
extern tQueued_headup gQueued_headups[4];
extern int gOld_times[10];
extern int gLast_fancy_headup;
extern tU32 gLast_time_earn_time;
extern tU32 gLast_centre_headup;
extern tU32 gLast_fancy_time;
extern int gQueued_headup_count;
extern tU32 gLast_earn_time;
extern tU32 gLast_time_credit_amount;
extern int gLast_credit_amount;
extern tHeadup gHeadups[15];
extern int gLaps_headup;
extern int gCar_kill_count_headup;
extern int gTimer_headup;
extern int gTime_awarded_headup;
extern int gPed_kill_count_headup;
extern int gDim_amount;
extern br_pixelmap* gHeadup_images[32];
extern int gNet_cash_headup;
extern int gNet_ped_headup;
extern int gCredits_lost_headup;
extern int gCredits_won_headup;

void GetTimerString(char* pStr, int pFudge_colon);

void InitHeadups(void);

void ClearHeadup(int pIndex);

void ClearHeadupSlot(int pSlot_index);

void ClearHeadups(void);

int HeadupActive(int pIndex);

void DRPixelmapText(br_pixelmap* pPixelmap, int pX, int pY, tDR_font* pFont, char* pText, int pRight_edge);

void DRPixelmapCleverText2(br_pixelmap* pPixelmap, int pX, int pY, tDR_font* pFont, char* pText, int pRight_edge);

void DeviouslyDimRectangle(br_pixelmap* pPixelmap, int pLeft, int pTop, int pRight, int pBottom, int pKnock_out_corners);

void DimRectangle(br_pixelmap* pPixelmap, int pLeft, int pTop, int pRight, int pBottom, int pKnock_out_corners);

void DimAFewBits(void);

void KillOldestQueuedHeadup(void);

void DubreyBar(int pX_index, int pY, int pColour);

void DoPSPowerHeadup(int pY, int pLevel, char* pName, int pBar_colour);

void DoPSPowerupHeadups(void);

void DoHeadups(tU32 pThe_time);

int FindAHeadupHoleWoofBarkSoundsABitRude(int pSlot_index);

int DRTextWidth(tDR_font* pFont, char* pText);

int DRTextCleverWidth(tDR_font* pFont, signed char* pText);

void DRPixelmapCentredText(br_pixelmap* pPixelmap, int pX, int pY, tDR_font* pFont, char* pText);

int IsHeadupTextClever(signed char* pText);

int MungeHeadupWidth(tHeadup* pHeadup);

int NewTextHeadupSlot2(int pSlot_index, int pFlash_rate, int pLifetime, int pFont_index, char* pText, int pQueue_it);

int NewTextHeadupSlot(int pSlot_index, int pFlash_rate, int pLifetime, int pFont_index, char* pText);

int NewImageHeadupSlot(int pSlot_index, int pFlash_rate, int pLifetime, int pImage_index);

void DoFancyHeadup(int pIndex);

void AdjustHeadups(void);

void MoveHeadupTo(int pHeadup_index, int pNew_x, int pNew_y);

void ChangeHeadupText(int pHeadup_index, char* pNew_text);

void ChangeHeadupImage(int pHeadup_index, int pNew_image);

void ChangeHeadupColour(int pHeadup_index, int pNew_colour);

void DoDamageScreen(tU32 pThe_time);

void PoshDrawLine(float pAngle, br_pixelmap* pDestn, int pX1, int pY1, int pX2, int pY2, int pColour);

void DoInstruments(tU32 pThe_time);

void DoSteeringWheel(tU32 pThe_time);

void ChangingView(void);

void EarnCredits2(int pAmount, char* pPrefix_text);

void EarnCredits(int pAmount);

int SpendCredits(int pAmount);

void AwardTime(tU32 pTime);

void DrawRectangle(br_pixelmap* pPixelmap, int pLeft, int pTop, int pRight, int pBottom, int pColour);

void DrawRRectangle(br_pixelmap* pPixelmap, int pLeft, int pTop, int pRight, int pBottom, int pColour);

void OoerrIveGotTextInMeBoxMissus(int pFont_index, char* pText, br_pixelmap* pPixelmap, int pLeft, int pTop, int pRight, int pBottom, int pCentred);

void TransBrPixelmapText(br_pixelmap* pPixelmap, int pX, int pY, br_uint_32 pColour, br_font* pFont, char* pText);

void TransDRPixelmapText(br_pixelmap* pPixelmap, int pX, int pY, tDR_font* pFont, char* pText, int pRight_edge);

void TransDRPixelmapCleverText(br_pixelmap* pPixelmap, int pX, int pY, tDR_font* pFont, char* pText, int pRight_edge);

#endif
