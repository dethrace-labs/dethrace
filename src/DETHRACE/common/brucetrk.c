#include "brucetrk.h"

#include "brender.h"
#include "globvars.h"
#include "globvrbm.h"
#include "init.h"
#include "world.h"
#include <stdlib.h>

br_actor* gMr_blendy;
int gDefault_blend_pc;

// IDA: void __usercall AllocateActorMatrix(tTrack_spec *pTrack_spec@<EAX>, br_actor ****pDst@<EDX>)
void AllocateActorMatrix(tTrack_spec* pTrack_spec, br_actor**** pDst) {
    tU16 z;
    LOG_TRACE("(%p, %p)", pTrack_spec, pDst);

    *pDst = BrMemAllocate(sizeof(intptr_t) * pTrack_spec->ncolumns_z, kMem_columns_z);
    for (z = 0; z < pTrack_spec->ncolumns_z; z++) {
        (*pDst)[z] = BrMemAllocate(sizeof(intptr_t) * pTrack_spec->ncolumns_x, kMem_columns_x);
        memset((*pDst)[z], 0, sizeof(intptr_t) * pTrack_spec->ncolumns_x);
    }
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
    STUB();
}

// IDA: br_uint_32 __cdecl FindNonCarsCB(br_actor *pActor, tTrack_spec *pTrack_spec)
br_uint_32 FindNonCarsCB(br_actor* pActor, tTrack_spec* pTrack_spec) {
    int i;
    br_scalar r1;
    br_scalar r2;
    br_scalar r3;
    LOG_TRACE("(%p, %p)", pActor, pTrack_spec);

    STUB();
    return 0;
}

// IDA: br_uint_32 __cdecl ProcessModelsCB(br_actor *pActor, tTrack_spec *pTrack_spec)
br_uint_32 ProcessModelsCB(br_actor* pActor, tTrack_spec* pTrack_spec) {
    unsigned int x;
    unsigned int z;
    int group;
    LOG_TRACE("(%p, %p)", pActor, pTrack_spec);

    if (sscanf(pActor->identifier, "%u%u", &x, &z) != 2 || pTrack_spec->ncolumns_x <= x || pTrack_spec->ncolumns_z <= z) {
        if (*pActor->identifier != '%'
            || sscanf((const char*)pActor->identifier + 1, "%u%u", &x, &z) != 2
            || pTrack_spec->ncolumns_x <= x
            || pTrack_spec->ncolumns_z <= z) {
            BrActorEnum(pActor, (br_actor_enum_cbfn*)ProcessModelsCB, pTrack_spec);
        } else {
            pTrack_spec->lollipops[z][x] = pActor;
        }
    } else {
        pActor->material = gDefault_track_material;
        pTrack_spec->columns[z][x] = pActor;
        gMr_blendy = 0;
        if (pActor->model && !gAusterity_mode) {
            StripBlendedFaces(pActor, pActor->model);
        }
        BrActorEnum(pActor, (br_actor_enum_cbfn*)FindNonCarsCB, pTrack_spec);
        if (gMr_blendy) {
            BrActorAdd(pActor, gMr_blendy);
            BrModelAdd(gMr_blendy->model);
            for (group = 0; group < V11MODEL(gMr_blendy->model)->ngroups; group++) {
                V11MODEL(gMr_blendy->model)->groups[group].face_colours[0] = gMr_blendy->model->faces[V11MODEL(gMr_blendy->model)->groups[group].face_user[0]].material->colour;
            }
            gMr_blendy->model->flags &= 0xFF7Fu;
            DodgyModelUpdate(gMr_blendy->model);
            pTrack_spec->blends[z][x] = gMr_blendy;
        }
    }
    return 0;
}

// IDA: void __usercall ProcessModels(tTrack_spec *pTrack_spec@<EAX>)
void ProcessModels(tTrack_spec* pTrack_spec) {
    LOG_TRACE("(%p)", pTrack_spec);

    BrActorEnum(pTrack_spec->the_actor, (br_actor_enum_cbfn*)ProcessModelsCB, pTrack_spec);
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

    unsplit = 0;
    switch (sscanf(pTrack_spec->the_actor->identifier, "%u%u%f%d", &x, &z, &extra_room, &ad)) {
    case 3:
        BrFailure(
            "Attempt to extract columns from invalid track\n"
            "(It might have been produced by an ancient preproc.\n"
            "This is no longer supported.\n");
        break;

    case 4:
        pTrack_spec->ampersand_digits = ad;
        break;

    default:
        unsplit = 1;
        x = 1;
        z = 1;
        extra_room = 0.0;
        pTrack_spec->ampersand_digits = 0;
    }
    pTrack_spec->ncolumns_x = x;
    pTrack_spec->ncolumns_z = z;

    BrActorToBounds(&bounds, pTrack_spec->the_actor);
    pTrack_spec->column_size_x = (bounds.max.v[0] - bounds.min.v[0] + extra_room * 2.0) / (double)pTrack_spec->ncolumns_x;
    pTrack_spec->column_size_z = (bounds.max.v[2] - bounds.min.v[2] + extra_room * 2.0) / (double)pTrack_spec->ncolumns_z;
    pTrack_spec->origin_x = bounds.min.v[0] - extra_room;
    pTrack_spec->origin_z = bounds.min.v[2] - extra_room;
    AllocateActorMatrix(pTrack_spec, &pTrack_spec->columns);
    AllocateActorMatrix(pTrack_spec, &pTrack_spec->lollipops);
    AllocateActorMatrix(pTrack_spec, &pTrack_spec->blends);
    if (pTrack_spec->ampersand_digits <= 0) {
        pTrack_spec->non_car_list = NULL;
    } else {
        pTrack_spec->non_car_list = BrMemAllocate(sizeof(intptr_t) * pTrack_spec->ampersand_digits, kMem_non_car_list);
    }
    if (unsplit) {
        **pTrack_spec->columns = pTrack_spec->the_actor;
    } else {
        ProcessModels(pTrack_spec);
    }
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

    BrZbSceneRenderAdd(pWorld);
}

// IDA: br_scalar __cdecl GetYonFactor()
br_scalar GetYonFactor() {
    return gYon_factor;
}

// IDA: void __cdecl SetYonFactor(br_scalar pNew)
void SetYonFactor(br_scalar pNew) {
    gYon_factor = pNew;
}
