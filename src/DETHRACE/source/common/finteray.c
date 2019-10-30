#include "finteray.h"

// Global variables
br_matrix34 gPick_model_to_view;
int gPling_materials;
int gTemp_group;
br_model *gNearest_model;
int gNfaces;
br_model *gSelected_model;
int gNearest_face_group;
br_material *gSub_material;
int gNearest_face;
br_material *gReal_material;
br_scalar gNearest_T;
tFace_ref *gPling_face;

// Offset: 0
// Size: 104
int BadDiv(br_scalar a, br_scalar b) {
}

// Offset: 104
// Size: 73
// EAX: a
// EDX: b
void DRVector2AccumulateScale(br_vector2 *a, br_vector2 *b, br_scalar s) {
}

// Offset: 180
// Size: 552
// EAX: b
// EDX: rp
// EBX: rd
int PickBoundsTestRay(br_bounds *b, br_vector3 *rp, br_vector3 *rd, br_scalar t_near, br_scalar t_far, br_scalar *new_t_near, br_scalar *new_t_far) {
  int i;
  float s;
  float t;
}

// Offset: 732
// Size: 710
// EAX: ap
// EDX: pPosition
// EBX: pDir
// ECX: model
int ActorRayPick2D(br_actor *ap, br_vector3 *pPosition, br_vector3 *pDir, br_model *model, br_material *material, dr_pick2d_cbfn *callback) {
  br_actor *a;
  br_model *this_model;
  br_material *this_material;
  br_scalar t_near;
  br_scalar t_far;
  int r;
  br_matrix34 mat;
  br_matrix34 invmat;
  br_vector3 pos;
  br_vector3 dir;
  void *arg;
}

// Offset: 1444
// Size: 97
// EAX: world
// EDX: pPosition
// EBX: pDir
// ECX: callback
int DRSceneRayPick2D(br_actor *world, br_vector3 *pPosition, br_vector3 *pDir, dr_pick2d_cbfn *callback) {
}

// Offset: 1544
// Size: 2059
// EAX: model
// EDX: material
// EBX: ray_pos
// ECX: ray_dir
int DRModelPick2D(br_model *model, br_material *material, br_vector3 *ray_pos, br_vector3 *ray_dir, br_scalar t_near, br_scalar t_far, dr_modelpick2d_cbfn *callback) {
  void *arg;
  DR_FACE *fp;
  int f;
  int axis_m;
  int axis_0;
  int axis_1;
  br_scalar t;
  br_scalar n;
  br_scalar d;
  br_vector3 p;
  float u0;
  float u1;
  float u2;
  float v0;
  float v1;
  float v2;
  br_scalar v0i1;
  br_scalar v0i2;
  float alpha;
  float beta;
  float f_d;
  float f_n;
  br_scalar s_alpha;
  br_scalar s_beta;
  br_vector2 map;
  int v;
  int e;
  int r;
  br_material *this_material;
  br_scalar numerator;
  float f_numerator;
  int group;
}

// Offset: 3604
// Size: 88
int FindHighestPolyCallBack(br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pT, int pF, int pE, int pV, br_vector3 *pPoint) {
  br_vector2 *pMap;
  void *pArg;
}

// Offset: 3692
// Size: 112
int FindHighestCallBack(br_actor *pActor, br_model *pModel, br_material *pMaterial, br_vector3 *pRay_pos, br_vector3 *pRay_dir, br_scalar pT_near, br_scalar pT_far) {
  void *pArg;
}

// Offset: 3804
// Size: 247
// EAX: pPosition
// EDX: pDir
// EBX: nor
// ECX: t
void FindFace(br_vector3 *pPosition, br_vector3 *pDir, br_vector3 *nor, br_scalar *t, br_material **material) {
  int group;
}

// Offset: 4052
// Size: 44
void EnablePlingMaterials() {
}

// Offset: 4096
// Size: 44
void DisablePlingMaterials() {
}

// Offset: 4140
// Size: 1148
// EAX: pFace
// EDX: ray_pos
// EBX: ray_dir
// ECX: normal
void CheckSingleFace(tFace_ref *pFace, br_vector3 *ray_pos, br_vector3 *ray_dir, br_vector3 *normal, br_scalar *rt) {
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
  br_material *this_material;
}

// Offset: 5288
// Size: 1519
// EAX: pNum_rays
// EDX: pFace
// EBX: ray_pos
// ECX: ray_dir
void MultiRayCheckSingleFace(int pNum_rays, tFace_ref *pFace, br_vector3 *ray_pos, br_vector3 *ray_dir, br_vector3 *normal, br_scalar *rt, signed char i, double t, enum numerator, br_scalar d, signed char p) {
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
  br_material *this_material;
}

// Offset: 6808
// Size: 662
// EAX: b2
// EDX: b1
// EBX: m
void GetNewBoundingBox(br_bounds *b2, br_bounds *b1, br_matrix34 *m) {
  br_vector3 a;
  br_vector3 c[3];
  int j;
}

// Offset: 7472
// Size: 1493
// EAX: bnds
// EDX: face_list
// EBX: max_face
int FindFacesInBox(tBounds *bnds, tFace_ref *face_list) {
  int max_face;
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
  tTrack_spec *track_spec;
}

// Offset: 8968
// Size: 945
// EAX: bnds
// EDX: face_list
// EBX: max_face
int FindFacesInBox2(tBounds *bnds, tFace_ref *face_list) {
  int max_face;
  br_vector3 a;
  br_vector3 b;
  br_vector3 c[3];
  int i;
  int j;
}

// Offset: 9916
// Size: 1001
// EAX: bnds
// EDX: ap
// EBX: model
// ECX: material
int ActorBoxPick(tBounds *bnds, br_actor *ap, br_model *model, br_material *material, tFace_ref *face_list, int max_face) {
  br_matrix34 *pMat;
  br_model *this_model;
  br_material *this_material;
  int i;
  int n;
  int test_children;
  br_actor *a;
  br_actor *next_a;
  br_matrix34 mat;
  br_matrix34 mat2;
  br_matrix34 invmat;
  br_matrix34 box_to_actor;
  tBounds new_bounds;
  br_bounds br_bnds;
}

// Offset: 10920
// Size: 2803
// EAX: actor
// EDX: bnds
// EBX: model
// ECX: model_material
int ModelPickBox(br_actor *actor, tBounds *bnds, br_model *model, br_material *model_material, tFace_ref *face_list, int max_face) {
  br_matrix34 *pMat;
  int f;
  int i;
  int n;
  int group;
  DR_FACE *fp;
  int v1;
  int v2;
  int v3;
  br_vector3 polygon[12];
  br_vector3 a;
  br_vector3 tv;
  br_scalar t;
  v11model *prepared;
}

// Offset: 13724
// Size: 613
// EAX: p
// EDX: nv
// EBX: i
void ClipToPlaneGE(br_vector3 *p, int *nv, int i) {
  br_scalar limit;
  int last_vertex;
  int j;
  int vertex;
  int k;
  br_vector3 p2[12];
}

// Offset: 14340
// Size: 613
// EAX: p
// EDX: nv
// EBX: i
void ClipToPlaneLE(br_vector3 *p, int *nv, int i) {
  br_scalar limit;
  int last_vertex;
  int j;
  int vertex;
  int k;
  br_vector3 p2[12];
}

// Offset: 14956
// Size: 165
// EAX: b1
// EDX: b2
int BoundsOverlapTest(br_bounds *b1, br_bounds *b2) {
}

// Offset: 15124
// Size: 1164
// EAX: b1
// EDX: b2
// EBX: M
int BoundsTransformTest(br_bounds *b1, br_bounds *b2, br_matrix34 *M) {
  br_scalar val;
  br_vector3 o;
}

// Offset: 16288
// Size: 757
// EAX: o
// EDX: p
// EBX: pB
// ECX: pHit_point
int LineBoxColl(br_vector3 *o, br_vector3 *p, br_bounds *pB, br_vector3 *pHit_point) {
  br_vector3 dir;
  int inside;
  int quad[3];
  int i;
  int which_plane;
  br_scalar max_t[3];
  br_scalar cp[3];
}

// Offset: 17048
// Size: 298
// EAX: pB
// EDX: pC
int SphereBoxIntersection(br_bounds *pB, br_vector3 *pC, br_scalar pR_squared, br_vector3 *pHit_point) {
  int i;
  br_scalar d;
}

// Offset: 17348
// Size: 296
// EAX: o
// EDX: p
// EBX: pB
// ECX: pHit_point
int LineBoxCollWithSphere(br_vector3 *o, br_vector3 *p, br_bounds *pB, br_vector3 *pHit_point) {
  int i;
  int plane;
}

// Offset: 17644
// Size: 278
// EAX: v1
// EDX: v2
int CompVert(int v1, int v2) {
  br_vertex *vl;
  br_vector3 tv;
  br_vector2 tv2;
}

// Offset: 17924
// Size: 305
// EAX: pFace
void SetFacesGroup(int pFace) {
  int f;
  int v;
  int i;
}

// Offset: 18232
// Size: 463
// EAX: pDir
void SelectFace(br_vector3 *pDir) {
  tCar_spec *c;
  br_vector3 dir;
  br_vector3 normal;
  br_scalar t;
  br_model *old_model;
  int i;
}

// Offset: 18696
// Size: 448
// EAX: min
// EDX: max
void GetTilingLimits(br_vector2 *min, br_vector2 *max) {
  int f;
  int i;
  int j;
  br_vertex *verts;
  br_face *faces;
}

// Offset: 19144
// Size: 379
// EAX: pD
// EDX: factor
void Scale(int pD, int factor) {
  br_vector2 min;
  br_vector2 max;
  int f;
  int v;
  br_scalar d;
  br_vertex *verts;
  br_face *faces;
}

// Offset: 19524
// Size: 46
void ScaleUpX() {
}

// Offset: 19572
// Size: 46
void ScaleDnX() {
}

// Offset: 19620
// Size: 49
void ScaleUpY() {
}

// Offset: 19672
// Size: 49
void ScaleDnY() {
}

// Offset: 19724
// Size: 93
void SelectFaceForward() {
}

// Offset: 19820
// Size: 93
void SelectFaceDown() {
}

