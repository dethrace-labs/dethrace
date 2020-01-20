#include "v1dbfile.h"

#include <stdlib.h>

br_chunks_table_entry MaterialLoadEntries[8];
br_chunks_table MaterialLoadTable;
br_chunks_table ActorLoadTable;
br_file_struct br_camera_F;
br_file_enum camera_type_F;
br_chunks_table ModelLoadTable;
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
br_file_struct_member br_model_FM[2];
br_file_struct br_model_F;
br_file_struct_member br_old_face_FM[5];
br_file_struct br_old_face_F;
br_file_struct_member br_old_face_1_FM[5];
br_file_struct br_old_face_1_F;
br_file_struct_member br_face_FM[5];
br_file_struct br_face_F;
br_file_struct_member br_old_vertex_uv_FM[5];
br_file_struct br_vertex_uv_F;
br_file_struct br_old_vertex_uv_F;
br_file_struct_member br_vertex_uv_FM[2];
br_file_struct br_vertex_F;
br_file_struct_member br_vertex_FM[3];
br_chunks_table_entry ActorLoadEntries[21];
br_chunks_table_entry ModelLoadEntries[15];
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
}

// Offset: 174
// Size: 126
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_VERTICES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
}

// Offset: 321
// Size: 144
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_VERTICES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    void* ptr;
}

// Offset: 484
// Size: 139
// EAX: df
// EDX: vertices
// EBX: nvertices
int FopWrite_VERTEX_UV(br_datafile* df, br_vertex* vertices, int nvertices) {
}

// Offset: 641
// Size: 118
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_VERTEX_UV(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
}

// Offset: 783
// Size: 144
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_VERTICES_UV(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    void* ptr;
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
}

// Offset: 1151
// Size: 208
// EAX: df
// EDX: materials
// EBX: nmaterials
int FopWrite_MATERIAL_INDEX(br_datafile* df, br_material** materials, int nmaterials) {
    int i;
    int s;
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
}

// Offset: 1911
// Size: 139
// EAX: df
// EDX: faces
// EBX: nfaces
int FopWrite_FACES(br_datafile* df, br_face* faces, int nfaces) {
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
}

// Offset: 3306
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_MODEL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
}

// Offset: 3453
// Size: 125
// EAX: df
// EDX: mp
int FopWrite_MODEL(br_datafile* df, br_model* mp) {
    br_model temp_model;
}

// Offset: 3598
// Size: 124
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_MODEL_1(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
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
}

// Offset: 3984
// Size: 122
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_PIVOT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
}

// Offset: 4127
// Size: 120
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_MATERIAL_OLD(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_material* mp;
}

// Offset: 4264
// Size: 120
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_MATERIAL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_material* mp;
}

// Offset: 4402
// Size: 109
// EAX: df
// EDX: mp
int FopWrite_MATERIAL(br_datafile* df, br_material* mp) {
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
}

// Offset: 4781
// Size: 144
// EAX: df
// EDX: id
// EBX: pixelmap
int FopWrite_PIXELMAP_REF(br_datafile* df, int id, br_pixelmap* pixelmap) {
}

// Offset: 4940
// Size: 109
// EAX: df
// EDX: ap
int FopWrite_ACTOR(br_datafile* df, br_actor* ap) {
}

// Offset: 5063
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* ap;
}

// Offset: 5216
// Size: 145
// EAX: df
// EDX: model
int FopWrite_ACTOR_MODEL(br_datafile* df, br_model* model) {
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
}

// Offset: 5505
// Size: 145
// EAX: df
// EDX: material
int FopWrite_ACTOR_MATERIAL(br_datafile* df, br_material* material) {
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
}

// Offset: 5798
// Size: 66
// EAX: df
int FopWrite_ACTOR_TRANSFORM(br_datafile* df) {
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
}

// Offset: 6017
// Size: 66
// EAX: df
int FopWrite_ACTOR_LIGHT(br_datafile* df) {
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
}

// Offset: 6214
// Size: 66
// EAX: df
int FopWrite_ACTOR_CAMERA(br_datafile* df) {
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
}

// Offset: 6412
// Size: 66
// EAX: df
int FopWrite_ACTOR_BOUNDS(br_datafile* df) {
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
}

// Offset: 6614
// Size: 66
// EAX: df
int FopWrite_ACTOR_CLIP_PLANE(br_datafile* df) {
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
}

// Offset: 6819
// Size: 66
// EAX: df
int FopWrite_ACTOR_ADD_CHILD(br_datafile* df) {
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
}

// Offset: 7024
// Size: 197
// EAX: df
// EDX: t
int FopWrite_TRANSFORM(br_datafile* df, br_transform* t) {
    transform_type* tt;
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
}

// Offset: 7489
// Size: 109
// EAX: df
// EDX: bp
int FopWrite_BOUNDS(br_datafile* df, br_bounds* bp) {
}

// Offset: 7613
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_BOUNDS(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_bounds3* bp;
}

// Offset: 7760
// Size: 109
// EAX: df
// EDX: pp
int FopWrite_PLANE(br_datafile* df, br_vector4* pp) {
}

// Offset: 7883
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_PLANE(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_vector4* pp;
}

// Offset: 8030
// Size: 109
// EAX: df
// EDX: lp
int FopWrite_LIGHT(br_datafile* df, br_light* lp) {
}

// Offset: 8153
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_LIGHT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_light* lp;
}

// Offset: 8301
// Size: 109
// EAX: df
// EDX: cp
int FopWrite_CAMERA(br_datafile* df, br_camera* cp) {
}

// Offset: 8425
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_CAMERA(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_camera* cp;
}

// Offset: 8573
// Size: 162
br_uint_32 BrModelLoadMany(char* filename, br_model** models, br_uint_16 num) {
    int count;
    int r;
    br_datafile* df;
}

// Offset: 8746
// Size: 82
int PtrCompare(void* a, void* b) {
}

// Offset: 8839
// Size: 720
br_uint_32 WriteModel(br_model* mp, br_datafile* df) {
    br_material** mindex;
    br_vertex* vp;
    int nmaterials;
    int i;
    int has_uv;
}

// Offset: 9575
// Size: 209
br_uint_32 BrModelSaveMany(char* filename, br_model** models, br_uint_16 num) {
    br_datafile* df;
    int i;
    int m;
}

// Offset: 9800
// Size: 162
br_uint_32 BrActorLoadMany(char* filename, br_actor** actors, br_uint_16 num) {
    br_datafile* df;
    int count;
    int r;
}

// Offset: 9973
// Size: 400
// EAX: a
// EDX: df
int WriteActor(br_actor* a, br_datafile* df) {
    br_actor* ap;
    br_actor* last_ap;
}

// Offset: 10389
// Size: 156
br_uint_32 BrActorSaveMany(char* filename, br_actor** actors, br_uint_16 num) {
    br_datafile* df;
    int i;
}

// Offset: 10564
// Size: 162
br_uint_32 BrMaterialLoadMany(char* filename, br_material** materials, br_uint_16 num) {
    br_datafile* df;
    int count;
    int r;
}

// Offset: 10740
// Size: 199
br_uint_32 WriteMaterial(br_material* mp, br_datafile* df) {
}

// Offset: 10958
// Size: 202
br_uint_32 BrMaterialSaveMany(char* filename, br_material** materials, br_uint_16 num) {
    br_datafile* df;
    int i;
    int count;
}

// Offset: 11172
// Size: 81
br_model* BrModelLoad(char* filename) {
    br_model* ptr;
}

// Offset: 11265
// Size: 58
br_uint_32 BrModelSave(char* filename, br_model* ptr) {
}

// Offset: 11338
// Size: 81
br_material* BrMaterialLoad(char* filename) {
    br_material* ptr;
}

// Offset: 11434
// Size: 58
br_uint_32 BrMaterialSave(char* filename, br_material* ptr) {
}

// Offset: 11504
// Size: 81
br_actor* BrActorLoad(char* filename) {
    br_actor* ptr;
}

// Offset: 11597
// Size: 58
br_uint_32 BrActorSave(char* filename, br_actor* ptr) {
}

// Offset: 11672
// Size: 174
br_error BrModelFileCount(char* filename, br_uint_16* num) {
    br_datafile* df;
}

// Offset: 11863
// Size: 174
br_error BrActorFileCount(char* filename, br_uint_16* num) {
    br_datafile* df;
}

// Offset: 12057
// Size: 174
br_error BrMaterialFileCount(char* filename, br_uint_16* num) {
    br_datafile* df;
}
