#include "prepmesh.h"
#include "CORE/FW/diag.h"
#include "CORE/MATH/plane.h"
#include "CORE/STD/brmath.h"
#include "harness.h"

br_model* compareModel;
char* pm_edge_scratch;
pm_temp_edge** pm_edge_hash;
int num_edges;
pm_temp_edge* pm_edge_table;
char rscid[51];

// IDA: int __usercall addEdge@<EAX>(br_uint_16 first@<EAX>, br_uint_16 last@<EDX>)
int addEdge(br_uint_16 first, br_uint_16 last) {
    pm_temp_edge *tep;
    LOG_TRACE("(%d, %d)", first, last);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall prepareEdges(v11group *group@<EAX>, br_model *model@<EDX>)
void prepareEdges(v11group* group, br_model* model) {
    br_size_t scratch_size;
    br_face *mfp;
    v11face *fp;
    int f;
    LOG_TRACE("(%p, %p)", group, model);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall BrPrepareEdges(br_model *model@<EAX>)
void BrPrepareEdges(br_model* model) {
    int g;
    v11model *v11m;
    LOG_TRACE("(%p)", model);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl FacesCompare(void *p1, void *p2)
int FacesCompare(void* p1, void* p2) {
    br_face *f1;
    br_face *f2;
    LOG_TRACE("(%p, %p)", p1, p2);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl TVCompare_XYZ(void *p1, void *p2)
int TVCompare_XYZ(void* p1, void* p2) {
    prep_vertex *tv1;
    prep_vertex *tv2;
    br_vertex *v1;
    br_vertex *v2;
    int i;
    LOG_TRACE("(%p, %p)", p1, p2);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl TVCompare_MXYZUVN(void *p1, void *p2)
int TVCompare_MXYZUVN(void* p1, void* p2) {
    prep_vertex *tv1;
    prep_vertex *tv2;
    br_vertex *v1;
    br_vertex *v2;
    int i;
    LOG_TRACE("(%p, %p)", p1, p2);
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl TVCompare_MVN(void *p1, void *p2)
int TVCompare_MVN(void* p1, void* p2) {
    prep_vertex *tv1;
    prep_vertex *tv2;
    int i;
    LOG_TRACE("(%p, %p)", p1, p2);
    NOT_IMPLEMENTED();
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
    prep_vertex **outer;
    prep_vertex **inner;
    LOG_TRACE("(%p, %f, %p, %p)", model, crease_limit, start, end);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SmoothingCreased(br_model *model@<EAX>, br_scalar crease_limit, prep_vertex **start, prep_vertex **end)
void SmoothingCreased(br_model* model, br_scalar crease_limit, prep_vertex** start, prep_vertex** end) {
    br_vector3 o_n;
    prep_vertex **outer;
    prep_vertex **inner;
    LOG_TRACE("(%p, %f, %p, %p)", model, crease_limit, start, end);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CopyVertex(v11group *group@<EAX>, int v@<EDX>, prep_vertex *src@<EBX>, br_model *model@<ECX>)
void CopyVertex(v11group* group, int v, prep_vertex* src, br_model* model) {
    br_vertex *srcv;
    br_vector3 n;
    LOG_TRACE("(%p, %d, %p, %p)", group, v, src, model);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CopyFace(v11group *group@<EAX>, int f@<EDX>, br_face *src@<EBX>, br_model *model@<ECX>)
void CopyFace(v11group* group, int f, br_face* src, br_model* model) {
    LOG_TRACE("(%p, %d, %p, %p)", group, f, src, model);
    NOT_IMPLEMENTED();
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

    LOG_WARN("Ignoring this one for now");

    return;
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
    v11face *fp;
    LOG_TRACE("(%p)", v11m);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RegenerateVertexNormals(v11model *v11m@<EAX>)
void RegenerateVertexNormals(v11model* v11m) {
    int g;
    int v;
    int f;
    v11face *fp;
    fmt_vertex *vp;
    br_vector3 *normals;
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
            model->flags &= 0x7f;
            for (f = 0; f < model->nfaces; f++) {
                fp = &model->faces[f];
                if (!fp->material) {
                    model->flags |= BR_MODF_UPDATEABLE;
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
                    prepareEdges(&v11m->groups[g], model);
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

        // We arent generating optimized model or storing at this point, so
        // we do not want to free the faces/verts

        // if (model->flags >= 0) {
        //     if (model->faces) {
        //         BrResFree(model->faces);
        //     }
        //     if (model->vertices) {
        //         BrResFree(model->vertices);
        //     }
        //     model->vertices = 0;
        //     model->nfaces = 0;
        //     model->nvertices = 0;
        //     model->faces = 0;
        // }
    }
}

// IDA: void __usercall BrModelClear(br_model *model@<EAX>)
void BrModelClear(br_model* model) {
    LOG_TRACE("(%p)", model);
    NOT_IMPLEMENTED();
}
