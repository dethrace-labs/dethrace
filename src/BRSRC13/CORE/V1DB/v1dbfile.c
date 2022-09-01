#include "v1dbfile.h"

#include "CORE/FW/brqsort.h"
#include "CORE/FW/datafile.h"
#include "CORE/FW/diag.h"
#include "CORE/FW/file.h"
#include "CORE/FW/genfile.h"
#include "CORE/FW/resource.h"
#include "CORE/FW/scratch.h"
#include "CORE/V1DB/actsupt.h"
#include "CORE/V1DB/chunkids.h"
#include "CORE/V1DB/dbsetup.h"
#include "CORE/V1DB/matsupt.h"
#include "CORE/V1DB/modsupt.h"
#include "CORE/V1DB/regsupt.h"
#include "CORE/V1DB/stackids.h"
#include "harness/trace.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

br_file_struct_member br_vertex_FM[3] = {
    { DF_TYPE_BR_SCALAR, offsetof(br_vertex, p.v[0]), "p.v[x]", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_vertex, p.v[1]), "p.v[y]", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_vertex, p.v[2]), "p.v[z]", NULL }
};
br_file_struct br_vertex_F = { "br_vertex", BR_ASIZE(br_vertex_FM), br_vertex_FM, sizeof(br_vertex) };

br_file_struct_member br_vertex_uv_FM[] = {
    { DF_TYPE_BR_SCALAR, offsetof(br_vertex, map.v[0]), "map.v[0]", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_vertex, map.v[1]), "map.v[1]", NULL }
};
br_file_struct br_vertex_uv_F = { "br_vertex_uv", BR_ASIZE(br_vertex_uv_FM), br_vertex_uv_FM, sizeof(br_vertex) };

br_file_struct_member br_old_vertex_uv_FM[5] = {
    { DF_TYPE_BR_SCALAR, offsetof(br_vertex, p.v[0]), "p.v[X]", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_vertex, p.v[1]), "p.v[Y]", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_vertex, p.v[2]), "p.v[Z]", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_vertex, map.v[0]), "map.v[0]", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_vertex, map.v[1]), "map.v[1]", NULL },
};
br_file_struct br_old_vertex_uv_F = { "br_old_vertex_uv", BR_ASIZE(br_old_vertex_uv_FM), br_old_vertex_uv_FM, sizeof(br_vertex) };

br_file_struct_member br_face_FM[5] = {
    { DF_TYPE_BR_UINT_16, offsetof(br_face, vertices[0]), "vertices[0]", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_face, vertices[1]), "vertices[1]", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_face, vertices[2]), "vertices[2]", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_face, smoothing), "smoothing", NULL },
    { DF_TYPE_BR_UINT_8, offsetof(br_face, flags), "flags", NULL }
};
br_file_struct br_face_F = { "br_face", BR_ASIZE(br_face_FM), br_face_FM, sizeof(br_face) };

br_file_struct_member br_old_face_1_FM[5] = {
    { DF_TYPE_BR_UINT_16, offsetof(br_face, vertices[0]), "vertices[0]", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_face, vertices[1]), "vertices[1]", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_face, vertices[2]), "vertices[2]", NULL },
    { DF_TYPE_BR_UINT_8, offsetof(br_face, smoothing), "smoothing", NULL },
    { DF_TYPE_BR_UINT_8, offsetof(br_face, flags), "flags", NULL },
};
br_file_struct br_old_face_1_F = { "br_old_face_1", BR_ASIZE(br_old_face_1_FM), br_old_face_1_FM, sizeof(br_face) };

br_file_struct_member br_old_face_FM[5] = {
    { DF_TYPE_BR_UINT_16, offsetof(br_face, vertices[0]), "vertices[0]", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_face, vertices[1]), "vertices[1]", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_face, vertices[2]), "vertices[2]", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_face, material), "material", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_face, smoothing), "smoothing", NULL }, // DF_TYPE_BR_UINT_32 in EXE
};
br_file_struct br_old_face_F = { "br_old_face", BR_ASIZE(br_old_face_FM), br_old_face_FM, sizeof(br_face) };

br_file_struct_member br_model_FM[2] = {
    { DF_TYPE_BR_UINT_16, offsetof(br_model, flags), "flags", NULL },
    { DF_TYPE_ASCIZ, offsetof(br_model, identifier), "identifier", NULL }
};
br_file_struct br_model_F = { "br_model", BR_ASIZE(br_model_FM), br_model_FM, sizeof(br_model) };

br_file_struct_member br_old_model_1_FM[1] = {
    { DF_TYPE_ASCIZ, offsetof(br_model, identifier), "identifier", NULL },
};
br_file_struct br_old_model_1_F = { "br_old_model_1", BR_ASIZE(br_old_model_1_FM), br_old_model_1_FM, sizeof(br_model) };

br_file_struct_member br_pivot_FM[3] = {
    { DF_TYPE_BR_SCALAR, offsetof(br_model, pivot.v[0]), "pivot.v[X]", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_model, pivot.v[1]), "pivot.v[Y]", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_model, pivot.v[2]), "pivot.v[Z]", NULL },
};
br_file_struct br_pivot_F = { "br_pivot", BR_ASIZE(br_pivot_FM), br_pivot_FM, sizeof(br_model) };

br_file_struct_member br_material_old_FM[13] = {
    { DF_TYPE_BR_COLOUR, offsetof(br_material, colour), "colour", NULL },
    { DF_TYPE_BR_UINT_8, offsetof(br_material, opacity), "opacity", NULL },
    { DF_TYPE_BR_UFRACTION, offsetof(br_material, ka), "ka", NULL },
    { DF_TYPE_BR_UFRACTION, offsetof(br_material, kd), "kd", NULL },
    { DF_TYPE_BR_UFRACTION, offsetof(br_material, ks), "ks", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_material, power), "power", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_material, flags), "flags", NULL },
    { DF_TYPE_BR_VECTOR2, offsetof(br_material, map_transform.m[0]), "map_transform.m[0]", NULL },
    { DF_TYPE_BR_VECTOR2, offsetof(br_material, map_transform.m[1]), "map_transform.m[1]", NULL },
    { DF_TYPE_BR_VECTOR2, offsetof(br_material, map_transform.m[2]), "map_transform.m[2]", NULL },
    { DF_TYPE_BR_UINT_8, offsetof(br_material, index_base), "index_base", NULL },
    { DF_TYPE_BR_UINT_8, offsetof(br_material, index_range), "index_range", NULL },
    { DF_TYPE_ASCIZ, offsetof(br_material, identifier), "identifier", NULL }
};
br_file_struct br_material_old_F = { "br_material_old", BR_ASIZE(br_material_old_FM), br_material_old_FM, sizeof(br_material) };

br_file_struct_member br_material_FM[17] = {
    { DF_TYPE_BR_COLOUR, offsetof(br_material, colour), "colour", NULL },
    { DF_TYPE_BR_UINT_8, offsetof(br_material, opacity), "opacity", NULL },
    { DF_TYPE_BR_UFRACTION, offsetof(br_material, ka), "ka", NULL },
    { DF_TYPE_BR_UFRACTION, offsetof(br_material, kd), "kd", NULL },
    { DF_TYPE_BR_UFRACTION, offsetof(br_material, ks), "ks", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_material, power), "power", NULL },
    { DF_TYPE_BR_UINT_32, offsetof(br_material, flags), "flags", NULL },
    { DF_TYPE_BR_VECTOR2, offsetof(br_material, map_transform.m[0]), "map_transform.m[0]", NULL },
    { DF_TYPE_BR_VECTOR2, offsetof(br_material, map_transform.m[1]), "map_transform.m[1]", NULL },
    { DF_TYPE_BR_VECTOR2, offsetof(br_material, map_transform.m[2]), "map_transform.m[2]", NULL },
    { DF_TYPE_BR_UINT_8, offsetof(br_material, index_base), "index_base", NULL },
    { DF_TYPE_BR_UINT_8, offsetof(br_material, index_range), "index_range", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_material, fog_min), "fog_min", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_material, fog_max), "fog_max", NULL },
    { DF_TYPE_BR_COLOUR, offsetof(br_material, fog_colour), "fog_colour", NULL },
    { DF_TYPE_BR_INT_32, offsetof(br_material, subdivide_tolerance), "subdivide_tolerance", NULL },
    { DF_TYPE_ASCIZ, offsetof(br_material, identifier), "identifier", NULL },
};
br_file_struct br_material_F = { "br_material", BR_ASIZE(br_material_FM), br_material_FM, sizeof(br_material) };

struct {
    br_uint_32 id;
    size_t offset;
    int table;
} MaterialMaps[5] = {
    { CHUNKID_MATERIAL_COLOUR_MAP, offsetof(br_material, colour_map), 0 },
    { CHUNKID_MATERIAL_INDEX_BLEND, offsetof(br_material, index_blend), 1 },
    { CHUNKID_MATERIAL_INDEX_SHADE, offsetof(br_material, index_shade), 1 },
    { CHUNKID_MATERIAL_SCREENDOOR, offsetof(br_material, screendoor), 1 },
    { CHUNKID_MATERIAL_INDEX_FOG, offsetof(br_material, index_fog), 1 },
};

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
br_file_enum actor_type_F = { BR_ASIZE(actor_type_FM), actor_type_FM };

br_file_enum_member render_style_FM[8] = {
    { 0, "BR_RSTYLE_DEFAULT"},
    { 1, "BR_RSTYLE_NONE"},
    { 2, "BR_RSTYLE_POINTS"},
    { 3, "BR_RSTYLE_EDGES"},
    { 4, "BR_RSTYLE_FACES"},
    { 5, "BR_RSTYLE_BOUNDING_POINTS"},
    { 6, "BR_RSTYLE_BOUNDING_EDGES"}, 
    { 7, "BR_RSTYLE_BOUNDING_FACES"},
};
br_file_enum render_style_F = { 8, render_style_FM };

br_file_struct_member br_actor_FM[3] = {
    { DF_TYPE_ENUM_8, offsetof(br_actor, type), "type", &actor_type_F },
    { DF_TYPE_ENUM_8, offsetof(br_actor, render_style), "render_style", &render_style_F },
    { DF_TYPE_ASCIZ, offsetof(br_actor, identifier), "identifier", NULL }
};
br_file_struct br_actor_F = { "br_actor", BR_ASIZE(br_actor_FM), br_actor_FM, sizeof(br_actor) };

br_file_struct_member br_transform_matrix34_FM[4] = {
    { DF_TYPE_BR_VECTOR3, offsetof(br_transform, t.mat.m[0]), "t.mat.m[0]", NULL },
    { DF_TYPE_BR_VECTOR3, offsetof(br_transform, t.mat.m[1]), "t.mat.m[1]", NULL },
    { DF_TYPE_BR_VECTOR3, offsetof(br_transform, t.mat.m[2]), "t.mat.m[2]", NULL },
    { DF_TYPE_BR_VECTOR3, offsetof(br_transform, t.mat.m[3]), "t.mat.m[3]", NULL }
};
br_file_struct br_transform_matrix34_F = { "br_transform_matrix34", BR_ASIZE(br_transform_matrix34_FM), br_transform_matrix34_FM, sizeof(br_transform) };

br_file_struct_member br_transform_quat_FM[5] = {
    { DF_TYPE_BR_SCALAR, offsetof(br_transform, t.quat.q.x), "t.quat.q.x", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_transform, t.quat.q.y), "t.quat.q.y", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_transform, t.quat.q.z), "t.quat.q.z", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_transform, t.quat.q.w), "t.quat.q.w", NULL },
    { DF_TYPE_BR_VECTOR3, offsetof(br_transform, t.quat.t), "t.quat.t", NULL },
};
br_file_struct br_transform_quat_F = { "br_transform_quat", BR_ASIZE(br_transform_quat_FM), br_transform_quat_FM, sizeof(br_transform) };

br_file_enum_member angle_order_FM[24] = {
    { 0, "BR_EULER_XYZ_S" },
    { 8, "BR_EULER_XYX_S" },
    { 4, "BR_EULER_XZY_S" },
    { 12, "BR_EULER_XZX_S" },
    { 1, "BR_EULER_YZX_S" },
    { 9, "BR_EULER_YZY_S" },
    { 5, "BR_EULER_YXZ_S" },
    { 13, "BR_EULER_YXY_S" },
    { 2, "BR_EULER_ZXY_S" },
    { 10, "BR_EULER_ZXZ_S" },
    { 6, "BR_EULER_ZYX_S" },
    { 14, "BR_EULER_ZYZ_S" },
    { 16, "BR_EULER_ZYX_R" },
    { 24, "BR_EULER_XYX_R" },
    { 20, "BR_EULER_YZX_R" },
    { 28, "BR_EULER_XZX_R" },
    { 17, "BR_EULER_XZY_R" },
    { 25, "BR_EULER_YZY_R" },
    { 21, "BR_EULER_ZXY_R" },
    { 29, "BR_EULER_YXY_R" },
    { 18, "BR_EULER_YXZ_R" },
    { 26, "BR_EULER_ZXZ_R" },
    { 22, "BR_EULER_XYZ_R" },
    { 30, "BR_EULER_ZYZ_R" },
};
br_file_enum angle_order_F = { BR_ASIZE(angle_order_FM), angle_order_FM };

br_file_struct_member br_transform_euler_FM[5] = {
    { DF_TYPE_ENUM_8, offsetof(br_transform, t.euler.e.order), "t.euler.e.order", &angle_order_F },
    { DF_TYPE_BR_ANGLE, offsetof(br_transform, t.euler.e.a), "t.euler.e.a", NULL },
    { DF_TYPE_BR_ANGLE, offsetof(br_transform, t.euler.e.b), "t.euler.e.b", NULL },
    { DF_TYPE_BR_ANGLE, offsetof(br_transform, t.euler.e.c), "t.euler.e.c", NULL },
    { DF_TYPE_BR_VECTOR3, offsetof(br_transform, t.euler.t), "t.euler.t", NULL },
};
br_file_struct br_transform_euler_F = { "br_transform_euler", BR_ASIZE(br_transform_euler_FM), br_transform_euler_FM, sizeof(br_transform) };

br_file_struct_member br_transform_look_up_FM[3] = {
    { DF_TYPE_BR_VECTOR3, offsetof(br_transform, t.look_up.look), "t.look_up.look", NULL },
    { DF_TYPE_BR_VECTOR3, offsetof(br_transform, t.look_up.up), "t.look_up.up", NULL },
    { DF_TYPE_BR_VECTOR3, offsetof(br_transform, t.look_up.t), "t.look_up.t", NULL },
};
br_file_struct br_transform_look_up_F =  { "br_transform_look_up", BR_ASIZE(br_transform_look_up_FM), br_transform_look_up_FM, sizeof(br_transform) };

br_file_struct_member br_transform_translation_FM[1] = {
    { DF_TYPE_BR_VECTOR3, offsetof(br_transform, t.translate.t), "t.translate.t", NULL },
};
br_file_struct br_transform_translation_F = { "br_transform_translation", BR_ASIZE(br_transform_translation_FM), br_transform_translation_FM, sizeof(br_transform) };

transform_type TransformTypes[7] = {
    { CHUNKID_TRANSFORM_MAT34, &br_transform_matrix34_F },
    { CHUNKID_TRANSFORM_MAT34_LP, &br_transform_matrix34_F },
    { CHUNKID_TRANSFORM_QUAT, &br_transform_quat_F },
    { CHUNKID_TRANSFORM_EULER, &br_transform_euler_F },
    { CHUNKID_TRANSFORM_LOOK_UP, &br_transform_look_up_F },
    { CHUNKID_TRANSFORM_TRANSLATION, &br_transform_translation_F },
    { CHUNKID_TRANSFORM_IDENTITY, NULL }
};

br_file_struct_member br_bounds3_FM[2] = {
    { DF_TYPE_BR_VECTOR3, offsetof(br_bounds3, min), "min", NULL },
    { DF_TYPE_BR_VECTOR3, offsetof(br_bounds3, max), "max", NULL },
};
br_file_struct br_bounds3_F = { "br_bounds3", BR_ASIZE(br_bounds3_FM), br_bounds3_FM, sizeof(br_bounds3) };

br_file_struct_member br_plane_FM[1] = {
    { DF_TYPE_BR_VECTOR4, offsetof(br_vector4, v), "eqn", NULL },
};
br_file_struct br_plane_F = { "br_plane", BR_ASIZE(br_plane_FM), br_plane_FM, sizeof(br_vector4) };

br_file_enum_member light_type_FM[6] = {
    { 0, "BR_LIGHT_POINT"},
    { 1, "BR_LIGHT_DIRECT"},
    { 2, "BR_LIGHT_SPOT"},
    { 4, "BR_LIGHT_VIEW|BR_LIGHT_POINT"},
    { 5, "BR_LIGHT_VIEW|BR_LIGHT_DIRECT"},
    { 6, "BR_LIGHT_VIEW|BR_LIGHT_SPOT"},
};
br_file_enum light_type_F = { BR_ASIZE(light_type_FM), light_type_FM };

br_file_struct_member br_light_FM[8] = {
    { DF_TYPE_ENUM_8, offsetof(br_light, type), "type", &light_type_F },
    { DF_TYPE_BR_COLOUR, offsetof(br_light, colour), "colour", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_light, attenuation_c), "attenuation_c", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_light, attenuation_l), "attenuation_l", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_light, attenuation_q), "attenuation_q", NULL },
    { DF_TYPE_BR_ANGLE, offsetof(br_light, cone_inner), "cone_inner", NULL },
    { DF_TYPE_BR_ANGLE, offsetof(br_light, cone_outer), "cone_outer", NULL },
    { DF_TYPE_ASCIZ, offsetof(br_light, identifier), "identifier", NULL },
};
br_file_struct br_light_F = { "br_light", BR_ASIZE(br_light_FM), br_light_FM, sizeof(br_light) };

br_file_enum_member camera_type_FM[2] = {
    { 0, "BR_CAMERA_PARALLEL"},
    { 1, "BR_CAMERA_PERSPECTIVE"},
};
br_file_enum camera_type_F = { BR_ASIZE(camera_type_FM), camera_type_FM };

br_file_struct_member br_camera_FM[6] = {
    { DF_TYPE_ENUM_8, offsetof(br_camera, type), "type", &camera_type_F },
    { DF_TYPE_BR_ANGLE, offsetof(br_camera, field_of_view), "field_of_view", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_camera, hither_z), "hither_z", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_camera, yon_z), "yon_z", NULL },
    { DF_TYPE_BR_SCALAR, offsetof(br_camera, aspect), "aspect", NULL },
    { DF_TYPE_ASCIZ, offsetof(br_camera, identifier), "identifier", NULL },
};
br_file_struct br_camera_F = { "br_camera", BR_ASIZE(br_camera_FM), br_camera_FM, sizeof(br_camera) };

br_chunks_table_entry ModelLoadEntries[15] = {
    { CHUNKID_END, 0u, FopRead_END },
    { CHUNKID_OLD_MATERIAL_INDEX, 0u, FopRead_OLD_MATERIAL_INDEX },
    { CHUNKID_OLD_VERTICES, 0u, FopRead_OLD_VERTICES },
    { CHUNKID_OLD_VERTICES_UV, 0u, FopRead_OLD_VERTICES_UV },
    { CHUNKID_OLD_FACES, 0u, FopRead_OLD_FACES },
    { CHUNKID_OLD_MODEL, 0u, FopRead_OLD_MODEL },
    { CHUNKID_OLD_FACES_1, 1u, FopRead_OLD_FACES_1 },
    { CHUNKID_OLD_MODEL_1, 0u, FopRead_OLD_MODEL_1 },
    { CHUNKID_MODEL, 0u, FopRead_MODEL },
    { CHUNKID_MATERIAL_INDEX, 1u, FopRead_MATERIAL_INDEX },
    { CHUNKID_MODEL_VERTICES, 1u, FopRead_VERTICES },
    { CHUNKID_MODEL_VERTEX_UV, 1u, FopRead_VERTEX_UV },
    { CHUNKID_MODEL_FACES, 1u, FopRead_FACES },
    { CHUNKID_MODEL_FACE_MATERIAL, 0u, FopRead_FACE_MATERIAL },
    { CHUNKID_MODEL_PIVOT, 0u, FopRead_PIVOT },
};
br_chunks_table ModelLoadTable = { BR_ASIZE(ModelLoadEntries), ModelLoadEntries };

br_chunks_table_entry ActorLoadEntries[21] = {
    { CHUNKID_END, 0u, FopRead_END },
    { CHUNKID_ACTOR, 0u, FopRead_ACTOR },
    { CHUNKID_ACTOR_MODEL, 0u, FopRead_ACTOR_MODEL },
    { CHUNKID_ACTOR_TRANSFORM, 0u, FopRead_ACTOR_TRANSFORM },
    { CHUNKID_ACTOR_MATERIAL, 0u, FopRead_ACTOR_MATERIAL },
    { CHUNKID_ACTOR_LIGHT, 0u, FopRead_ACTOR_LIGHT },
    { CHUNKID_ACTOR_CAMERA, 0u, FopRead_ACTOR_CAMERA },
    { CHUNKID_ACTOR_BOUNDS, 0u, FopRead_ACTOR_BOUNDS },
    { CHUNKID_ACTOR_CLIP_PLANE, 0u, FopRead_ACTOR_CLIP_PLANE },
    { CHUNKID_ACTOR_ADD_CHILD, 0u, FopRead_ACTOR_ADD_CHILD },
    { CHUNKID_TRANSFORM_MAT34, 0u, FopRead_TRANSFORM },
    { CHUNKID_TRANSFORM_MAT34_LP, 0u, FopRead_TRANSFORM },
    { CHUNKID_TRANSFORM_QUAT, 0u, FopRead_TRANSFORM },
    { CHUNKID_TRANSFORM_EULER, 0u, FopRead_TRANSFORM },
    { CHUNKID_TRANSFORM_LOOK_UP, 0u, FopRead_TRANSFORM },
    { CHUNKID_TRANSFORM_TRANSLATION, 0u, FopRead_TRANSFORM },
    { CHUNKID_TRANSFORM_IDENTITY, 0u, FopRead_TRANSFORM },
    { CHUNKID_BOUNDS, 0u, FopRead_BOUNDS },
    { CHUNKID_LIGHT, 0u, FopRead_LIGHT },
    { CHUNKID_CAMERA, 0u, FopRead_CAMERA },
    { CHUNKID_PLANE, 0u, FopRead_PLANE },
};
br_chunks_table ActorLoadTable = { BR_ASIZE(ActorLoadEntries), ActorLoadEntries };

br_chunks_table_entry MaterialLoadEntries[8] = {
    { CHUNKID_END, 0u, FopRead_END },
    { CHUNKID_MATERIAL_OLD, 0u, FopRead_MATERIAL_OLD },
    { CHUNKID_MATERIAL_COLOUR_MAP, 0u, FopRead_PIXELMAP_REF },
    { CHUNKID_MATERIAL_INDEX_BLEND, 0u, FopRead_PIXELMAP_REF },
    { CHUNKID_MATERIAL_INDEX_SHADE, 0u, FopRead_PIXELMAP_REF },
    { CHUNKID_MATERIAL_SCREENDOOR, 0u, FopRead_PIXELMAP_REF },
    { CHUNKID_MATERIAL_INDEX_FOG, 0u, FopRead_PIXELMAP_REF },
    { CHUNKID_MATERIAL, 0u, FopRead_MATERIAL },
};
br_chunks_table MaterialLoadTable = { BR_ASIZE(MaterialLoadEntries), MaterialLoadEntries };

// IDA: int __usercall FopWrite_VERTICES@<EAX>(br_datafile *df@<EAX>, br_vertex *vertices@<EDX>, int nvertices@<EBX>)
int FopWrite_VERTICES(br_datafile* df, br_vertex* vertices, int nvertices) {
    LOG_TRACE("(%p, %p, %d)", df, vertices, nvertices);

    df->prims->chunk_write(df, CHUNKID_MODEL_VERTICES, df->prims->struct_size(df, &br_vertex_F, NULL) * nvertices + df->prims->count_size(df));
    df->prims->count_write(df, nvertices);
    DfStructWriteArray(df, &br_vertex_F, vertices, nvertices);
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

    count = length / df->prims->struct_size(df, &br_vertex_F, NULL);
    ptr = BrResAllocate(v1db.res, count * sizeof(br_vertex), BR_MEMORY_VERTICES);
    DfStructReadArray(df, &br_vertex_F, ptr, count);
    DfPush(DF_VERTEX, ptr, count);
    return 0;
}

// IDA: int __usercall FopWrite_VERTEX_UV@<EAX>(br_datafile *df@<EAX>, br_vertex *vertices@<EDX>, int nvertices@<EBX>)
int FopWrite_VERTEX_UV(br_datafile* df, br_vertex* vertices, int nvertices) {
    LOG_TRACE("(%p, %p, %d)", df, vertices, nvertices);

    df->prims->chunk_write(df, CHUNKID_MODEL_VERTEX_UV, df->prims->struct_size(df, &br_vertex_uv_F, NULL) * nvertices + df->prims->count_size(df));
    df->prims->count_write(df, nvertices);
    DfStructWriteArray(df, &br_vertex_uv_F, vertices, nvertices);
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

    count = length / df->prims->struct_size(df, &br_old_vertex_uv_F, NULL);
    ptr = BrResAllocate(v1db.res, count * sizeof(br_vertex), BR_MEMORY_VERTICES);
    DfStructReadArray(df, &br_old_vertex_uv_F, ptr, count);
    DfPush(DF_VERTEX, ptr, count);
}

// IDA: int __usercall FopRead_MATERIAL_INDEX@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_MATERIAL_INDEX(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    char name[256];
    br_material** mip;
    br_uint_32 i;
    LOG_TRACE9("(%p, %d, %d, %d)", df, id, length, count);

    mip = (br_material**)BrResAllocate(v1db.res, sizeof(br_material*) * (count + 1), BR_MEMORY_MATERIAL_INDEX);
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

    s = df->prims->count_size(df);
    for (i = 0; i < nmaterials; i++) {
        s += df->prims->name_size(df, materials[i]->identifier);
    }
    df->prims->chunk_write(df, CHUNKID_MATERIAL_INDEX, s);
    df->prims->count_write(df, nmaterials);
    for (i = 0; i < nmaterials; i++) {
        df->prims->name_write(df, materials[i]->identifier);
    }
}

// IDA: int __usercall FopRead_OLD_MATERIAL_INDEX@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_OLD_MATERIAL_INDEX(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    char* mblock;
    char* cp;
    int i;
    int num_materials;
    br_material** mip;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    mblock = BrScratchAllocate(length);
    if (BrFileRead(mblock, 1, length, df->h) != length) {
        BrFailure("could not read material index");
    }
    num_materials = 0;
    cp = mblock;
    for (i = 0; i < length; i++) {
        if (*cp == '\0') {
            num_materials++;
        }
        cp++;
    }
    mip = BrResAllocate(v1db.res, num_materials * sizeof(br_material*), BR_MEMORY_MATERIAL_INDEX);
    cp = mblock;
    for (i = 0; i < num_materials; i++) {
        mip[i] = BrMaterialFind(cp);
        while (*cp != '\0') {
            cp++;
        }
        cp++;
    }
    BrScratchFree(mblock);
    DfPush(DF_MATERIAL_INDEX, mip, num_materials);
    return 0;
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
    }
    return 0;
}

// IDA: int __usercall FopWrite_FACES@<EAX>(br_datafile *df@<EAX>, br_face *faces@<EDX>, int nfaces@<EBX>)
int FopWrite_FACES(br_datafile* df, br_face* faces, int nfaces) {
    LOG_TRACE("(%p, %p, %d)", df, faces, nfaces);

    df->prims->chunk_write(df, CHUNKID_MODEL_FACES, df->prims->struct_size(df, &br_face_F, NULL) * nfaces + df->prims->count_size(df));
    df->prims->count_write(df, nfaces);
    DfStructWriteArray(df, &br_face_F, faces, nfaces);
    return 0;
}

// IDA: int __usercall FopRead_OLD_FACES_1@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_OLD_FACES_1(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    int i;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    mp = DfTop(DF_MODEL, NULL);
    mp->faces = BrResAllocate(mp, count * sizeof(br_face), BR_MEMORY_FACES);
    mp->nfaces = count;
    DfStructReadArray(df, &br_old_face_1_F, mp->faces, (br_uint_16)count);
    for (i = 0; i < mp->nfaces; i++) {
        if (mp->faces[i].smoothing == 0) {
            mp->faces[i].smoothing = -1;
        } else {
            mp->faces[i].smoothing = 1 << ((mp->faces[i].smoothing - 1) % 16);
        }
    }
    return 0;
}

// IDA: int __usercall FopRead_OLD_FACES@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_OLD_FACES(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_face* fp;
    br_material** mip;
    int mi_count;
    unsigned int i;
    void* ptr;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    ptr = DfPop(DF_VERTEX, (int*)&i);
    mip = DfPop(DF_MATERIAL_INDEX, &mi_count);
    DfPush(DF_VERTEX, ptr, i);
    mi_count = length / df->prims->struct_size(df, &br_old_face_F, NULL);
    fp = BrResAllocate(v1db.res, sizeof(br_face) * mi_count, BR_MEMORY_FACES);
    DfStructReadArray(df, &br_old_face_F, fp, mi_count);
    for (i = 0; i < mi_count; i++) {
        fp[i].material = mip[*(br_uint_16*)&fp[i].material];
        if (fp[i].smoothing == 0) {
            fp[i].smoothing = -1;
        }
    }
    BrResFree(mip);
    DfPush(DF_FACE, fp, mi_count);
    return 0;
}

// IDA: int __usercall FopWrite_FACE_MATERIAL@<EAX>(br_datafile *df@<EAX>, br_face *faces@<EDX>, int nfaces@<EBX>, br_material **mindex@<ECX>, int nmaterials)
int FopWrite_FACE_MATERIAL(br_datafile* df, br_face* faces, int nfaces, br_material** mindex, int nmaterials) {
    br_uint_16* block;
    br_uint_16* ip;
    br_face* fp;
    int i;
    int j;
    LOG_TRACE("(%p, %p, %d, %p, %d)", df, faces, nfaces, mindex, nmaterials);

    FopWrite_MATERIAL_INDEX(df, mindex, nmaterials);
    block = BrResAllocate(v1db.res, nfaces * sizeof(br_uint_16), BR_MEMORY_MATERIAL_INDEX);
    for (i = 0; i < nfaces; i++) {
        block[i] = 0;
        if (faces[i].material != NULL) {
            for (j = 0; j < nmaterials; j++) {
                if (faces[i].material == mindex[j]) {
                    block[i] = j + 1;
                    break;
                }
            }
        }
    }
    df->prims->chunk_write(df, CHUNKID_MODEL_FACE_MATERIAL, df->prims->block_size(df, block, nfaces, 0, 1, sizeof(br_uint_16)));
    df->prims->block_write(df, block, nfaces, 0, 1, sizeof(br_uint_16));
    BrResFree(block);
    return 0;
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
    
    memcpy(&temp_model, mp, sizeof(br_model));
    df->prims->chunk_write(df, CHUNKID_MODEL, df->prims->struct_size(df, &br_model_F, mp));
    df->prims->struct_write(df, &br_model_F, mp);
    return 0;
}

// IDA: int __usercall FopRead_OLD_MODEL_1@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_OLD_MODEL_1(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    mp = BrModelAllocate(NULL, 0, 0);
    df->res = mp;
    df->prims->struct_read(df, &br_old_model_1_F, mp);
    DfPush(DF_MODEL, df->res, 1);
    return 0;
}

// IDA: int __usercall FopRead_OLD_MODEL@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_OLD_MODEL(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    int i;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    mp = BrModelAllocate(NULL, 0, 0);
    df->res = mp;
    df->prims->struct_read(df, &br_model_F, mp);
    df->res = NULL;
    mp->faces = DfPop(DF_FACE, &i);
    mp->nfaces = i;
    mp->vertices = DfPop(DF_VERTEX, &i);
    mp->nvertices = i;
    BrResAdd(mp, mp->faces);
    BrResAdd(mp, mp->vertices);
    mp->flags = mp->flags & (BR_STATE_SURFACE | BR_STATE_MATRIX | BR_STATE_ENABLE | BR_STATE_CLIP);
    DfPush(DF_MODEL, mp, 1);
    return 0;
}

// IDA: int __usercall FopRead_PIVOT@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_PIVOT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_model* mp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    mp = DfPop(DF_MODEL, NULL);
    df->res = mp;
    df->prims->struct_read(df, &br_pivot_F, mp);
    df->res = NULL;
    DfPush(DF_MODEL, mp, 1);
    return 0;
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

    mp = BrMaterialAllocate(NULL);
    df->res = mp;
    df->prims->struct_read(df, &br_material_F, mp);
    df->res = NULL;
    DfPush(DF_MATERIAL, mp, 1);
    return 0;
}

// IDA: int __usercall FopWrite_MATERIAL@<EAX>(br_datafile *df@<EAX>, br_material *mp@<EDX>)
int FopWrite_MATERIAL(br_datafile* df, br_material* mp) {
    LOG_TRACE("(%p, %p)", df, mp);

    df->prims->chunk_write(df, CHUNKID_MATERIAL, df->prims->struct_size(df, &br_material_F, mp));
    df->prims->struct_write(df,&br_material_F, mp);
    return 0;
}

// IDA: int __usercall FopRead_PIXELMAP_REF@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_PIXELMAP_REF(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_pixelmap* pm;
    char name[256];
    char* mp;
    int i;
    LOG_TRACE9("(%p, %d, %d, %d)", df, id, length, count);

    for (i = 0; i < BR_ASIZE(MaterialMaps); i++) {
        if (id == MaterialMaps[i].id) {
            break;
        }
    }

    mp = DfTop(DF_MATERIAL, NULL);
    df->prims->name_read(df, name);
    if (MaterialMaps[i].table != 0) {
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

    df->prims->chunk_write(df, id, df->prims->name_size(df, pixelmap->identifier));
    df->prims->name_write(df, pixelmap->identifier);
    return 0;
}

// IDA: int __usercall FopWrite_ACTOR@<EAX>(br_datafile *df@<EAX>, br_actor *ap@<EDX>)
int FopWrite_ACTOR(br_datafile* df, br_actor* ap) {
    LOG_TRACE("(%p, %p)", df, ap);

    df->prims->chunk_write(df, CHUNKID_ACTOR, df->prims->struct_size(df, &br_actor_F, ap));
    df->prims->struct_write(df, &br_actor_F, ap);
    return 0;
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

    df->prims->chunk_write(df, CHUNKID_ACTOR_MODEL, df->prims->name_size(df, model->identifier));
    df->prims->name_write(df, model->identifier);
    return 0;
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

    df->prims->chunk_write(df, CHUNKID_ACTOR_MATERIAL, df->prims->name_size(df, material->identifier));
    df->prims->name_write(df, material->identifier);
    return 0;
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

    df->prims->chunk_write(df, CHUNKID_ACTOR_TRANSFORM, 0);
    return 0;
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

    df->prims->chunk_write(df, CHUNKID_ACTOR_LIGHT, 0);
    return 0;
}

// IDA: int __usercall FopRead_ACTOR_LIGHT@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_ACTOR_LIGHT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_light* lp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    lp = DfPop(DF_LIGHT, NULL);
    a = DfTop(DF_ACTOR, NULL);
    a->type_data = lp;
    return 0;
}

// IDA: int __usercall FopWrite_ACTOR_CAMERA@<EAX>(br_datafile *df@<EAX>)
int FopWrite_ACTOR_CAMERA(br_datafile* df) {
    LOG_TRACE("(%p)", df);

    df->prims->chunk_write(df, CHUNKID_ACTOR_CAMERA, 0);
    return 0;
}

// IDA: int __usercall FopRead_ACTOR_CAMERA@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_ACTOR_CAMERA(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_light* cp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    cp = DfPop(DF_CAMERA, NULL);
    a = DfTop(DF_ACTOR, NULL);
    a->type_data = cp;
    return 0;
}

// IDA: int __usercall FopWrite_ACTOR_BOUNDS@<EAX>(br_datafile *df@<EAX>)
int FopWrite_ACTOR_BOUNDS(br_datafile* df) {
    LOG_TRACE("(%p)", df);

    df->prims->chunk_write(df, CHUNKID_ACTOR_BOUNDS, 0);
    return 0;
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

    df->prims->chunk_write(df, CHUNKID_ACTOR_CLIP_PLANE, 0);
    return 0;
}

// IDA: int __usercall FopRead_ACTOR_CLIP_PLANE@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_ACTOR_CLIP_PLANE(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_actor* a;
    br_vector4* vp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    vp = DfPop(DF_CLIP_PLANE, NULL);
    a = DfTop(DF_ACTOR, NULL);
    a->type_data = vp;
    return 0;
}

// IDA: int __usercall FopWrite_ACTOR_ADD_CHILD@<EAX>(br_datafile *df@<EAX>)
int FopWrite_ACTOR_ADD_CHILD(br_datafile* df) {
    LOG_TRACE("(%p)", df);

    df->prims->chunk_write(df, CHUNKID_ACTOR_ADD_CHILD, 0);
    return 0;
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

    tt = &TransformTypes[t->type];
    if (tt->fs == NULL) {
        df->prims->chunk_write(df, tt->id, 0);
    } else {
        df->prims->chunk_write(df, tt->id, df->prims->struct_size(df, tt->fs, t));
        df->prims->struct_size(df, tt->fs, &t);
        df->prims->struct_write(df, tt->fs, t);
    }
    return 0;
}

// IDA: int __usercall FopRead_TRANSFORM@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_TRANSFORM(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    int t;
    br_transform* tp;
    LOG_TRACE9("(%p, %d, %d, %d)", df, id, length, count);

    for (t = 0; t < BR_ASIZE(TransformTypes); t++) {
        if (id == TransformTypes[t].id) {
            break;
        }
    }
    if (t == BR_ASIZE(TransformTypes)) {
        LOG_PANIC("transform type not found!");
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

    df->prims->chunk_write(df, CHUNKID_BOUNDS, df->prims->struct_size(df, &br_bounds3_F, bp));
    df->prims->struct_write(df, &br_bounds3_F, bp);
    return 0;
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

    df->prims->chunk_write(df, CHUNKID_PLANE, df->prims->struct_size(df, &br_plane_F, pp));
    df->prims->struct_write(df, &br_plane_F, pp);
    return 0;
}

// IDA: int __usercall FopRead_PLANE@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_PLANE(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_vector4* pp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    pp = BrResAllocate(v1db.res, sizeof(br_vector4), BR_MEMORY_CLIP_PLANE);
    df->res = pp;
    df->prims->struct_read(df, &br_plane_F, pp);
    df->res = NULL;
    DfPush(DF_CLIP_PLANE, pp, 1);
    return 0;
}

// IDA: int __usercall FopWrite_LIGHT@<EAX>(br_datafile *df@<EAX>, br_light *lp@<EDX>)
int FopWrite_LIGHT(br_datafile* df, br_light* lp) {
    LOG_TRACE("(%p, %p)", df, lp);

    df->prims->chunk_write(df, CHUNKID_LIGHT, df->prims->struct_size(df, &br_light_F, lp));
    df->prims->struct_write(df, &br_light_F, lp);
    return 0;
}

// IDA: int __usercall FopRead_LIGHT@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_LIGHT(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_light* lp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    lp = BrResAllocate(v1db.res, sizeof(br_light), BR_MEMORY_LIGHT);
    df->res = lp;
    df->prims->struct_read(df, &br_light_F, lp);
    df->res = NULL;
    DfPush(DF_LIGHT, lp, 1);
    return 0;
}

// IDA: int __usercall FopWrite_CAMERA@<EAX>(br_datafile *df@<EAX>, br_camera *cp@<EDX>)
int FopWrite_CAMERA(br_datafile* df, br_camera* cp) {
    LOG_TRACE("(%p, %p)", df, cp);

    df->prims->chunk_write(df, CHUNKID_CAMERA, df->prims->struct_size(df, &br_camera_F, cp));
    df->prims->struct_write(df, &br_camera_F, cp);
    return 0;
}

// IDA: int __usercall FopRead_CAMERA@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_CAMERA(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_camera* cp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    cp = BrResAllocate(v1db.res, sizeof(br_camera), BR_MEMORY_CAMERA);
    df->res = cp;
    df->prims->struct_read(df, &br_camera_F, cp);
    df->res = NULL;
    DfPush(DF_CAMERA, cp, 1);
    return 0;
}

// IDA: br_uint_32 __cdecl BrModelLoadMany(char *filename, br_model **models, br_uint_16 num)
br_uint_32 BrModelLoadMany(char* filename, br_model** models, br_uint_16 num) {
    int count;
    int r;
    br_datafile* df;
    LOG_TRACE9("(\"%s\", %p, %d)", filename, models, num);

    df = DfOpen(filename, 0, BRT_FLOAT);
    if (df == NULL) {
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
    } while (r != 0);
    DfClose(df);
    return count;
}

// IDA: int __cdecl PtrCompare(void *a, void *b)
int PtrCompare(void* a, void* b) {
    LOG_TRACE("(%p, %p)", a, b);

    if (*(uintptr_t*)a > *(uintptr_t*)b) {
        return -1;
    }
    return *(uintptr_t*)a < *(uintptr_t*)b;
}

// IDA: br_uint_32 __cdecl WriteModel(br_model *mp, br_datafile *df)
br_uint_32 WriteModel(br_model* mp, br_datafile* df) {
    br_material** mindex;
    br_vertex* vp;
    int nmaterials;
    int i;
    int has_uv;
    LOG_TRACE("(%p, %p)", mp, df);

    nmaterials = 0;
    if (mp->vertices == NULL || mp->faces == NULL) {
        if (mp->identifier != NULL) {
            BrWarning(mp->identifier);
        }
        BrWarning("Model must have vertex and face information to save.");
        BrWarning("This information may have neem stripped on addition to");
        BrWarning("the registry. To prevent this, set the BR_MODF_UPDATEABLE");
        BrFailure("flag before adding the model to the registry.");
    }
    mindex = BrResAllocate(v1db.res, mp->nfaces * sizeof(br_material*), BR_MEMORY_MATERIAL_INDEX);
    for (i = 0; i < mp->nfaces; i++) {
        mindex[i] = mp->faces[i].material;
    }
    BrQsort(mindex, mp->nfaces, sizeof(br_material*), (br_qsort_cbfn*)PtrCompare);
    nmaterials = 1;
    for (i = 1; i < mp->nfaces; i++) {
        if (mindex[nmaterials-1] == mindex[i]) {
            mindex[nmaterials] = mindex[i];
            nmaterials++;
        }
    }
    if (mindex[nmaterials-1] == NULL) {
        nmaterials = 0;
    }
    has_uv = 0;
    for (i = 0; i < mp->nvertices; i++) {
        if (mp->vertices[i].map.v[0] != 0.f || mp->vertices[i].map.v[1] != 0.f) {
            has_uv = 1;
            break;
        }
    }

    FopWrite_MODEL(df, mp);
    if (mp->nvertices != 0) {
        FopWrite_VERTICES(df, mp->vertices, mp->nvertices);
        if (has_uv != 0) {
            FopWrite_VERTEX_UV(df, mp->vertices, mp->nvertices);
        }
    }
    if (mp->nfaces != 0) {
        FopWrite_FACES(df, mp->faces, mp->nfaces);
        if (nmaterials != 0) {
            FopWrite_FACE_MATERIAL(df, mp->faces, mp->nfaces, mindex, nmaterials);
        }
    }
    FopWrite_END(df);
    BrResFree(mindex);
    return 0;
}

// IDA: br_uint_32 __cdecl BrModelSaveMany(char *filename, br_model **models, br_uint_16 num)
br_uint_32 BrModelSaveMany(char* filename, br_model** models, br_uint_16 num) {
    br_datafile* df;
    int i;
    int m;
    LOG_TRACE("(\"%s\", %p, %d)", filename, models, num);

    df = DfOpen(filename, 1, BRT_FLOAT);
    if (df == NULL) {
        return 0;
    }
    FopWrite_FILE_INFO(df, FILE_TYPE_MODEL);
    if (models == NULL) {
        BrModelEnum(NULL, (br_model_enum_cbfn*)&WriteModel, df);
        m = BrModelCount(NULL);
    } else {
        for (i = 0; i < num; i++) {
            WriteModel(models[i], df);
        }
        m = num;
    }
    DfClose(df);
    return m;
}

// IDA: br_uint_32 __cdecl BrActorLoadMany(char *filename, br_actor **actors, br_uint_16 num)
br_uint_32 BrActorLoadMany(char* filename, br_actor** actors, br_uint_16 num) {
    br_datafile* df;
    int count;
    int r;
    LOG_TRACE9("(\"%s\", %p, %d)", filename, actors, num);

    df = DfOpen(filename, 0, BRT_FLOAT);
    if (df == NULL) {
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
    } while (r != 0);
    DfClose(df);
    return count;
}

// IDA: int __usercall WriteActor@<EAX>(br_actor *a@<EAX>, br_datafile *df@<EDX>)
int WriteActor(br_actor* a, br_datafile* df) {
    br_actor* ap;
    br_actor* last_ap;
    LOG_TRACE("(%p, %p)", a, df);

    FopWrite_ACTOR(df, a);
    if (a->t.type != BR_TRANSFORM_IDENTITY) {
        FopWrite_TRANSFORM(df, &a->t);
        FopWrite_ACTOR_TRANSFORM(df);
    }
    if (a->material != NULL) {
        FopWrite_ACTOR_MATERIAL(df, a->material);
    }
    if (a->model != NULL) {
        FopWrite_ACTOR_MODEL(df, a->model);
    }
    if (a->type_data != NULL) {
        switch (a->type) {
        case BR_ACTOR_LIGHT:
            FopWrite_LIGHT(df, a->type_data);
            FopWrite_ACTOR_LIGHT(df);
            break;
        case BR_ACTOR_CAMERA:
            FopWrite_CAMERA(df, a->type_data);
            FopWrite_ACTOR_CAMERA(df);
            break;
        case BR_ACTOR_BOUNDS:
            FopWrite_BOUNDS(df, a->type_data);
            FopWrite_ACTOR_BOUNDS(df);
            break;
        case BR_ACTOR_CLIP_PLANE:
            FopWrite_PLANE(df, a->type_data);
            FopWrite_ACTOR_CLIP_PLANE(df);
            break;
        }
    }
    if (a->children != NULL){
        last_ap = a->children;
        while (last_ap->next != NULL) {
            last_ap = last_ap->next;
        }
        ap = last_ap;
        while (1) {
            WriteActor(ap, df);
            FopWrite_ACTOR_ADD_CHILD(df);
            if (a->children == ap) {
                break;
            }
            ap = (br_actor*)ap->prev;
        }
    }
    return 0;
}

// IDA: br_uint_32 __cdecl BrActorSaveMany(char *filename, br_actor **actors, br_uint_16 num)
br_uint_32 BrActorSaveMany(char* filename, br_actor** actors, br_uint_16 num) {
    br_datafile* df;
    int i;
    LOG_TRACE("(\"%s\", %p, %d)", filename, actors, num);

    df = DfOpen(filename, 1, BRT_FLOAT);
    if (df == NULL) {
        return 0;
    }
    FopWrite_FILE_INFO(df, FILE_TYPE_ACTORS);
    for (i = 0; i < num; i++) {
        WriteActor(actors[i], df);
        FopWrite_END(df);
    }
    DfClose(df);
    return num;
}

// IDA: br_uint_32 __cdecl BrMaterialLoadMany(char *filename, br_material **materials, br_uint_16 num)
br_uint_32 BrMaterialLoadMany(char* filename, br_material** materials, br_uint_16 num) {
    br_datafile* df;
    int count;
    int r;

    df = DfOpen(filename, 0, BRT_FLOAT);
    if (df == NULL) {
        return 0;
    }
    count = 0;
    do {
        if (count >= num) {
            break;
        }
        r = DfChunksInterpret(df, &MaterialLoadTable);
        if (DfTopType() == DF_MATERIAL) {
            materials[count] = DfPop(DF_MATERIAL, 0);
            ++count;
        }
    } while (r != 0);
    DfClose(df);
    return count;
}

// IDA: br_uint_32 __cdecl WriteMaterial(br_material *mp, br_datafile *df)
br_uint_32 WriteMaterial(br_material* mp, br_datafile* df) {
    LOG_TRACE("(%p, %p)", mp, df);

    FopWrite_MATERIAL(df, mp);
    if (mp->colour_map != NULL) {
        FopWrite_PIXELMAP_REF(df, CHUNKID_MATERIAL_COLOUR_MAP, mp->colour_map);
    }
    if (mp->index_shade != NULL) {
        FopWrite_PIXELMAP_REF(df, CHUNKID_MATERIAL_INDEX_SHADE, mp->index_shade);
    }
    if (mp->index_blend != NULL) {
        FopWrite_PIXELMAP_REF(df, CHUNKID_MATERIAL_INDEX_BLEND, mp->index_blend);
    }
    if (mp->screendoor != NULL) {
        FopWrite_PIXELMAP_REF(df, CHUNKID_MATERIAL_SCREENDOOR, mp->screendoor);
    }
    if (mp->index_fog != NULL) {
        FopWrite_PIXELMAP_REF(df, CHUNKID_MATERIAL_INDEX_FOG, mp->index_fog);
    }
    FopWrite_END(df);
    return 0;
}

// IDA: br_uint_32 __cdecl BrMaterialSaveMany(char *filename, br_material **materials, br_uint_16 num)
br_uint_32 BrMaterialSaveMany(char* filename, br_material** materials, br_uint_16 num) {
    br_datafile* df;
    int i;
    int count;
    LOG_TRACE("(\"%s\", %p, %d)", filename, materials, num);

    df = DfOpen(filename, 1, BRT_FLOAT);
    if (df == NULL) {
        return 0;
    }
#ifdef BRENDER_FIX_BUGS
    FopWrite_FILE_INFO(df, FILE_TYPE_MATERIAL);
#else
    FopWrite_FILE_INFO(df, FILE_TYPE_MATERIAL_OLD);
#endif
    if (materials == NULL) {
        BrMaterialEnum(NULL, (br_material_enum_cbfn*)WriteMaterial, df);
        count = BrMaterialCount(NULL);
    } else {
        for (i = 0; i < num; i++) {
            WriteMaterial(materials[i], df);
        }
        count = num;
    }
    DfClose(df);
    return count;
}

// IDA: br_model* __cdecl BrModelLoad(char *filename)
br_model* BrModelLoad(char* filename) {
    br_model* ptr;
    LOG_TRACE("(\"%s\")", filename);

    if (BrModelLoadMany(filename, &ptr, 1) == 1) {
        return ptr;
    }
    return NULL;
}

// IDA: br_uint_32 __cdecl BrModelSave(char *filename, br_model *ptr)
br_uint_32 BrModelSave(char* filename, br_model* ptr) {
    LOG_TRACE("(\"%s\", %p)", filename, ptr);

    return BrModelSaveMany(filename, &ptr, 1);
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

    return BrMaterialSaveMany(filename, &ptr, 1);
}

// IDA: br_actor* __cdecl BrActorLoad(char *filename)
br_actor* BrActorLoad(char* filename) {
    br_actor* ptr;
    LOG_TRACE("(\"%s\")", filename);

    if (BrActorLoadMany(filename, &ptr, 1) == 1) {
        return ptr;
    }
    LOG_DEBUG("loadmany fail");
    return NULL;
}

// IDA: br_uint_32 __cdecl BrActorSave(char *filename, br_actor *ptr)
br_uint_32 BrActorSave(char* filename, br_actor* ptr) {
    LOG_TRACE("(\"%s\", %p)", filename, ptr);

    return BrActorSaveMany(filename, &ptr, 1);
}

// IDA: br_error __cdecl BrModelFileCount(char *filename, br_uint_16 *num)
br_error BrModelFileCount(char* filename, br_uint_16* num) {
    br_datafile* df;
    LOG_TRACE("(\"%s\", %p)", filename, num);

    df = DfOpen(filename, 0, BRT_FLOAT);
    if (df == NULL) {
        if (num != NULL) {
            *num = 0;
        }
        return 0x1002;
    }
    if (num == NULL) {
        return 0;
    }
    *num = 0;
    while (DfChunksInterpret(df, &ModelLoadTable) != 0) {
        if (DfTopType() == DF_MODEL) {
            *num = *num + 1;
            DfPop(DF_MODEL, NULL);
        }
    }
    DfClose(df);
    return 0;
}

// IDA: br_error __cdecl BrActorFileCount(char *filename, br_uint_16 *num)
br_error BrActorFileCount(char* filename, br_uint_16* num) {
    br_datafile* df;
    LOG_TRACE("(\"%s\", %p)", filename, num);

    df = DfOpen(filename, 0, BRT_FLOAT);
    if (df == NULL) {
        if (num != NULL) {
            *num = 0;
        }
        return 0x1002;
    }
    if (num == NULL) {
        return 0;
    }
    *num = 0;
    while (DfChunksInterpret(df, &ActorLoadTable) != 0) {
        if (DfTopType() == DF_ACTOR) {
            *num = *num + 1;
            DfPop(DF_ACTOR, NULL);
        }
    }
    DfClose(df);
    return 0;
}

// IDA: br_error __cdecl BrMaterialFileCount(char *filename, br_uint_16 *num)
br_error BrMaterialFileCount(char* filename, br_uint_16* num) {
    br_datafile* df;
    LOG_TRACE("(\"%s\", %p)", filename, num);

    df = DfOpen(filename, 0, BRT_FLOAT);
    if (df == NULL) {
        if (num != NULL) {
            *num = 0;
        }
        return 0x1002;
    }
    if (num == NULL) {
        return 0;
    }
    *num = 0;
    while (DfChunksInterpret(df, &MaterialLoadTable) != 0) {
        if (DfTopType() == DF_MATERIAL) {
            *num = *num + 1;
            DfPop(DF_MATERIAL, NULL);
        }
    }
    DfClose(df);
    return 0;
}
