#ifndef _DEPTH_H_
#define _DEPTH_H_

#include "br_types.h"
#include "dr_types.h"

int Log2(int pNumber);

br_scalar CalculateWrappingMultiplier(br_scalar pValue, br_scalar pYon);

br_scalar DepthCueingShiftToDistance(int pShift);

void FogAccordingToGPSCDE(br_material* pMaterial);

void FrobFog();

void InstantDepthChange(tDepth_effect_type pType, br_pixelmap* pSky_texture, int pStart, int pEnd);

br_scalar Tan(br_scalar pAngle);

br_scalar EdgeU(br_angle pSky, br_angle pView, br_angle pPerfect);

void MungeSkyModel(br_actor* pCamera, br_model* pModel);

br_model* CreateHorizonModel(br_actor* pCamera);

void LoadDepthTable(char* pName, br_pixelmap** pTable, int* pPower);

void InitDepthEffects();

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

void LessDepthFactor();

void MoreDepthFactor();

void LessDepthFactor2();

void MoreDepthFactor2();

void AssertYons();

void IncreaseYon();

void DecreaseYon();

void SetYon(br_scalar pYon);

br_scalar GetYon();

void IncreaseAngle();

void DecreaseAngle();

void ToggleDepthMode();

int GetSkyTextureOn();

void SetSkyTextureOn(int pOn);

void ToggleSkyQuietly();

void ToggleSky();

int GetDepthCueingOn();

void SetDepthCueingOn(int pOn);

void ToggleDepthCueingQuietly();

void ToggleDepthCueing();

void ChangeDepthEffect();

void MungeForwardSky();

void MungeRearviewSky();

#endif
