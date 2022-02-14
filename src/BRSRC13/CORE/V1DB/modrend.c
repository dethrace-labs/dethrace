#include "modrend.h"
#include "dbsetup.h"
#include "harness/hooks.h"
#include "harness/trace.h"

render_style_cbfn RenderStyleCalls[8] = {
    renderFaces,                /* BR_RSTYLE_DEFAULT			*/
    nullRender,                 /* BR_RSTYLE_NONE				*/
    renderPoints,               /* BR_RSTYLE_POINTS				*/
    renderEdges,                /* BR_RSTYLE_EDGES				*/
    renderFaces,                /* BR_RSTYLE_FACES				*/
    boundingBoxRenderPoints,    /* BR_RSTYLE_BOUNDING_POINTS	*/
    boundingBoxRenderEdges,     /* BR_RSTYLE_BOUNDING_EDGES		*/
    boundingBoxRenderFaces,     /* BR_RSTYLE_BOUNDING_FACES		*/
};

v11face bounds_faces[12] = {
    { { 5, 6, 7 }, {  0, 13, 14}, { {  1.0f,  0.0f,  0.0f, 1.0f } } },
    { { 5, 4, 6 }, {  3,  4,  0}, { {  1.0f,  0.0f,  0.0f, 1.0f } } },
    { { 7, 6, 2 }, { 13,  6,  0}, { {  0.0f,  1.0f,  0.0f, 1.0f } } },
    { { 7, 2, 3 }, {  0,  5, 16}, { {  0.0f,  1.0f,  0.0f, 1.0f } } },
    { { 1, 5, 7 }, { 11, 14,  0}, { {  0.0f,  0.0f,  1.0f, 1.0f } } },
    { { 1, 7, 3 }, {  0, 16, 12}, { {  0.0f,  0.0f,  1.0f, 1.0f } } },
    { { 3, 0, 1 }, {  0,  8, 12}, { { -1.0f,  0.0f,  0.0f, 1.0f } } },
    { { 3, 2, 0 }, {  5,  1,  0}, { { -1.0f,  0.0f,  0.0f, 1.0f } } },
    { { 1, 0, 4 }, {  8,  9,  0}, { {  0.0f, -1.0f,  0.0f, 1.0f } } },
    { { 1, 4, 5 }, {  0,  3, 11}, { {  0.0f, -1.0f,  0.0f, 1.0f } } },
    { { 0, 6, 4 }, {  0,  4,  9}, { {  0.0f,  0.0f, -1.0f, 1.0f } } },
    { { 0, 2, 6 }, {  1,  6,  0}, { {  0.0f,  0.0f, -1.0f, 1.0f } } },
};
br_colour bounds_colours[12] = { 0 };
fmt_vertex bounds_vertices[8] = {
    { {{ -1.0f, -1.0f, -1.0f }}, {{ 0.0f, 0.0f }}, {{ -0.666f, -0.333f, -0.666f }} },
    { {{ -1.0f, -1.0f,  1.0f }}, {{ 0.0f, 0.0f }}, {{ -0.333f, -0.666f,  0.666f }} },
    { {{ -1.0f,  1.0f, -1.0f }}, {{ 0.0f, 1.0f }}, {{ -0.408f,  0.816f, -0.408f }} },
    { {{ -1.0f,  1.0f,  1.0f }}, {{ 0.0f, 1.0f }}, {{ -0.816f,  0.408f,  0.408f }} },
    { {{  1.0f, -1.0f, -1.0f }}, {{ 1.0f, 0.0f }}, {{  0.408f, -0.816f, -0.408f }} },
    { {{  1.0f, -1.0f,  1.0f }}, {{ 1.0f, 0.0f }}, {{  0.816f, -0.408f,  0.408f }} },
    { {{  1.0f,  1.0f, -1.0f }}, {{ 1.0f, 1.0f }}, {{  0.666f,  0.333f, -0.666f }} },
    { {{  1.0f,  1.0f,  1.0f }}, {{ 1.0f, 1.0f }}, {{  0.333f,  0.666f,  0.666f }} },
};
v11group bounds_face_groups[1] = {
    { NULL, bounds_faces, bounds_colours, NULL, bounds_vertices, bounds_colours, NULL, BR_ASIZE(bounds_faces), BR_ASIZE(bounds_vertices), 18 },
};
v11model bounds_prepared = { 0, 0, BR_ASIZE(bounds_face_groups), { { 0 } }, bounds_face_groups };
br_model bounds_model = {
    0,
    "Bounds",
    NULL,
    NULL,
    0,
    0,
    { { 0.f, 0.f, 0.f } },
    0,
    NULL,
    NULL,
    0,
    0.f,
    { { { 0.f, 0.f, 0.f } }, { { 0.f, 0.f, 0.f } } },
    &bounds_prepared,
    NULL,
};

// IDA: void __usercall renderFaces(br_actor *actor@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, int on_screen)
void renderFaces(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    LOG_TRACE9("(%p, %p, %p, %p, %d, %d)", actor, model, material, render_data, style, on_screen);
    Harness_Hook_renderFaces(model, material, BRT_TRIANGLE);
}

// IDA: void __usercall renderEdges(br_actor *actor@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, int on_screen)
void renderEdges(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", actor, model, material, render_data, style, on_screen);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall renderPoints(br_actor *actor@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, int on_screen)
void renderPoints(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", actor, model, material, render_data, style, on_screen);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall nullRender(br_actor *actor@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, int on_screen)
void nullRender(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", actor, model, material, render_data, style, on_screen);
}

// IDA: br_model* __usercall makeMeshFromBounds@<EAX>(br_bounds *b@<EAX>)
br_model* makeMeshFromBounds(br_bounds* b) {
    int i;
    LOG_TRACE("(%p)", b);

    // 1. Modify vertex points
    bounds_vertices[0].p.v[0] = b->min.v[0];
    bounds_vertices[0].p.v[1] = b->min.v[1];
    bounds_vertices[0].p.v[2] = b->min.v[2];

    bounds_vertices[1].p.v[0] = b->min.v[0];
    bounds_vertices[1].p.v[1] = b->min.v[1];
    bounds_vertices[1].p.v[2] = b->max.v[2];

    bounds_vertices[2].p.v[0] = b->min.v[0];
    bounds_vertices[2].p.v[1] = b->max.v[1];
    bounds_vertices[2].p.v[2] = b->min.v[2];

    bounds_vertices[3].p.v[0] = b->min.v[0];
    bounds_vertices[3].p.v[1] = b->max.v[1];
    bounds_vertices[3].p.v[2] = b->max.v[2];

    bounds_vertices[4].p.v[0] = b->max.v[0];
    bounds_vertices[4].p.v[1] = b->min.v[1];
    bounds_vertices[4].p.v[2] = b->min.v[2];

    bounds_vertices[5].p.v[0] = b->max.v[0];
    bounds_vertices[5].p.v[1] = b->min.v[1];
    bounds_vertices[5].p.v[2] = b->max.v[2];

    bounds_vertices[6].p.v[0] = b->max.v[0];
    bounds_vertices[6].p.v[1] = b->max.v[1];
    bounds_vertices[6].p.v[2] = b->min.v[2];

    bounds_vertices[7].p.v[0] = b->max.v[0];
    bounds_vertices[7].p.v[1] = b->max.v[1];
    bounds_vertices[7].p.v[2] = b->max.v[2];

    // 2. Modify d in the face equation (a*x+b*y+c*y+c=0)
    for (i = 0; i < BR_ASIZE(bounds_faces)/4; i++) {
        bounds_faces[2*i+0].eqn.v[3] = b->min.v[i];
        bounds_faces[2*i+1].eqn.v[3] = b->min.v[i];
        bounds_faces[2*i+6].eqn.v[3] = b->max.v[i];
        bounds_faces[2*i+7].eqn.v[3] = b->max.v[i];
    }

    // 3. Copy bounds
    bounds_model.bounds = *b;

    return &bounds_model;
}

// IDA: void __usercall boundingBoxRenderPoints(br_actor *actor@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, int on_screen)
void boundingBoxRenderPoints(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", actor, model, material, render_data, style, on_screen);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall boundingBoxRenderEdges(br_actor *actor@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, int on_screen)
void boundingBoxRenderEdges(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", actor, model, material, render_data, style, on_screen);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall boundingBoxRenderFaces(br_actor *actor@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, int on_screen)
void boundingBoxRenderFaces(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", actor, model, material, render_data, style, on_screen);
    NOT_IMPLEMENTED();
}
