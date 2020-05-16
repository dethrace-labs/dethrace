#include "prepmesh.h"
#include "debug.h"

br_model* compareModel;
char* pm_edge_scratch;
pm_temp_edge** pm_edge_hash;
int num_edges;
pm_temp_edge* pm_edge_table;
char rscid[51];

// Offset: 8
// Size: 246
// EAX: first
// EDX: last
int addEdge(br_uint_16 first, br_uint_16 last) {
    pm_temp_edge* tep;
    NOT_IMPLEMENTED();
}

// Offset: 267
// Size: 461
// EAX: group
// EDX: model
void prepareEdges(v11group* group, br_model* model) {
    br_size_t scratch_size;
    br_face* mfp;
    v11face* fp;
    int f;
    NOT_IMPLEMENTED();
}

// Offset: 743
// Size: 115
// EAX: model
void BrPrepareEdges(br_model* model) {
    int g;
    v11model* v11m;
    NOT_IMPLEMENTED();
}

// Offset: 871
// Size: 102
int FacesCompare(void* p1, void* p2) {
    br_face* f1;
    br_face* f2;
    NOT_IMPLEMENTED();
}

// Offset: 987
// Size: 268
int TVCompare_XYZ(void* p1, void* p2) {
    prep_vertex* tv1;
    prep_vertex* tv2;
    br_vertex* v1;
    br_vertex* v2;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 1273
// Size: 610
int TVCompare_MXYZUVN(void* p1, void* p2) {
    prep_vertex* tv1;
    prep_vertex* tv2;
    br_vertex* v1;
    br_vertex* v2;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 1897
// Size: 383
int TVCompare_MVN(void* p1, void* p2) {
    prep_vertex* tv1;
    prep_vertex* tv2;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 2304
// Size: 88
br_fraction BrScalarToFractionClamp(br_scalar s) {
    NOT_IMPLEMENTED();
}

// Offset: 2411
// Size: 284
// EAX: model
void PrepareFaceNormals(br_model* model) {
    br_vertex* vertices;
    br_vector4 v4;
    br_face* fp;
    int f;
    NOT_IMPLEMENTED();
}

// Offset: 2705
// Size: 296
// EAX: model
void Smoothing(br_model* model, br_scalar crease_limit, prep_vertex** start, prep_vertex** end) {
    prep_vertex** outer;
    prep_vertex** inner;
    NOT_IMPLEMENTED();
}

// Offset: 3018
// Size: 500
// EAX: model
void SmoothingCreased(br_model* model, br_scalar crease_limit, prep_vertex** start, prep_vertex** end) {
    br_vector3 o_n;
    prep_vertex** outer;
    prep_vertex** inner;
    NOT_IMPLEMENTED();
}

// Offset: 3529
// Size: 479
// EAX: group
// EDX: v
// EBX: src
// ECX: model
void CopyVertex(v11group* group, int v, prep_vertex* src, br_model* model) {
    br_vertex* srcv;
    br_vector3 n;
    NOT_IMPLEMENTED();
}

// Offset: 4017
// Size: 293
// EAX: group
// EDX: f
// EBX: src
// ECX: model
void CopyFace(v11group* group, int f, br_face* src, br_model* model) {
    NOT_IMPLEMENTED();
}

// Offset: 4324
// Size: 2516
// EAX: model
void PrepareGroups(br_model* model) {
    br_qsort_cbfn* vertex_compare_smoothing;
    br_qsort_cbfn* vertex_compare_groups;
    void (*smoothing_fn)(br_model*, br_scalar, prep_vertex**, prep_vertex**);
    void* vp;
    br_size_t block_size;
    prep_vertex* temp_verts;
    prep_vertex* gtvp;
    prep_vertex** sorted_vertices;
    br_face* fp;
    int g;
    int f;
    int v;
    int i;
    int ntemps;
    int count;
    int nf;
    int nv;
    int ng;
    int old_count;
    br_scalar crease_limit;
    v11model* v11m;
    v11group* v11g;
    v11face* v11f;
    fmt_vertex* v11v;
    br_colour* v11fcolours;
    br_colour* v11vcolours;
    br_uint_16* v11fuser;
    br_uint_16* v11vuser;
    br_face** sorted_faces;
    char* cp;
    NOT_IMPLEMENTED();
}

// Offset: 6862
// Size: 170
// EAX: model
void PrepareBoundingRadius(br_model* model) {
    float d;
    float max;
    int v;
    br_vertex* vp;
    NOT_IMPLEMENTED();
}

// Offset: 7051
// Size: 272
// EAX: model
void PrepareBoundingBox(br_model* model) {
    int axis;
    int v;
    br_vertex* vp;
    br_scalar x;
    NOT_IMPLEMENTED();
}

// Offset: 7345
// Size: 268
// EAX: v11m
void RegenerateFaceNormals(v11model* v11m) {
    int g;
    int f;
    v11face* fp;
    NOT_IMPLEMENTED();
}

// Offset: 7637
// Size: 762
// EAX: v11m
void RegenerateVertexNormals(v11model* v11m) {
    int g;
    int v;
    int f;
    v11face* fp;
    fmt_vertex* vp;
    br_vector3* normals;
    NOT_IMPLEMENTED();
}

// Offset: 8413
// Size: 1585
void BrModelUpdate(br_model* model, br_uint_16 flags) {
    int g;
    int f;
    int v;
    v11model* v11m;
    fmt_vertex* fvp;
    v11face* ffp;
    br_vertex* vp;
    br_face* fp;
    NOT_IMPLEMENTED();
}

// Offset: 10011
// Size: 109
// EAX: model
void BrModelClear(br_model* model) {
    NOT_IMPLEMENTED();
}
