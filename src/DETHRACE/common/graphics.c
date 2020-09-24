#include "graphics.h"

#include "CORE/V1DB/modsupt.h"
#include "brender.h"
#include "common/flicplay.h"
#include "errors.h"
#include "globvars.h"
#include "grafdata.h"
#include "init.h"
#include "loading.h"
#include "pc-dos/dossys.h"
#include "sound.h"
#include "utility.h"
#include <stdlib.h>
#include <unistd.h>

#include <math.h>

int gArrows[2][4][60];
int gMap_colours[4];
br_vector3 gShadow_points[8];
tConcussion gConcussion;
tClip_details gShadow_clip_planes[8];
br_matrix34 gSheer_mat;
br_actor* gLollipops[100];
tWobble_spec gWobble_array[5];
br_matrix34 gIdentity34;
tSaved_table gSaved_shade_tables[100];
int* gCursor_giblet_sequences[4];
int gCursor_y_offsets[8];
int gCursor_x_offsets[8];
tCursor_giblet gCursor_giblets[45];
int gCursor_gib_x_offsets[8];
int gCursor_gib_y_offsets[8];
tTransient_bm gTransient_bitmaps[50];
float gCosine_array[64];
br_pixelmap* gCursors[8];
br_pixelmap* gCursor_giblet_images[18];
br_scalar gShadow_hither_min_move;
br_pixelmap* gEval_1;
br_pixelmap* gEval_2;
br_vector3 gShadow_light_z;
br_vector3 gShadow_light_x;
int gShadow_dim_amount;
br_colour gRGB_colours[9];
int gNumber_of_lollipops;
br_vector3 gShadow_light_ray;
br_scalar gShadow_hither_z_move;
int gFancy_shadow;
tShadow_level gShadow_level;
br_model* gShadow_model;
br_actor* gShadow_actor;
int gShadow_clip_plane_count;
br_pixelmap* gPalette_conversion_table;
br_material* gShadow_material;
int gSaved_table_count;
int gCursor_giblet_sequence3[5];
int gCursor_giblet_sequence2[5];
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

int gCurrent_cursor_index;
int gCursor_giblet_sequence1[5];
int gCursor_giblet_sequence0[7];
int gPalette_index;
int gCursor_transient_index;
char* gScratch_pixels;
br_pixelmap* gScratch_palette;
int gPalette_munged = 0;
int gLast_palette_change;
int gColourValues[1];
br_pixelmap* gOrig_render_palette;
int gNext_transient;
br_pixelmap* gCurrent_palette;
br_pixelmap* gRender_palette;
br_pixelmap* gCurrent_splash;
br_pixelmap* gCurrent_conversion_table;
float gCamera_to_horiz_angle;
int gColours[9];
br_pixelmap* gFlic_palette;
tDR_font gFonts[21];
char* gCurrent_palette_pixels;
int gAR_fudge_headups;
float gMap_render_x;
float gMap_render_y;
int gFaded_palette = 0;
int gWidth;
int gMap_render_height_i;
int gScreen_wobble_x;
int gScreen_wobble_y;
br_scalar gCurrent_ambience;
int gY_offset;
float gMap_render_width;
float gMap_render_height;
int gMap_render_width_i;
int gMouse_started;
int gMouse_in_use;
int gHeight;
int gMouse_last_y_coord;
int gMouse_last_x_coord;
br_scalar gAmbient_adjustment;
int gMap_render_x_i;
int gX_offset;
int gMap_render_y_i;
int gMirror_on;
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
    NOT_IMPLEMENTED();
}

// IDA: int __usercall AddToLollipopQueue@<EAX>(br_actor *pActor@<EAX>, int pIndex@<EDX>)
int AddToLollipopQueue(br_actor* pActor, int pIndex) {
    LOG_TRACE("(%p, %d)", pActor, pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RenderLollipops()
void RenderLollipops() {
    int i;
    int must_relink;
    br_actor** the_actor;
    br_actor* old_parent;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawDigitAt(br_pixelmap *gImage@<EAX>, int pX@<EDX>, int pY@<EBX>, int pY_pitch@<ECX>, int pValue)
void DrawDigitAt(br_pixelmap* gImage, int pX, int pY, int pY_pitch, int pValue) {
    LOG_TRACE("(%p, %d, %d, %d, %d)", gImage, pX, pY, pY_pitch, pValue);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawNumberAt(br_pixelmap *gImage@<EAX>, int pX@<EDX>, int pY@<EBX>, int pX_pitch@<ECX>, int pY_pitch, int pValue, int pDigit_count, int pLeading_zeroes)
void DrawNumberAt(br_pixelmap* gImage, int pX, int pY, int pX_pitch, int pY_pitch, int pValue, int pDigit_count, int pLeading_zeroes) {
    int i;
    int the_value;
    LOG_TRACE("(%p, %d, %d, %d, %d, %d, %d, %d)", gImage, pX, pY, pX_pitch, pY_pitch, pValue, pDigit_count, pLeading_zeroes);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall BuildColourTable(br_pixelmap *pPalette@<EAX>)
void BuildColourTable(br_pixelmap* pPalette) {
    int i;
    int j;
    int nearest_index;
    int red;
    int green;
    int blue;
    float nearest_distance;
    float distance;
    LOG_TRACE("(%p)", pPalette);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ClearConcussion()
void ClearConcussion() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: tS8* __usercall SkipLines@<EAX>(tS8 *pSource@<EAX>, int pCount@<EDX>)
tS8* SkipLines(tS8* pSource, int pCount) {
    int i;
    int j;
    int number_of_chunks;
    int chunk_length;
    LOG_TRACE("(%p, %d)", pSource, pCount);
    NOT_IMPLEMENTED();
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
    LOG_TRACE("(%p, %d, %d, %d, %d, %p, %d, %d, %d, %d)", pDest, pDest_x, pOffset_x, pDest_y, pOffset_y, pSource, pSource_x, pSource_y, pWidth, pHeight);
    NOT_IMPLEMENTED();
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
    gRender_screen = DRPixelmapAllocateSub(gBack_screen, pX_offset, pY_offset, pHeight, pWidth);
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl AdjustRenderScreenSize()
void AdjustRenderScreenSize() {
    int switched_res;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScreenSmaller()
void ScreenSmaller() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScreenLarger()
void ScreenLarger() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DRSetPaletteEntries(br_pixelmap *pPalette@<EAX>, int pFirst_colour@<EDX>, int pCount@<EBX>)
void DRSetPaletteEntries(br_pixelmap* pPalette, int pFirst_colour, int pCount) {
    LOG_TRACE("(%p, %d, %d)", pPalette, pFirst_colour, pCount);
    if (!pFirst_colour) {
        ((br_int_32*)pPalette->pixels)[0] = 0;
    }
    memcpy(gCurrent_palette_pixels + 4 * pFirst_colour, pPalette->pixels + 4 * pFirst_colour, 4 * pCount);
    if (!gFaded_palette) {
        PDSetPaletteEntries(pPalette, pFirst_colour, pCount);
    }
    gPalette_munged = 1;
}

// IDA: void __usercall DRSetPalette3(br_pixelmap *pThe_palette@<EAX>, int pSet_current_palette@<EDX>)
void DRSetPalette3(br_pixelmap* pThe_palette, int pSet_current_palette) {
    LOG_TRACE("(%p, %d)", pThe_palette, pSet_current_palette);
    NOT_IMPLEMENTED();
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
    gPalette_munged |= pThe_palette != gRender_palette;
}

// IDA: void __usercall DRSetPalette(br_pixelmap *pThe_palette@<EAX>)
void DRSetPalette(br_pixelmap* pThe_palette) {
    ((br_int_32*)pThe_palette->pixels)[0] = 0;
    memcpy(gCurrent_palette_pixels, pThe_palette->pixels, 0x400u);
    if (!gFaded_palette) {
        PDSetPalette(pThe_palette);
    }
    gPalette_munged |= pThe_palette != gRender_palette;
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitWobbleStuff()
void InitWobbleStuff() {
    int i;

    for (i = 0; i < 5; i++) {
        gWobble_array[i].amplitude_x = 0;
    }

    for (i = 0; i < 64; i++) {
        gCosine_array[i] = cosf((double)i * 0.015625f * 3.141592653589793f * 0.5f);
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall TryThisEdge(tCar_spec *pCar@<EAX>, br_vector3 *pLight@<EDX>, int pIndex_1@<EBX>, br_scalar pSign_1, int pIndex_2, br_scalar pSign_2, int pPoint_index_1, int pPoint_index_2, br_scalar pY_offset)
void TryThisEdge(tCar_spec* pCar, br_vector3* pLight, int pIndex_1, br_scalar pSign_1, int pIndex_2, br_scalar pSign_2, int pPoint_index_1, int pPoint_index_2, br_scalar pY_offset) {
    br_scalar dot_1;
    br_scalar dot_2;
    br_scalar mult;
    LOG_TRACE("(%p, %p, %d, %f, %d, %f, %d, %d, %f)", pCar, pLight, pIndex_1, pSign_1, pIndex_2, pSign_2, pPoint_index_1, pPoint_index_2, pY_offset);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall DistanceFromPlane@<ST0>(br_vector3 *pPos@<EAX>, br_scalar pA, br_scalar pB, br_scalar pC, br_scalar pD)
br_scalar DistanceFromPlane(br_vector3* pPos, br_scalar pA, br_scalar pB, br_scalar pC, br_scalar pD) {
    br_vector3 normal;
    LOG_TRACE("(%p, %f, %f, %f, %f)", pPos, pA, pB, pC, pD);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}

// IDA: void __usercall FlashyMapCheckpoint(int pIndex@<EAX>, tU32 pTime@<EDX>)
void FlashyMapCheckpoint(int pIndex, tU32 pTime) {
    tCheckpoint* cp;
    static tU32 last_flash;
    static int flash_state;
    LOG_TRACE("(%d, %d)", pIndex, pTime);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ConditionallyFillWithSky@<EAX>(br_pixelmap *pPixelmap@<EAX>)
int ConditionallyFillWithSky(br_pixelmap* pPixelmap) {
    int bgnd_col;
    LOG_TRACE("(%p)", pPixelmap);
    NOT_IMPLEMENTED();
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
    int real_origin_x;
    int real_origin_y;
    int real_base_x;
    int real_base_y;
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
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitPaletteAnimate()
void InitPaletteAnimate() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl RevertPalette()
void RevertPalette() {
    gPalette_munged = 0;
    gPalette_index = 0;
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

    gPalette_index = 0;
    gPalette_munged = 0;
    DRSetPalette2(gRender_palette, 1);
}

// IDA: void __usercall Darken(tU8 *pPtr@<EAX>, unsigned int pDarken_amount@<EDX>)
void Darken(tU8* pPtr, unsigned int pDarken_amount) {
    unsigned int value;
    LOG_TRACE("(%p, %d)", pPtr, pDarken_amount);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetFadedPalette(int pDegree@<EAX>)
void SetFadedPalette(int pDegree) {
    int j;
    br_pixelmap* the_palette;
    unsigned char* the_pixels; //JeffH added unsigned
    LOG_TRACE10("(%d)", pDegree);

    memcpy(gScratch_pixels, gCurrent_palette->pixels, 0x400u);
    the_pixels = (unsigned char*)gScratch_pixels;

    for (j = 0; j < 1024; j += 4) {
        the_pixels[j] = (pDegree * the_pixels[j]) / 256;
        the_pixels[j + 1] = (pDegree * the_pixels[j + 1]) / 256;
        the_pixels[j + 2] = (pDegree * the_pixels[j + 2]) / 256;
        the_pixels[j + 3] = (pDegree * the_pixels[j + 3]) / 256;
    }
    ((int32_t*)gScratch_palette->pixels)[0] = 0;
    if (!gFaded_palette) {
        PDSetPalette(gScratch_palette);
    }
    gPalette_munged |= gScratch_palette != gRender_palette;
}

// IDA: void __cdecl FadePaletteDown()
void FadePaletteDown() {
    int i;
    int start_time;
    int the_time;

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
        DRSetPalette2(gCurrent_palette, 1);
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
        RevertPalette();
        DRSetPalette(gRender_palette);
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
        if (gPalette_munged) {
            ResetPalette();
            gPalette_munged = 0;
        }
        if (gCurrent_splash) {
            BrMapRemove(gCurrent_splash);
            BrPixelmapFree(gCurrent_splash);
            gCurrent_splash = 0;
            FadePaletteDown();
            ClearEntireScreen();
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
    return ChangeAmbience(gAmbient_adjustment);
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
    NOT_IMPLEMENTED();
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
    //LOG_TRACE("(%p, %d, %d, %p, %d, %d, %d, %d)", pDest, pDest_x, pDest_y, pSource, pSource_x, pSource_y, pWidth, pHeight);

    source_row_wrap = pSource->row_bytes - pWidth;
    dest_row_wrap = pDest->row_bytes - pWidth;
    dest_ptr = pDest->pixels + (pDest->row_bytes * pDest_y + pDest_x);
    source_ptr = pSource->pixels + (pSource->row_bytes * pSource_y + pSource_x);

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
    NOT_IMPLEMENTED();
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
    dest_ptr = pDest->pixels + (pDest->row_bytes * pDest_y + pDest_x);
    source_ptr = pSource->pixels + (pSource->row_bytes * pSource_y + pSource_x);

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
    SILENT_STUB();
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
    SILENT_STUB();
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
    gFancy_shadow = 1;
    gShadow_material = BrMaterialFind("SHADOW.MAT");
    gShadow_light_ray.v[0] = 0.0;
    gShadow_light_ray.v[1] = -1.0;
    gShadow_light_ray.v[2] = 0.0;
    // dword_1316E4 = 0;
    // dword_1316BC = 0;
    // dword_1316C0 = 0;
    // dword_1316CC = 0;
    // dword_1316D0 = 0;
    // dword_1316C8 = 0x3F800000;
    // dword_1316E0 = 0xBF800000;
    // dword_1316C4 = 0xBF800000;

    gShadow_model = BrModelAllocate(NULL, 0, 0);
    gShadow_model->flags = 6;
    gShadow_actor = BrActorAllocate(BR_ACTOR_MODEL, 0);
    gShadow_actor->model = gShadow_model;
    return BrActorAdd(gUniverse_actor, gShadow_actor);
}

// IDA: br_uint_32 __cdecl SaveShadeTable(br_pixelmap *pTable, void *pArg)
br_uint_32 SaveShadeTable(br_pixelmap* pTable, void* pArg) {
    LOG_TRACE("(%p, %p)", pTable, pArg);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SaveShadeTables()
void SaveShadeTables() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DisposeSavedShadeTables()
void DisposeSavedShadeTables() {
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
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
