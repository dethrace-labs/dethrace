#include "v1dbfile.h"

#include "CORE/FW/datafile.h"
#include "CORE/FW/diag.h"
#include "CORE/FW/genfile.h"
#include "CORE/FW/resource.h"
#include "CORE/FW/scratch.h"
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
br_file_struct_member br_bounds3_FM[2] = { { 20u, 0u, "min", NULL }, { 20u, 12u, "max", NULL } };
br_file_struct br_bounds3_F = { "br_bounds3", 2u, br_bounds3_FM, 24 };
br_file_struct_member br_plane_FM[1];
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

br_file_struct_member br_transform_matrix34_FM[] = {
    { 20u, offsetof(br_transform, t.mat.m[0]), "t.mat.m[0]", NULL },
    { 20u, offsetof(br_transform, t.mat.m[1]), "t.mat.m[1]", NULL },
    { 20u, offsetof(br_transform, t.mat.m[2]), "t.mat.m[2]", NULL },
    { 20u, offsetof(br_transform, t.mat.m[3]), "t.mat.m[3]", NULL }
};
br_file_struct br_transform_matrix34_F = { "br_transform_matrix34", 4u, br_transform_matrix34_FM, sizeof(br_transform) };

transform_type TransformTypes[] = {
    { 43u, &br_transform_matrix34_F },
    { 44u, &br_transform_matrix34_F },
    { 45u, &br_transform_quat_F },
    { 46u, &br_transform_euler_F },
    { 47u, &br_transform_look_up_F },
    { 48u, &br_transform_translation_F },
    { 49u, NULL }
};

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
    { 3u, offsetof(br_face, vertices[1]), "vertices[1]", NULL },
    { 3u, offsetof(br_face, vertices[2]), "vertices[2]", NULL },
    { 3u, offsetof(br_face, smoothing), "smoothing", NULL },
    { 1u, offsetof(br_face, flags), "flags", NULL }
};
br_file_struct br_face_F = { "br_face", 5u, br_face_FM, sizeof(br_face) };

br_file_struct_member br_old_vertex_uv_FM[5];
br_file_struct br_old_vertex_uv_F;

br_file_struct_member br_vertex_uv_FM[] = {
    { 10, offsetof(br_vertex, map.v[0]), "map.v[0]", NULL },
    { 10u, offsetof(br_vertex, map.v[1]), "map.v[1]", NULL }
};
br_file_struct br_vertex_uv_F = { "br_vertex_uv", 2, br_vertex_uv_FM, sizeof(br_vertex) };

br_file_struct_member br_vertex_FM[3] = {
    { 10, offsetof(br_vertex, p.v[0]), "p.v[x]", NULL },
    { 10, offsetof(br_vertex, p.v[1]), "p.v[y]", NULL },
    { 10, offsetof(br_vertex, p.v[2]), "p.v[z]", NULL }
};
br_file_struct br_vertex_F = { "br_vertex", 3, br_vertex_FM, sizeof(br_vertex) };

br_chunks_table_entry ModelLoadEntries[] = {
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

// Added by Jeff.
#define DF_MATERIAL 3
#define DF_ACTOR 4
#define DF_MATERIAL_INDEX 5
#define DF_MODEL 8
#define DF_TRANSFORM 16
#define DF_BOUNDS 19
int gFace_order_key = 0;

char rscid[53];

// IDA: int __usercall FopWrite_VERTICES@<EAX>(br_datafile *df@<EAX>, br_vertex *vertices@<EDX>, int nvertices@<EBX>)
int FopWrite_VERTICES(br_datafile* df, br_vertex* vertices, int nvertices) {
    LOG_TRACE("(%p, %p, %d)", df, vertices, nvertices);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_VERTICES@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_VERTICES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    int i;
    LOG_TRACE9("(%p, %d, %d, %d)", df, id, length, count);

    mp = DfTop(DF_MODEL, NULL);
    mp->vertices = BrResAllocate(mp, sizeof(br_vertex) * count, BR_MEMORY_VERTICES);
    DfStructReadArray(df, &br_vertex_F, mp->vertices, count);
    mp->nvertices = count;
    return 0;
}

// IDA: int __usercall FopRead_OLD_VERTICES@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_OLD_VERTICES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    void* ptr;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopWrite_VERTEX_UV@<EAX>(br_datafile *df@<EAX>, br_vertex *vertices@<EDX>, int nvertices@<EBX>)
int FopWrite_VERTEX_UV(br_datafile* df, br_vertex* vertices, int nvertices) {
    LOG_TRACE("(%p, %p, %d)", df, vertices, nvertices);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_VERTEX_UV@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_VERTEX_UV(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    LOG_TRACE9("(%p, %d, %d, %d)", df, id, length, count);

    mp = DfTop(DF_MODEL, NULL);
    if (count > mp->nvertices) {
        BrFailure("Vertex UV: too many entries");
    }
    DfStructReadArray(df, &br_vertex_uv_F, mp->vertices, count);
    return 0;
}

// IDA: int __usercall FopRead_OLD_VERTICES_UV@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_OLD_VERTICES_UV(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    void* ptr;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_MATERIAL_INDEX@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_MATERIAL_INDEX(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    char name[256];
    br_material** mip;
    br_uint_32 i;
    LOG_TRACE9("(%p, %d, %d, %d)", df, id, length, count);

    mip = (br_material**)BrResAllocate(v1db.res, sizeof(intptr_t) * (count + 1), BR_MEMORY_MATERIAL_INDEX);
    mip[0] = NULL;
    for (i = 1; i < count + 1; i++) {
        df->prims->name_read(df, name);
        mip[i] = BrMaterialFind(name);
    }
    DfPush(DF_MATERIAL_INDEX, mip, count + 1);
    return 0;
}

// IDA: int __usercall FopWrite_MATERIAL_INDEX@<EAX>(br_datafile *df@<EAX>, br_material **materials@<EDX>, int nmaterials@<EBX>)
int FopWrite_MATERIAL_INDEX(br_datafile* df, br_material** materials, int nmaterials) {
    int i;
    int s;
    LOG_TRACE("(%p, %p, %d)", df, materials, nmaterials);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_OLD_MATERIAL_INDEX@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_OLD_MATERIAL_INDEX(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    char* mblock;
    char* cp;
    int i;
    int num_materials;
    br_material** mip;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_FACES@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_FACES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    int i;
    LOG_TRACE9("(%p, %d, %d, %d)", df, id, length, count);
    mp = DfTop(DF_MODEL, 0);
    mp->faces = (br_face*)BrResAllocate(mp, sizeof(br_face) * count, BR_MEMORY_FACES);
    mp->nfaces = count;
    DfStructReadArray(df, &br_face_F, mp->faces, count);
    for (i = 0; i < mp->nfaces; i++) {
        if (!mp->faces[i].smoothing) {
            mp->faces[i].smoothing = -1;
        }
        mp->faces[i].index = i;
        mp->faces[i].order_key = gFace_order_key++;
    }

    return 0;
}

// IDA: int __usercall FopWrite_FACES@<EAX>(br_datafile *df@<EAX>, br_face *faces@<EDX>, int nfaces@<EBX>)
int FopWrite_FACES(br_datafile* df, br_face* faces, int nfaces) {
    LOG_TRACE("(%p, %p, %d)", df, faces, nfaces);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_OLD_FACES_1@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_OLD_FACES_1(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    int i;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_OLD_FACES@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_OLD_FACES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_face* fp;
    br_material** mip;
    int mi_count;
    unsigned int i;
    void* ptr;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopWrite_FACE_MATERIAL@<EAX>(br_datafile *df@<EAX>, br_face *faces@<EDX>, int nfaces@<EBX>, br_material **mindex@<ECX>, int nmaterials)
int FopWrite_FACE_MATERIAL(br_datafile* df, br_face* faces, int nfaces, br_material** mindex, int nmaterials) {
    br_uint_16* block;
    br_uint_16* ip;
    br_face* fp;
    int i;
    int j;
    LOG_TRACE("(%p, %p, %d, %p, %d)", df, faces, nfaces, mindex, nmaterials);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_FACE_MATERIAL@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_FACE_MATERIAL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_material** mindex;
    int nmaterials;
    br_model* mp;
    br_face* fp;
    br_uint_16* block;
    br_uint_16* ip;
    int block_count;
    int i;
    LOG_TRACE9("(%p, %d, %d, %d)", df, id, length, count);

    mindex = DfPop(DF_MATERIAL_INDEX, &nmaterials);
    mp = DfTop(DF_MODEL, 0);

    block = BrScratchAllocate(length);
    block_count = mp->nfaces;
    block = df->prims->block_read(df, block, &block_count, sizeof(br_uint_16), 0);

    if (block_count > mp->nfaces)
        BrFailure("Face Materials: too many entries");

    fp = mp->faces;
    ip = block;
    for (i = 0; i < block_count; i++) {
        fp->material = (*ip < nmaterials) ? mindex[*ip] : NULL;
        fp++;
        ip++;
    }

    BrScratchFree(block);
    BrResFree(mindex);
    return 0;
}

// IDA: int __usercall FopRead_MODEL@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
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

// IDA: int __usercall FopWrite_MODEL@<EAX>(br_datafile *df@<EAX>, br_model *mp@<EDX>)
int FopWrite_MODEL(br_datafile* df, br_model* mp) {
    br_model temp_model;
    LOG_TRACE("(%p, %p)", df, mp);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_OLD_MODEL_1@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_OLD_MODEL_1(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_OLD_MODEL@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_OLD_MODEL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    int i;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_PIVOT@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_PIVOT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_MATERIAL_OLD@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_MATERIAL_OLD(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_material* mp;
    mp = BrMaterialAllocate(NULL);
    df->res = mp;
    df->prims->struct_read(df, &br_material_old_F, mp);
    df->res = NULL;
    DfPush(DF_MATERIAL, mp, 1);
    return 0;
}

// IDA: int __usercall FopRead_MATERIAL@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_MATERIAL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_material* mp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopWrite_MATERIAL@<EAX>(br_datafile *df@<EAX>, br_material *mp@<EDX>)
int FopWrite_MATERIAL(br_datafile* df, br_material* mp) {
    LOG_TRACE("(%p, %p)", df, mp);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_PIXELMAP_REF@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_PIXELMAP_REF(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_pixelmap* pm;
    char name[256];
    char* mp;
    int i;
    LOG_TRACE9("(%p, %d, %d, %d)", df, id, length, count);

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

// IDA: int __usercall FopWrite_PIXELMAP_REF@<EAX>(br_datafile *df@<EAX>, int id@<EDX>, br_pixelmap *pixelmap@<EBX>)
int FopWrite_PIXELMAP_REF(br_datafile* df, int id, br_pixelmap* pixelmap) {
    LOG_TRACE("(%p, %d, %p)", df, id, pixelmap);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopWrite_ACTOR@<EAX>(br_datafile *df@<EAX>, br_actor *ap@<EDX>)
int FopWrite_ACTOR(br_datafile* df, br_actor* ap) {
    LOG_TRACE("(%p, %p)", df, ap);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_ACTOR@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_ACTOR(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* ap;
    LOG_TRACE9("(%p, %d, %d, %d)", df, id, length, count);

    ap = BrActorAllocate(BR_ACTOR_NONE, NULL);
    df->res = ap;
    df->prims->struct_read(df, &br_actor_F, ap);
    df->res = NULL;
    ap->t.type = BR_TRANSFORM_IDENTITY;
    DfPush(DF_ACTOR, ap, 1);
    return 0;
}

// IDA: int __usercall FopWrite_ACTOR_MODEL@<EAX>(br_datafile *df@<EAX>, br_model *model@<EDX>)
int FopWrite_ACTOR_MODEL(br_datafile* df, br_model* model) {
    LOG_TRACE9("(%p, %p)", df, model);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_ACTOR_MODEL@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_ACTOR_MODEL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    char name[256];
    br_actor* a;
    LOG_TRACE9("(%p, %d, %d, %d)", df, id, length, count);

    a = DfTop(DF_ACTOR, 0);
    df->prims->name_read(df, name);
    a->model = BrModelFind(name);
    return 0;
}

// IDA: int __usercall FopWrite_ACTOR_MATERIAL@<EAX>(br_datafile *df@<EAX>, br_material *material@<EDX>)
int FopWrite_ACTOR_MATERIAL(br_datafile* df, br_material* material) {
    LOG_TRACE("(%p, %p)", df, material);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_ACTOR_MATERIAL@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_ACTOR_MATERIAL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    char name[256];
    br_actor* a;
    LOG_TRACE9("(%p, %d, %d, %d)", df, id, length, count);
    a = DfTop(DF_ACTOR, NULL);
    df->prims->name_read(df, name);
    a->material = BrMaterialFind(name);
    return 0;
}

// IDA: int __usercall FopWrite_ACTOR_TRANSFORM@<EAX>(br_datafile *df@<EAX>)
int FopWrite_ACTOR_TRANSFORM(br_datafile* df) {
    LOG_TRACE("(%p)", df);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_ACTOR_TRANSFORM@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_ACTOR_TRANSFORM(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_transform* tp;
    LOG_TRACE9("(%p, %d, %d, %d)", df, id, length, count);

    tp = DfPop(DF_TRANSFORM, NULL);
    a = DfTop(DF_ACTOR, NULL);
    memcpy(&a->t, tp, sizeof(br_transform));
    BrResFree(tp);
    return 0;
}

// IDA: int __usercall FopWrite_ACTOR_LIGHT@<EAX>(br_datafile *df@<EAX>)
int FopWrite_ACTOR_LIGHT(br_datafile* df) {
    LOG_TRACE("(%p)", df);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_ACTOR_LIGHT@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_ACTOR_LIGHT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_light* lp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopWrite_ACTOR_CAMERA@<EAX>(br_datafile *df@<EAX>)
int FopWrite_ACTOR_CAMERA(br_datafile* df) {
    LOG_TRACE("(%p)", df);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_ACTOR_CAMERA@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_ACTOR_CAMERA(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_light* cp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopWrite_ACTOR_BOUNDS@<EAX>(br_datafile *df@<EAX>)
int FopWrite_ACTOR_BOUNDS(br_datafile* df) {
    LOG_TRACE("(%p)", df);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_ACTOR_BOUNDS@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_ACTOR_BOUNDS(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_bounds* bp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    bp = DfPop(DF_BOUNDS, 0);
    a = DfTop(DF_ACTOR, 0);
    a->type_data = bp;
    return 0;
}

// IDA: int __usercall FopWrite_ACTOR_CLIP_PLANE@<EAX>(br_datafile *df@<EAX>)
int FopWrite_ACTOR_CLIP_PLANE(br_datafile* df) {
    LOG_TRACE("(%p)", df);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_ACTOR_CLIP_PLANE@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_ACTOR_CLIP_PLANE(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_vector4* vp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopWrite_ACTOR_ADD_CHILD@<EAX>(br_datafile *df@<EAX>)
int FopWrite_ACTOR_ADD_CHILD(br_datafile* df) {
    LOG_TRACE("(%p)", df);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_ACTOR_ADD_CHILD@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_ACTOR_ADD_CHILD(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_actor* p;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    a = DfPop(DF_ACTOR, 0);
    p = DfTop(DF_ACTOR, 0);
    BrActorAdd(p, a);

    return 0;
}

// IDA: int __usercall FopWrite_TRANSFORM@<EAX>(br_datafile *df@<EAX>, br_transform *t@<EDX>)
int FopWrite_TRANSFORM(br_datafile* df, br_transform* t) {
    transform_type* tt;
    LOG_TRACE("(%p, %p)", df, t);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_TRANSFORM@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_TRANSFORM(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    int t;
    br_transform* tp;
    LOG_TRACE9("(%p, %d, %d, %d)", df, id, length, count);

    for (t = 0; t < COUNT_OF(TransformTypes); t++) {
        if (id == TransformTypes[t].id) {
            break;
        }
    }
    if (t == 7) {
        LOG_PANIC("transform type not found!")
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

// IDA: int __usercall FopWrite_BOUNDS@<EAX>(br_datafile *df@<EAX>, br_bounds *bp@<EDX>)
int FopWrite_BOUNDS(br_datafile* df, br_bounds* bp) {
    LOG_TRACE("(%p, %p)", df, bp);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_BOUNDS@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_BOUNDS(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_bounds3* bp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    bp = BrResAllocate(v1db.res, sizeof(br_bounds3), BR_MEMORY_BOUNDS);
    df->res = bp;
    df->prims->struct_read(df, &br_bounds3_F, bp);
    df->res = NULL;
    DfPush(DF_BOUNDS, bp, 1);

    return 0;
}

// IDA: int __usercall FopWrite_PLANE@<EAX>(br_datafile *df@<EAX>, br_vector4 *pp@<EDX>)
int FopWrite_PLANE(br_datafile* df, br_vector4* pp) {
    LOG_TRACE("(%p, %p)", df, pp);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_PLANE@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_PLANE(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_vector4* pp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopWrite_LIGHT@<EAX>(br_datafile *df@<EAX>, br_light *lp@<EDX>)
int FopWrite_LIGHT(br_datafile* df, br_light* lp) {
    LOG_TRACE("(%p, %p)", df, lp);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_LIGHT@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_LIGHT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_light* lp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopWrite_CAMERA@<EAX>(br_datafile *df@<EAX>, br_camera *cp@<EDX>)
int FopWrite_CAMERA(br_datafile* df, br_camera* cp) {
    LOG_TRACE("(%p, %p)", df, cp);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_CAMERA@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_CAMERA(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_camera* cp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrModelLoadMany(char *filename, br_model **models, br_uint_16 num)
br_uint_32 BrModelLoadMany(char* filename, br_model** models, br_uint_16 num) {
    int count;
    int r;
    br_datafile* df;
    LOG_TRACE9("(\"%s\", %p, %d)", filename, models, num);

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

// IDA: int __cdecl PtrCompare(void *a, void *b)
int PtrCompare(void* a, void* b) {
    LOG_TRACE("(%p, %p)", a, b);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl WriteModel(br_model *mp, br_datafile *df)
br_uint_32 WriteModel(br_model* mp, br_datafile* df) {
    br_material** mindex;
    br_vertex* vp;
    int nmaterials;
    int i;
    int has_uv;
    LOG_TRACE("(%p, %p)", mp, df);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrModelSaveMany(char *filename, br_model **models, br_uint_16 num)
br_uint_32 BrModelSaveMany(char* filename, br_model** models, br_uint_16 num) {
    br_datafile* df;
    int i;
    int m;
    LOG_TRACE("(\"%s\", %p, %d)", filename, models, num);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrActorLoadMany(char *filename, br_actor **actors, br_uint_16 num)
br_uint_32 BrActorLoadMany(char* filename, br_actor** actors, br_uint_16 num) {
    br_datafile* df;
    int count;
    int r;
    LOG_TRACE9("(\"%s\", %p, %d)", filename, actors, num);

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

// IDA: int __usercall WriteActor@<EAX>(br_actor *a@<EAX>, br_datafile *df@<EDX>)
int WriteActor(br_actor* a, br_datafile* df) {
    br_actor* ap;
    br_actor* last_ap;
    LOG_TRACE("(%p, %p)", a, df);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrActorSaveMany(char *filename, br_actor **actors, br_uint_16 num)
br_uint_32 BrActorSaveMany(char* filename, br_actor** actors, br_uint_16 num) {
    br_datafile* df;
    int i;
    LOG_TRACE("(\"%s\", %p, %d)", filename, actors, num);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrMaterialLoadMany(char *filename, br_material **materials, br_uint_16 num)
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
        } while (r);
        DfClose(df);
    }
    return count;
}

// IDA: br_uint_32 __cdecl WriteMaterial(br_material *mp, br_datafile *df)
br_uint_32 WriteMaterial(br_material* mp, br_datafile* df) {
    LOG_TRACE("(%p, %p)", mp, df);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrMaterialSaveMany(char *filename, br_material **materials, br_uint_16 num)
br_uint_32 BrMaterialSaveMany(char* filename, br_material** materials, br_uint_16 num) {
    br_datafile* df;
    int i;
    int count;
    LOG_TRACE("(\"%s\", %p, %d)", filename, materials, num);
    NOT_IMPLEMENTED();
}

// IDA: br_model* __cdecl BrModelLoad(char *filename)
br_model* BrModelLoad(char* filename) {
    br_model* ptr;
    LOG_TRACE("(\"%s\")", filename);

    if (BrModelLoadMany(filename, &ptr, 1u) == 1) {
        return ptr;
    }
    return NULL;
}

// IDA: br_uint_32 __cdecl BrModelSave(char *filename, br_model *ptr)
br_uint_32 BrModelSave(char* filename, br_model* ptr) {
    LOG_TRACE("(\"%s\", %p)", filename, ptr);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __cdecl BrMaterialLoad(char *filename)
br_material* BrMaterialLoad(char* filename) {
    br_material* ptr;
    if (BrMaterialLoadMany(filename, &ptr, 1) == 1) {
        return ptr;
    }
    return NULL;
}

// IDA: br_uint_32 __cdecl BrMaterialSave(char *filename, br_material *ptr)
br_uint_32 BrMaterialSave(char* filename, br_material* ptr) {
    LOG_TRACE("(\"%s\", %p)", filename, ptr);
    NOT_IMPLEMENTED();
}

// IDA: br_actor* __cdecl BrActorLoad(char *filename)
br_actor* BrActorLoad(char* filename) {
    LOG_TRACE("(\"%s\")", filename);

    br_actor* ptr;
    if (BrActorLoadMany(filename, &ptr, 1u) == 1) {
        return ptr;
    }
    LOG_DEBUG("loadmany fail");
    return NULL;
}

// IDA: br_uint_32 __cdecl BrActorSave(char *filename, br_actor *ptr)
br_uint_32 BrActorSave(char* filename, br_actor* ptr) {
    LOG_TRACE("(\"%s\", %p)", filename, ptr);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrModelFileCount(char *filename, br_uint_16 *num)
br_error BrModelFileCount(char* filename, br_uint_16* num) {
    br_datafile* df;
    LOG_TRACE("(\"%s\", %p)", filename, num);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrActorFileCount(char *filename, br_uint_16 *num)
br_error BrActorFileCount(char* filename, br_uint_16* num) {
    br_datafile* df;
    LOG_TRACE("(\"%s\", %p)", filename, num);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrMaterialFileCount(char *filename, br_uint_16 *num)
br_error BrMaterialFileCount(char* filename, br_uint_16* num) {
    br_datafile* df;
    LOG_TRACE("(\"%s\", %p)", filename, num);
    NOT_IMPLEMENTED();
}
