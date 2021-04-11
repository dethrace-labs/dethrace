#include "finteray.h"
#include "brender.h"
#include "globvars.h"
#include "raycast.h"
#include <stdlib.h>

br_matrix34 gPick_model_to_view;
int gPling_materials;
int gTemp_group;
br_model* gNearest_model;
int gNfaces;
br_model* gSelected_model;
int gNearest_face_group;
br_material* gSub_material;
int gNearest_face;
br_material* gReal_material;
br_scalar gNearest_T;
tFace_ref* gPling_face;

// IDA: void __usercall DRVector2AccumulateScale(br_vector2 *a@<EAX>, br_vector2 *b@<EDX>, br_scalar s)
void DRVector2AccumulateScale(br_vector2* a, br_vector2* b, br_scalar s) {
    LOG_TRACE("(%p, %p, %f)", a, b, s);

    a->v[0] = b->v[0] * s + a->v[0];
    a->v[1] = b->v[1] * s + a->v[1];
}

// IDA: int __usercall ActorRayPick2D@<EAX>(br_actor *ap@<EAX>, br_vector3 *pPosition@<EDX>, br_vector3 *pDir@<EBX>, br_model *model@<ECX>, br_material *material, dr_pick2d_cbfn *callback)
int ActorRayPick2D(br_actor* ap, br_vector3* pPosition, br_vector3* pDir, br_model* model, br_material* material, dr_pick2d_cbfn* callback) {
    br_actor* a;
    br_model* this_model;
    br_material* this_material;
    br_scalar t_near;
    br_scalar t_far;
    int r;
    br_matrix34 mat;
    br_matrix34 invmat;
    br_vector3 pos;
    br_vector3 dir;
    void* arg;
    LOG_TRACE("(%p, %p, %p, %p, %p, %p)", ap, pPosition, pDir, model, material, callback);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DRSceneRayPick2D@<EAX>(br_actor *world@<EAX>, br_vector3 *pPosition@<EDX>, br_vector3 *pDir@<EBX>, dr_pick2d_cbfn *callback@<ECX>)
int DRSceneRayPick2D(br_actor* world, br_vector3* pPosition, br_vector3* pDir, dr_pick2d_cbfn* callback) {
    LOG_TRACE("(%p, %p, %p, %p)", world, pPosition, pDir, callback);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl FindHighestPolyCallBack(br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3 *pPoint, br_vector2 *pMap, void *pArg)
int FindHighestPolyCallBack(br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3* pPoint, br_vector2* pMap, void* pArg) {
    LOG_TRACE("(%p, %p, %p, %p, %f, %d, %d, %d, %p, %p, %p)", pModel, pMaterial, pRay_pos, pRay_dir, pT, pF, pE, pV, pPoint, pMap, pArg);

    if (pPoint->v[1] > gCurrent_y) {
        if (gLowest_y_above > pPoint->v[1]) {
            gLowest_y_above = pPoint->v[1];
            gAbove_face_index = pF;
            gAbove_model = pModel;
            LOG_DEBUG("mat %s, raypos x %f, y %f, z %f", pMaterial->identifier, pRay_pos->v[0], pRay_pos->v[1], pRay_pos->v[2]);
            LOG_DEBUG("foundhighabove face %d, x %f, y %f, z %f", pF, pPoint->v[0], pPoint->v[1], pPoint->v[2]);
        }
    } else if (pPoint->v[1] > gHighest_y_below) {
        gHighest_y_below = pPoint->v[1];
        gBelow_face_index = pF;
        gBelow_model = pModel;
        //LOG_DEBUG("foundhighbelow '%s', face %d, y %f", pModel->identifier, pF, pPoint->v[1]);
    }
    return 0;
}

// IDA: int __cdecl FindHighestCallBack(br_actor *pActor, br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pT_near, br_scalar pT_far, void *pArg)
int FindHighestCallBack(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT_near, br_scalar pT_far, void* pArg) {
    LOG_TRACE("(%p, %p, %p, %p, %p, %f, %f, %p)", pActor, pModel, pMaterial, pRay_pos, pRay_dir, pT_near, pT_far, pArg);

    if (gProgram_state.current_car.current_car_actor < 0
        || gProgram_state.current_car.car_model_actors[gProgram_state.current_car.current_car_actor].actor != pActor) {
        DRModelPick2D(pModel, pMaterial, pRay_pos, pRay_dir, pT_near, pT_far, FindHighestPolyCallBack, pArg);
    }
    return 0;
}

// IDA: void __usercall FindFace(br_vector3 *pPosition@<EAX>, br_vector3 *pDir@<EDX>, br_vector3 *nor@<EBX>, br_scalar *t@<ECX>, br_material **material)
void FindFace(br_vector3* pPosition, br_vector3* pDir, br_vector3* nor, br_scalar* t, br_material** material) {
    int group;
    LOG_TRACE("(%p, %p, %p, %p, %p)", pPosition, pDir, nor, t, material);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl EnablePlingMaterials()
void EnablePlingMaterials() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DisablePlingMaterials()
void DisablePlingMaterials() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CheckSingleFace(tFace_ref *pFace@<EAX>, br_vector3 *ray_pos@<EDX>, br_vector3 *ray_dir@<EBX>, br_vector3 *normal@<ECX>, br_scalar *rt)
void CheckSingleFace(tFace_ref* pFace, br_vector3* ray_pos, br_vector3* ray_dir, br_vector3* normal, br_scalar* rt) {
    br_scalar t;
    br_scalar numerator;
    br_scalar d;
    br_vector3 p;
    br_vector3 tv;
    int axis_m;
    int axis_0;
    int axis_1;
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
    double f_numerator;
    br_material* this_material;
    LOG_TRACE("(%p, %p, %p, %p, %p)", pFace, ray_pos, ray_dir, normal, rt);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall MultiRayCheckSingleFace(int pNum_rays@<EAX>, tFace_ref *pFace@<EDX>, br_vector3 *ray_pos@<EBX>, br_vector3 *ray_dir@<ECX>, br_vector3 *normal, br_scalar *rt)
void MultiRayCheckSingleFace(int pNum_rays, tFace_ref* pFace, br_vector3* ray_pos, br_vector3* ray_dir, br_vector3* normal, br_scalar* rt) {
    int i;
    br_scalar t[4];
    br_scalar numerator;
    br_scalar d;
    br_vector3 p[4];
    br_vector3 tv;
    int axis_m;
    int axis_0;
    int axis_1;
    double u0[4];
    double u1;
    double u2;
    double v0[4];
    double v1;
    double v2;
    br_scalar v0i1;
    br_scalar v0i2;
    double alpha;
    double beta;
    double f_d;
    double f_n;
    double f_numerator;
    br_material* this_material;
    LOG_TRACE("(%d, %p, %p, %p, %p, %p)", pNum_rays, pFace, ray_pos, ray_dir, normal, rt);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetNewBoundingBox(br_bounds *b2@<EAX>, br_bounds *b1@<EDX>, br_matrix34 *m@<EBX>)
void GetNewBoundingBox(br_bounds* b2, br_bounds* b1, br_matrix34* m) {
    br_vector3 a;
    br_vector3 c[3];
    int j;
    LOG_TRACE("(%p, %p, %p)", b2, b1, m);

    BrMatrix34ApplyP(&b2->min, &b1->min, m);
    b2->max.v[0] = b2->min.v[0];
    b2->max.v[1] = b2->min.v[1];
    b2->max.v[2] = b2->min.v[2];
    a.v[0] = b1->max.v[0] - b1->min.v[0];
    a.v[1] = b1->max.v[1] - b1->min.v[1];
    a.v[2] = b1->max.v[2] - b1->min.v[2];
    for (j = 0; j < 3; j++) {
        c[j].v[0] = m->m[j][0] * a.v[j];
        c[j].v[1] = m->m[j][1] * a.v[j];
        c[j].v[2] = m->m[j][2] * a.v[j];
    }
    for (j = 0; j < 3; ++j) {
        b2->min.v[j] = (double)(c[2].v[j] < 0.0) * c[2].v[j]
            + (double)(c[1].v[j] < 0.0) * c[1].v[j]
            + (double)(c[0].v[j] < 0.0) * c[0].v[j]
            + b2->min.v[j];
        b2->max.v[j] = (double)(c[0].v[j] > 0.0) * c[0].v[j]
            + (double)(c[2].v[j] > 0.0) * c[2].v[j]
            + (double)(c[1].v[j] > 0.0) * c[1].v[j]
            + b2->max.v[j];
    }
}

// IDA: int __usercall FindFacesInBox@<EAX>(tBounds *bnds@<EAX>, tFace_ref *face_list@<EDX>, int max_face@<EBX>)
int FindFacesInBox(tBounds* bnds, tFace_ref* face_list, int max_face) {
    br_vector3 a;
    br_vector3 b;
    br_vector3 c[3];
    int i;
    int j;
    int x;
    int z;
    tU8 cx_min;
    tU8 cx_max;
    tU8 cz_min;
    tU8 cz_max;
    tTrack_spec* track_spec;
    LOG_TRACE("(%p, %p, %d)", bnds, face_list, max_face);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FindFacesInBox2@<EAX>(tBounds *bnds@<EAX>, tFace_ref *face_list@<EDX>, int max_face@<EBX>)
int FindFacesInBox2(tBounds* bnds, tFace_ref* face_list, int max_face) {
    br_vector3 a;
    br_vector3 b;
    br_vector3 c[3];
    int i;
    int j;
    LOG_TRACE("(%p, %p, %d)", bnds, face_list, max_face);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ActorBoxPick@<EAX>(tBounds *bnds@<EAX>, br_actor *ap@<EDX>, br_model *model@<EBX>, br_material *material@<ECX>, tFace_ref *face_list, int max_face, br_matrix34 *pMat)
int ActorBoxPick(tBounds* bnds, br_actor* ap, br_model* model, br_material* material, tFace_ref* face_list, int max_face, br_matrix34* pMat) {
    br_model* this_model;
    br_material* this_material;
    int i;
    int n;
    int test_children;
    br_actor* a;
    br_actor* next_a;
    br_matrix34 mat;
    br_matrix34 mat2;
    br_matrix34 invmat;
    br_matrix34 box_to_actor;
    tBounds new_bounds;
    br_bounds br_bnds;
    LOG_TRACE("(%p, %p, %p, %p, %p, %d, %p)", bnds, ap, model, material, face_list, max_face, pMat);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall ModelPickBox@<EAX>(br_actor *actor@<EAX>, tBounds *bnds@<EDX>, br_model *model@<EBX>, br_material *model_material@<ECX>, tFace_ref *face_list, int max_face, br_matrix34 *pMat)
int ModelPickBox(br_actor* actor, tBounds* bnds, br_model* model, br_material* model_material, tFace_ref* face_list, int max_face, br_matrix34* pMat) {
    int f;
    int i;
    int n;
    int group;
    DR_FACE* fp;
    int v1;
    int v2;
    int v3;
    br_vector3 polygon[12];
    br_vector3 a;
    br_vector3 tv;
    br_scalar t;
    v11model* prepared;
    LOG_TRACE("(%p, %p, %p, %p, %p, %d, %p)", actor, bnds, model, model_material, face_list, max_face, pMat);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ClipToPlaneGE(br_vector3 *p@<EAX>, int *nv@<EDX>, int i@<EBX>, br_scalar limit)
void ClipToPlaneGE(br_vector3* p, int* nv, int i, br_scalar limit) {
    int last_vertex;
    int j;
    int vertex;
    int k;
    br_vector3 p2[12];
    LOG_TRACE("(%p, %p, %d, %f)", p, nv, i, limit);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ClipToPlaneLE(br_vector3 *p@<EAX>, int *nv@<EDX>, int i@<EBX>, br_scalar limit)
void ClipToPlaneLE(br_vector3* p, int* nv, int i, br_scalar limit) {
    int last_vertex;
    int j;
    int vertex;
    int k;
    br_vector3 p2[12];
    LOG_TRACE("(%p, %p, %d, %f)", p, nv, i, limit);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall BoundsOverlapTest@<EAX>(br_bounds *b1@<EAX>, br_bounds *b2@<EDX>)
int BoundsOverlapTest(br_bounds* b1, br_bounds* b2) {
    LOG_TRACE("(%p, %p)", b1, b2);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall BoundsTransformTest@<EAX>(br_bounds *b1@<EAX>, br_bounds *b2@<EDX>, br_matrix34 *M@<EBX>)
int BoundsTransformTest(br_bounds* b1, br_bounds* b2, br_matrix34* M) {
    br_scalar val;
    br_vector3 o;
    LOG_TRACE("(%p, %p, %p)", b1, b2, M);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall LineBoxColl@<EAX>(br_vector3 *o@<EAX>, br_vector3 *p@<EDX>, br_bounds *pB@<EBX>, br_vector3 *pHit_point@<ECX>)
int LineBoxColl(br_vector3* o, br_vector3* p, br_bounds* pB, br_vector3* pHit_point) {
    br_vector3 dir;
    int inside;
    int quad[3];
    int i;
    int which_plane;
    br_scalar max_t[3];
    br_scalar cp[3];
    LOG_TRACE("(%p, %p, %p, %p)", o, p, pB, pHit_point);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SphereBoxIntersection@<EAX>(br_bounds *pB@<EAX>, br_vector3 *pC@<EDX>, br_scalar pR_squared, br_vector3 *pHit_point)
int SphereBoxIntersection(br_bounds* pB, br_vector3* pC, br_scalar pR_squared, br_vector3* pHit_point) {
    int i;
    br_scalar d;
    LOG_TRACE("(%p, %p, %f, %p)", pB, pC, pR_squared, pHit_point);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall LineBoxCollWithSphere@<EAX>(br_vector3 *o@<EAX>, br_vector3 *p@<EDX>, br_bounds *pB@<EBX>, br_vector3 *pHit_point@<ECX>)
int LineBoxCollWithSphere(br_vector3* o, br_vector3* p, br_bounds* pB, br_vector3* pHit_point) {
    int i;
    int plane;
    LOG_TRACE("(%p, %p, %p, %p)", o, p, pB, pHit_point);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall CompVert@<EAX>(int v1@<EAX>, int v2@<EDX>)
int CompVert(int v1, int v2) {
    br_vertex* vl;
    br_vector3 tv;
    br_vector2 tv2;
    LOG_TRACE("(%d, %d)", v1, v2);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetFacesGroup(int pFace@<EAX>)
void SetFacesGroup(int pFace) {
    int f;
    int v;
    int i;
    LOG_TRACE("(%d)", pFace);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SelectFace(br_vector3 *pDir@<EAX>)
void SelectFace(br_vector3* pDir) {
    tCar_spec* c;
    br_vector3 dir;
    br_vector3 normal;
    br_scalar t;
    br_model* old_model;
    int i;
    LOG_TRACE("(%p)", pDir);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetTilingLimits(br_vector2 *min@<EAX>, br_vector2 *max@<EDX>)
void GetTilingLimits(br_vector2* min, br_vector2* max) {
    int f;
    int i;
    int j;
    br_vertex* verts;
    br_face* faces;
    LOG_TRACE("(%p, %p)", min, max);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall Scale(int pD@<EAX>, int factor@<EDX>)
void Scale(int pD, int factor) {
    br_vector2 min;
    br_vector2 max;
    int f;
    int v;
    br_scalar d;
    br_vertex* verts;
    br_face* faces;
    LOG_TRACE("(%d, %d)", pD, factor);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleUpX()
void ScaleUpX() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleDnX()
void ScaleDnX() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleUpY()
void ScaleUpY() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl ScaleDnY()
void ScaleDnY() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SelectFaceForward()
void SelectFaceForward() {
    br_vector3 dir;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SelectFaceDown()
void SelectFaceDown() {
    br_vector3 dir;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
