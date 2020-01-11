#ifndef _RAYCAST_H_
#define _RAYCAST_H_

#include "br_types.h"
#include "dr_types.h"

// Offset: 0
// Size: 190
// EAX: a
// EDX: world
// EBX: m
int DRActorToRoot(br_actor* a, br_actor* world, br_matrix34* m);

// Offset: 192
// Size: 220
void InitRayCasting();

// Offset: 412
// Size: 104
int BadDiv(br_scalar a, br_scalar b);

// Offset: 592
// Size: 544
// EAX: b
// EDX: rp
// EBX: rd
int PickBoundsTestRay(br_bounds* b, br_vector3* rp, br_vector3* rd, br_scalar t_near, br_scalar t_far, br_scalar* new_t_near, br_scalar* new_t_far);

// Offset: 1136
// Size: 619
// EAX: ap
// EDX: model
// EBX: material
// ECX: callback
int ActorPick2D(br_actor* ap, br_model* model, br_material* material, dr_pick2d_cbfn* callback, void* arg);

// Offset: 1756
// Size: 342
// EAX: world
// EDX: camera
// EBX: viewport
// ECX: pick_x
int DRScenePick2DXY(br_actor* world, br_actor* camera, br_pixelmap* viewport, int pick_x, int pick_y, dr_pick2d_cbfn* callback, void* arg);

// Offset: 2100
// Size: 266
// EAX: world
// EDX: camera
// EBX: callback
// ECX: arg
int DRScenePick2D(br_actor* world, br_actor* camera, dr_pick2d_cbfn* callback, void* arg);

// Offset: 2368
// Size: 1777
// EAX: model
// EDX: material
// EBX: ray_pos
// ECX: ray_dir
int DRModelPick2D(br_model* model, br_material* material, br_vector3* ray_pos, br_vector3* ray_dir, br_scalar t_near, br_scalar t_far, dr_modelpick2d_cbfn* callback, void* arg);

// Offset: 4404
// Size: 194
// EAX: pPosition
// EDX: gWorld
void FindBestY(br_vector3* pPosition, br_actor* gWorld, br_scalar pStarting_height, br_scalar* pNearest_y_above, br_scalar* pNearest_y_below, br_model** pNearest_above_model, br_model** pNearest_below_model, int* pNearest_above_face_index, int* pNearest_below_face_index);

// Offset: 4600
// Size: 102
int FindYVerticallyBelowPolyCallBack(br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3* pPoint, br_vector2* pMap, void* pArg);

// Offset: 4704
// Size: 112
int FindYVerticallyBelowCallBack(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT_near, br_scalar pT_far, void* pArg);

// Offset: 4816
// Size: 558
// EAX: pPosition
br_scalar FindYVerticallyBelow(br_vector3* pPosition);

// Offset: 5376
// Size: 110
// EAX: pCast_point
br_scalar FindYVerticallyBelow2(br_vector3* pCast_point);

#endif
