#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "br_types.h"
#include "dr_types.h"

extern int gPalette_munged;
extern int gColourValues[1];
extern int gNext_transient;
extern int gCursor_x_offsets[8];
extern int gCursor_y_offsets[8];
extern int gCursor_gib_x_offsets[8];
extern int gCursor_gib_y_offsets[8];
extern int gCursor_giblet_sequence0[7];
extern int gCursor_giblet_sequence1[5];
extern int gCursor_giblet_sequence2[5];
extern int gCursor_giblet_sequence3[5];
extern int *gCursor_giblet_sequences[4];
extern char *gFont_names[21];
extern br_colour gRGB_colours[9];
extern br_matrix34 gSheer_mat;
extern br_matrix34 gIdentity34;
extern tShadow_level gShadow_level;
extern br_scalar gShadow_hither_z_move;
extern br_scalar gShadow_hither_min_move;
extern int gArrows[2][4][60];
extern float gMap_render_x;
extern float gMap_render_y;
extern float gMap_render_width;
extern float gMap_render_height;
extern int gMouse_started;
extern int gFaded_palette;
extern int gAR_fudge_headups;
extern br_pixelmap *gCurrent_splash;
extern br_pixelmap *gCurrent_conversion_table;
extern int gMap_colours[4];
extern br_vector3 gShadow_points[8];
extern tConcussion gConcussion;
extern tClip_details gShadow_clip_planes[8];
extern br_actor *gLollipops[100];
extern tWobble_spec gWobble_array[5];
extern tSaved_table gSaved_shade_tables[100];
extern tCursor_giblet gCursor_giblets[45];
extern tTransient_bm gTransient_bitmaps[50];
extern float gCosine_array[64];
extern br_pixelmap *gCursors[8];
extern br_pixelmap *gCursor_giblet_images[18];
extern br_pixelmap *gEval_1;
extern br_pixelmap *gEval_2;
extern br_vector3 gShadow_light_z;
extern br_vector3 gShadow_light_x;
extern int gShadow_dim_amount;
extern int gNumber_of_lollipops;
extern br_vector3 gShadow_light_ray;
extern int gFancy_shadow;
extern br_model *gShadow_model;
extern br_actor *gShadow_actor;
extern int gShadow_clip_plane_count;
extern br_pixelmap *gPalette_conversion_table;
extern br_material *gShadow_material;
extern int gSaved_table_count;
extern int gCurrent_cursor_index;
extern int gPalette_index;
extern int gCursor_transient_index;
extern char *gScratch_pixels;
extern br_pixelmap *gScratch_palette;
extern int gLast_palette_change;
extern br_pixelmap *gOrig_render_palette;
extern br_pixelmap *gCurrent_palette;
extern br_pixelmap *gRender_palette;
extern float gCamera_to_horiz_angle;
extern int gColours[9];
extern br_pixelmap *gFlic_palette;
extern tDR_font gFonts[21];
extern char *gCurrent_palette_pixels;
extern int gWidth;
extern int gMap_render_height_i;
extern int gScreen_wobble_x;
extern int gScreen_wobble_y;
extern br_scalar gCurrent_ambience;
extern int gY_offset;
extern int gMap_render_width_i;
extern int gMouse_in_use;
extern int gHeight;
extern int gMouse_last_y_coord;
extern int gMouse_last_x_coord;
extern br_scalar gAmbient_adjustment;
extern int gMap_render_x_i;
extern int gX_offset;
extern int gMap_render_y_i;
extern int gMirror_on__graphics; // suffix added to avoid duplicate symbol
extern br_scalar gYon_squared;

void TurnOnPaletteConversion();

void TurnOffPaletteConversion();

void ResetLollipopQueue();

int AddToLollipopQueue(br_actor* pActor, int pIndex);

void RenderLollipops();

void DRDrawLine(br_pixelmap* pDestn, int pX1, int pY1, int pX2, int pY2, int pColour);

void DrawDigitAt(br_pixelmap* gImage, int pX, int pY, int pY_pitch, int pValue);

void DrawNumberAt(br_pixelmap* gImage, int pX, int pY, int pX_pitch, int pY_pitch, int pValue, int pDigit_count, int pLeading_zeroes);

void BuildColourTable(br_pixelmap* pPalette);

void ClearConcussion();

tS8* SkipLines(tS8* pSource, int pCount);

void CopyWords(char* pDst, char* pSrc, int pN);

void Copy8BitStripImageTo16Bit(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pOffset_x, br_int_16 pDest_y, br_int_16 pOffset_y, tS8* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_uint_16 pWidth, br_uint_16 pHeight);

void CopyStripImage(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pOffset_x, br_int_16 pDest_y, br_int_16 pOffset_y, tS8* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_uint_16 pWidth, br_uint_16 pHeight);

void SetBRenderScreenAndBuffers(int pX_offset, int pY_offset, int pWidth, int pHeight);

void SetIntegerMapRenders();

void AdjustRenderScreenSize();

void ScreenSmaller();

void ScreenLarger();

void DRSetPaletteEntries(br_pixelmap* pPalette, int pFirst_colour, int pCount);

void DRSetPalette3(br_pixelmap* pThe_palette, int pSet_current_palette);

void DRSetPalette2(br_pixelmap* pThe_palette, int pSet_current_palette);

void DRSetPalette(br_pixelmap* pThe_palette);

void InitializePalettes();

void SwitchToPalette(char* pPal_name);

void ClearEntireScreen();

void ClearWobbles();

void InitWobbleStuff();

void NewScreenWobble(double pAmplitude_x, double pAmplitude_y, double pPeriod);

void SetScreenWobble(int pWobble_x, int pWobble_y);

void ResetScreenWobble();

void CalculateWobblitude(tU32 pThe_time);

void CalculateConcussion(tU32 pThe_time);

void SufferFromConcussion(float pSeriousness);

void ProcessNonTrackActors(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world, br_matrix34* pOld_camera_matrix);

int OppositeColour(int pColour);

void DrawMapBlip(tCar_spec* pCar, tU32 pTime, br_matrix34* pTrans, br_vector3* pPos, int pColour);

void DrawMapSmallBlip(tU32 pTime, br_vector3* pPos, int pColour);

void MungeClipPlane(br_vector3* pLight, tCar_spec* pCar, br_vector3* p1, br_vector3* p2, br_scalar pY_offset);

void TryThisEdge(tCar_spec* pCar, br_vector3* pLight, int pIndex_1, br_scalar pSign_1, int pIndex_2, br_scalar pSign_2, int pPoint_index_1, int pPoint_index_2, br_scalar pY_offset);

br_scalar DistanceFromPlane(br_vector3* pPos, br_scalar pA, br_scalar pB, br_scalar pC, br_scalar pD);

void DisableLights();

void EnableLights();

void ProcessShadow(tCar_spec* pCar, br_actor* pWorld, tTrack_spec* pTrack_spec, br_actor* pCamera, br_matrix34* pCamera_to_world_transform, br_scalar pDistance_factor);

void RenderShadows(br_actor* pWorld, tTrack_spec* pTrack_spec, br_actor* pCamera, br_matrix34* pCamera_to_world_transform);

void FlashyMapCheckpoint(int pIndex, tU32 pTime);

int ConditionallyFillWithSky(br_pixelmap* pPixelmap);

void RenderAFrame(int pDepth_mask_on);

void InitPaletteAnimate();

void RevertPalette();

void MungePalette();

void ResetPalette();

void Darken(tU8* pPtr, unsigned int pDarken_amount);

void SetFadedPalette(int pDegree);

void FadePaletteDown();

void FadePaletteUp();

void KillSplashScreen();

void EnsureRenderPalette();

void SplashScreenWith(char* pPixmap_name);

void EnsurePaletteUp();

br_uint_32 AmbientificateMaterial(br_material* pMat, void* pArg);

void ChangeAmbience(br_scalar pDelta);

void InitAmbience();

void DRPixelmapRectangleMaskedCopy(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pDest_y, br_pixelmap* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_int_16 pWidth, br_int_16 pHeight);

void DRMaskedStamp(br_int_16 pDest_x, br_int_16 pDest_y, br_pixelmap* pSource);

void DRPixelmapRectangleOnscreenCopy(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pDest_y, br_pixelmap* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_int_16 pWidth, br_int_16 pHeight);

void DRPixelmapRectangleShearedCopy(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pDest_y, br_pixelmap* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_int_16 pWidth, br_int_16 pHeight, tX1616 pShear);

void DRPixelmapRectangleVScaledCopy(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pDest_y, br_pixelmap* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_int_16 pWidth, br_int_16 pHeight);

void InitTransientBitmaps();

int AllocateTransientBitmap(int pWidth, int pHeight, int pUser_data);

void DeallocateTransientBitmap(int pIndex);

void DeallocateAllTransientBitmaps();

void RemoveTransientBitmaps(int pGraphically_remove_them);

void SaveTransient(int pIndex, int pX_coord, int pY_coord);

void DrawCursorGiblet(tCursor_giblet* pGib);

void ProcessCursorGiblets(int pPeriod);

int NewCursorGiblet(int pX_coord, int pY_coord, float pX_speed, float pY_speed, tU32 pDrop_time);

int DoMouseCursor();

int AllocateCursorTransient();

void StartMouseCursor();

void EndMouseCursor();

void LoadFont(int pFont_ID);

void DisposeFont(int pFont_ID);

void InitDRFonts();

void DrawDropImage(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip, int pOffset);

void DropInImageFromTop(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip);

void DropOutImageThruBottom(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip);

void DropInImageFromBottom(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip);

void DropOutImageThruTop(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip);

void DrawTellyLine(br_pixelmap* pImage, int pLeft, int pTop, int pPercentage);

void DrawTellyImage(br_pixelmap* pImage, int pLeft, int pTop, int pPercentage);

void TellyInImage(br_pixelmap* pImage, int pLeft, int pTop);

void TellyOutImage(br_pixelmap* pImage, int pLeft, int pTop);

void SetShadowLevel(tShadow_level pLevel);

tShadow_level GetShadowLevel();

void ToggleShadow();

void InitShadow();

br_uint_32 SaveShadeTable(br_pixelmap* pTable, void* pArg);

void SaveShadeTables();

void DisposeSavedShadeTables();

void ShadowMode();

int SwitchToRealResolution();

int SwitchToLoresMode();

void DRPixelmapDoubledCopy(br_pixelmap* pDestn, br_pixelmap* pSource, int pSource_width, int pSource_height, int pX_offset, int pY_offset);

#endif
