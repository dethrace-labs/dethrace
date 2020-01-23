#include <stdio.h>

#include "CORE/FW/brlists.h"
#include "CORE/FW/fwsetup.h"
#include "CORE/FW/resource.h"
#include "CORE/MATH/matrix34.h"
#include "CORE/V1DB/dbsetup.h"
#include "CORE/V1DB/enables.h"
#include "actsupt.h"
#include "debug.h"

char rscid[53];

// Offset: 12
// Size: 158
br_uint_32 BrActorEnum(br_actor* parent, br_actor_enum_cbfn* callback, void* arg) {
    br_actor* a;
    br_actor* next;
    br_uint_32 r;
}

// Offset: 188
// Size: 321
br_uint_32 BrActorSearchMany(br_actor* root, char* pattern, br_actor** actors, int max) {
    br_actor* a;
    char* sub;
    int n;
    int remaining;
}

// Offset: 523
// Size: 103
br_actor* BrActorSearch(br_actor* root, char* pattern) {
    br_actor* a;
}

// Offset: 640
// Size: 117
// EAX: a
// EDX: d
void RenumberActor(br_actor* a, int d) {
    br_actor* ac;
}

// Offset: 768
// Size: 230
br_actor* BrActorAdd(br_actor* parent, br_actor* a) {
    br_actor* ac;
}

// Offset: 1012
// Size: 177
br_actor* BrActorRemove(br_actor* a) {
    br_actor* ac;
}

// Offset: 1203
// Size: 167
void BrActorRelink(br_actor* parent, br_actor* a) {
    br_matrix34 mat;
}

// Offset: 1386
// Size: 503
br_actor* BrActorAllocate(br_uint_8 type, void* type_data) {
    br_actor* a;
    br_light* light;
    br_camera* camera;
    br_bounds* bounds;
    br_vector4* clip_plane;
    LOG_TRACE("(%d, %p)", type, type_data);

    a = BrResAllocate(v1db.res, sizeof(br_actor), BR_MEMORY_ACTOR);
    BrSimpleNewList(&a->children);
    a->type = type;
    a->depth = 0;
    a->t.type = 0;
    a->type_data = NULL;
    BrMatrix34Identity(&a->t.t.mat);

    if (type_data) {
        a->type_data = type_data;
    } else {
        switch (type) {
        case BR_ACTOR_NONE:
            break;
        case BR_ACTOR_LIGHT:
            light = BrResAllocate(a, sizeof(br_light), BR_MEMORY_LIGHT);
            a->type_data = light;
            light->type = BR_LIGHT_DIRECT;
            light->colour = BR_COLOUR_RGB(255, 255, 255);
            light->attenuation_c = 1.0f;
            light->cone_outer = BR_ANGLE_DEG(15);
            light->cone_inner = BR_ANGLE_DEG(10);
            break;
        case BR_ACTOR_CAMERA:
            camera = (br_camera*)BrResAllocate(a, sizeof(br_camera), BR_MEMORY_CAMERA);
            a->type_data = camera;
            camera->type = BR_CAMERA_PERSPECTIVE_FOV;
            camera->field_of_view = BR_ANGLE_DEG(45.0f);
            camera->hither_z = 1.0f;
            camera->yon_z = 10.0f;
            camera->aspect = 1.0f;
            break;
        case BR_ACTOR_BOUNDS:
        case BR_ACTOR_BOUNDS_CORRECT:
            bounds = BrResAllocate(a, sizeof(br_bounds), BR_MEMORY_CLIP_PLANE);
            a->type_data = bounds;
            break;
        case BR_ACTOR_CLIP_PLANE:
            clip_plane = BrResAllocate(a, sizeof(br_vector4), BR_MEMORY_CLIP_PLANE);
            clip_plane->v[0] = 0;
            clip_plane->v[1] = 0;
            clip_plane->v[2] = 1.0;
            clip_plane->v[3] = 0;
            a->type_data = clip_plane;
        default:
            LOG_WARN("Warning: Unknown type %d for BrActorAllocate", type);
        }
    }
    return a;
}

// Offset: 1907
// Size: 152
// EAX: a
void InternalActorFree(br_actor* a) {
    while (a->children) {
        BrSimpleRemove((br_simple_node*)a->children);
        InternalActorFree(a);
    }
    BrActorEnableCheck(a);
    BrResFree(a);
}

// Offset: 2071
// Size: 103
void BrActorFree(br_actor* a) {
    while (a->children != NULL) {
        BrSimpleRemove(a->children);
        InternalActorFree(a);
    }
    BrActorEnableCheck(a);
    BrResFree(a);
}

// Offset: 2186
// Size: 283
// EAX: a
// EDX: world
// EBX: m
br_boolean ActorToRoot(br_actor* a, br_actor* world, br_matrix34* m) {
}

// Offset: 2486
// Size: 339
// EAX: a
// EDX: world
// EBX: m
// ECX: type
br_boolean ActorToRootTyped(br_actor* a, br_actor* world, br_matrix34* m, br_int_32* type) {
    br_int_32 t;
}

// Offset: 2847
// Size: 572
// EAX: mat
// EDX: field_of_view
void Matrix4PerspectiveNew(br_matrix4* mat, br_angle field_of_view, br_scalar aspect, br_scalar hither, br_scalar yon, br_scalar origin_x, br_scalar origin_y) {
    br_scalar scale;
}

// Offset: 3441
// Size: 944
// EAX: mat
// EDX: camera
br_token CameraToScreenMatrix4(br_matrix4* mat, br_actor* camera) {
    br_camera* camera_type;
    br_matrix34 mat34;
}

// Offset: 4408
// Size: 866
br_uint_16 BrActorToActorMatrix34(br_matrix34* m, br_actor* a, br_actor* b) {
    br_matrix34 mata;
    br_matrix34 matb;
    br_matrix34 matc;
    br_uint_8 at;
    br_uint_8 bt;
}

// Offset: 5297
// Size: 178
void BrActorToScreenMatrix4(br_matrix4* m, br_actor* a, br_actor* camera) {
    br_matrix34 a2c;
}

// Offset: 5497
// Size: 409
// EAX: d
// EDX: s
// EBX: m
void BrMatrix34ApplyBounds(br_bounds* d, br_bounds* s, br_matrix34* m) {
    int i;
    int j;
    br_scalar a;
    br_scalar b;
}

// Offset: 5920
// Size: 397
// EAX: dest
// EDX: ap
// EBX: model
void ActorToBounds(br_bounds* dest, br_actor* ap, br_model* model) {
    br_actor* a;
    br_bounds new;
    br_matrix34 m2v;
    int i;
}

// Offset: 6333
// Size: 310
br_bounds* BrActorToBounds(br_bounds* b, br_actor* ap) {
    br_matrix34 m2v;
    br_model* model;
    br_actor* a;
}
