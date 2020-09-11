#include "brucetrk.h"

#include "CORE/STD/brstdlib.h"
#include "globvrbm.h"
#include <stdlib.h>

br_actor* gMr_blendy;
int gDefault_blend_pc;

void AllocateActorMatrix(tTrack_spec* pTrack_spec, br_actor**** pDst) {
    tU16 z;
    char* s = "jeff";
    BrStrLen(s);
    NOT_IMPLEMENTED();
}

void DisposeActorMatrix(tTrack_spec* pTrack_spec, br_actor**** pVictim, int pRemove_act_mod) {
    tU16 z;
    tU16 x;
    NOT_IMPLEMENTED();
}

void DisposeColumns(tTrack_spec* pTrack_spec) {
    NOT_IMPLEMENTED();
}

void XZToColumnXZ(tU8* pColumn_x, tU8* pColumn_z, br_scalar pX, br_scalar pZ, tTrack_spec* pTrack_spec) {
    br_scalar x;
    br_scalar z;
    NOT_IMPLEMENTED();
}

void StripBlendedFaces(br_actor* pActor, br_model* pModel) {
    int i;
    br_face* face;
    int changed_one;
    char s[256];
    static tU16 nfaces_allocated;
    NOT_IMPLEMENTED();
}

br_uint_32 FindNonCarsCB(br_actor* pActor, tTrack_spec* pTrack_spec) {
    int i;
    br_scalar r1;
    br_scalar r2;
    br_scalar r3;
    NOT_IMPLEMENTED();
}

br_uint_32 ProcessModelsCB(br_actor* pActor, tTrack_spec* pTrack_spec) {
    unsigned int x;
    unsigned int z;
    int group;
    NOT_IMPLEMENTED();
}

void ProcessModels(tTrack_spec* pTrack_spec) {
    NOT_IMPLEMENTED();
}

void ExtractColumns(tTrack_spec* pTrack_spec) {
    unsigned int x;
    unsigned int z;
    int ad;
    int unsplit;
    float e;
    br_scalar extra_room;
    br_bounds bounds;
    NOT_IMPLEMENTED();
}

void LollipopizeActor4(br_actor* pActor, br_matrix34* pRef_to_world, br_actor* pCamera) {
    NOT_IMPLEMENTED();
}

br_uint_32 LollipopizeChildren(br_actor* pActor, void* pArg) {
    tMatrix_and_actor* maa;
    NOT_IMPLEMENTED();
}

void DrawColumns(int pDraw_blends, tTrack_spec* pTrack_spec, int pMin_x, int pMax_x, int pMin_z, int pMax_z, br_matrix34* pCamera_to_world) {
    tU8 column_x;
    tU8 column_z;
    tU8 column_x2;
    tU8 column_z2;
    tMatrix_and_actor maa;
    br_actor* blended_polys;
    NOT_IMPLEMENTED();
}

void RenderTrack(br_actor* pWorld, tTrack_spec* pTrack_spec, br_actor* pCamera, br_matrix34* pCamera_to_world, int pRender_blends) {
    static tU8 column_x;
    static tU8 column_z;
    static tU8 min_x;
    static tU8 max_x;
    static tU8 min_z;
    static tU8 max_z;
    static br_vector3 edge_before;
    static br_vector3 edge_after;
    static br_camera* camera;
    static br_scalar tan_fov_ish;
    static br_actor* result;
    NOT_IMPLEMENTED();
}

br_scalar GetYonFactor() {
    return gYon_factor;
}

void SetYonFactor(br_scalar pNew) {
    gYon_factor = pNew;
}
