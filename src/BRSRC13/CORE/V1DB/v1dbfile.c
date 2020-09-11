#include "v1dbfile.h"

#include "CORE/FW/datafile.h"
#include "CORE/FW/genfile.h"
#include "CORE/FW/resource.h"
#include "CORE/V1DB/actsupt.h"
#include "CORE/V1DB/dbsetup.h"
#include "CORE/V1DB/matsupt.h"
#include "CORE/V1DB/modsupt.h"
#include "CORE/V1DB/regsupt.h"
#include "harness.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

br_chunks_table_entry MaterialLoadEntries[8] = {
    { 0u, 0u, &FopRead_END },
    { 4u, 0u, &FopRead_MATERIAL_OLD },
    { 28u, 0u, &FopRead_PIXELMAP_REF },
    { 30u, 0u, &FopRead_PIXELMAP_REF },
    { 31u, 0u, &FopRead_PIXELMAP_REF },
    { 32u, 0u, &FopRead_PIXELMAP_REF },
    { 59u, 0u, &FopRead_PIXELMAP_REF },
    { 60u, 0u, &FopRead_MATERIAL }
};

br_chunks_table MaterialLoadTable = { 8, MaterialLoadEntries };

br_file_struct br_camera_F;
br_file_enum camera_type_F;

br_file_struct_member br_camera_FM[6];
br_file_enum_member camera_type_FM[2];
br_file_struct br_light_F;
br_file_enum light_type_F;
br_file_enum_member light_type_FM[6];
br_file_struct_member br_light_FM[8];
br_file_struct br_plane_F;
br_file_struct_member br_bounds3_FM[2];
br_file_struct br_bounds3_F;
br_file_struct_member br_plane_FM[1];
transform_type TransformTypes[7];
br_file_struct br_transform_translation_F;
br_file_struct_member br_transform_translation_FM[1];
br_file_enum angle_order_F;
br_file_struct_member br_transform_euler_FM[5];
br_file_struct br_transform_look_up_F;
br_file_struct_member br_transform_look_up_FM[3];
br_file_struct br_transform_euler_F;
br_file_struct br_transform_quat_F;
br_file_struct_member br_transform_quat_FM[5];
br_file_enum_member angle_order_FM[24];
br_file_struct br_transform_matrix34_F;
br_file_struct_member br_transform_matrix34_FM[4];
br_file_enum_member render_style_FM[8];

br_file_enum_member actor_type_FM[8] = {
    { 0, "BR_ACTOR_NONE" },
    { 1, "BR_ACTOR_MODEL" },
    { 2, "BR_ACTOR_LIGHT" },
    { 3, "BR_ACTOR_CAMERA" },
    { 4, "_BR_ACTOR_RESERVED" },
    { 5, "BR_ACTOR_BOUNDS" },
    { 6, "BR_ACTOR_BOUNDS_CORRECT" },
    { 7, "BR_ACTOR_CLIP_PLANE" }
};
br_file_enum actor_type_F = { 8u, actor_type_FM };

br_file_struct_member br_actor_FM[3] = {
    { 13u, offsetof(br_actor, type), "type", NULL /*actor_type_FM */ },
    { 13u, offsetof(br_actor, render_style), NULL /*render_style_FM*/ },
    { 17u, offsetof(br_actor, identifier), NULL }
};
br_file_struct br_actor_F = { "br_actor", 3u, br_actor_FM, sizeof(br_actor) };
br_file_enum render_style_F;

br_file_struct_member br_material_FM[17];
br_file_struct br_material_F;

br_file_struct_member br_material_old_FM[13] = {
    { 18u, offsetof(br_material, colour), "colour", NULL },
    { 1u, offsetof(br_material, opacity), "opacity", NULL },
    { 12u, offsetof(br_material, ka), "ka", NULL },
    { 12u, offsetof(br_material, kd), "kd", NULL },
    { 12u, offsetof(br_material, ks), "ks", NULL },
    { 10u, offsetof(br_material, power), "power", NULL },
    { 3u, offsetof(br_material, flags), "flags", NULL },
    { 19u, offsetof(br_material, map_transform), "map_transform.m[0]", NULL },
    { 19u, offsetof(br_material, map_transform) + sizeof(br_scalar) * 2, "map_transform.m[1]", NULL },
    { 19u, offsetof(br_material, map_transform) + sizeof(br_scalar) * 4, "map_transform.m[2]", NULL },
    { 1u, offsetof(br_material, index_base), "index_base", NULL },
    { 1u, offsetof(br_material, index_range), "index_range", NULL },
    { 17u, offsetof(br_material, identifier), "identifier", NULL }
};
br_file_struct br_material_old_F = { "br_material_old", 13, br_material_old_FM, sizeof(br_material) };

br_file_struct_member br_pivot_FM[3];
br_file_struct br_pivot_F;

br_file_struct_member br_old_model_1_FM[1];
br_file_struct br_old_model_1_F;

br_file_struct_member br_model_FM[2] = {
    { 3u, offsetof(br_model, flags), "flags", NULL },
    { 17u, offsetof(br_model, identifier), "identifier", NULL }
};
br_file_struct br_model_F = { "br_model", 2u, br_model_FM, sizeof(br_model) };

br_file_struct_member br_old_face_FM[5];
br_file_struct br_old_face_F;

br_file_struct_member br_old_face_1_FM[5];
br_file_struct br_old_face_1_F;

br_file_struct_member br_face_FM[5] = {
    { 3u, offsetof(br_face, vertices), "vertices[0]", NULL },
    { 3u, offsetof(br_face, vertices) + 2, "vertices[1]", NULL },
    { 3u, offsetof(br_face, vertices) + 4, "vertices[2]", NULL },
    { 3u, offsetof(br_face, smoothing), "smoothing", NULL },
    { 1u, offsetof(br_face, flags), "flags", NULL }
};
br_file_struct br_face_F = { "br_face", 5u, br_face_FM, sizeof(br_face) };

br_file_struct_member br_old_vertex_uv_FM[5];
br_file_struct br_old_vertex_uv_F;

br_file_struct_member br_vertex_uv_FM[2];
br_file_struct br_vertex_uv_F;

br_file_struct_member br_vertex_FM[3] = {
    { 10, offsetof(br_vertex, p) + 0, "p.v[x]", NULL },
    { 10, offsetof(br_vertex, p) + 4, "p.v[y]", NULL },
    { 10, offsetof(br_vertex, p) + 8, "p.v[z]", NULL }
};
br_file_struct br_vertex_F = { "br_vertex", 3, br_vertex_FM, sizeof(br_vertex) };

br_chunks_table_entry ModelLoadEntries[15] = {
    { 0u, 0u, &FopRead_END },
    { 9u, 0u, &FopRead_OLD_MATERIAL_INDEX },
    { 10u, 0u, &FopRead_OLD_VERTICES },
    { 11u, 0u, &FopRead_OLD_VERTICES_UV },
    { 12u, 0u, &FopRead_OLD_FACES },
    { 13u, 0u, &FopRead_OLD_MODEL },
    { 25u, 1u, &FopRead_OLD_FACES_1 },
    { 27u, 0u, &FopRead_OLD_MODEL_1 },
    { 54u, 0u, &FopRead_MODEL },
    { 22u, 1u, &FopRead_MATERIAL_INDEX },
    { 23u, 1u, &FopRead_VERTICES },
    { 24u, 1u, &FopRead_VERTEX_UV },
    { 53u, 1u, &FopRead_FACES },
    { 26u, 0u, &FopRead_FACE_MATERIAL },
    { 21u, 0u, &FopRead_PIVOT }
};
br_chunks_table ModelLoadTable = { 15, ModelLoadEntries };

br_chunks_table_entry ActorLoadEntries[21] = {
    { 0u, 0u, &FopRead_END },
    { 35u, 0u, &FopRead_ACTOR },
    { 36u, 0u, &FopRead_ACTOR_MODEL },
    { 37u, 0u, &FopRead_ACTOR_TRANSFORM },
    { 38u, 0u, &FopRead_ACTOR_MATERIAL },
    { 39u, 0u, &FopRead_ACTOR_LIGHT },
    { 40u, 0u, &FopRead_ACTOR_CAMERA },
    { 41u, 0u, &FopRead_ACTOR_BOUNDS },
    { 55u, 0u, &FopRead_ACTOR_CLIP_PLANE },
    { 42u, 0u, &FopRead_ACTOR_ADD_CHILD },
    { 43u, 0u, &FopRead_TRANSFORM },
    { 44u, 0u, &FopRead_TRANSFORM },
    { 45u, 0u, &FopRead_TRANSFORM },
    { 46u, 0u, &FopRead_TRANSFORM },
    { 47u, 0u, &FopRead_TRANSFORM },
    { 48u, 0u, &FopRead_TRANSFORM },
    { 49u, 0u, &FopRead_TRANSFORM },
    { 50u, 0u, &FopRead_BOUNDS },
    { 51u, 0u, &FopRead_LIGHT },
    { 52u, 0u, &FopRead_CAMERA },
    { 56u, 0u, &FopRead_PLANE }
};
br_chunks_table ActorLoadTable = { 21, ActorLoadEntries };

struct {
    br_uint_32 id;
    size_t offset;
    int table;
} MaterialMaps[4] = {
    { 28u, offsetof(br_material, colour_map), 0 },
    { 30u, offsetof(br_material, index_blend), 1 },
    { 31u, offsetof(br_material, index_shade), 1 },
    { 32u, offsetof(br_material, screendoor), 1 }
};

// Added by JeffH.
#define DF_MATERIAL 3
#define DF_ACTOR 4
#define DF_MODEL 8
#define DF_TRANSFORM 16

char rscid[53];

// Offset: 18
// Size: 139
int FopWrite_VERTICES(br_datafile* df, br_vertex* vertices, int nvertices) {
    NOT_IMPLEMENTED();
}

// Offset: 174
// Size: 126
int FopRead_VERTICES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    int i;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    mp = DfTop(DF_MODEL, NULL);
    mp->vertices = BrResAllocate(mp, sizeof(br_vertex) * count, BR_MEMORY_VERTICES);
    DfStructReadArray(df, &br_vertex_F, mp->vertices, count);
    for (i = 0; i < count; i++) {
        LOG_DEBUG("vert %f, %f, %f", mp->vertices[i].p.v[0], mp->vertices[i].p.v[1], mp->vertices[i].p.v[2]);
    }
    mp->nvertices = count;
    return 0;
}

// Offset: 321
// Size: 144
int FopRead_OLD_VERTICES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    void* ptr;
    NOT_IMPLEMENTED();
}

// Offset: 484
// Size: 139
int FopWrite_VERTEX_UV(br_datafile* df, br_vertex* vertices, int nvertices) {
    NOT_IMPLEMENTED();
}

// Offset: 641
// Size: 118
int FopRead_VERTEX_UV(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    NOT_IMPLEMENTED();
}

// Offset: 783
// Size: 144
int FopRead_OLD_VERTICES_UV(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    void* ptr;
    NOT_IMPLEMENTED();
}

// Offset: 950
// Size: 177
int FopRead_MATERIAL_INDEX(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    char name[256];
    br_material** mip;
    br_uint_32 i;
    NOT_IMPLEMENTED();
}

// Offset: 1151
// Size: 208
int FopWrite_MATERIAL_INDEX(br_datafile* df, br_material** materials, int nmaterials) {
    int i;
    int s;
    NOT_IMPLEMENTED();
}

// Offset: 1386
// Size: 289
int FopRead_OLD_MATERIAL_INDEX(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    char* mblock;
    char* cp;
    int i;
    int num_materials;
    br_material** mip;
    NOT_IMPLEMENTED();
}

// Offset: 1689
// Size: 207
int FopRead_FACES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    int i;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    mp = DfTop(DF_MODEL, 0);
    mp->faces = (br_face*)BrResAllocate(mp, sizeof(br_face) * count, BR_MEMORY_FACES);
    mp->nfaces = count;
    DfStructReadArray(df, &br_face_F, mp->faces, count);
    for (i = 0; i < mp->nfaces; i++) {
        if (!mp->faces[i].smoothing) {
            mp->faces[i].smoothing = -1;
        }
    }

    for (i = 0; i < count; i++) {
        LOG_DEBUG("face %d, %d, %d %d", mp->faces[i].vertices[0], mp->faces[i].vertices[1], mp->faces[i].vertices[2], mp->faces[i].flags);
    }

    return 0;
}

// Offset: 1911
// Size: 139
int FopWrite_FACES(br_datafile* df, br_face* faces, int nfaces) {
    NOT_IMPLEMENTED();
}

// Offset: 2070
// Size: 271
int FopRead_OLD_FACES_1(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 2359
// Size: 292
int FopRead_OLD_FACES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_face* fp;
    br_material** mip;
    int mi_count;
    unsigned int i;
    void* ptr;
    NOT_IMPLEMENTED();
}

// Offset: 2674
// Size: 293
int FopWrite_FACE_MATERIAL(br_datafile* df, br_face* faces, int nfaces, br_material** mindex, int nmaterials) {
    br_uint_16* block;
    br_uint_16* ip;
    br_face* fp;
    int i;
    int j;
    NOT_IMPLEMENTED();
}

// Offset: 2989
// Size: 303
int FopRead_FACE_MATERIAL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_material** mindex;
    int nmaterials;
    br_model* mp;
    br_face* fp;
    br_uint_16* block;
    br_uint_16* ip;
    int block_count;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 3306
// Size: 132
int FopRead_MODEL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    mp = BrModelAllocate(NULL, 0, 0);
    mp->identifier = NULL;
    df->res = mp;
    df->prims->struct_read(df, &br_model_F, mp);
    df->res = NULL;
    mp->flags &= (BR_MODF_DONT_WELD | BR_MODF_KEEP_ORIGINAL | BR_MODF_GENERATE_TAGS | BR_MODF_QUICK_UPDATE);
    DfPush(DF_MODEL, mp, 1);
    return 0;
}

// Offset: 3453
// Size: 125
int FopWrite_MODEL(br_datafile* df, br_model* mp) {
    br_model temp_model;
    NOT_IMPLEMENTED();
}

// Offset: 3598
// Size: 124
int FopRead_OLD_MODEL_1(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    NOT_IMPLEMENTED();
}

// Offset: 3740
// Size: 230
int FopRead_OLD_MODEL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 3984
// Size: 122
int FopRead_PIVOT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    NOT_IMPLEMENTED();
}

// Offset: 4127
// Size: 120
int FopRead_MATERIAL_OLD(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_material* mp;
    mp = BrMaterialAllocate(NULL);
    df->res = mp;
    df->prims->struct_read(df, &br_material_old_F, mp);
    df->res = NULL;
    DfPush(DF_MATERIAL, mp, 1);
    return 0;
}

// Offset: 4264
// Size: 120
int FopRead_MATERIAL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_material* mp;
    NOT_IMPLEMENTED();
}

// Offset: 4402
// Size: 109
int FopWrite_MATERIAL(br_datafile* df, br_material* mp) {
    NOT_IMPLEMENTED();
}

// Offset: 4532
// Size: 227
int FopRead_PIXELMAP_REF(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_pixelmap* pm;
    char name[256];
    char* mp;
    int i;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    for (i = 0; i < 5; i++) {

        if (id == MaterialMaps[i].id) {
            break;
        }
    }

    mp = DfTop(DF_MATERIAL, NULL);
    df->prims->name_read(df, name);
    if (MaterialMaps[i].table) {
        pm = BrTableFind(name);
    } else {
        pm = BrMapFind(name);
    }
    *(intptr_t*)(mp + MaterialMaps[i].offset) = (intptr_t*)pm;
    return 0;
}

// Offset: 4781
// Size: 144
int FopWrite_PIXELMAP_REF(br_datafile* df, int id, br_pixelmap* pixelmap) {
    NOT_IMPLEMENTED();
}

// Offset: 4940
// Size: 109
int FopWrite_ACTOR(br_datafile* df, br_actor* ap) {
    NOT_IMPLEMENTED();
}

// Offset: 5063
// Size: 132
int FopRead_ACTOR(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* ap;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    ap = BrActorAllocate(BR_ACTOR_NONE, NULL);
    df->res = ap;
    df->prims->struct_read(df, &br_actor_F, ap);
    df->res = NULL;
    ap->t.type = BR_TRANSFORM_IDENTITY;
    DfPush(DF_ACTOR, ap, 1);
    return 0;
}

// Offset: 5216
// Size: 145
int FopWrite_ACTOR_MODEL(br_datafile* df, br_model* model) {
    NOT_IMPLEMENTED();
}

// Offset: 5381
// Size: 100
int FopRead_ACTOR_MODEL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    char name[256];
    br_actor* a;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    a = DfTop(DF_ACTOR, 0);
    df->prims->name_read(df, name);
    a->model = BrModelFind(name);
    return 0;
}

// Offset: 5505
// Size: 145
int FopWrite_ACTOR_MATERIAL(br_datafile* df, br_material* material) {
    NOT_IMPLEMENTED();
}

// Offset: 5673
// Size: 100
int FopRead_ACTOR_MATERIAL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    char name[256];
    br_actor* a;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    a = DfTop(DF_ACTOR, NULL);
    df->prims->name_read(df, name);
    a->material = BrMaterialFind(name);
}

// Offset: 5798
// Size: 66
int FopWrite_ACTOR_TRANSFORM(br_datafile* df) {
    NOT_IMPLEMENTED();
}

// Offset: 5888
// Size: 108
int FopRead_ACTOR_TRANSFORM(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_transform* tp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    tp = DfPop(DF_TRANSFORM, NULL);
    a = DfTop(DF_ACTOR, NULL);
    memcpy(&a->t, tp, sizeof(br_transform));
    BrResFree(tp);
    return 0;
}

// Offset: 6017
// Size: 66
int FopWrite_ACTOR_LIGHT(br_datafile* df) {
    NOT_IMPLEMENTED();
}

// Offset: 6103
// Size: 89
int FopRead_ACTOR_LIGHT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_light* lp;
    NOT_IMPLEMENTED();
}

// Offset: 6214
// Size: 66
int FopWrite_ACTOR_CAMERA(br_datafile* df) {
    NOT_IMPLEMENTED();
}

// Offset: 6301
// Size: 89
int FopRead_ACTOR_CAMERA(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_light* cp;
    NOT_IMPLEMENTED();
}

// Offset: 6412
// Size: 66
int FopWrite_ACTOR_BOUNDS(br_datafile* df) {
    NOT_IMPLEMENTED();
}

// Offset: 6499
// Size: 89
int FopRead_ACTOR_BOUNDS(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_bounds* bp;
    NOT_IMPLEMENTED();
}

// Offset: 6614
// Size: 66
int FopWrite_ACTOR_CLIP_PLANE(br_datafile* df) {
    NOT_IMPLEMENTED();
}

// Offset: 6705
// Size: 89
int FopRead_ACTOR_CLIP_PLANE(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_vector4* vp;
    NOT_IMPLEMENTED();
}

// Offset: 6819
// Size: 66
int FopWrite_ACTOR_ADD_CHILD(br_datafile* df) {
    NOT_IMPLEMENTED();
}

// Offset: 6909
// Size: 96
int FopRead_ACTOR_ADD_CHILD(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_actor* p;
    NOT_IMPLEMENTED();
}

// Offset: 7024
// Size: 197
int FopWrite_TRANSFORM(br_datafile* df, br_transform* t) {
    transform_type* tt;
    NOT_IMPLEMENTED();
}

// Offset: 7239
// Size: 234
int FopRead_TRANSFORM(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    int t;
    br_transform* tp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    for (t = 0; t < 7; t++) {
        if (id == TransformTypes[t].id) {
            break;
        }
    }
    tp = (br_transform*)BrResAllocate(v1db.res, sizeof(br_transform), BR_MEMORY_TRANSFORM);
    tp->type = t;
    df->res = tp;
    if (TransformTypes[t].fs) {
        df->prims->struct_read(df, TransformTypes[t].fs, tp);
    }
    df->res = NULL;
    DfPush(DF_TRANSFORM, tp, 1);
    return 0;
}

// Offset: 7489
// Size: 109
int FopWrite_BOUNDS(br_datafile* df, br_bounds* bp) {
    NOT_IMPLEMENTED();
}

// Offset: 7613
// Size: 132
int FopRead_BOUNDS(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_bounds3* bp;
    NOT_IMPLEMENTED();
}

// Offset: 7760
// Size: 109
int FopWrite_PLANE(br_datafile* df, br_vector4* pp) {
    NOT_IMPLEMENTED();
}

// Offset: 7883
// Size: 132
int FopRead_PLANE(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_vector4* pp;
    NOT_IMPLEMENTED();
}

// Offset: 8030
// Size: 109
int FopWrite_LIGHT(br_datafile* df, br_light* lp) {
    NOT_IMPLEMENTED();
}

// Offset: 8153
// Size: 132
int FopRead_LIGHT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_light* lp;
    NOT_IMPLEMENTED();
}

// Offset: 8301
// Size: 109
int FopWrite_CAMERA(br_datafile* df, br_camera* cp) {
    NOT_IMPLEMENTED();
}

// Offset: 8425
// Size: 132
int FopRead_CAMERA(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_camera* cp;
    NOT_IMPLEMENTED();
}

// Offset: 8573
// Size: 162
br_uint_32 BrModelLoadMany(char* filename, br_model** models, br_uint_16 num) {
    int count;
    int r;
    br_datafile* df;
    LOG_TRACE("(\"%s\", %p, %d)", filename, models, num);

    df = DfOpen(filename, 0, BRT_FLOAT);
    if (!df) {
        return 0;
    }

    count = 0;
    do {
        if (count >= num) {
            break;
        }
        r = DfChunksInterpret(df, &ModelLoadTable);
        if (DfTopType() == DF_MODEL) {
            models[count] = DfPop(DF_MODEL, 0);
            count++;
        }
    } while (r);
    DfClose(df);
    return count;
}

// Offset: 8746
// Size: 82
int PtrCompare(void* a, void* b) {
    NOT_IMPLEMENTED();
}

// Offset: 8839
// Size: 720
br_uint_32 WriteModel(br_model* mp, br_datafile* df) {
    br_material** mindex;
    br_vertex* vp;
    int nmaterials;
    int i;
    int has_uv;
    NOT_IMPLEMENTED();
}

// Offset: 9575
// Size: 209
br_uint_32 BrModelSaveMany(char* filename, br_model** models, br_uint_16 num) {
    br_datafile* df;
    int i;
    int m;
    NOT_IMPLEMENTED();
}

// Offset: 9800
// Size: 162
br_uint_32 BrActorLoadMany(char* filename, br_actor** actors, br_uint_16 num) {
    br_datafile* df;
    int count;
    int r;
    LOG_TRACE("(\"%s\", %p, %d)", filename, actors, num);

    df = DfOpen(filename, 0, BRT_FLOAT);
    if (!df) {
        return 0;
    }

    count = 0;
    do {
        if (count >= num) {
            break;
        }
        r = DfChunksInterpret(df, &ActorLoadTable);
        if (DfTopType() == DF_ACTOR) {
            actors[count] = DfPop(DF_ACTOR, NULL);
            count++;
        }
    } while (r);
    DfClose(df);
    return count;
}

// Offset: 9973
// Size: 400
int WriteActor(br_actor* a, br_datafile* df) {
    br_actor* ap;
    br_actor* last_ap;
    NOT_IMPLEMENTED();
}

// Offset: 10389
// Size: 156
br_uint_32 BrActorSaveMany(char* filename, br_actor** actors, br_uint_16 num) {
    br_datafile* df;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 10564
// Size: 162
br_uint_32 BrMaterialLoadMany(char* filename, br_material** materials, br_uint_16 num) {
    br_datafile* df;
    int count = 0;
    int r = 0;

    df = DfOpen(filename, 0, BRT_FLOAT);
    if (df) {
        do {
            if (count >= num) {
                break;
            }
            r = DfChunksInterpret(df, &MaterialLoadTable);
            if (DfTopType() == DF_MATERIAL) {
                materials[count] = DfPop(DF_MATERIAL, 0);
                ++count;
            }
            LOG_DEBUG("r=%d", r);
        } while (r);
        DfClose(df);
    }
    return count;
}

// Offset: 10740
// Size: 199
br_uint_32 WriteMaterial(br_material* mp, br_datafile* df) {
    NOT_IMPLEMENTED();
}

// Offset: 10958
// Size: 202
br_uint_32 BrMaterialSaveMany(char* filename, br_material** materials, br_uint_16 num) {
    br_datafile* df;
    int i;
    int count;
    NOT_IMPLEMENTED();
}

// Offset: 11172
// Size: 81
br_model* BrModelLoad(char* filename) {
    br_model* ptr;
    LOG_TRACE("(\"%s\")", filename);

    if (BrModelLoadMany(filename, &ptr, 1u) == 1) {
        return ptr;
    }
    return NULL;
}

// Offset: 11265
// Size: 58
br_uint_32 BrModelSave(char* filename, br_model* ptr) {
    NOT_IMPLEMENTED();
}

// Offset: 11338
// Size: 81
br_material* BrMaterialLoad(char* filename) {
    br_material* ptr;
    if (BrMaterialLoadMany(filename, &ptr, 1) == 1) {
        return ptr;
    }
    return NULL;
}

// Offset: 11434
// Size: 58
br_uint_32 BrMaterialSave(char* filename, br_material* ptr) {
    NOT_IMPLEMENTED();
}

// Offset: 11504
// Size: 81
br_actor* BrActorLoad(char* filename) {
    br_actor* ptr;
    if (BrActorLoadMany(filename, &ptr, 1u) == 1) {
        return ptr;
    }
    return NULL;
}

// Offset: 11597
// Size: 58
br_uint_32 BrActorSave(char* filename, br_actor* ptr) {
    NOT_IMPLEMENTED();
}

// Offset: 11672
// Size: 174
br_error BrModelFileCount(char* filename, br_uint_16* num) {
    br_datafile* df;
    NOT_IMPLEMENTED();
}

// Offset: 11863
// Size: 174
br_error BrActorFileCount(char* filename, br_uint_16* num) {
    br_datafile* df;
    NOT_IMPLEMENTED();
}

// Offset: 12057
// Size: 174
br_error BrMaterialFileCount(char* filename, br_uint_16* num) {
    br_datafile* df;
    NOT_IMPLEMENTED();
}
