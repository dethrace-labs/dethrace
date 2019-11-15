#ifndef _FINTERAY_H_
#define _FINTERAY_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 0
// Size: 104
int BadDiv(br_scalar a, br_scalar b);

// Offset: 104
// Size: 73
// EAX: a
// EDX: b
void DRVector2AccumulateScale(br_vector2 *a, br_vector2 *b, br_scalar s);

// Offset: 180
// Size: 552
// EAX: b
// EDX: rp
// EBX: rd
int PickBoundsTestRay(br_bounds *b, br_vector3 *rp, br_vector3 *rd, br_scalar t_near, br_scalar t_far, br_scalar *new_t_near, br_scalar *new_t_far);

// Offset: 732
// Size: 710
// EAX: ap
// EDX: pPosition
// EBX: pDir
// ECX: model
int ActorRayPick2D(br_actor *ap, br_vector3 *pPosition, br_vector3 *pDir, br_model *model, br_material *material, dr_pick2d_cbfn *callback);

// Offset: 1444
// Size: 97
// EAX: world
// EDX: pPosition
// EBX: pDir
// ECX: callback
int DRSceneRayPick2D(br_actor *world, br_vector3 *pPosition, br_vector3 *pDir, dr_pick2d_cbfn *callback);

// Offset: 1544
// Size: 2059
// EAX: model
// EDX: material
// EBX: ray_pos
// ECX: ray_dir
int DRModelPick2D(br_model *model, br_material *material, br_vector3 *ray_pos, br_vector3 *ray_dir, br_scalar t_near, br_scalar t_far, dr_modelpick2d_cbfn *callback, void *arg);

// Offset: 3604
// Size: 88
int FindHighestPolyCallBack(br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3 *pPoint, br_vector2 *pMap, void *pArg);

// Offset: 3692
// Size: 112
int FindHighestCallBack(br_actor *pActor, br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pT_near, br_scalar pT_far, void *pArg);

// Offset: 3804
// Size: 247
// EAX: pPosition
// EDX: pDir
// EBX: nor
// ECX: t
void FindFace(br_vector3 *pPosition, br_vector3 *pDir, br_vector3 *nor, br_scalar *t, br_material **material);

// Offset: 4052
// Size: 44
void EnablePlingMaterials();

// Offset: 4096
// Size: 44
void DisablePlingMaterials();

// Offset: 4140
// Size: 1148
// EAX: pFace
// EDX: ray_pos
// EBX: ray_dir
// ECX: normal
void CheckSingleFace(tFace_ref *pFace, br_vector3 *ray_pos, br_vector3 *ray_dir, br_vector3 *normal, br_scalar *rt);

// Offset: 5288
// Size: 1519
// EAX: pNum_rays
// EDX: pFace
// EBX: ray_pos
// ECX: ray_dir
void MultiRayCheckSingleFace(int pNum_rays, tFace_ref *pFace, br_vector3 *ray_pos, br_vector3 *ray_dir, br_vector3 *normal, br_scalar *rt);

// Offset: 6808
// Size: 662
// EAX: b2
// EDX: b1
// EBX: m
void GetNewBoundingBox(br_bounds *b2, br_bounds *b1, br_matrix34 *m);

// Offset: 7472
// Size: 1493
// EAX: bnds
// EDX: face_list
// EBX: max_face
int FindFacesInBox(tBounds *bnds, tFace_ref *face_list, int max_face);

// Offset: 8968
// Size: 945
// EAX: bnds
// EDX: face_list
// EBX: max_face
int FindFacesInBox2(tBounds *bnds, tFace_ref *face_list, int max_face);

// Offset: 9916
// Size: 1001
// EAX: bnds
// EDX: ap
// EBX: model
// ECX: material
int ActorBoxPick(tBounds *bnds, br_actor *ap, br_model *model, br_material *material, tFace_ref *face_list, int max_face, br_matrix34 *pMat);

// Offset: 10920
// Size: 2803
// EAX: actor
// EDX: bnds
// EBX: model
// ECX: model_material
int ModelPickBox(br_actor *actor, tBounds *bnds, br_model *model, br_material *model_material, tFace_ref *face_list, int max_face, br_matrix34 *pMat);

// Offset: 13724
// Size: 613
// EAX: p
// EDX: nv
// EBX: i
void ClipToPlaneGE(br_vector3 *p, int *nv, int i, br_scalar limit);

// Offset: 14340
// Size: 613
// EAX: p
// EDX: nv
// EBX: i
void ClipToPlaneLE(br_vector3 *p, int *nv, int i, br_scalar limit);

// Offset: 14956
// Size: 165
// EAX: b1
// EDX: b2
int BoundsOverlapTest(br_bounds *b1, br_bounds *b2);

// Offset: 15124
// Size: 1164
// EAX: b1
// EDX: b2
// EBX: M
int BoundsTransformTest(br_bounds *b1, br_bounds *b2, br_matrix34 *M);

// Offset: 16288
// Size: 757
// EAX: o
// EDX: p
// EBX: pB
// ECX: pHit_point
int LineBoxColl(br_vector3 *o, br_vector3 *p, br_bounds *pB, br_vector3 *pHit_point);

// Offset: 17048
// Size: 298
// EAX: pB
// EDX: pC
int SphereBoxIntersection(br_bounds *pB, br_vector3 *pC, br_scalar pR_squared, br_vector3 *pHit_point);

// Offset: 17348
// Size: 296
// EAX: o
// EDX: p
// EBX: pB
// ECX: pHit_point
int LineBoxCollWithSphere(br_vector3 *o, br_vector3 *p, br_bounds *pB, br_vector3 *pHit_point);

// Offset: 17644
// Size: 278
// EAX: v1
// EDX: v2
int CompVert(int v1, int v2);

// Offset: 17924
// Size: 305
// EAX: pFace
void SetFacesGroup(int pFace);

// Offset: 18232
// Size: 463
// EAX: pDir
void SelectFace(br_vector3 *pDir);

// Offset: 18696
// Size: 448
// EAX: min
// EDX: max
void GetTilingLimits(br_vector2 *min, br_vector2 *max);

// Offset: 19144
// Size: 379
// EAX: pD
// EDX: factor
void Scale(int pD, int factor);

// Offset: 19524
// Size: 46
void ScaleUpX();

// Offset: 19572
// Size: 46
void ScaleDnX();

// Offset: 19620
// Size: 49
void ScaleUpY();

// Offset: 19672
// Size: 49
void ScaleDnY();

// Offset: 19724
// Size: 93
void SelectFaceForward();

// Offset: 19820
// Size: 93
void SelectFaceDown();

#endif
