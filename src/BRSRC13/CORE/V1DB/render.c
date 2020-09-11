#include "render.h"
#include "harness.h"

char rscid[55];

// IDA: void __cdecl BrDbModelRender(br_actor *actor, br_model *model, br_material *material, void *render_data, br_uint_8 style, int on_screen, int use_custom)
void BrDbModelRender(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen, int use_custom) {
    br_int_32 count;
    br_token_value tv[5];
    LOG_TRACE("(%p, %p, %p, %p, %d, %d, %d)", actor, model, material, render_data, style, on_screen, use_custom);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrOnScreenCheck(br_bounds3 *bounds)
br_uint_32 BrOnScreenCheck(br_bounds3* bounds) {
    br_token r;
    LOG_TRACE("(%p)", bounds);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_16 __usercall prependActorTransform@<AX>(br_actor *ap@<EAX>, br_uint_16 t@<EDX>)
br_uint_16 prependActorTransform(br_actor* ap, br_uint_16 t) {
    br_matrix34 mt;
    LOG_TRACE("(%p, %d)", ap, t);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_16 __usercall prependMatrix@<AX>(br_matrix34 *mat@<EAX>, br_uint_16 mat_t@<EDX>, br_uint_16 t@<EBX>)
br_uint_16 prependMatrix(br_matrix34* mat, br_uint_16 mat_t, br_uint_16 t) {
    br_matrix34 mt;
    LOG_TRACE("(%p, %d, %d)", mat, mat_t, t);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall actorRender(br_actor *ap@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, br_uint_16 t)
void actorRender(br_actor* ap, br_model* model, br_material* material, void* render_data, br_uint_8 style, br_uint_16 t) {
    br_material* this_material;
    br_model* this_model;
    void* this_render_data;
    br_actor* a;
    br_token s;
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", ap, model, material, render_data, style, t);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall actorRenderOnScreen(br_actor *ap@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, br_uint_16 t)
void actorRenderOnScreen(br_actor* ap, br_model* model, br_material* material, void* render_data, br_uint_8 style, br_uint_16 t) {
    br_material* this_material;
    br_model* this_model;
    void* this_render_data;
    br_actor* a;
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", ap, model, material, render_data, style, t);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall sceneRenderWorld(br_actor *world@<EAX>)
void sceneRenderWorld(br_actor* world) {
    br_model* model;
    br_material* material;
    void* render_data;
    br_uint_8 style;
    br_actor* a;
    LOG_TRACE("(%p)", world);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall sceneRenderAdd(br_actor *tree@<EAX>)
void sceneRenderAdd(br_actor* tree) {
    br_material* material;
    br_model* model;
    void* render_data;
    br_uint_8 style;
    br_actor* a;
    br_int_32 t;
    br_matrix34 m;
    LOG_TRACE("(%p)", tree);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrDbSceneRenderBegin(br_actor *world, br_actor *camera)
void BrDbSceneRenderBegin(br_actor* world, br_actor* camera) {
    br_matrix34 tfm;
    br_matrix4 vtos;
    br_actor* a;
    int i;
    br_token vtos_type;
    br_uint_32 dummy;
    LOG_TRACE("(%p, %p)", world, camera);
    NOT_IMPLEMENTED();
}

// IDA: br_renderbounds_cbfn* __cdecl BrDbSetRenderBoundsCallback(br_renderbounds_cbfn *new_cbfn)
br_renderbounds_cbfn* BrDbSetRenderBoundsCallback(br_renderbounds_cbfn* new_cbfn) {
    br_renderbounds_cbfn* old_cbfn;
    LOG_TRACE("(%p)", new_cbfn);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetOrigin(br_pixelmap *buffer@<EAX>)
void SetOrigin(br_pixelmap* buffer) {
    LOG_TRACE("(%p)", buffer);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SetViewport(br_pixelmap *buffer@<EAX>)
void SetViewport(br_pixelmap* buffer) {
    LOG_TRACE("(%p)", buffer);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrZbSceneRenderBegin(br_actor *world, br_actor *camera, br_pixelmap *colour_buffer, br_pixelmap *depth_buffer)
void BrZbSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer) {
    LOG_TRACE("(%p, %p, %p, %p)", world, camera, colour_buffer, depth_buffer);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrZbSceneRenderAdd(br_actor *tree)
void BrZbSceneRenderAdd(br_actor* tree) {
    LOG_TRACE("(%p)", tree);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrZbSceneRenderEnd()
void BrZbSceneRenderEnd() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrZbSceneRender(br_actor *world, br_actor *camera, br_pixelmap *colour_buffer, br_pixelmap *depth_buffer)
void BrZbSceneRender(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer, br_pixelmap* depth_buffer) {
    br_actor* a;
    LOG_TRACE("(%p, %p, %p, %p)", world, camera, colour_buffer, depth_buffer);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrZsSceneRenderBegin(br_actor *world, br_actor *camera, br_pixelmap *colour_buffer)
void BrZsSceneRenderBegin(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer) {
    br_camera* camera_data;
    LOG_TRACE("(%p, %p, %p)", world, camera, colour_buffer);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrZsSceneRenderAdd(br_actor *tree)
void BrZsSceneRenderAdd(br_actor* tree) {
    LOG_TRACE("(%p)", tree);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrZsSceneRenderEnd()
void BrZsSceneRenderEnd() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrZsSceneRender(br_actor *world, br_actor *camera, br_pixelmap *colour_buffer)
void BrZsSceneRender(br_actor* world, br_actor* camera, br_pixelmap* colour_buffer) {
    br_actor* a;
    LOG_TRACE("(%p, %p, %p)", world, camera, colour_buffer);
    NOT_IMPLEMENTED();
}

// IDA: br_primitive_cbfn* __cdecl BrZsPrimitiveCallbackSet(br_primitive_cbfn *new_cbfn)
br_primitive_cbfn* BrZsPrimitiveCallbackSet(br_primitive_cbfn* new_cbfn) {
    br_primitive_cbfn* old_cbfn;
    LOG_TRACE("(%p)", new_cbfn);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrZbModelRender(br_actor *actor, br_model *model, br_material *material, br_uint_8 style, int on_screen, int use_custom)
void BrZbModelRender(br_actor* actor, br_model* model, br_material* material, br_uint_8 style, int on_screen, int use_custom) {
    LOG_TRACE("(%p, %p, %p, %d, %d, %d)", actor, model, material, style, on_screen, use_custom);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrZsModelRender(br_actor *actor, br_model *model, br_material *material, br_order_table *order_table, br_uint_8 style, int on_screen, int use_custom)
void BrZsModelRender(br_actor* actor, br_model* model, br_material* material, br_order_table* order_table, br_uint_8 style, int on_screen, int use_custom) {
    LOG_TRACE("(%p, %p, %p, %p, %d, %d, %d)", actor, model, material, order_table, style, on_screen, use_custom);
    NOT_IMPLEMENTED();
}

// IDA: br_renderbounds_cbfn* __cdecl BrZbRenderBoundsCallbackSet(br_renderbounds_cbfn *new_cbfn)
br_renderbounds_cbfn* BrZbRenderBoundsCallbackSet(br_renderbounds_cbfn* new_cbfn) {
    LOG_TRACE("(%p)", new_cbfn);
    NOT_IMPLEMENTED();
}

// IDA: br_renderbounds_cbfn* __cdecl BrZsRenderBoundsCallbackSet(br_renderbounds_cbfn *new_cbfn)
br_renderbounds_cbfn* BrZsRenderBoundsCallbackSet(br_renderbounds_cbfn* new_cbfn) {
    LOG_TRACE("(%p)", new_cbfn);
    NOT_IMPLEMENTED();
}
