#include "finteray.h"
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

void DRVector2AccumulateScale(br_vector2* a, br_vector2* b, br_scalar s) {
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

int DRSceneRayPick2D(br_actor* world, br_vector3* pPosition, br_vector3* pDir, dr_pick2d_cbfn* callback) {
    NOT_IMPLEMENTED();
}

int FindHighestPolyCallBack(br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3* pPoint, br_vector2* pMap, void* pArg) {
    NOT_IMPLEMENTED();
}

int FindHighestCallBack(br_actor* pActor, br_model* pModel, br_material* pMaterial, br_vector3* pRay_pos, br_vector3* pRay_dir, br_scalar pT_near, br_scalar pT_far, void* pArg) {
    NOT_IMPLEMENTED();
}

void FindFace(br_vector3* pPosition, br_vector3* pDir, br_vector3* nor, br_scalar* t, br_material** material) {
    int group;
    NOT_IMPLEMENTED();
}

void EnablePlingMaterials() {
    NOT_IMPLEMENTED();
}

void DisablePlingMaterials() {
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void GetNewBoundingBox(br_bounds* b2, br_bounds* b1, br_matrix34* m) {
    br_vector3 a;
    br_vector3 c[3];
    int j;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

int FindFacesInBox2(tBounds* bnds, tFace_ref* face_list, int max_face) {
    br_vector3 a;
    br_vector3 b;
    br_vector3 c[3];
    int i;
    int j;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void ClipToPlaneGE(br_vector3* p, int* nv, int i, br_scalar limit) {
    int last_vertex;
    int j;
    int vertex;
    int k;
    br_vector3 p2[12];
    NOT_IMPLEMENTED();
}

void ClipToPlaneLE(br_vector3* p, int* nv, int i, br_scalar limit) {
    int last_vertex;
    int j;
    int vertex;
    int k;
    br_vector3 p2[12];
    NOT_IMPLEMENTED();
}

int BoundsOverlapTest(br_bounds* b1, br_bounds* b2) {
    NOT_IMPLEMENTED();
}

int BoundsTransformTest(br_bounds* b1, br_bounds* b2, br_matrix34* M) {
    br_scalar val;
    br_vector3 o;
    NOT_IMPLEMENTED();
}

int LineBoxColl(br_vector3* o, br_vector3* p, br_bounds* pB, br_vector3* pHit_point) {
    br_vector3 dir;
    int inside;
    int quad[3];
    int i;
    int which_plane;
    br_scalar max_t[3];
    br_scalar cp[3];
    NOT_IMPLEMENTED();
}

int SphereBoxIntersection(br_bounds* pB, br_vector3* pC, br_scalar pR_squared, br_vector3* pHit_point) {
    int i;
    br_scalar d;
    NOT_IMPLEMENTED();
}

int LineBoxCollWithSphere(br_vector3* o, br_vector3* p, br_bounds* pB, br_vector3* pHit_point) {
    int i;
    int plane;
    NOT_IMPLEMENTED();
}

int CompVert(int v1, int v2) {
    br_vertex* vl;
    br_vector3 tv;
    br_vector2 tv2;
    NOT_IMPLEMENTED();
}

void SetFacesGroup(int pFace) {
    int f;
    int v;
    int i;
    NOT_IMPLEMENTED();
}

void SelectFace(br_vector3* pDir) {
    tCar_spec* c;
    br_vector3 dir;
    br_vector3 normal;
    br_scalar t;
    br_model* old_model;
    int i;
    NOT_IMPLEMENTED();
}

void GetTilingLimits(br_vector2* min, br_vector2* max) {
    int f;
    int i;
    int j;
    br_vertex* verts;
    br_face* faces;
    NOT_IMPLEMENTED();
}

void Scale(int pD, int factor) {
    br_vector2 min;
    br_vector2 max;
    int f;
    int v;
    br_scalar d;
    br_vertex* verts;
    br_face* faces;
    NOT_IMPLEMENTED();
}

void ScaleUpX() {
    NOT_IMPLEMENTED();
}

void ScaleDnX() {
    NOT_IMPLEMENTED();
}

void ScaleUpY() {
    NOT_IMPLEMENTED();
}

void ScaleDnY() {
    NOT_IMPLEMENTED();
}

void SelectFaceForward() {
    NOT_IMPLEMENTED();
}

void SelectFaceDown() {
    NOT_IMPLEMENTED();
}
