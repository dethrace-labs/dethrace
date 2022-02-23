#include "modsupt.h"
#include "CORE/FW/resource.h"
#include "CORE/MATH/matrix34.h"
#include "CORE/V1DB/dbsetup.h"
#include "harness/trace.h"
#include <math.h>

// FIXME: move this definition (+BR_MAC3+BR_MAC4) to br_defs.h (and remove from matrix34.c matrix4.c ...)
#define BR_MAC2(A, B, C, D) ((A) * (B) + (C) * (D))

// IDA: void __cdecl BrModelApplyMap(br_model *model, int map_type, br_matrix34 *xform)
void BrModelApplyMap(br_model* model, int map_type, br_matrix34* xform) {
    int v;
    br_vertex* vp;
    br_vector3 mv;
    br_matrix34 default_xform;
    br_scalar d;
    LOG_TRACE("(%p, %d, %p)", model, map_type, xform);

    if (xform == NULL) {
        BrMatrix34Identity(&default_xform);
        xform = &default_xform;
    }
    for (v = 0; v < model->nvertices; v++, vp++) {
        vp = &model->vertices[v];
        BrMatrix34ApplyP(&mv, &vp->p, xform);
        switch (map_type) {
        case BR_APPLYMAP_PLANE:
            vp->map.v[0] = (mv.v[0] + 1.f) * 0.5f;
            vp->map.v[1] = (mv.v[1] + 1.f) * 0.5f;
            break;
        case BR_APPLYMAP_SPHERE:
            vp->map.v[0] = BrAngleToDegrees(BR_ATAN2(-mv.v[2], mv.v[0])) / 360.f;
            d = BR_MAC2(mv.v[0], mv.v[0], mv.v[2], mv.v[2]);
            vp->map.v[1] = 1.f - BrAngleToDegrees(BR_ATAN2(d, mv.v[1])) / 180.f;
            break;
        case BR_APPLYMAP_CYLINDER:
            vp->map.v[0] = BrAngleToDegrees(BR_ATAN2(-mv.v[2], mv.v[0])) / 360.f;
            vp->map.v[1] = (mv.v[1] + 1.f) * 0.5f;
            break;
        case BR_APPLYMAP_DISC:
            vp->map.v[0] = BrAngleToDegrees(BR_ATAN2(-mv.v[1], mv.v[0])) / 360.f;
            vp->map.v[1] = sqrtf(BR_MAC2(mv.v[0], mv.v[0], mv.v[1], mv.v[1]));
            break;
        case BR_APPLYMAP_NONE:
            vp->map.v[0] = 0.f;
            vp->map.v[1] = 0.f;
            break;
        }
    }
}

// IDA: br_matrix34* __cdecl BrModelFitMap(br_model *model, int axis_0, int axis_1, br_matrix34 *transform)
br_matrix34* BrModelFitMap(br_model* model, int axis_0, int axis_1, br_matrix34* transform) {
    br_vector3 axis_3;
    br_vector3 tr;
    br_vector3 sc;
    int i;
    static br_vector3 axis_vectors[6] = {
        { {  1.f,  0.f,  0.f } },
        { {  0.f,  1.f,  0.f } },
        { {  0.f,  0.f,  1.f } },
        { { -1.f,  0.f,  0.f } },
        { {  0.f, -1.f,  0.f } },
        { {  0.f,  0.f, -1.f } },
    };
    LOG_TRACE("(%p, %d, %d, %p)", model, axis_0, axis_1, transform);

    BrMatrix34Identity(transform);
    axis_3.v[0] = axis_vectors[axis_1].v[2] * axis_vectors[axis_0].v[1] - axis_vectors[axis_1].v[1] * axis_vectors[axis_0].v[2];
    axis_3.v[1] = axis_vectors[axis_1].v[0] * axis_vectors[axis_0].v[2] - axis_vectors[axis_1].v[2] * axis_vectors[axis_0].v[0];
    axis_3.v[2] = axis_vectors[axis_1].v[1] * axis_vectors[axis_0].v[0] - axis_vectors[axis_1].v[0] * axis_vectors[axis_0].v[1];
    for (i = 0; i < 3; i++) {
        transform->m[i][0] = axis_vectors[axis_0].v[i];
        transform->m[i][1] = axis_vectors[axis_1].v[i];
        transform->m[i][2] = axis_3.v[i];
    }
    for (i = 0; i < 3; i++) {
        tr.v[i] = -(model->bounds.max.v[i] * .5f + model->bounds.min.v[i] * .5f);
    }
    for (i = 0; i < 3; i++) {
        if (model->bounds.max.v[i] == model->bounds.min.v[i]) {
            sc.v[i] = 1.f;
        } else {
            sc.v[i] = 1.f / (model->bounds.max.v[i] * .5f - model->bounds.min.v[i] * .5f);
        }
    }

    BrMatrix34PreScale(transform, sc.v[0], sc.v[1], sc.v[2]);
    BrMatrix34PreTranslate(transform, tr.v[0], tr.v[1], tr.v[2]);
    return transform;
}

// IDA: void __cdecl BrModelFree(br_model *m)
void BrModelFree(br_model* m) {
    BrResFree(m);
}

// IDA: br_model* __cdecl BrModelAllocate(char *name, int nvertices, int nfaces)
br_model* BrModelAllocate(char* name, int nvertices, int nfaces) {
    br_model* m;

    m = BrResAllocate(v1db.res, sizeof(br_model), BR_MEMORY_MODEL);
    m->nfaces = nfaces;
    m->nvertices = nvertices;
    if (name != NULL) {
        m->identifier = BrResStrDup(m, name);
    }
    if (nvertices != 0) {
        m->vertices = BrResAllocate(m, sizeof(br_vertex) * nvertices, BR_MEMORY_VERTICES);
    }
    if (nfaces != 0) {
        m->faces = BrResAllocate(m, sizeof(br_face) * nfaces, BR_MEMORY_FACES);
    }
    return m;
}

// IDA: br_primitive_list* __cdecl BrPrimitiveListAllocate(br_uint_32 prim_type, br_uint_16 num_prims)
br_primitive_list* BrPrimitiveListAllocate(br_uint_32 prim_type, br_uint_16 num_prims) {
    LOG_TRACE("(%d, %d)", prim_type, num_prims);

    return NULL;
}

// IDA: br_uint_32 __cdecl BrModelAddPrimitiveList(br_model *model, br_primitive_list *primitive_list)
br_uint_32 BrModelAddPrimitiveList(br_model* model, br_primitive_list* primitive_list) {
    LOG_TRACE("(%p, %p)", model, primitive_list);

    return NULL;
}
