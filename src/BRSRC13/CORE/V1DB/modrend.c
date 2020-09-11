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

void renderFaces(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    NOT_IMPLEMENTED();
}

void renderEdges(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    NOT_IMPLEMENTED();
}

void renderPoints(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    NOT_IMPLEMENTED();
}

void nullRender(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    NOT_IMPLEMENTED();
}

br_model* makeMeshFromBounds(br_bounds* b) {
    int i;
    NOT_IMPLEMENTED();
}

void boundingBoxRenderPoints(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    NOT_IMPLEMENTED();
}

void boundingBoxRenderEdges(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    NOT_IMPLEMENTED();
}

void boundingBoxRenderFaces(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen) {
    NOT_IMPLEMENTED();
}
