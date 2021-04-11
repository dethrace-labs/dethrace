#include "prepmesh.h"
#include "CORE/FW/brqsort.h"
#include "CORE/FW/diag.h"
#include "CORE/FW/resource.h"
#include "CORE/FW/scratch.h"
#include "CORE/MATH/plane.h"
#include "CORE/MATH/vector.h"
#include "CORE/STD/brmath.h"
#include "CORE/STD/brstdlib.h"
#include "angles.h"
#include "harness.h"
#include <assert.h>

br_model* compareModel;
char* pm_edge_scratch;
pm_temp_edge** pm_edge_hash;
int num_edges;
pm_temp_edge* pm_edge_table;
char rscid[51];

#define PREP_ALIGN(f) (f)

// IDA: int __usercall addEdge@<EAX>(br_uint_16 first@<EAX>, br_uint_16 last@<EDX>)
int addEdge(br_uint_16 first, br_uint_16 last) {
    pm_temp_edge* tep;
    LOG_TRACE("(%d, %d)", first, last);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall prepareEdges(v11group *group@<EAX>, br_model *model@<EDX>)
void prepareEdges(v11group* group, br_model* model) {
    br_size_t scratch_size;
    br_face* mfp;
    v11face* fp;
    int f;
    LOG_TRACE("(%p, %p)", group, model);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall BrPrepareEdges(br_model *model@<EAX>)
void BrPrepareEdges(br_model* model) {
    int g;
    v11model* v11m;
    LOG_TRACE("(%p)", model);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl FacesCompare(void *p1, void *p2)
int FacesCompare(const void* p1, const void* p2) {
    br_face* f1;
    br_face* f2;
    //LOG_TRACE9("(%p, %p)", p1, p2);

    f1 = *(br_face**)p1;
    f2 = *(br_face**)p2;

    // Added by Jeff to give us stable ordering for debugging
    // if (f1->material == f2->material) {
    //     return 0;
    // }
    // if (f1->order_key > f2->order_key) {
    //     return 1;
    // }
    // return -1;
    // //////

    // Original code:
    if (f1->material > f2->material) {
        return 1;
    }
    if (f1->material < f2->material) {
        return -1;
    }
    return 0;
}

// IDA: int __cdecl TVCompare_XYZ(void *p1, void *p2)
int TVCompare_XYZ(const void* p1, const void* p2) {
    prep_vertex* tv1;
    prep_vertex* tv2;
    br_vertex* v1;
    br_vertex* v2;
    int i;
    //LOG_TRACE9("(%p, %p)", p1, p2);

    tv1 = *(prep_vertex**)p1;
    tv2 = *(prep_vertex**)p2;

    assert(compareModel != NULL);

    if (tv1->v == tv2->v)
        return 0;

    v1 = compareModel->vertices + tv1->v;
    v2 = compareModel->vertices + tv2->v;

    for (i = 0; i < 3; i++) {
        if (v1->p.v[i] > v2->p.v[i])
            return 1;
        if (v1->p.v[i] < v2->p.v[i])
            return -1;
    }

    return 0;
}

// IDA: int __cdecl TVCompare_MXYZUVN(void *p1, void *p2)
int TVCompare_MXYZUVN(const void* p1, const void* p2) {
    prep_vertex* tv1;
    prep_vertex* tv2;
    br_vertex* v1;
    br_vertex* v2;
    int i;
    //LOG_TRACE("(%p, %p)", p1, p2);

    tv1 = *(struct prep_vertex**)p1;
    tv2 = *(struct prep_vertex**)p2;

    assert(compareModel != NULL);

    if (compareModel->faces[tv1->f].material > compareModel->faces[tv2->f].material)
        return 1;
    if (compareModel->faces[tv1->f].material < compareModel->faces[tv2->f].material)
        return -1;

    if (tv1->v != tv2->v) {
        v1 = compareModel->vertices + tv1->v;
        v2 = compareModel->vertices + tv2->v;

        for (i = 0; i < 3; i++) {
            if (v1->p.v[i] > v2->p.v[i])
                return 1;
            if (v1->p.v[i] < v2->p.v[i])
                return -1;
        }

        for (i = 0; i < 2; i++) {
            if (v1->map.v[i] > v2->map.v[i])
                return 1;
            if (v1->map.v[i] < v2->map.v[i])
                return -1;
        }
    }

    for (i = 0; i < 3; i++) {
        if (tv1->n.v[i] > tv2->n.v[i])
            return 1;
        if (tv1->n.v[i] < tv2->n.v[i])
            return -1;
    }

    return 0;
}

// IDA: int __cdecl TVCompare_MVN(void *p1, void *p2)
int TVCompare_MVN(const void* p1, const void* p2) {
    prep_vertex* tv1;
    prep_vertex* tv2;
    int i;
    //LOG_TRACE9("(%p, %p)", p1, p2);

    tv1 = *(prep_vertex**)p1;
    tv2 = *(prep_vertex**)p2;

    // if (compareModel->faces[tv1->f].material != NULL) {
    //     LOG_DEBUG("%s, %s", compareModel->faces[tv1->f].material->identifier, compareModel->faces[tv2->f].material->identifier);
    // }
    if (compareModel->faces[tv1->f].material > compareModel->faces[tv2->f].material)
        return 1;
    if (compareModel->faces[tv1->f].material < compareModel->faces[tv2->f].material)
        return -1;

    if (tv1->v > tv2->v)
        return 1;
    if (tv1->v < tv2->v)
        return -1;

    /*
	 * Compare Normal
	 */
    for (i = 0; i < 3; i++) {
        if (tv1->n.v[i] > tv2->n.v[i])
            return 1;
        if (tv1->n.v[i] < tv2->n.v[i])
            return -1;
    }
    return 0;
}

// IDA: br_fraction __cdecl BrScalarToFractionClamp(br_scalar s)
br_fraction BrScalarToFractionClamp(br_scalar s) {
    if (s >= 1.0f) {
        return 1.0f;
    }
    if (s < -1.0f) {
        return -0.99999988f;
    }
    return s;
}

// IDA: void __usercall PrepareFaceNormals(br_model *model@<EAX>)
void PrepareFaceNormals(br_model* model) {
    br_vertex* vertices;
    br_vector4 v4;
    br_face* fp;
    int f;

    for (f = 0; f < model->nfaces; f++) {
        fp = &model->faces[f];
        BrPlaneEquation(&v4, &model->vertices[fp->vertices[0]].p, &model->vertices[fp->vertices[1]].p, &model->vertices[fp->vertices[2]].p);
        fp->n.v[0] = BrScalarToFractionClamp(v4.v[0]);
        fp->n.v[1] = BrScalarToFractionClamp(v4.v[1]);
        fp->n.v[2] = BrScalarToFractionClamp(v4.v[2]);
        fp->d = v4.v[3];
    }
}

// IDA: void __usercall Smoothing(br_model *model@<EAX>, br_scalar crease_limit, prep_vertex **start, prep_vertex **end)
void Smoothing(br_model* model, br_scalar crease_limit, prep_vertex** start, prep_vertex** end) {
    prep_vertex** outer;
    prep_vertex** inner;
    LOG_TRACE9("(%p, %f, %p, %p)", model, crease_limit, start, end);

    for (outer = start; outer < end; outer++) {
        for (inner = start; inner < end; inner++) {
            if ((inner == outer) || (model->faces[(*outer)->f].smoothing & model->faces[(*inner)->f].smoothing)) {
                BrVector3Accumulate(&(*outer)->n, (br_vector3*)&model->faces[(*inner)->f].n);
            }
        }
    }
}

// IDA: void __usercall SmoothingCreased(br_model *model@<EAX>, br_scalar crease_limit, prep_vertex **start, prep_vertex **end)
void SmoothingCreased(br_model* model, br_scalar crease_limit, prep_vertex** start, prep_vertex** end) {
    br_vector3 o_n;
    prep_vertex** outer;
    prep_vertex** inner;
    LOG_TRACE("(%p, %f, %p, %p)", model, crease_limit, start, end);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CopyVertex(v11group *group@<EAX>, int v@<EDX>, prep_vertex *src@<EBX>, br_model *model@<ECX>)
void CopyVertex(v11group* group, int v, prep_vertex* src, br_model* model) {
    br_vertex* srcv;
    br_vector3 n;
    LOG_TRACE9("(%p, %d, %p, %p)", group, v, src, model);

    srcv = model->vertices + src->v;
    br_vector3 temp;

    group->vertices[v].p.v[0] = srcv->p.v[0] - model->pivot.v[0];
    group->vertices[v].p.v[1] = srcv->p.v[1] - model->pivot.v[1];
    group->vertices[v].p.v[2] = srcv->p.v[2] - model->pivot.v[2];

    group->vertices[v].map = srcv->map;

    BrVector3Normalise(&group->vertices[v].n, &src->n);

    group->vertex_colours[v] = BR_COLOUR_RGBA(srcv->red, srcv->grn, srcv->blu, srcv->index);
    group->vertex_user[v] = src->v;
}

// IDA: void __usercall CopyFace(v11group *group@<EAX>, int f@<EDX>, br_face *src@<EBX>, br_model *model@<ECX>)
void CopyFace(v11group* group, int f, br_face* src, br_model* model) {
    LOG_TRACE9("(%p, %d, %p, %p)", group, f, src, model);

    group->faces[f].vertices[0] = src->vertices[0];
    group->faces[f].vertices[1] = src->vertices[1];
    group->faces[f].vertices[2] = src->vertices[2];
    group->face_colours[f] = BR_COLOUR_RGBA(src->red, src->grn, src->blu, src->index);
    group->faces[f].eqn.v[0] = src->n.v[0];
    group->faces[f].eqn.v[1] = src->n.v[1];
    group->faces[f].eqn.v[2] = src->n.v[2];
    group->faces[f].eqn.v[3] = src->d;
    group->face_user[f] = src - model->faces;
}

// IDA: void __usercall PrepareGroups(br_model *model@<EAX>)
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

    if (model->flags & BR_MODF_DONT_WELD) {
        vertex_compare_smoothing = TVCompare_XYZ;
        vertex_compare_groups = TVCompare_MVN;
    } else {
        vertex_compare_smoothing = TVCompare_XYZ;
        vertex_compare_groups = TVCompare_MXYZUVN;
    }

    if (model->flags & BR_MODF_CREASE) {
        crease_limit = BR_COS(model->crease_angle);
        smoothing_fn = SmoothingCreased;
    } else {
        crease_limit = BR_SCALAR(0.0);
        smoothing_fn = Smoothing;
    }

    compareModel = model;

    ntemps = model->nfaces * 3;

    block_size = ntemps * (sizeof(*temp_verts) + sizeof(*sorted_vertices)) + model->nfaces * sizeof(sorted_faces);
    temp_verts = BrScratchAllocate(block_size);

    BrMemSet(temp_verts, 0, block_size);

    sorted_vertices = (struct prep_vertex**)(temp_verts + ntemps);
    sorted_faces = (struct br_face**)(sorted_vertices + ntemps);

    gtvp = temp_verts;
    for (i = 0, f = 0, fp = model->faces; f < model->nfaces; f++, fp++) {
        sorted_faces[f] = fp;
        for (v = 0; v < 3; v++, i++, gtvp++) {

            if (model->flags & BR_MODF_CUSTOM_NORMALS) {
                TELL_ME_IF_WE_PASS_THIS_WAY();
                // gtvp->n.v[0] = model->vertices[fp->vertices[v]].n.v[0];
                // gtvp->n.v[1] = model->vertices[fp->vertices[v]].n.v[1];
                // gtvp->n.v[2] = model->vertices[fp->vertices[v]].n.v[2];
            }

            gtvp->v = fp->vertices[v];
            gtvp->f = f;

            sorted_vertices[i] = gtvp;
        }

        if (fp->smoothing == 0)
            fp->smoothing = (br_uint_16)~0;
    }

    BrQsort(sorted_faces, model->nfaces, sizeof(*sorted_faces), FacesCompare);

    if (!(model->flags & BR_MODF_CUSTOM_NORMALS)) {

        BrQsort(sorted_vertices, ntemps, sizeof(*sorted_vertices), vertex_compare_smoothing);
        for (v = 0, i = 0; v < ntemps - 1; v++) {
            if (vertex_compare_smoothing(sorted_vertices + v, sorted_vertices + v + 1)) {
                smoothing_fn(model, crease_limit, sorted_vertices + i, sorted_vertices + v + 1);
                i = v + 1;
            }
        }

        smoothing_fn(model, crease_limit, sorted_vertices + i, sorted_vertices + ntemps);
    }

    BrQsort(sorted_vertices, ntemps, sizeof(*sorted_vertices), vertex_compare_groups);

    for (v = 0, nv = 1; v < ntemps - 1; v++)
        if (vertex_compare_groups(sorted_vertices + v, sorted_vertices + v + 1))
            nv++;

    for (f = 1, ng = 1; f < model->nfaces; f++) {
        if (sorted_faces[f]->material != sorted_faces[f - 1]->material) {
            ng++;
        }
    }

    nf = model->nfaces;

    block_size = PREP_ALIGN(sizeof(struct v11model)) + PREP_ALIGN(ng * sizeof(struct v11group)) +

        PREP_ALIGN(nf * sizeof(*v11g->faces)) +

        PREP_ALIGN(nv * sizeof(*v11g->vertices)) +

        PREP_ALIGN(nf * sizeof(br_colour)) + PREP_ALIGN(nv * sizeof(br_colour)) + nv * sizeof(br_int_16) + nf * sizeof(br_int_16);

    if (model->prepared && block_size > ((struct v11model*)(model->prepared))->size) {
        BrResFree(model->prepared);
        model->prepared = NULL;
    }

    if (model->prepared == NULL) {
        model->prepared = BrResAllocate(model, block_size, BR_MEMORY_PREPARED_MODEL);
        ((struct v11model*)(model->prepared))->size = block_size;
    }

    cp = model->prepared;

    v11m = (void*)cp;
    cp += PREP_ALIGN(sizeof(struct v11model));
    v11g = (void*)cp;
    cp += PREP_ALIGN(ng * sizeof(struct v11group));

    v11f = (void*)cp;
    cp += PREP_ALIGN(nf * sizeof(struct v11face));

    v11v = (void*)cp;
    cp += PREP_ALIGN(nv * sizeof(struct fmt_vertex));

    v11vcolours = (void*)cp;
    cp += PREP_ALIGN(nv * sizeof(br_colour));
    v11fcolours = (void*)cp;
    cp += PREP_ALIGN(nf * sizeof(br_colour));

    v11vuser = (void*)cp;
    cp += nv * sizeof(br_uint_16);
    v11fuser = (void*)cp;
    cp += nf * sizeof(br_uint_16);

    v11m->groups = v11g;
    v11m->ngroups = ng;

    v11g[0].faces = v11f;
    v11g[0].face_colours = v11fcolours;
    v11g[0].face_user = v11fuser;
    v11g[0].stored = sorted_faces[0]->material ? sorted_faces[0]->material->stored : NULL;
    v11g[0].nfaces = 0;

    for (f = 0, g = 0; f < model->nfaces - 1; f++) {

        CopyFace(v11g + g, v11g[g].nfaces, sorted_faces[f], model);
        v11g[g].nfaces++;

        if (sorted_faces[f]->material != sorted_faces[f + 1]->material) {
            g++;
            v11g[g].faces = v11f + f + 1;
            v11g[g].face_colours = v11fcolours + f + 1;
            v11g[g].face_user = v11fuser + f + 1;
            v11g[g].stored = sorted_faces[f + 1]->material ? sorted_faces[f + 1]->material->stored : NULL;
            v11g[g].nfaces = 0;
        }
    }

    CopyFace(v11g + g, v11g[g].nfaces, sorted_faces[f], model);
    v11g[g].nfaces++;
    v11g[0].vertices = v11v;

    v11g[0].vertex_colours = v11vcolours;
    v11g[0].vertex_user = v11vuser;

    v11g[0].nvertices = 1;
    CopyVertex(v11g, 0, sorted_vertices[0], model);

    for (v = 0, g = 0, count = 0; v < ntemps - 1; v++) {

        if (model->faces[sorted_vertices[v]->f].material != model->faces[sorted_vertices[v + 1]->f].material) {
            g++;
            v11g[g].vertices = v11v + count + 1;
            v11g[g].vertex_colours = v11vcolours + count + 1;
            v11g[g].vertex_user = v11vuser + count + 1;
            v11g[g].nvertices = 0;
        }

        old_count = count;
        if (vertex_compare_groups(sorted_vertices + v, sorted_vertices + v + 1)) {

            count++;
            sorted_vertices[v]->v = count;
            CopyVertex(v11g + g, v11g[g].nvertices, sorted_vertices[v + 1], model);
            v11g[g].nvertices++;
        }
        sorted_vertices[v]->v = old_count;
    }

    sorted_vertices[v]->v = count;

    int minus = 0;
    for (g = 0; g < ng; g++) {
        for (f = 0; f < v11g[g].nfaces; f++) {
            i = v11g[g].vertices - v11v;
            v = v11g[g].face_user[f] * 3;
            v11g[g].faces[f].vertices[0] = temp_verts[v + 0].v - i;
            v11g[g].faces[f].vertices[1] = temp_verts[v + 1].v - i;
            v11g[g].faces[f].vertices[2] = temp_verts[v + 2].v - i;
        }
        minus += v11g[g].nfaces * 3;
    }
    BrScratchFree(temp_verts);
}

// IDA: void __usercall PrepareBoundingRadius(br_model *model@<EAX>)
void PrepareBoundingRadius(br_model* model) {
    float d;
    float max = 0.0f;
    int v;
    br_vertex* vp;

    for (v = 0; v < model->nvertices; v++) {
        vp = &model->vertices[v];
        d = vp->p.v[1] * vp->p.v[1] + vp->p.v[0] * vp->p.v[0] + vp->p.v[2] * vp->p.v[2];
        if (d > max) {
            max = d;
        }
    }
    model->radius = BrFloatSqrt(max);
}

// IDA: void __usercall PrepareBoundingBox(br_model *model@<EAX>)
void PrepareBoundingBox(br_model* model) {
    int axis;
    int v;
    br_vertex* vp;
    br_scalar x;

    vp = &model->vertices[0];
    for (axis = 0; axis < 3; axis++) {
        model->bounds.min.v[axis] = vp->p.v[axis];
        model->bounds.max.v[axis] = vp->p.v[axis];
    }

    for (v = 1; v < model->nvertices; v++) {
        vp = &model->vertices[v];
        for (axis = 0; axis < 3; axis++) {
            x = vp->p.v[axis];
            if (x > model->bounds.max.v[axis]) {
                model->bounds.max.v[axis] = x;
            }
            if (x < model->bounds.min.v[axis]) {
                model->bounds.min.v[axis] = x;
            }
        }
    }
}

// IDA: void __usercall RegenerateFaceNormals(v11model *v11m@<EAX>)
void RegenerateFaceNormals(v11model* v11m) {
    int g;
    int f;
    v11face* fp;
    LOG_TRACE("(%p)", v11m);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RegenerateVertexNormals(v11model *v11m@<EAX>)
void RegenerateVertexNormals(v11model* v11m) {
    int g;
    int v;
    int f;
    v11face* fp;
    fmt_vertex* vp;
    br_vector3* normals;
    LOG_TRACE("(%p)", v11m);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrModelUpdate(br_model *model, br_uint_16 flags)
void BrModelUpdate(br_model* model, br_uint_16 flags) {
    int g;
    int f;
    int v;
    v11model* v11m;
    fmt_vertex* fvp;
    v11face* ffp;
    br_vertex* vp;
    br_face* fp;
    LOG_TRACE("(%p, %d)", model, flags);

    if (!(model->flags & BR_MODF_PREPREPARED)) {
        if (!model->faces || !model->vertices) {
            BrFailure("BrModelUpdate: model has no faces or vertices (%s)", model->identifier ? model->identifier : "<NULL>");
        }
        if (flags & BR_MODU_UNKNOWN) {
            flags |= BR_MODU_NORMALS;
        }
        if (model->flags & (BR_MODF_KEEP_ORIGINAL | BR_MODF_GENERATE_TAGS)) {
            model->flags |= BR_MODF_UPDATEABLE;
        }
        if (!(model->flags & 0x400) && (flags & (BR_MODU_MATERIALS | BR_MODU_GROUPS | BR_MODU_NORMALS))) {
            PrepareBoundingRadius(model);
            PrepareBoundingBox(model);
        }
        if (!model->prepared || flags & 0xFFD0) {
            if (!model->faces || !model->vertices) {
                return;
            }
            PrepareFaceNormals(model);
            f = 0;
            fp = model->faces;
            model->flags &= ~MODF_USES_DEFAULT;
            for (f = 0; f < model->nfaces; f++) {
                fp = &model->faces[f];
                if (!fp->material) {
                    model->flags |= MODF_USES_DEFAULT;
                }

                for (v = 0; v < 3; v++) {
                    if (fp->vertices[v] >= model->nvertices) {
                        BrFailure("face references invalid vertex f=%d v=%d", f, v);
                    }
                }
            }

            PrepareGroups(model);
            v11m = (v11model*)model->prepared;
            if (v11m) {
                for (g = 0; g < v11m->ngroups; g++) {
                    //prepareEdges(&v11m->groups[g], model);
                }
            }
        } else {
            LOG_WARN("Do we ever get here??");
            // v11m = (v11model*)model->prepared;
            // if (model->vertices && flags & 0xF) {
            //     for (g = 0; g < v11m->ngroups; g++) {
            //         // v5 = 0;
            //         // v39 = 0;
            //         // v30 = 0;
            //         // v40 = 0;
            //         // v29 = 36 * i;

            //         for (v = 0; v < v11m->groups[g].nvertices; v++) {
            //             fvp = &v11m->groups[g].vertices[v];
            //             vp = &model->vertices[v11m->groups[g].vertex_user[v]]; // v8->vertex_user[v5]];
            //             if (flags & BR_MODU_NORMALS) {
            //                 fvp->p.v[0] = vp->p.v[0] - model->pivot.v[0];
            //                 fvp->p.v[1] = vp->p.v[1] - model->pivot.v[1];
            //                 fvp->p.v[2] = vp->p.v[2] - model->pivot.v[2];
            //             }
            //             if (flags & BR_MODU_EDGES) {
            //                 v31 = _byteswap_ulong(*(_DWORD*)&vp->index);
            //                 v11m->groups[g].vertex_colours[v] = v31;
            //             }
            //             if (flags & BR_MODU_RADIUS) {
            //                 fvp->map.v[0] = vp->map.v[0];
            //                 fvp->map.v[1] = vp->map.v[1];
            //             }
            //             if (flags & BR_MODU_GROUPS && model->flags & 0x200) {
            //                 fvp->n.v[0] = vp->n.v[0];
            //                 fvp->n.v[1] = vp->n.v[1];
            //                 fvp->n.v[2] = vp->n.v[2];
            //             }
            //             ++v5;
            //             v39 += 32;
            //             v30 += 4;

            //             ++v;
            //         }
            //     }
            // }
            // if (model->faces && flags & BR_MODU_MATERIALS) {
            //     v35 = 0;
            //     v27 = 0;
            //     while (v11m->ngroups > v27) {
            //         v9 = 0;
            //         v10 = 0;
            //         v28 = v35;
            //         for (j = 0;; j += 4) {
            //             v12 = (v11group*)((char*)v11m->groups + v28); // not sure about this one
            //             if (v9 >= v12->nfaces) {
            //                 break;
            //             }
            //             v31 = 40 * v12->face_user[v10];
            //             fp = (br_face*)((char*)model->faces + v31);
            //             if (flags & 0x20) {
            //                 v32 = fp->index << 24;
            //                 v31 = (fp->red << 16) | v32;
            //                 v32 = fp->grn << 8;
            //                 v12->face_colours[j / 4] = fp->blu | v32 | v31;
            //             }
            //             ++v9;
            //             ++v10;
            //         }
            //         ++v27;
            //         v35 += 36;
            //     }
            // }
            // if (flags & 1) {
            //     if (!(model->flags & 0x200)) {
            //         RegenerateVertexNormals(v11m);
            //     }
            //     RegenerateFaceNormals(v11m);
            // }
        }

        if (!(model->flags & BR_MODF_UPDATEABLE)) {
            if (model->faces) {
                BrResFree(model->faces);
            }
            if (model->vertices) {
                BrResFree(model->vertices);
            }
            model->vertices = 0;
            model->nfaces = 0;
            model->nvertices = 0;
            model->faces = 0;
        }
    }
}

// IDA: void __usercall BrModelClear(br_model *model@<EAX>)
void BrModelClear(br_model* model) {
    LOG_TRACE("(%p)", model);

    // does something with ->stored field, we don't care
}
