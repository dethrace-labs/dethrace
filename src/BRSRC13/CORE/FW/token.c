#include "token.h"
#include "CORE/FW/fwsetup.h"
#include "CORE/FW/brlists.h"
#include "CORE/FW/pattern.h"
#include "CORE/FW/resource.h"
#include "CORE/STD/brstdlib.h"
#include "harness/trace.h"

br_token_entry predefinedTokens[457] = {
    { { NULL, NULL, }, "ABGR_1_5_5_5",                 BRT_NONE,             BRT_ABGR_1_5_5_5,             12, },
    { { NULL, NULL, }, "ABGR_4_4_4_4",                 BRT_NONE,             BRT_ABGR_4_4_4_4,             12, },
    { { NULL, NULL, }, "ABGR_8_8_8_8",                 BRT_NONE,             BRT_ABGR_8_8_8_8,             12, },
    { { NULL, NULL, }, "ACCEPT",                       BRT_NONE,             BRT_ACCEPT,                   6,  },
    { { NULL, NULL, }, "AINDEX_4_4",                   BRT_NONE,             BRT_AINDEX_4_4,               10, },
    { { NULL, NULL, }, "AINDEX_8_8",                   BRT_NONE,             BRT_AINDEX_8_8,               10, },
    { { NULL, NULL, }, "ALPHA",                        BRT_NONE,             BRT_ALPHA,                    5,  },
    { { NULL, NULL, }, "ALWAYS",                       BRT_NONE,             BRT_ALWAYS,                   6,  },
    { { NULL, NULL, }, "AMBIENT_F",                    BRT_FLOAT,            BRT_AMBIENT_F,                7,  },
    { { NULL, NULL, }, "AMBIENT_X",                    BRT_FIXED,            BRT_AMBIENT_X,                7,  },
    { { NULL, NULL, }, "ANGLE",                        BRT_NONE,             BRT_ANGLE,                    5,  },
    { { NULL, NULL, }, "ANTIALIASING_T",               BRT_TOKEN,            BRT_ANTIALIASING_T,           12, },
    { { NULL, NULL, }, "ANTIALIASING_TL",              BRT_TOKEN_LIST,       BRT_ANTIALIASING_TL,          12, },
    { { NULL, NULL, }, "ARGB_1_5_5_5",                 BRT_NONE,             BRT_ARGB_1_5_5_5,             12, },
    { { NULL, NULL, }, "ARGB_4_4_4_4",                 BRT_NONE,             BRT_ARGB_4_4_4_4,             12, },
    { { NULL, NULL, }, "ARGB_8_8_8_8",                 BRT_NONE,             BRT_ARGB_8_8_8_8,             12, },
    { { NULL, NULL, }, "ATTENUATION_C_F",              BRT_FLOAT,            BRT_ATTENUATION_C_F,          13, },
    { { NULL, NULL, }, "ATTENUATION_C_X",              BRT_FIXED,            BRT_ATTENUATION_C_X,          13, },
    { { NULL, NULL, }, "ATTENUATION_L_F",              BRT_FLOAT,            BRT_ATTENUATION_L_F,          13, },
    { { NULL, NULL, }, "ATTENUATION_L_X",              BRT_FIXED,            BRT_ATTENUATION_L_X,          13, },
    { { NULL, NULL, }, "ATTENUATION_Q_F",              BRT_FLOAT,            BRT_ATTENUATION_Q_F,          13, },
    { { NULL, NULL, }, "ATTENUATION_Q_X",              BRT_FIXED,            BRT_ATTENUATION_Q_X,          13, },
    { { NULL, NULL, }, "A_4",                          BRT_NONE,             BRT_A_4,                      3,  },
    { { NULL, NULL, }, "A_8",                          BRT_NONE,             BRT_A_8,                      3,  },
    { { NULL, NULL, }, "BEST_QUALITY",                 BRT_NONE,             BRT_BEST_QUALITY,             12, },
    { { NULL, NULL, }, "BEST_SPEED",                   BRT_NONE,             BRT_BEST_SPEED,               10, },
    { { NULL, NULL, }, "BGRA_4_4_4_4",                 BRT_NONE,             BRT_BGRA_4_4_4_4,             12, },
    { { NULL, NULL, }, "BGRA_8_8_8_8",                 BRT_NONE,             BRT_BGRA_8_8_8_8,             12, },
    { { NULL, NULL, }, "BGRX_8_8_8_8",                 BRT_NONE,             BRT_BGRX_8_8_8_8,             12, },
    { { NULL, NULL, }, "BGR_2_3_2",                    BRT_NONE,             BRT_BGR_2_3_2,                9,  },
    { { NULL, NULL, }, "BGR_5_6_5",                    BRT_NONE,             BRT_BGR_5_6_5,                9,  },
    { { NULL, NULL, }, "BGR_8_8_8",                    BRT_NONE,             BRT_BGR_8_8_8,                9,  },
    { { NULL, NULL, }, "BLENDER_PROGRAM_P",            BRT_POINTER,          BRT_BLENDER_PROGRAM_P,        15, },
    { { NULL, NULL, }, "BLEND_B",                      BRT_BOOLEAN,          BRT_BLEND_B,                  5,  },
    { { NULL, NULL, }, "BLEND_CONSTANT",               BRT_NONE,             BRT_BLEND_CONSTANT,           14, },
    { { NULL, NULL, }, "BLEND_DIMMED",                 BRT_NONE,             BRT_BLEND_DIMMED,             12, },
    { { NULL, NULL, }, "BLEND_MODE_T",                 BRT_TOKEN,            BRT_BLEND_MODE_T,             10, },
    { { NULL, NULL, }, "BLEND_MODE_TL",                BRT_TOKEN_LIST,       BRT_BLEND_MODE_TL,            10, },
    { { NULL, NULL, }, "BLEND_STANDARD",               BRT_NONE,             BRT_BLEND_STANDARD,           14, },
    { { NULL, NULL, }, "BLEND_SUMMED",                 BRT_NONE,             BRT_BLEND_SUMMED,             12, },
    { { NULL, NULL, }, "BLUE",                         BRT_NONE,             BRT_BLUE,                     4,  },
    { { NULL, NULL, }, "BOOLEAN",                      BRT_NONE,             BRT_BOOLEAN,                  7,  },
    { { NULL, NULL, }, "BOTTOM_TOP",                   BRT_NONE,             BRT_BOTTOM_TOP,               10, },
    { { NULL, NULL, }, "BOUNDS",                       BRT_NONE,             BRT_BOUNDS,                   6,  },
    { { NULL, NULL, }, "BOUNDS_B",                     BRT_BOOLEAN,          BRT_BOUNDS_B,                 6,  },
    { { NULL, NULL, }, "BRENDER_DEVICES_STR",          BRT_STRING,           BRT_BRENDER_DEVICES_STR,      15, },
    { { NULL, NULL, }, "BRENDER_DRIVERS_STR",          BRT_STRING,           BRT_BRENDER_DRIVERS_STR,      15, },
    { { NULL, NULL, }, "BRENDER_PATH_STR",             BRT_STRING,           BRT_BRENDER_PATH_STR,         12, },
    { { NULL, NULL, }, "BRENDER_VERSION_U32",          BRT_UINT_32,          BRT_BRENDER_VERSION_U32,      15, },
    { { NULL, NULL, }, "BUCKET_SORT",                  BRT_NONE,             BRT_BUCKET_SORT,              11, },
    { { NULL, NULL, }, "BUFFER_STORED",                BRT_NONE,             BRT_BUFFER_STORED,            13, },
    { { NULL, NULL, }, "BUMP_O",                       BRT_OBJECT,           BRT_BUMP_O,                   4,  },
    { { NULL, NULL, }, "BUMP_TVL",                     BRT_TOKEN_VALUE_LIST, BRT_BUMP_TVL,                 4,  },
    { { NULL, NULL, }, "CACHED_B",                     BRT_BOOLEAN,          BRT_CACHED_B,                 6,  },
    { { NULL, NULL, }, "CACHED_SIZE_U32",              BRT_UINT_32,          BRT_CACHED_SIZE_U32,          11, },
    { { NULL, NULL, }, "CACHED_TOTAL_U32",             BRT_UINT_32,          BRT_CACHED_TOTAL_U32,         12, },
    { { NULL, NULL, }, "CAN_SHARE_B",                  BRT_BOOLEAN,          BRT_CAN_SHARE_B,              9,  },
    { { NULL, NULL, }, "CLAMP",                        BRT_NONE,             BRT_CLAMP,                    5,  },
    { { NULL, NULL, }, "CLIP",                         BRT_NONE,             BRT_CLIP,                     4,  },
    { { NULL, NULL, }, "CLIPPING_B",                   BRT_BOOLEAN,          BRT_CLIPPING_B,               8,  },
    { { NULL, NULL, }, "CLIP_MAX_I32",                 BRT_INT_32,           BRT_CLIP_MAX_I32,             8,  },
    { { NULL, NULL, }, "CLONE",                        BRT_NONE,             BRT_CLONE,                    5,  },
    { { NULL, NULL, }, "CLUT_MAX_I32",                 BRT_INT_32,           BRT_CLUT_MAX_I32,             8,  },
    { { NULL, NULL, }, "CLUT_O",                       BRT_OBJECT,           BRT_CLUT_O,                   4,  },
    { { NULL, NULL, }, "COLOUR_B",                     BRT_BOOLEAN,          BRT_COLOUR_B,                 6,  },
    { { NULL, NULL, }, "COLOUR_BUFFER_O",              BRT_OBJECT,           BRT_COLOUR_BUFFER_O,          13, },
    { { NULL, NULL, }, "COLOUR_I32",                   BRT_INT_32,           BRT_COLOUR_I32,               6,  },
    { { NULL, NULL, }, "COLOUR_MAP_O",                 BRT_OBJECT,           BRT_COLOUR_MAP_O,             10, },
    { { NULL, NULL, }, "COLOUR_MAP_TVL",               BRT_TOKEN_VALUE_LIST, BRT_COLOUR_MAP_TVL,           10, },
    { { NULL, NULL, }, "COLOUR_RGB",                   BRT_NONE,             BRT_COLOUR_RGB,               10, },
    { { NULL, NULL, }, "COLOUR_SOURCE_T",              BRT_TOKEN,            BRT_COLOUR_SOURCE_T,          13, },
    { { NULL, NULL, }, "COLOUR_T",                     BRT_TOKEN,            BRT_COLOUR_T,                 6,  },
    { { NULL, NULL, }, "COLOUR_TL",                    BRT_TOKEN_LIST,       BRT_COLOUR_TL,                6,  },
    { { NULL, NULL, }, "COLOUR_WRITE_B",               BRT_BOOLEAN,          BRT_COLOUR_WRITE_B,           12, },
    { { NULL, NULL, }, "CONSTANT_STRING",              BRT_NONE,             BRT_CONSTANT_STRING,          15, },
    { { NULL, NULL, }, "COPY_BITS_F",                  BRT_FLOAT,            BRT_COPY_BITS_F,              9,  },
    { { NULL, NULL, }, "COPY_BITS_X",                  BRT_FIXED,            BRT_COPY_BITS_X,              9,  },
    { { NULL, NULL, }, "COPY_DESTINATION_TVL",         BRT_TOKEN_VALUE_LIST, BRT_COPY_DESTINATION_TVL,     16, },
    { { NULL, NULL, }, "COPY_F",                       BRT_FLOAT,            BRT_COPY_F,                   4,  },
    { { NULL, NULL, }, "COPY_SOURCE_TVL",              BRT_TOKEN_VALUE_LIST, BRT_COPY_SOURCE_TVL,          11, },
    { { NULL, NULL, }, "COPY_X",                       BRT_FIXED,            BRT_COPY_X,                   4,  },
    { { NULL, NULL, }, "CREATOR_CSTR",                 BRT_CONSTANT_STRING,  BRT_CREATOR_CSTR,             7,  },
    { { NULL, NULL, }, "CULL",                         BRT_NONE,             BRT_CULL,                     4,  },
    { { NULL, NULL, }, "CULL_B",                       BRT_BOOLEAN,          BRT_CULL_B,                   4,  },
    { { NULL, NULL, }, "DDI_VERSION_U32",              BRT_UINT_32,          BRT_DDI_VERSION_U32,          11, },
    { { NULL, NULL, }, "DECAL_B",                      BRT_BOOLEAN,          BRT_DECAL_B,                  5,  },
    { { NULL, NULL, }, "DEFAULT",                      BRT_NONE,             BRT_DEFAULT,                  7,  },
    { { NULL, NULL, }, "DEFAULT_BASE_I32",             BRT_INT_32,           BRT_DEFAULT_BASE_I32,         12, },
    { { NULL, NULL, }, "DEFAULT_DEVICE_STR",           BRT_STRING,           BRT_DEFAULT_DEVICE_STR,       14, },
    { { NULL, NULL, }, "DEFAULT_RANGE_I32",            BRT_INT_32,           BRT_DEFAULT_RANGE_I32,        13, },
    { { NULL, NULL, }, "DEPTH",                        BRT_NONE,             BRT_DEPTH,                    5,  },
    { { NULL, NULL, }, "DEPTH_BUFFER_O",               BRT_OBJECT,           BRT_DEPTH_BUFFER_O,           12, },
    { { NULL, NULL, }, "DEPTH_TEST_T",                 BRT_TOKEN,            BRT_DEPTH_TEST_T,             10, },
    { { NULL, NULL, }, "DEPTH_TEST_TL",                BRT_TOKEN_LIST,       BRT_DEPTH_TEST_TL,            10, },
    { { NULL, NULL, }, "DEPTH_WRITE_B",                BRT_BOOLEAN,          BRT_DEPTH_WRITE_B,            11, },
    { { NULL, NULL, }, "DESTINATION_O",                BRT_OBJECT,           BRT_DESTINATION_O,            11, },
    { { NULL, NULL, }, "DEVICE",                       BRT_NONE,             BRT_DEVICE,                   6,  },
    { { NULL, NULL, }, "DEVICE_CLUT",                  BRT_NONE,             BRT_DEVICE_CLUT,              11, },
    { { NULL, NULL, }, "DEVICE_PIXELMAP",              BRT_NONE,             BRT_DEVICE_PIXELMAP,          15, },
    { { NULL, NULL, }, "DIBSECTION",                   BRT_NONE,             BRT_DIBSECTION,               10, },
    { { NULL, NULL, }, "DIFFUSE_F",                    BRT_FLOAT,            BRT_DIFFUSE_F,                7,  },
    { { NULL, NULL, }, "DIFFUSE_X",                    BRT_FIXED,            BRT_DIFFUSE_X,                7,  },
    { { NULL, NULL, }, "DIRECT",                       BRT_NONE,             BRT_DIRECT,                   6,  },
    { { NULL, NULL, }, "DIRECT3D_HAL",                 BRT_NONE,             BRT_DIRECT3D_HAL,             12, },
    { { NULL, NULL, }, "DIRECT3D_RAMP",                BRT_NONE,             BRT_DIRECT3D_RAMP,            13, },
    { { NULL, NULL, }, "DIRECT3D_RENDER_METHOD_T",     BRT_TOKEN,            BRT_DIRECT3D_RENDER_METHOD_T, 22, },
    { { NULL, NULL, }, "DIRECT3D_RGB",                 BRT_NONE,             BRT_DIRECT3D_RGB,             12, },
    { { NULL, NULL, }, "DIRECTDRAW",                   BRT_NONE,             BRT_DIRECTDRAW,               10, },
    { { NULL, NULL, }, "DIRECTDRAW_DEVICENAME_CSTR",   BRT_CONSTANT_STRING,  BRT_DIRECTDRAW_DEVICENAME_CSTR, 21, },
    { { NULL, NULL, }, "DIRECTION_V3_F",               BRT_VECTOR3_FLOAT,    BRT_DIRECTION_V3_F,           9,  },
    { { NULL, NULL, }, "DIRECTION_V3_X",               BRT_VECTOR3_FIXED,    BRT_DIRECTION_V3_X,           9,  },
    { { NULL, NULL, }, "DIRECT_F",                     BRT_FLOAT,            BRT_DIRECT_F,                 6,  },
    { { NULL, NULL, }, "DIRECT_X",                     BRT_FIXED,            BRT_DIRECT_X,                 6,  },
    { { NULL, NULL, }, "DITHER",                       BRT_NONE,             BRT_DITHER,                   6,  },
    { { NULL, NULL, }, "DITHER_B",                     BRT_BOOLEAN,          BRT_DITHER_B,                 6,  },
    { { NULL, NULL, }, "DITHER_COLOUR_B",              BRT_BOOLEAN,          BRT_DITHER_COLOUR_B,          13, },
    { { NULL, NULL, }, "DITHER_COLOUR_T",              BRT_TOKEN,            BRT_DITHER_COLOUR_T,          13, },
    { { NULL, NULL, }, "DITHER_COLOUR_TL",             BRT_TOKEN_LIST,       BRT_DITHER_COLOUR_TL,         13, },
    { { NULL, NULL, }, "DITHER_MAP_B",                 BRT_BOOLEAN,          BRT_DITHER_MAP_B,             10, },
    { { NULL, NULL, }, "DITHER_MAP_T",                 BRT_TOKEN,            BRT_DITHER_MAP_T,             10, },
    { { NULL, NULL, }, "DITHER_MAP_TL",                BRT_TOKEN_LIST,       BRT_DITHER_MAP_TL,            10, },
    { { NULL, NULL, }, "DONT_CARE",                    BRT_NONE,             BRT_DONT_CARE,                9,  },
    { { NULL, NULL, }, "D_15",                         BRT_NONE,             BRT_D_15,                     4,  },
    { { NULL, NULL, }, "D_16",                         BRT_NONE,             BRT_D_16,                     4,  },
    { { NULL, NULL, }, "D_24",                         BRT_NONE,             BRT_D_24,                     4,  },
    { { NULL, NULL, }, "D_31",                         BRT_NONE,             BRT_D_31,                     4,  },
    { { NULL, NULL, }, "D_32",                         BRT_NONE,             BRT_D_32,                     4,  },
    { { NULL, NULL, }, "D_FP15",                       BRT_NONE,             BRT_D_FP15,                   6,  },
    { { NULL, NULL, }, "D_FP16",                       BRT_NONE,             BRT_D_FP16,                   6,  },
    { { NULL, NULL, }, "ENABLE",                       BRT_NONE,             BRT_ENABLE,                   6,  },
    { { NULL, NULL, }, "ENVIRONMENT_INFINITE",         BRT_NONE,             BRT_ENVIRONMENT_INFINITE,     20, },
    { { NULL, NULL, }, "ENVIRONMENT_LOCAL",            BRT_NONE,             BRT_ENVIRONMENT_LOCAL,        17, },
    { { NULL, NULL, }, "EQUAL",                        BRT_NONE,             BRT_EQUAL,                    5,  },
    { { NULL, NULL, }, "EVEN_ODD",                     BRT_NONE,             BRT_EVEN_ODD,                 8,  },
    { { NULL, NULL, }, "FACILITY",                     BRT_NONE,             BRT_FACILITY,                 8,  },
    { { NULL, NULL, }, "FACILITY_O",                   BRT_OBJECT,           BRT_FACILITY_O,               8,  },
    { { NULL, NULL, }, "FIXED",                        BRT_NONE,             BRT_FIXED,                    5,  },
    { { NULL, NULL, }, "FLOAT",                        BRT_NONE,             BRT_FLOAT,                    5,  },
    { { NULL, NULL, }, "FOG_MAX_F",                    BRT_FLOAT,            BRT_FOG_MAX_F,                7,  },
    { { NULL, NULL, }, "FOG_MAX_X",                    BRT_FIXED,            BRT_FOG_MAX_X,                7,  },
    { { NULL, NULL, }, "FOG_MIN_F",                    BRT_FLOAT,            BRT_FOG_MIN_F,                7,  },
    { { NULL, NULL, }, "FOG_MIN_X",                    BRT_FIXED,            BRT_FOG_MIN_X,                7,  },
    { { NULL, NULL, }, "FOG_RGB",                      BRT_COLOUR_RGB,       BRT_FOG_RGB,                  3,  },
    { { NULL, NULL, }, "FOG_T",                        BRT_TOKEN,            BRT_FOG_T,                    3,  },
    { { NULL, NULL, }, "FOG_TL",                       BRT_TOKEN_LIST,       BRT_FOG_TL,                   3,  },
    { { NULL, NULL, }, "FONT_NORMAL_O",                BRT_OBJECT,           BRT_FONT_NORMAL_O,            11, },
    { { NULL, NULL, }, "FONT_NORMAL_P",                BRT_POINTER,          BRT_FONT_NORMAL_P,            11, },
    { { NULL, NULL, }, "FORCE_FRONT_B",                BRT_BOOLEAN,          BRT_FORCE_FRONT_B,            11, },
    { { NULL, NULL, }, "FORMAT_T",                     BRT_TOKEN,            BRT_FORMAT_T,                 6,  },
    { { NULL, NULL, }, "GEOMETRY",                     BRT_NONE,             BRT_GEOMETRY,                 8,  },
    { { NULL, NULL, }, "GEOMETRY_LIGHTING",            BRT_NONE,             BRT_GEOMETRY_LIGHTING,        17, },
    { { NULL, NULL, }, "GEOMETRY_MAP",                 BRT_NONE,             BRT_GEOMETRY_MAP,             12, },
    { { NULL, NULL, }, "GEOMETRY_PRIMITIVES",          BRT_NONE,             BRT_GEOMETRY_PRIMITIVES,      19, },
    { { NULL, NULL, }, "GEOMETRY_STORED",              BRT_NONE,             BRT_GEOMETRY_STORED,          15, },
    { { NULL, NULL, }, "GEOMETRY_V1_BUCKETS",          BRT_NONE,             BRT_GEOMETRY_V1_BUCKETS,      19, },
    { { NULL, NULL, }, "GEOMETRY_V1_MODEL",            BRT_NONE,             BRT_GEOMETRY_V1_MODEL,        17, },
    { { NULL, NULL, }, "GEOMETRY_X",                   BRT_NONE,             BRT_GEOMETRY_X,               10, },
    { { NULL, NULL, }, "GEOMETRY_Y",                   BRT_NONE,             BRT_GEOMETRY_Y,               10, },
    { { NULL, NULL, }, "GEOMETRY_Z",                   BRT_NONE,             BRT_GEOMETRY_Z,               10, },
    { { NULL, NULL, }, "GREATER",                      BRT_NONE,             BRT_GREATER,                  7,  },
    { { NULL, NULL, }, "GREATER_OR_EQUAL",             BRT_NONE,             BRT_GREATER_OR_EQUAL,         16, },
    { { NULL, NULL, }, "GREEN",                        BRT_NONE,             BRT_GREEN,                    5,  },
    { { NULL, NULL, }, "HANDLE",                       BRT_NONE,             BRT_HANDLE,                   6,  },
    { { NULL, NULL, }, "HEIGHT_I32",                   BRT_INT_32,           BRT_HEIGHT_I32,               6,  },
    { { NULL, NULL, }, "HEIGHT_MAX_I32",               BRT_INT_32,           BRT_HEIGHT_MAX_I32,           10, },
    { { NULL, NULL, }, "HEIGHT_MIN_I32",               BRT_INT_32,           BRT_HEIGHT_MIN_I32,           10, },
    { { NULL, NULL, }, "HEIGHT_OPTIMUM_I32",           BRT_INT_32,           BRT_HEIGHT_OPTIMUM_I32,       14, },
    { { NULL, NULL, }, "HEIGHT_T",                     BRT_TOKEN,            BRT_HEIGHT_T,                 6,  },
    { { NULL, NULL, }, "HEIGHT_TL",                    BRT_TOKEN_LIST,       BRT_HEIGHT_TL,                6,  },
    { { NULL, NULL, }, "HIDDEN",                       BRT_NONE,             BRT_HIDDEN,                   6,  },
    { { NULL, NULL, }, "HIDDEN_BUFFER",                BRT_NONE,             BRT_HIDDEN_BUFFER,            13, },
    { { NULL, NULL, }, "HIDDEN_SURFACE",               BRT_NONE,             BRT_HIDDEN_SURFACE,           14, },
    { { NULL, NULL, }, "HOST_MEMORY_U32",              BRT_UINT_32,          BRT_HOST_MEMORY_U32,          11, },
    { { NULL, NULL, }, "IDENTIFIER_CSTR",              BRT_CONSTANT_STRING,  BRT_IDENTIFIER_CSTR,          10, },
    { { NULL, NULL, }, "INDEX",                        BRT_NONE,             BRT_INDEX,                    5,  },
    { { NULL, NULL, }, "INDEXA_4_4",                   BRT_NONE,             BRT_INDEXA_4_4,               10, },
    { { NULL, NULL, }, "INDEXA_8_8",                   BRT_NONE,             BRT_INDEXA_8_8,               10, },
    { { NULL, NULL, }, "INDEXED_B",                    BRT_BOOLEAN,          BRT_INDEXED_B,                7,  },
    { { NULL, NULL, }, "INDEX_1",                      BRT_NONE,             BRT_INDEX_1,                  7,  },
    { { NULL, NULL, }, "INDEX_2",                      BRT_NONE,             BRT_INDEX_2,                  7,  },
    { { NULL, NULL, }, "INDEX_4",                      BRT_NONE,             BRT_INDEX_4,                  7,  },
    { { NULL, NULL, }, "INDEX_8",                      BRT_NONE,             BRT_INDEX_8,                  7,  },
    { { NULL, NULL, }, "INDEX_BASE_I32",               BRT_INT_32,           BRT_INDEX_BASE_I32,           10, },
    { { NULL, NULL, }, "INDEX_BLEND_O",                BRT_OBJECT,           BRT_INDEX_BLEND_O,            11, },
    { { NULL, NULL, }, "INDEX_BLEND_TVL",              BRT_TOKEN_VALUE_LIST, BRT_INDEX_BLEND_TVL,          11, },
    { { NULL, NULL, }, "INDEX_FOG_O",                  BRT_OBJECT,           BRT_INDEX_FOG_O,              9,  },
    { { NULL, NULL, }, "INDEX_FOG_TVL",                BRT_TOKEN_VALUE_LIST, BRT_INDEX_FOG_TVL,            9,  },
    { { NULL, NULL, }, "INDEX_LIGHT_O",                BRT_OBJECT,           BRT_INDEX_LIGHT_O,            11, },
    { { NULL, NULL, }, "INDEX_LIGHT_TVL",              BRT_TOKEN_VALUE_LIST, BRT_INDEX_LIGHT_TVL,          11, },
    { { NULL, NULL, }, "INDEX_RANGE_I32",              BRT_INT_32,           BRT_INDEX_RANGE_I32,          11, },
    { { NULL, NULL, }, "INDEX_SHADE_O",                BRT_OBJECT,           BRT_INDEX_SHADE_O,            11, },
    { { NULL, NULL, }, "INDEX_SHADE_TVL",              BRT_TOKEN_VALUE_LIST, BRT_INDEX_SHADE_TVL,          11, },
    { { NULL, NULL, }, "INI_STR",                      BRT_STRING,           BRT_INI_STR,                  3,  },
    { { NULL, NULL, }, "INTEL",                        BRT_NONE,             BRT_INTEL,                    5,  },
    { { NULL, NULL, }, "INTEL_386",                    BRT_NONE,             BRT_INTEL_386,                9,  },
    { { NULL, NULL, }, "INTEL_486",                    BRT_NONE,             BRT_INTEL_486,                9,  },
    { { NULL, NULL, }, "INTEL_PENTIUM",                BRT_NONE,             BRT_INTEL_PENTIUM,            13, },
    { { NULL, NULL, }, "INTEL_PENTIUM_PRO",            BRT_NONE,             BRT_INTEL_PENTIUM_PRO,        17, },
    { { NULL, NULL, }, "INTERLACE_B",                  BRT_BOOLEAN,          BRT_INTERLACE_B,              9,  },
    { { NULL, NULL, }, "INT_16",                       BRT_NONE,             BRT_INT_16,                   6,  },
    { { NULL, NULL, }, "INT_32",                       BRT_NONE,             BRT_INT_32,                   6,  },
    { { NULL, NULL, }, "INT_8",                        BRT_NONE,             BRT_INT_8,                    5,  },
    { { NULL, NULL, }, "KEY_IS_TRANSPARENT",           BRT_NONE,             BRT_KEY_IS_TRANSPARENT,       18, },
    { { NULL, NULL, }, "LEFT_RIGHT",                   BRT_NONE,             BRT_LEFT_RIGHT,               10, },
    { { NULL, NULL, }, "LENGTH_PRESERVING",            BRT_NONE,             BRT_LENGTH_PRESERVING,        17, },
    { { NULL, NULL, }, "LESS",                         BRT_NONE,             BRT_LESS,                     4,  },
    { { NULL, NULL, }, "LESS_OR_EQUAL",                BRT_NONE,             BRT_LESS_OR_EQUAL,            13, },
    { { NULL, NULL, }, "LIGHT",                        BRT_NONE,             BRT_LIGHT,                    5,  },
    { { NULL, NULL, }, "LIGHTING_B",                   BRT_BOOLEAN,          BRT_LIGHTING_B,               8,  },
    { { NULL, NULL, }, "LIGHT_MAX_I32",                BRT_INT_32,           BRT_LIGHT_MAX_I32,            9,  },
    { { NULL, NULL, }, "LIGHT_VERTICES",               BRT_NONE,             BRT_LIGHT_VERTICES,           14, },
    { { NULL, NULL, }, "LINE",                         BRT_NONE,             BRT_LINE,                     4,  },
    { { NULL, NULL, }, "LINEAR",                       BRT_NONE,             BRT_LINEAR,                   6,  },
    { { NULL, NULL, }, "LPDIRECTDRAWSURFACE_P",        BRT_POINTER,          BRT_LPDIRECTDRAWSURFACE_P,    19, },
    { { NULL, NULL, }, "MAPPING_SOURCE_T",             BRT_TOKEN,            BRT_MAPPING_SOURCE_T,         14, },
    { { NULL, NULL, }, "MAP_ANTIALIASING_T",           BRT_TOKEN,            BRT_MAP_ANTIALIASING_T,       16, },
    { { NULL, NULL, }, "MAP_ANTIALIASING_TL",          BRT_TOKEN_LIST,       BRT_MAP_ANTIALIASING_TL,      16, },
    { { NULL, NULL, }, "MAP_HEIGHT_LIMIT_T",           BRT_TOKEN,            BRT_MAP_HEIGHT_LIMIT_T,       16, },
    { { NULL, NULL, }, "MAP_HEIGHT_LIMIT_TL",          BRT_TOKEN_LIST,       BRT_MAP_HEIGHT_LIMIT_TL,      16, },
    { { NULL, NULL, }, "MAP_INTERPOLATION_MAG_T",      BRT_TOKEN,            BRT_MAP_INTERPOLATION_MAG_T,  21, },
    { { NULL, NULL, }, "MAP_INTERPOLATION_MAG_TL",     BRT_TOKEN_LIST,       BRT_MAP_INTERPOLATION_MAG_TL, 21, },
    { { NULL, NULL, }, "MAP_INTERPOLATION_MIN_T",      BRT_TOKEN,            BRT_MAP_INTERPOLATION_MIN_T,  21, },
    { { NULL, NULL, }, "MAP_INTERPOLATION_MIN_TL",     BRT_TOKEN_LIST,       BRT_MAP_INTERPOLATION_MIN_TL, 21, },
    { { NULL, NULL, }, "MAP_INTERPOLATION_T",          BRT_TOKEN,            BRT_MAP_INTERPOLATION_T,      17, },
    { { NULL, NULL, }, "MAP_INTERPOLATION_TL",         BRT_TOKEN_LIST,       BRT_MAP_INTERPOLATION_TL,     17, },
    { { NULL, NULL, }, "MAP_MATRIX_M23_F",             BRT_MATRIX23_FLOAT,   BRT_MAP_MATRIX_M23_F,         10, },
    { { NULL, NULL, }, "MAP_MATRIX_M23_X",             BRT_MATRIX23_FIXED,   BRT_MAP_MATRIX_M23_X,         10, },
    { { NULL, NULL, }, "MAP_WIDTH_LIMIT_T",            BRT_TOKEN,            BRT_MAP_WIDTH_LIMIT_T,        15, },
    { { NULL, NULL, }, "MAP_WIDTH_LIMIT_TL",           BRT_TOKEN_LIST,       BRT_MAP_WIDTH_LIMIT_TL,       15, },
    { { NULL, NULL, }, "MATCH_TVL",                    BRT_TOKEN_VALUE_LIST, BRT_MATCH_TVL,                5,  },
    { { NULL, NULL, }, "MATRIX",                       BRT_NONE,             BRT_MATRIX,                   6,  },
    { { NULL, NULL, }, "MATRIX23_FIXED",               BRT_NONE,             BRT_MATRIX23_FIXED,           14, },
    { { NULL, NULL, }, "MATRIX23_FLOAT",               BRT_NONE,             BRT_MATRIX23_FLOAT,           14, },
    { { NULL, NULL, }, "MATRIX34_FIXED",               BRT_NONE,             BRT_MATRIX34_FIXED,           14, },
    { { NULL, NULL, }, "MATRIX34_FLOAT",               BRT_NONE,             BRT_MATRIX34_FLOAT,           14, },
    { { NULL, NULL, }, "MATRIX4_FIXED",                BRT_NONE,             BRT_MATRIX4_FIXED,            13, },
    { { NULL, NULL, }, "MATRIX4_FLOAT",                BRT_NONE,             BRT_MATRIX4_FLOAT,            13, },
    { { NULL, NULL, }, "MAX_V2_F",                     BRT_VECTOR2_FLOAT,    BRT_MAX_V2_F,                 3,  },
    { { NULL, NULL, }, "MAX_V2_I",                     BRT_VECTOR2_INTEGER,  BRT_MAX_V2_I,                 3,  },
    { { NULL, NULL, }, "MAX_V2_X",                     BRT_VECTOR2_FIXED,    BRT_MAX_V2_X,                 3,  },
    { { NULL, NULL, }, "MEMORY_MAPPED_B",              BRT_BOOLEAN,          BRT_MEMORY_MAPPED_B,          13, },
    { { NULL, NULL, }, "MIN_V2_F",                     BRT_VECTOR2_FLOAT,    BRT_MIN_V2_F,                 3,  },
    { { NULL, NULL, }, "MIN_V2_I",                     BRT_VECTOR2_INTEGER,  BRT_MIN_V2_I,                 3,  },
    { { NULL, NULL, }, "MIN_V2_X",                     BRT_VECTOR2_FIXED,    BRT_MIN_V2_X,                 3,  },
    { { NULL, NULL, }, "MIP",                          BRT_NONE,             BRT_MIP,                      3,  },
    { { NULL, NULL, }, "MIPS",                         BRT_NONE,             BRT_MIPS,                     4,  },
    { { NULL, NULL, }, "MIP_INTERPOLATION_T",          BRT_TOKEN,            BRT_MIP_INTERPOLATION_T,      17, },
    { { NULL, NULL, }, "MIP_INTERPOLATION_TL",         BRT_TOKEN_LIST,       BRT_MIP_INTERPOLATION_TL,     17, },
    { { NULL, NULL, }, "MIP_PER_PIXEL",                BRT_NONE,             BRT_MIP_PER_PIXEL,            13, },
    { { NULL, NULL, }, "MIP_PER_PRIMITIVE",            BRT_NONE,             BRT_MIP_PER_PRIMITIVE,        17, },
    { { NULL, NULL, }, "MIRROR",                       BRT_NONE,             BRT_MIRROR,                   6,  },
    { { NULL, NULL, }, "MODEL",                        BRT_NONE,             BRT_MODEL,                    5,  },
    { { NULL, NULL, }, "MODEL_TO_SCREEN_M4_F",         BRT_MATRIX4_FLOAT,    BRT_MODEL_TO_SCREEN_M4_F,     15, },
    { { NULL, NULL, }, "MODEL_TO_SCREEN_M4_X",         BRT_MATRIX4_FIXED,    BRT_MODEL_TO_SCREEN_M4_X,     15, },
    { { NULL, NULL, }, "MODEL_TO_VIEW_HINT_T",         BRT_TOKEN,            BRT_MODEL_TO_VIEW_HINT_T,     18, },
    { { NULL, NULL, }, "MODEL_TO_VIEW_M34_F",          BRT_MATRIX34_FLOAT,   BRT_MODEL_TO_VIEW_M34_F,      13, },
    { { NULL, NULL, }, "MODEL_TO_VIEW_M34_X",          BRT_MATRIX34_FIXED,   BRT_MODEL_TO_VIEW_M34_X,      13, },
    { { NULL, NULL, }, "MODE_U32",                     BRT_UINT_32,          BRT_MODE_U32,                 4,  },
    { { NULL, NULL, }, "MODULATE_B",                   BRT_BOOLEAN,          BRT_MODULATE_B,               8,  },
    { { NULL, NULL, }, "MOTOROLA",                     BRT_NONE,             BRT_MOTOROLA,                 8,  },
    { { NULL, NULL, }, "NEAREST",                      BRT_NONE,             BRT_NEAREST,                  7,  },
    { { NULL, NULL, }, "NEVER",                        BRT_NONE,             BRT_NEVER,                    5,  },
    { { NULL, NULL, }, "NONE",                         BRT_NONE,             BRT_NONE,                     4,  },
    { { NULL, NULL, }, "NOT_EQUAL",                    BRT_NONE,             BRT_NOT_EQUAL,                9,  },
    { { NULL, NULL, }, "NO_RENDER",                    BRT_NONE,             BRT_NO_RENDER,                9,  },
    { { NULL, NULL, }, "OBJECT",                       BRT_NONE,             BRT_OBJECT,                   6,  },
    { { NULL, NULL, }, "OBJECT_CONTAINER",             BRT_NONE,             BRT_OBJECT_CONTAINER,         16, },
    { { NULL, NULL, }, "OBJECT_LIST",                  BRT_NONE,             BRT_OBJECT_LIST,              11, },
    { { NULL, NULL, }, "ODD_EVEN",                     BRT_NONE,             BRT_ODD_EVEN,                 8,  },
    { { NULL, NULL, }, "OFFSCREEN",                    BRT_NONE,             BRT_OFFSCREEN,                9,  },
    { { NULL, NULL, }, "ONE_SIDED",                    BRT_NONE,             BRT_ONE_SIDED,                9,  },
    { { NULL, NULL, }, "OPACITY_F",                    BRT_FLOAT,            BRT_OPACITY_F,                7,  },
    { { NULL, NULL, }, "OPACITY_SOURCE_T",             BRT_TOKEN,            BRT_OPACITY_SOURCE_T,         14, },
    { { NULL, NULL, }, "OPACITY_X",                    BRT_FIXED,            BRT_OPACITY_X,                7,  },
    { { NULL, NULL, }, "ORIGIN_V2_I",                  BRT_VECTOR2_INTEGER,  BRT_ORIGIN_V2_I,              6,  },
    { { NULL, NULL, }, "OUTPUT",                       BRT_NONE,             BRT_OUTPUT,                   6,  },
    { { NULL, NULL, }, "OUTPUT_FACILITY",              BRT_NONE,             BRT_OUTPUT_FACILITY,          15, },
    { { NULL, NULL, }, "OUTPUT_FACILITY_O",            BRT_OBJECT,           BRT_OUTPUT_FACILITY_O,        15, },
    { { NULL, NULL, }, "PARALLEL",                     BRT_NONE,             BRT_PARALLEL,                 8,  },
    { { NULL, NULL, }, "PARTIAL",                      BRT_NONE,             BRT_PARTIAL,                  7,  },
    { { NULL, NULL, }, "PARTS_TL",                     BRT_TOKEN_LIST,       BRT_PARTS_TL,                 5,  },
    { { NULL, NULL, }, "PARTS_U32",                    BRT_UINT_32,          BRT_PARTS_U32,                5,  },
    { { NULL, NULL, }, "PERSPECTIVE",                  BRT_NONE,             BRT_PERSPECTIVE,              11, },
    { { NULL, NULL, }, "PERSPECTIVE_B",                BRT_BOOLEAN,          BRT_PERSPECTIVE_B,            11, },
    { { NULL, NULL, }, "PERSPECTIVE_T",                BRT_TOKEN,            BRT_PERSPECTIVE_T,            11, },
    { { NULL, NULL, }, "PERSPECTIVE_TL",               BRT_TOKEN_LIST,       BRT_PERSPECTIVE_TL,           11, },
    { { NULL, NULL, }, "PIXELMAP_MAX_I32",             BRT_INT_32,           BRT_PIXELMAP_MAX_I32,         12, },
    { { NULL, NULL, }, "PIXEL_BITS_I32",               BRT_INT_32,           BRT_PIXEL_BITS_I32,           10, },
    { { NULL, NULL, }, "PIXEL_CHANNELS_I32",           BRT_INT_32,           BRT_PIXEL_CHANNELS_I32,       14, },
    { { NULL, NULL, }, "PIXEL_CHANNELS_TL",            BRT_TOKEN_LIST,       BRT_PIXEL_CHANNELS_TL,        14, },
    { { NULL, NULL, }, "PIXEL_GET_F",                  BRT_FLOAT,            BRT_PIXEL_GET_F,              9,  },
    { { NULL, NULL, }, "PIXEL_GET_X",                  BRT_FIXED,            BRT_PIXEL_GET_X,              9,  },
    { { NULL, NULL, }, "PIXEL_SET_F",                  BRT_FLOAT,            BRT_PIXEL_SET_F,              9,  },
    { { NULL, NULL, }, "PIXEL_SET_X",                  BRT_FIXED,            BRT_PIXEL_SET_X,              9,  },
    { { NULL, NULL, }, "PIXEL_TYPE_U8",                BRT_UINT_8,           BRT_PIXEL_TYPE_U8,            10, },
    { { NULL, NULL, }, "PLANE",                        BRT_NONE,             BRT_PLANE,                    5,  },
    { { NULL, NULL, }, "PLANE_V4_F",                   BRT_VECTOR4_FLOAT,    BRT_PLANE_V4_F,               5,  },
    { { NULL, NULL, }, "PLANE_V4_X",                   BRT_VECTOR4_FIXED,    BRT_PLANE_V4_X,               5,  },
    { { NULL, NULL, }, "POINT",                        BRT_NONE,             BRT_POINT,                    5,  },
    { { NULL, NULL, }, "POINTER",                      BRT_NONE,             BRT_POINTER,                  7,  },
    { { NULL, NULL, }, "POINTER_LIST",                 BRT_NONE,             BRT_POINTER_LIST,             12, },
    { { NULL, NULL, }, "POSITION_V3_F",                BRT_VECTOR3_FLOAT,    BRT_POSITION_V3_F,            8,  },
    { { NULL, NULL, }, "POSITION_V3_X",                BRT_VECTOR3_FIXED,    BRT_POSITION_V3_X,            8,  },
    { { NULL, NULL, }, "POWERPC",                      BRT_NONE,             BRT_POWERPC,                  7,  },
    { { NULL, NULL, }, "POWER_OF_2",                   BRT_NONE,             BRT_POWER_OF_2,               10, },
    { { NULL, NULL, }, "PREFER_SHARE_B",               BRT_BOOLEAN,          BRT_PREFER_SHARE_B,           12, },
    { { NULL, NULL, }, "PRIMITIVE",                    BRT_NONE,             BRT_PRIMITIVE,                9,  },
    { { NULL, NULL, }, "PRIMITIVES",                   BRT_NONE,             BRT_PRIMITIVES,               10, },
    { { NULL, NULL, }, "PRIMITIVE_BLOCK_P",            BRT_POINTER,          BRT_PRIMITIVE_BLOCK_P,        15, },
    { { NULL, NULL, }, "PRIMITIVE_CUSTOM_P",           BRT_POINTER,          BRT_PRIMITIVE_CUSTOM_P,       16, },
    { { NULL, NULL, }, "PRIMITIVE_FILTER_P",           BRT_POINTER,          BRT_PRIMITIVE_FILTER_P,       16, },
    { { NULL, NULL, }, "PRIMITIVE_LIBRARY",            BRT_NONE,             BRT_PRIMITIVE_LIBRARY,        17, },
    { { NULL, NULL, }, "PRIMITIVE_LIBRARY_O",          BRT_OBJECT,           BRT_PRIMITIVE_LIBRARY_O,      17, },
    { { NULL, NULL, }, "PRIMITIVE_LIBRARY_P",          BRT_POINTER,          BRT_PRIMITIVE_LIBRARY_P,      17, },
    { { NULL, NULL, }, "PRIMITIVE_STATE",              BRT_NONE,             BRT_PRIMITIVE_STATE,          15, },
    { { NULL, NULL, }, "PRIVATE",                      BRT_NONE,             BRT_PRIVATE,                  7,  },
    { { NULL, NULL, }, "PRODUCT_CSTR",                 BRT_CONSTANT_STRING,  BRT_PRODUCT_CSTR,             7,  },
    { { NULL, NULL, }, "PRODUCT_VERSION_CSTR",         BRT_CONSTANT_STRING,  BRT_PRODUCT_VERSION_CSTR,     15, },
    { { NULL, NULL, }, "QUAD",                         BRT_NONE,             BRT_QUAD,                     4,  },
    { { NULL, NULL, }, "QUAD_STRIP",                   BRT_NONE,             BRT_QUAD_STRIP,               10, },
    { { NULL, NULL, }, "RED",                          BRT_NONE,             BRT_RED,                      3,  },
    { { NULL, NULL, }, "REGISTRY_STR",                 BRT_STRING,           BRT_REGISTRY_STR,             8,  },
    { { NULL, NULL, }, "REJECT",                       BRT_NONE,             BRT_REJECT,                   6,  },
    { { NULL, NULL, }, "RENDERER",                     BRT_NONE,             BRT_RENDERER,                 8,  },
    { { NULL, NULL, }, "RENDERER_FACILITY",            BRT_NONE,             BRT_RENDERER_FACILITY,        17, },
    { { NULL, NULL, }, "RENDERER_FACILITY_O",          BRT_OBJECT,           BRT_RENDERER_FACILITY_O,      17, },
    { { NULL, NULL, }, "RENDERER_MAX_I32",             BRT_INT_32,           BRT_RENDERER_MAX_I32,         12, },
    { { NULL, NULL, }, "RENDERER_O",                   BRT_OBJECT,           BRT_RENDERER_O,               8,  },
    { { NULL, NULL, }, "RENDERER_STATE_STORED",        BRT_NONE,             BRT_RENDERER_STATE_STORED,    21, },
    { { NULL, NULL, }, "RENDER_B",                     BRT_BOOLEAN,          BRT_RENDER_B,                 6,  },
    { { NULL, NULL, }, "RGB",                          BRT_NONE,             BRT_RGB,                      3,  },
    { { NULL, NULL, }, "RGBA_4_4_4_4",                 BRT_NONE,             BRT_RGBA_4_4_4_4,             12, },
    { { NULL, NULL, }, "RGBA_8_8_8_8",                 BRT_NONE,             BRT_RGBA_8_8_8_8,             12, },
    { { NULL, NULL, }, "RGBX_8_8_8_8",                 BRT_NONE,             BRT_RGBX_8_8_8_8,             12, },
    { { NULL, NULL, }, "RGB_2_3_2",                    BRT_NONE,             BRT_RGB_2_3_2,                9,  },
    { { NULL, NULL, }, "RGB_5_6_5",                    BRT_NONE,             BRT_RGB_5_6_5,                9,  },
    { { NULL, NULL, }, "RGB_8_8_8",                    BRT_NONE,             BRT_RGB_8_8_8,                9,  },
    { { NULL, NULL, }, "RIGHT_LEFT",                   BRT_NONE,             BRT_RIGHT_LEFT,               10, },
    { { NULL, NULL, }, "SCREEN_DOOR_O",                BRT_OBJECT,           BRT_SCREEN_DOOR_O,            11, },
    { { NULL, NULL, }, "SCREEN_DOOR_TVL",              BRT_TOKEN_VALUE_LIST, BRT_SCREEN_DOOR_TVL,          11, },
    { { NULL, NULL, }, "SHARED_B",                     BRT_BOOLEAN,          BRT_SHARED_B,                 6,  },
    { { NULL, NULL, }, "SMOOTH_B",                     BRT_BOOLEAN,          BRT_SMOOTH_B,                 6,  },
    { { NULL, NULL, }, "SPACE_T",                      BRT_TOKEN,            BRT_SPACE_T,                  5,  },
    { { NULL, NULL, }, "SPECULAR_F",                   BRT_FLOAT,            BRT_SPECULAR_F,               8,  },
    { { NULL, NULL, }, "SPECULAR_POWER_F",             BRT_FLOAT,            BRT_SPECULAR_POWER_F,         14, },
    { { NULL, NULL, }, "SPECULAR_POWER_X",             BRT_FIXED,            BRT_SPECULAR_POWER_X,         14, },
    { { NULL, NULL, }, "SPECULAR_X",                   BRT_FIXED,            BRT_SPECULAR_X,               8,  },
    { { NULL, NULL, }, "SPOT",                         BRT_NONE,             BRT_SPOT,                     4,  },
    { { NULL, NULL, }, "SPOT_INNER_F",                 BRT_FLOAT,            BRT_SPOT_INNER_F,             10, },
    { { NULL, NULL, }, "SPOT_INNER_X",                 BRT_FIXED,            BRT_SPOT_INNER_X,             10, },
    { { NULL, NULL, }, "SPOT_OUTER_F",                 BRT_FLOAT,            BRT_SPOT_OUTER_F,             10, },
    { { NULL, NULL, }, "SPOT_OUTER_X",                 BRT_FIXED,            BRT_SPOT_OUTER_X,             10, },
    { { NULL, NULL, }, "SPRITE",                       BRT_NONE,             BRT_SPRITE,                   6,  },
    { { NULL, NULL, }, "SQUARE",                       BRT_NONE,             BRT_SQUARE,                   6,  },
    { { NULL, NULL, }, "STATE_RESTORE",                BRT_NONE,             BRT_STATE_RESTORE,            13, },
    { { NULL, NULL, }, "STEREO_B",                     BRT_BOOLEAN,          BRT_STEREO_B,                 6,  },
    { { NULL, NULL, }, "STEREO_T",                     BRT_TOKEN,            BRT_STEREO_T,                 6,  },
    { { NULL, NULL, }, "STEREO_TL",                    BRT_TOKEN_LIST,       BRT_STEREO_TL,                6,  },
    { { NULL, NULL, }, "STORED_STATE_O",               BRT_OBJECT,           BRT_STORED_STATE_O,           12, },
    { { NULL, NULL, }, "STRETCHDIBITS",                BRT_NONE,             BRT_STRETCHDIBITS,            13, },
    { { NULL, NULL, }, "STRETCH_COPY_DESTINATION_TVL", BRT_TOKEN_VALUE_LIST, BRT_STRETCH_COPY_DESTINATION_TVL, 24, },
    { { NULL, NULL, }, "STRETCH_COPY_F",               BRT_FLOAT,            BRT_STRETCH_COPY_F,           12, },
    { { NULL, NULL, }, "STRETCH_COPY_SOURCE_TVL",      BRT_TOKEN_VALUE_LIST, BRT_STRETCH_COPY_SOURCE_TVL,  19, },
    { { NULL, NULL, }, "STRETCH_COPY_T",               BRT_TOKEN,            BRT_STRETCH_COPY_T,           12, },
    { { NULL, NULL, }, "STRETCH_COPY_TL",              BRT_TOKEN_LIST,       BRT_STRETCH_COPY_TL,          12, },
    { { NULL, NULL, }, "STRETCH_COPY_X",               BRT_FIXED,            BRT_STRETCH_COPY_X,           12, },
    { { NULL, NULL, }, "STRING",                       BRT_NONE,             BRT_STRING,                   6,  },
    { { NULL, NULL, }, "SUBDIVIDE",                    BRT_NONE,             BRT_SUBDIVIDE,                9,  },
    { { NULL, NULL, }, "SUBDIVIDE_TOLERANCE_I32",      BRT_INT_32,           BRT_SUBDIVIDE_TOLERANCE_I32,  19, },
    { { NULL, NULL, }, "SURFACE",                      BRT_NONE,             BRT_SURFACE,                  7,  },
    { { NULL, NULL, }, "SURFACE_B",                    BRT_BOOLEAN,          BRT_SURFACE_B,                7,  },
    { { NULL, NULL, }, "SURFACE_C_CUSTOM_P",           BRT_POINTER,          BRT_SURFACE_C_CUSTOM_P,       16, },
    { { NULL, NULL, }, "SURFACE_C_FILTER_P",           BRT_POINTER,          BRT_SURFACE_C_FILTER_P,       16, },
    { { NULL, NULL, }, "SURFACE_V_CUSTOM_P",           BRT_POINTER,          BRT_SURFACE_V_CUSTOM_P,       16, },
    { { NULL, NULL, }, "SURFACE_V_FILTER_P",           BRT_POINTER,          BRT_SURFACE_V_FILTER_P,       16, },
    { { NULL, NULL, }, "SYCHRONISE_I32",               BRT_INT_32,           BRT_SYCHRONISE_I32,           10, },
    { { NULL, NULL, }, "TEXTURE_MEMORY_U32",           BRT_UINT_32,          BRT_TEXTURE_MEMORY_U32,       14, },
    { { NULL, NULL, }, "TEXTURE_O",                    BRT_OBJECT,           BRT_TEXTURE_O,                7,  },
    { { NULL, NULL, }, "TEXTURE_TVL",                  BRT_TOKEN_VALUE_LIST, BRT_TEXTURE_TVL,              7,  },
    { { NULL, NULL, }, "TITLE_CSTR",                   BRT_CONSTANT_STRING,  BRT_TITLE_CSTR,               5,  },
    { { NULL, NULL, }, "TOKEN",                        BRT_NONE,             BRT_TOKEN,                    5,  },
    { { NULL, NULL, }, "TOKEN_LIST",                   BRT_NONE,             BRT_TOKEN_LIST,               10, },
    { { NULL, NULL, }, "TOKEN_VALUE_LIST",             BRT_NONE,             BRT_TOKEN_VALUE_LIST,         16, },
    { { NULL, NULL, }, "TOP_BOTTOM",                   BRT_NONE,             BRT_TOP_BOTTOM,               10, },
    { { NULL, NULL, }, "TRANSFORM_B",                  BRT_BOOLEAN,          BRT_TRANSFORM_B,              9,  },
    { { NULL, NULL, }, "TRIANGLE",                     BRT_NONE,             BRT_TRIANGLE,                 8,  },
    { { NULL, NULL, }, "TRI_FAN",                      BRT_NONE,             BRT_TRI_FAN,                  7,  },
    { { NULL, NULL, }, "TRI_STRIP",                    BRT_NONE,             BRT_TRI_STRIP,                9,  },
    { { NULL, NULL, }, "TWO_SIDED",                    BRT_NONE,             BRT_TWO_SIDED,                9,  },
    { { NULL, NULL, }, "TYPE_T",                       BRT_TOKEN,            BRT_TYPE_T,                   4,  },
    { { NULL, NULL, }, "UINT_16",                      BRT_NONE,             BRT_UINT_16,                  7,  },
    { { NULL, NULL, }, "UINT_32",                      BRT_NONE,             BRT_UINT_32,                  7,  },
    { { NULL, NULL, }, "UINT_8",                       BRT_NONE,             BRT_UINT_8,                   6,  },
    { { NULL, NULL, }, "UNKNOWN",                      BRT_NONE,             BRT_UNKNOWN,                  7,  },
    { { NULL, NULL, }, "USE_NO_3D_B",                  BRT_BOOLEAN,          BRT_USE_NO_3D_B,              9,  },
    { { NULL, NULL, }, "USE_NO_DEPTH_B",               BRT_BOOLEAN,          BRT_USE_NO_DEPTH_B,           12, },
    { { NULL, NULL, }, "USE_T",                        BRT_TOKEN,            BRT_USE_T,                    3,  },
    { { NULL, NULL, }, "V1BUCKETS",                    BRT_NONE,             BRT_V1BUCKETS,                9,  },
    { { NULL, NULL, }, "V1INSERT_ARG1_P",              BRT_POINTER,          BRT_V1INSERT_ARG1_P,          13, },
    { { NULL, NULL, }, "V1INSERT_ARG2_P",              BRT_POINTER,          BRT_V1INSERT_ARG2_P,          13, },
    { { NULL, NULL, }, "V1INSERT_ARG3_P",              BRT_POINTER,          BRT_V1INSERT_ARG3_P,          13, },
    { { NULL, NULL, }, "V1INSERT_FUNCTION_P",          BRT_POINTER,          BRT_V1INSERT_FUNCTION_P,      17, },
    { { NULL, NULL, }, "V1MODEL",                      BRT_NONE,             BRT_V1MODEL,                  7,  },
    { { NULL, NULL, }, "V1ORDER_TABLE_P",              BRT_POINTER,          BRT_V1ORDER_TABLE_P,          13, },
    { { NULL, NULL, }, "V1PRIMITIVE_HEAP_P",           BRT_POINTER,          BRT_V1PRIMITIVE_HEAP_P,       16, },
    { { NULL, NULL, }, "VECTOR2_FIXED",                BRT_NONE,             BRT_VECTOR2_FIXED,            13, },
    { { NULL, NULL, }, "VECTOR2_FLOAT",                BRT_NONE,             BRT_VECTOR2_FLOAT,            13, },
    { { NULL, NULL, }, "VECTOR2_INTEGER",              BRT_NONE,             BRT_VECTOR2_INTEGER,          15, },
    { { NULL, NULL, }, "VECTOR3_FIXED",                BRT_NONE,             BRT_VECTOR3_FIXED,            13, },
    { { NULL, NULL, }, "VECTOR3_FLOAT",                BRT_NONE,             BRT_VECTOR3_FLOAT,            13, },
    { { NULL, NULL, }, "VECTOR3_INTEGER",              BRT_NONE,             BRT_VECTOR3_INTEGER,          15, },
    { { NULL, NULL, }, "VECTOR4_FIXED",                BRT_NONE,             BRT_VECTOR4_FIXED,            13, },
    { { NULL, NULL, }, "VECTOR4_FLOAT",                BRT_NONE,             BRT_VECTOR4_FLOAT,            13, },
    { { NULL, NULL, }, "VECTOR4_INTEGER",              BRT_NONE,             BRT_VECTOR4_INTEGER,          15, },
    { { NULL, NULL, }, "VERSION_U32",                  BRT_UINT_32,          BRT_VERSION_U32,              7,  },
    { { NULL, NULL, }, "VERTEX_DBL",                   BRT_NONE,             BRT_VERTEX_DBL,               10, },
    { { NULL, NULL, }, "VERTEX_SGL",                   BRT_NONE,             BRT_VERTEX_SGL,               10, },
    { { NULL, NULL, }, "VERTICAL_BLANK",               BRT_NONE,             BRT_VERTICAL_BLANK,           14, },
    { { NULL, NULL, }, "VIDEO_MEMORY_U32",             BRT_UINT_32,          BRT_VIDEO_MEMORY_U32,         12, },
    { { NULL, NULL, }, "VIEW",                         BRT_NONE,             BRT_VIEW,                     4,  },
    { { NULL, NULL, }, "VIEW_TO_ENVIRONMENT_HINT_T",   BRT_TOKEN,            BRT_VIEW_TO_ENVIRONMENT_HINT_T, 24, },
    { { NULL, NULL, }, "VIEW_TO_ENVIRONMENT_M34_F",    BRT_MATRIX34_FLOAT,   BRT_VIEW_TO_ENVIRONMENT_M34_F, 19, },
    { { NULL, NULL, }, "VIEW_TO_ENVIRONMENT_M34_X",    BRT_MATRIX34_FIXED,   BRT_VIEW_TO_ENVIRONMENT_M34_X, 19, },
    { { NULL, NULL, }, "VIEW_TO_SCREEN_HINT_T",        BRT_TOKEN,            BRT_VIEW_TO_SCREEN_HINT_T,    19, },
    { { NULL, NULL, }, "VIEW_TO_SCREEN_M4_F",          BRT_MATRIX4_FLOAT,    BRT_VIEW_TO_SCREEN_M4_F,      14, },
    { { NULL, NULL, }, "VIEW_TO_SCREEN_M4_X",          BRT_MATRIX4_FIXED,    BRT_VIEW_TO_SCREEN_M4_X,      14, },
    { { NULL, NULL, }, "WIDTH_I32",                    BRT_INT_32,           BRT_WIDTH_I32,                5,  },
    { { NULL, NULL, }, "WIDTH_MAX_I32",                BRT_INT_32,           BRT_WIDTH_MAX_I32,            9,  },
    { { NULL, NULL, }, "WIDTH_MIN_I32",                BRT_INT_32,           BRT_WIDTH_MIN_I32,            9,  },
    { { NULL, NULL, }, "WIDTH_OPTIMUM_I32",            BRT_INT_32,           BRT_WIDTH_OPTIMUM_I32,        13, },
    { { NULL, NULL, }, "WIDTH_T",                      BRT_TOKEN,            BRT_WIDTH_T,                  5,  },
    { { NULL, NULL, }, "WIDTH_TL",                     BRT_TOKEN_LIST,       BRT_WIDTH_TL,                 5,  },
    { { NULL, NULL, }, "WINDOW_BUFFER_METHOD_T",       BRT_TOKEN,            BRT_WINDOW_BUFFER_METHOD_T,   20, },
    { { NULL, NULL, }, "WINDOW_FULLSCREEN_B",          BRT_BOOLEAN,          BRT_WINDOW_FULLSCREEN_B,      17, },
    { { NULL, NULL, }, "WINDOW_HANDLE_H",              BRT_HANDLE,           BRT_WINDOW_HANDLE_H,          13, },
    { { NULL, NULL, }, "WINDOW_NAME_CSTR",             BRT_CONSTANT_STRING,  BRT_WINDOW_NAME_CSTR,         11, },
    { { NULL, NULL, }, "WINDOW_PROCEDURE_P",           BRT_POINTER,          BRT_WINDOW_PROCEDURE_P,       16, },
    { { NULL, NULL, }, "WING",                         BRT_NONE,             BRT_WING,                     4,  },
    { { NULL, NULL, }, "WORD_BYTES_I32",               BRT_INT_32,           BRT_WORD_BYTES_I32,           10, },
    { { NULL, NULL, }, "WRAP",                         BRT_NONE,             BRT_WRAP,                     4,  },
    { { NULL, NULL, }, "WRITABLE_BASE_I32",            BRT_INT_32,           BRT_WRITABLE_BASE_I32,        13, },
    { { NULL, NULL, }, "WRITABLE_RANGE_I32",           BRT_INT_32,           BRT_WRITABLE_RANGE_I32,       14, },
    { { NULL, NULL, }, "XBGR_1_5_5_5",                 BRT_NONE,             BRT_XBGR_1_5_5_5,             12, },
    { { NULL, NULL, }, "XBGR_8_8_8_8",                 BRT_NONE,             BRT_XBGR_8_8_8_8,             12, },
    { { NULL, NULL, }, "XRGB_1_5_5_5",                 BRT_NONE,             BRT_XRGB_1_5_5_5,             12, },
    { { NULL, NULL, }, "XRGB_8_8_8_8",                 BRT_NONE,             BRT_XRGB_8_8_8_8,             12, },
    { { NULL, NULL, }, "XYZ_16_16_16",                 BRT_NONE,             BRT_XYZ_16_16_16,             12, },
    { { NULL, NULL, }, "XYZ_8_8_8",                    BRT_NONE,             BRT_XYZ_8_8_8,                9,  },
    { { NULL, NULL, }, "YUV_16",                       BRT_NONE,             BRT_YUV_16,                   6,  },
    { { NULL, NULL, }, "ZERO_IS_TRANSPARENT",          BRT_NONE,             BRT_ZERO_IS_TRANSPARENT,      19, },
    { { NULL, NULL, }, "ZTRANSPARENCY_B",              BRT_BOOLEAN,          BRT_ZTRANSPARENCY_B,          13, },
    { { NULL, NULL, }, "Z_WRITE_ALWAYS",               BRT_NONE,             BRT_Z_WRITE_ALWAYS,           14, },
    { { NULL, NULL, }, "Z_WRITE_CONDITIONAL",          BRT_NONE,             BRT_Z_WRITE_CONDITIONAL,      19, },
    { { NULL, NULL, }, "Z_WRITE_CONDITIONAL_NO_Z",     BRT_NONE,             BRT_Z_WRITE_CONDITIONAL_NO_Z, 24, },
    { { NULL, NULL, }, "Z_WRITE_NEVER",                BRT_NONE,             BRT_Z_WRITE_NEVER,            13, },
    { { NULL, NULL, }, "Z_WRITE_T",                    BRT_TOKEN,            BRT_Z_WRITE_T,                7,  },
    { { NULL, NULL, }, "Z_WRITE_TL",                   BRT_TOKEN_LIST,       BRT_Z_WRITE_TL,               7,  },
};
token_type tokenTypes[37] = {
    { "_M34_X", BRT_MATRIX34_FIXED,   6, },
    { "_M23_F", BRT_MATRIX23_FLOAT,   6, },
    { "_M23_X", BRT_MATRIX23_FIXED,   6, },
    { "_M34_F", BRT_MATRIX34_FLOAT,   6, },
    { "_V3_X",  BRT_VECTOR3_FIXED,    5, },
    { "_V4_X",  BRT_VECTOR4_FIXED,    5, },
    { "_M4_F",  BRT_MATRIX4_FLOAT,    5, },
    { "_M4_X",  BRT_MATRIX4_FIXED,    5, },
    { "_V3_I",  BRT_VECTOR3_INTEGER,  5, },
    { "_V4_F",  BRT_VECTOR4_FLOAT,    5, },
    { "_V2_X",  BRT_VECTOR2_FIXED,    5, },
    { "_V2_I",  BRT_VECTOR2_INTEGER,  5, },
    { "_CSTR",  BRT_CONSTANT_STRING,  5, },
    { "_V4_I",  BRT_VECTOR4_INTEGER,  5, },
    { "_V2_F",  BRT_VECTOR2_FLOAT,    5, },
    { "_V3_F",  BRT_VECTOR3_FLOAT,    5, },
    { "_STR",   BRT_STRING,           4, },
    { "_U32",   BRT_UINT_32,          4, },
    { "_RGB",   BRT_COLOUR_RGB,       4, },
    { "_I16",   BRT_INT_16,           4, },
    { "_U16",   BRT_UINT_16,          4, },
    { "_TVL",   BRT_TOKEN_VALUE_LIST, 4, },
    { "_I32",   BRT_INT_32,           4, },
    { "_OL",    BRT_OBJECT_LIST,      3, },
    { "_I8",    BRT_INT_8,            3, },
    { "_PL",    BRT_POINTER_LIST,     3, },
    { "_TL",    BRT_TOKEN_LIST,       3, },
    { "_U8",    BRT_UINT_8,           3, },
    { "_H",     BRT_HANDLE,           2, },
    { "_P",     BRT_POINTER,          2, },
    { "_X",     BRT_FIXED,            2, },
    { "_T",     BRT_TOKEN,            2, },
    { "_A",     BRT_ANGLE,            2, },
    { "_O",     BRT_OBJECT,           2, },
    { "_F",     BRT_FLOAT,            2, },
    { "_B",     BRT_BOOLEAN,          2, },
    { NULL,     BRT_NONE,             0, },
};

// IDA: void __cdecl BrTokenBegin()
void BrTokenBegin(void) {
    int i;
    LOG_TRACE("()");

    BrNewList(&fw.tokens);
    for (i = 0; i < BR_ASIZE(predefinedTokens); i++) {
        BrAddHead(&fw.tokens, &predefinedTokens[i].node);
    }
    fw.next_free_token = _BRT_LAST_BUILTIN_TOKEN + 1;
}

// IDA: br_token __cdecl BrTokenCreate(char *identifier, br_token type)
br_token BrTokenCreate(char* identifier, br_token type) {
    br_token_entry* te;
    int i;
    int l;
    LOG_TRACE("(\"%s\", %d)", identifier, type);

    l = BrStrLen(identifier);
    te = (br_token_entry*)fw.tokens.head;
    while (te->node.next != NULL) {
        if (BrStrCmp(identifier, te->identifier) == 0) {
            return te->token;
        }
        te = (br_token_entry*)te->node.next;
    }

    if (type == 0) {
        type = BRT_NONE;
        for (i = 0; i < BR_ASIZE(tokenTypes); i++) {
            if ((tokenTypes[i].length < l) && (BrStrNCmp(identifier + l - tokenTypes[i].length, tokenTypes[i].identifier, tokenTypes[i].length) == 0)) {
                type = tokenTypes[i].type;
                break;
            }
        }
    } else {
        for (i = 0; i < BR_ASIZE(tokenTypes); i++) {
            if (tokenTypes[i].type == type) {
                break;
            }
        }
        if (i >= BR_ASIZE(tokenTypes)) {
            return 0;
        }
        if (l <= tokenTypes[i].length) {
            return 0;
        }
        if (BrStrNCmp(identifier + (l - tokenTypes[i].length), tokenTypes[i].identifier, tokenTypes[i].length) != 0) {
            return 0;
        }
    }
    te = BrResAllocate(fw.res, sizeof(br_token_entry), BR_MEMORY_TOKEN);
    te->identifier = BrResStrDup(te, identifier);
    te->type = type;
    te->base_length = l - tokenTypes[i].length;
    te->token = fw.next_free_token;
    fw.next_free_token++;
    BrAddHead(&fw.tokens, &te->node);
    return te->token;
}

// IDA: char* __cdecl BrTokenIdentifier(br_token t)
char* BrTokenIdentifier(br_token t) {
    br_token_entry* te;
    LOG_TRACE("(%d)", t);

    te = (br_token_entry*)fw.tokens.head;
    while (te->node.next != NULL) {
        if (t == te->token) {
            return te->identifier;
        }
        te = (br_token_entry*)te->node.next;
    }
    return NULL;

}

// IDA: br_token __cdecl BrTokenType(br_token t)
br_token BrTokenType(br_token t) {
    br_token_entry* te;
    LOG_TRACE("(%d)", t);

    te = (br_token_entry*)fw.tokens.head;
    while (te->node.next != NULL) {
        if (t == te->token) {
            return te->type;
        }
        te = (br_token_entry*)te->node.next;
    }
    return 0;
}

// IDA: br_int_32 __cdecl BrTokenCount(char *pattern)
br_int_32 BrTokenCount(char* pattern) {
    br_token_entry* te;
    int n;
    LOG_TRACE("(\"%s\")", pattern);

    n = 0;
    if (pattern == NULL) {
        te = (br_token_entry*)fw.tokens.head;
        while (te->node.next != NULL) {
            n++;
            te = (br_token_entry*)te->node.next;
        }
    } else {
        te = (br_token_entry*)fw.tokens.head;
        while (te->node.next != NULL) {
            if (BrNamePatternMatch(pattern, te->identifier) != 0) {
                n++;
            }
            te = (br_token_entry*)te->node.next;
        }
    }
    return n;
}

// IDA: br_token __cdecl BrTokenFind(char *pattern)
br_token BrTokenFind(char* pattern) {
    br_token_entry* te;
    LOG_TRACE("(\"%s\")", pattern);

    te = (br_token_entry*)fw.tokens.head;
    while (te->node.next != NULL) {
        if (BrNamePatternMatch(pattern, te->identifier) != 0) {
            return te->token;
        }
        te = (br_token_entry*)te->node.next;
    }
    return 0;
}

// IDA: br_int_32 __cdecl BrTokenFindMany(char *pattern, br_token *tokens, br_int_32 max_tokens)
br_int_32 BrTokenFindMany(char* pattern, br_token* tokens, br_int_32 max_tokens) {
    br_token_entry* te;
    int n;
    LOG_TRACE("(\"%s\", %p, %d)", pattern, tokens, max_tokens);

    n = 0;
    te = (br_token_entry*)fw.tokens.head;
    while (te->node.next != NULL) {
        if (BrNamePatternMatch(pattern, te->identifier) != 0) {
            *tokens = te->token;
            tokens++;
            n++;
        }
        te = (br_token_entry*)te->node.next;
    }
    return n;
}

// IDA: br_token __usercall BrTokenFindType@<EAX>(br_token *ptype@<EAX>, char *base@<EDX>, br_token *types@<EBX>, br_int_32 ntypes@<ECX>)
br_token BrTokenFindType(br_token* ptype, char* base, br_token* types, br_int_32 ntypes) {
    br_token_entry* te;
    int l;
    int t;
    LOG_TRACE("(%p, \"%s\", %p, %d)", ptype, base, types, ntypes);

    l = BrStrLen(base);
    te = (br_token_entry*)fw.tokens.head;
    while (te->node.next != NULL) {
        if ((l == te->base_length) && BrStrNICmp(te->identifier, base, te->base_length) == 0) {
            for (t = 0; t < ntypes; t++) {
                if (te->type == types[t]) {
                    if (ptype != NULL) {
                        *ptype = te->type;
                    }
                    return te->token;
                }
            }
        }
        te = (br_token_entry*)te->node.next;
    }
    return 0;
}
