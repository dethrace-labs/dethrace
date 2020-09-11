#include "enables.h"
#include "dbsetup.h"
#include "harness.h"
#include <stddef.h>

char rscid[51];

// Offset: 12
// Size: 295
void actorEnable(br_v1db_enable* e, br_actor* a) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 320
// Size: 191
void actorDisable(br_v1db_enable* e, br_actor* a) {
    int i;
    STUB();
}

// Offset: 525
// Size: 71
void BrLightEnable(br_actor* l) {
    NOT_IMPLEMENTED();
}

// Offset: 611
// Size: 71
void BrLightDisable(br_actor* l) {
    NOT_IMPLEMENTED();
}

// Offset: 700
// Size: 71
void BrClipPlaneEnable(br_actor* c) {
    NOT_IMPLEMENTED();
}

// Offset: 790
// Size: 71
void BrClipPlaneDisable(br_actor* c) {
    actorDisable(&v1db.enabled_clip_planes, c);
}

// Offset: 882
// Size: 71
void BrHorizonPlaneEnable(br_actor* h) {
    NOT_IMPLEMENTED();
}

// Offset: 975
// Size: 74
void BrHorizonPlaneDisable(br_actor* h) {
    NOT_IMPLEMENTED();
}

// Offset: 1066
// Size: 55
br_actor* BrEnvironmentSet(br_actor* a) {
    br_actor* old_a;
    old_a = v1db.enabled_environment;
    v1db.enabled_environment = a;
    return old_a;
    NOT_IMPLEMENTED();
}

// Offset: 1131
// Size: 284
br_boolean setupView(br_matrix34* view_to_this, br_matrix34* this_to_view, br_matrix34* world_to_view, br_int_32 w2vt, br_actor* world, br_actor* a) {
    br_matrix34 this_to_world;
    br_int_32 root_t;
    br_int_32 t;
    NOT_IMPLEMENTED();
}

// Offset: 1429
// Size: 1051
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

// Offset: 2498
// Size: 535
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

// Offset: 3052
// Size: 293
void BrSetupEnvironment(br_actor* world, br_matrix34* world_to_view, br_int_32 w2vt) {
    br_matrix34 view_to_this;
    br_matrix34 this_to_view;
    br_token h;
    NOT_IMPLEMENTED();
}

// Offset: 3361
// Size: 39
void BrSetupHorizons(br_actor* world, br_matrix34* world_to_view, br_int_32 w2vt) {
    NOT_IMPLEMENTED();
}

// Offset: 3419
// Size: 136
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
