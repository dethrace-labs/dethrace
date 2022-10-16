#ifndef _DEPTH_H_
#define _DEPTH_H_

#include "brender/br_types.h"
#include "dr_types.h"

extern tDepth_effect gDistance_depth_effects[4];
extern int gSky_on;
extern int gDepth_cueing_on;
extern tDepth_effect_type gSwap_depth_effect_type;
extern br_scalar gSky_height;
extern br_scalar gSky_x_multiplier;
extern br_scalar gSky_width;
extern br_scalar gSky_y_multiplier;
extern tU32 gLast_depth_change;
extern br_scalar gOld_yon;
extern br_pixelmap* gWater_shade_table;
extern br_material* gHorizon_material;
extern br_model* gRearview_sky_model;
extern int gFog_shade_table_power;
extern br_actor* gRearview_sky_actor;
extern int gAcid_shade_table_power;
extern int gWater_shade_table_power;
extern br_model* gForward_sky_model;
extern br_actor* gForward_sky_actor;
extern int gDepth_shade_table_power;
extern br_pixelmap* gFog_shade_table;
extern int gSwap_depth_effect_start;
extern br_pixelmap* gDepth_shade_table;
extern tSpecial_volume* gLast_camera_special_volume;
extern br_pixelmap* gAcid_shade_table;
extern int gSwap_depth_effect_end;
extern br_pixelmap* gSwap_sky_texture;
extern br_angle gOld_fov;
extern br_angle gSky_image_width;
extern br_angle gSky_image_height;
extern br_angle gSky_image_underground;

int Log2(int pNumber);

br_scalar CalculateWrappingMultiplier(br_scalar pValue, br_scalar pYon);

br_scalar DepthCueingShiftToDistance(int pShift);

void FogAccordingToGPSCDE(br_material* pMaterial);

void FrobFog(void);

void InstantDepthChange(tDepth_effect_type pType, br_pixelmap* pSky_texture, int pStart, int pEnd);

br_scalar Tan(br_scalar pAngle);

br_scalar EdgeU(br_angle pSky, br_angle pView, br_angle pPerfect);

void MungeSkyModel(br_actor* pCamera, br_model* pModel);

br_model* CreateHorizonModel(br_actor* pCamera);

void LoadDepthTable(char* pName, br_pixelmap** pTable, int* pPower);

void InitDepthEffects(void);

void DoDepthByShadeTable(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_pixelmap* pShade_table, int pShade_table_power, int pStart, int pEnd);

void ExternalSky(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world);

void DoHorizon(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world);

void DoDepthCue(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer);

void DoFog(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer);

void DepthEffect(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world);

void DepthEffectSky(br_pixelmap* pRender_buffer, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world);

void DoWobbleCamera(br_actor* pCamera);

void DoDrugWobbleCamera(br_actor* pCamera);

void DoSpecialCameraEffect(br_actor* pCamera, br_matrix34* pCamera_to_world);

void LessDepthFactor(void);

void MoreDepthFactor(void);

void LessDepthFactor2(void);

void MoreDepthFactor2(void);

void AssertYons(void);

void IncreaseYon(void);

void DecreaseYon(void);

void SetYon(br_scalar pYon);

br_scalar GetYon(void);

void IncreaseAngle(void);

void DecreaseAngle(void);

void ToggleDepthMode(void);

int GetSkyTextureOn(void);

void SetSkyTextureOn(int pOn);

void ToggleSkyQuietly(void);

void ToggleSky(void);

int GetDepthCueingOn(void);

void SetDepthCueingOn(int pOn);

void ToggleDepthCueingQuietly(void);

void ToggleDepthCueing(void);

void ChangeDepthEffect(void);

void MungeForwardSky(void);

void MungeRearviewSky(void);

#endif
