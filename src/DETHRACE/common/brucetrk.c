#include "brucetrk.h"

#include "CORE/STD/brstdlib.h"
#include "globvrbm.h"
#include <stdlib.h>

br_actor* gMr_blendy;
int gDefault_blend_pc;

// IDA: void __usercall AllocateActorMatrix(tTrack_spec *pTrack_spec@<EAX>, br_actor ****pDst@<EDX>)
void AllocateActorMatrix(tTrack_spec* pTrack_spec, br_actor**** pDst) {
    tU16 z;
    LOG_TRACE("(%p, %p)", pTrack_spec, pDst);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DisposeActorMatrix(tTrack_spec *pTrack_spec@<EAX>, br_actor ****pVictim@<EDX>, int pRemove_act_mod@<EBX>)
void DisposeActorMatrix(tTrack_spec* pTrack_spec, br_actor**** pVictim, int pRemove_act_mod) {
    tU16 z;
    tU16 x;
    LOG_TRACE("(%p, %p, %d)", pTrack_spec, pVictim, pRemove_act_mod);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DisposeColumns(tTrack_spec *pTrack_spec@<EAX>)
void DisposeColumns(tTrack_spec* pTrack_spec) {
    LOG_TRACE("(%p)", pTrack_spec);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall XZToColumnXZ(tU8 *pColumn_x@<EAX>, tU8 *pColumn_z@<EDX>, br_scalar pX, br_scalar pZ, tTrack_spec *pTrack_spec)
void XZToColumnXZ(tU8* pColumn_x, tU8* pColumn_z, br_scalar pX, br_scalar pZ, tTrack_spec* pTrack_spec) {
    br_scalar x;
    br_scalar z;
    LOG_TRACE("(%p, %p, %f, %f, %p)", pColumn_x, pColumn_z, pX, pZ, pTrack_spec);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall StripBlendedFaces(br_actor *pActor@<EAX>, br_model *pModel@<EDX>)
void StripBlendedFaces(br_actor* pActor, br_model* pModel) {
    int i;
    br_face* face;
    int changed_one;
    char s[256];
    static tU16 nfaces_allocated;
    LOG_TRACE("(%p, %p)", pActor, pModel);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl FindNonCarsCB(br_actor *pActor, tTrack_spec *pTrack_spec)
br_uint_32 FindNonCarsCB(br_actor* pActor, tTrack_spec* pTrack_spec) {
    int i;
    br_scalar r1;
    br_scalar r2;
    br_scalar r3;
    LOG_TRACE("(%p, %p)", pActor, pTrack_spec);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl ProcessModelsCB(br_actor *pActor, tTrack_spec *pTrack_spec)
br_uint_32 ProcessModelsCB(br_actor* pActor, tTrack_spec* pTrack_spec) {
    unsigned int x;
    unsigned int z;
    int group;
    LOG_TRACE("(%p, %p)", pActor, pTrack_spec);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ProcessModels(tTrack_spec *pTrack_spec@<EAX>)
void ProcessModels(tTrack_spec* pTrack_spec) {
    LOG_TRACE("(%p)", pTrack_spec);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ExtractColumns(tTrack_spec *pTrack_spec@<EAX>)
void ExtractColumns(tTrack_spec* pTrack_spec) {
    unsigned int x;
    unsigned int z;
    int ad;
    int unsplit;
    float e;
    br_scalar extra_room;
    br_bounds bounds;
    LOG_TRACE("(%p)", pTrack_spec);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall LollipopizeActor4(br_actor *pActor@<EAX>, br_matrix34 *pRef_to_world@<EDX>, br_actor *pCamera@<EBX>)
void LollipopizeActor4(br_actor* pActor, br_matrix34* pRef_to_world, br_actor* pCamera) {
    LOG_TRACE("(%p, %p, %p)", pActor, pRef_to_world, pCamera);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl LollipopizeChildren(br_actor *pActor, void *pArg)
br_uint_32 LollipopizeChildren(br_actor* pActor, void* pArg) {
    tMatrix_and_actor* maa;
    LOG_TRACE("(%p, %p)", pActor, pArg);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DrawColumns(int pDraw_blends@<EAX>, tTrack_spec *pTrack_spec@<EDX>, int pMin_x@<EBX>, int pMax_x@<ECX>, int pMin_z, int pMax_z, br_matrix34 *pCamera_to_world)
void DrawColumns(int pDraw_blends, tTrack_spec* pTrack_spec, int pMin_x, int pMax_x, int pMin_z, int pMax_z, br_matrix34* pCamera_to_world) {
    tU8 column_x;
    tU8 column_z;
    tU8 column_x2;
    tU8 column_z2;
    tMatrix_and_actor maa;
    br_actor* blended_polys;
    LOG_TRACE("(%d, %p, %d, %d, %d, %d, %p)", pDraw_blends, pTrack_spec, pMin_x, pMax_x, pMin_z, pMax_z, pCamera_to_world);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RenderTrack(br_actor *pWorld@<EAX>, tTrack_spec *pTrack_spec@<EDX>, br_actor *pCamera@<EBX>, br_matrix34 *pCamera_to_world@<ECX>, int pRender_blends)
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
    LOG_TRACE("(%p, %p, %p, %p, %d)", pWorld, pTrack_spec, pCamera, pCamera_to_world, pRender_blends);
    NOT_IMPLEMENTED();
}

// IDA: br_scalar __cdecl GetYonFactor()
br_scalar GetYonFactor() {
    return gYon_factor;
}

// IDA: void __cdecl SetYonFactor(br_scalar pNew)
void SetYonFactor(br_scalar pNew) {
    gYon_factor = pNew;
}
