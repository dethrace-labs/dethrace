#include "v1dbfile.h"

#include "CORE/FW/datafile.h"
#include "CORE/FW/genfile.h"
#include "CORE/FW/resource.h"
#include "CORE/V1DB/modsupt.h"
#include "debug.h"
#include <stddef.h>
#include <stdlib.h>

br_chunks_table_entry MaterialLoadEntries[8];
br_chunks_table MaterialLoadTable;
br_chunks_table ActorLoadTable;
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
br_file_enum actor_type_F;
br_file_struct_member br_actor_FM[3];
br_file_struct br_actor_F;
br_file_enum render_style_F;
br_file_enum_member actor_type_FM[8];
br_file_struct br_material_old_F;
br_file_struct_member br_material_FM[17];
br_file_struct br_material_F;
br_file_struct_member br_material_old_FM[13];
br_file_struct br_pivot_F;
br_file_struct_member br_pivot_FM[3];
br_file_struct br_old_model_1_F;
br_file_struct_member br_old_model_1_FM[1];
br_file_struct_member br_model_FM[2] = {
    { 3u, offsetof(br_model, flags), "flags", NULL },
    { 17u, offsetof(br_model, identifier), "identifier", NULL }
};
br_file_struct br_model_F = { "br_model", 2u, br_model_FM, 84 };

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

br_file_struct br_face_F = { "br_face", 5u, br_face_FM, 40 };
br_file_struct_member br_old_vertex_uv_FM[5];
br_file_struct br_vertex_uv_F;
br_file_struct br_old_vertex_uv_F;
br_file_struct_member br_vertex_uv_FM[2];
br_file_struct_member br_vertex_FM[3] = {
    { 10, offsetof(br_vertex, p) + 0, "p.v[x]", NULL },
    { 10, offsetof(br_vertex, p) + 4, "p.v[y]", NULL },
    { 10, offsetof(br_vertex, p) + 8, "p.v[z]", NULL }
};
br_file_struct br_vertex_F = { "br_vertex", 3, br_vertex_FM, 40 };

br_chunks_table_entry ActorLoadEntries[21];
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

struct {
    br_uint_32 id;
    size_t offset;
    int table;
} MaterialMaps;
char rscid[53];

// Offset: 18
// Size: 139
// EAX: df
// EDX: vertices
// EBX: nvertices
int FopWrite_VERTICES(br_datafile* df, br_vertex* vertices, int nvertices) {
    NOT_IMPLEMENTED();
}

// Offset: 174
// Size: 126
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_VERTICES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    int i;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    mp = DfTop(8, NULL);
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
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_VERTICES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    void* ptr;
    NOT_IMPLEMENTED();
}

// Offset: 484
// Size: 139
// EAX: df
// EDX: vertices
// EBX: nvertices
int FopWrite_VERTEX_UV(br_datafile* df, br_vertex* vertices, int nvertices) {
    NOT_IMPLEMENTED();
}

// Offset: 641
// Size: 118
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_VERTEX_UV(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    NOT_IMPLEMENTED();
}

// Offset: 783
// Size: 144
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_VERTICES_UV(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    void* ptr;
    NOT_IMPLEMENTED();
}

// Offset: 950
// Size: 177
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_MATERIAL_INDEX(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    char name[256];
    br_material** mip;
    br_uint_32 i;
    NOT_IMPLEMENTED();
}

// Offset: 1151
// Size: 208
// EAX: df
// EDX: materials
// EBX: nmaterials
int FopWrite_MATERIAL_INDEX(br_datafile* df, br_material** materials, int nmaterials) {
    int i;
    int s;
    NOT_IMPLEMENTED();
}

// Offset: 1386
// Size: 289
// EAX: df
// EDX: id
// EBX: length
// ECX: count
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
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_FACES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    int i;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);
    mp = DfTop(8, 0);
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
// EAX: df
// EDX: faces
// EBX: nfaces
int FopWrite_FACES(br_datafile* df, br_face* faces, int nfaces) {
    NOT_IMPLEMENTED();
}

// Offset: 2070
// Size: 271
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_FACES_1(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 2359
// Size: 292
// EAX: df
// EDX: id
// EBX: length
// ECX: count
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
// EAX: df
// EDX: faces
// EBX: nfaces
// ECX: mindex
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
// EAX: df
// EDX: id
// EBX: length
// ECX: count
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
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_MODEL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    mp = BrModelAllocate(NULL, 0, 0);
    mp->identifier = NULL;
    df->res = mp;
    df->prims->struct_read(df, &br_model_F, mp);
    df->res = NULL;

    LOG_DEBUG("model id=%s\n", mp->identifier);

    mp->flags &= (BR_MODF_DONT_WELD | BR_MODF_KEEP_ORIGINAL | BR_MODF_GENERATE_TAGS | BR_MODF_QUICK_UPDATE);
    DfPush(8, mp, 1);
    return 0;
}

// Offset: 3453
// Size: 125
// EAX: df
// EDX: mp
int FopWrite_MODEL(br_datafile* df, br_model* mp) {
    br_model temp_model;
    NOT_IMPLEMENTED();
}

// Offset: 3598
// Size: 124
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_MODEL_1(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    NOT_IMPLEMENTED();
}

// Offset: 3740
// Size: 230
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_MODEL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 3984
// Size: 122
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_PIVOT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    NOT_IMPLEMENTED();
}

// Offset: 4127
// Size: 120
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_MATERIAL_OLD(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_material* mp;
    NOT_IMPLEMENTED();
}

// Offset: 4264
// Size: 120
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_MATERIAL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_material* mp;
    NOT_IMPLEMENTED();
}

// Offset: 4402
// Size: 109
// EAX: df
// EDX: mp
int FopWrite_MATERIAL(br_datafile* df, br_material* mp) {
    NOT_IMPLEMENTED();
}

// Offset: 4532
// Size: 227
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_PIXELMAP_REF(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_pixelmap* pm;
    char name[256];
    char* mp;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 4781
// Size: 144
// EAX: df
// EDX: id
// EBX: pixelmap
int FopWrite_PIXELMAP_REF(br_datafile* df, int id, br_pixelmap* pixelmap) {
    NOT_IMPLEMENTED();
}

// Offset: 4940
// Size: 109
// EAX: df
// EDX: ap
int FopWrite_ACTOR(br_datafile* df, br_actor* ap) {
    NOT_IMPLEMENTED();
}

// Offset: 5063
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* ap;
    NOT_IMPLEMENTED();
}

// Offset: 5216
// Size: 145
// EAX: df
// EDX: model
int FopWrite_ACTOR_MODEL(br_datafile* df, br_model* model) {
    NOT_IMPLEMENTED();
}

// Offset: 5381
// Size: 100
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR_MODEL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    char name[256];
    br_actor* a;
    NOT_IMPLEMENTED();
}

// Offset: 5505
// Size: 145
// EAX: df
// EDX: material
int FopWrite_ACTOR_MATERIAL(br_datafile* df, br_material* material) {
    NOT_IMPLEMENTED();
}

// Offset: 5673
// Size: 100
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR_MATERIAL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    char name[256];
    br_actor* a;
    NOT_IMPLEMENTED();
}

// Offset: 5798
// Size: 66
// EAX: df
int FopWrite_ACTOR_TRANSFORM(br_datafile* df) {
    NOT_IMPLEMENTED();
}

// Offset: 5888
// Size: 108
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR_TRANSFORM(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_transform* tp;
    NOT_IMPLEMENTED();
}

// Offset: 6017
// Size: 66
// EAX: df
int FopWrite_ACTOR_LIGHT(br_datafile* df) {
    NOT_IMPLEMENTED();
}

// Offset: 6103
// Size: 89
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR_LIGHT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_light* lp;
    NOT_IMPLEMENTED();
}

// Offset: 6214
// Size: 66
// EAX: df
int FopWrite_ACTOR_CAMERA(br_datafile* df) {
    NOT_IMPLEMENTED();
}

// Offset: 6301
// Size: 89
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR_CAMERA(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_light* cp;
    NOT_IMPLEMENTED();
}

// Offset: 6412
// Size: 66
// EAX: df
int FopWrite_ACTOR_BOUNDS(br_datafile* df) {
    NOT_IMPLEMENTED();
}

// Offset: 6499
// Size: 89
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR_BOUNDS(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_bounds* bp;
    NOT_IMPLEMENTED();
}

// Offset: 6614
// Size: 66
// EAX: df
int FopWrite_ACTOR_CLIP_PLANE(br_datafile* df) {
    NOT_IMPLEMENTED();
}

// Offset: 6705
// Size: 89
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR_CLIP_PLANE(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_vector4* vp;
    NOT_IMPLEMENTED();
}

// Offset: 6819
// Size: 66
// EAX: df
int FopWrite_ACTOR_ADD_CHILD(br_datafile* df) {
    NOT_IMPLEMENTED();
}

// Offset: 6909
// Size: 96
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR_ADD_CHILD(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_actor* p;
    NOT_IMPLEMENTED();
}

// Offset: 7024
// Size: 197
// EAX: df
// EDX: t
int FopWrite_TRANSFORM(br_datafile* df, br_transform* t) {
    transform_type* tt;
    NOT_IMPLEMENTED();
}

// Offset: 7239
// Size: 234
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_TRANSFORM(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    int t;
    br_transform* tp;
    NOT_IMPLEMENTED();
}

// Offset: 7489
// Size: 109
// EAX: df
// EDX: bp
int FopWrite_BOUNDS(br_datafile* df, br_bounds* bp) {
    NOT_IMPLEMENTED();
}

// Offset: 7613
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_BOUNDS(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_bounds3* bp;
    NOT_IMPLEMENTED();
}

// Offset: 7760
// Size: 109
// EAX: df
// EDX: pp
int FopWrite_PLANE(br_datafile* df, br_vector4* pp) {
    NOT_IMPLEMENTED();
}

// Offset: 7883
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_PLANE(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_vector4* pp;
    NOT_IMPLEMENTED();
}

// Offset: 8030
// Size: 109
// EAX: df
// EDX: lp
int FopWrite_LIGHT(br_datafile* df, br_light* lp) {
    NOT_IMPLEMENTED();
}

// Offset: 8153
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_LIGHT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_light* lp;
    NOT_IMPLEMENTED();
}

// Offset: 8301
// Size: 109
// EAX: df
// EDX: cp
int FopWrite_CAMERA(br_datafile* df, br_camera* cp) {
    NOT_IMPLEMENTED();
}

// Offset: 8425
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
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
        LOG_DEBUG("r %d", r);
        if (DfTopType() == 8) {
            LOG_DEBUG("poppping model");
            models[count] = DfPop(8, 0);
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
    NOT_IMPLEMENTED();
}

// Offset: 9973
// Size: 400
// EAX: a
// EDX: df
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
    int count;
    int r;
    NOT_IMPLEMENTED();
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
        LOG_DEBUG("returning ptr %p", ptr);
        return ptr;
    } else {
        return NULL;
    }
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
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
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
