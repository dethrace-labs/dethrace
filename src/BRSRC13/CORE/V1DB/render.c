#include "render.h"
#include "harness.h"

char rscid[55];

void BrDbModelRender(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen, int use_custom) {
    br_int_32 count;
    br_token_value tv[5];
    NOT_IMPLEMENTED();
}

br_uint_32 BrOnScreenCheck(br_bounds3* bounds) {
    br_token r;
    NOT_IMPLEMENTED();
}

br_uint_16 prependActorTransform(br_actor* ap, br_uint_16 t) {
    br_matrix34 mt;
    NOT_IMPLEMENTED();
}

br_uint_16 prependMatrix(br_matrix34* mat, br_uint_16 mat_t, br_uint_16 t) {
    br_matrix34 mt;
    NOT_IMPLEMENTED();
}

void actorRender(br_actor* ap, br_model* model, br_material* material, void* render_data, br_uint_8 style, br_uint_16 t) {
    br_material* this_material;
    br_model* this_model;
    void* this_render_data;
    br_actor* a;
    br_token s;
    NOT_IMPLEMENTED();
}

void actorRenderOnScreen(br_actor* ap, br_model* model, br_material* material, void* render_data, br_uint_8 style, br_uint_16 t) {
    br_material* this_material;
    br_model* this_model;
    void* this_render_data;
    br_actor* a;
    NOT_IMPLEMENTED();
}

void sceneRenderWorld(br_actor* world) {
    br_model* model;
    br_material* material;
    void* render_data;
    br_uint_8 style;
    br_actor* a;
    NOT_IMPLEMENTED();
}

void sceneRenderAdd(br_actor* tree) {
    br_material* material;
    br_model* model;
    void* render_data;
    br_uint_8 style;
    br_actor* a;
    br_int_32 t;
    br_matrix34 m;
    NOT_IMPLEMENTED();
}

void BrDbSceneRenderBegin(br_actor* world, br_actor* camera) {
    br_matrix34 tfm;
    br_matrix4 vtos;
    br_actor* a;
    int i;
    br_token vtos_type;
    br_uint_32 dummy;
    NOT_IMPLEMENTED();
}

br_renderbounds_cbfn* BrDbSetRenderBoundsCallback(br_renderbounds_cbfn* new_cbfn) {
    br_renderbounds_cbfn* old_cbfn;
    NOT_IMPLEMENTED();
}

void SetOrigin(br_pixelmap* buffer) {
    NOT_IMPLEMENTED();
}

void SetViewport(br_pixelmap* buffer) {
    NOT_IMPLEMENTED();
}

void BrZbSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer) {
    NOT_IMPLEMENTED();
}

void BrZbSceneRenderAdd(br_actor* tree) {
    NOT_IMPLEMENTED();
}

void BrZbSceneRenderEnd() {
    NOT_IMPLEMENTED();
}

void BrZbSceneRender(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer) {
    br_actor* a;
    NOT_IMPLEMENTED();
}

void BrZsSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer) {
    br_camera* camera_data;
    NOT_IMPLEMENTED();
}

void BrZsSceneRenderAdd(br_actor* tree) {
    NOT_IMPLEMENTED();
}

void BrZsSceneRenderEnd() {
    NOT_IMPLEMENTED();
}

void BrZsSceneRender(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer) {
    br_actor* a;
    NOT_IMPLEMENTED();
}

br_primitive_cbfn* BrZsPrimitiveCallbackSet(br_primitive_cbfn* new_cbfn) {
    br_primitive_cbfn* old_cbfn;
    NOT_IMPLEMENTED();
}

void BrZbModelRender(br_actor* actor, br_model* model, br_material* material, br_uint_8 style, int on_screen, int use_custom) {
    NOT_IMPLEMENTED();
}

void BrZsModelRender(br_actor* actor, br_model* model, br_material* material, br_order_table* order_table, br_uint_8 style, int on_screen, int use_custom) {
    NOT_IMPLEMENTED();
}

br_renderbounds_cbfn* BrZbRenderBoundsCallbackSet(br_renderbounds_cbfn* new_cbfn) {
    NOT_IMPLEMENTED();
}

br_renderbounds_cbfn* BrZsRenderBoundsCallbackSet(br_renderbounds_cbfn* new_cbfn) {
    NOT_IMPLEMENTED();
}
