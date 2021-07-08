#include "displays.h"
#include "brender.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrpb.h"
#include "graphics.h"
#include "netgame.h"
#include "pd/sys.h"
#include "utility.h"
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

    TimerString(gTimer, pStr, pFudge_colon, 0);
}

// IDA: void __cdecl InitHeadups()
void InitHeadups() {
    int i;
    LOG_TRACE("()");
    for (i = 0; i < 15; i++) {
        gHeadups[i].type = eHeadup_unused;
    }
    gBR_fonts[0] = BrFontProp7x9;
    gBR_fonts[1] = BrFontFixed3x5;
    gBR_fonts[2] = gFont_7;
    gBR_fonts[3] = gHeadup_font;
}

// IDA: void __usercall ClearHeadup(int pIndex@<EAX>)
void ClearHeadup(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    gHeadups[pIndex].type = eHeadup_unused;
}

// IDA: void __usercall ClearHeadupSlot(int pSlot_index@<EAX>)
void ClearHeadupSlot(int pSlot_index) {
    int i;
    tHeadup* the_headup;
    LOG_TRACE("(%d)", pSlot_index);

    the_headup = gHeadups;
    for (i = 0; i < COUNT_OF(gHeadups); i++) {
        if (the_headup->type && the_headup->slot_index == pSlot_index) {
            ClearHeadup(i);
            return;
        }
        the_headup++;
    }
}

// IDA: void __cdecl ClearHeadups()
void ClearHeadups() {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gHeadups); i++) {
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

    x = pX;
    len = strlen((char*)pText);
    ch = (unsigned char*)pText;
    if (pX >= 0 && pPixelmap->width >= pRight_edge && pY >= 0 && pY + pFont->height <= pPixelmap->height) {
        for (i = 0; i < len; i++) {
            if (*ch < 224) {
                chr = *ch - pFont->offset;
                ch_width = pFont->width_table[chr];
                DRPixelmapRectangleOnscreenCopy(
                    gBack_screen,
                    x,
                    pY,
                    pFont->images,
                    0,
                    chr * pFont->height,
                    ch_width,
                    pFont->height);
                x += ch_width + pFont->spacing;
            } else {
                new_font = &gFonts[-*ch + 256];
                pY -= (new_font->height - pFont->height) / 2;
                pFont = new_font;
            }
            ch++;
        }
    } else {
        for (i = 0; i < len; i++) {
            if (*ch < 224) {
                chr = *ch - pFont->offset;
                ch_width = pFont->width_table[chr];
                DRPixelmapRectangleMaskedCopy(
                    gBack_screen,
                    x,
                    pY,
                    pFont->images,
                    0,
                    chr * pFont->height,
                    ch_width,
                    pFont->height);
                x += ch_width + pFont->spacing;
            } else {
                new_font = &gFonts[-*ch + 256];
                pY -= (new_font->height - pFont->height) / 2;
                pFont = new_font;
            }
            ch++;
        }
    }
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
    LOG_TRACE9("(%p, %d, %d, %d, %d, %d)", pPixelmap, pLeft, pTop, pRight, pBottom, pKnock_out_corners);
    SILENT_STUB();
}

// IDA: void __cdecl DimAFewBits()
void DimAFewBits() {
    int i;
    LOG_TRACE("()");

    int dim_index; // Added
    dim_index = gProgram_state.cockpit_on && gProgram_state.cockpit_image_index >= 0;
    for (i = 0; i < gProgram_state.current_car.dim_count[dim_index]; i++) {
        DimRectangle(
            gBack_screen,
            gProgram_state.current_car.dim_left[dim_index][i],
            gProgram_state.current_car.dim_top[dim_index][i],
            gProgram_state.current_car.dim_right[dim_index][i],
            gProgram_state.current_car.dim_bottom[dim_index][i],
            1);
    }
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
    SILENT_STUB();
}

// IDA: void __usercall DoHeadups(tU32 pThe_time@<EAX>)
void DoHeadups(tU32 pThe_time) {
    int i;
    int x_offset;
    int y_offset;
    tHeadup* the_headup;
    int time_factor;
    LOG_TRACE("(%d)", pThe_time);

    if (gNet_mode) {
        DoNetScores();
    }
    if (gQueued_headup_count && PDGetTotalTime() - gLast_centre_headup >= 1000) {
        NewTextHeadupSlot(4, gQueued_headups[0].flash_rate,
            gQueued_headups[0].lifetime,
            gQueued_headups[0].font_index,
            gQueued_headups[0].text);
        KillOldestQueuedHeadup();
    }

    the_headup = gHeadups;
    for (i = 0; i < 15; i++) {
        the_headup = &gHeadups[i];
        if (the_headup->type
            && (gProgram_state.which_view == eView_forward || !the_headup->cockpit_anchored)
            && (the_headup->type == eHeadup_image
                || the_headup->type == eHeadup_fancy
                || (the_headup->type == eHeadup_text && the_headup->data.text_info.text[0])
                || ((the_headup->type == eHeadup_coloured_text || the_headup->type == eHeadup_box_text)
                    && the_headup->data.text_info.text[0]))) {
            if (the_headup->type == eHeadup_fancy || !the_headup->end_time || the_headup->end_time > pThe_time) {
                if (the_headup->dimmed_background) {
                    DimRectangle(
                        gBack_screen,
                        the_headup->dim_left,
                        the_headup->dim_top,
                        the_headup->dim_right,
                        the_headup->dim_bottom,
                        1);
                }
                if (!the_headup->flash_period
                    || Flash(the_headup->flash_period, &the_headup->last_flash, &the_headup->flash_state)) {
                    switch (the_headup->type) {
                    case eHeadup_text:
                        if (the_headup->cockpit_anchored) {
                            y_offset = gScreen_wobble_y;
                        } else {
                            y_offset = 0;
                        }
                        if (the_headup->cockpit_anchored) {
                            x_offset = gScreen_wobble_x;
                        } else {
                            x_offset = 0;
                        }
                        TransBrPixelmapText(
                            gBack_screen,
                            x_offset + the_headup->x,
                            y_offset + the_headup->y,
                            the_headup->data.text_info.colour,
                            the_headup->data.text_info.font,
                            (signed char*)the_headup->data.text_info.text);
                        break;
                    case eHeadup_coloured_text:
                        if (the_headup->clever) {
                            if (the_headup->cockpit_anchored) {
                                y_offset = gScreen_wobble_y;
                            } else {
                                y_offset = 0;
                            }
                            if (the_headup->cockpit_anchored) {
                                x_offset = gScreen_wobble_x;
                            } else {
                                x_offset = 0;
                            }
                            TransDRPixelmapCleverText(
                                gBack_screen,
                                x_offset + the_headup->x,
                                y_offset + the_headup->y,
                                the_headup->data.coloured_text_info.coloured_font,
                                the_headup->data.text_info.text,
                                the_headup->right_edge);
                        } else {
                            if (the_headup->cockpit_anchored) {
                                y_offset = gScreen_wobble_y;
                            } else {
                                y_offset = 0;
                            }
                            if (the_headup->cockpit_anchored) {
                                x_offset = gScreen_wobble_x;
                            } else {
                                x_offset = 0;
                            }
                            TransDRPixelmapText(
                                gBack_screen,
                                x_offset + the_headup->x,
                                y_offset + the_headup->y,
                                the_headup->data.coloured_text_info.coloured_font,
                                the_headup->data.text_info.text,
                                the_headup->right_edge);
                        }
                        break;
                    case eHeadup_image:
                        if (the_headup->cockpit_anchored) {
                            y_offset = gScreen_wobble_y;
                        } else {
                            y_offset = 0;
                        }
                        if (the_headup->cockpit_anchored) {
                            x_offset = gScreen_wobble_x;
                        } else {
                            x_offset = 0;
                        }
                        DRPixelmapRectangleMaskedCopy(
                            gBack_screen,
                            x_offset + the_headup->x,
                            y_offset + the_headup->y,
                            the_headup->data.image_info.image,
                            0,
                            0,
                            the_headup->data.image_info.image->width,
                            the_headup->data.image_info.image->height);
                        break;

                    case eHeadup_fancy:
                        switch (the_headup->data.fancy_info.fancy_stage) {
                        case eFancy_stage_incoming:
                            the_headup->data.fancy_info.offset -= 500 * gFrame_period / 1000;
                            if (the_headup->data.fancy_info.offset <= the_headup->data.fancy_info.shear_amount) {
                                the_headup->data.fancy_info.offset = the_headup->data.fancy_info.shear_amount;
                                the_headup->data.fancy_info.fancy_stage = eFancy_stage_halting;
                                the_headup->data.fancy_info.start_time = GetTotalTime();
                            }
                            continue;
                        case eFancy_stage_halting:
                            time_factor = 1000 * (pThe_time - the_headup->data.fancy_info.start_time) / 100;
                            if (time_factor > 1000) {
                                if (time_factor > 1500) {
                                    time_factor = 1500;
                                    the_headup->data.fancy_info.fancy_stage = eFancy_stage_waiting;
                                    the_headup->data.fancy_info.start_time = GetTotalTime();
                                }
                                DRPixelmapRectangleShearedCopy(
                                    gBack_screen,
                                    the_headup->x - (1500 - time_factor) * the_headup->data.fancy_info.shear_amount / 500,
                                    the_headup->y,
                                    the_headup->data.image_info.image,
                                    0,
                                    0,
                                    the_headup->data.image_info.image->width,
                                    the_headup->data.image_info.image->height,
                                    (((1500 - time_factor) * the_headup->data.fancy_info.shear_amount / 500) << 16)
                                        / the_headup->data.image_info.image->height);
                            } else {
                                DRPixelmapRectangleShearedCopy(
                                    gBack_screen,
                                    the_headup->x - the_headup->data.fancy_info.shear_amount * (time_factor - 500) / 500,
                                    the_headup->y,
                                    the_headup->data.image_info.image,
                                    0,
                                    0,
                                    the_headup->data.image_info.image->width,
                                    the_headup->data.image_info.image->height,
                                    ((the_headup->data.fancy_info.shear_amount * (time_factor - 500) / 500) << 16)
                                        / the_headup->data.image_info.image->height);
                            }
                            break;
                        case eFancy_stage_waiting:
                            if (pThe_time - the_headup->data.fancy_info.start_time > 1000) {
                                the_headup->data.fancy_info.fancy_stage = eFancy_stage_readying;
                                the_headup->data.fancy_info.start_time = GetTotalTime();
                            }
                            DRPixelmapRectangleMaskedCopy(
                                gBack_screen,
                                the_headup->x,
                                the_headup->y,
                                the_headup->data.image_info.image,
                                0,
                                0,
                                the_headup->data.image_info.image->width,
                                the_headup->data.image_info.image->height);
                            break;
                        case eFancy_stage_readying:
                            time_factor = 1000 * (pThe_time - the_headup->data.fancy_info.start_time) / 100;
                            if (time_factor > 1000) {
                                time_factor = 1000;
                                the_headup->data.fancy_info.fancy_stage = eFancy_stage_leaving;
                                the_headup->data.fancy_info.start_time = GetTotalTime();
                                the_headup->data.fancy_info.offset = 0;
                            }
                            DRPixelmapRectangleShearedCopy(
                                gBack_screen,
                                the_headup->x,
                                the_headup->y,
                                the_headup->data.image_info.image,
                                0,
                                0,
                                the_headup->data.image_info.image->width,
                                the_headup->data.image_info.image->height,
                                -(((time_factor * the_headup->data.fancy_info.shear_amount / 1000) << 16)
                                    / the_headup->data.image_info.image->height));
                            break;
                        case eFancy_stage_leaving:
                            the_headup->data.fancy_info.offset -= 500 * gFrame_period / 0x3E8;
                            if (the_headup->data.fancy_info.offset <= the_headup->data.fancy_info.end_offset) {
                                ClearHeadup(i);
                                break;
                            }
                            DRPixelmapRectangleShearedCopy(
                                gBack_screen,
                                the_headup->data.fancy_info.offset + the_headup->x,
                                the_headup->y,
                                the_headup->data.image_info.image,
                                0,
                                0,
                                the_headup->data.image_info.image->width,
                                the_headup->data.image_info.image->height,
                                -65536);
                            break;
                        default:
                            continue;
                        }
                        break;

                    case eHeadup_box_text:
                        if (the_headup->cockpit_anchored) {
                            y_offset = gScreen_wobble_y;
                        } else {
                            y_offset = 0;
                        }
                        if (the_headup->cockpit_anchored) {
                            x_offset = gScreen_wobble_y;
                        } else {
                            x_offset = 0;
                        }
                        OoerrIveGotTextInMeBoxMissus(
                            the_headup->data.coloured_text_info.coloured_font - gFonts,
                            the_headup->data.coloured_text_info.text,
                            gBack_screen,
                            gBack_screen->width / 10,
                            x_offset + the_headup->y,
                            9 * gBack_screen->width / 10,
                            y_offset + the_headup->y + 60,
                            1);
                        break;
                    default:
                        break;
                    }
                }
            } else {
                ClearHeadup(i);
            }
        }
    }
    DoPSPowerupHeadups();
}

// IDA: int __usercall FindAHeadupHoleWoofBarkSoundsABitRude@<EAX>(int pSlot_index@<EAX>)
int FindAHeadupHoleWoofBarkSoundsABitRude(int pSlot_index) {
    int i;
    int empty_one;
    tHeadup* the_headup;
    LOG_TRACE("(%d)", pSlot_index);

    the_headup = gHeadups;
    empty_one = -1;
    for (i = 0; i < COUNT_OF(gHeadups); i++) {
        if (pSlot_index >= 0 && the_headup->slot_index == pSlot_index) {
            return i;
        }
        if (the_headup->type == eHeadup_unused) {
            empty_one = i;
            break;
        }
        the_headup++;
    }
    return empty_one;
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

    result = 0;
    len = strlen((char*)pText) + 1;
    i = 0;
    c = (unsigned char*)pText;
    while (len - 1 > i) {
        if (*c < 224u) {
            if (len - 2 > i) {
                result += pFont->spacing + pFont->width_table[*c - pFont->offset];
            } else {
                result += pFont->width_table[*c - pFont->offset];
            }
        } else {
            pFont = &gFonts[-*c + 256];
        }
        ++i;
        ++c;
    }
    return result;
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

    while (*pText) {
        pText++;
        if (*pText < 0) {
            return 1;
        }
    }
    return 0;
}

// IDA: int __usercall MungeHeadupWidth@<EAX>(tHeadup *pHeadup@<EAX>)
int MungeHeadupWidth(tHeadup* pHeadup) {
    int width;
    LOG_TRACE("(%p)", pHeadup);

    width = 0;
    if (pHeadup->type == eHeadup_box_text) {
        return 0;
    }
    if (pHeadup->type == eHeadup_coloured_text) {
        pHeadup->clever = IsHeadupTextClever((signed char*)(&pHeadup->data.text_info.text));
        if (pHeadup->justification) {
            if (pHeadup->justification == eJust_right) {
                if (pHeadup->clever) {
                    width = DRTextCleverWidth(
                        pHeadup->data.coloured_text_info.coloured_font,
                        (signed char*)(&pHeadup->data.text_info.text));
                } else {
                    width = DRTextWidth(pHeadup->data.coloured_text_info.coloured_font, pHeadup->data.text_info.text);
                }
                pHeadup->x = pHeadup->original_x - width;
            } else if (pHeadup->justification == eJust_centre) {
                if (pHeadup->clever) {
                    width = DRTextCleverWidth(
                        pHeadup->data.coloured_text_info.coloured_font,
                        (signed char*)(&pHeadup->data.text_info.text));
                } else {
                    width = DRTextWidth(pHeadup->data.coloured_text_info.coloured_font, pHeadup->data.text_info.text);
                }
                pHeadup->x = pHeadup->original_x - width / 2;
            }
        } else {
            pHeadup->x = pHeadup->original_x;
        }
    } else {
        pHeadup->clever = 0;
        if (pHeadup->justification) {
            if (pHeadup->justification == eJust_right) {
                width = BrPixelmapTextWidth(gBack_screen, pHeadup->data.text_info.font, pHeadup->data.text_info.text);
                pHeadup->x = pHeadup->original_x - width;
            } else if (pHeadup->justification == eJust_centre) {
                width = BrPixelmapTextWidth(gBack_screen, pHeadup->data.text_info.font, pHeadup->data.text_info.text);
                pHeadup->x = pHeadup->original_x - width / 2;
            }
        } else {
            pHeadup->x = pHeadup->original_x;
        }
    }
    return width;
}

// IDA: int __usercall NewTextHeadupSlot2@<EAX>(int pSlot_index@<EAX>, int pFlash_rate@<EDX>, int pLifetime@<EBX>, int pFont_index@<ECX>, char *pText, int pQueue_it)
int NewTextHeadupSlot2(int pSlot_index, int pFlash_rate, int pLifetime, int pFont_index, char* pText, int pQueue_it) {
    int index;
    tHeadup* the_headup;
    tHeadup_slot* headup_slot;
    tU32 time;
    LOG_TRACE("(%d, %d, %d, %d, \"%s\", %d)", pSlot_index, pFlash_rate, pLifetime, pFont_index, pText, pQueue_it);

    time = PDGetTotalTime();
    if (pQueue_it && pSlot_index == 4 && (unsigned int)(time - gLast_centre_headup) < 1000) {
        if (gQueued_headup_count == 4) {
            KillOldestQueuedHeadup();
        }
        gQueued_headups[gQueued_headup_count].flash_rate = pFlash_rate;
        gQueued_headups[gQueued_headup_count].lifetime = pLifetime;
        gQueued_headups[gQueued_headup_count].font_index = pFont_index;
        strcpy(gQueued_headups[gQueued_headup_count].text, pText);
        gQueued_headup_count++;
        index = -1;
    } else {
        index = FindAHeadupHoleWoofBarkSoundsABitRude(pSlot_index);
        if (index >= 0) {
            if (pSlot_index == 4) {
                gLast_centre_headup = time;
            }
            headup_slot = &gProgram_state.current_car.headup_slots[gProgram_state.cockpit_on][pSlot_index];
            the_headup = &gHeadups[index];
            the_headup->data.coloured_text_info.coloured_font = &gFonts[-pFont_index];
            if (pSlot_index == 4) {
                the_headup->type = eHeadup_box_text;
            } else {
                the_headup->type = eHeadup_coloured_text;
            }
            if (!pText) {
                LOG_PANIC("panic");
            }
            strcpy(the_headup->data.text_info.text, pText);

            the_headup->slot_index = pSlot_index;
            the_headup->justification = headup_slot->justification;
            if (pSlot_index < 0) {
                the_headup->cockpit_anchored = 0;
            } else {
                the_headup->cockpit_anchored = headup_slot->cockpit_anchored;
            }
            the_headup->dimmed_background = headup_slot->dimmed_background;
            the_headup->dim_left = headup_slot->dim_left;
            the_headup->dim_top = headup_slot->dim_top;
            the_headup->dim_right = headup_slot->dim_right;
            the_headup->dim_bottom = headup_slot->dim_bottom;
            the_headup->original_x = headup_slot->x;
            the_headup->right_edge = MungeHeadupWidth(the_headup) + the_headup->x;
            the_headup->y = headup_slot->y;
            if (pFlash_rate) {
                the_headup->flash_period = 1000 / pFlash_rate;
            } else {
                the_headup->flash_period = 0;
            }
            the_headup->last_flash = 0;
            the_headup->flash_state = 0;
            if (pLifetime) {
                the_headup->end_time = GetTotalTime() + pLifetime;
            } else {
                the_headup->end_time = 0;
            }
        }
    }
    return index;
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

    index = FindAHeadupHoleWoofBarkSoundsABitRude(pSlot_index);
    if (index >= 0) {
        headup_slot = &gProgram_state.current_car.headup_slots[gProgram_state.cockpit_on][pSlot_index];
        the_headup = &gHeadups[index];
        the_headup->type = eHeadup_image;
        the_headup->slot_index = pSlot_index;
        the_headup->justification = headup_slot->justification;
        if (pSlot_index < 0) {
            the_headup->cockpit_anchored = 0;
        } else {
            the_headup->cockpit_anchored = headup_slot->cockpit_anchored;
        }
        the_headup->dimmed_background = headup_slot->dimmed_background;
        the_headup->dim_left = headup_slot->dim_left;
        the_headup->dim_top = headup_slot->dim_top;
        the_headup->dim_right = headup_slot->dim_right;
        the_headup->dim_bottom = headup_slot->dim_bottom;
        the_headup->original_x = headup_slot->x;

        if (headup_slot->justification) {
            if (headup_slot->justification == eJust_right) {
                the_headup->x = the_headup->original_x - gHeadup_images[pImage_index]->width;
            } else if (headup_slot->justification == eJust_centre) {
                the_headup->x = the_headup->original_x - gHeadup_images[pImage_index]->width / 2;
            }
        } else {
            the_headup->x = the_headup->original_x;
        }
        the_headup->y = headup_slot->y;
        if (pFlash_rate) {
            the_headup->flash_period = 1000 / pFlash_rate;
        } else {
            the_headup->flash_period = 0;
        }
        the_headup->last_flash = 0;
        the_headup->flash_state = 0;
        if (pLifetime) {
            the_headup->end_time = GetTotalTime() + pLifetime;
        } else {
            the_headup->end_time = 0;
        }
        the_headup->data.image_info.image = gHeadup_images[pImage_index];
    }
    return index;
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

    the_headup = gHeadups;
    for (i = 0; i < COUNT_OF(gHeadups); i++) {
        if (the_headup->type) {
            delta_x = gProgram_state.current_car.headup_slots[gProgram_state.cockpit_on && gProgram_state.cockpit_image_index >= 0][the_headup->slot_index].x
                - gProgram_state.current_car.headup_slots[!gProgram_state.cockpit_on || gProgram_state.cockpit_image_index < 0][the_headup->slot_index].x;
            delta_y = gProgram_state.current_car.headup_slots[gProgram_state.cockpit_on && gProgram_state.cockpit_image_index >= 0][the_headup->slot_index].y
                - gProgram_state.current_car.headup_slots[!gProgram_state.cockpit_on || gProgram_state.cockpit_image_index < 0][the_headup->slot_index].y;
            the_headup->x += delta_x;
            the_headup->original_x += delta_x;
            the_headup->y += delta_y;
            the_headup->dim_left += delta_x;
            the_headup->dim_top += delta_y;
            the_headup->dim_right += delta_x;
            the_headup->dim_bottom += delta_y;
        }
        the_headup++;
    }
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

    if (pHeadup_index >= 0) {
        the_headup = &gHeadups[pHeadup_index];
        strcpy(the_headup->data.text_info.text, pNew_text);
        MungeHeadupWidth(the_headup);
    }
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
    SILENT_STUB();
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
    SILENT_STUB();
}

// IDA: void __usercall DoSteeringWheel(tU32 pThe_time@<EAX>)
void DoSteeringWheel(tU32 pThe_time) {
    br_pixelmap* hands_image;
    int hands_index;
    LOG_TRACE("(%d)", pThe_time);
    SILENT_STUB();
}

// IDA: void __cdecl ChangingView()
void ChangingView() {
    tU32 the_time;
    LOG_TRACE("()");
    SILENT_STUB();
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
    start_line = 0;

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

    if (gAusterity_mode && FlicsPlayedFromDisk() && gCached_font != pFont) {
        if (gCached_font && gCached_font - gFonts > 13) {
            DisposeFont(gCached_font - gFonts);
        }
        gCached_font = pFont;
    }
    LoadFont(pFont - gFonts);
    DRPixelmapCleverText2(pPixelmap, pX, pY - (TranslationMode() == 0 ? 0 : 2), pFont, (signed char*)pText, pRight_edge);
}
