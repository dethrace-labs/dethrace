#ifndef _BRUCETRK_H_
#define _BRUCETRK_H_

#include "br_types.h"
#include "dr_types.h"

extern br_actor* gMr_blendy;
extern int gDefault_blend_pc;

void AllocateActorMatrix(tTrack_spec* pTrack_spec, br_actor**** pDst);

void DisposeActorMatrix(tTrack_spec* pTrack_spec, br_actor**** pVictim, int pRemove_act_mod);

void DisposeColumns(tTrack_spec* pTrack_spec);

void XZToColumnXZ(tU8* pColumn_x, tU8* pColumn_z, br_scalar pX, br_scalar pZ, tTrack_spec* pTrack_spec);

void StripBlendedFaces(br_actor* pActor, br_model* pModel);

/*br_uint_32*/ intptr_t FindNonCarsCB(br_actor* pActor, tTrack_spec* pTrack_spec);

/*br_uint_32*/ intptr_t ProcessModelsCB(br_actor* pActor, tTrack_spec* pTrack_spec);

void ProcessModels(tTrack_spec* pTrack_spec);

void ExtractColumns(tTrack_spec* pTrack_spec);

void LollipopizeActor4(br_actor* pActor, br_matrix34* pRef_to_world, br_actor* pCamera);

/*br_uint_32*/ intptr_t LollipopizeChildren(br_actor* pActor, void* pArg);

void DrawColumns(int pDraw_blends, tTrack_spec* pTrack_spec, int pMin_x, int pMax_x, int pMin_z, int pMax_z, br_matrix34* pCamera_to_world);

void RenderTrack(br_actor* pWorld, tTrack_spec* pTrack_spec, br_actor* pCamera, br_matrix34* pCamera_to_world, int pRender_blends);

br_scalar GetYonFactor();

void SetYonFactor(br_scalar pNew);

#endif
