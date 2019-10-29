#include "dr_types.h"
#include "br_types.h"
// Offset: 0
// Size: 119
void DrawDot(br_scalar z, tU8 *scr_ptr, tU16 *depth_ptr, tU8 *shade_ptr);

// Offset: 120
// Size: 234
// EAX: pScreen
void SetWorldToScreen(br_pixelmap *pScreen);

// Offset: 356
// Size: 171
// EAX: pStart
// EDX: pEnd
void DrawLine3DThroughBRender(br_vector3 *pStart, br_vector3 *pEnd);

// Offset: 528
// Size: 490
// EAX: start
// EDX: end
// EBX: pScreen
// ECX: pDepth_buffer
int DrawLine3D(br_vector3 *start, br_vector3 *end, br_pixelmap *pScreen, br_pixelmap *pDepth_buffer, br_pixelmap *shade_table);

// Offset: 1020
// Size: 2221
// EAX: o
// EDX: p
// EBX: pScreen
// ECX: pDepth_buffer
int DrawLine2D(br_vector3 *o, br_vector3 *p, br_pixelmap *pScreen, br_pixelmap *pDepth_buffer, br_scalar brightness, br_pixelmap *shade_table);

// Offset: 3244
// Size: 209
// EAX: pCol
void SetLineModelCols(tU8 pCol);

// Offset: 3456
// Size: 633
// EAX: pRender_screen
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pTime
void ReplaySparks(br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_actor *pCamera, tU32 pTime);

// Offset: 4092
// Size: 2299
// EAX: pRender_screen
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pCamera_to_world
void RenderSparks(br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_actor *pCamera, br_matrix34 *pCamera_to_world, tU32 pTime);

// Offset: 6392
// Size: 309
// EAX: pCar
// EDX: pPos
// EBX: pVel
void CreateSingleSpark(tCar_spec *pCar, br_vector3 *pPos, br_vector3 *pVel);

// Offset: 6704
// Size: 1570
// EAX: pos
// EDX: v
// EBX: pForce
void CreateSparks(br_vector3 *pos, br_vector3 *v, br_vector3 *pForce, br_scalar sparkiness, tCar_spec *pCar);

// Offset: 8276
// Size: 1009
// EAX: pos
// EDX: v
// EBX: pForce
// ECX: pCar1
void CreateSparkShower(br_vector3 *pos, br_vector3 *v, br_vector3 *pForce, tCar_spec *pCar1, tCar_spec *pCar2);

// Offset: 9288
// Size: 305
// EAX: pSpark_num
// EDX: pos
// EBX: length
void AdjustSpark(int pSpark_num, br_vector3 *pos, br_vector3 *length, wchar_t tv, br_memory_classes mat);

// Offset: 9596
// Size: 206
// EAX: pShrapnel_num
// EDX: pos
// EBX: pAge
// ECX: pMaterial
void AdjustShrapnel(int pShrapnel_num, br_vector3 *pos, tU16 pAge, br_material *pMaterial, ldiv_t i, br_memory_classes __unk5__);

// Offset: 9804
// Size: 44
void ResetSparks();

// Offset: 9848
// Size: 114
void ResetShrapnel();

// Offset: 9964
// Size: 854
// EAX: pos
// EDX: v
// EBX: pNormal
void CreateShrapnelShower(br_vector3 *pos, br_vector3 *v, br_vector3 *pNormal, br_scalar pForce, tCar_spec *c1, tCar_spec *c2);

// Offset: 10820
// Size: 57
br_scalar random();

// Offset: 10880
// Size: 523
void InitShrapnel();

// Offset: 11404
// Size: 111
void LoadInShrapnel();

// Offset: 11516
// Size: 73
// EAX: i
void KillShrapnel(int i);

// Offset: 11592
// Size: 179
void DisposeShrapnel();

// Offset: 11772
// Size: 249
// EAX: pTime
void ReplayShrapnel(tU32 pTime);

// Offset: 12024
// Size: 958
// EAX: pTime
void MungeShrapnel(tU32 pTime);

// Offset: 12984
// Size: 295
// EAX: mat
// EDX: r
// EBX: a
void DrMatrix34Rotate(br_matrix34 *mat, br_angle r, br_vector3 *a);

// Offset: 13280
// Size: 245
// EAX: l
// EDX: x
void SmokeLine(int l, int x, br_scalar zbuff, int r_squared, tU8 *scr_ptr, tU16 *depth_ptr, tU8 *shade_ptr, br_scalar r_multiplier, br_scalar z_multiplier, br_scalar shade_offset, void (i)(), br_memory_classes r_multiplier_int, unsigned short z, br_vector3 *__unk14__, br_scalar __unk15__, br_scalar __unk16__);

// Offset: 13528
// Size: 2390
// EAX: o
void SmokeCircle(br_vector3 *o, br_scalar r, br_scalar extra_z, br_scalar strength, br_scalar pAspect, br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_pixelmap *pShade_table);

// Offset: 15920
// Size: 112
int CmpSmokeZ(void *p1, void *p2);

// Offset: 16032
// Size: 506
void RenderRecordedSmokeCircles();

// Offset: 16540
// Size: 266
// EAX: pCent
void RecordSmokeCircle(br_vector3 *pCent, br_scalar pR, br_scalar pStrength, br_pixelmap *pShade, br_scalar pAspect);

// Offset: 16808
// Size: 506
// EAX: o
void SmokeCircle3D(br_vector3 *o, br_scalar r, br_scalar strength, br_scalar pAspect, br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_pixelmap *pShade_table, br_actor *pCam);

// Offset: 17316
// Size: 319
// EAX: pRender_screen
// EDX: pDepth_buffer
// EBX: pCamera
void ReplaySmoke(br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_actor *pCamera);

// Offset: 17636
// Size: 935
// EAX: pCar
// EDX: wheel
// EBX: pTime
void GenerateContinuousSmoke(tCar_spec *pCar, int wheel);

// Offset: 18572
// Size: 91
void DustRotate();

// Offset: 18664
// Size: 1813
// EAX: pRender_screen
// EDX: pDepth_buffer
// EBX: pCamera
// ECX: pCamera_to_world
void RenderSmoke(br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_actor *pCamera, br_matrix34 *pCamera_to_world, tU32 pTime);

// Offset: 20480
// Size: 672
// EAX: pos
// EDX: v
void CreatePuffOfSmoke(br_vector3 *pos, br_vector3 *v, br_scalar strength, br_scalar pDecay_factor, int pType);

// Offset: 21152
// Size: 44
void ResetSmoke();

// Offset: 21196
// Size: 143
// EAX: pIndex
// EDX: pType
// EBX: pPos
void AdjustSmoke(int pIndex, tU8 pType, br_vector3 *pPos, br_scalar pRadius, br_scalar pStrength, int __unk5__, br_memory_classes __unk6__, char __unk8__);

// Offset: 21340
// Size: 34
void ActorError();

// Offset: 21376
// Size: 254
// EAX: pIndex
// EDX: pCar
// EBX: pVertex
// ECX: pColour
void AdjustSmokeColumn(int pIndex, tCar_spec *pCar, int pVertex, int pColour, unsigned int i);

// Offset: 21632
// Size: 880
// EAX: pCar
// EDX: pColour
// EBX: pVertex_index
// ECX: pLifetime
void CreateSmokeColumn(tCar_spec *pCar, int pColour, int pVertex_index);

// Offset: 22512
// Size: 309
void GenerateSmokeShades();

// Offset: 22824
// Size: 96
void GenerateItFoxShadeTable();

// Offset: 22920
// Size: 153
// EAX: pIndex
// EDX: pFrame_count
void AdjustFlame(int pIndex, int pFrame_count, br_scalar pScale_x, br_scalar pScale_y, br_scalar pOffset_x, br_scalar pOffset_z, unsigned short i, br_memory_classes j, union actor);

// Offset: 23076
// Size: 373
// EAX: col
// EDX: actor
void ReplayFlame(tSmoke_column *col, br_actor *actor);

// Offset: 23452
// Size: 935
// EAX: c
// EDX: pPos
// EBX: pTime
void FlameAnimate(int c, br_vector3 *pPos, tU32 pTime, int col, br_memory_classes actor);

// Offset: 24388
// Size: 537
// EAX: i
// EDX: pTime
// EBX: pRet_car_pos
void DoSmokeColumn(int i, tU32 pTime, br_vector3 *pRet_car_pos, signed char c, _complex actor);

// Offset: 24928
// Size: 132
// EAX: pTime
void ReplaySmokeColumn(tU32 pTime);

// Offset: 25060
// Size: 1157
// EAX: pTime
void MungeSmokeColumn(tU32 pTime);

// Offset: 26220
// Size: 304
void DisposeFlame();

// Offset: 26524
// Size: 840
void InitFlame();

// Offset: 27364
// Size: 1197
// EAX: pF
void InitSplash(FILE *pF);

// Offset: 28564
// Size: 257
void DisposeSplash();

// Offset: 28824
// Size: 306
// EAX: pRender_screen
// EDX: pDepth_buffer
// EBX: pCamera
void DrawTheGlow(br_pixelmap *pRender_screen, br_pixelmap *pDepth_buffer, br_actor *pCamera);

// Offset: 29132
// Size: 944
// EAX: pCar
void PipeInstantUnSmudge(tCar_spec *pCar);

// Offset: 30076
// Size: 1828
// EAX: pCar
// EDX: fire_point
void SmudgeCar(tCar_spec *pCar);

// Offset: 31904
// Size: 105
void ResetSmokeColumns();

// Offset: 32012
// Size: 45
// EAX: pSmoke_on
void SetSmokeOn(int pSmoke_on);

// Offset: 32060
// Size: 47
// EAX: pSmoke_on
void ReallySetSmokeOn(int pSmoke_on);

// Offset: 32108
// Size: 53
// EAX: pSmoke_on
void SetSmoke(int pSmoke_on);

// Offset: 32164
// Size: 45
int GetSmokeOn();

// Offset: 32212
// Size: 107
// EAX: pCar
void StopCarSmoking(tCar_spec *pCar);

// Offset: 32320
// Size: 91
// EAX: pCar
void StopCarSmokingInstantly(tCar_spec *pCar);

// Offset: 32412
// Size: 245
// EAX: pCar
// EDX: pDamage_index
// EBX: pColour
void ConditionalSmokeColumn(tCar_spec *pCar, int pDamage_index);

// Offset: 32660
// Size: 1065
// EAX: pCar
// EDX: sp
// EBX: normal
// ECX: pTime
void SingleSplash(tCar_spec *pCar, br_vector3 *sp, br_vector3 *normal, tU32 pTime);

// Offset: 33728
// Size: 3206
// EAX: pCar
// EDX: pTime
void CreateSplash(tCar_spec *pCar, tU32 pTime);

// Offset: 36936
// Size: 977
// EAX: pTime
void MungeSplash(tU32 pTime);

// Offset: 37916
// Size: 143
void RenderSplashes();

// Offset: 38060
// Size: 207
// EAX: f
void GetSmokeShadeTables(FILE *f);

// Offset: 38268
// Size: 117
void FreeSmokeShadeTables();

// Offset: 38388
// Size: 80
// EAX: pF
void LoadInKevStuff(FILE *pF);

// Offset: 38468
// Size: 49
void DisposeKevStuff();

// Offset: 38520
// Size: 259
// EAX: pCar
void DisposeKevStuffCar(tCar_spec *pCar);

// Offset: 38780
// Size: 829
void DoTrueColModelThing(br_actor *actor, br_model *pModel, br_material *material, void *render_data);

// Offset: 39612
// Size: 856
void DoModelThing(br_actor *actor, br_model *pModel, br_material *material, void *render_data);

// Offset: 40468
// Size: 204
// EAX: pActor
// EDX: pShade
void SetModelShade(br_actor *pActor, br_pixelmap *pShade);

// Offset: 40672
// Size: 414
// EAX: pCar
void MakeCarIt(tCar_spec *pCar);

// Offset: 41088
// Size: 1093
// EAX: pCar
void StopCarBeingIt(tCar_spec *pCar);

