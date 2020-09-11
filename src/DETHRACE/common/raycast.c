#include "raycast.h"
#include "CORE/V1DB/actsupt.h"
#include <stdlib.h>

br_matrix34 gPick_model_to_view_raycast; //added _raycast suffix to avoid name collision
int gBelow_face_index;
br_scalar gCurrent_y;
int gAbove_face_index;
br_model* gAbove_model;
br_model* gBelow_model;
br_scalar gHighest_y_below;
br_actor* gY_picking_camera;
br_scalar gLowest_y_above;

// IDA: int __usercall DRActorToRoot@<EAX>(br_actor *a@<EAX>, br_actor *world@<EDX>, br_matrix34 *m@<EBX>)
int DRActorToRoot(br_actor* a, br_actor* world, br_matrix34* m) {
    LOG_TRACE("(%p, %p, %p)", a, world, m);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitRayCasting()
void InitRayCasting() {
    br_camera* camera_ptr;
    LOG_TRACE("()");
    br_actor* a;

    a = BrActorAllocate(BR_ACTOR_CAMERA, NULL);
    camera_ptr = a->type_data;
    camera_ptr->type = BR_CAMERA_PERSPECTIVE_FOV;
    camera_ptr->field_of_view = BR_ANGLE_DEG(70.0f);
    camera_ptr->hither_z = 0.001;
    camera_ptr->yon_z = 1000.0;
    camera_ptr->aspect = 1.0;
    a->t.t.quat.q.x = 1.0;
    a->t.t.quat.q.y = 0.0;
    a->t.t.quat.q.z = 0.0;
    a->t.t.quat.q.w = 0.0;
    a->t.t.mat.m[1][1] = 0.0;
    a->t.t.mat.m[1][2] = -1.0;
    a->t.t.mat.m[2][0] = 0.0;
    a->t.t.mat.m[2][1] = 1.0;
    a->t.t.mat.m[2][2] = 0.0;
    gY_picking_camera = a;
}

// IDA: int __cdecl BadDiv(br_scalar a, br_scalar b)
int BadDiv(br_scalar a, br_scalar b) {
    LOG_TRACE("(%f, %f)", a, b);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall PickBoundsTestRay@<EAX>(br_bounds *b@<EAX>, br_vector3 *rp@<EDX>, br_vector3 *rd@<EBX>, br_scalar t_near, br_scalar t_far, br_scalar *new_t_near, br_scalar *new_t_far)
int PickBoundsTestRay(br_bounds* b, br_vector3* rp, br_vector3* rd, br_scalar t_near, br_scalar t_far, br_scalar* new_t_near, br_scalar* new_t_far) {
    int i;
    float s;
    float t;
    LOG_TRACE("(%p, %p, %p, %f, %f, %p, %p)", b, rp, rd, t_near, t_far, new_t_near, new_t_far);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ActorPick2D@<EAX>(br_actor *ap@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, dr_pick2d_cbfn *callback@<ECX>, void *arg)
int ActorPick2D(br_actor* ap, br_model* model, br_material* material, dr_pick2d_cbfn* callback, void* arg) {
    br_actor *a;
    br_model *this_model;
    br_material *this_material;
    br_matrix34 m_to_v;
    br_matrix34 v_to_m;
    br_scalar t_near;
    br_scalar t_far;
    int r;
    LOG_TRACE("(%p, %p, %p, %p, %p)", ap, model, material, callback, arg);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRScenePick2DXY@<EAX>(br_actor *world@<EAX>, br_actor *camera@<EDX>, br_pixelmap *viewport@<EBX>, int pick_x@<ECX>, int pick_y, dr_pick2d_cbfn *callback, void *arg)
int DRScenePick2DXY(br_actor* world, br_actor* camera, br_pixelmap* viewport, int pick_x, int pick_y, dr_pick2d_cbfn* callback, void* arg) {
    br_matrix34 camera_tfm;
    br_scalar scale;
    br_scalar cos_angle;
    br_scalar sin_angle;
    br_camera *camera_data;
    br_angle view_over_2;
    LOG_TRACE("(%p, %p, %p, %d, %d, %p, %p)", world, camera, viewport, pick_x, pick_y, callback, arg);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRScenePick2D@<EAX>(br_actor *world@<EAX>, br_actor *camera@<EDX>, dr_pick2d_cbfn *callback@<EBX>, void *arg@<ECX>)
int DRScenePick2D(br_actor* world, br_actor* camera, dr_pick2d_cbfn* callback, void* arg) {
    br_matrix34 camera_tfm;
    br_scalar scale;
    br_camera *camera_data;
    LOG_TRACE("(%p, %p, %p, %p)", world, camera, callback, arg);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRModelPick2D@<EAX>(br_model *model@<EAX>, br_material *material@<EDX>, br_vector3 *ray_pos@<EBX>, br_vector3 *ray_dir@<ECX>, br_scalar t_near, br_scalar t_far, dr_modelpick2d_cbfn *callback, void *arg)
int DRModelPick2D(br_model* model, br_material* material, br_vector3* ray_pos, br_vector3* ray_dir, br_scalar t_near, br_scalar t_far, dr_modelpick2d_cbfn* callback, void* arg) {
    DR_FACE *fp;
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
    br_material *this_material;
    br_scalar numerator;
    double f_numerator;
    LOG_TRACE("(%p, %p, %p, %p, %f, %f, %p, %p)", model, material, ray_pos, ray_dir, t_near, t_far, callback, arg);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall FindBestY(br_vector3 *pPosition@<EAX>, br_actor *gWorld@<EDX>, br_scalar pStarting_height, br_scalar *pNearest_y_above, br_scalar *pNearest_y_below, br_model **pNearest_above_model, br_model **pNearest_below_model, int *pNearest_above_face_index, int *pNearest_below_face_index)
void FindBestY(br_vector3* pPosition, br_actor* gWorld, br_scalar pStarting_height, br_scalar* pNearest_y_above, br_scalar* pNearest_y_below, br_model** pNearest_above_model, br_model** pNearest_below_model, int* pNearest_above_face_index, int* pNearest_below_face_index) {
    LOG_TRACE("(%p, %p, %f, %p, %p, %p, %p, %p, %p)", pPosition, gWorld, pStarting_height, pNearest_y_above, pNearest_y_below, pNearest_above_model, pNearest_below_model, pNearest_above_face_index, pNearest_below_face_index);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl FindYVerticallyBelowPolyCallBack(br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3 *pPoint, br_vector2 *pMap, void *pArg)
int FindYVerticallyBelowPolyCallBack(br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3* pPoint, br_vector2* pMap, void* pArg) {
    br_scalar the_y;
    LOG_TRACE("(%p, %p, %p, %p, %f, %d, %d, %d, %p, %p, %p)", pModel, pMaterial, pRay_pos, pRay_dir, pT, pF, pE, pV, pPoint, pMap, pArg);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl FindYVerticallyBelowCallBack(br_actor *pActor, br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pT_near, br_scalar pT_far, void *pArg)
int FindYVerticallyBelowCallBack(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT_near, br_scalar pT_far, void* pArg) {
    LOG_TRACE("(%p, %p, %p, %p, %p, %f, %f, %p)", pActor, pModel, pMaterial, pRay_pos, pRay_dir, pT_near, pT_far, pArg);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall FindYVerticallyBelow@<ST0>(br_vector3 *pPosition@<EAX>)
br_scalar FindYVerticallyBelow(br_vector3* pPosition) {
    tU8 cx;
    tU8 cz;
    tU8 x;
    tU8 z;
    tTrack_spec *track_spec;
    LOG_TRACE("(%p)", pPosition);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __usercall FindYVerticallyBelow2@<ST0>(br_vector3 *pCast_point@<EAX>)
br_scalar FindYVerticallyBelow2(br_vector3* pCast_point) {
    br_scalar result;
    int number_of_attempts;
    br_vector3 cast_point;
    LOG_TRACE("(%p)", pCast_point);
    NOT_IMPLEMENTED();
}
