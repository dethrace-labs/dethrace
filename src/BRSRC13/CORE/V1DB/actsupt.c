#include <stdio.h>

#include "CORE/FW/brlists.h"
#include "CORE/FW/fwsetup.h"
#include "CORE/FW/resource.h"
#include "CORE/MATH/matrix34.h"
#include "CORE/MATH/transfrm.h"
#include "CORE/V1DB/dbsetup.h"
#include "CORE/V1DB/enables.h"
#include "actsupt.h"
#include "harness.h"

char rscid[53];

// IDA: br_uint_32 __cdecl BrActorEnum(br_actor *parent, br_actor_enum_cbfn *callback, void *arg)
br_uint_32 BrActorEnum(br_actor* parent, br_actor_enum_cbfn* callback, void* arg) {
    br_actor* a;
    br_actor* next;
    br_uint_32 r;
    LOG_TRACE("(%p, %p, %p)", parent, callback, arg);

    a = parent->children;

    while (a) {
        next = a->next;

        if ((r = callback(a, arg)))
            return r;

        a = next;
    }

    return 0;
}

// IDA: br_uint_32 __cdecl BrActorSearchMany(br_actor *root, char *pattern, br_actor **actors, int max)
br_uint_32 BrActorSearchMany(br_actor* root, char* pattern, br_actor** actors, int max) {
    br_actor* a;
    char* sub;
    int n;
    int remaining;
    LOG_TRACE("(%p, \"%s\", %p, %d)", root, pattern, actors, max);
    NOT_IMPLEMENTED();
}

// IDA: br_actor* __cdecl BrActorSearch(br_actor *root, char *pattern)
br_actor* BrActorSearch(br_actor* root, char* pattern) {
    br_actor* a;
    LOG_TRACE("(%p, \"%s\")", root, pattern);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RenumberActor(br_actor *a@<EAX>, int d@<EDX>)
void RenumberActor(br_actor* a, int d) {
    br_actor* ac;
    LOG_TRACE("(%p, %d)", a, d);

    ac = a->children;
    a->depth = d;
    while (ac) {
        d++;
        RenumberActor(ac, d);
        ac = ac->next;
    }
}

// IDA: br_actor* __cdecl BrActorAdd(br_actor *parent, br_actor *a)
br_actor* BrActorAdd(br_actor* parent, br_actor* a) {
    br_actor* ac;
    br_actor* ac2;
    LOG_TRACE("(%p, %p)", parent, a);

    BrSimpleAddHead((br_simple_list*)&parent->children, (br_simple_node*)a);
    a->parent = parent;

    a->depth = parent->depth + 1;
    for (ac = a->children; ac != NULL; ac = ac->next) {
        ac->depth = a->depth + 1;
        ac2 = ac->children;
        while (ac2 != NULL) {
            RenumberActor(ac2, a->depth + 2);
            ac2 = ac2->next;
        }
    }
    return a;
}

// IDA: br_actor* __cdecl BrActorRemove(br_actor *a)
br_actor* BrActorRemove(br_actor* a) {
    br_actor* ac;
    LOG_TRACE("(%p)", a);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrActorRelink(br_actor *parent, br_actor *a)
void BrActorRelink(br_actor* parent, br_actor* a) {
    br_matrix34 mat;
    LOG_TRACE("(%p, %p)", parent, a);
    NOT_IMPLEMENTED();
}

// IDA: br_actor* __cdecl BrActorAllocate(br_uint_8 type, void *type_data)
br_actor* BrActorAllocate(br_uint_8 type, void* type_data) {
    br_actor* a;
    br_light* light;
    br_camera* camera;
    br_bounds* bounds;
    br_vector4* clip_plane;
    LOG_TRACE("(%d, %p)", type, type_data);

    a = BrResAllocate(v1db.res, sizeof(br_actor), BR_MEMORY_ACTOR);
    BrSimpleNewList((br_simple_list*)&a->children);
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
            break;
        case BR_ACTOR_MODEL:
            // nothing to do
            break;
        default:
            LOG_WARN("Warning: Unknown type %d for BrActorAllocate", type);
        }
    }
    return a;
}

// IDA: void __usercall InternalActorFree(br_actor *a@<EAX>)
void InternalActorFree(br_actor* a) {
    while (a->children) {
        BrSimpleRemove((br_simple_node*)a->children);
        InternalActorFree(a);
    }
    BrActorEnableCheck(a);
    BrResFree(a);
}

// IDA: void __cdecl BrActorFree(br_actor *a)
void BrActorFree(br_actor* a) {
    while (a->children) {
        BrSimpleRemove((br_simple_node*)a->children);
        InternalActorFree(a);
    }
    BrActorEnableCheck(a);
    BrResFree(a);
}

// IDA: br_boolean __usercall ActorToRoot@<EAX>(br_actor *a@<EAX>, br_actor *world@<EDX>, br_matrix34 *m@<EBX>)
br_boolean ActorToRoot(br_actor* a, br_actor* world, br_matrix34* m) {
    LOG_TRACE("(%p, %p, %p)", a, world, m);
    NOT_IMPLEMENTED();
}

// IDA: br_boolean __usercall ActorToRootTyped@<EAX>(br_actor *a@<EAX>, br_actor *world@<EDX>, br_matrix34 *m@<EBX>, br_int_32 *type@<ECX>)
br_boolean ActorToRootTyped(br_actor* a, br_actor* world, br_matrix34* m, br_int_32* type) {
    br_int_32 t;
    LOG_TRACE("(%p, %p, %p, %p)", a, world, m, type);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall Matrix4PerspectiveNew(br_matrix4 *mat@<EAX>, br_angle field_of_view@<EDX>, br_scalar aspect, br_scalar hither, br_scalar yon, br_scalar origin_x, br_scalar origin_y)
void Matrix4PerspectiveNew(br_matrix4* mat, br_angle field_of_view, br_scalar aspect, br_scalar hither, br_scalar yon, br_scalar origin_x, br_scalar origin_y) {
    br_scalar scale;
    LOG_TRACE("(%p, %d, %f, %f, %f, %f, %f)", mat, field_of_view, aspect, hither, yon, origin_x, origin_y);
    NOT_IMPLEMENTED();
}

// IDA: br_token __usercall CameraToScreenMatrix4@<EAX>(br_matrix4 *mat@<EAX>, br_actor *camera@<EDX>)
br_token CameraToScreenMatrix4(br_matrix4* mat, br_actor* camera) {
    br_camera* camera_type;
    br_matrix34 mat34;
    LOG_TRACE("(%p, %p)", mat, camera);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_16 __cdecl BrActorToActorMatrix34(br_matrix34 *m, br_actor *a, br_actor *b)
br_uint_16 BrActorToActorMatrix34(br_matrix34* m, br_actor* a, br_actor* b) {
    br_matrix34 mata;
    br_matrix34 matb;
    br_matrix34 matc;
    br_uint_8 at;
    br_uint_8 bt;
    LOG_TRACE("(%p, %p, %p)", m, a, b);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrActorToScreenMatrix4(br_matrix4 *m, br_actor *a, br_actor *camera)
void BrActorToScreenMatrix4(br_matrix4* m, br_actor* a, br_actor* camera) {
    br_matrix34 a2c;
    LOG_TRACE("(%p, %p, %p)", m, a, camera);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall BrMatrix34ApplyBounds(br_bounds *d@<EAX>, br_bounds *s@<EDX>, br_matrix34 *m@<EBX>)
void BrMatrix34ApplyBounds(br_bounds* A, br_bounds* B, br_matrix34* C) {
    int i;
    int j;
    br_scalar a;
    br_scalar b;
    LOG_TRACE("(%p, %p, %p)", A, B, C);

    A->min.v[0] = A->max.v[0] = C->m[3][0];
    A->min.v[1] = A->max.v[1] = C->m[3][1];
    A->min.v[2] = A->max.v[2] = C->m[3][2];

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            a = C->m[j][i] * B->min.v[j];
            b = C->m[j][i] * B->max.v[j];
            if (a < b) {
                A->min.v[i] += a;
                A->max.v[i] += b;
            } else {
                A->min.v[i] += b;
                A->max.v[i] += a;
            }
        }
    }
}

// IDA: void __usercall ActorToBounds(br_bounds *dest@<EAX>, br_actor *ap@<EDX>, br_model *model@<EBX>)
void ActorToBounds(br_bounds* dest, br_actor* ap, br_model* model) {
    br_actor* a;
    br_bounds new;
    br_matrix34 m2v;
    int i;
    LOG_TRACE("(%p, %p, %p)", dest, ap, model);

    if (ap->model != NULL)
        model = ap->model;

    m2v = v1db.model_to_view;
    BrMatrix34PreTransform(&v1db.model_to_view, &ap->t);

    if (ap->type == BR_ACTOR_MODEL) {
        BrMatrix34ApplyBounds(&new, &model->bounds, &v1db.model_to_view);

        for (i = 0; i < 3; i++) {
            if (new.min.v[i] < dest->min.v[i])
                dest->min.v[i] = new.min.v[i];

            if (new.max.v[i] > dest->max.v[i])
                dest->max.v[i] = new.max.v[i];
        }
    }

    BR_FOR_SIMPLELIST(&ap->children, a)
    ActorToBounds(dest, a, model);

    v1db.model_to_view = m2v;
}

// IDA: br_bounds* __cdecl BrActorToBounds(br_bounds *b, br_actor *ap)
br_bounds* BrActorToBounds(br_bounds* b, br_actor* ap) {
    br_matrix34 m2v;
    br_model* model;
    br_actor* a;
    LOG_TRACE("(%p, %p)", b, ap);

    m2v = v1db.model_to_view;

    if (ap->model == NULL)
        model = v1db.default_model;
    else
        model = ap->model;

    BrMatrix34Identity(&v1db.model_to_view);

    if (ap->type == BR_ACTOR_MODEL) {
        *b = model->bounds;
    } else {
        b->min.v[0] = b->min.v[1] = b->min.v[2] = BR_SCALAR_MAX;
        b->max.v[0] = b->max.v[1] = b->max.v[2] = BR_SCALAR_MIN;
    }

    BR_FOR_SIMPLELIST(&ap->children, a)
    ActorToBounds(b, a, model);

    v1db.model_to_view = m2v;
    return b;
}
