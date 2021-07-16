#include "render.h"
#include "dbsetup.h"
#include "harness_hooks.h"
#include "harness_trace.h"
#include "matrix34.h"
#include "modrend.h"
#include "transfrm.h"
#include <string.h>

char rscid[55];

#define RendererModelMul(self, a1) self->dispatch->_modelMulF(self, a1);
#define RendererStatePush(self, a1) self->dispatch->_statePush(self, a1);
#define RendererStatePop(self, a1) self->dispatch->_statePop(self, a1);

// IDA: void __cdecl BrDbModelRender(br_actor *actor, br_model *model, br_material *material, void *render_data, br_uint_8 style, int on_screen, int use_custom)
void BrDbModelRender(br_actor* actor, br_model* model, br_material* material, void* render_data, br_uint_8 style, int on_screen, int use_custom) {
    br_int_32 count;
    br_token_value tv[5];
    LOG_TRACE("(%p, %p, %p, %p, %d, %d, %d)", actor, model, material, render_data, style, on_screen, use_custom);

    // if (strcmp(model->identifier, "NEWEAGLE.DAT") == 0) {
    //     LOG_DEBUG("rendering eagle!");
    // } else {
    //     return;
    // }

    v1db.model_to_screen_valid = 0;
    if (use_custom && (model->flags & BR_MODF_CUSTOM)) {
        TELL_ME_IF_WE_PASS_THIS_WAY();
        model->custom(actor, model, material, render_data, style, on_screen);
        return;
    }

    if (model->prepared == NULL && model->stored == NULL) {
        TELL_ME_IF_WE_PASS_THIS_WAY();
        // TODO:    BrFailure("Tried to render un-prepared model %s", model->identifier ? model->identifier : "<NULL>");
    }

    RenderStyleCalls[style](actor, model, material, render_data, style, on_screen);
}

// IDA: br_uint_32 __cdecl BrOnScreenCheck(br_bounds3 *bounds)
br_uint_32 BrOnScreenCheck(br_bounds3* bounds) {
    br_token r;
    LOG_TRACE("(%p)", bounds);

    return 1;
}

// IDA: br_uint_16 __usercall prependActorTransform@<AX>(br_actor *ap@<EAX>, br_uint_16 t@<EDX>)
br_uint_16 prependActorTransform(br_actor* ap, br_uint_16 t) {
    br_matrix34 mt;
    LOG_TRACE("(%p, %d)", ap, t);

    if (BrTransformTypeIsMatrix34(ap->t.type)) {
        RendererModelMul(v1db.renderer, (void*)&ap->t.t.mat);
    } else {
        BrTransformToMatrix34(&mt, &ap->t);
        RendererModelMul(v1db.renderer, (void*)&mt);
    }
    t = BrTransformCombineTypes(t, ap->t.type);

    // RendererPartSet(v1db.renderer, BRT_MATRIX, 0,
    //     BRT_MODEL_TO_VIEW_HINT_T, BrTransformTypeIsLP(t) ? BRT_LENGTH_PRESERVING : BRT_NONE);

    return t;
}

// IDA: br_uint_16 __usercall prependMatrix@<AX>(br_matrix34 *mat@<EAX>, br_uint_16 mat_t@<EDX>, br_uint_16 t@<EBX>)
br_uint_16 prependMatrix(br_matrix34* mat, br_uint_16 mat_t, br_uint_16 t) {
    br_matrix34 mt;
    LOG_TRACE("(%p, %d, %d)", mat, mat_t, t);

    RendererModelMul(v1db.renderer, (void*)mat);

    t = BrTransformCombineTypes(t, mat_t);

    return t;
}

// IDA: void __usercall actorRender(br_actor *ap@<EAX>, br_model *model@<EDX>, br_material *material@<EBX>, void *render_data@<ECX>, br_uint_8 style, br_uint_16 t)
void actorRender(br_actor* ap, br_model* model, br_material* material, void* render_data, br_uint_8 style, br_uint_16 t) {
    br_material* this_material;
    br_model* this_model;
    void* this_render_data;
    br_actor* a;
    br_token s;
    LOG_TRACE("(%p, %p, %p, %p, %d, %d)", ap, model, material, render_data, style, t);

    if (ap->children == NULL && ap->type != BR_ACTOR_MODEL)
        return;

    if (ap->render_style != BR_RSTYLE_DEFAULT)
        style = ap->render_style;

    if (style == BR_RSTYLE_NONE)
        return;

    this_material = ap->material ? ap->material : material;
    this_model = ap->model ? ap->model : model;
    this_render_data = ap->render_data ? ap->render_data : render_data;

    if (ap->t.type == BR_TRANSFORM_IDENTITY) {

        switch (ap->type) {

        case BR_ACTOR_MODEL:

            if ((s = BrOnScreenCheck(&this_model->bounds)) != BRT_REJECT) {
                BrDbModelRender(ap, this_model, this_material, this_render_data, style, s, 1);
            }
            break;

        case BR_ACTOR_BOUNDS:

            if (BrOnScreenCheck(ap->type_data) == BRT_REJECT)
                return;
            break;

        case BR_ACTOR_BOUNDS_CORRECT:

            switch (BrOnScreenCheck(ap->type_data)) {

            case BRT_ACCEPT:

                BR_FOR_SIMPLELIST(&ap->children, a)
                actorRenderOnScreen(a, this_model, this_material, this_render_data, style, t);

            case BRT_REJECT:

                return;
            }
        }

        BR_FOR_SIMPLELIST(&ap->children, a)
        actorRender(a, this_model, this_material, this_render_data, style, t);

        return;
    }

    RendererStatePush(v1db.renderer, BR_STATE_MATRIX);

    t = prependActorTransform(ap, t);

    switch (ap->type) {

    case BR_ACTOR_MODEL:
        if ((s = BrOnScreenCheck(&this_model->bounds)) != BRT_REJECT) {
            BrDbModelRender(ap, this_model, this_material, this_render_data, style, s, 1);
        }
        break;

    case BR_ACTOR_BOUNDS:
        if (BrOnScreenCheck(ap->type_data) == BRT_REJECT) {
            RendererStatePop(v1db.renderer, BR_STATE_MATRIX);
            return;
        }
        break;

    case BR_ACTOR_BOUNDS_CORRECT:
        switch (BrOnScreenCheck(ap->type_data)) {

        case BRT_ACCEPT:
            BR_FOR_SIMPLELIST(&ap->children, a)
            actorRenderOnScreen(a, this_model, this_material, this_render_data, style, t);

        case BRT_REJECT:
            RendererStatePop(v1db.renderer, BR_STATE_MATRIX);
            return;
        }
    }

    BR_FOR_SIMPLELIST(&ap->children, a)
    actorRender(a, this_model, this_material, this_render_data, style, t);
    RendererStatePop(v1db.renderer, BR_STATE_MATRIX);
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

    material = NULL;
    model = NULL;
    render_data = NULL;
    style = BR_RSTYLE_DEFAULT;

    if (tree->parent == NULL) {
        actorRender(tree,
            v1db.default_model,
            v1db.default_material,
            v1db.default_render_data, BR_RSTYLE_DEFAULT,
            (br_uint_16)v1db.ttype);
        return;
    }

    t = BR_TRANSFORM_IDENTITY;
    BrMatrix34Identity(&m);

    for (a = tree->parent; a; a = a->parent) {
        if (material == NULL && a->material)
            material = a->material;

        if (model == NULL && a->model)
            model = a->model;

        if (render_data == NULL && a->render_data)
            render_data = a->render_data;
        if (a->render_style != BR_RSTYLE_DEFAULT)
            style = a->render_style;

        if (a == v1db.render_root)
            break;

        if (a->t.type != BR_TRANSFORM_IDENTITY) {
            BrMatrix34PostTransform(&m, &a->t);
            t = BrTransformCombineTypes(t, a->t.type);
        }
    }

    if (material == NULL)
        material = v1db.default_material;

    if (model == NULL)
        model = v1db.default_model;

    if (render_data == NULL)
        render_data = v1db.default_render_data;

    if (t == BR_TRANSFORM_IDENTITY) {
        actorRender(tree, model, material, render_data, style, (br_uint_16)v1db.ttype);
    } else {
        RendererStatePush(v1db.renderer, BR_STATE_MATRIX);
        t = prependMatrix(&m, (unsigned short)t, (unsigned short)v1db.ttype);
        actorRender(tree, model, material, render_data, style, (br_uint_16)t);
        RendererStatePop(v1db.renderer, BR_STATE_MATRIX);
    }
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
    //LOG_TRACE("(%p, %p, %p, %p)", world, camera, colour_buffer, depth_buffer);
    Harness_Hook_BrZbSceneRenderBegin(world, camera, colour_buffer, depth_buffer);
}

// IDA: void __cdecl BrZbSceneRenderAdd(br_actor *tree)
void BrZbSceneRenderAdd(br_actor* tree) {
    //LOG_TRACE("(%p)", tree);
    sceneRenderAdd(tree);
}

// IDA: void __cdecl BrZbSceneRenderEnd()
void BrZbSceneRenderEnd() {
    //LOG_TRACE("()");
    Harness_Hook_BrZbSceneRenderEnd();
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
