#ifndef _RAYCAST_H_
#define _RAYCAST_H_

#include "brender/br_types.h"
#include "dr_types.h"

extern br_matrix34 gPick_model_to_view__raycast; // suffix added to avoid duplicate symbol
extern int gBelow_face_index;
extern br_scalar gCurrent_y;
extern int gAbove_face_index;
extern br_model* gAbove_model;
extern br_model* gBelow_model;
extern br_scalar gHighest_y_below;
extern br_actor* gY_picking_camera;
extern br_scalar gLowest_y_above;

int DRActorToRoot(br_actor* a, br_actor* world, br_matrix34* m);

void InitRayCasting(void);

// Suffix added to avoid duplicate symbol
int BadDiv__raycast(br_scalar a, br_scalar b);

// Suffix added to avoid duplicate symbol
void DRVector2AccumulateScale__raycast(br_vector2* a, br_vector2* b, br_scalar s);

// Suffix added to avoid duplicate symbol
int PickBoundsTestRay__raycast(br_bounds* b, br_vector3* rp, br_vector3* rd, br_scalar t_near, br_scalar t_far, br_scalar* new_t_near, br_scalar* new_t_far);

int ActorPick2D(br_actor* ap, br_model* model, br_material* material, dr_pick2d_cbfn* callback, void* arg);

int DRScenePick2DXY(br_actor* world, br_actor* camera, br_pixelmap* viewport, int pick_x, int pick_y, dr_pick2d_cbfn* callback, void* arg);

int DRScenePick2D(br_actor* world, br_actor* camera, dr_pick2d_cbfn* callback, void* arg);

int DRModelPick2D__raycast(br_model* model, br_material* material, br_vector3* ray_pos, br_vector3* ray_dir, br_scalar t_near, br_scalar t_far, dr_modelpick2d_cbfn* callback, void* arg);

// Suffix added to avoid duplicate symbol
int FindHighestPolyCallBack__raycast(br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3* pPoint, br_vector2* pMap, void* pArg);

// Suffix added to avoid duplicate symbol
int FindHighestCallBack__raycast(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT_near, br_scalar pT_far, void* pArg);

void FindBestY(br_vector3* pPosition, br_actor* gWorld, br_scalar pStarting_height, br_scalar* pNearest_y_above, br_scalar* pNearest_y_below, br_model** pNearest_above_model, br_model** pNearest_below_model, int* pNearest_above_face_index, int* pNearest_below_face_index);

int FindYVerticallyBelowPolyCallBack(br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3* pPoint, br_vector2* pMap, void* pArg);

int FindYVerticallyBelowCallBack(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT_near, br_scalar pT_far, void* pArg);

br_scalar FindYVerticallyBelow(br_vector3* pPosition);

br_scalar FindYVerticallyBelow2(br_vector3* pCast_point);

#endif
