#ifndef _SPARK_H_
#define _SPARK_H_

#include "br_types.h"
#include "dr_types.h"

extern int gNext_spark;
extern int gSpark_flags;
extern int gNext_shrapnel;
extern int gShrapnel_flags;
extern br_model *gShrapnel_model[2];
extern int gSmoke_flags;
extern int gSmoke_num;
extern int gOffset;
extern int gColumn_flags;
extern int gNext_column;
extern br_pixelmap *gBlack_smoke_shade_table;
extern br_pixelmap *gDark_smoke_shade_table;
extern br_pixelmap *gGrey_smoke_shade_table;
extern int gSmoke_on;
extern int gNum_splash_types;
extern int gIt_type;
extern br_pixelmap *gIt_shade_table;
extern br_pixelmap **gDust_table;
extern br_pixelmap *gFlame_map[20];
extern tBRender_smoke *gBR_smoke_pointers[30];
extern tSplash gSplash[32];
extern br_material *gSplash_material[20];
extern tBRender_smoke gBR_smoke_structs[30];
extern tSmoke_column gSmoke_column[25];
extern br_matrix4 gCameraToScreen;
extern tSpark gSparks[32];
extern br_pixelmap *gShade_list[16];
extern int gN_BR_smoke_structs;
extern tSmoke gSmoke[25];
extern tU32 gSplash_flags;
extern tU32 gNext_splash;
extern br_model *gLollipop_model;
extern int gNum_dust_tables;
extern br_model *gSplash_model;
extern int gDust_rotate;
extern br_camera *gSpark_cam;
extern br_material *gBlack_material;
extern tShrapnel gShrapnel[15];

void DrawDot(br_scalar z, tU8* scr_ptr, tU16* depth_ptr, tU8* shade_ptr);

void SetWorldToScreen(br_pixelmap* pScreen);

void DrawLine3DThroughBRender(br_vector3* pStart, br_vector3* pEnd);

int DrawLine3D(br_vector3* start, br_vector3* end, br_pixelmap* pScreen, br_pixelmap* pDepth_buffer, br_pixelmap* shade_table);

int DrawLine2D(br_vector3* o, br_vector3* p, br_pixelmap* pScreen, br_pixelmap* pDepth_buffer, br_scalar brightness, br_pixelmap* shade_table);

void SetLineModelCols(tU8 pCol);

void ReplaySparks(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera, tU32 pTime);

void RenderSparks(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world, tU32 pTime);

void CreateSingleSpark(tCar_spec* pCar, br_vector3* pPos, br_vector3* pVel);

void CreateSparks(br_vector3* pos, br_vector3* v, br_vector3* pForce, br_scalar sparkiness, tCar_spec* pCar);

void CreateSparkShower(br_vector3* pos, br_vector3* v, br_vector3* pForce, tCar_spec* pCar1, tCar_spec* pCar2);

void AdjustSpark(int pSpark_num, br_vector3* pos, br_vector3* length);

void AdjustShrapnel(int pShrapnel_num, br_vector3* pos, tU16 pAge, br_material* pMaterial);

void ResetSparks();

void ResetShrapnel();

void CreateShrapnelShower(br_vector3* pos, br_vector3* v, br_vector3* pNormal, br_scalar pForce, tCar_spec* c1, tCar_spec* c2);

void InitShrapnel();

void LoadInShrapnel();

void KillShrapnel(int i);

void DisposeShrapnel();

void ReplayShrapnel(tU32 pTime);

void MungeShrapnel(tU32 pTime);

void DrMatrix34Rotate(br_matrix34* mat, br_angle r, br_vector3* a);

void SmokeLine(int l, int x, br_scalar zbuff, int r_squared, tU8* scr_ptr, tU16* depth_ptr, tU8* shade_ptr, br_scalar r_multiplier, br_scalar z_multiplier, br_scalar shade_offset);

void SmokeCircle(br_vector3* o, br_scalar r, br_scalar extra_z, br_scalar strength, br_scalar pAspect, br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_pixelmap* pShade_table);

int CmpSmokeZ(void* p1, void* p2);

void RenderRecordedSmokeCircles();

void RecordSmokeCircle(br_vector3* pCent, br_scalar pR, br_scalar pStrength, br_pixelmap* pShade, br_scalar pAspect);

void SmokeCircle3D(br_vector3* o, br_scalar r, br_scalar strength, br_scalar pAspect, br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_pixelmap* pShade_table, br_actor* pCam);

void ReplaySmoke(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera);

void GenerateContinuousSmoke(tCar_spec* pCar, int wheel, tU32 pTime);

void DustRotate();

void RenderSmoke(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera, br_matrix34* pCamera_to_world, tU32 pTime);

void CreatePuffOfSmoke(br_vector3* pos, br_vector3* v, br_scalar strength, br_scalar pDecay_factor, int pType, tCar_spec* pC);

void ResetSmoke();

void AdjustSmoke(int pIndex, tU8 pType, br_vector3* pPos, br_scalar pRadius, br_scalar pStrength);

void ActorError();

void AdjustSmokeColumn(int pIndex, tCar_spec* pCar, int pVertex, int pColour);

void CreateSmokeColumn(tCar_spec* pCar, int pColour, int pVertex_index, tU32 pLifetime);

void GenerateSmokeShades();

void GenerateItFoxShadeTable();

void AdjustFlame(int pIndex, int pFrame_count, br_scalar pScale_x, br_scalar pScale_y, br_scalar pOffset_x, br_scalar pOffset_z);

void ReplayFlame(tSmoke_column* col, br_actor* actor);

void FlameAnimate(int c, br_vector3* pPos, tU32 pTime);

void DoSmokeColumn(int i, tU32 pTime, br_vector3* pRet_car_pos);

void ReplaySmokeColumn(tU32 pTime);

void MungeSmokeColumn(tU32 pTime);

void DisposeFlame();

void InitFlame();

void InitSplash(FILE* pF);

void DisposeSplash();

void DrawTheGlow(br_pixelmap* pRender_screen, br_pixelmap* pDepth_buffer, br_actor* pCamera);

void PipeInstantUnSmudge(tCar_spec* pCar);

void SmudgeCar(tCar_spec* pCar, int fire_point);

void ResetSmokeColumns();

void SetSmokeOn(int pSmoke_on);

void ReallySetSmokeOn(int pSmoke_on);

void SetSmoke(int pSmoke_on);

int GetSmokeOn();

void StopCarSmoking(tCar_spec* pCar);

void StopCarSmokingInstantly(tCar_spec* pCar);

void ConditionalSmokeColumn(tCar_spec* pCar, int pDamage_index, int pColour);

void SingleSplash(tCar_spec* pCar, br_vector3* sp, br_vector3* normal, tU32 pTime);

void CreateSplash(tCar_spec* pCar, tU32 pTime);

void MungeSplash(tU32 pTime);

void RenderSplashes();

void GetSmokeShadeTables(FILE* f);

void FreeSmokeShadeTables();

void LoadInKevStuff(FILE* pF);

void DisposeKevStuff();

void DisposeKevStuffCar(tCar_spec* pCar);

void DoTrueColModelThing(br_actor* actor, br_model* pModel, br_material* material, void* render_data, br_uint_8 style, int on_screen);

void DoModelThing(br_actor* actor, br_model* pModel, br_material* material, void* render_data, br_uint_8 style, int on_screen);

void SetModelShade(br_actor* pActor, br_pixelmap* pShade);

void MakeCarIt(tCar_spec* pCar);

void StopCarBeingIt(tCar_spec* pCar);

#endif
