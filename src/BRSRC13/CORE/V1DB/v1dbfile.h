#ifndef _V1DBFILE_H_
#define _V1DBFILE_H_

#include "br_types.h"

// Offset: 18
// Size: 139
// EAX: df
// EDX: vertices
// EBX: nvertices
int FopWrite_VERTICES(br_datafile* df, br_vertex* vertices, int nvertices);

// Offset: 174
// Size: 126
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_VERTICES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 321
// Size: 144
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_VERTICES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 484
// Size: 139
// EAX: df
// EDX: vertices
// EBX: nvertices
int FopWrite_VERTEX_UV(br_datafile* df, br_vertex* vertices, int nvertices);

// Offset: 641
// Size: 118
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_VERTEX_UV(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 783
// Size: 144
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_VERTICES_UV(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 950
// Size: 177
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_MATERIAL_INDEX(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 1151
// Size: 208
// EAX: df
// EDX: materials
// EBX: nmaterials
int FopWrite_MATERIAL_INDEX(br_datafile* df, br_material** materials, int nmaterials);

// Offset: 1386
// Size: 289
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_MATERIAL_INDEX(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 1689
// Size: 207
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_FACES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 1911
// Size: 139
// EAX: df
// EDX: faces
// EBX: nfaces
int FopWrite_FACES(br_datafile* df, br_face* faces, int nfaces);

// Offset: 2070
// Size: 271
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_FACES_1(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 2359
// Size: 292
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_FACES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 2674
// Size: 293
// EAX: df
// EDX: faces
// EBX: nfaces
// ECX: mindex
int FopWrite_FACE_MATERIAL(br_datafile* df, br_face* faces, int nfaces, br_material** mindex, int nmaterials);

// Offset: 2989
// Size: 303
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_FACE_MATERIAL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 3306
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_MODEL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 3453
// Size: 125
// EAX: df
// EDX: mp
int FopWrite_MODEL(br_datafile* df, br_model* mp);

// Offset: 3598
// Size: 124
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_MODEL_1(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 3740
// Size: 230
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_MODEL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 3984
// Size: 122
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_PIVOT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 4127
// Size: 120
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_MATERIAL_OLD(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 4264
// Size: 120
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_MATERIAL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 4402
// Size: 109
// EAX: df
// EDX: mp
int FopWrite_MATERIAL(br_datafile* df, br_material* mp);

// Offset: 4532
// Size: 227
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_PIXELMAP_REF(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 4781
// Size: 144
// EAX: df
// EDX: id
// EBX: pixelmap
int FopWrite_PIXELMAP_REF(br_datafile* df, int id, br_pixelmap* pixelmap);

// Offset: 4940
// Size: 109
// EAX: df
// EDX: ap
int FopWrite_ACTOR(br_datafile* df, br_actor* ap);

// Offset: 5063
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 5216
// Size: 145
// EAX: df
// EDX: model
int FopWrite_ACTOR_MODEL(br_datafile* df, br_model* model);

// Offset: 5381
// Size: 100
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR_MODEL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 5505
// Size: 145
// EAX: df
// EDX: material
int FopWrite_ACTOR_MATERIAL(br_datafile* df, br_material* material);

// Offset: 5673
// Size: 100
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR_MATERIAL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 5798
// Size: 66
// EAX: df
int FopWrite_ACTOR_TRANSFORM(br_datafile* df);

// Offset: 5888
// Size: 108
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR_TRANSFORM(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 6017
// Size: 66
// EAX: df
int FopWrite_ACTOR_LIGHT(br_datafile* df);

// Offset: 6103
// Size: 89
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR_LIGHT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 6214
// Size: 66
// EAX: df
int FopWrite_ACTOR_CAMERA(br_datafile* df);

// Offset: 6301
// Size: 89
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR_CAMERA(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 6412
// Size: 66
// EAX: df
int FopWrite_ACTOR_BOUNDS(br_datafile* df);

// Offset: 6499
// Size: 89
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR_BOUNDS(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 6614
// Size: 66
// EAX: df
int FopWrite_ACTOR_CLIP_PLANE(br_datafile* df);

// Offset: 6705
// Size: 89
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR_CLIP_PLANE(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 6819
// Size: 66
// EAX: df
int FopWrite_ACTOR_ADD_CHILD(br_datafile* df);

// Offset: 6909
// Size: 96
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ACTOR_ADD_CHILD(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 7024
// Size: 197
// EAX: df
// EDX: t
int FopWrite_TRANSFORM(br_datafile* df, br_transform* t);

// Offset: 7239
// Size: 234
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_TRANSFORM(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 7489
// Size: 109
// EAX: df
// EDX: bp
int FopWrite_BOUNDS(br_datafile* df, br_bounds* bp);

// Offset: 7613
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_BOUNDS(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 7760
// Size: 109
// EAX: df
// EDX: pp
int FopWrite_PLANE(br_datafile* df, br_vector4* pp);

// Offset: 7883
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_PLANE(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 8030
// Size: 109
// EAX: df
// EDX: lp
int FopWrite_LIGHT(br_datafile* df, br_light* lp);

// Offset: 8153
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_LIGHT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 8301
// Size: 109
// EAX: df
// EDX: cp
int FopWrite_CAMERA(br_datafile* df, br_camera* cp);

// Offset: 8425
// Size: 132
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_CAMERA(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 8573
// Size: 162
br_uint_32 BrModelLoadMany(char* filename, br_model** models, br_uint_16 num);

// Offset: 8746
// Size: 82
int PtrCompare(void* a, void* b);

// Offset: 8839
// Size: 720
br_uint_32 WriteModel(br_model* mp, br_datafile* df);

// Offset: 9575
// Size: 209
br_uint_32 BrModelSaveMany(char* filename, br_model** models, br_uint_16 num);

// Offset: 9800
// Size: 162
br_uint_32 BrActorLoadMany(char* filename, br_actor** actors, br_uint_16 num);

// Offset: 9973
// Size: 400
// EAX: a
// EDX: df
int WriteActor(br_actor* a, br_datafile* df);

// Offset: 10389
// Size: 156
br_uint_32 BrActorSaveMany(char* filename, br_actor** actors, br_uint_16 num);

// Offset: 10564
// Size: 162
br_uint_32 BrMaterialLoadMany(char* filename, br_material** materials, br_uint_16 num);

// Offset: 10740
// Size: 199
br_uint_32 WriteMaterial(br_material* mp, br_datafile* df);

// Offset: 10958
// Size: 202
br_uint_32 BrMaterialSaveMany(char* filename, br_material** materials, br_uint_16 num);

// Offset: 11172
// Size: 81
br_model* BrModelLoad(char* filename);

// Offset: 11265
// Size: 58
br_uint_32 BrModelSave(char* filename, br_model* ptr);

// Offset: 11338
// Size: 81
br_material* BrMaterialLoad(char* filename);

// Offset: 11434
// Size: 58
br_uint_32 BrMaterialSave(char* filename, br_material* ptr);

// Offset: 11504
// Size: 81
br_actor* BrActorLoad(char* filename);

// Offset: 11597
// Size: 58
br_uint_32 BrActorSave(char* filename, br_actor* ptr);

// Offset: 11672
// Size: 174
br_error BrModelFileCount(char* filename, br_uint_16* num);

// Offset: 11863
// Size: 174
br_error BrActorFileCount(char* filename, br_uint_16* num);

// Offset: 12057
// Size: 174
br_error BrMaterialFileCount(char* filename, br_uint_16* num);

#endif
