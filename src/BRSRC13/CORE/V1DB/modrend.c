#include "modrend.h"
#include "harness.h"

br_colour bounds_colours[12];
fmt_vertex bounds_vertices[8];
v11face bounds_faces[12];
void (*RenderStyleCalls[8])(br_actor*, br_model*, br_material*, void*, br_uint_8, int);
v11model bounds_prepared;
br_model bounds_model;
v11group bounds_face_groups[1];
char rscid[49];

// Offset: 12
// Size: 226
void renderFaces(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    NOT_IMPLEMENTED();
}

// Offset: 250
// Size: 143
void renderEdges(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    NOT_IMPLEMENTED();
}

// Offset: 406
// Size: 143
void renderPoints(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    NOT_IMPLEMENTED();
}

// Offset: 560
// Size: 42
void nullRender(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    NOT_IMPLEMENTED();
}

// Offset: 621
// Size: 467
br_model* makeMeshFromBounds(br_bounds* b) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 1112
// Size: 79
void boundingBoxRenderPoints(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    NOT_IMPLEMENTED();
}

// Offset: 1214
// Size: 79
void boundingBoxRenderEdges(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    NOT_IMPLEMENTED();
}

// Offset: 1316
// Size: 79
void boundingBoxRenderFaces(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    NOT_IMPLEMENTED();
}
