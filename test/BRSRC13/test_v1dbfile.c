#include "tests.h"

#include "CORE/FW/resource.h"
#include "CORE/MATH/angles.h"
#include "CORE/MATH/vector.h"
#include "CORE/PIXELMAP/pmfile.h"
#include "CORE/V1DB/actsupt.h"
#include "CORE/FW/datafile.h"
#include "CORE/V1DB/modsupt.h"
#include "CORE/V1DB/prepmesh.h"
#include "CORE/V1DB/regsupt.h"
#include "CORE/PIXELMAP/pixelmap.h"
#include "CORE/V1DB/matsupt.h"
#include "CORE/V1DB/v1dbfile.h"
#include "CORE/MATH/matrix23.h"

#include <stdio.h>
#include <string.h>

static void add_test_table(int i) {
    br_pixelmap *map;
    char name[128];

    sprintf(name, "test_table_%d", i);

    // First remove all maps by this name
    while (1) {
        map = BrTableFind(name);
        if (map == NULL) {
            break;
        }
        BrTableRemove(map);
        BrResFree(map);
    }

    // Then create a new one
    map = BrPixelmapAllocate(i % 2 == 0 ? BR_PMT_INDEX_8 : BR_PMT_RGBA_8888, 1 << i, 1 << i, NULL, BR_PMAF_NORMAL);
    map->identifier = BrResStrDup(map, name);
    if (i % 2 == 0) {
        map->map = BrPixelmapAllocate(BR_PMT_RGBA_8888, 8, 8, NULL, BR_PMAF_NORMAL);
    }
    BrTableAdd(map);
}

static void register_test_tables() {
    int i;
    int nb;

    nb = 5;
    for (i = 0; i < nb; i++) {
        add_test_table(i);
    }
}

static void add_test_map(int nb) {
    br_pixelmap *map;
    char name[128];

    sprintf(name, "test_map_%d", nb);

    // First remove all maps by this name
    while (1) {
        map = BrMapFind(name);
        if (map == NULL) {
            break;
        }
        BrMapRemove(map);
        BrResFree(map);
    }

    // Then create a new one
    map = BrPixelmapAllocate(nb % 2 == 0 ? BR_PMT_INDEX_8 : BR_PMT_RGB_555, 2 << nb, 2 << nb, NULL, BR_PMAF_NORMAL);
    map->identifier = BrResStrDup(map, name);
    if (nb % 2 == 0) {
        map->map = BrPixelmapAllocate(BR_PMT_YUV_888, 8, 8, NULL, BR_PMAF_NORMAL);
    }
    BrMapAdd(map);
}

static void register_test_maps() {
    int i;
    int nb;

    nb = 5;
    for (i = 0; i < nb; i++) {
        add_test_map(i);
    }
}

static void add_test_material(int nb) {
    br_material *mat;
    char name[128];

    sprintf(name, "test_material_%d", nb);

    // First remove all maps by this name
    while (1) {
        mat = BrMaterialFind(name);
        if (mat == NULL) {
            break;
        }
        BrMaterialRemove(mat);
        BrResFree(mat);
    }

    // Then create a new one
    mat = BrMaterialAllocate(name);

    mat->colour = BR_COLOUR_RGB(145 + nb, 23 + nb, 17 + nb);
    mat->opacity = 127 + nb * 1;
    mat->ka = 0.25f + nb * 0.125;
    mat->kd = 0.75f + nb * 0.125;
    mat->ks = 0.15f + nb * 0.125;
    mat->power = 19 + 4 * nb;
    mat->flags = BR_MATF_DECAL | BR_MATF_SMOOTH;
    BrMatrix23Rotate(&mat->map_transform, BrDegreeToAngle(30 + nb * 30));
    BrMatrix23PostTranslate(&mat->map_transform, 0.5f - nb * 0.125, 0.5f + nb * 0.125);
    mat->index_base = 13 + nb * 3;
    mat->index_range = 42 + nb * 3;;
    mat->fog_min = 1.f + nb * .125f;
    mat->fog_max = 2.f + nb * .125f;
    mat->fog_colour = BR_COLOUR_RGB(10, 20, 30); 
    mat->subdivide_tolerance = 5 + nb;

    mat->index_blend = BrTableFind("test_table_0");
    TEST_ASSERT_NOT_NULL(mat->index_blend);
    if (nb % 2 == 1) {
        mat->colour_map = BrMapFind("test_map_1");
        TEST_ASSERT_NOT_NULL(mat->colour_map);
    }
    if (nb % 3 == 2) {
        mat->index_blend = BrTableFind("test_table_2");
        TEST_ASSERT_NOT_NULL(mat->index_blend);
    }
    mat->index_fog = BrTableFind("test_table_3");
    TEST_ASSERT_NOT_NULL(mat->index_fog);
    if (nb % 4 == 3) {
        mat->screendoor = BrTableFind("test_table_4");
        TEST_ASSERT_NOT_NULL(mat->screendoor);
    }
    BrMaterialAdd(mat);
}

static void register_test_materials() {
    int i;
    int nb;

    nb = 5;
    for (i = 0; i < nb; i++) {
        add_test_material(i);
    }
}

static void add_test_model(int nb) {
    br_model *mod;
    int nvertices;
    int nfaces;
    int i;
    char name[128];

    sprintf(name, "test_model_%d", nb);

    // First remove all maps by this name
    while (1) {
        mod = BrModelFind(name);
        if (mod == NULL) {
            break;
        }
        BrModelRemove(mod);
        BrResFree(mod);
    }

    nvertices = 3 + nb;
    nfaces = nb + 1;

    // Then create a new one
    mod = BrModelAllocate(name, nvertices, nfaces);

    for (i = 0; i < nvertices; i++) {
        BrVector3Set(&mod->vertices[i].p, BR_SCALAR(i) + nb, BR_SCALAR(i % 2) + nb, BR_SCALAR(i) + nb);
        if (i % 2 != 0) {
            BrVector2Set(&mod->vertices[i].map, BR_SCALAR(i / 2) * 2 / nvertices, BR_SCALAR((i + 1) % 2));
        }
    }

    for (i = 0; i < nfaces; i++) {
        mod->faces[i].flags = i;
        mod->faces[i].smoothing = i * 3;
        mod->faces[i].vertices[0] = i;
        mod->faces[i].vertices[1] = i + 1;
        mod->faces[i].vertices[2] = i + 2;
    }

    mod->flags = BR_MODF_UPDATEABLE;
    if (nb % 2 != 0) {
        mod->flags |= BR_MODF_PREPREPARED;
    }
    BrVector3Set(&mod->pivot, 0.5f * nb, 0.25f * nb, 0.125f * nb);

    BrModelAdd(mod);
}

static void register_test_models() {
    int i;
    int nb;

    nb = 5;
    for (i = 0; i < nb; i++) {
        add_test_model(i);
    }
}

static br_actor* create_test_actor(char *name, int type, int transform, int nb) {
    char buffer[128];
    br_actor* act;
    br_actor* child;
    int depth;
    br_light* light;
    br_camera* camera;
    br_bounds* bounds;
    br_vector4* clip_plane;

    act = BrActorAllocate(type, NULL);
    act->identifier = BrResStrDup(act, name);
    act->t.type = transform;
    switch (transform) {
        case BR_TRANSFORM_MATRIX34:
        case BR_TRANSFORM_MATRIX34_LP:
            act->t.t.mat.m[0][0] = BR_SCALAR(1.f);
            act->t.t.mat.m[0][1] = BR_SCALAR(2.f);
            act->t.t.mat.m[0][2] = BR_SCALAR(3.f);
            act->t.t.mat.m[1][0] = BR_SCALAR(4.f);
            act->t.t.mat.m[1][1] = BR_SCALAR(5.f);
            act->t.t.mat.m[1][2] = BR_SCALAR(6.f);
            act->t.t.mat.m[2][0] = BR_SCALAR(7.f);
            act->t.t.mat.m[2][1] = BR_SCALAR(8.f);
            act->t.t.mat.m[2][2] = BR_SCALAR(9.f);
            act->t.t.mat.m[3][0] = BR_SCALAR(10.f);
            act->t.t.mat.m[3][1] = BR_SCALAR(11.f);
            act->t.t.mat.m[3][2] = BR_SCALAR(12.f);
            break;
        case BR_TRANSFORM_QUAT:
            act->t.t.quat.q.x = BR_SCALAR(.1);
            act->t.t.quat.q.y = BR_SCALAR(.2);
            act->t.t.quat.q.z = BR_SCALAR(.3);
            act->t.t.quat.q.w = BR_SCALAR(.4);
            act->t.t.quat.t.v[0] = BR_SCALAR(2.f);
            act->t.t.quat.t.v[1] = BR_SCALAR(3.f);
            act->t.t.quat.t.v[2] = BR_SCALAR(4.f);
            break;
        case BR_TRANSFORM_LOOK_UP:
            act->t.t.look_up.look.v[0] = BR_SCALAR(1.f);
            act->t.t.look_up.look.v[1] = BR_SCALAR(2.f);
            act->t.t.look_up.look.v[2] = BR_SCALAR(3.f);
            act->t.t.look_up.up.v[0] = BR_SCALAR(4.f);
            act->t.t.look_up.up.v[1] = BR_SCALAR(5.f);
            act->t.t.look_up.up.v[2] = BR_SCALAR(6.f);
            act->t.t.look_up.t.v[0] = BR_SCALAR(7.f);
            act->t.t.look_up.t.v[1] = BR_SCALAR(8.f);
            act->t.t.look_up.t.v[2] = BR_SCALAR(9.f);
            break;
        case BR_TRANSFORM_EULER:
            act->t.t.euler.e.a = (br_angle)0x1000;
            act->t.t.euler.e.b = (br_angle)0x2000;
            act->t.t.euler.e.c = (br_angle)0x3000;
            act->t.t.euler.e.order = BR_EULER_XZY_R;
            act->t.t.euler.t.v[0] = BR_SCALAR(3.f);
            act->t.t.euler.t.v[1] = BR_SCALAR(6.f);
            act->t.t.euler.t.v[2] = BR_SCALAR(9.f);
            break;
    }
    switch (type) {
    case BR_ACTOR_MODEL:
        sprintf(buffer, "test_model_%d", nb % 6);
        act->model = BrModelFind(buffer);
        TEST_ASSERT_NOT_NULL(act->model);
        sprintf(buffer, "test_material_%d", nb % 5);
        act->material = BrMaterialFind(buffer);
        TEST_ASSERT_NOT_NULL(act->material);
        act->render_style = BR_RSTYLE_FACES;
        break;
    case BR_ACTOR_LIGHT:
        light = act->type_data;
        light->colour = BR_COLOUR_RGBA(0x12 + nb, 0x34 + nb, 0x56 + nb, 0);
        light->attenuation_c = BR_SCALAR(0.125f + 0.125f * nb);
        light->attenuation_l = BR_SCALAR(0.0625f + 0.125f * nb);
        light->attenuation_q = BR_SCALAR(0.25f + 0.125f * nb);
        light->cone_outer = BR_ANGLE_DEG(60 + 30 * nb);
        light->cone_inner = BR_ANGLE_DEG(30 - 30 * nb);
        break;
    case BR_ACTOR_CAMERA:
        camera = act->type_data;
        camera->field_of_view = 0x2000;
        camera->hither_z = BR_SCALAR(0.75f + 0.25f * nb);
        camera->hither_z = BR_SCALAR(2.f + nb);
        camera->aspect = BR_SCALAR(1.33333333333f);
        camera->width = BR_SCALAR(2.25f + 0.0625f * nb);
        camera->height = BR_SCALAR(1.25f + 0.25f * nb);
        camera->distance = BR_SCALAR(1.75f + 0.0625f * nb );
        break;
    case BR_ACTOR_BOUNDS:
    case BR_ACTOR_BOUNDS_CORRECT:
        bounds = act->type_data;
        BrVector3Set(&bounds->min, BR_SCALAR(1.25f), BR_SCALAR(1.75f), BR_SCALAR(0.25f));
        BrVector3Set(&bounds->max, BR_SCALAR(7.75f), BR_SCALAR(2.25f), BR_SCALAR(2.0625f));
        break;
    case BR_ACTOR_CLIP_PLANE:
        clip_plane = act->type_data;
        clip_plane->v[0] = BR_SCALAR(1.f);
        clip_plane->v[1] = BR_SCALAR(1.25f);
        clip_plane->v[2] = BR_SCALAR(1.5f);
        clip_plane->v[3] = BR_SCALAR(1.75f);
        break;
    }
    return act;
}

#define TEST_COMPARE_MATERIAL(MATREF, MAT) do {                                                         \
    TEST_ASSERT_NOT_NULL(MATREF);                                                                       \
    TEST_ASSERT_NOT_NULL(MAT);                                                                          \
    TEST_ASSERT_EQUAL_STRING((MATREF)->identifier, (MAT)->identifier);                                  \
    TEST_ASSERT_EQUAL_UINT32((MATREF)->colour, (MAT)->colour);                                          \
    TEST_ASSERT_EQUAL_UINT8((MATREF)->opacity, (MAT)->opacity);                                         \
    TEST_ASSERT_FLOAT_WITHIN(1e-5, (MATREF)->ka, (MAT)->ka);                                            \
    TEST_ASSERT_FLOAT_WITHIN(1e-5, (MATREF)->kd, (MAT)->kd);                                            \
    TEST_ASSERT_FLOAT_WITHIN(1e-5, (MATREF)->ks, (MAT)->ks);                                            \
    TEST_ASSERT_EQUAL_UINT32((MATREF)->power, (MAT)->power);                                            \
    TEST_ASSERT_EQUAL_UINT32((MATREF)->flags, (MAT)->flags);                                            \
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-5, (MATREF)->map_transform.m, (MAT)->map_transform.m, 6);         \
                                                                                                        \
    TEST_ASSERT_EQUAL_UINT8((MATREF)->index_base, (MAT)->index_base);                                   \
    TEST_ASSERT_EQUAL_UINT8((MATREF)->index_range, (MAT)->index_range);                                 \
    TEST_ASSERT_FLOAT_WITHIN(1e-5, (MATREF)->fog_min, (MAT)->fog_min);                                  \
    TEST_ASSERT_FLOAT_WITHIN(1e-5, (MATREF)->fog_max, (MAT)->fog_max);                                  \
    TEST_ASSERT_EQUAL_UINT32((MATREF)->fog_colour, (MAT)->fog_colour);                                  \
    TEST_ASSERT_EQUAL_INT32((MATREF)->subdivide_tolerance, (MAT)->subdivide_tolerance);                 \
                                                                                                        \
    TEST_ASSERT_EQUAL_PTR((MATREF)->index_blend, (MAT)->index_blend);                                   \
    TEST_ASSERT_EQUAL_PTR((MATREF)->colour_map, (MAT)->colour_map);                                     \
    TEST_ASSERT_EQUAL_PTR((MATREF)->index_shade, (MAT)->index_shade);                                   \
    TEST_ASSERT_EQUAL_PTR((MATREF)->index_fog, (MAT)->index_fog);                                       \
    TEST_ASSERT_EQUAL_PTR((MATREF)->screendoor, (MAT)->screendoor);                                     \
} while (0)

#define TEST_COMPARE_MODEL(MODEL_REF, MODEL) do {                                                                                                                                       \
    int private_index;                                                                                                                                                                  \
    TEST_ASSERT_NOT_NULL(MODEL_REF);                                                                                                                                                    \
    TEST_ASSERT_NOT_NULL(MODEL);                                                                                                                                                        \
    TEST_ASSERT_EQUAL_STRING((MODEL_REF)->identifier, (MODEL)->identifier);                                                                                                             \
    TEST_ASSERT_EQUAL_UINT16((MODEL_REF)->flags & (BR_MODF_DONT_WELD | BR_MODF_KEEP_ORIGINAL | BR_MODF_GENERATE_TAGS | BR_MODF_QUICK_UPDATE), (MODEL)->flags);                          \
    TEST_ASSERT_EQUAL_UINT16((MODEL_REF)->nvertices, (MODEL)->nvertices);                                                                                                               \
    TEST_ASSERT_EQUAL_UINT16((MODEL_REF)->nfaces, (MODEL)->nfaces);                                                                                                                     \
                                                                                                                                                                                        \
    for (private_index = 0; private_index < (MODEL_REF)->nvertices; private_index++) {                                                                                                  \
        TEST_ASSERT_FLOAT_WITHIN(1e-5, (MODEL_REF)->vertices[private_index].p.v[0], (MODEL)->vertices[private_index].p.v[0]);                                                           \
        TEST_ASSERT_FLOAT_WITHIN(1e-5, (MODEL_REF)->vertices[private_index].p.v[1], (MODEL)->vertices[private_index].p.v[1]);                                                           \
        TEST_ASSERT_FLOAT_WITHIN(1e-5, (MODEL_REF)->vertices[private_index].p.v[2], (MODEL)->vertices[private_index].p.v[2]);                                                           \
        TEST_ASSERT_FLOAT_WITHIN(1e-5, (MODEL_REF)->vertices[private_index].map.v[0], (MODEL)->vertices[private_index].map.v[0]);                                                       \
        TEST_ASSERT_FLOAT_WITHIN(1e-5, (MODEL_REF)->vertices[private_index].map.v[1], (MODEL)->vertices[private_index].map.v[1]);                                                       \
    }                                                                                                                                                                                   \
                                                                                                                                                                                        \
    for (private_index = 0; private_index < (MODEL_REF)->nfaces; private_index++) {                                                                                                     \
        TEST_ASSERT_EQUAL_UINT16((MODEL_REF)->faces[private_index].vertices[0], (MODEL)->faces[private_index].vertices[0]);                                                             \
        TEST_ASSERT_EQUAL_UINT16((MODEL_REF)->faces[private_index].vertices[1], (MODEL)->faces[private_index].vertices[1]);                                                             \
        TEST_ASSERT_EQUAL_UINT16((MODEL_REF)->faces[private_index].vertices[2], (MODEL)->faces[private_index].vertices[2]);                                                             \
        TEST_ASSERT_EQUAL_UINT16((MODEL_REF)->faces[private_index].smoothing == 0 ? 0xffff : (MODEL_REF)->faces[private_index].smoothing, (MODEL)->faces[private_index].smoothing);     \
    }                                                                                                                                                                                   \
} while (0)

static void compare_actor(br_actor *actor_ref, br_actor *actor) {
    br_actor *child_ref;
    br_actor *child;

    LOG_DEBUG("Comparing actor_ref \"%s\" with actor \"%s\"", actor_ref->identifier, actor->identifier);
    TEST_ASSERT_NOT_NULL(actor_ref);
    TEST_ASSERT_NOT_NULL(actor);
    TEST_ASSERT_EQUAL_STRING(actor_ref->identifier, actor->identifier);
    TEST_ASSERT_EQUAL_UINT16(actor_ref->t.type, actor->t.type);
    switch (actor_ref->t.type) {
    case BR_TRANSFORM_MATRIX34:
    case BR_TRANSFORM_MATRIX34_LP:
        TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-3, actor_ref->t.t.mat.m, actor->t.t.mat.m, 12);
        break;
    case BR_TRANSFORM_EULER:
        TEST_ASSERT_EQUAL_UINT16(actor_ref->t.t.euler.e.a, actor->t.t.euler.e.a);
        TEST_ASSERT_EQUAL_UINT16(actor_ref->t.t.euler.e.b, actor->t.t.euler.e.b);
        TEST_ASSERT_EQUAL_UINT16(actor_ref->t.t.euler.e.c, actor->t.t.euler.e.c);
        TEST_ASSERT_EQUAL_UINT8(actor_ref->t.t.euler.e.order, actor->t.t.euler.e.order);
        TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-3, actor_ref->t.t.euler.t.v, actor->t.t.euler.t.v, 3);
        break;
    case BR_TRANSFORM_QUAT:
        TEST_ASSERT_FLOAT_WITHIN(1e-3, actor_ref->t.t.quat.q.x, actor->t.t.quat.q.x);
        TEST_ASSERT_FLOAT_WITHIN(1e-3, actor_ref->t.t.quat.q.y, actor->t.t.quat.q.y);
        TEST_ASSERT_FLOAT_WITHIN(1e-3, actor_ref->t.t.quat.q.z, actor->t.t.quat.q.z);
        TEST_ASSERT_FLOAT_WITHIN(1e-3, actor_ref->t.t.quat.q.w, actor->t.t.quat.q.w);
        TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-3, actor_ref->t.t.quat.t.v, actor->t.t.quat.t.v, 3);
        break;
    case BR_TRANSFORM_TRANSLATION:
        TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-3, actor_ref->t.t.translate.t.v, actor->t.t.translate.t.v, 3);
        break;
    case BR_TRANSFORM_LOOK_UP:
        TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-3, actor_ref->t.t.look_up.look.v, actor->t.t.look_up.look.v, 3);
        TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-3, actor_ref->t.t.look_up.up.v, actor->t.t.look_up.up.v, 3);
        TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-3, actor_ref->t.t.look_up.t.v, actor->t.t.look_up.t.v, 3);
        break;
    }
    TEST_ASSERT_EQUAL_PTR(actor_ref->model, actor->model);
    TEST_ASSERT_EQUAL_PTR(actor_ref->material, actor->material);
    switch (actor_ref->type) {
    case BR_ACTOR_CLIP_PLANE:
        TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-5, ((br_vector4*)((actor_ref)->type_data))->v, ((br_vector4*)(actor->type_data))->v, 4);
        break;
    case BR_ACTOR_LIGHT:
        TEST_ASSERT_EQUAL_UINT8_MESSAGE(((br_light*)(actor_ref->type_data))->type, ((br_light*)(actor->type_data))->type, "actor->light_data->type");
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(((br_light*)(actor_ref->type_data))->colour, ((br_light*)(actor->type_data))->colour, "actor->light_data->colour");
        TEST_ASSERT_EQUAL_FLOAT_MESSAGE(((br_light*)(actor_ref->type_data))->attenuation_c, ((br_light*)(actor->type_data))->attenuation_c, "actor->light_data->attenuation_c");
        TEST_ASSERT_EQUAL_FLOAT_MESSAGE(((br_light*)(actor_ref->type_data))->attenuation_l, ((br_light*)(actor->type_data))->attenuation_l, "actor->light_data->attenuation_l");
        TEST_ASSERT_EQUAL_FLOAT_MESSAGE(((br_light*)(actor_ref->type_data))->attenuation_q, ((br_light*)(actor->type_data))->attenuation_q, "actor->light_data->attenuation_q");
        TEST_ASSERT_EQUAL_UINT16_MESSAGE(((br_light*)(actor_ref->type_data))->cone_outer, ((br_light*)(actor->type_data))->cone_outer, "actor->light_data->cone_outer");
        TEST_ASSERT_EQUAL_UINT16_MESSAGE(((br_light*)(actor_ref->type_data))->cone_inner, ((br_light*)(actor->type_data))->cone_inner, "actor->light_data->cone_inner");
        break;
    case BR_ACTOR_CAMERA:
        TEST_ASSERT_EQUAL_UINT8_MESSAGE(((br_camera*)(actor_ref->type_data))->type, ((br_camera*)(actor->type_data))->type, "actor->camera_data->type");
        TEST_ASSERT_EQUAL_UINT16_MESSAGE(((br_camera*)(actor_ref->type_data))->field_of_view, ((br_camera*)(actor->type_data))->field_of_view, "actor->camera_data->field_of_view");
        TEST_ASSERT_EQUAL_FLOAT_MESSAGE(((br_camera*)(actor_ref->type_data))->hither_z, ((br_camera*)(actor->type_data))->hither_z, "actor->camera_data->hither_z");
        TEST_ASSERT_EQUAL_FLOAT_MESSAGE(((br_camera*)(actor_ref->type_data))->yon_z, ((br_camera*)(actor->type_data))->yon_z, "actor->camera_data->yon_z");
        TEST_ASSERT_EQUAL_FLOAT_MESSAGE(((br_camera*)(actor_ref->type_data))->aspect, ((br_camera*)(actor->type_data))->aspect, "actor->camera_data->aspect");
        break;
    case BR_ACTOR_BOUNDS:
    case BR_ACTOR_BOUNDS_CORRECT:
        TEST_ASSERT_EQUAL_FLOAT_ARRAY(((br_bounds3*)(actor_ref->type_data))->min.v, ((br_bounds3*)(actor->type_data))->min.v, 3);
        TEST_ASSERT_EQUAL_FLOAT_ARRAY(((br_bounds3*)(actor_ref->type_data))->max.v, ((br_bounds3*)(actor->type_data))->max.v, 3);
        break;
    }
    if (actor_ref->children != NULL) {
        child_ref = actor_ref->children;
        child = actor->children;
        while (child_ref != NULL) {
            compare_actor(child_ref, child);
            child_ref = child_ref->next;
            child = child->next;
        }
    }
}
    
#define TEST_COMPARE_ACTOR(ACTOR_REF, ACTOR) do { \
    compare_actor(ACTOR_REF, ACTOR); \
} while (0)

static void register_test_items() {
    register_test_maps();
    register_test_tables();
    register_test_materials();
    register_test_models();
}

uint8_t binary_material_lone[] = {
  0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06,
  0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x52,
  0x92, 0x18, 0x12, 0x80, 0x3e, 0xc0, 0x00, 0x00, 0x3f, 0x60, 0x00, 0x00,
  0x3e, 0x8c, 0xcc, 0xcd, 0x41, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44,
  0x3f, 0x00, 0x03, 0xa0, 0x3f, 0x5d, 0xb1, 0xbf, 0xbf, 0x5d, 0xb1, 0xbf,
  0x3f, 0x00, 0x03, 0xa0, 0x3e, 0xc0, 0x00, 0x00, 0x3f, 0x20, 0x00, 0x00,
  0x10, 0x2d, 0x3f, 0x90, 0x00, 0x00, 0x40, 0x08, 0x00, 0x00, 0x0a, 0x14,
  0x1e, 0x00, 0x00, 0x00, 0x06, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x6d, 0x61,
  0x74, 0x65, 0x72, 0x69, 0x61, 0x6c, 0x5f, 0x31, 0x00, 0x00, 0x00, 0x00,
  0x1c, 0x00, 0x00, 0x00, 0x0b, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x6d, 0x61,
  0x70, 0x5f, 0x31, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x0d,
  0x74, 0x65, 0x73, 0x74, 0x5f, 0x74, 0x61, 0x62, 0x6c, 0x65, 0x5f, 0x30,
  0x00, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x0d, 0x74, 0x65, 0x73,
  0x74, 0x5f, 0x74, 0x61, 0x62, 0x6c, 0x65, 0x5f, 0x33, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
char *text_material_lone = 
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_MATERIAL                       # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*MATERIAL         18" HOST_NL
    "  struct    br_material" HOST_NL
    "    colour    146,24,18                                # colour" HOST_NL
    "    uint_8    128                                      # opacity" HOST_NL
    "    ufraction 0.375                                    # ka" HOST_NL
    "    ufraction 0.875                                    # kd" HOST_NL
    "    ufraction 0.275                                    # ks" HOST_NL
    "    scalar    23                                       # power" HOST_NL
    "    uint_32   68                                       # flags" HOST_NL
    "    vector2   0.500055,0.865993                        # map_transform.m[0]" HOST_NL
    "    vector2   -0.865993,0.500055                       # map_transform.m[1]" HOST_NL
    "    vector2   0.375,0.625                              # map_transform.m[2]" HOST_NL
    "    uint_8    16                                       # index_base" HOST_NL
    "    uint_8    45                                       # index_range" HOST_NL
    "    scalar    1.125                                    # fog_min" HOST_NL
    "    scalar    2.125                                    # fog_max" HOST_NL
    "    colour    10,20,30                                 # fog_colour" HOST_NL
    "    int_32    6                                        # subdivide_tolerance" HOST_NL
    "    asciz     \"test_material_1\"                        # identifier" HOST_NL
    "*COLOUR_MAP_REF   1" HOST_NL
    "  name \"test_map_1\"" HOST_NL
    "*INDEX_BLEND_REF  1" HOST_NL
    "  name \"test_table_0\"" HOST_NL
    "*INDEX_FOG_REF    1" HOST_NL
    "  name \"test_table_3\"" HOST_NL
    "*END              0" HOST_NL;
static void test_v1dbfile_save_load_material_lone() {
    br_material *material_ref;
    br_material* materials_read[2];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    int i;
    int res;

    register_test_items();

    material_ref = BrMaterialFind("test_material_1");
    TEST_ASSERT_NOT_NULL(material_ref);

    create_temp_file(tmpfilename_binary, "material_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrMaterialSave(tmpfilename_binary, material_ref);
    TEST_ASSERT_EQUAL_INT(1, res);

    create_temp_file(tmpfilename_text, "material_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrMaterialSave(tmpfilename_text, material_ref);
    TEST_ASSERT_EQUAL_INT(1, res);

    puts("Read binary material");
    materials_read[0] = BrMaterialLoad(tmpfilename_binary);
    puts("Read text material");
    materials_read[1] = BrMaterialLoad(tmpfilename_text);
    
    for (i = 0; i < BR_ASIZE(materials_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_COMPARE_MATERIAL(material_ref, materials_read[i]);
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_material_lone, tmpfilename_binary, sizeof(binary_material_lone));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_material_lone, tmpfilename_text);

    BrResFree(materials_read[0]);
    BrResFree(materials_read[1]);
}

uint8_t binary_material_many[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x52,
    0x91, 0x17, 0x11, 0x7f, 0x3e, 0x80, 0x00, 0x00, 0x3f, 0x40, 0x00, 0x00,
    0x3e, 0x19, 0x99, 0x9a, 0x41, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44,
    0x3f, 0x5d, 0xb4, 0xe3, 0x3e, 0xff, 0xfc, 0x60, 0xbe, 0xff, 0xfc, 0x60,
    0x3f, 0x5d, 0xb4, 0xe3, 0x3f, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,
    0x0d, 0x2a, 0x3f, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x0a, 0x14,
    0x1e, 0x00, 0x00, 0x00, 0x05, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x6d, 0x61,
    0x74, 0x65, 0x72, 0x69, 0x61, 0x6c, 0x5f, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x1e, 0x00, 0x00, 0x00, 0x0d, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x74, 0x61,
    0x62, 0x6c, 0x65, 0x5f, 0x30, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x00,
    0x00, 0x0d, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x74, 0x61, 0x62, 0x6c, 0x65,
    0x5f, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x52, 0x92, 0x18, 0x12, 0x80, 0x3e,
    0xc0, 0x00, 0x00, 0x3f, 0x60, 0x00, 0x00, 0x3e, 0x8c, 0xcc, 0xcd, 0x41,
    0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x3f, 0x00, 0x03, 0xa0, 0x3f,
    0x5d, 0xb1, 0xbf, 0xbf, 0x5d, 0xb1, 0xbf, 0x3f, 0x00, 0x03, 0xa0, 0x3e,
    0xc0, 0x00, 0x00, 0x3f, 0x20, 0x00, 0x00, 0x10, 0x2d, 0x3f, 0x90, 0x00,
    0x00, 0x40, 0x08, 0x00, 0x00, 0x0a, 0x14, 0x1e, 0x00, 0x00, 0x00, 0x06,
    0x74, 0x65, 0x73, 0x74, 0x5f, 0x6d, 0x61, 0x74, 0x65, 0x72, 0x69, 0x61,
    0x6c, 0x5f, 0x31, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x0b,
    0x74, 0x65, 0x73, 0x74, 0x5f, 0x6d, 0x61, 0x70, 0x5f, 0x31, 0x00, 0x00,
    0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x0d, 0x74, 0x65, 0x73, 0x74, 0x5f,
    0x74, 0x61, 0x62, 0x6c, 0x65, 0x5f, 0x30, 0x00, 0x00, 0x00, 0x00, 0x3b,
    0x00, 0x00, 0x00, 0x0d, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x74, 0x61, 0x62,
    0x6c, 0x65, 0x5f, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x52, 0x93, 0x19, 0x13,
    0x81, 0x3f, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x3e, 0xcc, 0xcc,
    0xcd, 0x41, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0xb3, 0x3b, 0xbd,
    0x2e, 0x3f, 0x80, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0xb3, 0x3b, 0xbd,
    0x2e, 0x3e, 0x80, 0x00, 0x00, 0x3f, 0x40, 0x00, 0x00, 0x13, 0x30, 0x3f,
    0xa0, 0x00, 0x00, 0x40, 0x10, 0x00, 0x00, 0x0a, 0x14, 0x1e, 0x00, 0x00,
    0x00, 0x07, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x6d, 0x61, 0x74, 0x65, 0x72,
    0x69, 0x61, 0x6c, 0x5f, 0x32, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00,
    0x00, 0x0d, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x74, 0x61, 0x62, 0x6c, 0x65,
    0x5f, 0x32, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x0d, 0x74,
    0x65, 0x73, 0x74, 0x5f, 0x74, 0x61, 0x62, 0x6c, 0x65, 0x5f, 0x33, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c,
    0x00, 0x00, 0x00, 0x52, 0x94, 0x1a, 0x14, 0x82, 0x3f, 0x20, 0x00, 0x00,
    0x3f, 0x90, 0x00, 0x00, 0x3f, 0x06, 0x66, 0x66, 0x41, 0xf8, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x44, 0xbe, 0xff, 0xfc, 0x61, 0x3f, 0x5d, 0xb4, 0xe3,
    0xbf, 0x5d, 0xb4, 0xe3, 0xbe, 0xff, 0xfc, 0x61, 0x3e, 0x00, 0x00, 0x00,
    0x3f, 0x60, 0x00, 0x00, 0x16, 0x33, 0x3f, 0xb0, 0x00, 0x00, 0x40, 0x18,
    0x00, 0x00, 0x0a, 0x14, 0x1e, 0x00, 0x00, 0x00, 0x08, 0x74, 0x65, 0x73,
    0x74, 0x5f, 0x6d, 0x61, 0x74, 0x65, 0x72, 0x69, 0x61, 0x6c, 0x5f, 0x33,
    0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x0b, 0x74, 0x65, 0x73,
    0x74, 0x5f, 0x6d, 0x61, 0x70, 0x5f, 0x31, 0x00, 0x00, 0x00, 0x00, 0x1e,
    0x00, 0x00, 0x00, 0x0d, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x74, 0x61, 0x62,
    0x6c, 0x65, 0x5f, 0x30, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
    0x0d, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x74, 0x61, 0x62, 0x6c, 0x65, 0x5f,
    0x34, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x0d, 0x74, 0x65,
    0x73, 0x74, 0x5f, 0x74, 0x61, 0x62, 0x6c, 0x65, 0x5f, 0x33, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
char *text_material_many = 
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_MATERIAL                       # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*MATERIAL         18" HOST_NL
    "  struct    br_material" HOST_NL
    "    colour    145,23,17                                # colour" HOST_NL
    "    uint_8    127                                      # opacity" HOST_NL
    "    ufraction 0.25                                     # ka" HOST_NL
    "    ufraction 0.75                                     # kd" HOST_NL
    "    ufraction 0.15                                     # ks" HOST_NL
    "    scalar    19                                       # power" HOST_NL
    "    uint_32   68                                       # flags" HOST_NL
    "    vector2   0.866041,0.499972                        # map_transform.m[0]" HOST_NL
    "    vector2   -0.499972,0.866041                       # map_transform.m[1]" HOST_NL
    "    vector2   0.5,0.5                                  # map_transform.m[2]" HOST_NL
    "    uint_8    13                                       # index_base" HOST_NL
    "    uint_8    42                                       # index_range" HOST_NL
    "    scalar    1                                        # fog_min" HOST_NL
    "    scalar    2                                        # fog_max" HOST_NL
    "    colour    10,20,30                                 # fog_colour" HOST_NL
    "    int_32    5                                        # subdivide_tolerance" HOST_NL
    "    asciz     \"test_material_0\"                        # identifier" HOST_NL
    "*INDEX_BLEND_REF  1" HOST_NL
    "  name \"test_table_0\"" HOST_NL
    "*INDEX_FOG_REF    1" HOST_NL
    "  name \"test_table_3\"" HOST_NL
    "*END              0" HOST_NL
    "*MATERIAL         18" HOST_NL
    "  struct    br_material" HOST_NL
    "    colour    146,24,18                                # colour" HOST_NL
    "    uint_8    128                                      # opacity" HOST_NL
    "    ufraction 0.375                                    # ka" HOST_NL
    "    ufraction 0.875                                    # kd" HOST_NL
    "    ufraction 0.275                                    # ks" HOST_NL
    "    scalar    23                                       # power" HOST_NL
    "    uint_32   68                                       # flags" HOST_NL
    "    vector2   0.500055,0.865993                        # map_transform.m[0]" HOST_NL
    "    vector2   -0.865993,0.500055                       # map_transform.m[1]" HOST_NL
    "    vector2   0.375,0.625                              # map_transform.m[2]" HOST_NL
    "    uint_8    16                                       # index_base" HOST_NL
    "    uint_8    45                                       # index_range" HOST_NL
    "    scalar    1.125                                    # fog_min" HOST_NL
    "    scalar    2.125                                    # fog_max" HOST_NL
    "    colour    10,20,30                                 # fog_colour" HOST_NL
    "    int_32    6                                        # subdivide_tolerance" HOST_NL
    "    asciz     \"test_material_1\"                        # identifier" HOST_NL
    "*COLOUR_MAP_REF   1" HOST_NL
    "  name \"test_map_1\"" HOST_NL
    "*INDEX_BLEND_REF  1" HOST_NL
    "  name \"test_table_0\"" HOST_NL
    "*INDEX_FOG_REF    1" HOST_NL
    "  name \"test_table_3\"" HOST_NL
    "*END              0" HOST_NL
    "*MATERIAL         18" HOST_NL
    "  struct    br_material" HOST_NL
    "    colour    147,25,19                                # colour" HOST_NL
    "    uint_8    129                                      # opacity" HOST_NL
    "    ufraction 0.5                                      # ka" HOST_NL
    "    ufraction 1                                        # kd" HOST_NL
    "    ufraction 0.4                                      # ks" HOST_NL
    "    scalar    27                                       # power" HOST_NL
    "    uint_32   68                                       # flags" HOST_NL
    "    vector2   -4.37114e-08,1                           # map_transform.m[0]" HOST_NL
    "    vector2   -1,-4.37114e-08                          # map_transform.m[1]" HOST_NL
    "    vector2   0.25,0.75                                # map_transform.m[2]" HOST_NL
    "    uint_8    19                                       # index_base" HOST_NL
    "    uint_8    48                                       # index_range" HOST_NL
    "    scalar    1.25                                     # fog_min" HOST_NL
    "    scalar    2.25                                     # fog_max" HOST_NL
    "    colour    10,20,30                                 # fog_colour" HOST_NL
    "    int_32    7                                        # subdivide_tolerance" HOST_NL
    "    asciz     \"test_material_2\"                        # identifier" HOST_NL
    "*INDEX_BLEND_REF  1" HOST_NL
    "  name \"test_table_2\"" HOST_NL
    "*INDEX_FOG_REF    1" HOST_NL
    "  name \"test_table_3\"" HOST_NL
    "*END              0" HOST_NL
    "*MATERIAL         18" HOST_NL
    "  struct    br_material" HOST_NL
    "    colour    148,26,20                                # colour" HOST_NL
    "    uint_8    130                                      # opacity" HOST_NL
    "    ufraction 0.625                                    # ka" HOST_NL
    "    ufraction 1.125                                    # kd" HOST_NL
    "    ufraction 0.525                                    # ks" HOST_NL
    "    scalar    31                                       # power" HOST_NL
    "    uint_32   68                                       # flags" HOST_NL
    "    vector2   -0.499972,0.866041                       # map_transform.m[0]" HOST_NL
    "    vector2   -0.866041,-0.499972                      # map_transform.m[1]" HOST_NL
    "    vector2   0.125,0.875                              # map_transform.m[2]" HOST_NL
    "    uint_8    22                                       # index_base" HOST_NL
    "    uint_8    51                                       # index_range" HOST_NL
    "    scalar    1.375                                    # fog_min" HOST_NL
    "    scalar    2.375                                    # fog_max" HOST_NL
    "    colour    10,20,30                                 # fog_colour" HOST_NL
    "    int_32    8                                        # subdivide_tolerance" HOST_NL
    "    asciz     \"test_material_3\"                        # identifier" HOST_NL
    "*COLOUR_MAP_REF   1" HOST_NL
    "  name \"test_map_1\"" HOST_NL
    "*INDEX_BLEND_REF  1" HOST_NL
    "  name \"test_table_0\"" HOST_NL
    "*SCREENDOOR_REF   1" HOST_NL
    "  name \"test_table_4\"" HOST_NL
    "*INDEX_FOG_REF    1" HOST_NL
    "  name \"test_table_3\"" HOST_NL
    "*END              0" HOST_NL;
static void test_v1dbfile_save_load_material_many() {
#define MATERIAL_MANY_NB 4
#define MATERIAL_MANY_READ_CAPACITY (MATERIAL_MANY_NB*5)
    br_material *materials_ref[MATERIAL_MANY_NB];
    br_material* materials_read[2][MATERIAL_MANY_READ_CAPACITY];
    char name[128];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    int i;
    int k;
    int res;

    register_test_items();

    for (i = 0; i < MATERIAL_MANY_NB; i++) {
        sprintf(name, "test_material_%d", i);
        materials_ref[i] = BrMaterialFind(name);
        TEST_ASSERT_NOT_NULL(materials_ref[i]);
    }

    create_temp_file(tmpfilename_binary, "material_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrMaterialSaveMany(tmpfilename_binary, materials_ref, MATERIAL_MANY_NB);
    TEST_ASSERT_EQUAL_INT(MATERIAL_MANY_NB, res);

    create_temp_file(tmpfilename_text, "material_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrMaterialSaveMany(tmpfilename_text, materials_ref, MATERIAL_MANY_NB);
    TEST_ASSERT_EQUAL_INT(MATERIAL_MANY_NB, res);
    
    LOG_DEBUG("Read binary material");
    res = BrMaterialLoadMany(tmpfilename_binary, materials_read[0], MATERIAL_MANY_READ_CAPACITY);
    TEST_ASSERT_EQUAL(MATERIAL_MANY_NB, res);
    LOG_DEBUG("Read text material");
    res = BrMaterialLoadMany(tmpfilename_text, materials_read[1], MATERIAL_MANY_READ_CAPACITY);
    TEST_ASSERT_EQUAL(MATERIAL_MANY_NB, res);
    
    for (i = 0; i < BR_ASIZE(materials_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        for (k = 0; k < MATERIAL_MANY_NB; k++) {
            TEST_COMPARE_MATERIAL(materials_ref[k], materials_read[i][k]);
        }
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_material_many, tmpfilename_binary, sizeof(binary_material_many));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_material_many, tmpfilename_text);

    for (k = 0; k < MATERIAL_MANY_NB; k++) {
        BrResFree(materials_read[0][k]);
        BrResFree(materials_read[1][k]);
    }
}

uint8_t binary_model_lone[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0xfa, 0xce,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x0e,
    0x80, 0x80, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c,
    0x5f, 0x32, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x40, 0x00,
    0x00, 0x00, 0x05, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40,
    0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x40,
    0x40, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40,
    0x80, 0x00, 0x00, 0x40, 0xa0, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x40,
    0xa0, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40,
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x2c, 0x00,
    0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3e, 0xcc, 0xcc, 0xcd, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x00,
    0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x02, 0xff, 0xff, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x03,
    0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x06, 0x02, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
char *text_model_lone =
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_MODEL                          # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*MODEL            3" HOST_NL
    "  struct    br_model" HOST_NL
    "    uint_16   32896                                    # flags" HOST_NL
    "    asciz     \"test_model_2\"                           # identifier" HOST_NL
    "*VERTICES         21" HOST_NL
    " count 5" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    2                                        # p.v[x]" HOST_NL
    "    scalar    2                                        # p.v[y]" HOST_NL
    "    scalar    2                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    3                                        # p.v[x]" HOST_NL
    "    scalar    3                                        # p.v[y]" HOST_NL
    "    scalar    3                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    4                                        # p.v[x]" HOST_NL
    "    scalar    2                                        # p.v[y]" HOST_NL
    "    scalar    4                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    5                                        # p.v[x]" HOST_NL
    "    scalar    3                                        # p.v[y]" HOST_NL
    "    scalar    5                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    6                                        # p.v[x]" HOST_NL
    "    scalar    2                                        # p.v[y]" HOST_NL
    "    scalar    6                                        # p.v[z]" HOST_NL
    "*VERTEX_UV        16" HOST_NL
    " count 5" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0                                        # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0                                        # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0                                        # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0.4                                      # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0                                        # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "*FACES            19" HOST_NL
    " count 3" HOST_NL
    "  struct    br_face" HOST_NL
    "    uint_16   0                                        # vertices[0]" HOST_NL
    "    uint_16   1                                        # vertices[1]" HOST_NL
    "    uint_16   2                                        # vertices[2]" HOST_NL
    "    uint_16   65535                                    # smoothing" HOST_NL
    "    uint_8    0                                        # flags" HOST_NL
    "  struct    br_face" HOST_NL
    "    uint_16   1                                        # vertices[0]" HOST_NL
    "    uint_16   2                                        # vertices[1]" HOST_NL
    "    uint_16   3                                        # vertices[2]" HOST_NL
    "    uint_16   3                                        # smoothing" HOST_NL
    "    uint_8    1                                        # flags" HOST_NL
    "  struct    br_face" HOST_NL
    "    uint_16   2                                        # vertices[0]" HOST_NL
    "    uint_16   3                                        # vertices[1]" HOST_NL
    "    uint_16   4                                        # vertices[2]" HOST_NL
    "    uint_16   6                                        # smoothing" HOST_NL
    "    uint_8    2                                        # flags" HOST_NL
    "*END              0" HOST_NL;
static void test_v1dbfile_save_load_model_lone() {
    br_model *model_ref;
    br_model* models_read[2];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    int i;
    int res;

    register_test_items();

    model_ref = BrModelFind("test_model_2");
    TEST_ASSERT_NOT_NULL(model_ref);    

    create_temp_file(tmpfilename_binary, "model_lone_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrModelSave(tmpfilename_binary, model_ref);
    TEST_ASSERT_EQUAL(1, res);

    create_temp_file(tmpfilename_text, "model_lone_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrModelSave(tmpfilename_text, model_ref);
    TEST_ASSERT_EQUAL(1, res);

    puts("Read binary model");
    models_read[0] = BrModelLoad(tmpfilename_binary);
    puts("Read text model");
    models_read[1] = BrModelLoad(tmpfilename_text);
    
    for (i = 0; i < BR_ASIZE(models_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_COMPARE_MODEL(model_ref, models_read[i]);
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_model_lone, tmpfilename_binary, sizeof(binary_model_lone));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_model_lone, tmpfilename_text);

    BrResFree(models_read[0]);
    BrResFree(models_read[1]);
}

uint8_t binary_model_many[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0xfa, 0xce,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x0e,
    0x80, 0x80, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c,
    0x5f, 0x30, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x28, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x3f,
    0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00, 0x0d, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36,
    0x00, 0x00, 0x00, 0x0e, 0x00, 0xc0, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x6d,
    0x6f, 0x64, 0x65, 0x6c, 0x5f, 0x31, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00,
    0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x04, 0x3f, 0x80, 0x00, 0x00, 0x3f,
    0x80, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40,
    0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x3f,
    0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x40,
    0x00, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00,
    0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00, 0x16, 0x00,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x03, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00,
    0x0e, 0x80, 0x80, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x6d, 0x6f, 0x64, 0x65,
    0x6c, 0x5f, 0x32, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x40,
    0x00, 0x00, 0x00, 0x05, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00,
    0x40, 0x40, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
    0x40, 0x80, 0x00, 0x00, 0x40, 0xa0, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00,
    0x40, 0xa0, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
    0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x2c,
    0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3e, 0xcc, 0xcc, 0xcd, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35,
    0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x02, 0xff, 0xff, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00,
    0x03, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x06, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00,
    0x00, 0x00, 0x0e, 0x00, 0xc0, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x6d, 0x6f,
    0x64, 0x65, 0x6c, 0x5f, 0x33, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00,
    0x00, 0x4c, 0x00, 0x00, 0x00, 0x06, 0x40, 0x40, 0x00, 0x00, 0x40, 0x40,
    0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x40, 0x80,
    0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x40, 0xa0, 0x00, 0x00, 0x40, 0x40,
    0x00, 0x00, 0x40, 0xa0, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x40, 0x80,
    0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x40, 0xe0, 0x00, 0x00, 0x40, 0x40,
    0x00, 0x00, 0x40, 0xe0, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x40, 0x80,
    0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00,
    0x00, 0x34, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0xaa, 0xaa, 0xab, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x2a,
    0xaa, 0xab, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00,
    0x00, 0x28, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x03, 0x01,
    0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x06, 0x02, 0x00, 0x03, 0x00,
    0x04, 0x00, 0x05, 0x00, 0x09, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};
char *text_model_many =
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_MODEL                          # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*MODEL            3" HOST_NL
    "  struct    br_model" HOST_NL
    "    uint_16   32896                                    # flags" HOST_NL
    "    asciz     \"test_model_0\"                           # identifier" HOST_NL
    "*VERTICES         13" HOST_NL
    " count 3" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    0                                        # p.v[x]" HOST_NL
    "    scalar    0                                        # p.v[y]" HOST_NL
    "    scalar    0                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    1                                        # p.v[x]" HOST_NL
    "    scalar    1                                        # p.v[y]" HOST_NL
    "    scalar    1                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    2                                        # p.v[x]" HOST_NL
    "    scalar    0                                        # p.v[y]" HOST_NL
    "    scalar    2                                        # p.v[z]" HOST_NL
    "*FACES            7" HOST_NL
    " count 1" HOST_NL
    "  struct    br_face" HOST_NL
    "    uint_16   0                                        # vertices[0]" HOST_NL
    "    uint_16   1                                        # vertices[1]" HOST_NL
    "    uint_16   2                                        # vertices[2]" HOST_NL
    "    uint_16   65535                                    # smoothing" HOST_NL
    "    uint_8    0                                        # flags" HOST_NL
    "*END              0" HOST_NL
    "*MODEL            3" HOST_NL
    "  struct    br_model" HOST_NL
    "    uint_16   192                                      # flags" HOST_NL
    "    asciz     \"test_model_1\"                           # identifier" HOST_NL
    "*VERTICES         17" HOST_NL
    " count 4" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    1                                        # p.v[x]" HOST_NL
    "    scalar    1                                        # p.v[y]" HOST_NL
    "    scalar    1                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    2                                        # p.v[x]" HOST_NL
    "    scalar    2                                        # p.v[y]" HOST_NL
    "    scalar    2                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    3                                        # p.v[x]" HOST_NL
    "    scalar    1                                        # p.v[y]" HOST_NL
    "    scalar    3                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    4                                        # p.v[x]" HOST_NL
    "    scalar    2                                        # p.v[y]" HOST_NL
    "    scalar    4                                        # p.v[z]" HOST_NL
    "*VERTEX_UV        13" HOST_NL
    " count 4" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0                                        # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0                                        # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0                                        # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0.5                                      # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "*FACES            13" HOST_NL
    " count 2" HOST_NL
    "  struct    br_face" HOST_NL
    "    uint_16   0                                        # vertices[0]" HOST_NL
    "    uint_16   1                                        # vertices[1]" HOST_NL
    "    uint_16   2                                        # vertices[2]" HOST_NL
    "    uint_16   0                                        # smoothing" HOST_NL
    "    uint_8    0                                        # flags" HOST_NL
    "  struct    br_face" HOST_NL
    "    uint_16   1                                        # vertices[0]" HOST_NL
    "    uint_16   2                                        # vertices[1]" HOST_NL
    "    uint_16   3                                        # vertices[2]" HOST_NL
    "    uint_16   3                                        # smoothing" HOST_NL
    "    uint_8    1                                        # flags" HOST_NL
    "*END              0" HOST_NL
    "*MODEL            3" HOST_NL
    "  struct    br_model" HOST_NL
    "    uint_16   32896                                    # flags" HOST_NL
    "    asciz     \"test_model_2\"                           # identifier" HOST_NL
    "*VERTICES         21" HOST_NL
    " count 5" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    2                                        # p.v[x]" HOST_NL
    "    scalar    2                                        # p.v[y]" HOST_NL
    "    scalar    2                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    3                                        # p.v[x]" HOST_NL
    "    scalar    3                                        # p.v[y]" HOST_NL
    "    scalar    3                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    4                                        # p.v[x]" HOST_NL
    "    scalar    2                                        # p.v[y]" HOST_NL
    "    scalar    4                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    5                                        # p.v[x]" HOST_NL
    "    scalar    3                                        # p.v[y]" HOST_NL
    "    scalar    5                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    6                                        # p.v[x]" HOST_NL
    "    scalar    2                                        # p.v[y]" HOST_NL
    "    scalar    6                                        # p.v[z]" HOST_NL
    "*VERTEX_UV        16" HOST_NL
    " count 5" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0                                        # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0                                        # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0                                        # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0.4                                      # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0                                        # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "*FACES            19" HOST_NL
    " count 3" HOST_NL
    "  struct    br_face" HOST_NL
    "    uint_16   0                                        # vertices[0]" HOST_NL
    "    uint_16   1                                        # vertices[1]" HOST_NL
    "    uint_16   2                                        # vertices[2]" HOST_NL
    "    uint_16   65535                                    # smoothing" HOST_NL
    "    uint_8    0                                        # flags" HOST_NL
    "  struct    br_face" HOST_NL
    "    uint_16   1                                        # vertices[0]" HOST_NL
    "    uint_16   2                                        # vertices[1]" HOST_NL
    "    uint_16   3                                        # vertices[2]" HOST_NL
    "    uint_16   3                                        # smoothing" HOST_NL
    "    uint_8    1                                        # flags" HOST_NL
    "  struct    br_face" HOST_NL
    "    uint_16   2                                        # vertices[0]" HOST_NL
    "    uint_16   3                                        # vertices[1]" HOST_NL
    "    uint_16   4                                        # vertices[2]" HOST_NL
    "    uint_16   6                                        # smoothing" HOST_NL
    "    uint_8    2                                        # flags" HOST_NL
    "*END              0" HOST_NL
    "*MODEL            3" HOST_NL
    "  struct    br_model" HOST_NL
    "    uint_16   192                                      # flags" HOST_NL
    "    asciz     \"test_model_3\"                           # identifier" HOST_NL
    "*VERTICES         25" HOST_NL
    " count 6" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    3                                        # p.v[x]" HOST_NL
    "    scalar    3                                        # p.v[y]" HOST_NL
    "    scalar    3                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    4                                        # p.v[x]" HOST_NL
    "    scalar    4                                        # p.v[y]" HOST_NL
    "    scalar    4                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    5                                        # p.v[x]" HOST_NL
    "    scalar    3                                        # p.v[y]" HOST_NL
    "    scalar    5                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    6                                        # p.v[x]" HOST_NL
    "    scalar    4                                        # p.v[y]" HOST_NL
    "    scalar    6                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    7                                        # p.v[x]" HOST_NL
    "    scalar    3                                        # p.v[y]" HOST_NL
    "    scalar    7                                        # p.v[z]" HOST_NL
    "  struct    br_vertex" HOST_NL
    "    scalar    8                                        # p.v[x]" HOST_NL
    "    scalar    4                                        # p.v[y]" HOST_NL
    "    scalar    8                                        # p.v[z]" HOST_NL
    "*VERTEX_UV        19" HOST_NL
    " count 6" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0                                        # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0                                        # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0                                        # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0.333333                                 # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0                                        # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "  struct    br_vertex_uv" HOST_NL
    "    scalar    0.666667                                 # map.v[0]" HOST_NL
    "    scalar    0                                        # map.v[1]" HOST_NL
    "*FACES            25" HOST_NL
    " count 4" HOST_NL
    "  struct    br_face" HOST_NL
    "    uint_16   0                                        # vertices[0]" HOST_NL
    "    uint_16   1                                        # vertices[1]" HOST_NL
    "    uint_16   2                                        # vertices[2]" HOST_NL
    "    uint_16   0                                        # smoothing" HOST_NL
    "    uint_8    0                                        # flags" HOST_NL
    "  struct    br_face" HOST_NL
    "    uint_16   1                                        # vertices[0]" HOST_NL
    "    uint_16   2                                        # vertices[1]" HOST_NL
    "    uint_16   3                                        # vertices[2]" HOST_NL
    "    uint_16   3                                        # smoothing" HOST_NL
    "    uint_8    1                                        # flags" HOST_NL
    "  struct    br_face" HOST_NL
    "    uint_16   2                                        # vertices[0]" HOST_NL
    "    uint_16   3                                        # vertices[1]" HOST_NL
    "    uint_16   4                                        # vertices[2]" HOST_NL
    "    uint_16   6                                        # smoothing" HOST_NL
    "    uint_8    2                                        # flags" HOST_NL
    "  struct    br_face" HOST_NL
    "    uint_16   3                                        # vertices[0]" HOST_NL
    "    uint_16   4                                        # vertices[1]" HOST_NL
    "    uint_16   5                                        # vertices[2]" HOST_NL
    "    uint_16   9                                        # smoothing" HOST_NL
    "    uint_8    3                                        # flags" HOST_NL
    "*END              0" HOST_NL;
static void test_v1dbfile_save_load_model_many() {
#define MODEL_MANY_NB 4
#define MODEL_MANY_READ_CAPACITY (MATERIAL_MANY_NB*5)
    br_model *models_ref[MODEL_MANY_NB];
    br_model* models_read[2][MODEL_MANY_READ_CAPACITY];
    char name[128];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    int i;
    int k;
    int res;

    register_test_items();

    for (i = 0; i < MODEL_MANY_NB; i++) {
        sprintf(name, "test_model_%d", i);
        models_ref[i] = BrModelFind(name);
        TEST_ASSERT_NOT_NULL(models_ref[i]);
    }

    create_temp_file(tmpfilename_binary, "model_many_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrModelSaveMany(tmpfilename_binary, models_ref, MODEL_MANY_NB);
    TEST_ASSERT_EQUAL_INT(MODEL_MANY_NB, res);

    create_temp_file(tmpfilename_text, "model_many_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrModelSaveMany(tmpfilename_text, models_ref, MODEL_MANY_NB);
    TEST_ASSERT_EQUAL_INT(MODEL_MANY_NB, res);
    
    LOG_DEBUG("Read binary model");
    res = BrModelLoadMany(tmpfilename_binary, models_read[0], MODEL_MANY_READ_CAPACITY);
    TEST_ASSERT_EQUAL_INT(MODEL_MANY_NB, res);
    LOG_DEBUG("Read text model");
    res = BrModelLoadMany(tmpfilename_text, models_read[1], MODEL_MANY_READ_CAPACITY);
    TEST_ASSERT_EQUAL_INT(MODEL_MANY_NB, res);
    
    for (i = 0; i < BR_ASIZE(models_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        for (k = 0; k < MODEL_MANY_NB; k++) {
            TEST_COMPARE_MODEL(models_ref[k], models_read[i][k]);
        }
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_model_many, tmpfilename_binary, sizeof(binary_model_many));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_model_many, tmpfilename_text);

    for (k = 0; k < MATERIAL_MANY_NB; k++) {
        BrResFree(models_read[0][k]);
        BrResFree(models_read[1][k]);
    }
}

static br_uint_8 binary_actor_none_unity_lone[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x21,
    0x00, 0x00, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x61, 0x63, 0x74, 0x6f, 0x72,
    0x5f, 0x6e, 0x6f, 0x6e, 0x65, 0x5f, 0x74, 0x72, 0x61, 0x6e, 0x73, 0x66,
    0x6f, 0x72, 0x6d, 0x5f, 0x75, 0x6e, 0x69, 0x74, 0x79, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static char *text_actor_none_unity_lone = 
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_ACTORS                         # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*ACTOR            4" HOST_NL
    "  struct    br_actor" HOST_NL
    "    enum_8    BR_ACTOR_NONE                            # type" HOST_NL
    "    enum_8    BR_RSTYLE_DEFAULT                        # render_style" HOST_NL
    "    asciz     \"test_actor_none_transform_unity\"        # identifier" HOST_NL
    "*END              0" HOST_NL;
static void test_v1dbfile_save_load_actor_none_transform_unity_lone() {
    br_actor* actor_ref;
    br_actor* actors_read[2];
    char name[128];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    int i;
    int k;
    int res;

    register_test_items();
    actor_ref = create_test_actor("test_actor_none_transform_unity", BR_ACTOR_NONE, BR_TRANSFORM_IDENTITY,0);

    create_temp_file(tmpfilename_binary, "actor_none_transform_unity_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrActorSave(tmpfilename_binary, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);

    create_temp_file(tmpfilename_text, "actor_none_transform_unity_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrActorSave(tmpfilename_text, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);
    
    LOG_DEBUG("Read binary actor");
    actors_read[0] = BrActorLoad(tmpfilename_binary);
    TEST_ASSERT_NOT_NULL(actors_read[0]);
    LOG_DEBUG("Read text actor");
    actors_read[1] = BrActorLoad(tmpfilename_text);
    TEST_ASSERT_NOT_NULL(actors_read[1]);

    for (i = 0; i < BR_ASIZE(actors_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_COMPARE_ACTOR(actor_ref, actors_read[i]);
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_actor_none_unity_lone, tmpfilename_binary, sizeof(binary_actor_none_unity_lone));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_actor_none_unity_lone, tmpfilename_text);

    BrResFree(actor_ref);
    BrResFree(actors_read[0]);
    BrResFree(actors_read[1]);
}

static br_uint_8 binary_actor_none_matrix34_lone[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x24,
    0x00, 0x00, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x61, 0x63, 0x74, 0x6f, 0x72,
    0x5f, 0x6e, 0x6f, 0x6e, 0x65, 0x5f, 0x74, 0x72, 0x61, 0x6e, 0x73, 0x66,
    0x6f, 0x72, 0x6d, 0x5f, 0x6d, 0x61, 0x74, 0x72, 0x69, 0x78, 0x33, 0x34,
    0x00, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x00, 0x00, 0x30, 0x3f, 0x80, 0x00,
    0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x40, 0x80, 0x00,
    0x00, 0x40, 0xa0, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x40, 0xe0, 0x00,
    0x00, 0x41, 0x00, 0x00, 0x00, 0x41, 0x10, 0x00, 0x00, 0x41, 0x20, 0x00,
    0x00, 0x41, 0x30, 0x00, 0x00, 0x41, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,
};
static char *text_actor_none_matrix34_lone = 
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_ACTORS                         # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*ACTOR            4" HOST_NL
    "  struct    br_actor" HOST_NL
    "    enum_8    BR_ACTOR_NONE                            # type" HOST_NL
    "    enum_8    BR_RSTYLE_DEFAULT                        # render_style" HOST_NL
    "    asciz     \"test_actor_none_transform_matrix34\"     # identifier" HOST_NL
    "*TRANSFORM_MATRIX34 5" HOST_NL
    "  struct    br_transform_matrix34" HOST_NL
    "    vector3   1,2,3                                    # t.mat.m[0]" HOST_NL
    "    vector3   4,5,6                                    # t.mat.m[1]" HOST_NL
    "    vector3   7,8,9                                    # t.mat.m[2]" HOST_NL
    "    vector3   10,11,12                                 # t.mat.m[3]" HOST_NL
    "*ACTOR_TRANSFORM  0" HOST_NL
    "*END              0" HOST_NL;
static void test_v1dbfile_save_load_actor_none_transform_matrix34_lone() {
    br_actor* actor_ref;
    br_actor* actors_read[2];
    char name[128];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    int i;
    int k;
    int res;

    register_test_items();
    actor_ref = create_test_actor("test_actor_none_transform_matrix34", BR_ACTOR_NONE, BR_TRANSFORM_MATRIX34,0);

    create_temp_file(tmpfilename_binary, "actor_none_transform_matrix34_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrActorSave(tmpfilename_binary, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);

    create_temp_file(tmpfilename_text, "actor_none_transform_matrix34_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrActorSave(tmpfilename_text, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);
    
    LOG_DEBUG("Read binary actor");
    actors_read[0] = BrActorLoad(tmpfilename_binary);
    TEST_ASSERT_NOT_NULL(actors_read[0]);
    LOG_DEBUG("Read text actor");
    actors_read[1] = BrActorLoad(tmpfilename_text);
    TEST_ASSERT_NOT_NULL(actors_read[1]);

    for (i = 0; i < BR_ASIZE(actors_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_COMPARE_ACTOR(actor_ref, actors_read[i]);
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_actor_none_matrix34_lone, tmpfilename_binary, sizeof(binary_actor_none_matrix34_lone));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_actor_none_matrix34_lone, tmpfilename_text);

    BrResFree(actor_ref);
    BrResFree(actors_read[0]);
    BrResFree(actors_read[1]);
}

static br_uint_8 binary_actor_none_matrix34lp_lone[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x26,
    0x00, 0x00, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x61, 0x63, 0x74, 0x6f, 0x72,
    0x5f, 0x6e, 0x6f, 0x6e, 0x65, 0x5f, 0x74, 0x72, 0x61, 0x6e, 0x73, 0x66,
    0x6f, 0x72, 0x6d, 0x5f, 0x6d, 0x61, 0x74, 0x72, 0x69, 0x78, 0x33, 0x34,
    0x6c, 0x70, 0x00, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x30, 0x3f,
    0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x40,
    0x80, 0x00, 0x00, 0x40, 0xa0, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x40,
    0xe0, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x41, 0x10, 0x00, 0x00, 0x41,
    0x20, 0x00, 0x00, 0x41, 0x30, 0x00, 0x00, 0x41, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
};
static char *text_actor_none_matrix34lp_lone = 
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_ACTORS                         # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*ACTOR            4" HOST_NL
    "  struct    br_actor" HOST_NL
    "    enum_8    BR_ACTOR_NONE                            # type" HOST_NL
    "    enum_8    BR_RSTYLE_DEFAULT                        # render_style" HOST_NL
    "    asciz     \"test_actor_none_transform_matrix34lp\"   # identifier" HOST_NL
    "*TRANSFORM_MATRIX34_LP 5" HOST_NL
    "  struct    br_transform_matrix34" HOST_NL
    "    vector3   1,2,3                                    # t.mat.m[0]" HOST_NL
    "    vector3   4,5,6                                    # t.mat.m[1]" HOST_NL
    "    vector3   7,8,9                                    # t.mat.m[2]" HOST_NL
    "    vector3   10,11,12                                 # t.mat.m[3]" HOST_NL
    "*ACTOR_TRANSFORM  0" HOST_NL
    "*END              0" HOST_NL;
static void test_v1dbfile_save_load_actor_none_transform_matrix34lp_lone() {
    br_actor* actor_ref;
    br_actor* actors_read[2];
    char name[128];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    int i;
    int k;
    int res;

    register_test_items();
    actor_ref = create_test_actor("test_actor_none_transform_matrix34lp", BR_ACTOR_NONE, BR_TRANSFORM_MATRIX34_LP,0);

    create_temp_file(tmpfilename_binary, "actor_none_transform_matrix34lp_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrActorSave(tmpfilename_binary, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);

    create_temp_file(tmpfilename_text, "actor_none_transform_matrix34lp_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrActorSave(tmpfilename_text, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);
    
    LOG_DEBUG("Read binary actor");
    actors_read[0] = BrActorLoad(tmpfilename_binary);
    TEST_ASSERT_NOT_NULL(actors_read[0]);
    LOG_DEBUG("Read text actor");
    actors_read[1] = BrActorLoad(tmpfilename_text);
    TEST_ASSERT_NOT_NULL(actors_read[1]);

    for (i = 0; i < BR_ASIZE(actors_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_COMPARE_ACTOR(actor_ref, actors_read[i]);
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_actor_none_matrix34lp_lone, tmpfilename_binary, sizeof(binary_actor_none_matrix34lp_lone));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_actor_none_matrix34lp_lone, tmpfilename_text);

    BrResFree(actor_ref);
    BrResFree(actors_read[0]);
    BrResFree(actors_read[1]);
}

static br_uint_8 binary_actor_none_quat_lone[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x20,
    0x00, 0x00, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x61, 0x63, 0x74, 0x6f, 0x72,
    0x5f, 0x6e, 0x6f, 0x6e, 0x65, 0x5f, 0x74, 0x72, 0x61, 0x6e, 0x73, 0x66,
    0x6f, 0x72, 0x6d, 0x5f, 0x71, 0x75, 0x61, 0x74, 0x00, 0x00, 0x00, 0x00,
    0x2d, 0x00, 0x00, 0x00, 0x1c, 0x3d, 0xcc, 0xcc, 0xcd, 0x3e, 0x4c, 0xcc,
    0xcd, 0x3e, 0x99, 0x99, 0x9a, 0x3e, 0xcc, 0xcc, 0xcd, 0x40, 0x00, 0x00,
    0x00, 0x40, 0x40, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,
};
static char *text_actor_none_quat_lone = 
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_ACTORS                         # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*ACTOR            4" HOST_NL
    "  struct    br_actor" HOST_NL
    "    enum_8    BR_ACTOR_NONE                            # type" HOST_NL
    "    enum_8    BR_RSTYLE_DEFAULT                        # render_style" HOST_NL
    "    asciz     \"test_actor_none_transform_quat\"         # identifier" HOST_NL
    "*TRANSFORM_QUAT   6" HOST_NL
    "  struct    br_transform_quat" HOST_NL
    "    scalar    0.1                                      # t.quat.q.x" HOST_NL
    "    scalar    0.2                                      # t.quat.q.y" HOST_NL
    "    scalar    0.3                                      # t.quat.q.z" HOST_NL
    "    scalar    0.4                                      # t.quat.q.w" HOST_NL
    "    vector3   2,3,4                                    # t.quat.t" HOST_NL
    "*ACTOR_TRANSFORM  0" HOST_NL
    "*END              0" HOST_NL;
static void test_v1dbfile_save_load_actor_none_transform_quat_lone() {
    br_actor* actor_ref;
    br_actor* actors_read[2];
    char name[128];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    int i;
    int k;
    int res;

    register_test_items();
    actor_ref = create_test_actor("test_actor_none_transform_quat", BR_ACTOR_NONE, BR_TRANSFORM_QUAT,0);

    create_temp_file(tmpfilename_binary, "actor_none_transform_quat_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrActorSave(tmpfilename_binary, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);

    create_temp_file(tmpfilename_text, "actor_none_transform_quat_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrActorSave(tmpfilename_text, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);
    
    LOG_DEBUG("Read binary actor");
    actors_read[0] = BrActorLoad(tmpfilename_binary);
    TEST_ASSERT_NOT_NULL(actors_read[0]);
    LOG_DEBUG("Read text actor");
    actors_read[1] = BrActorLoad(tmpfilename_text);
    TEST_ASSERT_NOT_NULL(actors_read[1]);

    for (i = 0; i < BR_ASIZE(actors_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_COMPARE_ACTOR(actor_ref, actors_read[i]);
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_actor_none_quat_lone, tmpfilename_binary, sizeof(binary_actor_none_quat_lone));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_actor_none_quat_lone, tmpfilename_text);

    BrResFree(actor_ref);
    BrResFree(actors_read[0]);
    BrResFree(actors_read[1]);
}

static br_uint_8 binary_actor_none_lookup_lone[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x22,
    0x00, 0x00, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x61, 0x63, 0x74, 0x6f, 0x72,
    0x5f, 0x6e, 0x6f, 0x6e, 0x65, 0x5f, 0x74, 0x72, 0x61, 0x6e, 0x73, 0x66,
    0x6f, 0x72, 0x6d, 0x5f, 0x6c, 0x6f, 0x6f, 0x6b, 0x75, 0x70, 0x00, 0x00,
    0x00, 0x00, 0x2f, 0x00, 0x00, 0x00, 0x24, 0x3f, 0x80, 0x00, 0x00, 0x40,
    0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x40,
    0xa0, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x40, 0xe0, 0x00, 0x00, 0x41,
    0x00, 0x00, 0x00, 0x41, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static char *text_actor_none_lookup_lone = 
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_ACTORS                         # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*ACTOR            4" HOST_NL
    "  struct    br_actor" HOST_NL
    "    enum_8    BR_ACTOR_NONE                            # type" HOST_NL
    "    enum_8    BR_RSTYLE_DEFAULT                        # render_style" HOST_NL
    "    asciz     \"test_actor_none_transform_lookup\"       # identifier" HOST_NL
    "*TRANSFORM_LOOK_UP 4" HOST_NL
    "  struct    br_transform_look_up" HOST_NL
    "    vector3   1,2,3                                    # t.look_up.look" HOST_NL
    "    vector3   4,5,6                                    # t.look_up.up" HOST_NL
    "    vector3   7,8,9                                    # t.look_up.t" HOST_NL
    "*ACTOR_TRANSFORM  0" HOST_NL
    "*END              0" HOST_NL;
static void test_v1dbfile_save_load_actor_none_transform_lookup_lone() {
    br_actor* actor_ref;
    br_actor* actors_read[2];
    char name[128];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    int i;
    int k;
    int res;

    register_test_items();
    actor_ref = create_test_actor("test_actor_none_transform_lookup", BR_ACTOR_NONE, BR_TRANSFORM_LOOK_UP,0);

    create_temp_file(tmpfilename_binary, "actor_none_transform_lookup_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrActorSave(tmpfilename_binary, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);

    create_temp_file(tmpfilename_text, "actor_none_transform_lookup_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrActorSave(tmpfilename_text, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);
    
    LOG_DEBUG("Read binary actor");
    actors_read[0] = BrActorLoad(tmpfilename_binary);
    TEST_ASSERT_NOT_NULL(actors_read[0]);
    LOG_DEBUG("Read text actor");
    actors_read[1] = BrActorLoad(tmpfilename_text);
    TEST_ASSERT_NOT_NULL(actors_read[1]);

    for (i = 0; i < BR_ASIZE(actors_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_COMPARE_ACTOR(actor_ref, actors_read[i]);
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_actor_none_lookup_lone, tmpfilename_binary, sizeof(binary_actor_none_lookup_lone));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_actor_none_lookup_lone, tmpfilename_text);

    BrResFree(actor_ref);
    BrResFree(actors_read[0]);
    BrResFree(actors_read[1]);
}

static br_uint_8 binary_actor_none_euler_lone[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x21,
    0x00, 0x00, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x61, 0x63, 0x74, 0x6f, 0x72,
    0x5f, 0x6e, 0x6f, 0x6e, 0x65, 0x5f, 0x74, 0x72, 0x61, 0x6e, 0x73, 0x66,
    0x6f, 0x72, 0x6d, 0x5f, 0x65, 0x75, 0x6c, 0x65, 0x72, 0x00, 0x00, 0x00,
    0x00, 0x2e, 0x00, 0x00, 0x00, 0x13, 0x11, 0x10, 0x00, 0x20, 0x00, 0x30,
    0x00, 0x40, 0x40, 0x00, 0x00, 0x40, 0xc0, 0x00, 0x00, 0x41, 0x10, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
};
static char *text_actor_none_euler_lone = 
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_ACTORS                         # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*ACTOR            4" HOST_NL
    "  struct    br_actor" HOST_NL
    "    enum_8    BR_ACTOR_NONE                            # type" HOST_NL
    "    enum_8    BR_RSTYLE_DEFAULT                        # render_style" HOST_NL
    "    asciz     \"test_actor_none_transform_euler\"        # identifier" HOST_NL
    "*TRANSFORM_EULER  6" HOST_NL
    "  struct    br_transform_euler" HOST_NL
    "    enum_8    BR_EULER_XZY_R                           # t.euler.e.order" HOST_NL
    "    angle     22.5                                     # t.euler.e.a" HOST_NL
    "    angle     45                                       # t.euler.e.b" HOST_NL
    "    angle     67.5                                     # t.euler.e.c" HOST_NL
    "    vector3   3,6,9                                    # t.euler.t" HOST_NL
    "*ACTOR_TRANSFORM  0" HOST_NL
    "*END              0" HOST_NL;
static void test_v1dbfile_save_load_actor_none_transform_euler_lone() {
    br_actor* actor_ref;
    br_actor* actors_read[2];
    char name[128];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    int i;
    int k;
    int res;

    register_test_items();
    actor_ref = create_test_actor("test_actor_none_transform_euler", BR_ACTOR_NONE, BR_TRANSFORM_EULER,0);

    create_temp_file(tmpfilename_binary, "actor_none_transform_euler_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrActorSave(tmpfilename_binary, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);

    create_temp_file(tmpfilename_text, "actor_none_transform_euler_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrActorSave(tmpfilename_text, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);
    
    LOG_DEBUG("Read binary actor");
    actors_read[0] = BrActorLoad(tmpfilename_binary);
    TEST_ASSERT_NOT_NULL(actors_read[0]);
    LOG_DEBUG("Read text actor");
    actors_read[1] = BrActorLoad(tmpfilename_text);
    TEST_ASSERT_NOT_NULL(actors_read[1]);

    for (i = 0; i < BR_ASIZE(actors_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_COMPARE_ACTOR(actor_ref, actors_read[i]);
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_actor_none_euler_lone, tmpfilename_binary, sizeof(binary_actor_none_euler_lone));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_actor_none_euler_lone, tmpfilename_text);

    BrResFree(actor_ref);
    BrResFree(actors_read[0]);
    BrResFree(actors_read[1]);
}

static br_uint_8 binary_actor_model_lone[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x12,
    0x01, 0x04, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x61, 0x63, 0x74, 0x6f, 0x72,
    0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x26, 0x00,
    0x00, 0x00, 0x10, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x6d, 0x61, 0x74, 0x65,
    0x72, 0x69, 0x61, 0x6c, 0x5f, 0x30, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00,
    0x00, 0x00, 0x0d, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x6d, 0x6f, 0x64, 0x65,
    0x6c, 0x5f, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static char *text_actor_model_lone = 
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_ACTORS                         # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*ACTOR            4" HOST_NL
    "  struct    br_actor" HOST_NL
    "    enum_8    BR_ACTOR_MODEL                           # type" HOST_NL
    "    enum_8    BR_RSTYLE_FACES                          # render_style" HOST_NL
    "    asciz     \"test_actor_model\"                       # identifier" HOST_NL
    "*ACTOR_MATERIAL   1" HOST_NL
    "  name \"test_material_0\"" HOST_NL
    "*ACTOR_MODEL      1" HOST_NL
    "  name \"test_model_0\"" HOST_NL
    "*END              0" HOST_NL;
static void test_v1dbfile_save_load_actor_model_lone() {
    br_actor* actor_ref;
    br_actor* actors_read[2];
    char name[128];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    int i;
    int k;
    int res;

    register_test_items();
    actor_ref = create_test_actor("test_actor_model", BR_ACTOR_MODEL, BR_TRANSFORM_IDENTITY, 0);

    create_temp_file(tmpfilename_binary, "actor_model_lone_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrActorSave(tmpfilename_binary, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);

    create_temp_file(tmpfilename_text, "actor_model_lone_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrActorSave(tmpfilename_text, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);
    
    LOG_DEBUG("Read binary actor");
    actors_read[0] = BrActorLoad(tmpfilename_binary);
    TEST_ASSERT_NOT_NULL(actors_read[0]);
    LOG_DEBUG("Read text actor");
    actors_read[1] = BrActorLoad(tmpfilename_text);
    TEST_ASSERT_NOT_NULL(actors_read[1]);

    for (i = 0; i < BR_ASIZE(actors_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_COMPARE_ACTOR(actor_ref, actors_read[i]);
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_actor_model_lone, tmpfilename_binary, sizeof(binary_actor_model_lone));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_actor_model_lone, tmpfilename_text);

    BrResFree(actor_ref);
    BrResFree(actors_read[0]);
    BrResFree(actors_read[1]);
}

static br_uint_8 binary_actor_light_lone[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x12,
    0x02, 0x00, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x61, 0x63, 0x74, 0x6f, 0x72,
    0x5f, 0x6c, 0x69, 0x67, 0x68, 0x74, 0x00, 0x00, 0x00, 0x00, 0x33, 0x00,
    0x00, 0x00, 0x15, 0x01, 0x12, 0x34, 0x56, 0x3e, 0x00, 0x00, 0x00, 0x3d,
    0x80, 0x00, 0x00, 0x3e, 0x80, 0x00, 0x00, 0x15, 0x54, 0x2a, 0xa8, 0x00,
    0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};
static char *text_actor_light_lone = 
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_ACTORS                         # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*ACTOR            4" HOST_NL
    "  struct    br_actor" HOST_NL
    "    enum_8    BR_ACTOR_LIGHT                           # type" HOST_NL
    "    enum_8    BR_RSTYLE_DEFAULT                        # render_style" HOST_NL
    "    asciz     \"test_actor_light\"                       # identifier" HOST_NL
    "*LIGHT            9" HOST_NL
    "  struct    br_light" HOST_NL
    "    enum_8    BR_LIGHT_DIRECT                          # type" HOST_NL
    "    colour    18,52,86                                 # colour" HOST_NL
    "    scalar    0.125                                    # attenuation_c" HOST_NL
    "    scalar    0.0625                                   # attenuation_l" HOST_NL
    "    scalar    0.25                                     # attenuation_q" HOST_NL
    "    angle     29.9927                                  # cone_inner" HOST_NL
    "    angle     59.9854                                  # cone_outer" HOST_NL
    "    asciz     NULL                                     # identifier" HOST_NL
    "*ACTOR_LIGHT      0" HOST_NL
    "*END              0" HOST_NL;
static void test_v1dbfile_save_load_actor_light_lone() {
    br_actor* actor_ref;
    br_actor* actors_read[2];
    char name[128];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    int i;
    int k;
    int res;

    register_test_items();
    actor_ref = create_test_actor("test_actor_light", BR_ACTOR_LIGHT, BR_TRANSFORM_IDENTITY, 0);

    create_temp_file(tmpfilename_binary, "actor_light_lone_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrActorSave(tmpfilename_binary, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);

    create_temp_file(tmpfilename_text, "actor_light_lone_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrActorSave(tmpfilename_text, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);
    
    LOG_DEBUG("Read binary actor");
    actors_read[0] = BrActorLoad(tmpfilename_binary);
    TEST_ASSERT_NOT_NULL(actors_read[0]);
    LOG_DEBUG("Read text actor");
    actors_read[1] = BrActorLoad(tmpfilename_text);
    TEST_ASSERT_NOT_NULL(actors_read[1]);

    for (i = 0; i < BR_ASIZE(actors_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_COMPARE_ACTOR(actor_ref, actors_read[i]);
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_actor_light_lone, tmpfilename_binary, sizeof(binary_actor_light_lone));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_actor_light_lone, tmpfilename_text);

    BrResFree(actor_ref);
    BrResFree(actors_read[0]);
    BrResFree(actors_read[1]);
}

static br_uint_8 binary_actor_camera_lone[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x13,
    0x03, 0x00, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x61, 0x63, 0x74, 0x6f, 0x72,
    0x5f, 0x63, 0x61, 0x6d, 0x65, 0x72, 0x61, 0x00, 0x00, 0x00, 0x00, 0x34,
    0x00, 0x00, 0x00, 0x0f, 0x01, 0x20, 0x00, 0x40, 0x00, 0x00, 0x00, 0x41,
    0x20, 0x00, 0x00, 0x3f, 0xaa, 0xaa, 0xab, 0x00, 0x00, 0x00, 0x00, 0x28,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static char *text_actor_camera_lone = 
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_ACTORS                         # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*ACTOR            4" HOST_NL
    "  struct    br_actor" HOST_NL
    "    enum_8    BR_ACTOR_CAMERA                          # type" HOST_NL
    "    enum_8    BR_RSTYLE_DEFAULT                        # render_style" HOST_NL
    "    asciz     \"test_actor_camera\"                      # identifier" HOST_NL
    "*CAMERA           7" HOST_NL
    "  struct    br_camera" HOST_NL
    "    enum_8    BR_CAMERA_PERSPECTIVE                    # type" HOST_NL
    "    angle     45                                       # field_of_view" HOST_NL
    "    scalar    2                                        # hither_z" HOST_NL
    "    scalar    10                                       # yon_z" HOST_NL
    "    scalar    1.33333                                  # aspect" HOST_NL
    "    asciz     NULL                                     # identifier" HOST_NL
    "*ACTOR_CAMERA     0" HOST_NL
    "*END              0" HOST_NL;
static void test_v1dbfile_save_load_actor_camera_lone() {
    br_actor* actor_ref;
    br_actor* actors_read[2];
    char name[128];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    int i;
    int k;
    int res;

    register_test_items();
    actor_ref = create_test_actor("test_actor_camera", BR_ACTOR_CAMERA, BR_TRANSFORM_IDENTITY, 0);

    create_temp_file(tmpfilename_binary, "actor_camera_lone_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrActorSave(tmpfilename_binary, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);

    create_temp_file(tmpfilename_text, "actor_camera_lone_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrActorSave(tmpfilename_text, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);
    
    LOG_DEBUG("Read binary actor");
    actors_read[0] = BrActorLoad(tmpfilename_binary);
    TEST_ASSERT_NOT_NULL(actors_read[0]);
    LOG_DEBUG("Read text actor");
    actors_read[1] = BrActorLoad(tmpfilename_text);
    TEST_ASSERT_NOT_NULL(actors_read[1]);

    for (i = 0; i < BR_ASIZE(actors_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_COMPARE_ACTOR(actor_ref, actors_read[i]);
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_actor_camera_lone, tmpfilename_binary, sizeof(binary_actor_camera_lone));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_actor_camera_lone, tmpfilename_text);

    BrResFree(actor_ref);
    BrResFree(actors_read[0]);
    BrResFree(actors_read[1]);
}

static br_uint_8 binary_actor_bounds_lone[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x13,
    0x05, 0x00, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x61, 0x63, 0x74, 0x6f, 0x72,
    0x5f, 0x62, 0x6f, 0x75, 0x6e, 0x64, 0x73, 0x00, 0x00, 0x00, 0x00, 0x32,
    0x00, 0x00, 0x00, 0x18, 0x3f, 0xa0, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00,
    0x3e, 0x80, 0x00, 0x00, 0x40, 0xf8, 0x00, 0x00, 0x40, 0x10, 0x00, 0x00,
    0x40, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static char *text_actor_bounds_lone = 
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_ACTORS                         # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*ACTOR            4" HOST_NL
    "  struct    br_actor" HOST_NL
    "    enum_8    BR_ACTOR_BOUNDS                          # type" HOST_NL
    "    enum_8    BR_RSTYLE_DEFAULT                        # render_style" HOST_NL
    "    asciz     \"test_actor_bounds\"                      # identifier" HOST_NL
    "*BOUNDS           3" HOST_NL
    "  struct    br_bounds3" HOST_NL
    "    vector3   1.25,1.75,0.25                           # min" HOST_NL
    "    vector3   7.75,2.25,2.0625                         # max" HOST_NL
    "*ACTOR_BOUNDS     0" HOST_NL
    "*END              0" HOST_NL;
static void test_v1dbfile_save_load_actor_bounds_lone() {
    br_actor* actor_ref;
    br_actor* actors_read[2];
    char name[128];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    int i;
    int k;
    int res;

    register_test_items();
    actor_ref = create_test_actor("test_actor_bounds", BR_ACTOR_BOUNDS, BR_TRANSFORM_IDENTITY, 0);

    create_temp_file(tmpfilename_binary, "actor_bounds_lone_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrActorSave(tmpfilename_binary, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);

    create_temp_file(tmpfilename_text, "actor_bounds_lone_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrActorSave(tmpfilename_text, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);
    
    LOG_DEBUG("Read binary actor");
    actors_read[0] = BrActorLoad(tmpfilename_binary);
    TEST_ASSERT_NOT_NULL(actors_read[0]);
    LOG_DEBUG("Read text actor");
    actors_read[1] = BrActorLoad(tmpfilename_text);
    TEST_ASSERT_NOT_NULL(actors_read[1]);

    for (i = 0; i < BR_ASIZE(actors_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_COMPARE_ACTOR(actor_ref, actors_read[i]);
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_actor_bounds_lone, tmpfilename_binary, sizeof(binary_actor_bounds_lone));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_actor_bounds_lone, tmpfilename_text);

    BrResFree(actor_ref);
    BrResFree(actors_read[0]);
    BrResFree(actors_read[1]);
}

static br_uint_8 binary_actor_clipplane_lone[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x16,
    0x07, 0x00, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x61, 0x63, 0x74, 0x6f, 0x72,
    0x5f, 0x63, 0x6c, 0x69, 0x70, 0x70, 0x6c, 0x61, 0x6e, 0x65, 0x00, 0x00,
    0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x10, 0x3f, 0x80, 0x00, 0x00, 0x3f,
    0xa0, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
};
static char *text_actor_clipplane_lone = 
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_ACTORS                         # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*ACTOR            4" HOST_NL
    "  struct    br_actor" HOST_NL
    "    enum_8    BR_ACTOR_CLIP_PLANE                      # type" HOST_NL
    "    enum_8    BR_RSTYLE_DEFAULT                        # render_style" HOST_NL
    "    asciz     \"test_actor_clipplane\"                   # identifier" HOST_NL
    "*PLANE            2" HOST_NL
    "  struct    br_plane" HOST_NL
    "    vector4   1,1.25,1.5,1.75                          # eqn" HOST_NL
    "*ACTOR_CLIP_PLANE 0" HOST_NL
    "*END              0" HOST_NL;
static void test_v1dbfile_save_load_actor_clipplane_lone() {
    br_actor* actor_ref;
    br_actor* actors_read[2];
    char name[128];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    int i;
    int k;
    int res;

    register_test_items();
    actor_ref = create_test_actor("test_actor_clipplane", BR_ACTOR_CLIP_PLANE, BR_TRANSFORM_IDENTITY, 0);

    create_temp_file(tmpfilename_binary, "actor_clipplane_lone_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrActorSave(tmpfilename_binary, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);

    create_temp_file(tmpfilename_text, "actor_clipplane_lone_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrActorSave(tmpfilename_text, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);
    
    LOG_DEBUG("Read binary actor");
    actors_read[0] = BrActorLoad(tmpfilename_binary);
    TEST_ASSERT_NOT_NULL(actors_read[0]);
    LOG_DEBUG("Read text actor");
    actors_read[1] = BrActorLoad(tmpfilename_text);
    TEST_ASSERT_NOT_NULL(actors_read[1]);

    for (i = 0; i < BR_ASIZE(actors_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_COMPARE_ACTOR(actor_ref, actors_read[i]);
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_actor_clipplane_lone, tmpfilename_binary, sizeof(binary_actor_clipplane_lone));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_actor_clipplane_lone, tmpfilename_text);

    BrResFree(actor_ref);
    BrResFree(actors_read[0]);
    BrResFree(actors_read[1]);
}

static br_uint_8 binary_actor_tree_lone[] = {
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x11,
    0x00, 0x00, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x61, 0x63, 0x74, 0x6f, 0x72,
    0x5f, 0x74, 0x72, 0x65, 0x65, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00,
    0x00, 0x08, 0x00, 0x00, 0x63, 0x68, 0x69, 0x6c, 0x64, 0x31, 0x00, 0x00,
    0x00, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00,
    0x00, 0x00, 0x08, 0x00, 0x00, 0x63, 0x68, 0x69, 0x6c, 0x64, 0x32, 0x00,
    0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};
static char *text_actor_tree_lone = 
    "*FILE_INFO        3" HOST_NL
    "  struct    file_info" HOST_NL
    "    enum_32   FILE_TYPE_ACTORS                         # type" HOST_NL
    "    uint_32   2                                        # version" HOST_NL
    "*ACTOR            4" HOST_NL
    "  struct    br_actor" HOST_NL
    "    enum_8    BR_ACTOR_NONE                            # type" HOST_NL
    "    enum_8    BR_RSTYLE_DEFAULT                        # render_style" HOST_NL
    "    asciz     \"test_actor_tree\"                        # identifier" HOST_NL
    "*ACTOR            4" HOST_NL
    "  struct    br_actor" HOST_NL
    "    enum_8    BR_ACTOR_NONE                            # type" HOST_NL
    "    enum_8    BR_RSTYLE_DEFAULT                        # render_style" HOST_NL
    "    asciz     \"child1\"                                 # identifier" HOST_NL
    "*ACTOR_ADD_CHILD  0" HOST_NL
    "*ACTOR            4" HOST_NL
    "  struct    br_actor" HOST_NL
    "    enum_8    BR_ACTOR_NONE                            # type" HOST_NL
    "    enum_8    BR_RSTYLE_DEFAULT                        # render_style" HOST_NL
    "    asciz     \"child2\"                                 # identifier" HOST_NL
    "*ACTOR_ADD_CHILD  0" HOST_NL
    "*END              0" HOST_NL;
static void test_v1dbfile_save_load_actor_tree_lone() {
    br_actor* actor_ref;
    br_actor* actors_read[2];
    br_actor* child;
    char name[128];
    char tmpfilename_binary[PATH_MAX+1];
    char tmpfilename_text[PATH_MAX+1];
    int i;
    int k;
    int res;

    register_test_items();
    actor_ref = create_test_actor("test_actor_tree", BR_ACTOR_NONE, BR_TRANSFORM_IDENTITY, 0);

    BrActorAdd(actor_ref, create_test_actor("child1", BR_ACTOR_NONE, BR_TRANSFORM_IDENTITY, 1));
    BrActorAdd(actor_ref, create_test_actor("child2", BR_ACTOR_NONE, BR_TRANSFORM_IDENTITY, 1));

    create_temp_file(tmpfilename_binary, "actor_tree_lone_binary");
    BrWriteModeSet(BR_FS_MODE_BINARY);
    res = BrActorSave(tmpfilename_binary, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);

    create_temp_file(tmpfilename_text, "actor_tree_lone_text");
    BrWriteModeSet(BR_FS_MODE_TEXT);
    res = BrActorSave(tmpfilename_text, actor_ref);
    TEST_ASSERT_EQUAL_INT(1, res);
    
    LOG_DEBUG("Read binary actor");
    actors_read[0] = BrActorLoad(tmpfilename_binary);
    TEST_ASSERT_NOT_NULL(actors_read[0]);
    LOG_DEBUG("Read text actor");
    actors_read[1] = BrActorLoad(tmpfilename_text);
    TEST_ASSERT_NOT_NULL(actors_read[1]);

    for (i = 0; i < BR_ASIZE(actors_read); i++) {
        LOG_DEBUG("Iteration %d (binary=0 text=1)", i);
        TEST_COMPARE_ACTOR(actor_ref, actors_read[i]);
    }
    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_actor_tree_lone, tmpfilename_binary, sizeof(binary_actor_tree_lone));
    TEST_ASSERT_EQUAL_FILE_TEXT(text_actor_tree_lone, tmpfilename_text);

    BrResFree(actor_ref);
    BrResFree(actors_read[0]);
    BrResFree(actors_read[1]);
}

void test_v1dbfile_BrModelLoad() {
    REQUIRES_DATA_DIRECTORY();

    br_model* m;
    m = BrModelLoad("DATA/MODELS/CPOINT.DAT");
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL_STRING("DIRECT.DAT", m->identifier);
    TEST_ASSERT_NOT_NULL(m->vertices);
    TEST_ASSERT_EQUAL_INT(54, m->nvertices);
    TEST_ASSERT_EQUAL_FLOAT(-0.0950154, m->vertices[0].p.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-0.030865, m->vertices[0].p.v[1]);
    TEST_ASSERT_EQUAL_FLOAT(-0.0157602, m->vertices[0].p.v[2]);
    TEST_ASSERT_EQUAL_INT(44, m->nfaces);
    TEST_ASSERT_NOT_NULL(m->faces);
    TEST_ASSERT_EQUAL_INT(26, m->faces[0].vertices[0]);
    TEST_ASSERT_EQUAL_INT(53, m->faces[0].vertices[1]);
    TEST_ASSERT_EQUAL_INT(43, m->faces[0].vertices[2]);
    BrModelFree(m);
}

void test_v1dbfile_BrActorLoad() {
    REQUIRES_DATA_DIRECTORY();

    br_actor* a;
    a = BrActorLoad("DATA/ACTORS/CPOINT.ACT");
    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_EQUAL_STRING("CPOINT.ACT", a->identifier);
    TEST_ASSERT_EQUAL_INT(1, a->type);
    BrActorFree(a);
}

void test_v1dbfile_BrMaterialLoad() {
    REQUIRES_DATA_DIRECTORY();

    br_material* m;
    br_pixelmap* pm;

    pm = BrPixelmapLoad("DATA/REG/PIXELMAP/OILSMEAR.PIX");
    TEST_ASSERT_NOT_NULL(pm);
    TEST_ASSERT_EQUAL_STRING("oilsmear.pix", pm->identifier);
    BrMapAdd(pm);

    m = BrMaterialLoad("DATA/REG/MATERIAL/OILSMEAR.MAT");
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL_INT(1, m->map_transform.m[0][0]);
    TEST_ASSERT_EQUAL_INT(0, m->map_transform.m[0][1]);
    TEST_ASSERT_EQUAL_INT(0, m->map_transform.m[1][0]);
    TEST_ASSERT_EQUAL_INT(1, m->map_transform.m[1][1]);
    TEST_ASSERT_EQUAL_INT(0, m->map_transform.m[2][0]);
    TEST_ASSERT_EQUAL_INT(0, m->map_transform.m[2][1]);

    TEST_ASSERT_EQUAL_PTR(pm, m->colour_map);
}

void test_v1dbfile_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_v1dbfile_save_load_material_lone);
    RUN_TEST(test_v1dbfile_save_load_material_many);
    RUN_TEST(test_v1dbfile_save_load_model_lone);
    RUN_TEST(test_v1dbfile_save_load_model_many);
    RUN_TEST(test_v1dbfile_save_load_actor_none_transform_unity_lone);
    RUN_TEST(test_v1dbfile_save_load_actor_none_transform_matrix34_lone);
    RUN_TEST(test_v1dbfile_save_load_actor_none_transform_matrix34lp_lone);
    RUN_TEST(test_v1dbfile_save_load_actor_none_transform_quat_lone);
    RUN_TEST(test_v1dbfile_save_load_actor_none_transform_lookup_lone);
    RUN_TEST(test_v1dbfile_save_load_actor_none_transform_euler_lone);
    RUN_TEST(test_v1dbfile_save_load_actor_model_lone);
    RUN_TEST(test_v1dbfile_save_load_actor_light_lone);
    RUN_TEST(test_v1dbfile_save_load_actor_camera_lone);
    RUN_TEST(test_v1dbfile_save_load_actor_bounds_lone);
    RUN_TEST(test_v1dbfile_save_load_actor_clipplane_lone);
    RUN_TEST(test_v1dbfile_save_load_actor_tree_lone);
    RUN_TEST(test_v1dbfile_BrModelLoad);
    RUN_TEST(test_v1dbfile_BrActorLoad);
    RUN_TEST(test_v1dbfile_BrMaterialLoad);
}
