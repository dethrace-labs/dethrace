#ifndef _DATAFILE_H_
#define _DATAFILE_H_

#include "brender/br_types.h"

// The names of these enum values are a wild guess
enum {
    DF_TYPE_BR_INT_8 = 0,
    DF_TYPE_BR_UINT_8 = 1,
    DF_TYPE_BR_INT_16 = 2,
    DF_TYPE_BR_UINT_16 = 3,
    DF_TYPE_BR_INT_32 = 4,
    DF_TYPE_BR_UINT_32 = 5,
    DF_TYPE_BR_FIXED = 6,
    DF_TYPE_BR_ANGLE = 7,
    DF_TYPE_FLOAT = 8,
    DF_TYPE_DOUBLE = 9,
    DF_TYPE_BR_SCALAR = 10,
    DF_TYPE_BR_FRACTION = 11,
    DF_TYPE_BR_UFRACTION = 12,
    DF_TYPE_ENUM_8 = 13,
    DF_TYPE_ENUM_16 = 14,
    DF_TYPE_ENUM_32 = 15,
    DF_TYPE_STRUCT = 16,
    DF_TYPE_ASCIZ = 17,
    DF_TYPE_BR_COLOUR = 18,
    DF_TYPE_BR_VECTOR2 = 19,
    DF_TYPE_BR_VECTOR3 = 20,
    DF_TYPE_BR_VECTOR4 = 21,
    DF_TYPE_BR_FRACTION_X = 22,
    DF_TYPE_BR_UFRACTION_X = 23,
    DF_TYPE_BR_FRACTION_F = 24,
    DF_TYPE_BR_UFRACTION_F = 25,
    DF_TYPE_BR_VECTOR2_X = 26,
    DF_TYPE_BR_VECTOR3_X = 27,
    DF_TYPE_BR_VECTOR4_X = 28,
    DF_TYPE_BR_VECTOR2_F = 29,
    DF_TYPE_BR_VECTOR3_F = 30,
    DF_TYPE_BR_VECTOR4_F = 31,
    DF_TYPE_COUNT,
};

enum {
    DF_CHUNKID_END = 0,
    DF_CHUNKID_IMAGE_PLANE = 1,
    DF_CHUNKID_RLE_IMAGE_PLANE = 2,
    DF_CHUNKID_PIXELMAP = 3,
    DF_CHUNKID_MATERIAL_OLD = 4,
    DF_CHUNKID_ADD_MATERIAL = 5,
    DF_CHUNKID_OLD_ACTOR = 6,
    DF_CHUNKID_OLD_ADD_SIBLING = 7,
    DF_CHUNKID_OLD_ADD_CHILD = 8,
    DF_CHUNKID_OLD_MATERIAL_INDEX = 9,
    DF_CHUNKID_OLD_VERTICES = 10,
    DF_CHUNKID_OLD_VERTICES_UV = 11,
    DF_CHUNKID_OLD_FACES = 12,
    DF_CHUNKID_OLD_MODEL = 13,
    DF_CHUNKID_ADD_MODEL = 14,
    DF_CHUNKID_ANIM = 15,
    DF_CHUNKID_ANIM_TRANSFORM = 16,
    DF_CHUNKID_ANIM_RATE = 17,
    DF_CHUNKID_FILE_INFO = 18,
    DF_CHUNKID_OLD_LIGHT = 19,
    DF_CHUNKID_OLD_CAMERA = 20,
    DF_CHUNKID_PIVOT = 21,
    DF_CHUNKID_MATERIAL_INDEX = 22,
    DF_CHUNKID_VERTICES = 23,
    DF_CHUNKID_VERTEX_UV = 24,
    DF_CHUNKID_OLD_FACES_1 = 25,
    DF_CHUNKID_FACE_MATERIAL = 26,
    DF_CHUNKID_OLD_MODEL_1 = 27,
    DF_CHUNKID_COLOUR_MAP_REF = 28,
    DF_CHUNKID_OPACITY_MAP_REF = 29,
    DF_CHUNKID_INDEX_BLEND_REF = 30,
    DF_CHUNKID_INDEX_SHADE_REF = 31,
    DF_CHUNKID_SCREENDOOR_REF = 32,
    DF_CHUNKID_PIXELS = 33,
    DF_CHUNKID_ADD_MAP = 34,
    DF_CHUNKID_ACTOR = 35,
    DF_CHUNKID_ACTOR_MODEL = 36,
    DF_CHUNKID_ACTOR_TRANSFORM = 37,
    DF_CHUNKID_ACTOR_MATERIAL = 38,
    DF_CHUNKID_ACTOR_LIGHT = 39,
    DF_CHUNKID_ACTOR_CAMERA = 40,
    DF_CHUNKID_ACTOR_BOUNDS = 41,
    DF_CHUNKID_ACTOR_ADD_CHILD = 42,
    DF_CHUNKID_TRANSFORM_MATRIX34 = 43,
    DF_CHUNKID_TRANSFORM_MATRIX34_LP = 44,
    DF_CHUNKID_TRANSFORM_QUAT = 45,
    DF_CHUNKID_TRANSFORM_EULER = 46,
    DF_CHUNKID_TRANSFORM_LOOK_UP = 47,
    DF_CHUNKID_TRANSFORM_TRANSLATION = 48,
    DF_CHUNKID_TRANSFORM_IDENTITY = 49,
    DF_CHUNKID_BOUNDS = 50,
    DF_CHUNKID_LIGHT = 51,
    DF_CHUNKID_CAMERA = 52,
    DF_CHUNKID_FACES = 53,
    DF_CHUNKID_MODEL = 54,
    DF_CHUNKID_ACTOR_CLIP_PLANE = 55,
    DF_CHUNKID_PLANE = 56,
    DF_CHUNKID_SATURN_FACES = 57,
    DF_CHUNKID_SATURN_MODEL = 58,
    DF_CHUNKID_INDEX_FOG_REF = 59,
    DF_CHUNKID_MATERIAL_OLD_2 = 60,
};

void DfPush(int type, void* value, int count);

void* DfPop(int type, int* countp);

void* DfTop(int type, int* countp);

int DfTopType();

int TextReadLine(br_datafile* df, char** ident, char** data);

br_uint_16 scalarTypeConvert(br_datafile* df, br_uint_16 t);

br_uint_32 DfStructWriteBinary(br_datafile* df, br_file_struct* str, void* base);

br_uint_32 DfStructReadBinary(br_datafile* df, br_file_struct* str, void* base);

int DfStructSizeBinary(br_datafile* df, br_file_struct* str, void* base);

int EnumFromString(br_file_enum* e, char* str);

char* EnumToString(br_file_enum* e, int num);

br_uint_32 DfStructWriteText(br_datafile* df, br_file_struct* str, void* base);

br_uint_32 StructWriteTextSub(br_datafile* df, br_file_struct* str, void* base, int indent);

br_uint_32 DfStructReadText(br_datafile* df, br_file_struct* str, void* base);

br_uint_32 StructReadTextSub(br_datafile* df, br_file_struct* str, void* base);

int DfStructSizeText(br_datafile* df, br_file_struct* str, void* base);

br_uint_32 DfStructWriteArray(br_datafile* df, br_file_struct* str, void* base, int n);

br_uint_32 DfStructReadArray(br_datafile* df, br_file_struct* str, void* base, int n);

int DfChunkWriteText(br_datafile* df, br_uint_32 id, br_uint_32 length);

int DfChunkReadText(br_datafile* df, br_uint_32* plength);

int DfChunkWriteBinary(br_datafile* df, br_uint_32 id, br_uint_32 length);

int DfChunkReadBinary(br_datafile* df, br_uint_32* plength);

void DfCountWriteText(br_datafile* df, br_uint_32 count);

br_uint_32 DfCountReadText(br_datafile* df);

void DfCountWriteBinary(br_datafile* df, br_uint_32 count);

br_uint_32 DfCountReadBinary(br_datafile* df);

int DfCountSizeText(br_datafile* df);

int DfCountSizeBinary(br_datafile* df);

br_uint_8* BlockWriteSetup(void* base, int block_size, int block_stride, int block_count, int size);

int DfBlockWriteText(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size);

void* DfBlockReadText(br_datafile* df, void* base, int* count, int size, int mtype);

int DfBlockWriteBinary(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size);

void* DfBlockReadBinary(br_datafile* df, void* base, int* count, int size, int mtype);

int DfBlockSizeText(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size);

int DfBlockSizeBinary(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size);

char* DfNameReadText(br_datafile* df, char* name);

int DfNameWriteText(br_datafile* df, char* name);

int DfNameSizeText(br_datafile* df, char* name);

char* DfNameReadBinary(br_datafile* df, char* name);

int DfNameWriteBinary(br_datafile* df, char* name);

int DfNameSizeBinary(br_datafile* df, char* name);

int DfSkipText(br_datafile* df, br_uint_32 length);

int DfSkipBinary(br_datafile* df, br_uint_32 length);

int DfChunksInterpret(br_datafile* df, br_chunks_table* table);

void BrNullOther();

int DfFileIdentify(br_uint_8* magics, br_size_t n_magics);

br_datafile* DfOpen(char* name, int write, br_token scalar_type);

void DfClose(br_datafile* df);

int BrWriteModeSet(int mode);

#endif
