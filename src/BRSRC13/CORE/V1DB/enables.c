#include "enables.h"
#include "CORE/FW/diag.h"
#include "CORE/FW/resource.h"
#include "CORE/MATH/matrix34.h"
#include "CORE/MATH/transfrm.h"
#include "CORE/V1DB/actsupt.h"
#include "dbsetup.h"
#include "harness/trace.h"
#include <stddef.h>

// IDA: void __usercall actorEnable(br_v1db_enable *e@<EAX>, br_actor *a@<EDX>)
void actorEnable(br_v1db_enable* e, br_actor* a) {
    int i;
    LOG_TRACE("(%p, %p)", e, a);

    if (!e->enabled) {
        e->enabled = BrResAllocate(v1db.res, e->max * sizeof(*e->enabled), BR_MEMORY_ENABLED_ACTORS);
    }

    for (i = 0; i < e->max; i++) {
        if (e->enabled[i] == a) {
            return;
        }
    }

    for (i = 0; i < e->max; i++) {
        if (!e->enabled[i]) {
            e->enabled[i] = a;
            e->count++;
            return;
        }
    }

    BrFailure("too many enabled %ss", e->name);
}

// IDA: void __usercall actorDisable(br_v1db_enable *e@<EAX>, br_actor *a@<EDX>)
void actorDisable(br_v1db_enable* e, br_actor* a) {
    int i;

    if (!e->enabled) {
        return;
    }

    for (i = 0; i < e->max; i++) {
        if (e->enabled[i] == a) {
            e->enabled[i] = NULL;
            e->count--;
            return;
        }
    }
}

// IDA: void __cdecl BrLightEnable(br_actor *l)
void BrLightEnable(br_actor* l) {
    LOG_TRACE("(%p)", l);

    actorEnable(&v1db.enabled_lights, l);
}

// IDA: void __cdecl BrLightDisable(br_actor *l)
void BrLightDisable(br_actor* l) {
    LOG_TRACE("(%p)", l);

    actorDisable(&v1db.enabled_lights, l);
}

// IDA: void __cdecl BrClipPlaneEnable(br_actor *c)
void BrClipPlaneEnable(br_actor* c) {
    LOG_TRACE("(%p)", c);

    actorEnable(&v1db.enabled_clip_planes, c);
}

// IDA: void __cdecl BrClipPlaneDisable(br_actor *c)
void BrClipPlaneDisable(br_actor* c) {

    actorDisable(&v1db.enabled_clip_planes, c);
}

// IDA: void __cdecl BrHorizonPlaneEnable(br_actor *h)
void BrHorizonPlaneEnable(br_actor* h) {
    LOG_TRACE("(%p)", h);

    actorEnable(&v1db.enabled_horizon_planes, h);
}

// IDA: void __cdecl BrHorizonPlaneDisable(br_actor *h)
void BrHorizonPlaneDisable(br_actor* h) {
    LOG_TRACE("(%p)", h);

    actorDisable(&v1db.enabled_horizon_planes, h);
}

// IDA: br_actor* __cdecl BrEnvironmentSet(br_actor *a)
br_actor* BrEnvironmentSet(br_actor* a) {
    br_actor* old_a;
    LOG_TRACE("(%p)", a);

    old_a = v1db.enabled_environment;
    v1db.enabled_environment = a;
    return old_a;
}

// IDA: br_boolean __usercall setupView@<EAX>(br_matrix34 *view_to_this@<EAX>, br_matrix34 *this_to_view@<EDX>, br_matrix34 *world_to_view@<EBX>, br_int_32 w2vt@<ECX>, br_actor *world, br_actor *a)
br_boolean setupView(br_matrix34* view_to_this, br_matrix34* this_to_view, br_matrix34* world_to_view, br_int_32 w2vt, br_actor* world, br_actor* a) {
    br_matrix34 this_to_world;
    br_int_32 root_t;
    br_int_32 t;
    LOG_TRACE("(%p, %p, %p, %d, %p, %p)", view_to_this, this_to_view, world_to_view, w2vt, world, a);
    if (ActorToRootTyped(a, world, &this_to_world, &root_t) == 0) {
        return 0;
    }
    BrMatrix34Mul(this_to_view, &this_to_world, world_to_view);
    t = BrTransformCombineTypes(root_t, w2vt);
    if (BrTransformTypeIsLP(t) != 0) {
        BrMatrix34LPInverse(view_to_this, this_to_view);
    } else {
        BrMatrix34Inverse(view_to_this, this_to_view);
    }
    return 1;
}

// IDA: void __usercall BrSetupLights(br_actor *world@<EAX>, br_matrix34 *world_to_view@<EDX>, br_int_32 w2vt@<EBX>)
void BrSetupLights(br_actor* world, br_matrix34* world_to_view, br_int_32 w2vt) {
    br_matrix34 this_to_view;
    br_matrix34 view_to_this;
    int light_part;
    int i;
    br_token_value tv[16];
    br_token_value* tvp;
    br_vector3 view_position;
    br_vector3 view_direction;
    br_light* light;
    LOG_TRACE("(%p, %p, %d)", world, world_to_view, w2vt);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall BrSetupClipPlanes(br_actor *world@<EAX>, br_matrix34 *world_to_view@<EDX>, br_int_32 w2vt@<EBX>, br_matrix4 *view_to_screen@<ECX>)
void BrSetupClipPlanes(br_actor* world, br_matrix34* world_to_view, br_int_32 w2vt, br_matrix4* view_to_screen) {
    br_matrix34 this_to_view;
    br_matrix34 view_to_this;
    br_matrix4 screen_to_view;
    br_matrix4 tmp4;
    br_matrix4 screen_to_this;
    int light_part;
    int i;
    br_int_32 clip_part;
    br_token_value tv[3];
    br_vector4 sp;
    LOG_TRACE("(%p, %p, %d, %p)", world, world_to_view, w2vt, view_to_screen);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall BrSetupEnvironment(br_actor *world@<EAX>, br_matrix34 *world_to_view@<EDX>, br_int_32 w2vt@<EBX>)
void BrSetupEnvironment(br_actor* world, br_matrix34* world_to_view, br_int_32 w2vt) {
    br_matrix34 view_to_this;
    br_matrix34 this_to_view;
    br_token h;
    LOG_TRACE("(%p, %p, %d)", world, world_to_view, w2vt);

    h = BRT_DONT_CARE;
    if (v1db.enabled_environment != NULL) {
        if (v1db.enabled_environment == world) {
            if (BrTransformTypeIsLP(w2vt) != 0) {
                BrMatrix34LPInverse(&view_to_this, world_to_view);
            } else {
                BrMatrix34Inverse(&view_to_this, world_to_view);
            }
            h = BRT_NONE;
        } else {
            if (setupView(&view_to_this, &this_to_view, world_to_view, w2vt, world, v1db.enabled_environment) != 0) {
                h = BRT_NONE;
            }
        }
    }
    if (h != BRT_DONT_CARE) {
        v1db.renderer->dispatch->_partSet(v1db.renderer, BRT_MATRIX, 0, BRT_VIEW_TO_ENVIRONMENT_M34_F, &view_to_this);
    }
    v1db.renderer->dispatch->_partSet(v1db.renderer, BRT_MATRIX, 0, BRT_VIEW_TO_ENVIRONMENT_HINT_T, h);
}

// IDA: void __usercall BrSetupHorizons(br_actor *world@<EAX>, br_matrix34 *world_to_view@<EDX>, br_int_32 w2vt@<EBX>)
void BrSetupHorizons(br_actor* world, br_matrix34* world_to_view, br_int_32 w2vt) {
    LOG_TRACE("(%p, %p, %d)", world, world_to_view, w2vt);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall BrActorEnableCheck(br_actor *a@<EAX>)
void BrActorEnableCheck(br_actor* a) {
    if (v1db.enabled_environment == a) {
        v1db.enabled_environment = NULL;
    }
    if (a->type == BR_ACTOR_LIGHT) {
        actorDisable(&v1db.enabled_lights, a);
    } else if (a->type == BR_ACTOR_HORIZONTAL_PLANE) {
        actorDisable(&v1db.enabled_horizon_planes, a);
    } else if (a->type <= BR_ACTOR_CLIP_PLANE) {
        actorDisable(&v1db.enabled_clip_planes, a);
    }
}
