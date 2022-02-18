#include "graphics.h"

#include "brender/brender.h"
#include "car.h"
#include "constants.h"
#include "controls.h"
#include "depth.h"
#include "displays.h"
#include "errors.h"
#include "finteray.h"
#include "flicplay.h"
#include "globvars.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "harness/hooks.h"
#include "harness/trace.h"
#include "init.h"
#include "loading.h"
#include "network.h"
#include "oil.h"
#include "opponent.h"
#include "pd/sys.h"
#include "pedestrn.h"
#include "piping.h"
#include "powerup.h"
#include "pratcam.h"
#include "replay.h"
#include "sound.h"
#include "spark.h"
#include "utility.h"
#include "world.h"
#include <stdlib.h>
#include <unistd.h>

#include <math.h>

int gPalette_munged;
int gColourValues[1];
int gNext_transient;
int gCursor_x_offsets[8];
int gCursor_y_offsets[8];
int gCursor_gib_x_offsets[8];
int gCursor_gib_y_offsets[8];
int gCursor_giblet_sequence0[7];
int gCursor_giblet_sequence1[5];
int gCursor_giblet_sequence2[5];
int gCursor_giblet_sequence3[5];
int* gCursor_giblet_sequences[4];
char* gFont_names[21] = {
    "TYPEABLE",
    "ORANGHED",
    "BLUEHEAD",
    "GREENHED",
    "MEDIUMHD",
    "TIMER",
    "NEWHITE",
    "NEWRED",
    "NEWBIGGR",
    "GRNDK",
    "GRNLIT",
    "GRYDK",
    "GRYLIT",
    "BUTTIN",
    "BUTTOUT",
    "LITPLAQ",
    "DRKPLAQ",
    "BUTTIN1",
    "BUTTOUT1",
    "LITPLAQ1",
    "DRKPLAQ1"
};
br_colour gRGB_colours[9] = {
    0u,
    16777215u,
    16711680u,
    65280u,
    255u,
    16776960u,
    65535u,
    16711935u,
    13649666u
};
br_matrix34 gSheer_mat = {
    { { 1.0, 0.0, 0.0 },
        { 0.0, 1.0, 0.0 },
        { 0.0, 0.0, 1.0 },
        { 0.0, 0.0, 0.0 } }
};
br_matrix34 gIdentity34 = {
    { { 1.0, 0.0, 0.0 },
        { 0.0, 1.0, 0.0 },
        { 0.0, 0.0, 1.0 },
        { 0.0, 0.0, 0.0 } }
};
tShadow_level gShadow_level = eShadow_us_only;
br_scalar gShadow_hither_z_move;
br_scalar gShadow_hither_min_move;
int gArrows[2][4][60];
float gMap_render_x;
float gMap_render_y;
float gMap_render_width;
float gMap_render_height;
int gMouse_started;
int gFaded_palette;
int gAR_fudge_headups;
br_pixelmap* gCurrent_splash;
br_pixelmap* gCurrent_conversion_table;
int gMap_colours[4] = { 4, 0, 52, 132 };
br_vector3 gShadow_points[8];
tConcussion gConcussion;
tClip_details gShadow_clip_planes[8];
br_actor* gLollipops[100];
tWobble_spec gWobble_array[5];
tSaved_table gSaved_shade_tables[100];
tCursor_giblet gCursor_giblets[45];
tTransient_bm gTransient_bitmaps[50];
float gCosine_array[64];
br_pixelmap* gCursors[8];
br_pixelmap* gCursor_giblet_images[18];
br_pixelmap* gEval_1;
br_pixelmap* gEval_2;
br_vector3 gShadow_light_z;
br_vector3 gShadow_light_x;
int gShadow_dim_amount;
int gNumber_of_lollipops;
br_vector3 gShadow_light_ray;
int gFancy_shadow;
br_model* gShadow_model;
br_actor* gShadow_actor;
int gShadow_clip_plane_count;
br_pixelmap* gPalette_conversion_table;
br_material* gShadow_material;
int gSaved_table_count;
int gCurrent_cursor_index;
int gPalette_index;
int gCursor_transient_index;
char* gScratch_pixels;
br_pixelmap* gScratch_palette;
int gLast_palette_change;
br_pixelmap* gOrig_render_palette;
br_pixelmap* gCurrent_palette;
br_pixelmap* gRender_palette;
float gCamera_to_horiz_angle;
int gColours[9];
br_pixelmap* gFlic_palette;
tDR_font gFonts[21];
char* gCurrent_palette_pixels;
int gWidth;
int gMap_render_height_i;
int gScreen_wobble_x;
int gScreen_wobble_y;
br_scalar gCurrent_ambience;
int gY_offset;
int gMap_render_width_i;
int gMouse_in_use;
int gHeight;
int gMouse_last_y_coord;
int gMouse_last_x_coord;
br_scalar gAmbient_adjustment;
int gMap_render_x_i;
int gX_offset;
int gMap_render_y_i;
int gMirror_on__graphics; // suffix added to avoid duplicate symbol
br_scalar gYon_squared;

// IDA: void __cdecl TurnOnPaletteConversion()
void TurnOnPaletteConversion() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl TurnOffPaletteConversion()
void TurnOffPaletteConversion() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ResetLollipopQueue()
void ResetLollipopQueue() {
    LOG_TRACE("()");

    gNumber_of_lollipops = 0;
}

// IDA: int __usercall AddToLollipopQueue@<EAX>(br_actor *pActor@<EAX>, int pIndex@<EDX>)
int AddToLollipopQueue(br_actor* pActor, int pIndex) {
    LOG_TRACE("(%p, %d)", pActor, pIndex);

    int result; // eax

    if (pIndex >= 0) {
        gLollipops[pIndex] = pActor;
        return pIndex;
    } else if (gNumber_of_lollipops >= 100) {
        return -1;
    } else {
        gLollipops[gNumber_of_lollipops] = pActor;
        gNumber_of_lollipops++;
        return gNumber_of_lollipops - 1;
    }
}

// IDA: void __cdecl RenderLollipops()
void RenderLollipops() {
    int i;
    int must_relink;
    br_actor** the_actor;
    br_actor* old_parent;
    LOG_TRACE("()");
    STUB_ONCE();
}

// IDA: void __usercall DRDrawLine(br_pixelmap *pDestn@<EAX>, int pX1@<EDX>, int pY1@<EBX>, int pX2@<ECX>, int pY2, int pColour)
void DRDrawLine(br_pixelmap* pDestn, int pX1, int pY1, int pX2, int pY2, int pColour) {
    tU8* d_ptr;
    tS32 y_delta;
    tS32 x_delta;
    tU32 current_y;
    tU32 current_x;
    int row_bytes;
    int x;
    int y;
    int the_diff;
    LOG_TRACE("(%p, %d, %d, %d, %d, %d)", pDestn, pX1, pY1, pX2, pY2, pColour);

    BrPixelmapLine(pDestn, pX1, pY1, pX2, pY2, pColour);
}

// IDA: void __usercall DrawDigitAt(br_pixelmap *gImage@<EAX>, int pX@<EDX>, int pY@<EBX>, int pY_pitch@<ECX>, int pValue)
void DrawDigitAt(br_pixelmap* gImage, int pX, int pY, int pY_pitch, int pValue) {
    LOG_TRACE("(%p, %d, %d, %d, %d)", gImage, pX, pY, pY_pitch, pValue);

    DRPixelmapRectangleMaskedCopy(gBack_screen, pX, pY, gImage, 0, pY_pitch * pValue, gImage->width, pY_pitch);
}

// IDA: void __usercall DrawNumberAt(br_pixelmap *gImage@<EAX>, int pX@<EDX>, int pY@<EBX>, int pX_pitch@<ECX>, int pY_pitch, int pValue, int pDigit_count, int pLeading_zeroes)
void DrawNumberAt(br_pixelmap* gImage, int pX, int pY, int pX_pitch, int pY_pitch, int pValue, int pDigit_count, int pLeading_zeroes) {
    int i;
    int the_value;
    LOG_TRACE("(%p, %d, %d, %d, %d, %d, %d, %d)", gImage, pX, pY, pX_pitch, pY_pitch, pValue, pDigit_count, pLeading_zeroes);

    for (i = pDigit_count - 1; i >= 0; i--) {
        the_value = pValue % 10;
        pValue /= 10;
        if (pValue || pLeading_zeroes || pDigit_count - 1 == i) {
            DrawDigitAt(gImage, pX + pX_pitch * i, pY, pY_pitch, the_value);
        }
    }
}

// IDA: void __usercall BuildColourTable(br_pixelmap *pPalette@<EAX>)
void BuildColourTable(br_pixelmap* pPalette) {
    int i;
    int j;
    int nearest_index = 0;
    int red;
    int green;
    int blue;
    float nearest_distance;
    float distance;
    LOG_TRACE("(%p)", pPalette);

#define SQR(i) i* i

    for (i = 0; i < 9; ++i) {
        nearest_distance = 0x48400000;
        red = (gRGB_colours[i] >> 16) & 0xFF;
        green = (gRGB_colours[i] >> 8) & 0xFF;
        blue = gRGB_colours[i] & 0xFF;
        for (j = 0; j < 256; j++) {
            distance = SQR((double)(signed int)(*((br_uint_8*)pPalette->pixels + 4 * j + 2) - red));
            distance += SQR((double)(signed int)(*((br_uint_8*)pPalette->pixels + 4 * j) - blue));
            distance += SQR((double)(signed int)(*((br_uint_8*)pPalette->pixels + 4 * j + 1) - green));
            if (distance < nearest_distance) {
                nearest_index = j;
                nearest_distance = distance;
            }
        }
        gColours[i] = nearest_index;
    }
}

// IDA: void __cdecl ClearConcussion()
void ClearConcussion() {
    LOG_TRACE("()");

    gConcussion.concussed = 0;
}

// IDA: tS8* __usercall SkipLines@<EAX>(tS8 *pSource@<EAX>, int pCount@<EDX>)
tS8* SkipLines(tS8* pSource, int pCount) {
    int i;
    int j;
    int number_of_chunks;
    int chunk_length;
    LOG_TRACE("(%p, %d)", pSource, pCount);

    for (i = 0; i < pCount; ++i) {
        number_of_chunks = *pSource++;
        for (j = 0; j < number_of_chunks; j++) {
            chunk_length = *pSource++;
            if (chunk_length < 0) {
                pSource -= chunk_length;
            }
        }
    }
    return pSource;
}

// IDA: void __usercall CopyWords(char *pDst@<EAX>, char *pSrc@<EDX>, int pN@<EBX>)
void CopyWords(char* pDst, char* pSrc, int pN) {
    tU16* dst;
    tU16* src;
    LOG_TRACE("(\"%s\", \"%s\", %d)", pDst, pSrc, pN);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall Copy8BitStripImageTo16Bit(br_pixelmap *pDest@<EAX>, br_int_16 pDest_x@<EDX>, br_int_16 pOffset_x@<EBX>, br_int_16 pDest_y@<ECX>, br_int_16 pOffset_y, tS8 *pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_uint_16 pWidth, br_uint_16 pHeight)
void Copy8BitStripImageTo16Bit(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pOffset_x, br_int_16 pDest_y, br_int_16 pOffset_y, tS8* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_uint_16 pWidth, br_uint_16 pHeight) {
    int i;
    int j;
    int height;
    int number_of_chunks;
    int old_x_byte;
    int x_byte;
    int off_the_left;
    int destn_width;
    int chunk_length;
    char* destn_ptr;
    char* destn_ptr2;
    LOG_TRACE("(%p, %d, %d, %d, %d, %p, %d, %d, %d, %d)", pDest, pDest_x, pOffset_x, pDest_y, pOffset_y, pSource, pSource_x, pSource_y, pWidth, pHeight);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CopyStripImage(br_pixelmap *pDest@<EAX>, br_int_16 pDest_x@<EDX>, br_int_16 pOffset_x@<EBX>, br_int_16 pDest_y@<ECX>, br_int_16 pOffset_y, tS8 *pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_uint_16 pWidth, br_uint_16 pHeight)
void CopyStripImage(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pOffset_x, br_int_16 pDest_y, br_int_16 pOffset_y, tS8* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_uint_16 pWidth, br_uint_16 pHeight) {
    int i;
    int j;
    int height;
    int number_of_chunks;
    int old_x_byte;
    int x_byte;
    int off_the_left;
    int destn_width;
    int chunk_length;
    char* destn_ptr;
    char* destn_ptr2;
    LOG_TRACE8("(%p, %d, %d, %d, %d, %p, %d, %d, %d, %d)", pDest, pDest_x, pOffset_x, pDest_y, pOffset_y, pSource, pSource_x, pSource_y, pWidth, pHeight);

    height = *(uint16_t*)pSource;
    pSource = pSource + 2;
    if (pDest_y + pOffset_y >= 0) {
        destn_ptr = (char*)pDest->pixels + pDest->row_bytes * (pDest_y + pOffset_y);
    } else {
        LOG_DEBUG("%d, %d", pDest_y, pOffset_y);

        pSource = SkipLines(pSource, -pDest_y - pOffset_y);
        destn_ptr = (char*)pDest->pixels;
        height += pDest_y + pOffset_y;
        pOffset_y = 0;
        pDest_y = 0;
    }

    if (height + pDest_y + pOffset_y > pDest->height) {
        height = pDest->height - pDest_y - pOffset_y;
    }
    off_the_left = pDest_x + pOffset_x;
    if (off_the_left > 0) {
        destn_ptr += off_the_left;
    }
    for (i = 0; i < height; i++) {
        destn_ptr2 = destn_ptr;
        number_of_chunks = *pSource;
        pSource++;
        x_byte = off_the_left;
        for (j = 0; j < number_of_chunks; j++) {
            chunk_length = *pSource++;
            if (chunk_length >= 0) {
                old_x_byte = x_byte;
                x_byte += chunk_length;
                if (old_x_byte >= 0) {
                    destn_ptr2 += chunk_length;
                } else if (x_byte > 0) {
                    destn_ptr2 += chunk_length + old_x_byte;
                }
            } else {
                old_x_byte = x_byte;
                x_byte += -chunk_length;
                if (old_x_byte >= 0) {
                    if (pDest->width >= x_byte) {
                        memcpy(destn_ptr2, pSource, -chunk_length);
                        destn_ptr2 += -chunk_length;
                    } else if (old_x_byte < pDest->width) {
                        memcpy(destn_ptr2, pSource, pDest->width - old_x_byte);
                    }
                } else if (x_byte > 0) {
                    memcpy(destn_ptr2, &pSource[-old_x_byte], -chunk_length + old_x_byte);
                    destn_ptr2 += -chunk_length + old_x_byte;
                }
                pSource += -chunk_length;
            }
        }
        destn_ptr += pDest->row_bytes;
    }
}

// IDA: void __usercall SetBRenderScreenAndBuffers(int pX_offset@<EAX>, int pY_offset@<EDX>, int pWidth@<EBX>, int pHeight@<ECX>)
void SetBRenderScreenAndBuffers(int pX_offset, int pY_offset, int pWidth, int pHeight) {
    LOG_TRACE("(%d, %d, %d, %d)", pX_offset, pY_offset, pWidth, pHeight);

    PDAllocateScreenAndBack();
    if (!pWidth) {
        pWidth = gBack_screen->width;
    }
    if (!pHeight) {
        pHeight = gBack_screen->height;
    }
    gRender_screen = DRPixelmapAllocateSub(gBack_screen, pX_offset, pY_offset, pWidth, pHeight);
    gWidth = pWidth;
    gHeight = pHeight;
    gY_offset = pY_offset;
    gX_offset = pX_offset;
    if (gGraf_specs[gGraf_spec_index].doubled) {
        gScreen->base_x = (gGraf_specs[gGraf_spec_index].phys_width - 2 * gGraf_specs[gGraf_spec_index].total_width) / 2;
        gScreen->base_y = (gGraf_specs[gGraf_spec_index].phys_height - 2 * gGraf_specs[gGraf_spec_index].total_height) / 2;
    } else {
        gScreen->base_x = (gGraf_specs[gGraf_spec_index].phys_width - gGraf_specs[gGraf_spec_index].total_width) / 2;
        gScreen->base_y = (gGraf_specs[gGraf_spec_index].phys_height - gGraf_specs[gGraf_spec_index].total_height) / 2;
    }

    gScreen->origin_x = 0;
    gScreen->origin_y = 0;
    if (!gBack_screen) {
        FatalError(1);
    }
    gDepth_buffer = BrPixelmapMatch(gBack_screen, BR_PMMATCH_DEPTH_16);
    if (!gDepth_buffer) {
        FatalError(2);
    }
    BrZbBegin(gRender_screen->type, gDepth_buffer->type);
    gBrZb_initialized = 1;
}

// IDA: void __cdecl SetIntegerMapRenders()
void SetIntegerMapRenders() {
    LOG_TRACE("()");

    STUB();
}

// IDA: void __cdecl AdjustRenderScreenSize()
void AdjustRenderScreenSize() {
    int switched_res;
    LOG_TRACE("()");

    switched_res = SwitchToRealResolution();
    ReinitialiseRenderStuff();
    if (gMap_mode) {
        gRender_screen->base_x = gMap_render_x_i;
        gRender_screen->base_y = gMap_render_y_i;
        gRender_screen->width = gMap_render_width_i;
        gRender_screen->height = gMap_render_height_i;
    } else {
        gRender_screen->base_x = gProgram_state.current_render_left;
        gRender_screen->base_y = gProgram_state.current_render_top;
        gRender_screen->height = gProgram_state.current_render_bottom - gProgram_state.current_render_top;
        gRender_screen->width = gProgram_state.current_render_right - gProgram_state.current_render_left;
        LOG_DEBUG("renderscreen is %d %d, %d x %d", gRender_screen->base_x, gRender_screen->base_y, gRender_screen->width, gRender_screen->height);
    }
    if (gRender_screen->row_bytes == gRender_screen->width) {
        gRender_screen->flags |= BR_PMF_ROW_WHOLEPIXELS;
    } else {
        gRender_screen->flags &= ~BR_PMF_ROW_WHOLEPIXELS;
        // TELL_ME_IF_WE_PASS_THIS_WAY();
    }
    gRender_screen->origin_x = gRender_screen->width / 2;
    gRender_screen->origin_y = gRender_screen->height / 2;
    gWidth = gRender_screen->width;
    gHeight = gRender_screen->height;
    ReinitialiseForwardCamera();
    if (switched_res) {
        SwitchToLoresMode();
    }
}

// IDA: void __cdecl ScreenSmaller()
void ScreenSmaller() {
    LOG_TRACE("()");

    if (!gMap_mode) {
        if (gProgram_state.cockpit_on) {
            ToggleCockpit();
        }
        gRender_indent++;
        if (gRender_indent > 8) {
            gRender_indent = 8;
        }
        AdjustRenderScreenSize();
    }
}

// IDA: void __cdecl ScreenLarger()
void ScreenLarger() {
    LOG_TRACE("()");

    if (!gMap_mode) {
        if (gProgram_state.cockpit_on) {
            ToggleCockpit();
        }
        gRender_indent--;
        if (gRender_indent < 0) {
            gRender_indent = 0;
        }
        AdjustRenderScreenSize();
    }
}

// IDA: void __usercall DRSetPaletteEntries(br_pixelmap *pPalette@<EAX>, int pFirst_colour@<EDX>, int pCount@<EBX>)
void DRSetPaletteEntries(br_pixelmap* pPalette, int pFirst_colour, int pCount) {
    LOG_TRACE("(%p, %d, %d)", pPalette, pFirst_colour, pCount);
    if (!pFirst_colour) {
        ((br_int_32*)pPalette->pixels)[0] = 0;
    }
    memcpy(gCurrent_palette_pixels + 4 * pFirst_colour, (char*)pPalette->pixels + 4 * pFirst_colour, 4 * pCount);
    if (!gFaded_palette) {
        PDSetPaletteEntries(pPalette, pFirst_colour, pCount);
    }
    gPalette_munged = 1;
}

// IDA: void __usercall DRSetPalette3(br_pixelmap *pThe_palette@<EAX>, int pSet_current_palette@<EDX>)
void DRSetPalette3(br_pixelmap* pThe_palette, int pSet_current_palette) {
    LOG_TRACE("(%p, %d)", pThe_palette, pSet_current_palette);

    if (pSet_current_palette) {
        memcpy(gCurrent_palette_pixels, pThe_palette->pixels, 0x400u);
    }
    if (!gFaded_palette) {
        PDSetPalette(pThe_palette);
    }
    if (pThe_palette != gRender_palette) {
        gPalette_munged |= 1u;
    }
}

// IDA: void __usercall DRSetPalette2(br_pixelmap *pThe_palette@<EAX>, int pSet_current_palette@<EDX>)
void DRSetPalette2(br_pixelmap* pThe_palette, int pSet_current_palette) {
    ((br_int_32*)pThe_palette->pixels)[0] = 0;
    if (pSet_current_palette) {
        memcpy(gCurrent_palette_pixels, pThe_palette->pixels, 0x400u);
    }
    if (!gFaded_palette) {
        PDSetPalette(pThe_palette);
    }
    if (pThe_palette != gRender_palette) {
        gPalette_munged |= 1u;
    }
}

// IDA: void __usercall DRSetPalette(br_pixelmap *pThe_palette@<EAX>)
void DRSetPalette(br_pixelmap* pThe_palette) {
    DRSetPalette2(pThe_palette, 1);
}

// IDA: void __cdecl InitializePalettes()
void InitializePalettes() {
    int j;
    gCurrent_palette_pixels = BrMemAllocate(0x400u, kMem_cur_pal_pixels);
    gCurrent_palette = DRPixelmapAllocate(BR_PMT_RGBX_888, 1u, 256, gCurrent_palette_pixels, 0);
    gRender_palette = BrTableFind("DRRENDER.PAL");
    if (!gRender_palette) {
        FatalError(10);
    }
    gOrig_render_palette = BrPixelmapAllocateSub(gRender_palette, 0, 0, gRender_palette->width, gRender_palette->height);
    gOrig_render_palette->pixels = BrMemAllocate(0x400u, kMem_render_pal_pixels);
    memcpy(gOrig_render_palette->pixels, gRender_palette->pixels, 0x400u);
    gFlic_palette = BrTableFind("DRACEFLC.PAL");
    if (!gFlic_palette) {
        FatalError(10);
    }
    DRSetPalette2(gFlic_palette, 1);
    gScratch_pixels = BrMemAllocate(0x400u, kMem_scratch_pal_pixels);
    gScratch_palette = DRPixelmapAllocate(BR_PMT_RGBX_888, 1u, 256, gScratch_pixels, 0);
    gPalette_conversion_table = BrTableFind("FLC2REND.TAB");
    gRender_shade_table = BrTableFind("DRRENDER.TAB");
    gEval_1 = LoadPixelmap("Evalu01.PIX");
}

// IDA: void __usercall SwitchToPalette(char *pPal_name@<EAX>)
void SwitchToPalette(char* pPal_name) {
    br_pixelmap* the_palette;
    LOG_TRACE("(\"%s\")", pPal_name);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ClearEntireScreen()
void ClearEntireScreen() {
    LOG_TRACE("()");

    if (gScreen) {
        BrPixelmapFill(gScreen, gGraf_specs[gGraf_spec_index].black_value);
    }
    BrPixelmapFill(gBack_screen, gGraf_specs[gGraf_spec_index].black_value);
    PDScreenBufferSwap(0);
}

// IDA: void __cdecl ClearWobbles()
void ClearWobbles() {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < COUNT_OF(gWobble_array); ++i) {
        gWobble_array[i].amplitude_x = 0.0;
    }
}

// IDA: void __cdecl InitWobbleStuff()
void InitWobbleStuff() {
    int i;

    ClearWobbles();
    for (i = 0; i < COUNT_OF(gCosine_array); i++) {
        gCosine_array[i] = cosf(i / 64.0f * 3.141592653589793f / 2.0f);
    }
}

// IDA: void __cdecl NewScreenWobble(double pAmplitude_x, double pAmplitude_y, double pPeriod)
void NewScreenWobble(double pAmplitude_x, double pAmplitude_y, double pPeriod) {
    int i;
    int oldest_time;
    int oldest_index;
    LOG_TRACE("(%d, %d, %d)", pAmplitude_x, pAmplitude_y, pPeriod);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetScreenWobble(int pWobble_x@<EAX>, int pWobble_y@<EDX>)
void SetScreenWobble(int pWobble_x, int pWobble_y) {
    LOG_TRACE("(%d, %d)", pWobble_x, pWobble_y);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ResetScreenWobble()
void ResetScreenWobble() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CalculateWobblitude(tU32 pThe_time@<EAX>)
void CalculateWobblitude(tU32 pThe_time) {
    int i;
    tU32 time_going;
    double angle;
    double mod_angle;
    double cosine_over_angle;
    LOG_TRACE("(%d)", pThe_time);

    gScreen_wobble_x = 0;
    gScreen_wobble_y = 0;
    STUB_ONCE();
}

// IDA: void __usercall CalculateConcussion(tU32 pThe_time@<EAX>)
void CalculateConcussion(tU32 pThe_time) {
    tU32 time_difference;
    int i;
    int j;
    float the_amplitude;
    float angle;
    float mod_angle;
    float cosine_over_angle;
    LOG_TRACE("(%d)", pThe_time);

    gConcussion.concussed = 0;
    STUB_ONCE();
}

// IDA: void __cdecl SufferFromConcussion(float pSeriousness)
void SufferFromConcussion(float pSeriousness) {
    int i;
    int j;
    LOG_TRACE("(%f)", pSeriousness);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ProcessNonTrackActors(br_pixelmap *pRender_buffer@<EAX>, br_pixelmap *pDepth_buffer@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world@<ECX>, br_matrix34 *pOld_camera_matrix)
void ProcessNonTrackActors(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world, br_matrix34* pOld_camera_matrix) {
    LOG_TRACE("(%p, %p, %p, %p, %p)", pRender_buffer, pDepth_buffer, pCamera, pCamera_to_world, pOld_camera_matrix);

    BrZbSceneRenderAdd(gNon_track_actor);
}

// IDA: int __usercall OppositeColour@<EAX>(int pColour@<EAX>)
int OppositeColour(int pColour) {
    int brightness;
    LOG_TRACE("(%d)", pColour);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawMapBlip(tCar_spec *pCar@<EAX>, tU32 pTime@<EDX>, br_matrix34 *pTrans@<EBX>, br_vector3 *pPos@<ECX>, int pColour)
void DrawMapBlip(tCar_spec* pCar, tU32 pTime, br_matrix34* pTrans, br_vector3* pPos, int pColour) {
    br_vector3 map_pos;
    int offset;
    int* arrow_ptr;
    int point_count;
    int colours[2];
    int x;
    int y;
    int colour;
    int i;
    int j;
    int temp;
    int from_x;
    int from_y;
    int to_x;
    int to_y;
    int arrow_index;
    tU32 time_diff;
    tU32 period;
    br_matrix34 car_in_map_space;
    float bearing;
    float cos_factor;
    float sin_factor;
    LOG_TRACE("(%p, %d, %p, %p, %d)", pCar, pTime, pTrans, pPos, pColour);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawMapSmallBlip(tU32 pTime@<EAX>, br_vector3 *pPos@<EDX>, int pColour@<EBX>)
void DrawMapSmallBlip(tU32 pTime, br_vector3* pPos, int pColour) {
    br_vector3 map_pos;
    int offset;
    tU32 time_diff;
    LOG_TRACE("(%d, %p, %d)", pTime, pPos, pColour);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MungeClipPlane(br_vector3 *pLight@<EAX>, tCar_spec *pCar@<EDX>, br_vector3 *p1@<EBX>, br_vector3 *p2@<ECX>, br_scalar pY_offset)
void MungeClipPlane(br_vector3* pLight, tCar_spec* pCar, br_vector3* p1, br_vector3* p2, br_scalar pY_offset) {
    br_vector3 v1;
    br_vector3 v2;
    br_vector3 v3;
    br_vector3 v4;
    br_scalar length;
    br_actor* new_clip;
    LOG_TRACE("(%p, %p, %p, %p, %f)", pLight, pCar, p1, p2, pY_offset);

    BrMatrix34ApplyP(&v1, p1, &pCar->car_master_actor->t.t.mat);
    BrMatrix34ApplyP(&v2, p2, &pCar->car_master_actor->t.t.mat);
    v3.v[0] = p2->v[0] - p1->v[0];
    v3.v[1] = p2->v[1] - p1->v[1];
    v3.v[2] = p2->v[2] - p1->v[2];
    v4.v[0] = pLight->v[2] * v3.v[1] - pLight->v[1] * v3.v[2];
    v4.v[1] = pLight->v[0] * v3.v[2] - pLight->v[2] * v3.v[0];
    v4.v[2] = pLight->v[1] * v3.v[0] - v3.v[1] * pLight->v[0];
    if (fabs(v4.v[0]) >= 0.01 || fabs(v4.v[1]) >= 0.01 || fabs(v4.v[2]) >= 0.01) {
        v3 = *p1;
        v3.v[1] = v3.v[1] - pY_offset;
        if (v3.v[1] * v4.v[1] + v4.v[2] * v3.v[2] + v4.v[0] * v3.v[0] > 0.0) {
            BrVector3Negate(&v4, &v4);
        }
        BrVector3Normalise(&v3, &v4);
        BrMatrix34ApplyV(&v4, &v3, &pCar->car_master_actor->t.t.mat);
        length = (v1.v[2] - v2.v[2]) * (v1.v[2] - v2.v[2]) + (v1.v[0] - v2.v[0]) * (v1.v[0] - v2.v[0]);

        new_clip = gShadow_clip_planes[gShadow_clip_plane_count].clip;
        ((br_vector4*)new_clip->type_data)->v[0] = v4.v[0];
        ((br_vector4*)new_clip->type_data)->v[1] = v4.v[1];
        ((br_vector4*)new_clip->type_data)->v[2] = v4.v[2];
        ((br_vector4*)new_clip->type_data)->v[3] = -(v4.v[1] * v1.v[1] + v4.v[2] * v1.v[2] + v1.v[0] * v4.v[0]);
        gShadow_clip_planes[gShadow_clip_plane_count].length = length;
        gShadow_clip_plane_count++;
    }
}

// IDA: void __usercall TryThisEdge(tCar_spec *pCar@<EAX>, br_vector3 *pLight@<EDX>, int pIndex_1@<EBX>, br_scalar pSign_1, int pIndex_2, br_scalar pSign_2, int pPoint_index_1, int pPoint_index_2, br_scalar pY_offset)
void TryThisEdge(tCar_spec* pCar, br_vector3* pLight, int pIndex_1, br_scalar pSign_1, int pIndex_2, br_scalar pSign_2, int pPoint_index_1, int pPoint_index_2, br_scalar pY_offset) {
    br_scalar dot_1;
    br_scalar dot_2;
    br_scalar mult;
    LOG_TRACE("(%p, %p, %d, %f, %d, %f, %d, %d, %f)", pCar, pLight, pIndex_1, pSign_1, pIndex_2, pSign_2, pPoint_index_1, pPoint_index_2, pY_offset);

    dot_1 = pSign_1 * pLight->v[pIndex_1];
    dot_2 = pSign_2 * pLight->v[pIndex_2];
    mult = dot_1 * dot_2;
    if (mult < 0 || (mult == 0 && (dot_1 > 0 || dot_2 > 0))) {
        if (gShadow_clip_plane_count < 6) {
            MungeClipPlane(pLight, pCar, &gShadow_points[pPoint_index_1], &gShadow_points[pPoint_index_2], pY_offset);
        }
    }
}

// IDA: br_scalar __usercall DistanceFromPlane@<ST0>(br_vector3 *pPos@<EAX>, br_scalar pA, br_scalar pB, br_scalar pC, br_scalar pD)
br_scalar DistanceFromPlane(br_vector3* pPos, br_scalar pA, br_scalar pB, br_scalar pC, br_scalar pD) {
    br_vector3 normal;
    LOG_TRACE("(%p, %f, %f, %f, %f)", pPos, pA, pB, pC, pD);

    return fabs((pPos->v[1] * pB + pPos->v[0] * pA + pPos->v[2] * pC + pD) / (pA * pA + pC * pC + pB * pB));
}

// IDA: void __cdecl DisableLights()
void DisableLights() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl EnableLights()
void EnableLights() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ProcessShadow(tCar_spec *pCar@<EAX>, br_actor *pWorld@<EDX>, tTrack_spec *pTrack_spec@<EBX>, br_actor *pCamera@<ECX>, br_matrix34 *pCamera_to_world_transform, br_scalar pDistance_factor)
void ProcessShadow(tCar_spec* pCar, br_actor* pWorld, tTrack_spec* pTrack_spec, br_actor* pCamera, br_matrix34* pCamera_to_world_transform, br_scalar pDistance_factor) {
    int i;
    int j;
    int face_count;
    int force_shadow;
    int models_used;
    int point_to_use;
    int oily_count;
    int f_num;
    br_vector3 pos;
    br_vector3 light_ray_car;
    br_vector3 temp_v;
    br_vector3 shadow_points_world[8];
    br_vector3 poly_centre;
    br_vector3 car_to_poly;
    br_vector3 ray;
    br_vector3 ray_pos;
    br_vector3 normal;
    br_vector3 the_normal;
    br_vector3 pos_cam_space;
    br_vector3* v0;
    br_vector3* v1;
    br_vector3* v2;
    br_vector4* clip_normal;
    br_scalar bounds_x_min;
    br_scalar bounds_x_max;
    br_scalar bounds_y_min;
    br_scalar bounds_y_max;
    br_scalar bounds_z_min;
    br_scalar bounds_z_max;
    br_scalar height;
    br_scalar oily_size;
    br_scalar highest_underneath;
    br_scalar shadow_scaling_factor;
    br_scalar y_offset;
    br_scalar most_neg_dotty;
    br_scalar mr_dotty;
    br_scalar first_poly_below;
    br_scalar distance;
    br_scalar camera_hither_fudge;
    br_scalar camera_angle_additional_fudge;
    br_scalar ray_length;
    tBounds kev_bounds;
    tFace_ref the_list[100];
    tFace_ref* face_ref;
    tFace_ref* list_ptr;
    br_renderbounds_cbfn* old_call_back;
    br_camera* camera_ptr;
    br_actor* oily_actor;
    br_model* model;
    br_material* material;
    br_vertex verts[48];
    br_face faces[16];
    LOG_TRACE("(%p, %p, %p, %p, %p, %f)", pCar, pWorld, pTrack_spec, pCamera, pCamera_to_world_transform, pDistance_factor);

    f_num = 0;
    bounds_x_min = pCar->bounds[1].min.v[0] / WORLD_SCALE;
    bounds_x_max = pCar->bounds[1].max.v[0] / WORLD_SCALE;
    bounds_y_min = pCar->bounds[1].min.v[1] / WORLD_SCALE;
    bounds_y_max = pCar->bounds[1].max.v[1] / WORLD_SCALE;
    bounds_z_min = pCar->bounds[1].min.v[2] / WORLD_SCALE;
    bounds_z_max = pCar->bounds[1].max.v[2] / WORLD_SCALE;
    gShadow_points[0].v[0] = bounds_x_max;
    gShadow_points[0].v[1] = bounds_y_max;
    gShadow_points[0].v[2] = bounds_z_max;
    gShadow_points[1].v[0] = bounds_x_max;
    gShadow_points[1].v[1] = bounds_y_max;
    gShadow_points[1].v[2] = bounds_z_min;
    gShadow_points[2].v[0] = bounds_x_min;
    gShadow_points[2].v[1] = bounds_y_max;
    gShadow_points[2].v[2] = bounds_z_min;
    gShadow_points[3].v[0] = bounds_x_min;
    gShadow_points[3].v[1] = bounds_y_max;
    gShadow_points[3].v[2] = bounds_z_max;
    gShadow_points[4].v[0] = bounds_x_max;
    gShadow_points[4].v[1] = bounds_y_min;
    gShadow_points[4].v[2] = bounds_z_max;
    gShadow_points[5].v[0] = bounds_x_max;
    gShadow_points[5].v[1] = bounds_y_min;
    gShadow_points[5].v[2] = bounds_z_min;
    gShadow_points[6].v[0] = bounds_x_min;
    gShadow_points[6].v[1] = bounds_y_min;
    gShadow_points[6].v[2] = bounds_z_min;
    gShadow_points[7].v[0] = bounds_x_min;
    gShadow_points[7].v[1] = bounds_y_min;
    gShadow_points[7].v[2] = bounds_z_max;
    gShadow_clip_plane_count = 0;
    BrMatrix34TApplyV(&light_ray_car, &gShadow_light_ray, &pCar->car_master_actor->t.t.mat);
    y_offset = (bounds_y_max + bounds_y_min) / 2.0;
    TryThisEdge(pCar, &light_ray_car, 2, 1.0, 1, 1.0, 0, 3, y_offset);
    TryThisEdge(pCar, &light_ray_car, 2, -1.0, 1, 1.0, 1, 2, y_offset);
    TryThisEdge(pCar, &light_ray_car, 2, -1.0, 1, -1.0, 6, 5, y_offset);
    TryThisEdge(pCar, &light_ray_car, 2, 1.0, 1, -1.0, 7, 4, y_offset);
    TryThisEdge(pCar, &light_ray_car, 0, 1.0, 1, 1.0, 1, 0, y_offset);
    TryThisEdge(pCar, &light_ray_car, 0, -1.0, 1, 1.0, 2, 3, y_offset);
    TryThisEdge(pCar, &light_ray_car, 0, -1.0, 1, -1.0, 7, 6, y_offset);
    TryThisEdge(pCar, &light_ray_car, 0, 1.0, 1, -1.0, 4, 5, y_offset);
    TryThisEdge(pCar, &light_ray_car, 0, 1.0, 2, 1.0, 4, 0, y_offset);
    TryThisEdge(pCar, &light_ray_car, 0, -1.0, 2, 1.0, 3, 7, y_offset);
    TryThisEdge(pCar, &light_ray_car, 0, -1.0, 2, -1.0, 2, 6, y_offset);
    TryThisEdge(pCar, &light_ray_car, 0, 1.0, 2, -1.0, 5, 1, y_offset);
    for (i = 0; i < gShadow_clip_plane_count; ++i) {
        BrClipPlaneEnable(gShadow_clip_planes[i].clip);
    }
    face_count = GetPrecalculatedFacesUnderCar(pCar, &face_ref);

    if (!gAction_replay_mode && pCar->number_of_wheels_on_ground >= 3 && face_count != 0) {
        highest_underneath = 0.0;
    } else {
        kev_bounds.original_bounds.min.v[0] = 1000.0;
        kev_bounds.original_bounds.min.v[1] = 1000.0;
        kev_bounds.original_bounds.min.v[2] = 1000.0;
        kev_bounds.original_bounds.max.v[0] = -1000.0;
        kev_bounds.original_bounds.max.v[1] = -1000.0;
        kev_bounds.original_bounds.max.v[2] = -1000.0;
        for (i = 0; i < 8; i++) {
            BrMatrix34ApplyP(&shadow_points_world[i], &gShadow_points[i], &pCar->car_master_actor->t.t.mat);
            if (shadow_points_world[i].v[0] >= kev_bounds.original_bounds.min.v[0]) {
                if (shadow_points_world[i].v[0] > kev_bounds.original_bounds.max.v[0]) {
                    kev_bounds.original_bounds.max.v[0] = shadow_points_world[i].v[0];
                }
            } else {
                kev_bounds.original_bounds.min.v[0] = shadow_points_world[i].v[0];
            }
            if (shadow_points_world[i].v[1] >= kev_bounds.original_bounds.min.v[1]) {
                if (shadow_points_world[i].v[1] > kev_bounds.original_bounds.max.v[1]) {
                    kev_bounds.original_bounds.max.v[1] = shadow_points_world[i].v[1];
                }
            } else {
                kev_bounds.original_bounds.min.v[1] = shadow_points_world[i].v[1];
            }
            if (shadow_points_world[i].v[2] >= kev_bounds.original_bounds.min.v[2]) {
                if (shadow_points_world[i].v[2] > kev_bounds.original_bounds.max.v[2]) {
                    kev_bounds.original_bounds.max.v[2] = shadow_points_world[i].v[2];
                }
            } else {
                kev_bounds.original_bounds.min.v[2] = shadow_points_world[i].v[2];
            }
        }
        kev_bounds.original_bounds.min.v[1] = kev_bounds.original_bounds.min.v[1] - 4.4000001;
        kev_bounds.mat = &gIdentity34;
        face_count = FindFacesInBox(&kev_bounds, the_list, 100);
        face_ref = the_list;
        highest_underneath = 1000.0;
        ray_length = kev_bounds.original_bounds.max.v[1] - kev_bounds.original_bounds.min.v[1];
        ray.v[0] = 0.0;
        ray.v[1] = -ray_length;
        ray.v[2] = 0.0;
        ray_pos = pCar->car_master_actor->t.t.translate.t;
        ray_pos.v[1] = kev_bounds.original_bounds.max.v[1];
    }
    if (face_count) {
        first_poly_below = -1000.0;
        i = 0;
        list_ptr = face_ref;
        for (i = 0; i < face_count; i++) {
            v1 = &list_ptr->v[1];
            v2 = &list_ptr->v[2];
            if (list_ptr->normal.v[1] >= -0.1 || (list_ptr->material && (list_ptr->material->flags & 0x1000) != 0)) {
                if (list_ptr->normal.v[1] < 0.0 || list_ptr->material && ((list_ptr->material->identifier && *list_ptr->material->identifier == '!') || list_ptr->material->index_blend)) {
                    list_ptr->d = 10000.0;
                } else if ((list_ptr->v[0].v[1] > pCar->pos.v[1] || v1->v[1] > pCar->pos.v[1] || v2->v[1] > pCar->pos.v[1]) && list_ptr->normal.v[1] < 0.1) {
                    list_ptr->d = 10000.0;
                }
            } else {
                poly_centre.v[0] = v1->v[0] + list_ptr->v[0].v[0];
                poly_centre.v[1] = v1->v[1] + list_ptr->v[0].v[1];
                poly_centre.v[2] = v1->v[2] + list_ptr->v[0].v[2];
                poly_centre.v[0] = v2->v[0] + poly_centre.v[0];
                poly_centre.v[1] = v2->v[1] + poly_centre.v[1];
                poly_centre.v[2] = v2->v[2] + poly_centre.v[2];
                poly_centre.v[0] = poly_centre.v[0] / 3.0;
                poly_centre.v[1] = poly_centre.v[1] / 3.0;
                poly_centre.v[2] = poly_centre.v[2] / 3.0;
                poly_centre.v[1] = (v2->v[1] + v1->v[1] + list_ptr->v[0].v[1]) / 3.0;
                if (poly_centre.v[1] > first_poly_below) {
                    car_to_poly.v[0] = poly_centre.v[0] - pCar->car_master_actor->t.t.mat.m[3][0];
                    car_to_poly.v[1] = poly_centre.v[1] - pCar->car_master_actor->t.t.mat.m[3][1];
                    car_to_poly.v[2] = poly_centre.v[2] - pCar->car_master_actor->t.t.mat.m[3][2];

                    if (BrVector3Dot(&list_ptr->normal, &car_to_poly) > 0.0) {
                        first_poly_below = poly_centre.v[1];
                    }
                }
                list_ptr->d = 10000.0;
            }
            list_ptr++;
        }
        list_ptr = face_ref;
        for (i = 0; i < face_count; i++) {
            if (list_ptr->d == 10000.0) {
                continue;
            }
            if (list_ptr->v[0].v[1] >= first_poly_below || list_ptr->v[1].v[1] >= first_poly_below || list_ptr->v[2].v[1] >= first_poly_below) {
                if (gFancy_shadow) {
                    faces[f_num].material = list_ptr->material;
                    if (list_ptr->material && list_ptr->material->colour_map && (list_ptr->material->flags & BR_MATF_LIGHT) == 0) {
                        list_ptr->material->flags |= BR_MATF_SMOOTH | BR_MATF_LIGHT;
                        BrMaterialUpdate(list_ptr->material, BR_MATU_RENDERING);
                    }
                } else {
                    faces[f_num].material = gShadow_material;
                }

                verts[3 * f_num].p.v[0] = list_ptr->v[0].v[0];
                verts[3 * f_num].p.v[1] = list_ptr->v[0].v[1];
                verts[3 * f_num].p.v[2] = list_ptr->v[0].v[2];
                verts[3 * f_num + 1].p.v[0] = list_ptr->v[1].v[0];
                verts[3 * f_num + 1].p.v[1] = list_ptr->v[1].v[1];
                verts[3 * f_num + 1].p.v[2] = list_ptr->v[1].v[2];
                verts[3 * f_num + 2].p.v[0] = list_ptr->v[2].v[0];
                verts[3 * f_num + 2].p.v[1] = list_ptr->v[2].v[1];
                verts[3 * f_num + 2].p.v[2] = list_ptr->v[2].v[2];
                verts[3 * f_num].map.v[0] = list_ptr->map[0]->v[0];
                verts[3 * f_num].map.v[1] = list_ptr->map[0]->v[1];
                verts[3 * f_num + 1].map.v[0] = list_ptr->map[1]->v[0];
                verts[3 * f_num + 1].map.v[1] = list_ptr->map[1]->v[1];
                verts[3 * f_num + 2].map.v[0] = list_ptr->map[2]->v[0];
                verts[3 * f_num + 2].map.v[1] = list_ptr->map[2]->v[1];
                faces[f_num].vertices[0] = 3 * f_num;
                faces[f_num].vertices[1] = 3 * f_num + 1;
                faces[f_num].vertices[2] = 3 * f_num + 2;
                f_num++;
                if (highest_underneath > 0.0) {
                    CheckSingleFace(list_ptr, &ray_pos, &ray, &normal, &distance);
                    if (distance < 1.0 && ray_length * distance < highest_underneath) {
                        highest_underneath = ray_length * distance;
                        // v22 = normal;
                    }
                }
                if (f_num >= 16) {
                    break;
                }
            }
            list_ptr++;
        }
        highest_underneath = highest_underneath - (bounds_y_max - bounds_y_min);
        if (highest_underneath < 2.2) {
            if (highest_underneath < 0.0) {
                highest_underneath = 0.0;
            }
        } else {
            highest_underneath = 2.2;
        }
        if (gFancy_shadow) {
            gShadow_dim_amount = ((2.2 - highest_underneath) * 5.0 / 2.2 + 2.5);
            for (i = 0; i < gSaved_table_count; i++) {
                gSaved_shade_tables[i].original->height = 1;
                gSaved_shade_tables[i].original->pixels = (char*)gDepth_shade_table->pixels + gShadow_dim_amount * gDepth_shade_table->row_bytes;
                BrTableUpdate(gSaved_shade_tables[i].original, BR_TABU_ALL);
            }
        }
        shadow_scaling_factor = (2.2 - highest_underneath) * 0.52 / 2.2 + 0.4;
        for (i = 0; i < gShadow_clip_plane_count; i++) {
            clip_normal = (br_vector4*)gShadow_clip_planes[i].clip->type_data;
            distance = DistanceFromPlane(&pCar->car_master_actor->t.t.euler.t, clip_normal->v[0], clip_normal->v[1], clip_normal->v[2], clip_normal->v[3]);
            gShadow_clip_planes[i].clip->t.t.mat.m[3][0] = (1.0 - shadow_scaling_factor) * distance * clip_normal->v[0];
            gShadow_clip_planes[i].clip->t.t.mat.m[3][1] = (1.0 - shadow_scaling_factor) * distance * clip_normal->v[1];
            gShadow_clip_planes[i].clip->t.t.mat.m[3][2] = (1.0 - shadow_scaling_factor) * distance * clip_normal->v[2];
        }

        camera_ptr = (br_camera*)gCamera->type_data;
        DRMatrix34TApplyP(&pos_cam_space, &pCar->car_master_actor->t.t.euler.t, &gCamera_to_world);
        if (pos_cam_space.v[2] >= 36.0 || pos_cam_space.v[2] >= camera_ptr->yon_z) {
            camera_hither_fudge = 0.0;
        } else {
            camera_angle_additional_fudge = sqr(camera_ptr->yon_z - camera_ptr->hither_z);
            camera_hither_fudge = camera_angle_additional_fudge * (pos_cam_space.v[2] * 1.0) / ((pos_cam_space.v[2] - camera_ptr->yon_z) * camera_ptr->yon_z * 65536.0);
            if (camera_hither_fudge < 0.0002) {
                camera_hither_fudge = 0.0002;
            }
            camera_ptr->hither_z += camera_hither_fudge;
        }
        if (f_num) {
            BrZbSceneRenderBegin(gUniverse_actor, gCamera, gRender_screen, gDepth_buffer);
            gShadow_model->vertices = verts;
            gShadow_model->faces = faces;
            gShadow_model->nfaces = f_num;
            gShadow_model->nvertices = 3 * f_num;
            gShadow_actor->render_style = BR_RSTYLE_FACES;
            BrModelAdd(gShadow_model);
            BrZbSceneRenderAdd(gShadow_actor);
            BrModelRemove(gShadow_model);
            if (pCar->shadow_intersection_flags) {
                oily_count = GetOilSpillCount();
                for (i = 0; i < oily_count; ++i) {
                    if (((1 << i) & pCar->shadow_intersection_flags) != 0) {
                        GetOilSpillDetails(i, &oily_actor, &oily_size);
                        if (oily_actor) {
                            MungeIndexedOilsHeightAboveGround(i);
                            BrZbSceneRenderAdd(oily_actor);
                        }
                    }
                }
            }
            BrZbSceneRenderEnd();
        }
        camera_ptr->hither_z -= camera_hither_fudge;
        for (i = 0; i < f_num; i++) {
            if (gFancy_shadow) {
                material = gShadow_model->faces[i].material;
                if (material) {
                    if (material->colour_map && (material->flags & BR_MATF_LIGHT) != 0) {
                        material->flags &= 0xFFFFFFF8;
                        BrMaterialUpdate(material, BR_MATU_RENDERING);
                    }
                }
            }
        }
    }
    gShadow_actor->render_style = BR_RSTYLE_NONE;
    for (i = 0; i < gShadow_clip_plane_count; i++) {
        BrClipPlaneDisable(gShadow_clip_planes[i].clip);
    }
}

// IDA: void __usercall RenderShadows(br_actor *pWorld@<EAX>, tTrack_spec *pTrack_spec@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world_transform@<ECX>)
void RenderShadows(br_actor* pWorld, tTrack_spec* pTrack_spec, br_actor* pCamera, br_matrix34* pCamera_to_world_transform) {
    int i;
    int cat;
    int car_count;
    tCar_spec* the_car;
    br_vector3 camera_to_car;
    br_scalar distance_factor;
    LOG_TRACE("(%p, %p, %p, %p)", pWorld, pTrack_spec, pCamera, pCamera_to_world_transform);

    if (gShadow_level == eShadow_none) {
        return;
    }
    for (cat = eVehicle_self;; ++cat) {
        if (gShadow_level == eShadow_everyone) {
            if (cat > 4) {
                break;
            }
        } else {
            if (cat > (gShadow_level == eShadow_us_and_opponents ? 3 : 0)) {
                break;
            }
        }

        if (cat == eVehicle_self) {
            car_count = 1;
        } else {
            car_count = GetCarCount(cat);
        }
        for (i = 0; i < car_count; i++) {
            if (cat == eVehicle_self) {
                the_car = &gProgram_state.current_car;
            } else {
                the_car = GetCarSpec(cat, i);
            }
            if (!the_car->active) {
                continue;
            }

            BrVector3Sub(&camera_to_car, (br_vector3*)gCamera_to_world.m[3], &the_car->car_master_actor->t.t.translate.t);
            distance_factor = BrVector3LengthSquared(&camera_to_car);
            if (gAction_replay_mode || distance_factor <= SHADOW_MAX_RENDER_DISTANCE) {
                ProcessShadow(the_car, gUniverse_actor, &gProgram_state.track_spec, gCamera, &gCamera_to_world, distance_factor);
            }
        }
    }
    if (gFancy_shadow) {
        for (i = 0; i < gSaved_table_count; i++) {
            gSaved_shade_tables[i].original->height = gSaved_shade_tables[i].copy->height;
            gSaved_shade_tables[i].original->pixels = gSaved_shade_tables[i].copy->pixels;
            BrTableUpdate(gSaved_shade_tables[i].original, 0x7FFF);
        }
    }
}

// IDA: void __usercall FlashyMapCheckpoint(int pIndex@<EAX>, tU32 pTime@<EDX>)
void FlashyMapCheckpoint(int pIndex, tU32 pTime) {
    tCheckpoint* cp;
    static tU32 last_flash;
    static int flash_state;
    LOG_TRACE("(%d, %d)", pIndex, pTime);
    STUB_ONCE();
}

// IDA: int __usercall ConditionallyFillWithSky@<EAX>(br_pixelmap *pPixelmap@<EAX>)
int ConditionallyFillWithSky(br_pixelmap* pPixelmap) {
    int bgnd_col;
    LOG_TRACE("(%p)", pPixelmap);

    STUB_ONCE();
    return 0;
}

// IDA: void __usercall RenderAFrame(int pDepth_mask_on@<EAX>)
void RenderAFrame(int pDepth_mask_on) {
    int cat;
    int i;
    int car_count;
    int flags;
    int x_shift;
    int y_shift;
    int cockpit_on;
    int real_origin_x = 0;
    int real_origin_y = 0;
    int real_base_x = 0;
    int real_base_y = 0;
    int map_timer_x;
    int map_timer_width;
    int ped_type;
    char* old_pixels;
    br_matrix34 old_camera_matrix;
    br_matrix34 old_mirror_cam_matrix;
    tU32 the_time;
    br_vector3* car_pos;
    br_vector3 pos;
    char the_text[256];
    tCar_spec* car;
    LOG_TRACE("(%d)", pDepth_mask_on);

    static int frame_counter;

    frame_counter++;

    gRender_screen->pixels = gBack_screen->pixels;
    the_time = GetTotalTime();
    old_pixels = gRender_screen->pixels;
    cockpit_on = gProgram_state.cockpit_on && gProgram_state.cockpit_image_index >= 0 && !gMap_mode;
    gMirror_on__graphics = gProgram_state.mirror_on && cockpit_on && gProgram_state.which_view == eView_forward;
    if (gMap_mode) {
        real_origin_x = gBack_screen->origin_x;
        real_origin_y = gBack_screen->origin_y;
        real_base_x = gBack_screen->base_x;
        real_base_y = gBack_screen->base_y;
        gBack_screen->origin_x = 0;
        gBack_screen->origin_y = 0;
        gBack_screen->base_x = 0;
        gBack_screen->base_y = 0;
        if (gCurrent_race.map_image) {
            if (gReal_graf_data_index) {
                BrPixelmapRectangleFill(gBack_screen, 0, 0, 640, 40, 0);
                BrPixelmapRectangleFill(gBack_screen, 0, 440, 640, 40, 0);
                DRPixelmapDoubledCopy(
                    gBack_screen,
                    gCurrent_race.map_image,
                    gCurrent_race.map_image->width,
                    gCurrent_race.map_image->height,
                    0,
                    40);
            } else {
                DRPixelmapCopy(gBack_screen, gCurrent_race.map_image);
            }
        }
        DimRectangle(
            gBack_screen,
            gMap_render_x_i - gCurrent_graf_data->map_render_x_marg,
            gMap_render_y_i - gCurrent_graf_data->map_render_y_marg,
            gMap_render_x_i + gMap_render_width_i + gCurrent_graf_data->map_render_x_marg,
            gMap_render_y_i + gMap_render_height_i + gCurrent_graf_data->map_render_y_marg,
            1);
    }
    if (!gAction_replay_mode) {
        CalculateWobblitude(the_time);
    }
    if (cockpit_on) {
        if (-gScreen_wobble_x <= gX_offset) {
            if (gScreen_wobble_x + gX_offset + gRender_screen->width <= gBack_screen->width) {
                x_shift = gScreen_wobble_x;
            } else {
                x_shift = gBack_screen->width - gRender_screen->width - gX_offset;
            }
        } else {
            x_shift = -gX_offset;
        }
        if (-gScreen_wobble_y <= gY_offset) {
            if (gScreen_wobble_y + gY_offset + gRender_screen->height <= gBack_screen->height) {
                y_shift = gScreen_wobble_y;
            } else {
                y_shift = gBack_screen->height - gRender_screen->height - gY_offset;
            }
        } else {
            y_shift = -gY_offset;
        }
    } else {
        x_shift = 0;
        y_shift = 0;
    }
    old_camera_matrix = gCamera->t.t.mat;
    if (gMirror_on__graphics) {
        old_mirror_cam_matrix = gRearview_camera->t.t.mat;
    }
    if (cockpit_on) {
        gSheer_mat.m[2][1] = (double)y_shift / (double)gRender_screen->height;
        gSheer_mat.m[2][0] = (double)-x_shift / (double)gRender_screen->width;
        BrMatrix34Pre(&gCamera->t.t.mat, &gSheer_mat);
        gCamera->t.t.mat.m[3][0] = gCamera->t.t.mat.m[3][0]
            - (double)gScreen_wobble_x * 1.5 / (double)gRender_screen->width / 6.9000001;
        gCamera->t.t.mat.m[3][1] = (double)gScreen_wobble_y * 1.5 / (double)gRender_screen->width / 6.9000001
            + gCamera->t.t.mat.m[3][1];
    }
    gRender_screen->pixels = (char*)gRender_screen->pixels + x_shift + y_shift * gRender_screen->row_bytes;
    CalculateConcussion(the_time);
    BrPixelmapRectangleFill(gDepth_buffer, 0, 0, gRender_screen->width, gRender_screen->height, 0xFFFFFFFF);
    if (gRender_indent && !gMap_mode) {
        BrPixelmapRectangleFill(
            gBack_screen,
            0,
            0,
            gGraf_specs[gGraf_spec_index].total_width,
            gProgram_state.current_render_top,
            0);
        BrPixelmapRectangleFill(
            gBack_screen,
            0,
            gProgram_state.current_render_bottom,
            gGraf_specs[gGraf_spec_index].total_width,
            gGraf_specs[gGraf_spec_index].total_height - gProgram_state.current_render_bottom,
            0);
        BrPixelmapRectangleFill(
            gBack_screen,
            0,
            gProgram_state.current_render_top,
            gProgram_state.current_render_left,
            gProgram_state.current_render_bottom - gProgram_state.current_render_top,
            0);
        BrPixelmapRectangleFill(
            gBack_screen,
            gProgram_state.current_render_right,
            gProgram_state.current_render_top,
            gGraf_specs[gGraf_spec_index].total_width - gProgram_state.current_render_right,
            gProgram_state.current_render_bottom - gProgram_state.current_render_top,
            0);
    }
    gRendering_mirror = 0;
    DoSpecialCameraEffect(gCamera, &gCamera_to_world);
    if (!ConditionallyFillWithSky(gRender_screen)
        && !gProgram_state.cockpit_on
        && !(gAction_replay_camera_mode && gAction_replay_mode)) {
        ExternalSky(gRender_screen, gDepth_buffer, gCamera, &gCamera_to_world);
    }
    for (i = 0; i < (gMap_mode == 0 ? 1 : 3); i++) {
        RenderShadows(gUniverse_actor, &gProgram_state.track_spec, gCamera, &gCamera_to_world);
        BrZbSceneRenderBegin(gUniverse_actor, gCamera, gRender_screen, gDepth_buffer);
        ProcessNonTrackActors(gRender_screen, gDepth_buffer, gCamera, &gCamera_to_world, &old_camera_matrix);
        ProcessTrack(gUniverse_actor, &gProgram_state.track_spec, gCamera, &gCamera_to_world, 0);
        RenderLollipops();
        DepthEffectSky(gRender_screen, gDepth_buffer, gCamera, &gCamera_to_world);
        DepthEffect(gRender_screen, gDepth_buffer, gCamera, &gCamera_to_world);
        if (!gAusterity_mode) {
            ProcessTrack(gUniverse_actor, &gProgram_state.track_spec, gCamera, &gCamera_to_world, 1);
        }
        RenderSplashes();
        RenderSmoke(gRender_screen, gDepth_buffer, gCamera, &gCamera_to_world, gFrame_period);
        RenderSparks(gRender_screen, gDepth_buffer, gCamera, &gCamera_to_world, gFrame_period);
        RenderProximityRays(gRender_screen, gDepth_buffer, gCamera, &gCamera_to_world, gFrame_period);
        BrZbSceneRenderEnd();
    }
    gCamera->t.t.mat = old_camera_matrix;
    if (gMirror_on__graphics) {
        // LOG_PANIC("mirror is on");
        BrPixelmapFill(gRearview_depth_buffer, 0xFFFFFFFF);
        gRendering_mirror = 1;
        DoSpecialCameraEffect(gRearview_camera, &gRearview_camera_to_world);
        ConditionallyFillWithSky(gRearview_screen);
        BrZbSceneRenderBegin(gUniverse_actor, gRearview_camera, gRearview_screen, gRearview_depth_buffer);
        ProcessNonTrackActors(
            gRearview_screen,
            gRearview_depth_buffer,
            gRearview_camera,
            &gRearview_camera_to_world,
            &old_mirror_cam_matrix);
        ProcessTrack(gUniverse_actor, &gProgram_state.track_spec, gRearview_camera, &gRearview_camera_to_world, 0);
        RenderLollipops();
        DepthEffectSky(gRearview_screen, gRearview_depth_buffer, gRearview_camera, &gRearview_camera_to_world);
        DepthEffect(gRearview_screen, gRearview_depth_buffer, gRearview_camera, &gRearview_camera_to_world);
        if (!gAusterity_mode) {
            ProcessTrack(gUniverse_actor, &gProgram_state.track_spec, gRearview_camera, &gRearview_camera_to_world, 1);
        }
        RenderSplashes();
        BrZbSceneRenderEnd();
        gRearview_camera->t.t.mat = old_mirror_cam_matrix;
        gRendering_mirror = 0;
    }
    if (gMap_mode) {
        if (gNet_mode == eNet_mode_none) {
            GetTimerString(the_text, 0);
            map_timer_width = DRTextWidth(&gFonts[2], the_text);
            map_timer_x = gCurrent_graf_data->map_timer_text_x - map_timer_width;
            BrPixelmapRectangleFill(
                gBack_screen,
                map_timer_x - gCurrent_graf_data->map_timer_border_x,
                gCurrent_graf_data->map_timer_text_y - gCurrent_graf_data->map_timer_border_y,
                map_timer_width + 2 * gCurrent_graf_data->map_timer_border_x,
                gFonts[2].height + 2 * gCurrent_graf_data->map_timer_border_y,
                0);
            TransDRPixelmapText(
                gBack_screen,
                map_timer_x,
                gCurrent_graf_data->map_timer_text_y,
                &gFonts[2],
                the_text,
                gBack_screen->width);
        }
        the_time = PDGetTotalTime();
        if (gNet_mode) {
            if (gCurrent_net_game->type == eNet_game_type_checkpoint) {
                flags = gNet_players[gThis_net_player_index].score;
                for (i = 0; gCurrent_race.check_point_count > i; ++i) {
                    if ((flags & 1) != 0) {
                        FlashyMapCheckpoint(i, the_time);
                    }
                    flags >>= 1;
                }
            } else if (gCurrent_net_game->type == eNet_game_type_sudden_death
                && gNet_players[gThis_net_player_index].score >= 0) {
                FlashyMapCheckpoint(
                    gNet_players[gThis_net_player_index].score % gCurrent_race.check_point_count,
                    the_time);
            }
        } else {
            FlashyMapCheckpoint(gCheckpoint - 1, the_time);
        }
        if (gShow_peds_on_map || (gNet_mode && gCurrent_net_game->options.show_powerups_on_map)) {
            for (i = 0; GetPedCount() > i; ++i) {
                ped_type = GetPedPosition(i, &pos);
                if (ped_type > 0 && gShow_peds_on_map) {
                    DrawMapSmallBlip(the_time, &pos, 52);
                } else if (ped_type < 0 && (gNet_mode && gCurrent_net_game->options.show_powerups_on_map)) {
                    DrawMapSmallBlip(the_time, &pos, 4);
                }
            }
        }
        if (gShow_opponents) {
            cat = eVehicle_opponent;
        } else {
            cat = eVehicle_self;
        }
        while (cat >= eVehicle_self) {
            if (cat) {
                car_count = GetCarCount(cat);
            } else {
                car_count = 1;
            }
            for (i = 0; i < car_count; i++) {
                if (cat) {
                    car = GetCarSpec(cat, i);
                } else {
                    car = &gProgram_state.current_car;
                }
                if (gNet_mode == eNet_mode_none || (!car->knackered && !NetPlayerFromCar(car)->wasted)) {
                    if (cat) {
                        car_pos = &GetCarSpec(cat, i)->car_master_actor->t.t.euler.t;
                    } else {
                        car_pos = &gSelf->t.t.euler.t;
                    }
                    if (gNet_mode) {
                        DrawMapBlip(
                            car,
                            the_time,
                            &car->car_master_actor->t.t.mat,
                            car_pos,
                            car->shrapnel_material[0]->index_range + car->shrapnel_material[0]->index_base - 1);
                    } else if (car->knackered) {
                        DrawMapBlip(car, the_time, &car->car_master_actor->t.t.mat, car_pos, 0);
                    } else {
                        DrawMapBlip(car, the_time, &car->car_master_actor->t.t.mat, car_pos, gMap_colours[cat]);
                    }
                }
            }
            cat--;
        }
        gBack_screen->origin_x = real_origin_x;
        gBack_screen->origin_y = real_origin_y;
        gBack_screen->base_x = real_base_x;
        gBack_screen->base_y = real_base_y;
    } else {
        if (cockpit_on) {
            CopyStripImage(
                gBack_screen,
                -gCurrent_graf_data->cock_margin_x,
                gScreen_wobble_x,
                -gCurrent_graf_data->cock_margin_y,
                gScreen_wobble_y,
                gProgram_state.current_car.cockpit_images[gProgram_state.cockpit_image_index],
                0,
                0,
                gCurrent_graf_data->total_cock_width,
                gCurrent_graf_data->total_cock_height);
            if (gMirror_on__graphics) {
                BrPixelmapRectangleCopy(
                    gBack_screen,
                    gScreen_wobble_x + gProgram_state.current_car.mirror_left,
                    gScreen_wobble_y + gProgram_state.current_car.mirror_top,
                    gRearview_screen,
                    -gRearview_screen->origin_x,
                    -gRearview_screen->origin_y,
                    gProgram_state.current_car.mirror_right - gProgram_state.current_car.mirror_left,
                    gProgram_state.current_car.mirror_bottom - gProgram_state.current_car.mirror_top);
            }
        }
        DimAFewBits();
        DoDamageScreen(the_time);
        if (!gAction_replay_mode || gAR_fudge_headups) {
            DoPratcam(the_time);
            DoHeadups(the_time);
        }
        DoInstruments(the_time);
        DoSteeringWheel(the_time);
        if (!gAction_replay_mode || gAR_fudge_headups) {
            DrawPowerups(the_time);
        }
    }
    if (gNet_mode) {
        DisplayUserMessage();
    }
    if (gAction_replay_mode && !gAR_fudge_headups) {
        DoActionReplayHeadups();
    }
    if (gAction_replay_mode) {
        SynchronizeActionReplay();
    } else {
        PipeFrameFinish();
    }
    gRender_screen->pixels = old_pixels;
    if (!gPalette_fade_time || GetRaceTime() > gPalette_fade_time + 500) {
        PDScreenBufferSwap(0);
    }
    if (gAction_replay_mode) {
        DoActionReplayPostSwap();
    }
}

// IDA: void __cdecl InitPaletteAnimate()
void InitPaletteAnimate() {
    LOG_TRACE("()");

    // dword_53E78C = 0;
    // dword_53F930 = 0;
}

// IDA: void __cdecl RevertPalette()
void RevertPalette() {
    memcpy(gRender_palette->pixels, gOrig_render_palette->pixels, 0x400u);
    DRSetPalette3(gRender_palette, 1);
}

// IDA: void __cdecl MungePalette()
void MungePalette() {
    tU8* p;
    tU8* q;
    int i;
    float damage;
    float throb_start;
    float throb_end;
    float throb_amount;
    float throb_amount_dash;
    float omega;
    tU32 period;
    tU32 the_time;
    static int palette_spammed;
    static float last_omega;
    static tU32 next_repair_time;
    static tU32 last_sound;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ResetPalette()
void ResetPalette() {
    LOG_TRACE("()");

    InitPaletteAnimate();
    DRSetPalette(gRender_palette);
}

// IDA: void __usercall Darken(tU8 *pPtr@<EAX>, unsigned int pDarken_amount@<EDX>)
void Darken(tU8* pPtr, unsigned int pDarken_amount) {
    unsigned int value;
    LOG_TRACE10("(%p, %d)", pPtr, pDarken_amount);

    *pPtr = (pDarken_amount * *pPtr) / 256;
}

// IDA: void __usercall SetFadedPalette(int pDegree@<EAX>)
void SetFadedPalette(int pDegree) {
    int j;
    br_pixelmap* the_palette;
    char* the_pixels;
    LOG_TRACE10("(%d)", pDegree);

    memcpy(gScratch_pixels, gCurrent_palette->pixels, 0x400u);
    for (j = 0; j < 256; j++) {
        Darken((tU8*)&gScratch_pixels[4 * j], pDegree);
        Darken((tU8*)&gScratch_pixels[4 * j + 1], pDegree);
        Darken((tU8*)&gScratch_pixels[4 * j + 2], pDegree);
        Darken((tU8*)&gScratch_pixels[4 * j + 3], pDegree);
    }
    DRSetPalette2(gScratch_palette, 0);
}

// IDA: void __cdecl FadePaletteDown()
void FadePaletteDown() {
    int i;
    int start_time;
    int the_time;
    LOG_TRACE("()");

    if (!gFaded_palette) {
        gFaded_palette = 1;
        MungeEngineNoise();
        gFaded_palette = 0;
        start_time = PDGetTotalTime();
        while (1) {
            the_time = PDGetTotalTime() - start_time;
            if (the_time >= 500) {
                break;
            }
            i = 256 - ((the_time * 256) / 500);
            SetFadedPalette(i);
        }
        SetFadedPalette(0);
        gFaded_palette = 1;
    }
}

// IDA: void __cdecl FadePaletteUp()
void FadePaletteUp() {
    int i;
    int start_time;
    int the_time;
    LOG_TRACE("()");

    if (gFaded_palette) {
        gFaded_palette = 0;
        start_time = PDGetTotalTime();
        while (1) {
            the_time = PDGetTotalTime() - start_time;
            if (the_time >= 500) {
                break;
            }
            i = (the_time * 256) / 500;
            SetFadedPalette(i);
        }
        DRSetPalette(gCurrent_palette);
    }
}

// IDA: void __cdecl KillSplashScreen()
void KillSplashScreen() {
    if (gCurrent_splash) {
        BrMapRemove(gCurrent_splash);
        BrPixelmapFree(gCurrent_splash);
        gCurrent_splash = 0;
        FadePaletteDown();
        ClearEntireScreen();
    }
}

// IDA: void __cdecl EnsureRenderPalette()
void EnsureRenderPalette() {
    LOG_TRACE("()");
    if (gPalette_munged) {
        ResetPalette();
        gPalette_munged = 0;
    }
}

// IDA: void __usercall SplashScreenWith(char *pPixmap_name@<EAX>)
void SplashScreenWith(char* pPixmap_name) {
    br_pixelmap* the_map;
    LOG_TRACE("(\"%s\")", pPixmap_name);

    the_map = BrMapFind(pPixmap_name);
    if (!gCurrent_splash || the_map != gCurrent_splash) {
        FadePaletteDown();
        EnsureRenderPalette();

        if (gCurrent_splash) {
            KillSplashScreen();
        }
        gCurrent_splash = the_map;
        if (!the_map) {
            the_map = LoadPixelmap(pPixmap_name);
            gCurrent_splash = the_map;
            if (the_map) {
                BrMapAdd(the_map);
            }
        }
        if (gCurrent_splash) {
            BrPixelmapRectangleCopy(
                gBack_screen,
                0,
                0,
                gCurrent_splash,
                0,
                0,
                gCurrent_splash->width,
                gCurrent_splash->height);
            PDScreenBufferSwap(0);
            if (gFaded_palette) {
                FadePaletteUp();
            }
        }
    }
}

// IDA: void __cdecl EnsurePaletteUp()
void EnsurePaletteUp() {
    if (gFaded_palette) {
        FadePaletteUp();
    }
}

// IDA: br_uint_32 __cdecl AmbientificateMaterial(br_material *pMat, void *pArg)
br_uint_32 AmbientificateMaterial(br_material* pMat, void* pArg) {
    float a;
    a = pMat->ka + *(br_scalar*)pArg;
    if (a >= 0.0) {
        if (a > 0.99f) {
            a = 0.99f;
        }
    } else {
        a = 0.0;
    }
    pMat->ka = a;
    return 0;
}

// IDA: void __cdecl ChangeAmbience(br_scalar pDelta)
void ChangeAmbience(br_scalar pDelta) {
    LOG_TRACE("(%f)", pDelta);
    BrMaterialEnum("*", AmbientificateMaterial, &pDelta);
}

// IDA: void __cdecl InitAmbience()
void InitAmbience() {
    LOG_TRACE("()");
    gCurrent_ambience = gAmbient_adjustment;
    ChangeAmbience(gAmbient_adjustment);
}

// IDA: void __usercall DRPixelmapRectangleMaskedCopy(br_pixelmap *pDest@<EAX>, br_int_16 pDest_x@<EDX>, br_int_16 pDest_y@<EBX>, br_pixelmap *pSource@<ECX>, br_int_16 pSource_x, br_int_16 pSource_y, br_int_16 pWidth, br_int_16 pHeight)
void DRPixelmapRectangleMaskedCopy(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pDest_y, br_pixelmap* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_int_16 pWidth, br_int_16 pHeight) {
    int y_count;
    int x_count;
    int dest_row_wrap;
    int source_row_wrap;
    int x_delta;
    tU8 the_byte;
    tU8* source_ptr;
    tU8* dest_ptr;
    tU8* conv_table;
    LOG_TRACE("(%p, %d, %d, %p, %d, %d, %d, %d)", pDest, pDest_x, pDest_y, pSource, pSource_x, pSource_y, pWidth, pHeight);

    source_ptr = (tU8*)pSource->pixels + (pSource->row_bytes * pSource_y + pSource_x);
    dest_ptr = (tU8*)pDest->pixels + (pDest->row_bytes * pDest_y + pDest_x);
    source_row_wrap = pSource->row_bytes - pWidth;
    dest_row_wrap = pDest->row_bytes - pWidth;

    if (pDest_y < 0) {
        pHeight += pDest_y;
        if (pHeight <= 0) {
            return;
        }
        source_ptr -= pDest_y * pSource->row_bytes;
        dest_ptr -= pDest_y * pDest->row_bytes;
        pDest_y = 0;
    }
    if (pDest->height > pDest_y) {
        if (pDest_y + pHeight > pDest->height) {
            pHeight = pDest->height - pDest_y;
        }
        if (pDest_x < 0) {
            pWidth += pDest_x;
            if (pWidth <= 0) {
                return;
            }
            source_ptr -= pDest_x;
            dest_ptr -= pDest_x;
            source_row_wrap -= pDest_x;
            dest_row_wrap -= pDest_x;
            pDest_x = 0;
        }
        if (pDest->width > pDest_x) {
            if (pDest_x + pWidth > pDest->width) {
                pWidth = pDest->width - pDest_x;
                source_row_wrap += pDest_x + pWidth - pDest->width;
                dest_row_wrap += pDest_x + pWidth - pDest->width;
            }
            if (gCurrent_conversion_table) {
                TELL_ME_IF_WE_PASS_THIS_WAY();
            } else {
                for (y_count = 0; y_count < pHeight; y_count++) {
                    for (x_count = 0; x_count < pWidth; x_count++) {
                        the_byte = *source_ptr;
                        if (the_byte) {
                            *dest_ptr = the_byte;
                        }
                        source_ptr++;
                        dest_ptr++;
                    }
                    source_ptr += source_row_wrap;
                    dest_ptr += dest_row_wrap;
                }
            }
        }
    }
}

// IDA: void __usercall DRMaskedStamp(br_int_16 pDest_x@<EAX>, br_int_16 pDest_y@<EDX>, br_pixelmap *pSource@<EBX>)
void DRMaskedStamp(br_int_16 pDest_x, br_int_16 pDest_y, br_pixelmap* pSource) {
    LOG_TRACE("(%d, %d, %p)", pDest_x, pDest_y, pSource);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRPixelmapRectangleOnscreenCopy(br_pixelmap *pDest@<EAX>, br_int_16 pDest_x@<EDX>, br_int_16 pDest_y@<EBX>, br_pixelmap *pSource@<ECX>, br_int_16 pSource_x, br_int_16 pSource_y, br_int_16 pWidth, br_int_16 pHeight)
void DRPixelmapRectangleOnscreenCopy(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pDest_y, br_pixelmap* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_int_16 pWidth, br_int_16 pHeight) {
    int y_count;
    int x_count;
    int dest_row_wrap;
    int source_row_wrap;
    int x_delta;
    tU8 the_byte;
    tU8* source_ptr;
    tU8* dest_ptr;
    tU8* conv_table;
    // LOG_TRACE("(%p, %d, %d, %p, %d, %d, %d, %d)", pDest, pDest_x, pDest_y, pSource, pSource_x, pSource_y, pWidth, pHeight);

    source_row_wrap = pSource->row_bytes - pWidth;
    dest_row_wrap = pDest->row_bytes - pWidth;
    dest_ptr = (tU8*)pDest->pixels + (pDest->row_bytes * pDest_y + pDest_x);
    source_ptr = (tU8*)pSource->pixels + (pSource->row_bytes * pSource_y + pSource_x);

    for (y_count = 0; y_count < pHeight; y_count++) {
        for (x_count = 0; x_count < pWidth; x_count++) {
            the_byte = *source_ptr;
            if (the_byte) {
                *dest_ptr = the_byte;
            }
            source_ptr++;
            dest_ptr++;
        }
        source_ptr += source_row_wrap;
        dest_ptr += dest_row_wrap;
    }
}

// IDA: void __usercall DRPixelmapRectangleShearedCopy(br_pixelmap *pDest@<EAX>, br_int_16 pDest_x@<EDX>, br_int_16 pDest_y@<EBX>, br_pixelmap *pSource@<ECX>, br_int_16 pSource_x, br_int_16 pSource_y, br_int_16 pWidth, br_int_16 pHeight, tX1616 pShear)
void DRPixelmapRectangleShearedCopy(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pDest_y, br_pixelmap* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_int_16 pWidth, br_int_16 pHeight, tX1616 pShear) {
    int y_count;
    int x_count;
    int dest_row_wrap;
    int source_row_wrap;
    int x_delta;
    int last_shear_x;
    int current_shear_x;
    int shear_x_difference;
    tU8 the_byte;
    tU8* source_ptr;
    tU8* dest_ptr;
    tU8* conv_table;
    tX1616 current_shear;
    LOG_TRACE("(%p, %d, %d, %p, %d, %d, %d, %d, %d)", pDest, pDest_x, pDest_y, pSource, pSource_x, pSource_y, pWidth, pHeight, pShear);

    current_shear = 0;
    last_shear_x = 0;
    source_ptr = (tU8*)pSource->pixels + pSource_x + pSource_y * pSource->row_bytes;
    dest_ptr = (tU8*)pDest->pixels + pDest_x + pDest_y * pDest->row_bytes;
    source_row_wrap = pSource->row_bytes - pWidth;
    dest_row_wrap = pDest->row_bytes - pWidth;
    if (pDest_y < 0) {
        pHeight += pDest_y;
        if (pHeight <= 0) {
            return;
        }
        source_ptr -= pDest_y * pSource->row_bytes;
        dest_ptr -= pDest_y * pDest->row_bytes;
        pDest_y = 0;
    }
    if (pDest->height > pDest_y) {
        if (pDest_y + pHeight > pDest->height) {
            pHeight = pDest->height - pDest_y;
        }
        if (pDest_x < 0) {
            pWidth += pDest_x;
            if (pWidth <= 0) {
                return;
            }
            source_ptr -= pDest_x;
            dest_ptr -= pDest_x;
            source_row_wrap -= pDest_x;
            dest_row_wrap -= pDest_x;
            pDest_x = 0;
        }
        if (pDest->width > pDest_x) {
            if (pDest_x + pWidth > pDest->width) {
                shear_x_difference = pDest_x + pWidth - pDest->width;
                pWidth = pDest->width - pDest_x;
                source_row_wrap += shear_x_difference;
                dest_row_wrap += shear_x_difference;
            }
            for (y_count = 0; pHeight > y_count; ++y_count) {
                for (x_count = 0; pWidth > x_count; ++x_count) {
                    the_byte = *source_ptr++;
                    if (the_byte) {
                        *dest_ptr = the_byte;
                    }
                    ++dest_ptr;
                }
                current_shear_x = (current_shear >> 16) - last_shear_x;
                dest_ptr += dest_row_wrap + current_shear_x;
                last_shear_x = current_shear >> 16;
                source_ptr += source_row_wrap;
                current_shear += pShear;
                pDest_x += current_shear_x;
                if (pDest_x < 0) {
                    pWidth += pDest_x;
                    source_ptr -= pDest_x;
                    dest_ptr -= pDest_x;
                    source_row_wrap -= pDest_x;
                    dest_row_wrap -= pDest_x;
                    pDest_x = 0;
                }
                if (pDest->width <= pDest_x) {
                    break;
                }
                if (pDest_x + pWidth > pDest->width) {
                    shear_x_difference = pDest_x + pWidth - pDest->width;
                    pWidth = pDest->width - pDest_x;
                    source_row_wrap += shear_x_difference;
                    dest_row_wrap += shear_x_difference;
                }
            }
        }
    }
}

// IDA: void __usercall DRPixelmapRectangleVScaledCopy(br_pixelmap *pDest@<EAX>, br_int_16 pDest_x@<EDX>, br_int_16 pDest_y@<EBX>, br_pixelmap *pSource@<ECX>, br_int_16 pSource_x, br_int_16 pSource_y, br_int_16 pWidth, br_int_16 pHeight)
void DRPixelmapRectangleVScaledCopy(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pDest_y, br_pixelmap* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_int_16 pWidth, br_int_16 pHeight) {
    int y_count;
    int x_count;
    int dest_row_wrap;
    int source_row_wrap;
    int x_delta;
    tU8 the_byte;
    tU8* source_ptr;
    tU8* dest_ptr;
    tU32 source_y;
    tU32 source_y_delta;
    tU32 old_source_y;
    LOG_TRACE("(%p, %d, %d, %p, %d, %d, %d, %d)", pDest, pDest_x, pDest_y, pSource, pSource_x, pSource_y, pWidth, pHeight);

    if (!pHeight) {
        return;
    }

    source_row_wrap = pSource->row_bytes - pWidth;
    dest_row_wrap = pDest->row_bytes - pWidth;
    dest_ptr = (tU8*)pDest->pixels + (pDest->row_bytes * pDest_y + pDest_x);
    source_ptr = (tU8*)pSource->pixels + (pSource->row_bytes * pSource_y + pSource_x);

    source_y = 0;
    source_y_delta = (pSource->height << 16) / pHeight - 0x10000;

    for (y_count = 0; y_count < pHeight; y_count++) {
        for (x_count = 0; x_count < pWidth; x_count++) {
            the_byte = *source_ptr;
            if (the_byte) {
                *dest_ptr = the_byte;
            }
            source_ptr++;
            dest_ptr++;
        }
        old_source_y = source_y;
        source_y += source_y_delta;
        source_ptr += (((source_y >> 16) - (old_source_y >> 16)) * pSource->row_bytes) + source_row_wrap;
        dest_ptr += dest_row_wrap;
    }
}

// IDA: void __cdecl InitTransientBitmaps()
void InitTransientBitmaps() {
    int i;
    LOG_TRACE("()");
    for (i = 0; i < 50; i++) {
        gTransient_bitmaps[i].pixmap = NULL;
        gTransient_bitmaps[i].in_use = 0;
    }
}

// IDA: int __usercall AllocateTransientBitmap@<EAX>(int pWidth@<EAX>, int pHeight@<EDX>, int pUser_data@<EBX>)
int AllocateTransientBitmap(int pWidth, int pHeight, int pUser_data) {
    int bm_index;
    LOG_TRACE("(%d, %d, %d)", pWidth, pHeight, pUser_data);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DeallocateTransientBitmap(int pIndex@<EAX>)
void DeallocateTransientBitmap(int pIndex) {
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DeallocateAllTransientBitmaps()
void DeallocateAllTransientBitmaps() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RemoveTransientBitmaps(int pGraphically_remove_them@<EAX>)
void RemoveTransientBitmaps(int pGraphically_remove_them) {
    int i;
    int order_number;
    STUB_ONCE();
}

// IDA: void __usercall SaveTransient(int pIndex@<EAX>, int pX_coord@<EDX>, int pY_coord@<EBX>)
void SaveTransient(int pIndex, int pX_coord, int pY_coord) {
    LOG_TRACE("(%d, %d, %d)", pIndex, pX_coord, pY_coord);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawCursorGiblet(tCursor_giblet *pGib@<EAX>)
void DrawCursorGiblet(tCursor_giblet* pGib) {
    br_pixelmap* the_image;
    LOG_TRACE("(%p)", pGib);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ProcessCursorGiblets(int pPeriod@<EAX>)
void ProcessCursorGiblets(int pPeriod) {
    int i;
    int kill_the_giblet;
    tU32 time_now;
    tCursor_giblet* gib;
    LOG_TRACE("(%d)", pPeriod);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall NewCursorGiblet@<EAX>(int pX_coord@<EAX>, int pY_coord@<EDX>, float pX_speed, float pY_speed, tU32 pDrop_time)
int NewCursorGiblet(int pX_coord, int pY_coord, float pX_speed, float pY_speed, tU32 pDrop_time) {
    int i;
    int the_width;
    int the_height;
    int sequence_number;
    LOG_TRACE("(%d, %d, %f, %f, %d)", pX_coord, pY_coord, pX_speed, pY_speed, pDrop_time);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl DoMouseCursor()
int DoMouseCursor() {
    int y_coord;
    int mouse_moved;
    int new_required;
    int giblet_index;
    int cursor_offset;
    int button_is_down;
    int giblet_chance;
    int giblet_count;
    tU32 this_call_time;
    static tU32 last_cursor_change;
    static tU32 last_call_time;
    static tU32 last_required_change;
    tS32 period;
    static int delta_x;
    static int required_cursor;
    static int zero_count;
    static int button_was_down;
    STUB_ONCE();
    return 0;
}

// IDA: int __cdecl AllocateCursorTransient()
int AllocateCursorTransient() {
    int i;
    int largest_width;
    int largest_height;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl StartMouseCursor()
void StartMouseCursor() {
    STUB();
}

// IDA: void __cdecl EndMouseCursor()
void EndMouseCursor() {
    STUB();
}

// IDA: void __usercall LoadFont(int pFont_ID@<EAX>)
void LoadFont(int pFont_ID) {
    tPath_name the_path;
    int i;
    int number_of_chars;
    FILE* f;
    tU32 the_size;
    LOG_TRACE("(%d)", pFont_ID);

    if (gFonts[pFont_ID].images) {
        return;
    }

    PathCat(the_path, gApplication_path, gGraf_specs[gGraf_spec_index].data_dir_name);
    PathCat(the_path, the_path, "FONTS");
    PathCat(the_path, the_path, gFont_names[pFont_ID]);
    number_of_chars = strlen(the_path);
    strcat(the_path, ".PIX");
    gFonts[pFont_ID].images = DRPixelmapLoad(the_path);

    if (!gFonts[pFont_ID].images) {
        FatalError(20, gFont_names[pFont_ID]);
    }
    if (!gFonts[pFont_ID].file_read_once) {
        strcpy(&the_path[number_of_chars + 1], "TXT");

        f = DRfopen(the_path, "rt");
        if (!f) {
            FatalError(21, gFont_names[pFont_ID]);
        }

        gFonts[pFont_ID].height = GetAnInt(f);
        gFonts[pFont_ID].width = GetAnInt(f);
        gFonts[pFont_ID].spacing = GetAnInt(f);
        gFonts[pFont_ID].offset = GetAnInt(f);
        gFonts[pFont_ID].num_entries = GetAnInt(f);
        if (gFonts[pFont_ID].width <= 0) {
            for (i = 0; i < gFonts[pFont_ID].num_entries; i++) {
                the_size = GetAnInt(f);
                gFonts[pFont_ID].width_table[i] = the_size;
            }
        }
        fclose(f);
        gFonts[pFont_ID].file_read_once = 1;
    }
}

// IDA: void __usercall DisposeFont(int pFont_ID@<EAX>)
void DisposeFont(int pFont_ID) {
    LOG_TRACE("(%d)", pFont_ID);
    if (gFonts[pFont_ID].images && (!TranslationMode() || (gAusterity_mode && FlicsPlayedFromDisk()))) {
        BrPixelmapFree(gFonts[pFont_ID].images);
        gFonts[pFont_ID].images = NULL;
        gFonts[pFont_ID].file_read_once = 0;
    }
}

// IDA: void __cdecl InitDRFonts()
void InitDRFonts() {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < 21; i++) {
        gFonts[i].file_read_once = 0;
        gFonts[i].images = NULL;
    }
}

// IDA: void __usercall DrawDropImage(br_pixelmap *pImage@<EAX>, int pLeft@<EDX>, int pTop@<EBX>, int pTop_clip@<ECX>, int pBottom_clip, int pOffset)
void DrawDropImage(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip, int pOffset) {
    int y;
    int src_y;
    int src_height;
    int y_diff;
    LOG_TRACE("(%p, %d, %d, %d, %d, %d)", pImage, pLeft, pTop, pTop_clip, pBottom_clip, pOffset);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DropInImageFromTop(br_pixelmap *pImage@<EAX>, int pLeft@<EDX>, int pTop@<EBX>, int pTop_clip@<ECX>, int pBottom_clip)
void DropInImageFromTop(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip) {
    tS32 start_time;
    tS32 the_time;
    int drop_distance;
    LOG_TRACE("(%p, %d, %d, %d, %d)", pImage, pLeft, pTop, pTop_clip, pBottom_clip);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DropOutImageThruBottom(br_pixelmap *pImage@<EAX>, int pLeft@<EDX>, int pTop@<EBX>, int pTop_clip@<ECX>, int pBottom_clip)
void DropOutImageThruBottom(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip) {
    tS32 start_time;
    tS32 the_time;
    int drop_distance;
    LOG_TRACE("(%p, %d, %d, %d, %d)", pImage, pLeft, pTop, pTop_clip, pBottom_clip);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DropInImageFromBottom(br_pixelmap *pImage@<EAX>, int pLeft@<EDX>, int pTop@<EBX>, int pTop_clip@<ECX>, int pBottom_clip)
void DropInImageFromBottom(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip) {
    tS32 start_time;
    tS32 the_time;
    int drop_distance;
    LOG_TRACE("(%p, %d, %d, %d, %d)", pImage, pLeft, pTop, pTop_clip, pBottom_clip);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DropOutImageThruTop(br_pixelmap *pImage@<EAX>, int pLeft@<EDX>, int pTop@<EBX>, int pTop_clip@<ECX>, int pBottom_clip)
void DropOutImageThruTop(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip) {
    tS32 start_time;
    tS32 the_time;
    int drop_distance;
    LOG_TRACE("(%p, %d, %d, %d, %d)", pImage, pLeft, pTop, pTop_clip, pBottom_clip);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawTellyLine(br_pixelmap *pImage@<EAX>, int pLeft@<EDX>, int pTop@<EBX>, int pPercentage@<ECX>)
void DrawTellyLine(br_pixelmap* pImage, int pLeft, int pTop, int pPercentage) {
    int the_width;
    int the_height;
    LOG_TRACE("(%p, %d, %d, %d)", pImage, pLeft, pTop, pPercentage);

    the_width = pImage->width;
    the_height = pImage->height / 2 + pTop;
    BrPixelmapLine(gBack_screen, pLeft, the_height, pLeft + the_width, the_height, 0);
    BrPixelmapLine(gBack_screen, the_width / 2 + pLeft - pPercentage * the_width / 200, the_height, the_width / 2 + pLeft + pPercentage * the_width / 200, the_height, 1);
    PDScreenBufferSwap(0);
}

// IDA: void __usercall DrawTellyImage(br_pixelmap *pImage@<EAX>, int pLeft@<EDX>, int pTop@<EBX>, int pPercentage@<ECX>)
void DrawTellyImage(br_pixelmap* pImage, int pLeft, int pTop, int pPercentage) {
    int the_height;
    LOG_TRACE("(%p, %d, %d, %d)", pImage, pLeft, pTop, pPercentage);

    BrPixelmapRectangleFill(gBack_screen, pLeft, pTop, pImage->width, pImage->height, 0);
    if (pPercentage != 1000) {
        DRPixelmapRectangleVScaledCopy(
            gBack_screen,
            pLeft,
            pTop + pImage->height * (100 - pPercentage) / 200,
            pImage,
            0,
            0,
            pImage->width,
            pPercentage * pImage->height / 100);
        PDScreenBufferSwap(0);
    }
}

// IDA: void __usercall TellyInImage(br_pixelmap *pImage@<EAX>, int pLeft@<EDX>, int pTop@<EBX>)
void TellyInImage(br_pixelmap* pImage, int pLeft, int pTop) {
    tS32 start_time;
    tS32 the_time;
    LOG_TRACE("(%p, %d, %d)", pImage, pLeft, pTop);

    start_time = PDGetTotalTime();
    while (1) {
        the_time = PDGetTotalTime();
        if (the_time - start_time > 100) {
            break;
        }
        DrawTellyLine(pImage, pLeft, pTop, 100 * (the_time - start_time) / 100);
    }
    start_time = PDGetTotalTime();
    while (1) {
        the_time = PDGetTotalTime();
        if (the_time - start_time > 100) {
            break;
        }
        DrawTellyImage(pImage, pLeft, pTop, 100 * (the_time - start_time) / 100);
    }
    DrawTellyImage(pImage, pLeft, pTop, 100);
}

// IDA: void __usercall TellyOutImage(br_pixelmap *pImage@<EAX>, int pLeft@<EDX>, int pTop@<EBX>)
void TellyOutImage(br_pixelmap* pImage, int pLeft, int pTop) {
    tS32 start_time;
    tS32 the_time;
    int drop_distance;
    LOG_TRACE("(%p, %d, %d)", pImage, pLeft, pTop);

    start_time = PDGetTotalTime();
    while (1) {
        the_time = PDGetTotalTime();
        if (the_time - start_time > 100) {
            break;
        }
        DrawTellyImage(pImage, pLeft, pTop, 100 * (100 - the_time + start_time) / 100);
    }

    DrawTellyImage(pImage, pLeft, pTop, 1000);

    start_time = PDGetTotalTime();
    while (1) {
        the_time = PDGetTotalTime();
        if (the_time - start_time > 100) {
            break;
        }
        DrawTellyLine(pImage, pLeft, pTop, 100 * (start_time + 100 - the_time) / 100);
    }

    DrawTellyLine(pImage, pLeft, pTop, 0);
}

// IDA: void __usercall SetShadowLevel(tShadow_level pLevel@<EAX>)
void SetShadowLevel(tShadow_level pLevel) {
    LOG_TRACE("(%d)", pLevel);
    gShadow_level = pLevel;
}

// IDA: tShadow_level __cdecl GetShadowLevel()
tShadow_level GetShadowLevel() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ToggleShadow()
void ToggleShadow() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitShadow()
void InitShadow() {
    int i;
    br_vector3 temp_v;
    LOG_TRACE("()");

    for (i = 0; i < 8; i++) {
        gShadow_clip_planes[i].clip = BrActorAllocate(BR_ACTOR_CLIP_PLANE, NULL);
        BrActorAdd(gUniverse_actor, gShadow_clip_planes[i].clip);
        BrClipPlaneDisable(gShadow_clip_planes[i].clip);
        BrMatrix34Identity(&gShadow_clip_planes[i].clip->t.t.mat);
    }
    gFancy_shadow = 0; // jeff changed
    gShadow_material = BrMaterialFind("SHADOW.MAT");
    gShadow_light_ray.v[0] = 0.0;
    gShadow_light_ray.v[1] = -1.0;
    gShadow_light_ray.v[2] = 0.0;
    gShadow_light_z.v[0] = 0.0;
    gShadow_light_z.v[1] = 0.0;
    gShadow_light_z.v[2] = -1.0;
    gShadow_light_x.v[1] = 0.0;
    gShadow_light_x.v[2] = 0.0;
    gShadow_light_x.v[0] = 1.0;

    gShadow_model = BrModelAllocate(NULL, 0, 0);
    gShadow_model->flags = BR_MODF_GENERATE_TAGS | BR_MODF_KEEP_ORIGINAL;
    gShadow_actor = BrActorAllocate(BR_ACTOR_MODEL, 0);
    gShadow_actor->model = gShadow_model;
    BrActorAdd(gUniverse_actor, gShadow_actor);
}

// IDA: br_uint_32 __cdecl SaveShadeTable(br_pixelmap *pTable, void *pArg)
br_uint_32 SaveShadeTable(br_pixelmap* pTable, void* pArg) {
    LOG_TRACE("(%p, %p)", pTable, pArg);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SaveShadeTables()
void SaveShadeTables() {
    LOG_TRACE("()");
    STUB();
}

// IDA: void __cdecl DisposeSavedShadeTables()
void DisposeSavedShadeTables() {
    int i;
    LOG_TRACE("()");

    STUB();
}

// IDA: void __cdecl ShadowMode()
void ShadowMode() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl SwitchToRealResolution()
int SwitchToRealResolution() {
    LOG_TRACE("()");

    if (gGraf_data_index == gReal_graf_data_index) {
        return 0;
    }
    gGraf_data_index = gReal_graf_data_index;
    gGraf_spec_index = gReal_graf_data_index;
    gCurrent_graf_data = &gGraf_data[gReal_graf_data_index];
    return 1;
}

// IDA: int __cdecl SwitchToLoresMode()
int SwitchToLoresMode() {
    LOG_TRACE("()");
    if (!gGraf_data_index || gGraf_data_index != gReal_graf_data_index) {
        return 0;
    }
    gGraf_data_index = 0;
    gGraf_spec_index = 0;
    gCurrent_graf_data = gGraf_data;
    return 1;
}

// IDA: void __usercall DRPixelmapDoubledCopy(br_pixelmap *pDestn@<EAX>, br_pixelmap *pSource@<EDX>, int pSource_width@<EBX>, int pSource_height@<ECX>, int pX_offset, int pY_offset)
void DRPixelmapDoubledCopy(br_pixelmap* pDestn, br_pixelmap* pSource, int pSource_width, int pSource_height, int pX_offset, int pY_offset) {
    tU16* sptr;
    tU16 pixels;
    tU8* dptr;
    tU8* dptr2;
    tU8 pixel_1;
    tU8 pixel_2;
    int i;
    int j;
    int dst_row_skip;
    int src_row_skip;
    int width_over_2;
    LOG_TRACE("(%p, %p, %d, %d, %d, %d)", pDestn, pSource, pSource_width, pSource_height, pX_offset, pY_offset);
    NOT_IMPLEMENTED();
}
