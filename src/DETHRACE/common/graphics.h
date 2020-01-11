#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "br_types.h"
#include "dr_types.h"

// Offset: 0
// Size: 44
void TurnOnPaletteConversion();

// Offset: 44
// Size: 44
void TurnOffPaletteConversion();

// Offset: 88
// Size: 44
void ResetLollipopQueue();

// Offset: 132
// Size: 124
// EAX: pActor
// EDX: pIndex
int AddToLollipopQueue(br_actor* pActor, int pIndex);

// Offset: 256
// Size: 237
void RenderLollipops();

// Offset: 496
// Size: 106
// EAX: pDestn
// EDX: pX1
// EBX: pY1
// ECX: pX2
void DRDrawLine(br_pixelmap* pDestn, int pX1, int pY1, int pX2, int pY2, int pColour);

// Offset: 604
// Size: 90
// EAX: gImage
// EDX: pX
// EBX: pY
// ECX: pY_pitch
void DrawDigitAt(br_pixelmap* gImage, int pX, int pY, int pY_pitch, int pValue);

// Offset: 696
// Size: 156
// EAX: gImage
// EDX: pX
// EBX: pY
// ECX: pX_pitch
void DrawNumberAt(br_pixelmap* gImage, int pX, int pY, int pX_pitch, int pY_pitch, int pValue, int pDigit_count, int pLeading_zeroes);

// Offset: 852
// Size: 350
// EAX: pPalette
void BuildColourTable(br_pixelmap* pPalette);

// Offset: 1204
// Size: 44
void ClearConcussion();

// Offset: 1248
// Size: 137
// EAX: pSource
// EDX: pCount
tS8* SkipLines(tS8* pSource, int pCount);

// Offset: 1388
// Size: 126
// EAX: pDst
// EDX: pSrc
// EBX: pN
void CopyWords(char* pDst, char* pSrc, int pN);

// Offset: 1516
// Size: 606
// EAX: pDest
// EDX: pDest_x
// EBX: pOffset_x
// ECX: pDest_y
void Copy8BitStripImageTo16Bit(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pOffset_x, br_int_16 pDest_y, br_int_16 pOffset_y, tS8* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_uint_16 pWidth, br_uint_16 pHeight);

// Offset: 2124
// Size: 651
// EAX: pDest
// EDX: pDest_x
// EBX: pOffset_x
// ECX: pDest_y
void CopyStripImage(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pOffset_x, br_int_16 pDest_y, br_int_16 pOffset_y, tS8* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_uint_16 pWidth, br_uint_16 pHeight);

// Offset: 2776
// Size: 507
// EAX: pX_offset
// EDX: pY_offset
// EBX: pWidth
// ECX: pHeight
void SetBRenderScreenAndBuffers(int pX_offset, int pY_offset, int pWidth, int pHeight);

// Offset: 3284
// Size: 172
void SetIntegerMapRenders();

// Offset: 3456
// Size: 370
void AdjustRenderScreenSize();

// Offset: 3828
// Size: 87
void ScreenSmaller();

// Offset: 3916
// Size: 87
void ScreenLarger();

// Offset: 4004
// Size: 138
// EAX: pPalette
// EDX: pFirst_colour
// EBX: pCount
void DRSetPaletteEntries(br_pixelmap* pPalette, int pFirst_colour, int pCount);

// Offset: 4144
// Size: 128
// EAX: pThe_palette
// EDX: pSet_current_palette
void DRSetPalette3(br_pixelmap* pThe_palette, int pSet_current_palette);

// Offset: 4272
// Size: 140
// EAX: pThe_palette
// EDX: pSet_current_palette
void DRSetPalette2(br_pixelmap* pThe_palette, int pSet_current_palette);

// Offset: 4412
// Size: 50
// EAX: pThe_palette
void DRSetPalette(br_pixelmap* pThe_palette);

// Offset: 4464
// Size: 415
void InitializePalettes();

// Offset: 4880
// Size: 66
// EAX: pPal_name
void SwitchToPalette(char* pPal_name);

// Offset: 4948
// Size: 104
void ClearEntireScreen();

// Offset: 5052
// Size: 75
void ClearWobbles();

// Offset: 5128
// Size: 108
void InitWobbleStuff();

// Offset: 5236
// Size: 198
void NewScreenWobble(double pAmplitude_x, double pAmplitude_y, double pPeriod);

// Offset: 5436
// Size: 54
// EAX: pWobble_x
// EDX: pWobble_y
void SetScreenWobble(int pWobble_x, int pWobble_y);

// Offset: 5492
// Size: 43
void ResetScreenWobble();

// Offset: 5536
// Size: 813
// EAX: pThe_time
void CalculateWobblitude(tU32 pThe_time);

// Offset: 6352
// Size: 605
// EAX: pThe_time
void CalculateConcussion(tU32 pThe_time);

// Offset: 6960
// Size: 171
void SufferFromConcussion(float pSeriousness);

// Offset: 7132
// Size: 56
// EAX: pRender_buffer
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pCamera_to_world
void ProcessNonTrackActors(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world, br_matrix34* pOld_camera_matrix);

// Offset: 7188
// Size: 119
// EAX: pColour
int OppositeColour(int pColour);

// Offset: 7308
// Size: 1804
// EAX: pCar
// EDX: pTime
// EBX: pTrans
// ECX: pPos
void DrawMapBlip(tCar_spec* pCar, tU32 pTime, br_matrix34* pTrans, br_vector3* pPos, int pColour);

// Offset: 9112
// Size: 257
// EAX: pTime
// EDX: pPos
// EBX: pColour
void DrawMapSmallBlip(tU32 pTime, br_vector3* pPos, int pColour);

// Offset: 9372
// Size: 651
// EAX: pLight
// EDX: pCar
// EBX: p1
// ECX: p2
void MungeClipPlane(br_vector3* pLight, tCar_spec* pCar, br_vector3* p1, br_vector3* p2, br_scalar pY_offset);

// Offset: 10024
// Size: 180
// EAX: pCar
// EDX: pLight
// EBX: pIndex_1
void TryThisEdge(tCar_spec* pCar, br_vector3* pLight, int pIndex_1, br_scalar pSign_1, int pIndex_2, br_scalar pSign_2, int pPoint_index_1, int pPoint_index_2, br_scalar pY_offset);

// Offset: 10204
// Size: 104
// EAX: pPos
br_scalar DistanceFromPlane(br_vector3* pPos, br_scalar pA, br_scalar pB, br_scalar pC, br_scalar pD);

// Offset: 10308
// Size: 84
void DisableLights();

// Offset: 10392
// Size: 84
void EnableLights();

// Offset: 10476
// Size: 4661
// EAX: pCar
// EDX: pWorld
// EBX: pTrack_spec
// ECX: pCamera
void ProcessShadow(tCar_spec* pCar, br_actor* pWorld, tTrack_spec* pTrack_spec, br_actor* pCamera, br_matrix34* pCamera_to_world_transform, br_scalar pDistance_factor);

// Offset: 15140
// Size: 519
// EAX: pWorld
// EDX: pTrack_spec
// EBX: pCamera
// ECX: pCamera_to_world_transform
void RenderShadows(br_actor* pWorld, tTrack_spec* pTrack_spec, br_actor* pCamera, br_matrix34* pCamera_to_world_transform);

// Offset: 15660
// Size: 247
// EAX: pIndex
// EDX: pTime
void FlashyMapCheckpoint(int pIndex, tU32 pTime);

// Offset: 15908
// Size: 252
// EAX: pPixelmap
int ConditionallyFillWithSky(br_pixelmap* pPixelmap);

// Offset: 16160
// Size: 3751
// EAX: pDepth_mask_on
void RenderAFrame(int pDepth_mask_on);

// Offset: 19912
// Size: 54
void InitPaletteAnimate();

// Offset: 19968
// Size: 76
void RevertPalette();

// Offset: 20044
// Size: 34
void MungePalette();

// Offset: 20080
// Size: 49
void ResetPalette();

// Offset: 20132
// Size: 65
// EAX: pPtr
// EDX: pDarken_amount
void Darken(tU8* pPtr, unsigned int pDarken_amount);

// Offset: 20200
// Size: 194
// EAX: pDegree
void SetFadedPalette(int pDegree);

// Offset: 20396
// Size: 147
void FadePaletteDown();

// Offset: 20544
// Size: 116
void FadePaletteUp();

// Offset: 20660
// Size: 91
void KillSplashScreen();

// Offset: 20752
// Size: 58
void EnsureRenderPalette();

// Offset: 20812
// Size: 248
// EAX: pPixmap_name
void SplashScreenWith(char* pPixmap_name);

// Offset: 21060
// Size: 48
void EnsurePaletteUp();

// Offset: 21108
// Size: 103
br_uint_32 AmbientificateMaterial(br_material* pMat, void* pArg);

// Offset: 21212
// Size: 60
void ChangeAmbience(br_scalar pDelta);

// Offset: 21272
// Size: 57
void InitAmbience();

// Offset: 21332
// Size: 785
// EAX: pDest
// EDX: pDest_x
// EBX: pDest_y
// ECX: pSource
void DRPixelmapRectangleMaskedCopy(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pDest_y, br_pixelmap* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_int_16 pWidth, br_int_16 pHeight);

// Offset: 22120
// Size: 86
// EAX: pDest_x
// EDX: pDest_y
// EBX: pSource
void DRMaskedStamp(br_int_16 pDest_x, br_int_16 pDest_y, br_pixelmap* pSource);

// Offset: 22208
// Size: 332
// EAX: pDest
// EDX: pDest_x
// EBX: pDest_y
// ECX: pSource
void DRPixelmapRectangleOnscreenCopy(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pDest_y, br_pixelmap* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_int_16 pWidth, br_int_16 pHeight);

// Offset: 22540
// Size: 823
// EAX: pDest
// EDX: pDest_x
// EBX: pDest_y
// ECX: pSource
void DRPixelmapRectangleShearedCopy(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pDest_y, br_pixelmap* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_int_16 pWidth, br_int_16 pHeight, tX1616 pShear);

// Offset: 23364
// Size: 341
// EAX: pDest
// EDX: pDest_x
// EBX: pDest_y
// ECX: pSource
void DRPixelmapRectangleVScaledCopy(br_pixelmap* pDest, br_int_16 pDest_x, br_int_16 pDest_y, br_pixelmap* pSource, br_int_16 pSource_x, br_int_16 pSource_y, br_int_16 pWidth, br_int_16 pHeight);

// Offset: 23708
// Size: 87
void InitTransientBitmaps();

// Offset: 23796
// Size: 183
// EAX: pWidth
// EDX: pHeight
// EBX: pUser_data
int AllocateTransientBitmap(int pWidth, int pHeight, int pUser_data);

// Offset: 23980
// Size: 96
// EAX: pIndex
void DeallocateTransientBitmap(int pIndex);

// Offset: 24076
// Size: 67
void DeallocateAllTransientBitmaps();

// Offset: 24144
// Size: 254
// EAX: pGraphically_remove_them
void RemoveTransientBitmaps(int pGraphically_remove_them);

// Offset: 24400
// Size: 190
// EAX: pIndex
// EDX: pX_coord
// EBX: pY_coord
void SaveTransient(int pIndex, int pX_coord, int pY_coord);

// Offset: 24592
// Size: 214
// EAX: pGib
void DrawCursorGiblet(tCursor_giblet* pGib);

// Offset: 24808
// Size: 743
// EAX: pPeriod
void ProcessCursorGiblets(int pPeriod);

// Offset: 25552
// Size: 578
// EAX: pX_coord
// EDX: pY_coord
int NewCursorGiblet(int pX_coord, int pY_coord, float pX_speed, float pY_speed, tU32 pDrop_time);

// Offset: 26132
// Size: 1373
int DoMouseCursor();

// Offset: 27508
// Size: 186
int AllocateCursorTransient();

// Offset: 27696
// Size: 138
void StartMouseCursor();

// Offset: 27836
// Size: 59
void EndMouseCursor();

// Offset: 27896
// Size: 568
// EAX: pFont_ID
void LoadFont(int pFont_ID);

// Offset: 28464
// Size: 141
// EAX: pFont_ID
void DisposeFont(int pFont_ID);

// Offset: 28608
// Size: 93
void InitDRFonts();

// Offset: 28704
// Size: 251
// EAX: pImage
// EDX: pLeft
// EBX: pTop
// ECX: pTop_clip
void DrawDropImage(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip, int pOffset);

// Offset: 28956
// Size: 161
// EAX: pImage
// EDX: pLeft
// EBX: pTop
// ECX: pTop_clip
void DropInImageFromTop(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip);

// Offset: 29120
// Size: 150
// EAX: pImage
// EDX: pLeft
// EBX: pTop
// ECX: pTop_clip
void DropOutImageThruBottom(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip);

// Offset: 29272
// Size: 152
// EAX: pImage
// EDX: pLeft
// EBX: pTop
// ECX: pTop_clip
void DropInImageFromBottom(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip);

// Offset: 29424
// Size: 161
// EAX: pImage
// EDX: pLeft
// EBX: pTop
// ECX: pTop_clip
void DropOutImageThruTop(br_pixelmap* pImage, int pLeft, int pTop, int pTop_clip, int pBottom_clip);

// Offset: 29588
// Size: 262
// EAX: pImage
// EDX: pLeft
// EBX: pTop
// ECX: pPercentage
void DrawTellyLine(br_pixelmap* pImage, int pLeft, int pTop, int pPercentage);

// Offset: 29852
// Size: 203
// EAX: pImage
// EDX: pLeft
// EBX: pTop
// ECX: pPercentage
void DrawTellyImage(br_pixelmap* pImage, int pLeft, int pTop, int pPercentage);

// Offset: 30056
// Size: 194
// EAX: pImage
// EDX: pLeft
// EBX: pTop
void TellyInImage(br_pixelmap* pImage, int pLeft, int pTop);

// Offset: 30252
// Size: 220
// EAX: pImage
// EDX: pLeft
// EBX: pTop
void TellyOutImage(br_pixelmap* pImage, int pLeft, int pTop);

// Offset: 30472
// Size: 45
// EAX: pLevel
void SetShadowLevel(tShadow_level pLevel);

// Offset: 30520
// Size: 45
tShadow_level GetShadowLevel();

// Offset: 30568
// Size: 247
void ToggleShadow();

// Offset: 30816
// Size: 503
void InitShadow();

// Offset: 31320
// Size: 140
br_uint_32 SaveShadeTable(br_pixelmap* pTable, void* pArg);

// Offset: 31460
// Size: 71
void SaveShadeTables();

// Offset: 31532
// Size: 84
void DisposeSavedShadeTables();

// Offset: 31616
// Size: 134
void ShadowMode();

// Offset: 31752
// Size: 114
int SwitchToRealResolution();

// Offset: 31868
// Size: 125
int SwitchToLoresMode();

// Offset: 31996
// Size: 507
// EAX: pDestn
// EDX: pSource
// EBX: pSource_width
// ECX: pSource_height
void DRPixelmapDoubledCopy(br_pixelmap* pDestn, br_pixelmap* pSource, int pSource_width, int pSource_height, int pX_offset, int pY_offset);

#endif
