#include "modsupt.h"
#include "CORE/FW/resource.h"
#include "CORE/V1DB/dbsetup.h"
#include "harness_trace.h"

// IDA: void __cdecl BrModelApplyMap(br_model *model, int map_type, br_matrix34 *xform)
void BrModelApplyMap(br_model* model, int map_type, br_matrix34* xform) {
    int v;
    br_vertex* vp;
    br_vector3 mv;
    br_matrix34 default_xform;
    br_scalar d;
    LOG_TRACE("(%p, %d, %p)", model, map_type, xform);
    NOT_IMPLEMENTED();
}

// IDA: br_matrix34* __cdecl BrModelFitMap(br_model *model, int axis_0, int axis_1, br_matrix34 *transform)
br_matrix34* BrModelFitMap(br_model* model, int axis_0, int axis_1, br_matrix34* transform) {
    br_vector3 axis_3;
    br_vector3 tr;
    br_vector3 sc;
    int i;
    static br_vector3 axis_vectors[6];
    LOG_TRACE("(%p, %d, %d, %p)", model, axis_0, axis_1, transform);
    NOT_IMPLEMENTED();
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
    if (name) {
        m->identifier = BrResStrDup(m, name);
    }
    if (nvertices) {
        m->vertices = (br_vertex*)BrResAllocate(m, sizeof(br_vertex) * nvertices, BR_MEMORY_VERTICES);
    }
    if (nfaces) {
        m->faces = (br_face*)BrResAllocate(m, sizeof(br_face) * nfaces, BR_MEMORY_FACES);
    }
    return m;
}

// IDA: br_primitive_list* __cdecl BrPrimitiveListAllocate(br_uint_32 prim_type, br_uint_16 num_prims)
br_primitive_list* BrPrimitiveListAllocate(br_uint_32 prim_type, br_uint_16 num_prims) {
    LOG_TRACE("(%d, %d)", prim_type, num_prims);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrModelAddPrimitiveList(br_model *model, br_primitive_list *primitive_list)
br_uint_32 BrModelAddPrimitiveList(br_model* model, br_primitive_list* primitive_list) {
    LOG_TRACE("(%p, %p)", model, primitive_list);
    NOT_IMPLEMENTED();
}
