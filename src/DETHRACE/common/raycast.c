#include "raycast.h"
#include <stdlib.h>

br_matrix34 gPick_model_to_view;
int gBelow_face_index;
br_scalar gCurrent_y;
int gAbove_face_index;
br_model* gAbove_model;
br_model* gBelow_model;
br_scalar gHighest_y_below;
br_actor* gY_picking_camera;
br_scalar gLowest_y_above;

// Offset: 0
// Size: 190
// EAX: a
// EDX: world
// EBX: m
int DRActorToRoot(br_actor* a, br_actor* world, br_matrix34* m) {
    NOT_IMPLEMENTED();
}

// Offset: 192
// Size: 220
void InitRayCasting() {
    NOT_IMPLEMENTED();
}

// Offset: 412
// Size: 104
int BadDiv(br_scalar a, br_scalar b) {
    NOT_IMPLEMENTED();
}

// Offset: 592
// Size: 544
// EAX: b
// EDX: rp
// EBX: rd
int PickBoundsTestRay(br_bounds* b, br_vector3* rp, br_vector3* rd, br_scalar t_near, br_scalar t_far, br_scalar* new_t_near, br_scalar* new_t_far) {
    int i;
    float s;
    float t;
    NOT_IMPLEMENTED();
}

// Offset: 1136
// Size: 619
// EAX: ap
// EDX: model
// EBX: material
// ECX: callback
int ActorPick2D(br_actor* ap, br_model* model, br_material* material, dr_pick2d_cbfn* callback, void* arg) {
    br_actor* a;
    br_model* this_model;
    br_material* this_material;
    br_matrix34 m_to_v;
    br_matrix34 v_to_m;
    br_scalar t_near;
    br_scalar t_far;
    int r;
    NOT_IMPLEMENTED();
}

// Offset: 1756
// Size: 342
// EAX: world
// EDX: camera
// EBX: viewport
// ECX: pick_x
int DRScenePick2DXY(br_actor* world, br_actor* camera, br_pixelmap* viewport, int pick_x, int pick_y, dr_pick2d_cbfn* callback, void* arg) {
    br_matrix34 camera_tfm;
    br_scalar scale;
    br_scalar cos_angle;
    br_scalar sin_angle;
    br_camera* camera_data;
    br_angle view_over_2;
    NOT_IMPLEMENTED();
}

// Offset: 2100
// Size: 266
// EAX: world
// EDX: camera
// EBX: callback
// ECX: arg
int DRScenePick2D(br_actor* world, br_actor* camera, dr_pick2d_cbfn* callback, void* arg) {
    br_matrix34 camera_tfm;
    br_scalar scale;
    br_camera* camera_data;
    NOT_IMPLEMENTED();
}

// Offset: 2368
// Size: 1777
// EAX: model
// EDX: material
// EBX: ray_pos
// ECX: ray_dir
int DRModelPick2D(br_model* model, br_material* material, br_vector3* ray_pos, br_vector3* ray_dir, br_scalar t_near, br_scalar t_far, dr_modelpick2d_cbfn* callback, void* arg) {
    DR_FACE* fp;
    int f;
    int axis_m;
    int axis_0;
    int axis_1;
    int group;
    br_scalar t;
    br_scalar n;
    br_scalar d;
    br_vector3 p;
    double u0;
    double u1;
    double u2;
    double v0;
    double v1;
    double v2;
    br_scalar v0i1;
    br_scalar v0i2;
    double alpha;
    double beta;
    double f_d;
    double f_n;
    br_scalar s_alpha;
    br_scalar s_beta;
    br_vector2 map;
    int v;
    int e;
    int r;
    br_material* this_material;
    br_scalar numerator;
    double f_numerator;
    NOT_IMPLEMENTED();
}

// Offset: 4404
// Size: 194
// EAX: pPosition
// EDX: gWorld
void FindBestY(br_vector3* pPosition, br_actor* gWorld, br_scalar pStarting_height, br_scalar* pNearest_y_above, br_scalar* pNearest_y_below, br_model** pNearest_above_model, br_model** pNearest_below_model, int* pNearest_above_face_index, int* pNearest_below_face_index) {
    NOT_IMPLEMENTED();
}

// Offset: 4600
// Size: 102
int FindYVerticallyBelowPolyCallBack(br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3* pPoint, br_vector2* pMap, void* pArg) {
    br_scalar the_y;
    NOT_IMPLEMENTED();
}

// Offset: 4704
// Size: 112
int FindYVerticallyBelowCallBack(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT_near, br_scalar pT_far, void* pArg) {
    NOT_IMPLEMENTED();
}

// Offset: 4816
// Size: 558
// EAX: pPosition
br_scalar FindYVerticallyBelow(br_vector3* pPosition) {
    tU8 cx;
    tU8 cz;
    tU8 x;
    tU8 z;
    tTrack_spec* track_spec;
    NOT_IMPLEMENTED();
}

// Offset: 5376
// Size: 110
// EAX: pCast_point
br_scalar FindYVerticallyBelow2(br_vector3* pCast_point) {
    br_scalar result;
    int number_of_attempts;
    br_vector3 cast_point;
    NOT_IMPLEMENTED();
}
