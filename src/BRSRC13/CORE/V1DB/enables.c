#include "enables.h"
#include "dbsetup.h"
#include "harness.h"
#include <stddef.h>

char rscid[51];

void actorEnable(br_v1db_enable* e, br_actor* a) {
    int i;
    NOT_IMPLEMENTED();
}

void actorDisable(br_v1db_enable* e, br_actor* a) {
    int i;
    STUB();
}

void BrLightEnable(br_actor* l) {
    NOT_IMPLEMENTED();
}

void BrLightDisable(br_actor* l) {
    NOT_IMPLEMENTED();
}

void BrClipPlaneEnable(br_actor* c) {
    NOT_IMPLEMENTED();
}

void BrClipPlaneDisable(br_actor* c) {
    actorDisable(&v1db.enabled_clip_planes, c);
}

void BrHorizonPlaneEnable(br_actor* h) {
    NOT_IMPLEMENTED();
}

void BrHorizonPlaneDisable(br_actor* h) {
    NOT_IMPLEMENTED();
}

br_actor* BrEnvironmentSet(br_actor* a) {
    br_actor* old_a;
    old_a = v1db.enabled_environment;
    v1db.enabled_environment = a;
    return old_a;
    NOT_IMPLEMENTED();
}

br_boolean setupView(br_matrix34* view_to_this, br_matrix34* this_to_view, br_matrix34* world_to_view, br_int_32 w2vt, br_actor* world, br_actor* a) {
    br_matrix34 this_to_world;
    br_int_32 root_t;
    br_int_32 t;
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

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
    NOT_IMPLEMENTED();
}

void BrSetupEnvironment(br_actor* world, br_matrix34* world_to_view, br_int_32 w2vt) {
    br_matrix34 view_to_this;
    br_matrix34 this_to_view;
    br_token h;
    NOT_IMPLEMENTED();
}

void BrSetupHorizons(br_actor* world, br_matrix34* world_to_view, br_int_32 w2vt) {
    NOT_IMPLEMENTED();
}

void BrActorEnableCheck(br_actor* a) {
    if (v1db.enabled_environment == a) {
        v1db.enabled_environment = NULL;
    }
    if (a->type == BR_ACTOR_LIGHT) {
        actorDisable(&v1db.enabled_lights, a);
    } else if (a->type == BR_ACTOR_MAX) {
        actorDisable(&v1db.enabled_horizon_planes, a);
    } else if (a->type <= BR_ACTOR_CLIP_PLANE) {
        actorDisable(&v1db.enabled_clip_planes, a);
    }
}
