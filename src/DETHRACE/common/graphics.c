#include "graphics.h"

#include "brender.h"
#include "errors.h"
#include "globvars.h"
#include "init.h"
#include "pc-dos/dossys.h"
#include "utility.h"
#include <stdlib.h>

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
char* gFont_names[21];
int gCurrent_cursor_index;
int gCursor_giblet_sequence1[5];
int gCursor_giblet_sequence0[7];
int gPalette_index;
int gCursor_transient_index;
char* gScratch_pixels;
br_pixelmap* gScratch_palette;
int gPalette_munged;
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
int gFaded_palette;
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

// Offset: 0
// Size: 44
void TurnOnPaletteConversion() {
    NOT_IMPLEMENTED();
}

// Offset: 44
// Size: 44
void TurnOffPaletteConversion() {
    NOT_IMPLEMENTED();
}

// Offset: 88
// Size: 44
void ResetLollipopQueue() {
    NOT_IMPLEMENTED();
}

// Offset: 132
// Size: 124
// EAX: pActor
// EDX: pIndex
int AddToLollipopQueue(br_actor* pActor, int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 256
// Size: 237
void RenderLollipops() {
    int must_relink;
    br_actor** the_actor;
    br_actor* old_parent;
    NOT_IMPLEMENTED();
}

// Offset: 496
// Size: 106
// EAX: pDestn
// EDX: pX1
// EBX: pY1
// ECX: pX2
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
    NOT_IMPLEMENTED();
}

// Offset: 604
// Size: 90
// EAX: gImage
// EDX: pX
// EBX: pY
// ECX: pY_pitch
void DrawDigitAt(br_pixelmap* gImage, int pX, int pY, int pY_pitch, int pValue) {
    NOT_IMPLEMENTED();
}

// Offset: 696
// Size: 156
// EAX: gImage
// EDX: pX
// EBX: pY
// ECX: pX_pitch
void DrawNumberAt(br_pixelmap* gImage, int pX, int pY, int pX_pitch, int pY_pitch, int pValue, int pDigit_count, int pLeading_zeroes) {
    int i;
    int the_value;
    NOT_IMPLEMENTED();
}

// Offset: 852
// Size: 350
// EAX: pPalette
void BuildColourTable(br_pixelmap* pPalette) {
    int i;
    int j;
    int nearest_index;
    int red;
    int green;
    int blue;
    float nearest_distance;
    float distance;
    NOT_IMPLEMENTED();
}

// Offset: 1204
// Size: 44
void ClearConcussion() {
    NOT_IMPLEMENTED();
}

// Offset: 1248
// Size: 137
// EAX: pSource
// EDX: pCount
tS8* SkipLines(tS8* pSource, int pCount) {
    int i;
    int j;
    int number_of_chunks;
    int chunk_length;
    NOT_IMPLEMENTED();
}

// Offset: 1388
// Size: 126
// EAX: pDst
// EDX: pSrc
// EBX: pN
void CopyWords(char* pDst, char* pSrc, int pN) {
    tU16* dst;
    tU16* src;
    NOT_IMPLEMENTED();
}

// Offset: 1516
// Size: 606
// EAX: pDest
// EDX: pDest_x
// EBX: pOffset_x
// ECX: pDest_y
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
    NOT_IMPLEMENTED();
}

// Offset: 2124
// Size: 651
// EAX: pDest
// EDX: pDest_x
// EBX: pOffset_x
// ECX: pDest_y
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
    NOT_IMPLEMENTED();
}

// Offset: 2776
// Size: 507
// EAX: pX_offset
// EDX: pY_offset
// EBX: pWidth
// ECX: pHeight
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

// Offset: 3284
// Size: 172
void SetIntegerMapRenders() {
    NOT_IMPLEMENTED();
}

// Offset: 3456
// Size: 370
void AdjustRenderScreenSize() {
    NOT_IMPLEMENTED();
}

// Offset: 3828
// Size: 87
void ScreenSmaller() {
    NOT_IMPLEMENTED();
}

// Offset: 3916
// Size: 87
void ScreenLarger() {
    NOT_IMPLEMENTED();
}

// Offset: 4004
// Size: 138
// EAX: pPalette
// EDX: pFirst_colour
// EBX: pCount
void DRSetPaletteEntries(br_pixelmap* pPalette, int pFirst_colour, int pCount) {
    NOT_IMPLEMENTED();
}

// Offset: 4144
// Size: 128
// EAX: pThe_palette
// EDX: pSet_current_palette
void DRSetPalette3(br_pixelmap* pThe_palette, int pSet_current_palette) {
    NOT_IMPLEMENTED();
}

// Offset: 4272
// Size: 140
// EAX: pThe_palette
// EDX: pSet_current_palette
void DRSetPalette2(br_pixelmap* pThe_palette, int pSet_current_palette) {
    NOT_IMPLEMENTED();
}

// Offset: 4412
// Size: 50
// EAX: pThe_palette
void DRSetPalette(br_pixelmap* pThe_palette) {
    NOT_IMPLEMENTED();
}

// Offset: 4464
// Size: 415
void InitializePalettes() {
    int j;
    NOT_IMPLEMENTED();
}

// Offset: 4880
// Size: 66
// EAX: pPal_name
void SwitchToPalette(char* pPal_name) {
    br_pixelmap* the_palette;
    NOT_IMPLEMENTED();
}

// Offset: 4948
// Size: 104
void ClearEntireScreen() {
    NOT_IMPLEMENTED();
}

// Offset: 5052
// Size: 75
void ClearWobbles() {
    NOT_IMPLEMENTED();
}

// Offset: 5128
// Size: 108
void InitWobbleStuff() {
    int i;

    for (i = 0; i < 5; i++) {
        gWobble_array[i].amplitude_x = 0;
    }

    for (i = 0; i < 64; i++) {
        gCosine_array[i] = cosf((double)i * 0.015625f * 3.141592653589793f * 0.5f);
    }
}

// Offset: 5236
// Size: 198
void NewScreenWobble(double pAmplitude_x, double pAmplitude_y, double pPeriod) {
    int i;
    int oldest_time;
    int oldest_index;
    NOT_IMPLEMENTED();
}

// Offset: 5436
// Size: 54
// EAX: pWobble_x
// EDX: pWobble_y
void SetScreenWobble(int pWobble_x, int pWobble_y) {
    NOT_IMPLEMENTED();
}

// Offset: 5492
// Size: 43
void ResetScreenWobble() {
    NOT_IMPLEMENTED();
}

// Offset: 5536
// Size: 813
// EAX: pThe_time
void CalculateWobblitude(tU32 pThe_time) {
    int i;
    tU32 time_going;
    double angle;
    double mod_angle;
    double cosine_over_angle;
    NOT_IMPLEMENTED();
}

// Offset: 6352
// Size: 605
// EAX: pThe_time
void CalculateConcussion(tU32 pThe_time) {
    tU32 time_difference;
    int i;
    int j;
    float the_amplitude;
    float angle;
    float mod_angle;
    float cosine_over_angle;
    NOT_IMPLEMENTED();
}

// Offset: 6960
// Size: 171
void SufferFromConcussion(float pSeriousness) {
    int i;
    int j;
    NOT_IMPLEMENTED();
}

// Offset: 7132
// Size: 56
// EAX: pRender_buffer
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pCamera_to_world
void ProcessNonTrackActors(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world, br_matrix34* pOld_camera_matrix) {
    NOT_IMPLEMENTED();
}

// Offset: 7188
// Size: 119
// EAX: pColour
int OppositeColour(int pColour) {
    int brightness;
    NOT_IMPLEMENTED();
}

// Offset: 7308
// Size: 1804
// EAX: pCar
// EDX: pTime
// EBX: pTrans
// ECX: pPos
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
    NOT_IMPLEMENTED();
}

// Offset: 9112
// Size: 257
// EAX: pTime
// EDX: pPos
// EBX: pColour
void DrawMapSmallBlip(tU32 pTime, br_vector3* pPos, int pColour) {
    br_vector3 map_pos;
    int offset;
    tU32 time_diff;
    NOT_IMPLEMENTED();
}

// Offset: 9372
// Size: 651
// EAX: pLight
// EDX: pCar
// EBX: p1
// ECX: p2
void MungeClipPlane(br_vector3* pLight, tCar_spec* pCar, br_vector3* p1, br_vector3* p2, br_scalar pY_offset) {
    br_vector3 v1;
    br_vector3 v2;
    br_vector3 v3;
    br_vector3 v4;
    br_scalar length;
    br_actor* new_clip;
    NOT_IMPLEMENTED();
}

// Offset: 10024
// Size: 180
// EAX: pCar
// EDX: pLight
// EBX: pIndex_1
void TryThisEdge(tCar_spec* pCar, br_vector3* pLight, int pIndex_1, br_scalar pSign_1, int pIndex_2, br_scalar pSign_2, int pPoint_index_1, int pPoint_index_2, br_scalar pY_offset) {
    br_scalar dot_1;
    br_scalar dot_2;
    br_scalar mult;
    NOT_IMPLEMENTED();
}

// Offset: 10204
// Size: 104
// EAX: pPos
br_scalar DistanceFromPlane(br_vector3* pPos, br_scalar pA, br_scalar pB, br_scalar pC, br_scalar pD) {
    br_vector3 normal;
    NOT_IMPLEMENTED();
}

// Offset: 10308
// Size: 84
void DisableLights() {
    NOT_IMPLEMENTED();
}

// Offset: 10392
// Size: 84
void EnableLights() {
    NOT_IMPLEMENTED();
}

// Offset: 10476
// Size: 4661
// EAX: pCar
// EDX: pWorld
// EBX: pTrack_spec
// ECX: pCamera
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
    NOT_IMPLEMENTED();
}

// Offset: 15140
// Size: 519
// EAX: pWorld
// EDX: pTrack_spec
// EBX: pCamera
// ECX: pCamera_to_world_transform
void RenderShadows(br_actor* pWorld, tTrack_spec* pTrack_spec, br_actor* pCamera, br_matrix34* pCamera_to_world_transform) {
    int i;
    int cat;
    int car_count;
    tCar_spec* the_car;
    br_vector3 camera_to_car;
    br_scalar distance_factor;
    NOT_IMPLEMENTED();
}

// Offset: 15660
// Size: 247
// EAX: pIndex
// EDX: pTime
void FlashyMapCheckpoint(int pIndex, tU32 pTime) {
    tCheckpoint* cp;
    static tU32 last_flash;
    static int flash_state;
    NOT_IMPLEMENTED();
}

// Offset: 15908
// Size: 252
// EAX: pPixelmap
int ConditionallyFillWithSky(br_pixelmap* pPixelmap) {
    int bgnd_col;
    NOT_IMPLEMENTED();
}

// Offset: 16160
// Size: 3751
// EAX: pDepth_mask_on
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
    NOT_IMPLEMENTED();
}

// Offset: 19912
// Size: 54
void InitPaletteAnimate() {
    NOT_IMPLEMENTED();
}

// Offset: 19968
// Size: 76
void RevertPalette() {
    NOT_IMPLEMENTED();
}

// Offset: 20044
// Size: 34
void MungePalette() {
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
    NOT_IMPLEMENTED();
}

// Offset: 20080
// Size: 49
void ResetPalette() {
    NOT_IMPLEMENTED();
}

// Offset: 20132
// Size: 65
// EAX: pPtr
// EDX: pDarken_amount
void Darken(tU8* pPtr, unsigned int pDarken_amount) {
    unsigned int value;
    NOT_IMPLEMENTED();
}

// Offset: 20200
// Size: 194
// EAX: pDegree
void SetFadedPalette(int pDegree) {
    int j;
    br_pixelmap* the_palette;
    char* the_pixels;
    NOT_IMPLEMENTED();
}

// Offset: 20396
// Size: 147
void FadePaletteDown() {
    int start_time;
    int the_time;
    NOT_IMPLEMENTED();
}

// Offset: 20544
// Size: 116
void FadePaletteUp() {
    int start_time;
    int the_time;
    NOT_IMPLEMENTED();
}

// Offset: 20660
// Size: 91
void KillSplashScreen() {
    NOT_IMPLEMENTED();
}

// Offset: 20752
// Size: 58
void EnsureRenderPalette() {
    NOT_IMPLEMENTED();
}

// Offset: 20812
// Size: 248
// EAX: pPixmap_name
void SplashScreenWith(char* pPixmap_name) {
    br_pixelmap* the_map;
    NOT_IMPLEMENTED();
}

// Offset: 21060
// Size: 48
void EnsurePaletteUp() {
    NOT_IMPLEMENTED();
}

// Offset: 21108
// Size: 103
br_uint_32 AmbientificateMaterial(br_material* pMat, void* pArg) {
    float a;
    NOT_IMPLEMENTED();
}

// Offset: 21212
// Size: 60
void ChangeAmbience(br_scalar pDelta) {
    NOT_IMPLEMENTED();
}

// Offset: 21272
// Size: 57
void InitAmbience() {
    NOT_IMPLEMENTED();
}

// Offset: 21332
// Size: 785
// EAX: pDest
// EDX: pDest_x
// EBX: pDest_y
// ECX: pSource
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
    NOT_IMPLEMENTED();
}

// Offset: 22120
// Size: 86
// EAX: pDest_x
// EDX: pDest_y
// EBX: pSource
void DRMaskedStamp(br_int_16 pDest_x, br_int_16 pDest_y, br_pixelmap* pSource) {
    NOT_IMPLEMENTED();
}

// Offset: 22208
// Size: 332
// EAX: pDest
// EDX: pDest_x
// EBX: pDest_y
// ECX: pSource
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
    NOT_IMPLEMENTED();
}

// Offset: 22540
// Size: 823
// EAX: pDest
// EDX: pDest_x
// EBX: pDest_y
// ECX: pSource
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
    NOT_IMPLEMENTED();
}

// Offset: 23364
// Size: 341
// EAX: pDest
// EDX: pDest_x
// EBX: pDest_y
// ECX: pSource
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
    NOT_IMPLEMENTED();
}

// Offset: 23708
// Size: 87
void InitTransientBitmaps() {
    NOT_IMPLEMENTED();
}

// Offset: 23796
// Size: 183
// EAX: pWidth
// EDX: pHeight
// EBX: pUser_data
int AllocateTransientBitmap(int pWidth, int pHeight, int pUser_data) {
    int bm_index;
    NOT_IMPLEMENTED();
}

// Offset: 23980
// Size: 96
// EAX: pIndex
void DeallocateTransientBitmap(int pIndex) {
    NOT_IMPLEMENTED();
}

// Offset: 24076
// Size: 67
void DeallocateAllTransientBitmaps() {
    NOT_IMPLEMENTED();
}

// Offset: 24144
// Size: 254
// EAX: pGraphically_remove_them
void RemoveTransientBitmaps(int pGraphically_remove_them) {
    int i;
    int order_number;
    NOT_IMPLEMENTED();
}

// Offset: 24400
// Size: 190
// EAX: pIndex
// EDX: pX_coord
// EBX: pY_coord
void SaveTransient(int pIndex, int pX_coord, int pY_coord) {
    NOT_IMPLEMENTED();
}

// Offset: 24592
// Size: 214
// EAX: pGib
void DrawCursorGiblet(tCursor_giblet* pGib) {
    br_pixelmap* the_image;
    NOT_IMPLEMENTED();
}

// Offset: 24808
// Size: 743
// EAX: pPeriod
void ProcessCursorGiblets(int pPeriod) {
    int i;
    int kill_the_giblet;
    tU32 time_now;
    tCursor_giblet* gib;
    NOT_IMPLEMENTED();
}

// Offset: 25552
// Size: 578
// EAX: pX_coord
// EDX: pY_coord
int NewCursorGiblet(int pX_coord, int pY_coord, float pX_speed, float pY_speed, tU32 pDrop_time) {
    int i;
    int the_width;
    int the_height;
    int sequence_number;
    NOT_IMPLEMENTED();
}

// Offset: 26132
// Size: 1373
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
    NOT_IMPLEMENTED();
}

// Offset: 27508
// Size: 186
int AllocateCursorTransient() {
    int largest_width;
    int largest_height;
    NOT_IMPLEMENTED();
}

// Offset: 27696
// Size: 138
void StartMouseCursor() {
    NOT_IMPLEMENTED();
}

// Offset: 27836
// Size: 59
void EndMouseCursor() {
    NOT_IMPLEMENTED();
}

// Offset: 27896
// Size: 568
// EAX: pFont_ID
void LoadFont(int pFont_ID) {
    tPath_name the_path;
    int i;
    int number_of_chars;
    FILE* f;
    tU32 the_size;
    NOT_IMPLEMENTED();
}

// Offset: 28464
// Size: 141
// EAX: pFont_ID
void DisposeFont(int pFont_ID) {
    NOT_IMPLEMENTED();
}

// Offset: 28608
// Size: 93
void InitDRFonts() {
    NOT_IMPLEMENTED();
}

// Offset: 28704
// Size: 251
// EAX: pImage
// EDX: pLeft
// EBX: pTop
// ECX: pTop_clip
void DrawDropImage(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip, int pOffset) {
    int y;
    int src_y;
    int src_height;
    int y_diff;
    NOT_IMPLEMENTED();
}

// Offset: 28956
// Size: 161
// EAX: pImage
// EDX: pLeft
// EBX: pTop
// ECX: pTop_clip
void DropInImageFromTop(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip) {
    tS32 start_time;
    tS32 the_time;
    int drop_distance;
    NOT_IMPLEMENTED();
}

// Offset: 29120
// Size: 150
// EAX: pImage
// EDX: pLeft
// EBX: pTop
// ECX: pTop_clip
void DropOutImageThruBottom(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip) {
    tS32 start_time;
    tS32 the_time;
    int drop_distance;
    NOT_IMPLEMENTED();
}

// Offset: 29272
// Size: 152
// EAX: pImage
// EDX: pLeft
// EBX: pTop
// ECX: pTop_clip
void DropInImageFromBottom(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip) {
    tS32 start_time;
    tS32 the_time;
    int drop_distance;
    NOT_IMPLEMENTED();
}

// Offset: 29424
// Size: 161
// EAX: pImage
// EDX: pLeft
// EBX: pTop
// ECX: pTop_clip
void DropOutImageThruTop(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip) {
    tS32 start_time;
    tS32 the_time;
    int drop_distance;
    NOT_IMPLEMENTED();
}

// Offset: 29588
// Size: 262
// EAX: pImage
// EDX: pLeft
// EBX: pTop
// ECX: pPercentage
void DrawTellyLine(br_pixelmap* pImage, int pLeft, int pTop, int pPercentage) {
    int the_width;
    int the_height;
    NOT_IMPLEMENTED();
}

// Offset: 29852
// Size: 203
// EAX: pImage
// EDX: pLeft
// EBX: pTop
// ECX: pPercentage
void DrawTellyImage(br_pixelmap* pImage, int pLeft, int pTop, int pPercentage) {
    int the_height;
    NOT_IMPLEMENTED();
}

// Offset: 30056
// Size: 194
// EAX: pImage
// EDX: pLeft
// EBX: pTop
void TellyInImage(br_pixelmap* pImage, int pLeft, int pTop) {
    tS32 start_time;
    tS32 the_time;
    NOT_IMPLEMENTED();
}

// Offset: 30252
// Size: 220
// EAX: pImage
// EDX: pLeft
// EBX: pTop
void TellyOutImage(br_pixelmap* pImage, int pLeft, int pTop) {
    tS32 start_time;
    tS32 the_time;
    int drop_distance;
    NOT_IMPLEMENTED();
}

// Offset: 30472
// Size: 45
// EAX: pLevel
void SetShadowLevel(tShadow_level pLevel) {
    LOG_TRACE("(%d)", pLevel);
    gShadow_level = pLevel;
}

// Offset: 30520
// Size: 45
tShadow_level GetShadowLevel() {
    return gShadow_level;
    NOT_IMPLEMENTED();
}

// Offset: 30568
// Size: 247
void ToggleShadow() {
    NOT_IMPLEMENTED();
}

// Offset: 30816
// Size: 503
void InitShadow() {
    int i;
    br_vector3 temp_v;
    NOT_IMPLEMENTED();
}

// Offset: 31320
// Size: 140
br_uint_32 SaveShadeTable(br_pixelmap* pTable, void* pArg) {
    NOT_IMPLEMENTED();
}

// Offset: 31460
// Size: 71
void SaveShadeTables() {
    NOT_IMPLEMENTED();
}

// Offset: 31532
// Size: 84
void DisposeSavedShadeTables() {
    NOT_IMPLEMENTED();
}

// Offset: 31616
// Size: 134
void ShadowMode() {
    NOT_IMPLEMENTED();
}

// Offset: 31752
// Size: 114
int SwitchToRealResolution() {
    NOT_IMPLEMENTED();
}

// Offset: 31868
// Size: 125
int SwitchToLoresMode() {
    NOT_IMPLEMENTED();
}

// Offset: 31996
// Size: 507
// EAX: pDestn
// EDX: pSource
// EBX: pSource_width
// ECX: pSource_height
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
    NOT_IMPLEMENTED();
}
