#include "dr_types.h"
#include "br_types.h"
// Offset: 0
// Size: 110
// EAX: pNumber
int Log2(int pNumber);

// Offset: 112
// Size: 130
br_scalar CalculateWrappingMultiplier(br_scalar pValue, br_scalar pYon);

// Offset: 244
// Size: 78
// EAX: pShift
br_scalar DepthCueingShiftToDistance(int pShift);

// Offset: 324
// Size: 210
// EAX: pMaterial
void FogAccordingToGPSCDE(br_material *pMaterial);

// Offset: 536
// Size: 206
void FrobFog();

// Offset: 744
// Size: 170
// EAX: pType
// EDX: pSky_texture
// EBX: pStart
// ECX: pEnd
void InstantDepthChange(tDepth_effect_type pType, br_pixelmap *pSky_texture, int pStart);

// Offset: 916
// Size: 90
br_scalar Tan(br_scalar pAngle);

// Offset: 1008
// Size: 213
// EAX: pSky
// EDX: pView
// EBX: pPerfect
br_scalar EdgeU(br_angle pSky, br_angle pView, br_angle pPerfect);

// Offset: 1224
// Size: 1602
// EAX: pCamera
// EDX: pModel
void MungeSkyModel(br_actor *pCamera, br_model *pModel);

// Offset: 2828
// Size: 882
// EAX: pCamera
br_model* CreateHorizonModel(br_actor *pCamera);

// Offset: 3712
// Size: 408
// EAX: pName
// EDX: pTable
// EBX: pPower
void LoadDepthTable(char *pName, br_pixelmap **pTable, int *pPower, div_t the_path);

// Offset: 4120
// Size: 611
void InitDepthEffects();

// Offset: 4732
// Size: 769
// EAX: pRender_buffer
// EDX: pDepth_buffer
// EBX: pShade_table
// ECX: pShade_table_power
void DoDepthByShadeTable(br_pixelmap *pRender_buffer, br_pixelmap *pDepth_buffer, br_pixelmap *pShade_table, int pShade_table_power);

// Offset: 5504
// Size: 1009
// EAX: pRender_buffer
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pCamera_to_world
void ExternalSky(br_pixelmap *pRender_buffer, br_pixelmap *pDepth_buffer, br_actor *pCamera, br_matrix34 *pCamera_to_world);

// Offset: 6516
// Size: 463
// EAX: pRender_buffer
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pCamera_to_world
void DoHorizon(br_pixelmap *pRender_buffer, br_pixelmap *pDepth_buffer, br_actor *pCamera, br_matrix34 *pCamera_to_world);

// Offset: 6980
// Size: 73
// EAX: pRender_buffer
// EDX: pDepth_buffer
void DoDepthCue(br_pixelmap *pRender_buffer, br_pixelmap *pDepth_buffer);

// Offset: 7056
// Size: 73
// EAX: pRender_buffer
// EDX: pDepth_buffer
void DoFog(br_pixelmap *pRender_buffer, br_pixelmap *pDepth_buffer);

// Offset: 7132
// Size: 91
// EAX: pRender_buffer
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pCamera_to_world
void DepthEffect(br_pixelmap *pRender_buffer, br_pixelmap *pDepth_buffer, br_actor *pCamera, br_matrix34 *pCamera_to_world);

// Offset: 7224
// Size: 93
// EAX: pRender_buffer
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pCamera_to_world
void DepthEffectSky(br_pixelmap *pRender_buffer, br_pixelmap *pDepth_buffer, br_actor *pCamera, br_matrix34 *pCamera_to_world);

// Offset: 7320
// Size: 627
// EAX: pCamera
void DoWobbleCamera(br_actor *pCamera);

// Offset: 7948
// Size: 627
// EAX: pCamera
void DoDrugWobbleCamera(br_actor *pCamera);

// Offset: 8576
// Size: 116
// EAX: pCamera
// EDX: pCamera_to_world
void DoSpecialCameraEffect(br_actor *pCamera, br_matrix34 *pCamera_to_world);

// Offset: 8692
// Size: 115
void LessDepthFactor();

// Offset: 8808
// Size: 115
void MoreDepthFactor();

// Offset: 8924
// Size: 115
void LessDepthFactor2();

// Offset: 9040
// Size: 115
void MoreDepthFactor2();

// Offset: 9156
// Size: 95
void AssertYons();

// Offset: 9252
// Size: 140
void IncreaseYon();

// Offset: 9392
// Size: 162
void DecreaseYon();

// Offset: 9556
// Size: 127
void SetYon(br_scalar pYon);

// Offset: 9684
// Size: 47
br_scalar GetYon();

// Offset: 9732
// Size: 162
void IncreaseAngle();

// Offset: 9896
// Size: 162
void DecreaseAngle();

// Offset: 10060
// Size: 261
void ToggleDepthMode();

// Offset: 10324
// Size: 45
int GetSkyTextureOn();

// Offset: 10372
// Size: 60
// EAX: pOn
void SetSkyTextureOn(int pOn);

// Offset: 10432
// Size: 124
void ToggleSkyQuietly();

// Offset: 10556
// Size: 188
void ToggleSky();

// Offset: 10744
// Size: 45
int GetDepthCueingOn();

// Offset: 10792
// Size: 71
// EAX: pOn
void SetDepthCueingOn(int pOn);

// Offset: 10864
// Size: 110
void ToggleDepthCueingQuietly();

// Offset: 10976
// Size: 188
void ToggleDepthCueing();

// Offset: 11164
// Size: 74
void ChangeDepthEffect();

// Offset: 11240
// Size: 34
void MungeForwardSky();

// Offset: 11276
// Size: 60
void MungeRearviewSky();

