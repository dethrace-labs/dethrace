#include "brucetrk.h"

// Global variables
br_actor *gMr_blendy;
int gDefault_blend_pc;

// Offset: 0
// Size: 201
// EAX: pTrack_spec
// EDX: pDst
void AllocateActorMatrix(tTrack_spec *pTrack_spec, br_actor ****pDst) {
  tU16 z;
}

// Offset: 204
// Size: 370
// EAX: pTrack_spec
// EDX: pVictim
// EBX: pRemove_act_mod
void DisposeActorMatrix(tTrack_spec *pTrack_spec, br_actor ****pVictim) {
  int pRemove_act_mod;
  tU16 z;
  tU16 x;
}

// Offset: 576
// Size: 131
// EAX: pTrack_spec
void DisposeColumns(tTrack_spec *pTrack_spec) {
}

// Offset: 708
// Size: 236
// EAX: pColumn_x
// EDX: pColumn_z
void XZToColumnXZ(tU8 *pColumn_x, tU8 *pColumn_z, br_scalar pX, br_scalar pZ, tTrack_spec *pTrack_spec) {
  br_scalar x;
  br_scalar z;
}

// Offset: 944
// Size: 758
// EAX: pActor
// EDX: pModel
void StripBlendedFaces(br_actor *pActor, br_model *pModel) {
  int i;
  br_face *face;
  int changed_one;
  char s[256];
  tU16 nfaces_allocated;
}

// Offset: 1704
// Size: 745
br_uint_32 FindNonCarsCB(br_actor *pActor, tTrack_spec *pTrack_spec) {
  int i;
  br_scalar r1;
  br_scalar r2;
  br_scalar r3;
}

// Offset: 2452
// Size: 582
br_uint_32 ProcessModelsCB(br_actor *pActor, tTrack_spec *pTrack_spec) {
  unsigned int x;
  unsigned int z;
  int group;
}

// Offset: 3036
// Size: 61
// EAX: pTrack_spec
void ProcessModels(tTrack_spec *pTrack_spec) {
}

// Offset: 3100
// Size: 440
// EAX: pTrack_spec
void ExtractColumns(tTrack_spec *pTrack_spec) {
  unsigned int x;
  unsigned int z;
  int ad;
  int unsplit;
  float e;
  br_scalar extra_room;
  br_bounds bounds;
}

// Offset: 3540
// Size: 201
// EAX: pActor
// EDX: pRef_to_world
// EBX: pCamera
void LollipopizeActor4(br_actor *pActor, br_matrix34 *pRef_to_world, br_actor *pCamera) {
}

// Offset: 3744
// Size: 65
br_uint_32 LollipopizeChildren(br_actor *pActor) {
  void *pArg;
  tMatrix_and_actor *maa;
}

// Offset: 3812
// Size: 1062
// EAX: pDraw_blends
// EDX: pTrack_spec
// EBX: pMin_x
// ECX: pMax_x
void DrawColumns(int pDraw_blends, tTrack_spec *pTrack_spec, int pMin_x, int pMax_x, int pMin_z, int pMax_z, br_matrix34 *pCamera_to_world, va_list column_x, br_memory_classes column_z) {
  tU8 column_x2;
  tU8 column_z2;
  tMatrix_and_actor maa;
  br_actor *blended_polys;
}

// Offset: 4876
// Size: 1321
// EAX: pWorld
// EDX: pTrack_spec
// EBX: pCamera
// ECX: pCamera_to_world
void RenderTrack(br_actor *pWorld, tTrack_spec *pTrack_spec, br_actor *pCamera, br_matrix34 *pCamera_to_world) {
  int pRender_blends;
  tU8 column_x;
  tU8 column_z;
  tU8 min_x;
  tU8 max_x;
  tU8 min_z;
  tU8 max_z;
  br_vector3 edge_before;
  br_vector3 edge_after;
  br_camera *camera;
  br_scalar tan_fov_ish;
  br_actor *result;
}

// Offset: 6200
// Size: 47
br_scalar GetYonFactor() {
}

// Offset: 6248
// Size: 44
void SetYonFactor(br_scalar pNew) {
}

