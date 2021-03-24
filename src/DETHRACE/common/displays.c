#include "displays.h"
#include "brender.h"
#include "common/flicplay.h"
#include "common/globvars.h"
#include "common/graphics.h"
#include <stdlib.h>

br_font* gBR_fonts[4];
tQueued_headup gQueued_headups[4];
int gOld_times[10];
int gLast_fancy_headup;
tU32 gLast_time_earn_time;
tU32 gLast_centre_headup;
tU32 gLast_fancy_time;
int gQueued_headup_count;
tU32 gLast_earn_time;
tU32 gLast_time_credit_amount;
int gLast_credit_amount;
tDR_font* gCached_font;
int gLast_time_credit_headup;
int gLast_fancy_index;
int gLast_credit_headup;
tHeadup gHeadups[15];
int gLaps_headup;
int gCar_kill_count_headup;
int gTimer_headup;
int gTime_awarded_headup;
int gPed_kill_count_headup;
int gDim_amount;
br_pixelmap* gHeadup_images[31];
int gNet_cash_headup;
int gNet_ped_headup;
int gCredits_lost_headup;
int gCredits_won_headup;

// IDA: void __usercall GetTimerString(char *pStr@<EAX>, int pFudge_colon@<EDX>)
void GetTimerString(char* pStr, int pFudge_colon) {
    LOG_TRACE("(\"%s\", %d)", pStr, pFudge_colon);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitHeadups()
void InitHeadups() {
    int i;
    LOG_TRACE("()");
    for (i = 0; i < 15; i++) {
        gHeadups[i].type = 0;
    }
    gBR_fonts[0] = BrFontProp7x9;
    gBR_fonts[1] = BrFontFixed3x5;
    gBR_fonts[2] = gFont_7;
    gBR_fonts[3] = gHeadup_font;
}

// IDA: void __usercall ClearHeadup(int pIndex@<EAX>)
void ClearHeadup(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ClearHeadupSlot(int pSlot_index@<EAX>)
void ClearHeadupSlot(int pSlot_index) {
    int i;
    tHeadup* the_headup;
    LOG_TRACE("(%d)", pSlot_index);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ClearHeadups()
void ClearHeadups() {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gHeadups); ++i) {
        if (gHeadups[i].type) {
            ClearHeadup(i);
        }
    }
    gLast_fancy_index = -1;
    gLast_credit_headup = -1;
    gLast_time_credit_headup = -1;
    gLast_earn_time = 0;
    gLast_fancy_time = 0;
    gLast_time_earn_time = 0;
    for (i = 0; i < COUNT_OF(gOld_times); i++) {
        gOld_times[i] = 0;
    }
    gQueued_headup_count = 0;
    gLast_centre_headup = 0;
}

// IDA: int __usercall HeadupActive@<EAX>(int pIndex@<EAX>)
int HeadupActive(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRPixelmapText(br_pixelmap *pPixelmap@<EAX>, int pX@<EDX>, int pY@<EBX>, tDR_font *pFont@<ECX>, char *pText, int pRight_edge)
void DRPixelmapText(br_pixelmap* pPixelmap, int pX, int pY, tDR_font* pFont, char* pText, int pRight_edge) {
    int i;
    int x;
    int len;
    int chr;
    int ch_width;
    unsigned char* ch;
    LOG_TRACE9("(%p, %d, %d, %p, \"%s\", %d)", pPixelmap, pX, pY, pFont, pText, pRight_edge);
    len = strlen(pText);
    if (pX >= 0 && pPixelmap->width >= pRight_edge && pY >= 0 && pY + pFont->height <= pPixelmap->height) {
        x = pX;
        for (i = 0; i < len; i++) {
            chr = pText[i] - pFont->offset;
            DRPixelmapRectangleOnscreenCopy(
                gBack_screen,
                x,
                pY,
                pFont->images,
                0,
                pFont->height * chr,
                pFont->width_table[chr],
                pFont->height);

            x += pFont->width_table[chr] + pFont->spacing;
        }
    } else {
        TELL_ME_IF_WE_PASS_THIS_WAY();
        //DRPixelmapRectangleMaskedCopy ...
    }
}

// IDA: void __usercall DRPixelmapCleverText2(br_pixelmap *pPixelmap@<EAX>, int pX@<EDX>, int pY@<EBX>, tDR_font *pFont@<ECX>, signed char *pText, int pRight_edge)
void DRPixelmapCleverText2(br_pixelmap* pPixelmap, int pX, int pY, tDR_font* pFont, signed char* pText, int pRight_edge) {
    int i;
    int x;
    int len;
    int chr;
    int ch_width;
    unsigned char* ch;
    tDR_font* new_font;
    LOG_TRACE("(%p, %d, %d, %p, %p, %d)", pPixelmap, pX, pY, pFont, pText, pRight_edge);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DeviouslyDimRectangle(br_pixelmap *pPixelmap@<EAX>, int pLeft@<EDX>, int pTop@<EBX>, int pRight@<ECX>, int pBottom, int pKnock_out_corners)
void DeviouslyDimRectangle(br_pixelmap* pPixelmap, int pLeft, int pTop, int pRight, int pBottom, int pKnock_out_corners) {
    LOG_TRACE("(%p, %d, %d, %d, %d, %d)", pPixelmap, pLeft, pTop, pRight, pBottom, pKnock_out_corners);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DimRectangle(br_pixelmap *pPixelmap, int pLeft, int pTop, int pRight, int pBottom, int pKnock_out_corners)
void DimRectangle(br_pixelmap* pPixelmap, int pLeft, int pTop, int pRight, int pBottom, int pKnock_out_corners) {
    tU8* ptr;
    tU8* depth_table_ptr;
    tU8* right_ptr;
    int x;
    int y;
    int line_skip;
    int width;
    LOG_TRACE("(%p, %d, %d, %d, %d, %d)", pPixelmap, pLeft, pTop, pRight, pBottom, pKnock_out_corners);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DimAFewBits()
void DimAFewBits() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl KillOldestQueuedHeadup()
void KillOldestQueuedHeadup() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DubreyBar(int pX_index@<EAX>, int pY@<EDX>, int pColour@<EBX>)
void DubreyBar(int pX_index, int pY, int pColour) {
    int x;
    LOG_TRACE("(%d, %d, %d)", pX_index, pY, pColour);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoPSPowerHeadup(int pY@<EAX>, int pLevel@<EDX>, char *pName@<EBX>, int pBar_colour@<ECX>)
void DoPSPowerHeadup(int pY, int pLevel, char* pName, int pBar_colour) {
    char s[16];
    int i;
    LOG_TRACE("(%d, %d, \"%s\", %d)", pY, pLevel, pName, pBar_colour);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoPSPowerupHeadups()
void DoPSPowerupHeadups() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoHeadups(tU32 pThe_time@<EAX>)
void DoHeadups(tU32 pThe_time) {
    int i;
    int x_offset;
    int y_offset;
    tHeadup* the_headup;
    int time_factor;
    LOG_TRACE("(%d)", pThe_time);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FindAHeadupHoleWoofBarkSoundsABitRude@<EAX>(int pSlot_index@<EAX>)
int FindAHeadupHoleWoofBarkSoundsABitRude(int pSlot_index) {
    int i;
    int empty_one;
    tHeadup* the_headup;
    LOG_TRACE("(%d)", pSlot_index);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRTextWidth@<EAX>(tDR_font *pFont@<EAX>, char *pText@<EDX>)
int DRTextWidth(tDR_font* pFont, char* pText) {
    int i;
    int len;
    int result;
    char* c;
    LOG_TRACE("(%p, \"%s\")", pFont, pText);

    c = pText;
    result = 0;
    len = strlen(pText);

    for (i = 0; i < len; i++) {
        result += pFont->width_table[*c - pFont->offset];
        c++;
    }
    return result + pFont->spacing * (len - 1);
}

// IDA: int __usercall DRTextCleverWidth@<EAX>(tDR_font *pFont@<EAX>, signed char *pText@<EDX>)
int DRTextCleverWidth(tDR_font* pFont, signed char* pText) {
    int i;
    int len;
    int result;
    unsigned char* c;
    LOG_TRACE("(%p, %p)", pFont, pText);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRPixelmapCentredText(br_pixelmap *pPixelmap@<EAX>, int pX@<EDX>, int pY@<EBX>, tDR_font *pFont@<ECX>, char *pText)
void DRPixelmapCentredText(br_pixelmap* pPixelmap, int pX, int pY, tDR_font* pFont, char* pText) {
    int width_over_2;
    LOG_TRACE("(%p, %d, %d, %p, \"%s\")", pPixelmap, pX, pY, pFont, pText);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall IsHeadupTextClever@<EAX>(signed char *pText@<EAX>)
int IsHeadupTextClever(signed char* pText) {
    LOG_TRACE("(%p)", pText);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall MungeHeadupWidth@<EAX>(tHeadup *pHeadup@<EAX>)
int MungeHeadupWidth(tHeadup* pHeadup) {
    int width;
    LOG_TRACE("(%p)", pHeadup);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NewTextHeadupSlot2@<EAX>(int pSlot_index@<EAX>, int pFlash_rate@<EDX>, int pLifetime@<EBX>, int pFont_index@<ECX>, char *pText, int pQueue_it)
int NewTextHeadupSlot2(int pSlot_index, int pFlash_rate, int pLifetime, int pFont_index, char* pText, int pQueue_it) {
    int index;
    tHeadup* the_headup;
    tHeadup_slot* headup_slot;
    tU32 time;
    LOG_TRACE("(%d, %d, %d, %d, \"%s\", %d)", pSlot_index, pFlash_rate, pLifetime, pFont_index, pText, pQueue_it);

    STUB();
    return -1;
}

// IDA: int __usercall NewTextHeadupSlot@<EAX>(int pSlot_index@<EAX>, int pFlash_rate@<EDX>, int pLifetime@<EBX>, int pFont_index@<ECX>, char *pText)
int NewTextHeadupSlot(int pSlot_index, int pFlash_rate, int pLifetime, int pFont_index, char* pText) {
    LOG_TRACE("(%d, %d, %d, %d, \"%s\")", pSlot_index, pFlash_rate, pLifetime, pFont_index, pText);

    return NewTextHeadupSlot2(pSlot_index, pFlash_rate, pLifetime, pFont_index, pText, 1);
}

// IDA: int __usercall NewImageHeadupSlot@<EAX>(int pSlot_index@<EAX>, int pFlash_rate@<EDX>, int pLifetime@<EBX>, int pImage_index@<ECX>)
int NewImageHeadupSlot(int pSlot_index, int pFlash_rate, int pLifetime, int pImage_index) {
    int index;
    tHeadup* the_headup;
    tHeadup_slot* headup_slot;
    LOG_TRACE("(%d, %d, %d, %d)", pSlot_index, pFlash_rate, pLifetime, pImage_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoFancyHeadup(int pIndex@<EAX>)
void DoFancyHeadup(int pIndex) {
    tU32 the_time;
    tHeadup* the_headup;
    int temp_ref;
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl AdjustHeadups()
void AdjustHeadups() {
    int i;
    int delta_x;
    int delta_y;
    tHeadup* the_headup;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MoveHeadupTo(int pHeadup_index@<EAX>, int pNew_x@<EDX>, int pNew_y@<EBX>)
void MoveHeadupTo(int pHeadup_index, int pNew_x, int pNew_y) {
    int delta_x;
    tHeadup* the_headup;
    LOG_TRACE("(%d, %d, %d)", pHeadup_index, pNew_x, pNew_y);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ChangeHeadupText(int pHeadup_index@<EAX>, char *pNew_text@<EDX>)
void ChangeHeadupText(int pHeadup_index, char* pNew_text) {
    tHeadup* the_headup;
    LOG_TRACE("(%d, \"%s\")", pHeadup_index, pNew_text);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ChangeHeadupImage(int pHeadup_index@<EAX>, int pNew_image@<EDX>)
void ChangeHeadupImage(int pHeadup_index, int pNew_image) {
    tHeadup* the_headup;
    LOG_TRACE("(%d, %d)", pHeadup_index, pNew_image);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ChangeHeadupColour(int pHeadup_index@<EAX>, int pNew_colour@<EDX>)
void ChangeHeadupColour(int pHeadup_index, int pNew_colour) {
    LOG_TRACE("(%d, %d)", pHeadup_index, pNew_colour);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoDamageScreen(tU32 pThe_time@<EAX>)
void DoDamageScreen(tU32 pThe_time) {
    int i;
    int y_pitch;
    int the_step;
    int the_wobble_x;
    int the_wobble_y;
    br_pixelmap* the_image;
    tDamage_unit* the_damage;
    LOG_TRACE("(%d)", pThe_time);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl PoshDrawLine(float pAngle, br_pixelmap *pDestn, int pX1, int pY1, int pX2, int pY2, int pColour)
void PoshDrawLine(float pAngle, br_pixelmap* pDestn, int pX1, int pY1, int pX2, int pY2, int pColour) {
    LOG_TRACE("(%f, %p, %d, %d, %d, %d, %d)", pAngle, pDestn, pX1, pY1, pX2, pY2, pColour);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoInstruments(tU32 pThe_time@<EAX>)
void DoInstruments(tU32 pThe_time) {
    br_pixelmap* speedo_image;
    br_pixelmap* tacho_image;
    int the_wobble_x;
    int the_wobble_y;
    int gear;
    double the_angle;
    double the_angle2;
    double sin_angle;
    double cos_angle;
    double speed_mph;
    LOG_TRACE("(%d)", pThe_time);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoSteeringWheel(tU32 pThe_time@<EAX>)
void DoSteeringWheel(tU32 pThe_time) {
    br_pixelmap* hands_image;
    int hands_index;
    LOG_TRACE("(%d)", pThe_time);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ChangingView()
void ChangingView() {
    tU32 the_time;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall EarnCredits2(int pAmount@<EAX>, char *pPrefix_text@<EDX>)
void EarnCredits2(int pAmount, char* pPrefix_text) {
    char s[256];
    int original_amount;
    tU32 the_time;
    LOG_TRACE("(%d, \"%s\")", pAmount, pPrefix_text);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall EarnCredits(int pAmount@<EAX>)
void EarnCredits(int pAmount) {
    LOG_TRACE("(%d)", pAmount);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SpendCredits@<EAX>(int pAmount@<EAX>)
int SpendCredits(int pAmount) {
    int amount;
    LOG_TRACE("(%d)", pAmount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall AwardTime(tU32 pTime@<EAX>)
void AwardTime(tU32 pTime) {
    char s[256];
    tU32 original_amount;
    tU32 the_time;
    int i;
    LOG_TRACE("(%d)", pTime);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawRectangle(br_pixelmap *pPixelmap@<EAX>, int pLeft@<EDX>, int pTop@<EBX>, int pRight@<ECX>, int pBottom, int pColour)
void DrawRectangle(br_pixelmap* pPixelmap, int pLeft, int pTop, int pRight, int pBottom, int pColour) {
    LOG_TRACE("(%p, %d, %d, %d, %d, %d)", pPixelmap, pLeft, pTop, pRight, pBottom, pColour);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawRRectangle(br_pixelmap *pPixelmap@<EAX>, int pLeft@<EDX>, int pTop@<EBX>, int pRight@<ECX>, int pBottom, int pColour)
void DrawRRectangle(br_pixelmap* pPixelmap, int pLeft, int pTop, int pRight, int pBottom, int pColour) {
    LOG_TRACE("(%p, %d, %d, %d, %d, %d)", pPixelmap, pLeft, pTop, pRight, pBottom, pColour);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall OoerrIveGotTextInMeBoxMissus(int pFont_index@<EAX>, char *pText@<EDX>, br_pixelmap *pPixelmap@<EBX>, int pLeft@<ECX>, int pTop, int pRight, int pBottom, int pCentred)
void OoerrIveGotTextInMeBoxMissus(int pFont_index, char* pText, br_pixelmap* pPixelmap, int pLeft, int pTop, int pRight, int pBottom, int pCentred) {
    tDR_font* font;
    int width;
    int current_width;
    int i;
    int centre;
    int line_char_index;
    int input_str_index;
    int start_line;
    int current_y;
    int font_needed_loading;
    char line[256];
    LOG_TRACE("(%d, \"%s\", %p, %d, %d, %d, %d, %d)", pFont_index, pText, pPixelmap, pLeft, pTop, pRight, pBottom, pCentred);

    font = &gFonts[pFont_index];
    current_width = 0;
    font_needed_loading = font->images == NULL;
    if (font_needed_loading) {
        LoadFont(pFont_index);
    }
    centre = (pRight + pLeft) / 2;
    current_y = pTop;
    width = pRight - pLeft;
    line_char_index = 0;
    input_str_index = 0;

    while (pText[input_str_index]) {
        line[line_char_index] = pText[input_str_index];
        line[line_char_index + 1] = 0;
        current_width += font->spacing + font->width_table[pText[input_str_index] - font->offset];
        if (current_width > width) {
            for (i = input_str_index; i >= start_line; i--) {
                if (pText[i] == ' ') {
                    break;
                }
            }
            if (i == start_line) {
                i = input_str_index;
            }
            line_char_index += i - input_str_index;
            input_str_index = i;
            if (pText[input_str_index] == ' ') {
                input_str_index++;
            }
            line[line_char_index] = 0;
            if (pCentred) {
                TransDRPixelmapText(gBack_screen, centre - (DRTextWidth(font, line) / 2), current_y, font, line, (DRTextWidth(font, line) / 2) + centre);
            } else {
                TransDRPixelmapText(gBack_screen, pLeft, current_y, font, line, pRight);
            }
            current_width = 0;
            current_y += 3 * (font->height - (TranslationMode() ? 2 : 0)) / 2;
            line_char_index = 0;
            start_line = input_str_index;
        } else {
            line_char_index++;
            input_str_index++;
        }
    }
    if (line_char_index != 0) {
        if (pCentred) {
            TransDRPixelmapText(gBack_screen, centre - (DRTextWidth(font, line) / 2), current_y, font, line, (DRTextWidth(font, line) / 2) + centre);
        } else {
            TransDRPixelmapText(gBack_screen, pLeft, current_y, font, line, pRight);
        }
    }
    if (font_needed_loading) {
        DisposeFont(pFont_index);
    }
}

// IDA: void __usercall TransBrPixelmapText(br_pixelmap *pPixelmap@<EAX>, int pX@<EDX>, int pY@<EBX>, br_uint_32 pColour@<ECX>, br_font *pFont, signed char *pText)
void TransBrPixelmapText(br_pixelmap* pPixelmap, int pX, int pY, br_uint_32 pColour, br_font* pFont, signed char* pText) {
    int i;
    int len;
    LOG_TRACE("(%p, %d, %d, %d, %p, %p)", pPixelmap, pX, pY, pColour, pFont, pText);

    len = (TranslationMode() == 0 ? 0 : 2);
    BrPixelmapText(pPixelmap, pX, pY - len, pColour, pFont, (char*)pText);
}

// IDA: void __usercall TransDRPixelmapText(br_pixelmap *pPixelmap@<EAX>, int pX@<EDX>, int pY@<EBX>, tDR_font *pFont@<ECX>, char *pText, int pRight_edge)
void TransDRPixelmapText(br_pixelmap* pPixelmap, int pX, int pY, tDR_font* pFont, char* pText, int pRight_edge) {
    LOG_TRACE("(%p, %d, %d, %p, \"%s\", %d)", pPixelmap, pX, pY, pFont, pText, pRight_edge);

    if (gAusterity_mode && FlicsPlayedFromDisk() && pFont != gCached_font) {
        if (gCached_font && gCached_font - gFonts > 13) {
            DisposeFont(gCached_font - gFonts);
        }
        gCached_font = pFont;
    }
    LoadFont(pFont - gFonts);
    DRPixelmapText(pPixelmap, pX, pY - (TranslationMode() ? 2 : 0), pFont, pText, pRight_edge);
}

// IDA: void __usercall TransDRPixelmapCleverText(br_pixelmap *pPixelmap@<EAX>, int pX@<EDX>, int pY@<EBX>, tDR_font *pFont@<ECX>, char *pText, int pRight_edge)
void TransDRPixelmapCleverText(br_pixelmap* pPixelmap, int pX, int pY, tDR_font* pFont, char* pText, int pRight_edge) {
    LOG_TRACE("(%p, %d, %d, %p, \"%s\", %d)", pPixelmap, pX, pY, pFont, pText, pRight_edge);
    NOT_IMPLEMENTED();
}
