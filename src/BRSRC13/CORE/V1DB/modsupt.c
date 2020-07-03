#include "modsupt.h"
#include "CORE/FW/resource.h"
#include "CORE/V1DB/dbsetup.h"
#include "harness.h"

char rscid[50];

// Offset: 16
// Size: 703
void BrModelApplyMap(br_model* model, int map_type, br_matrix34* xform) {
    int v;
    br_vertex* vp;
    br_vector3 mv;
    br_matrix34 default_xform;
    br_scalar d;
    NOT_IMPLEMENTED();
}

// Offset: 733
// Size: 697
br_matrix34* BrModelFitMap(br_model* model, int axis_0, int axis_1, br_matrix34* transform) {
    br_vector3 axis_3;
    br_vector3 tr;
    br_vector3 sc;
    int i;
    static br_vector3 axis_vectors[6];
    NOT_IMPLEMENTED();
}

// Offset: 1442
// Size: 73
void BrModelFree(br_model* m) {
    BrResFree(m);
}

// Offset: 1531
// Size: 250
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

// Offset: 1805
// Size: 40
br_primitive_list* BrPrimitiveListAllocate(br_uint_32 prim_type, br_uint_16 num_prims) {
    NOT_IMPLEMENTED();
}

// Offset: 1869
// Size: 40
br_uint_32 BrModelAddPrimitiveList(br_model* model, br_primitive_list* primitive_list) {
    NOT_IMPLEMENTED();
}
