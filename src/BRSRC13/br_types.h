#ifndef BR_TYPES_H
#define BR_TYPES_H

#include <setjmp.h>
#include <stdint.h>

typedef int32_t br_int_32;
typedef uint32_t br_uint_32;
typedef int16_t br_int_16;
typedef uint16_t br_uint_16;
typedef signed char br_int_8;
typedef unsigned char br_uint_8;
typedef uint32_t br_size_t;
typedef int32_t br_boolean;
typedef float br_float;
typedef br_uint_32 br_error;
typedef int32_t br_fixed_ls;
typedef int16_t br_fixed_lsf;
typedef uint32_t br_fixed_lu;
typedef uint16_t br_fixed_luf;
typedef int16_t br_fixed_ss;
typedef char br_fixed_ssf;
typedef uint16_t br_fixed_su;
typedef unsigned char br_fixed_suf;
typedef float br_scalar;
typedef float br_fraction;
typedef float br_ufraction;
typedef br_fixed_ls br_scalar_x;
typedef br_fixed_lsf br_fraction_x;
typedef br_fixed_luf br_ufraction_x;
typedef float br_scalar_f;
typedef float br_fraction_f;
typedef float br_ufraction_f;
typedef uint32_t br_colour;
typedef struct br_bounds3 br_bounds;
typedef struct br_bounds2_i br_bounds2i;
typedef struct br_bounds3_i br_bounds3i;
typedef struct br_bounds4_i br_bounds4i;
typedef br_fixed_luf br_angle;
typedef struct br_value_tag br_value;
typedef struct br_associative_array_tag br_associative_array;
typedef struct br_tri_strip br_tri_fan;
typedef struct br_tri_strip br_quad_strip;

typedef enum br_filesystem_attributes {
    BR_FS_ATTR_READABLE = 1,
    BR_FS_ATTR_WRITEABLE = 2,
    BR_FS_ATTR_HAS_TEXT = 4,
    BR_FS_ATTR_HAS_BINARY = 8,
    BR_FS_ATTR_HAS_ADVANCE = 16
} br_filesystem_attributes;

typedef enum br_filesystem_identify {
    BR_FS_MODE_BINARY = 0,
    BR_FS_MODE_TEXT = 1,
    BR_FS_MODE_UNKNOWN = 2
} br_filesystem_identify;

typedef enum br_memory_classes {
    BR_MEMORY_SCRATCH = 1,
    BR_MEMORY_PIXELMAP = 2,
    BR_MEMORY_PIXELS = 3,
    BR_MEMORY_VERTICES = 4,
    BR_MEMORY_FACES = 5,
    BR_MEMORY_GROUPS = 6,
    BR_MEMORY_MODEL = 7,
    BR_MEMORY_MATERIAL = 8,
    BR_MEMORY_MATERIAL_INDEX = 9,
    BR_MEMORY_ACTOR = 10,
    BR_MEMORY_PREPARED_VERTICES = 11,
    BR_MEMORY_PREPARED_FACES = 12,
    BR_MEMORY_LIGHT = 13,
    BR_MEMORY_CAMERA = 14,
    BR_MEMORY_BOUNDS = 15,
    BR_MEMORY_CLIP_PLANE = 16,
    BR_MEMORY_STRING = 17,
    BR_MEMORY_REGISTRY = 18,
    BR_MEMORY_TRANSFORM = 19,
    BR_MEMORY_RESOURCE_CLASS = 20,
    BR_MEMORY_FILE = 21,
    BR_MEMORY_ANCHOR = 22,
    BR_MEMORY_POOL = 23,
    BR_MEMORY_RENDER_MATERIAL = 24,
    BR_MEMORY_DATAFILE = 25,
    BR_MEMORY_IMAGE = 26,
    BR_MEMORY_IMAGE_ARENA = 27,
    BR_MEMORY_IMAGE_SECTIONS = 28,
    BR_MEMORY_IMAGE_NAMES = 29,
    BR_MEMORY_EXCEPTION_HANDLER = 30,
    BR_MEMORY_RENDER_DATA = 31,
    BR_MEMORY_TOKEN = 32,
    BR_MEMORY_TOKEN_MAP = 33,
    BR_MEMORY_OBJECT = 34,
    BR_MEMORY_OBJECT_DATA = 35,
    BR_MEMORY_DRIVER = 36,
    BR_MEMORY_LEXER = 37,
    BR_MEMORY_OBJECT_LIST = 38,
    BR_MEMORY_OBJECT_LIST_ENTRY = 39,
    BR_MEMORY_ENABLED_ACTORS = 40,
    BR_MEMORY_FMT_RESULTS = 41,
    BR_MEMORY_PREPARED_MODEL = 42,
    BR_MEMORY_ORDER_TABLE = 43,
    BR_MEMORY_TOKEN_VALUE = 44,
    BR_MEMORY_TOKEN_TEMPLATE = 45,
    BR_MEMORY_FREE = 127,
    BR_MEMORY_APPLICATION = 128,
    BR_MEMORY_MAX = 256
} br_memory_classes;

typedef enum br_pixelmap_allocate_flags {
    BR_PMAF_NORMAL = 0,
    BR_PMAF_INVERTED = 1,
    BR_PMAF_NO_PIXELS = 2
} br_pixelmap_allocate_flags;

typedef enum br_pixelmap_channel_mask {
    BR_PMCHAN_INDEX = 1,
    BR_PMCHAN_RGB = 2,
    BR_PMCHAN_DEPTH = 4,
    BR_PMCHAN_ALPHA = 8,
    BR_PMCHAN_YUV = 16,
    BR_PMCHAN_VECTOR = 32
} br_pixelmap_channel_mask;

typedef enum br_pmmatch_type {
    BR_PMMATCH_OFFSCREEN = 0,
    BR_PMMATCH_DEPTH_16 = 1,
    BR_PMMATCH_DEPTH = 2,
    BR_PMMATCH_HIDDEN = 3,
    BR_PMMATCH_HIDDEN_BUFFER = 4,
    BR_PMMATCH_NO_RENDER = 5,
    BR_PMMATCH_DEPTH_8 = 6,
    BR_PMMATCH_DEPTH_32 = 7,
    BR_PMMATCH_DEPTH_15 = 8,
    BR_PMMATCH_DEPTH_31 = 9,
    BR_PMMATCH_DEPTH_FP15 = 10,
    BR_PMMATCH_DEPTH_FP16 = 11,
    BR_PMMATCH_MAX = 12
} br_pmmatch_type;

typedef enum br_pixelmap_copy_function {
    BR_PMCOPY_NORMAL = 0,
    BR_PMCOPY_SRC_KEYED = 1,
    BR_PMCOPY_DST_KEYED = 2
} br_pixelmap_copy_function;

typedef enum br_clip_result {
    BR_CLIP_REJECT = 0,
    BR_CLIP_PARTIAL = 1,
    BR_CLIP_ACCEPT = 2
} br_clip_result;

typedef enum br_token {
    BRT_ABGR_1_5_5_5 = 319,
    BRT_ABGR_4_4_4_4 = 335,
    BRT_ABGR_8_8_8_8 = 331,
    BRT_ACCEPT = 276,
    BRT_AINDEX_4_4 = 345,
    BRT_AINDEX_8_8 = 344,
    BRT_ALPHA = 85,
    BRT_ALWAYS = 163,
    BRT_AMBIENT_F = 193,
    BRT_AMBIENT_X = 192,
    BRT_ANGLE = 13,
    BRT_ANTIALIASING_T = 298,
    BRT_ANTIALIASING_TL = 299,
    BRT_ARGB_1_5_5_5 = 318,
    BRT_ARGB_4_4_4_4 = 334,
    BRT_ARGB_8_8_8_8 = 330,
    BRT_ATTENUATION_C_F = 222,
    BRT_ATTENUATION_C_X = 221,
    BRT_ATTENUATION_L_F = 220,
    BRT_ATTENUATION_L_X = 219,
    BRT_ATTENUATION_Q_F = 224,
    BRT_ATTENUATION_Q_X = 223,
    BRT_A_4 = 450,
    BRT_A_8 = 341,
    BRT_BEST_QUALITY = 380,
    BRT_BEST_SPEED = 291,
    BRT_BGRA_4_4_4_4 = 336,
    BRT_BGRA_8_8_8_8 = 332,
    BRT_BGRX_8_8_8_8 = 328,
    BRT_BGR_2_3_2 = 323,
    BRT_BGR_5_6_5 = 321,
    BRT_BGR_8_8_8 = 325,
    BRT_BLENDER_PROGRAM_P = 458,
    BRT_BLEND_B = 133,
    BRT_BLEND_CONSTANT = 445,
    BRT_BLEND_DIMMED = 444,
    BRT_BLEND_MODE_T = 440,
    BRT_BLEND_MODE_TL = 441,
    BRT_BLEND_STANDARD = 442,
    BRT_BLEND_SUMMED = 443,
    BRT_BLUE = 83,
    BRT_BOOLEAN = 2,
    BRT_BOTTOM_TOP = 67,
    BRT_BOUNDS = 122,
    BRT_BOUNDS_B = 255,
    BRT_BRENDER_DEVICES_STR = 422,
    BRT_BRENDER_DRIVERS_STR = 420,
    BRT_BRENDER_PATH_STR = 419,
    BRT_BRENDER_VERSION_U32 = 356,
    BRT_BUCKET_SORT = 257,
    BRT_BUFFER_STORED = 45,
    BRT_BUMP_O = 170,
    BRT_BUMP_TVL = 401,
    BRT_CACHED_B = 286,
    BRT_CACHED_SIZE_U32 = 287,
    BRT_CACHED_TOTAL_U32 = 288,
    BRT_CAN_SHARE_B = 110,
    BRT_CLAMP = 308,
    BRT_CLIP = 121,
    BRT_CLIPPING_B = 254,
    BRT_CLIP_MAX_I32 = 105,
    BRT_CLONE = 102,
    BRT_CLUT_MAX_I32 = 90,
    BRT_CLUT_O = 68,
    BRT_COLOUR_B = 135,
    BRT_COLOUR_BUFFER_O = 249,
    BRT_COLOUR_I32 = 189,
    BRT_COLOUR_MAP_O = 165,
    BRT_COLOUR_MAP_TVL = 396,
    BRT_COLOUR_RGB = 14,
    BRT_COLOUR_SOURCE_T = 180,
    BRT_COLOUR_T = 136,
    BRT_COLOUR_TL = 297,
    BRT_COLOUR_WRITE_B = 132,
    BRT_CONSTANT_STRING = 32,
    BRT_COPY_BITS_F = 371,
    BRT_COPY_BITS_X = 370,
    BRT_COPY_DESTINATION_TVL = 361,
    BRT_COPY_F = 363,
    BRT_COPY_SOURCE_TVL = 360,
    BRT_COPY_X = 362,
    BRT_CREATOR_CSTR = 56,
    BRT_CULL = 116,
    BRT_CULL_B = 251,
    BRT_DDI_VERSION_U32 = 357,
    BRT_DECAL_B = 128,
    BRT_DEFAULT = 138,
    BRT_DEFAULT_BASE_I32 = 282,
    BRT_DEFAULT_DEVICE_STR = 421,
    BRT_DEFAULT_RANGE_I32 = 283,
    BRT_DEPTH = 84,
    BRT_DEPTH_BUFFER_O = 250,
    BRT_DEPTH_TEST_T = 155,
    BRT_DEPTH_TEST_TL = 304,
    BRT_DEPTH_WRITE_B = 131,
    BRT_DESTINATION_O = 114,
    BRT_DEVICE = 37,
    BRT_DEVICE_CLUT = 41,
    BRT_DEVICE_PIXELMAP = 40,
    BRT_DIBSECTION = 410,
    BRT_DIFFUSE_F = 195,
    BRT_DIFFUSE_X = 194,
    BRT_DIRECT = 208,
    BRT_DIRECT3D_HAL = 416,
    BRT_DIRECT3D_RAMP = 414,
    BRT_DIRECT3D_RENDER_METHOD_T = 413,
    BRT_DIRECT3D_RGB = 415,
    BRT_DIRECTDRAW = 411,
    BRT_DIRECTDRAW_DEVICENAME_CSTR = 412,
    BRT_DIRECTION_V3_F = 213,
    BRT_DIRECTION_V3_X = 214,
    BRT_DIRECT_F = 378,
    BRT_DIRECT_X = 377,
    BRT_DITHER = 431,
    BRT_DITHER_B = 457,
    BRT_DITHER_COLOUR_B = 129,
    BRT_DITHER_COLOUR_T = 293,
    BRT_DITHER_COLOUR_TL = 294,
    BRT_DITHER_MAP_B = 130,
    BRT_DITHER_MAP_T = 295,
    BRT_DITHER_MAP_TL = 296,
    BRT_DONT_CARE = 246,
    BRT_D_15 = 446,
    BRT_D_16 = 338,
    BRT_D_24 = 339,
    BRT_D_31 = 447,
    BRT_D_32 = 340,
    BRT_D_FP15 = 448,
    BRT_D_FP16 = 449,
    BRT_ENABLE = 119,
    BRT_ENVIRONMENT_INFINITE = 184,
    BRT_ENVIRONMENT_LOCAL = 183,
    BRT_EQUAL = 158,
    BRT_EVEN_ODD = 63,
    BRT_FACILITY = 38,
    BRT_FACILITY_O = 281,
    BRT_FIXED = 11,
    BRT_FLOAT = 12,
    BRT_FOG_MAX_F = 153,
    BRT_FOG_MAX_X = 154,
    BRT_FOG_MIN_F = 151,
    BRT_FOG_MIN_X = 152,
    BRT_FOG_RGB = 150,
    BRT_FOG_T = 149,
    BRT_FOG_TL = 303,
    BRT_FONT_NORMAL_O = 98,
    BRT_FONT_NORMAL_P = 97,
    BRT_FORCE_FRONT_B = 179,
    BRT_FORMAT_T = 113,
    BRT_GEOMETRY = 46,
    BRT_GEOMETRY_LIGHTING = 51,
    BRT_GEOMETRY_MAP = 185,
    BRT_GEOMETRY_PRIMITIVES = 48,
    BRT_GEOMETRY_STORED = 47,
    BRT_GEOMETRY_V1_BUCKETS = 50,
    BRT_GEOMETRY_V1_MODEL = 49,
    BRT_GEOMETRY_X = 186,
    BRT_GEOMETRY_Y = 187,
    BRT_GEOMETRY_Z = 188,
    BRT_GREATER = 156,
    BRT_GREATER_OR_EQUAL = 157,
    BRT_GREEN = 82,
    BRT_HANDLE = 354,
    BRT_HEIGHT_I32 = 72,
    BRT_HEIGHT_MAX_I32 = 74,
    BRT_HEIGHT_MIN_I32 = 73,
    BRT_HEIGHT_OPTIMUM_I32 = 392,
    BRT_HEIGHT_T = 389,
    BRT_HEIGHT_TL = 390,
    BRT_HIDDEN = 100,
    BRT_HIDDEN_BUFFER = 278,
    BRT_HIDDEN_SURFACE = 123,
    BRT_HOST_MEMORY_U32 = 93,
    BRT_IDENTIFIER_CSTR = 54,
    BRT_INDEX = 80,
    BRT_INDEXA_4_4 = 343,
    BRT_INDEXA_8_8 = 342,
    BRT_INDEXED_B = 86,
    BRT_INDEX_1 = 312,
    BRT_INDEX_2 = 313,
    BRT_INDEX_4 = 314,
    BRT_INDEX_8 = 315,
    BRT_INDEX_BASE_I32 = 139,
    BRT_INDEX_BLEND_O = 167,
    BRT_INDEX_BLEND_TVL = 398,
    BRT_INDEX_FOG_O = 394,
    BRT_INDEX_FOG_TVL = 402,
    BRT_INDEX_LIGHT_O = 169,
    BRT_INDEX_LIGHT_TVL = 400,
    BRT_INDEX_RANGE_I32 = 140,
    BRT_INDEX_SHADE_O = 166,
    BRT_INDEX_SHADE_TVL = 397,
    BRT_INI_STR = 418,
    BRT_INTEL = 423,
    BRT_INTEL_386 = 427,
    BRT_INTEL_486 = 428,
    BRT_INTEL_PENTIUM = 429,
    BRT_INTEL_PENTIUM_PRO = 430,
    BRT_INTERLACE_B = 456,
    BRT_INT_16 = 7,
    BRT_INT_32 = 9,
    BRT_INT_8 = 5,
    BRT_KEY_IS_TRANSPARENT = 353,
    BRT_LEFT_RIGHT = 64,
    BRT_LENGTH_PRESERVING = 237,
    BRT_LESS = 161,
    BRT_LESS_OR_EQUAL = 160,
    BRT_LIGHT = 120,
    BRT_LIGHTING_B = 178,
    BRT_LIGHT_MAX_I32 = 104,
    BRT_LIGHT_VERTICES = 273,
    BRT_LINE = 266,
    BRT_LINEAR = 147,
    BRT_LPDIRECTDRAWSURFACE_P = 432,
    BRT_MAPPING_SOURCE_T = 182,
    BRT_MAP_ANTIALIASING_T = 141,
    BRT_MAP_ANTIALIASING_TL = 300,
    BRT_MAP_HEIGHT_LIMIT_T = 310,
    BRT_MAP_HEIGHT_LIMIT_TL = 311,
    BRT_MAP_INTERPOLATION_MAG_T = 383,
    BRT_MAP_INTERPOLATION_MAG_TL = 384,
    BRT_MAP_INTERPOLATION_MIN_T = 385,
    BRT_MAP_INTERPOLATION_MIN_TL = 386,
    BRT_MAP_INTERPOLATION_T = 145,
    BRT_MAP_INTERPOLATION_TL = 301,
    BRT_MAP_MATRIX_M23_F = 200,
    BRT_MAP_MATRIX_M23_X = 201,
    BRT_MAP_WIDTH_LIMIT_T = 305,
    BRT_MAP_WIDTH_LIMIT_TL = 306,
    BRT_MATCH_TVL = 359,
    BRT_MATRIX = 118,
    BRT_MATRIX23_FIXED = 25,
    BRT_MATRIX23_FLOAT = 28,
    BRT_MATRIX34_FIXED = 26,
    BRT_MATRIX34_FLOAT = 29,
    BRT_MATRIX4_FIXED = 27,
    BRT_MATRIX4_FLOAT = 30,
    BRT_MAX_V2_F = 233,
    BRT_MAX_V2_I = 231,
    BRT_MAX_V2_X = 232,
    BRT_MEMORY_MAPPED_B = 87,
    BRT_MIN_V2_F = 230,
    BRT_MIN_V2_I = 228,
    BRT_MIN_V2_X = 229,
    BRT_MIP = 142,
    BRT_MIPS = 425,
    BRT_MIP_INTERPOLATION_T = 148,
    BRT_MIP_INTERPOLATION_TL = 302,
    BRT_MIP_PER_PIXEL = 143,
    BRT_MIP_PER_PRIMITIVE = 144,
    BRT_MIRROR = 309,
    BRT_MODEL = 177,
    BRT_MODEL_TO_SCREEN_M4_F = 247,
    BRT_MODEL_TO_SCREEN_M4_X = 248,
    BRT_MODEL_TO_VIEW_HINT_T = 236,
    BRT_MODEL_TO_VIEW_M34_F = 234,
    BRT_MODEL_TO_VIEW_M34_X = 235,
    BRT_MODE_U32 = 94,
    BRT_MODULATE_B = 134,
    BRT_MOTOROLA = 426,
    BRT_NEAREST = 146,
    BRT_NEVER = 162,
    BRT_NONE = 1,
    BRT_NOT_EQUAL = 159,
    BRT_NO_RENDER = 379,
    BRT_OBJECT = 15,
    BRT_OBJECT_CONTAINER = 36,
    BRT_OBJECT_LIST = 33,
    BRT_ODD_EVEN = 62,
    BRT_OFFSCREEN = 101,
    BRT_ONE_SIDED = 173,
    BRT_OPACITY_F = 191,
    BRT_OPACITY_SOURCE_T = 181,
    BRT_OPACITY_X = 190,
    BRT_ORIGIN_V2_I = 96,
    BRT_OUTPUT = 125,
    BRT_OUTPUT_FACILITY = 39,
    BRT_OUTPUT_FACILITY_O = 95,
    BRT_PARALLEL = 242,
    BRT_PARTIAL = 275,
    BRT_PARTS_TL = 107,
    BRT_PARTS_U32 = 112,
    BRT_PERSPECTIVE = 241,
    BRT_PERSPECTIVE_B = 127,
    BRT_PERSPECTIVE_T = 289,
    BRT_PERSPECTIVE_TL = 290,
    BRT_PIXELMAP_MAX_I32 = 89,
    BRT_PIXEL_BITS_I32 = 76,
    BRT_PIXEL_CHANNELS_I32 = 78,
    BRT_PIXEL_CHANNELS_TL = 79,
    BRT_PIXEL_GET_F = 374,
    BRT_PIXEL_GET_X = 373,
    BRT_PIXEL_SET_F = 376,
    BRT_PIXEL_SET_X = 375,
    BRT_PIXEL_TYPE_U8 = 75,
    BRT_PLANE = 225,
    BRT_PLANE_V4_F = 226,
    BRT_PLANE_V4_X = 227,
    BRT_POINT = 209,
    BRT_POINTER = 3,
    BRT_POINTER_LIST = 35,
    BRT_POSITION_V3_F = 211,
    BRT_POSITION_V3_X = 212,
    BRT_POWERPC = 424,
    BRT_POWER_OF_2 = 351,
    BRT_PREFER_SHARE_B = 111,
    BRT_PRIMITIVE = 124,
    BRT_PRIMITIVES = 270,
    BRT_PRIMITIVE_BLOCK_P = 393,
    BRT_PRIMITIVE_CUSTOM_P = 206,
    BRT_PRIMITIVE_FILTER_P = 207,
    BRT_PRIMITIVE_LIBRARY = 52,
    BRT_PRIMITIVE_LIBRARY_O = 88,
    BRT_PRIMITIVE_LIBRARY_P = 115,
    BRT_PRIMITIVE_STATE = 53,
    BRT_PRIVATE = 349,
    BRT_PRODUCT_CSTR = 57,
    BRT_PRODUCT_VERSION_CSTR = 58,
    BRT_QUAD = 268,
    BRT_QUAD_STRIP = 455,
    BRT_RED = 81,
    BRT_REGISTRY_STR = 417,
    BRT_REJECT = 277,
    BRT_RENDERER = 43,
    BRT_RENDERER_FACILITY = 42,
    BRT_RENDERER_FACILITY_O = 108,
    BRT_RENDERER_MAX_I32 = 106,
    BRT_RENDERER_O = 103,
    BRT_RENDERER_STATE_STORED = 44,
    BRT_RENDER_B = 256,
    BRT_RGB = 137,
    BRT_RGBA_4_4_4_4 = 337,
    BRT_RGBA_8_8_8_8 = 333,
    BRT_RGBX_8_8_8_8 = 329,
    BRT_RGB_2_3_2 = 322,
    BRT_RGB_5_6_5 = 320,
    BRT_RGB_8_8_8 = 324,
    BRT_RIGHT_LEFT = 65,
    BRT_SCREEN_DOOR_O = 168,
    BRT_SCREEN_DOOR_TVL = 399,
    BRT_SHARED_B = 109,
    BRT_SMOOTH_B = 126,
    BRT_SPACE_T = 175,
    BRT_SPECULAR_F = 197,
    BRT_SPECULAR_POWER_F = 199,
    BRT_SPECULAR_POWER_X = 198,
    BRT_SPECULAR_X = 196,
    BRT_SPOT = 210,
    BRT_SPOT_INNER_F = 218,
    BRT_SPOT_INNER_X = 217,
    BRT_SPOT_OUTER_F = 216,
    BRT_SPOT_OUTER_X = 215,
    BRT_SPRITE = 265,
    BRT_SQUARE = 350,
    BRT_STATE_RESTORE = 269,
    BRT_STEREO_B = 59,
    BRT_STEREO_T = 60,
    BRT_STEREO_TL = 61,
    BRT_STORED_STATE_O = 264,
    BRT_STRETCHDIBITS = 409,
    BRT_STRETCH_COPY_DESTINATION_TVL = 365,
    BRT_STRETCH_COPY_F = 369,
    BRT_STRETCH_COPY_SOURCE_TVL = 364,
    BRT_STRETCH_COPY_T = 366,
    BRT_STRETCH_COPY_TL = 367,
    BRT_STRETCH_COPY_X = 368,
    BRT_STRING = 31,
    BRT_SUBDIVIDE = 381,
    BRT_SUBDIVIDE_TOLERANCE_I32 = 382,
    BRT_SURFACE = 117,
    BRT_SURFACE_B = 253,
    BRT_SURFACE_C_CUSTOM_P = 202,
    BRT_SURFACE_C_FILTER_P = 203,
    BRT_SURFACE_V_CUSTOM_P = 204,
    BRT_SURFACE_V_FILTER_P = 205,
    BRT_SYCHRONISE_I32 = 372,
    BRT_TEXTURE_MEMORY_U32 = 92,
    BRT_TEXTURE_O = 164,
    BRT_TEXTURE_TVL = 395,
    BRT_TITLE_CSTR = 358,
    BRT_TOKEN = 4,
    BRT_TOKEN_LIST = 34,
    BRT_TOKEN_VALUE_LIST = 355,
    BRT_TOP_BOTTOM = 66,
    BRT_TRANSFORM_B = 252,
    BRT_TRIANGLE = 267,
    BRT_TRI_FAN = 454,
    BRT_TRI_STRIP = 453,
    BRT_TWO_SIDED = 174,
    BRT_TYPE_T = 172,
    BRT_UINT_16 = 8,
    BRT_UINT_32 = 10,
    BRT_UINT_8 = 6,
    BRT_UNKNOWN = 171,
    BRT_USE_NO_3D_B = 279,
    BRT_USE_NO_DEPTH_B = 280,
    BRT_USE_T = 99,
    BRT_V1BUCKETS = 272,
    BRT_V1INSERT_ARG1_P = 261,
    BRT_V1INSERT_ARG2_P = 262,
    BRT_V1INSERT_ARG3_P = 263,
    BRT_V1INSERT_FUNCTION_P = 260,
    BRT_V1MODEL = 271,
    BRT_V1ORDER_TABLE_P = 258,
    BRT_V1PRIMITIVE_HEAP_P = 259,
    BRT_VECTOR2_FIXED = 19,
    BRT_VECTOR2_FLOAT = 22,
    BRT_VECTOR2_INTEGER = 16,
    BRT_VECTOR3_FIXED = 20,
    BRT_VECTOR3_FLOAT = 23,
    BRT_VECTOR3_INTEGER = 17,
    BRT_VECTOR4_FIXED = 21,
    BRT_VECTOR4_FLOAT = 24,
    BRT_VECTOR4_INTEGER = 18,
    BRT_VERSION_U32 = 55,
    BRT_VERTEX_DBL = 452,
    BRT_VERTEX_SGL = 451,
    BRT_VERTICAL_BLANK = 274,
    BRT_VIDEO_MEMORY_U32 = 91,
    BRT_VIEW = 176,
    BRT_VIEW_TO_ENVIRONMENT_HINT_T = 245,
    BRT_VIEW_TO_ENVIRONMENT_M34_F = 243,
    BRT_VIEW_TO_ENVIRONMENT_M34_X = 244,
    BRT_VIEW_TO_SCREEN_HINT_T = 240,
    BRT_VIEW_TO_SCREEN_M4_F = 238,
    BRT_VIEW_TO_SCREEN_M4_X = 239,
    BRT_WIDTH_I32 = 69,
    BRT_WIDTH_MAX_I32 = 71,
    BRT_WIDTH_MIN_I32 = 70,
    BRT_WIDTH_OPTIMUM_I32 = 391,
    BRT_WIDTH_T = 387,
    BRT_WIDTH_TL = 388,
    BRT_WINDOW_BUFFER_METHOD_T = 407,
    BRT_WINDOW_FULLSCREEN_B = 406,
    BRT_WINDOW_HANDLE_H = 405,
    BRT_WINDOW_NAME_CSTR = 403,
    BRT_WINDOW_PROCEDURE_P = 404,
    BRT_WING = 408,
    BRT_WORD_BYTES_I32 = 77,
    BRT_WRAP = 307,
    BRT_WRITABLE_BASE_I32 = 284,
    BRT_WRITABLE_RANGE_I32 = 285,
    BRT_XBGR_1_5_5_5 = 317,
    BRT_XBGR_8_8_8_8 = 327,
    BRT_XRGB_1_5_5_5 = 316,
    BRT_XRGB_8_8_8_8 = 326,
    BRT_XYZ_16_16_16 = 347,
    BRT_XYZ_8_8_8 = 346,
    BRT_YUV_16 = 348,
    BRT_ZERO_IS_TRANSPARENT = 352,
    BRT_ZTRANSPARENCY_B = 439,
    BRT_Z_WRITE_ALWAYS = 436,
    BRT_Z_WRITE_CONDITIONAL = 438,
    BRT_Z_WRITE_CONDITIONAL_NO_Z = 437,
    BRT_Z_WRITE_NEVER = 435,
    BRT_Z_WRITE_T = 433,
    BRT_Z_WRITE_TL = 434,
    _BRT_MAX = 134217727
} br_token;

typedef enum br_apply_map_types {
    BR_APPLYMAP_PLANE = 0,
    BR_APPLYMAP_SPHERE = 1,
    BR_APPLYMAP_CYLINDER = 2,
    BR_APPLYMAP_DISC = 3,
    BR_APPLYMAP_NONE = 4
} br_apply_map_types;

typedef enum br_fitmap_axis {
    BR_FITMAP_PLUS_X = 0,
    BR_FITMAP_PLUS_Y = 1,
    BR_FITMAP_PLUS_Z = 2,
    BR_FITMAP_MINUS_X = 3,
    BR_FITMAP_MINUS_Y = 4,
    BR_FITMAP_MINUS_Z = 5
} br_fitmap_axis;
typedef void br_putline_cbfn(char*, void*);
typedef int br_mode_test_cbfn(br_uint_8*, br_size_t);
typedef br_uint_32 brfile_attributes_cbfn();
typedef void* brfile_open_read_cbfn(char*, br_size_t, br_mode_test_cbfn*, int*);
typedef void* brfile_open_write_cbfn(char*, int);
typedef void brfile_close_cbfn(void*);
typedef int brfile_eof_cbfn(void*);
typedef int brfile_getchr_cbfn(void*);
typedef void brfile_putchr_cbfn(int, void*);
typedef br_size_t brfile_read_cbfn(void*, br_size_t, unsigned int, void*);
typedef br_size_t brfile_write_cbfn(void*, br_size_t, unsigned int, void*);
typedef br_size_t brfile_getline_cbfn(char*, br_size_t, void*);
typedef void brfile_putline_cbfn(char*, void*);
typedef void brfile_advance_cbfn(br_size_t, void*);
typedef void* brmem_allocate_cbfn(br_size_t, br_uint_8);
typedef void brmem_free_cbfn(void*);
typedef br_size_t brmem_inquire_cbfn(br_uint_8);
typedef br_uint_32 brmem_align_cbfn(br_uint_8);
typedef void br_resourcefree_cbfn(void*, br_uint_8, br_size_t);
typedef void br_diag_warning_cbfn(char*);
typedef void br_diag_failure_cbfn(char*);
typedef struct br_resource_class br_resource_class;
typedef br_resource_class* br_resclass_find_cbfn(char*);
typedef br_uint_32 br_resclass_enum_cbfn(br_resource_class*, void*);
typedef int br_qsort_cbfn(void*, void*);
typedef br_uint_32 br_resenum_cbfn(void*, void*);
typedef struct br_device br_device;
typedef br_device* br_device_begin_fn(char*);
typedef br_boolean br_device_enum_cbfn(char*, br_uint_32, char*, char*, char*, char*, void*);
typedef struct br_outfcty_desc br_outfcty_desc;
typedef br_boolean br_outfcty_enum_cbfn(char*, br_outfcty_desc*, void*);
typedef struct br_actor br_actor;
typedef struct br_model br_model;
typedef struct br_material br_material;
typedef void br_model_custom_cbfn(br_actor*, br_model*, br_material*, void*, br_uint_8, int);
typedef int br_fmt_report_cbfn(char*);
typedef struct br_matrix4 br_matrix4;
typedef void br_renderbounds_cbfn(br_actor*, br_model*, br_material*, void*, br_uint_8, br_matrix4*, br_int_32*);
typedef struct br_primitive br_primitive;
typedef struct br_order_table br_order_table;
typedef void br_primitive_cbfn(br_primitive*, br_actor*, br_model*, br_material*, br_order_table*, br_scalar*);
typedef br_material* br_material_find_cbfn(char*);
typedef br_uint_32 br_material_enum_cbfn(br_material*, void*);
typedef br_model* br_model_find_cbfn(char*);
typedef br_uint_32 br_model_enum_cbfn(br_model*, void*);
typedef struct br_pixelmap br_pixelmap;
typedef br_pixelmap* br_map_find_cbfn(char*);
typedef br_uint_32 br_map_enum_cbfn(br_pixelmap*, void*);
typedef br_pixelmap* br_table_find_cbfn(char*);
typedef br_uint_32 br_table_enum_cbfn(br_pixelmap*, void*);
typedef br_uint_32 br_actor_enum_cbfn(br_actor*, void*);
typedef struct br_vector3 br_vector3;
typedef int br_pick2d_cbfn(br_actor*, br_model*, br_material*, br_vector3*, br_vector3*, br_scalar, br_scalar, void*);
typedef struct br_matrix34 br_matrix34;
typedef int br_pick3d_cbfn(br_actor*, br_model*, br_material*, br_matrix34*, br_bounds*, void*);
typedef struct br_vector2 br_vector2;
typedef int br_modelpick2d_cbfn(br_model*, br_material*, br_vector3*, br_vector3*, br_scalar, int, int, int, br_vector3*, br_vector2*, void*);

typedef enum view_to_screen_type {
    BR_VTOS_PARALLEL = 0,
    BR_VTOS_PERSPECTIVE = 1,
    BR_VTOS_4X4 = 2
} view_to_screen_type;

typedef void dev_fill_cbfn(br_device*, br_pixelmap*, br_uint_32);
typedef void dev_rectangle_copy_cbfn(br_device*, br_pixelmap*, br_uint_16, br_uint_16, br_pixelmap*, br_uint_16, br_uint_16, br_uint_16, br_uint_16);
typedef void dev_rectangle_fill_cbfn(br_device*, br_pixelmap*, br_uint_16, br_uint_16, br_uint_16, br_uint_16, br_uint_32);
typedef void dev_dirty_rectangle_copy_cbfn(br_device*, br_pixelmap*, br_pixelmap*, br_uint_16, br_uint_16, br_uint_16, br_uint_16);
typedef void dev_dirty_rectangle_fill_cbfn(br_device*, br_pixelmap*, br_uint_16, br_uint_16, br_uint_16, br_uint_16, br_uint_32);
typedef void dev_plot_cbfn(br_device*, br_pixelmap*, br_uint_16, br_uint_16, br_uint_32);
typedef void dev_copy_cbfn(br_device*, br_pixelmap*, br_pixelmap*);
typedef void dev_line_cbfn(br_device*, br_pixelmap*, br_int_16, br_int_16, br_int_16, br_int_16, br_uint_32);
typedef void dev_copy_bits_cbfn(br_device*, br_pixelmap*, br_int_16, br_int_16, br_uint_8*, br_uint_16, br_uint_16, br_uint_16, br_uint_16, br_uint_32);
typedef br_pixelmap* dev_double_buffer_cbfn(br_device*, br_pixelmap*, br_pixelmap*);
typedef br_pixelmap* dev_match_cbfn(br_device*, br_pixelmap*, br_uint_8);
typedef br_pixelmap* dev_clone_cbfn(br_device*, br_pixelmap*);
typedef void dev_free_cbfn(br_device*, br_pixelmap*);

typedef struct br_resource_class { // size: 20
    br_uint_32 reserved; // @0
    char* identifier; // @4
    br_uint_8 res_class; // @8
    br_resourcefree_cbfn* free_cb; // @12
    br_uint_32 alignment; // @16
} br_resource_class;

typedef struct br_filesystem {
    char* identifier;
    brfile_attributes_cbfn* attributes;
    brfile_open_read_cbfn* open_read;
    brfile_open_write_cbfn* open_write;
    brfile_close_cbfn* close;
    brfile_eof_cbfn* eof;
    brfile_getchr_cbfn* getchr;
    brfile_putchr_cbfn* putchr;
    brfile_read_cbfn* read;
    brfile_write_cbfn* write;
    brfile_getline_cbfn* getline;
    brfile_putline_cbfn* putline;
    brfile_advance_cbfn* advance;
} br_filesystem;

typedef struct br_allocator {
    char* identifier;
    brmem_allocate_cbfn* allocate;
    brmem_free_cbfn* free;
    brmem_inquire_cbfn* inquire;
    brmem_align_cbfn* align;
} br_allocator;

typedef struct br_diaghandler {
    char* identifier;
    br_diag_warning_cbfn* warning;
    br_diag_failure_cbfn* failure;
} br_diaghandler;

typedef struct br_errorhandler {
    char* identifier;
    br_diag_warning_cbfn* message;
    br_diag_failure_cbfn* error;
} br_errorhandler;

typedef struct br_colour_range {
    br_colour low;
    br_colour high;
} br_colour_range;

typedef struct br_matrix23 {
    br_scalar m[3][2];
} br_matrix23;

typedef struct br_matrix23_f {
    br_float m[3][2];
} br_matrix23_f;

typedef struct br_matrix23_x {
    br_fixed_ls m[3][2];
} br_matrix23_x;

typedef struct br_matrix34 {
    br_scalar m[4][3];
} br_matrix34;

typedef struct br_matrix34_f {
    br_float m[4][3];
} br_matrix34_f;

typedef struct br_matrix34_x {
    br_fixed_ls m[4][3];
} br_matrix34_x;

typedef struct br_matrix4 {
    br_scalar m[4][4];
} br_matrix4;

typedef struct br_matrix4_f {
    br_float m[4][4];
} br_matrix4_f;

typedef struct br_matrix4_x {
    br_fixed_ls m[4][4];
} br_matrix4_x;

typedef struct br_vector2 {
    br_scalar v[2];
} br_vector2;

typedef struct br_vector3 {
    br_scalar v[3];
} br_vector3;

typedef struct br_vector4 {
    br_scalar v[4];
} br_vector4;

typedef struct br_vector2_x {
    br_fixed_ls v[2];
} br_vector2_x;

typedef struct br_vector3_x {
    br_fixed_ls v[3];
} br_vector3_x;

typedef struct br_vector4_x {
    br_fixed_ls v[4];
} br_vector4_x;

typedef struct br_vector2_f {
    br_float v[2];
} br_vector2_f;

typedef struct br_vector3_f {
    br_float v[3];
} br_vector3_f;

typedef struct br_vector4_f {
    br_float v[4];
} br_vector4_f;

typedef struct br_vector2_i {
    br_int_32 v[2];
} br_vector2_i;

typedef struct br_vector3_i {
    br_int_32 v[3];
} br_vector3_i;

typedef struct br_vector4_i {
    br_int_32 v[4];
} br_vector4_i;

typedef struct br_fvector2 {
    br_fraction v[2];
} br_fvector2;

typedef struct br_fvector3 {
    br_fraction v[3];
} br_fvector3;

typedef struct br_fvector4 {
    br_fraction v[4];
} br_fvector4;

typedef struct br_fvector2_f {
    br_fraction_f v[2];
} br_fvector2_f;

typedef struct br_fvector3_f {
    br_fraction_f v[3];
} br_fvector3_f;

typedef struct br_fvector4_f {
    br_fraction v[4];
} br_fvector4_f;

typedef struct br_fvector2_x {
    br_fraction_x v[2];
} br_fvector2_x;

typedef struct br_fvector3_x {
    br_fraction_x v[3];
} br_fvector3_x;

typedef struct br_fvector4_x {
    br_fraction_x v[4];
} br_fvector4_x;

typedef struct br_bounds2 {
    br_vector2 min;
    br_vector2 max;
} br_bounds2;

typedef struct br_bounds3 {
    br_vector3 min;
    br_vector3 max;
} br_bounds3;

typedef struct br_bounds4 {
    br_vector4 min;
    br_vector4 max;
} br_bounds4;

typedef struct br_bounds2_f {
    br_vector2_f min;
    br_vector2_f max;
} br_bounds2_f;

typedef struct br_bounds3_f {
    br_vector3_f min;
    br_vector3_f max;
} br_bounds3_f;

typedef struct br_bounds4_f {
    br_vector4_f min;
    br_vector4_f max;
} br_bounds4_f;

typedef struct br_bounds2_x {
    br_vector2_x min;
    br_vector2_x max;
} br_bounds2_x;

typedef struct br_bounds3_x {
    br_vector3_x min;
    br_vector3_x max;
} br_bounds3_x;

typedef struct br_bounds4_x {
    br_vector4_x min;
    br_vector4_x max;
} br_bounds4_x;

typedef struct br_bounds2_i {
    br_vector2_i min;
    br_vector2_i max;
} br_bounds2_i;

typedef struct br_bounds3_i {
    br_vector3_i min;
    br_vector3_i max;
} br_bounds3_i;

typedef struct br_bounds4_i {
    br_vector4_i min;
    br_vector4_i max;
} br_bounds4_i;

typedef struct br_euler {
    br_angle a;
    br_angle b;
    br_angle c;
    br_uint_8 order;
} br_euler;

typedef struct br_quat {
    br_scalar x;
    br_scalar y;
    br_scalar z;
    br_scalar w;
} br_quat;

typedef struct br_transform {
    br_uint_16 type;
    struct {
        br_matrix34 mat;
        struct {
            br_euler e;
            br_scalar _pad[7];
            br_vector3 t;
        } a;
        struct {
            br_quat q;
            br_scalar _pad[5];
            br_vector3 t;
        } b;
        struct {
            br_vector3 look;
            br_vector3 up;
            br_scalar _pad[3];
            br_vector3 t;
        } c;
        struct {
            br_scalar _pad[9];
            br_vector3 t;
        } d;
    };
} br_transform;

typedef struct br_pixelmap {
    br_uint_32 _reserved;
    char* identifier;
    void* pixels;
    br_uint_32 pixels_qualifier;
    br_pixelmap* map;
    br_colour_range src_key;
    br_colour_range dst_key;
    br_uint_32 key;
    br_int_16 row_bytes;
    br_int_16 mip_offset;
    br_uint_8 type;
    br_uint_8 flags;
    br_uint_16 copy_function;
    br_uint_16 base_x;
    br_uint_16 base_y;
    br_uint_16 width;
    br_uint_16 height;
    br_int_16 origin_x;
    br_int_16 origin_y;
    void* user;
    void* stored;
} br_pixelmap;

typedef struct br_point {
    br_int_32 x;
    br_int_32 y;
} br_point;

typedef struct br_rectangle {
    br_int_32 x;
    br_int_32 y;
    br_int_32 w;
    br_int_32 h;
} br_rectangle;

typedef struct br_font {
    br_uint_32 flags;
    br_uint_16 glyph_x;
    br_uint_16 glyph_y;
    br_int_16 spacing_x;
    br_int_16 spacing_y;
    br_int_8* width;
    br_uint_16* encoding;
    br_uint_8* glyphs;
} br_font;

typedef struct br_object {
} br_object;

typedef struct br_token_value br_token_value;
typedef struct br_value_tag {
    br_uint_32 u32;
    br_boolean b;
    void* p;
    br_token t;
    br_int_8 i8;
    br_uint_8 u8;
    br_int_16 i16;
    br_uint_16 u16;
    br_int_32 i32;
    br_fixed_ls x;
    br_float f;
    br_scalar s;
    br_angle a;
    br_colour rgb;
    br_object* o;
    br_vector2_i* v2_i;
    br_vector3_i* v3_i;
    br_vector4_i* v4_i;
    br_vector2_x* v2_x;
    br_vector3_x* v3_x;
    br_vector4_x* v4_x;
    br_vector2_f* v2_f;
    br_vector3_f* v3_f;
    br_vector4_f* v4_f;
    br_matrix23_x* m23_x;
    br_matrix34_x* m34_x;
    br_matrix4_x* m4_x;
    br_matrix23_f* m23_f;
    br_matrix34_f* m34_f;
    br_matrix4_f* m4_f;
    char* str;
    char* cstr;
    br_object** hl;
    br_token* tl;
    void** pl;
    br_token_value* tvl;
} br_value;

typedef struct br_token_value {
    br_token t;
    br_value v;
} br_token_value;

typedef struct br_pool_block br_pool_block;
typedef struct br_pool_block {
    br_pool_block* next;
} br_pool_block;

typedef struct br_pool {
    br_pool_block* free;
    br_uint_32 block_size;
    br_uint_32 chunk_size;
    int mem_type;
} br_pool;

typedef struct br_primitive {
    br_primitive* next;
    br_uint_32 type;
    void* stored;
    void* v[3];
} br_primitive;

typedef struct br_order_table {
    br_primitive** table;
    br_uint_16 size;
    br_order_table* next;
    br_scalar min_z;
    br_scalar max_z;
    br_scalar sort_z;
    br_scalar scale;
    br_uint_32 flags;
    br_uint_16 type;
    br_uint_16 visits;
} br_order_table;

typedef struct br_primitive_heap {
    br_uint_8* base;
    br_size_t size;
    br_uint_8* current;
} br_primitive_heap;

typedef struct br_associative_array_tag {
    br_token_value* tv;
    br_uint_16 num_elements;
    br_uint_16 max_elements;
} br_associative_array;

typedef struct br_device { // size: 68
    br_uint_32 magic; // @0
    char* identifier; // @4
    br_font* default_font; // @8
    br_uint_32 qualifier; // @12
    dev_fill_cbfn* fill; // @16
    dev_rectangle_copy_cbfn* rectangle_copy; // @20
    dev_rectangle_fill_cbfn* rectangle_fill; // @24
    dev_dirty_rectangle_copy_cbfn* dirty_rectangle_copy; // @28
    dev_dirty_rectangle_fill_cbfn* dirty_rectangle_fill; // @32
    dev_plot_cbfn* plot; // @36
    dev_copy_cbfn* copy; // @40
    dev_line_cbfn* line; // @44
    dev_copy_bits_cbfn* copy_bits; // @48
    dev_double_buffer_cbfn* double_buffer; // @52
    dev_match_cbfn* match; // @56
    dev_clone_cbfn* clone; // @60
    dev_free_cbfn* free; // @64
} br_device;

typedef struct br_outfcty_desc {
    br_int_32 width;
    br_int_32 width_min;
    br_int_32 width_max;
    br_int_32 height;
    br_int_32 height_min;
    br_int_32 height_max;
    br_uint_8 pmtype;
    br_int_32 pmbits;
    br_boolean indexed;
    br_boolean fullscreen;
} br_outfcty_desc;

typedef struct br_renderer_facility {
} br_renderer_facility;

typedef struct br_device_pixelmap {
} br_device_pixelmap;

typedef struct br_primitive_library {
} br_primitive_library;

typedef struct br_geometry {
} br_geometry;

typedef struct br_renderer {
} br_renderer;

typedef struct br_material {
    br_uint_32 _reserved;
    char* identifier;
    br_colour colour;
    br_uint_8 opacity;
    br_ufraction ka;
    br_ufraction kd;
    br_ufraction ks;
    br_scalar power;
    br_uint_32 flags;
    br_matrix23 map_transform;
    br_uint_8 index_base;
    br_uint_8 index_range;
    br_pixelmap* colour_map;
    br_pixelmap* screendoor;
    br_pixelmap* index_shade;
    br_pixelmap* index_blend;
    br_pixelmap* index_fog;
    br_token_value* extra_surf;
    br_token_value* extra_prim;
    br_scalar fog_min;
    br_scalar fog_max;
    br_colour fog_colour;
    br_uint_32 flags_ext;
    br_pixelmap* colour_map_1;
    br_matrix23 map_transform_1;
    br_int_8 mip_modifier;
    br_uint_8 min_mip;
    br_uint_8 max_mip;
    br_uint_8 alpha_mode;
    br_uint_8 zbuffer_mode;
    br_uint_8 zbuffer_compare;
    br_int_32 subdivide_tolerance;
    void* user;
    void* stored;
} br_material;

typedef struct br_vertex {
    br_vector3 p;
    br_vector2 map;
    br_uint_8 index;
    br_uint_8 red;
    br_uint_8 grn;
    br_uint_8 blu;
    br_uint_16 _pad0;
    br_fvector3 n;
} br_vertex;

typedef struct br_face {
    br_uint_16 vertices[3];
    br_uint_16 smoothing;
    br_material* material;
    br_uint_8 index;
    br_uint_8 red;
    br_uint_8 grn;
    br_uint_8 blu;
    br_uint_8 flags;
    br_uint_8 _pad0;
    br_uint_32 _pad1;
    br_fvector3 n;
    br_scalar d;
} br_face;

typedef struct br_primitive_list br_primitive_list;
typedef struct br_primitive_list {
    br_primitive_list* next;
    br_uint_32 prim_type;
    br_uint_16 nprims;
    br_uint_16 nspares;
    void* prim;
    void* spare;
} br_primitive_list;

typedef struct br_vertex_single {
    br_vector3 p;
    br_vector2 map;
    br_uint_8 alpha;
    br_uint_8 red;
    br_uint_8 grn;
    br_uint_8 blu;
    br_uint_16 _pad0;
    br_fvector3 n;
} br_vertex_single;

typedef struct br_vertex_double {
    br_vector3 p;
    br_vector2 map0;
    br_uint_8 alpha0;
    br_uint_8 red0;
    br_uint_8 grn0;
    br_uint_8 blu0;
    br_vector2 map1;
    br_uint_8 alpha1;
    br_uint_8 red1;
    br_uint_8 grn1;
    br_uint_8 blu1;
    br_fvector3 n;
} br_vertex_double;

typedef struct br_point_prim {
    br_uint_16 vertices[1];
    br_material* material;
} br_point_prim;

typedef struct br_line {
    br_uint_16 vertices[2];
    br_material* material;
    br_uint_8 alpha0;
    br_uint_8 red0;
    br_uint_8 grn0;
    br_uint_8 blu0;
    br_uint_8 alpha1;
    br_uint_8 red1;
    br_uint_8 grn1;
    br_uint_8 blu1;
} br_line;

typedef struct br_triangle {
    br_uint_16 vertices[3];
    br_uint_16 smoothing;
    br_material* material;
    br_uint_8 alpha0;
    br_uint_8 red0;
    br_uint_8 grn0;
    br_uint_8 blu0;
    br_uint_8 alpha1;
    br_uint_8 red1;
    br_uint_8 grn1;
    br_uint_8 blu1;
    br_uint_8 flags;
    br_fvector3 n;
    br_scalar d;
} br_triangle;

typedef struct br_quad {
    br_uint_16 vertices[4];
    br_uint_16 smoothing;
    br_material* material;
    br_uint_8 alpha0;
    br_uint_8 red0;
    br_uint_8 grn0;
    br_uint_8 blu0;
    br_uint_8 alpha1;
    br_uint_8 red1;
    br_uint_8 grn1;
    br_uint_8 blu1;
    br_uint_8 flags;
    br_fvector3 n;
    br_scalar d;
} br_quad;

typedef struct br_strip_face_data {
    br_uint_16 smoothing;
    br_uint_8 alpha0;
    br_uint_8 red0;
    br_uint_8 grn0;
    br_uint_8 blu0;
    br_uint_8 alpha1;
    br_uint_8 red1;
    br_uint_8 grn1;
    br_uint_8 blu1;
    br_uint_8 flags;
    br_fvector3 n;
    br_scalar d;
} br_strip_face_data;

typedef struct br_tri_strip {
    br_uint_16 nvertices;
    br_uint_16* vertices;
    br_material* material;
    br_strip_face_data* face_data;
} br_tri_strip;

typedef struct br_actor {
    br_actor* next;
    br_actor** prev;
    br_actor* children;
    br_actor* parent;
    br_uint_16 depth;
    br_uint_8 type;
    char* identifier;
    br_model* model;
    br_material* material;
    br_uint_8 render_style;
    void* render_data;
    br_transform t;
    void* type_data;
    void* user;
} br_actor;

typedef struct br_model {
    br_uint_32 _reserved;
    char* identifier;
    br_vertex* vertices;
    br_face* faces;
    br_uint_16 nvertices;
    br_uint_16 nfaces;
    br_vector3 pivot;
    br_uint_16 flags;
    br_model_custom_cbfn* custom;
    void* user;
    br_angle crease_angle;
    br_scalar radius;
    br_bounds bounds;
    void* prepared;
    void* stored;
    br_uint_16 nprimitive_lists;
    br_primitive_list* primitive_list;
} br_model;

typedef struct br_camera {
    char* identifier;
    br_uint_8 type;
    br_angle field_of_view;
    br_scalar hither_z;
    br_scalar yon_z;
    br_scalar aspect;
    br_scalar width;
    br_scalar height;
    br_scalar distance;
    void* user;
} br_camera;

typedef struct br_light {
    char* identifier;
    br_uint_8 type;
    br_colour colour;
    br_scalar attenuation_c;
    br_scalar attenuation_l;
    br_scalar attenuation_q;
    br_angle cone_outer;
    br_angle cone_inner;
    void* user;
} br_light;

typedef struct br_fmt_results {
    br_actor** actors;
    br_model** models;
    br_light** lights;
    br_camera** cameras;
    br_vector4** clip_planes;
    br_bounds3** bounds;
    br_bounds3** bounds_correct;
    br_material** materials;
    br_pixelmap** pixelmaps;
    br_uint_32 nactors;
    br_uint_32 nmodels;
    br_uint_32 nlights;
    br_uint_32 ncameras;
    br_uint_32 nclip_planes;
    br_uint_32 nbounds;
    br_uint_32 nbounds_correct;
    br_uint_32 nmaterials;
    br_uint_32 npixelmaps;
} br_fmt_results;

typedef struct br_fmt_options {
    br_uint_32 convert_flags;
} br_fmt_options;

typedef struct br_3ds_options {
    br_uint_32 flags;
    br_scalar hither;
    br_scalar yon;
    br_scalar scale;
    br_fmt_report_cbfn* report;
} br_3ds_options;

typedef struct br_registery br_registry;
typedef void* br_find_failed_cbfn(char*);
typedef br_uint_32 br_enum_cbfn(void*, void*);
typedef void br_surface_fn(br_vertex*, br_fvector3*, br_scalar*);
typedef br_uint_32 br_face_surface_fn(br_vertex*, br_face*, int);
typedef struct br_active_light br_active_light;
typedef void br_light_sub_fn(br_vector3*, br_fvector3*, br_active_light*, br_scalar*);
typedef void br_model_update_cbfn(br_model*, br_uint_16);
typedef void br_material_update_cbfn(br_material*, br_uint_16);
typedef void br_table_update_cbfn(br_pixelmap*, br_uint_16);
typedef void br_map_update_cbfn(br_pixelmap*, br_uint_16);
typedef struct br_node br_node;
typedef struct br_node {
    br_node* next;
    br_node* prev;
} br_node;

typedef struct br_list {
    br_node* head;
    br_node* _null;
    br_node* tail;
} br_list;

typedef struct br_simple_node br_simple_node;
typedef struct br_simple_node {
    br_simple_node* next;
    // TODO: We changed this from "** prev" to "*prev". Is this really correct?
    br_simple_node* prev;
} br_simple_node;

typedef struct br_simple_list {
    br_simple_node* head;
} br_simple_list;

typedef struct br_registry_entry { // size: 12
    br_node node; // @0
    char** item; // @8
} br_registry_entry;

typedef struct br_registery { // size: 20
    br_list list; // @0
    int count; // @12
    br_find_failed_cbfn* find_failed_hook; // @16
} br_registry;

typedef struct br_active_light {
    br_scalar intensity;
    br_vector3 view_position;
    br_vector3 view_direction;
    br_vector3 position;
    br_vector3 direction;
    br_vector3 half;
    br_scalar spot_cosine_outer;
    br_scalar spot_cosine_inner;
    void (*light_sub_function)(br_vector3*, br_fvector3*, br_active_light*, br_scalar*);
    int type;
    br_light* light;
} br_active_light;

typedef struct br_active_clip_plane {
    br_vector4 screen_plane;
} br_active_clip_plane;

typedef struct fw_fn_table {
    br_surface_fn* light;
    br_surface_fn* light_material;
    br_surface_fn* light_vertex;
    br_surface_fn* light_texture;
    br_face_surface_fn* face_light;
    br_light_sub_fn* direct;
    br_light_sub_fn* point;
    br_light_sub_fn* point_attn;
    br_light_sub_fn* spot;
    br_light_sub_fn* spot_attn;
} fw_fn_table;

typedef struct br_image_section {
    char* name;
    void* base;
    br_size_t mem_offset;
    br_size_t mem_size;
    br_uint_32 data_offset;
    br_uint_32 data_size;
} br_image_section;

typedef struct br_image br_image;
typedef struct br_image {
    br_node node;
    char* identifier;
    br_int_32 type;
    br_int_32 ref_count;
    br_uint_32 ordinal_base;
    br_uint_32 n_functions;
    void** functions;
    br_uint_32 n_names;
    char** names;
    br_uint_16* name_ordinals;
    br_uint_16 n_imports;
    br_image** imports;
    br_uint_16 n_sections;
    br_image_section* sections;
    void* type_pointer;
} br_image;

typedef struct br_open_device {
    br_device* dev;
    br_image* image;
} br_open_device;

typedef struct br_framework_state { // size: 1136
    br_boolean active; // @0
    br_registry reg_resource_classes; // @4
    br_resource_class* resource_class_index[256]; // @24
    br_list tokens; // @1048
    br_token next_free_token; // @1060
    br_filesystem* fsys; // @1064
    br_allocator* mem; // @1068
    br_diaghandler* diag; // @1072
    int open_mode; // @1076
    void* res; // @1080
    void* scratch_ptr; // @1084
    br_size_t scratch_size; // @1088
    br_size_t scratch_last; // @1092
    int scratch_inuse; // @1096
    br_error last_error_type; // @1100
    void** last_error_value; // @1104
    br_list images; // @1108
    br_open_device* dev_slots; // @1120
    br_int_32 ndev_slots; // @1124
    br_associative_array* sys_config; // @1128
    br_boolean bAlreadyLoadedDrivers; // @1132
} br_framework_state;

// br_framework_state defined by "C:\DETHRACE\source\common\finteray.c" module
typedef struct br_framework_state_2 {
    br_surface_fn* surface_fn;
    br_surface_fn* surface_fn_after_map;
    br_surface_fn* surface_fn_after_copy;
    br_face_surface_fn* face_surface_fn;
    br_matrix23 map_transform;
    br_scalar index_base;
    br_scalar index_range;
    br_matrix4 model_to_screen;
    br_matrix4 view_to_screen;
    br_matrix34 model_to_view;
    br_matrix34 view_to_model;
    br_matrix34 model_to_environment;
    struct {
        br_matrix34 m;
        br_actor* a;
    };
    int vtos_type;
    br_vector3 eye_m;
    br_vector3 eye_m_normalised;
    br_material* material;
    br_active_light active_lights_model[16];
    br_active_light active_lights_view[16];
    br_uint_16 nactive_lights_model;
    br_uint_16 nactive_lights_view;
    int light_is_1md;
    br_vector3 eye_l;
    br_active_clip_plane active_clip_planes[6];
    br_uint_16 nactive_clip_planes;
    br_actor* enabled_lights[16];
    br_actor* enabled_clip_planes[6];
    br_actor* enabled_environment;
    br_pixelmap* output;
    br_scalar vp_width;
    br_scalar vp_height;
    br_scalar vp_ox;
    br_scalar vp_oy;
    int rendering;
    br_registry reg_models;
    br_registry reg_materials;
    br_registry reg_textures;
    br_registry reg_tables;
    br_registry reg_resource_classes;
    br_resource_class* resource_class_index[256];
    br_model_update_cbfn* model_update;
    br_material_update_cbfn* material_update;
    br_table_update_cbfn* table_update;
    br_map_update_cbfn* map_update;
    br_filesystem* fsys;
    br_allocator* mem;
    br_errorhandler* err;
    int open_mode;
    void* res;
    br_model* default_model;
    br_material* default_material;
    fw_fn_table fn_table;
    void* scratch_ptr;
    br_size_t scratch_size;
    br_size_t scratch_last;
    int scratch_inuse;
} br_framework_state_2;

typedef br_error br_exception;
typedef void br_resident_fn();
typedef struct br_tv_template_entry {
    br_token token;
    char* name;
    br_int_32 offset;
    br_int_16 flags;
    br_int_16 conv;
    br_int_32 conv_arg;
    br_int_32 mask;
} br_tv_template_entry;

typedef struct br_tv_template {
    int n_entries;
    br_tv_template_entry* entries;
    br_int_32 map_base;
    br_int_32 n_map_entries;
    br_tv_template_entry** map_query_entry;
    br_tv_template_entry** map_set_entry;
    br_boolean names_resolved;
    void* res;
} br_tv_template;

typedef struct br_object_container_dispatch br_object_container_dispatch;
typedef struct br_object_container {
    br_object_container_dispatch* dispatch;
} br_object_container;

typedef struct br_object_container_dispatch {
    void (*__reserved0)(br_object*);
    void (*__reserved1)(br_object*);
    void (*__reserved2)(br_object*);
    void (*__reserved3)(br_object*);
    void (*_free)(br_object*);
    char* (*_identifier)(br_object*);
    br_token (*_type)(br_object*);
    br_boolean (*_isType)(br_object*, br_token);
    br_device* (*_device)(br_object*);
    br_int_32 (*_space)(br_object*);
    br_tv_template* (*_templateQuery)(br_object*);
    br_error (*_query)(br_object*, br_uint_32*, br_token);
    br_error (*_queryBuffer)(br_object*, br_uint_32*, void*, br_size_t, br_token);
    br_error (*_queryMany)(br_object*, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_queryManySize)(br_object*, br_size_t*, br_token_value*);
    br_error (*_queryAll)(br_object*, br_token_value*, br_size_t);
    br_error (*_queryAllSize)(br_object*, br_size_t*);
    void* (*_listQuery)(br_object_container*);
    void* (*_tokensMatchBegin)(br_object_container*, br_token, br_token_value*);
    br_boolean (*_tokensMatch)(br_object_container*, br_object*, void*);
    void (*_tokensMatchEnd)(br_object_container*, void*);
    br_error (*_addFront)(br_object_container*, br_object*);
    br_error (*_removeFront)(br_object_container*, br_object**);
    br_error (*_remove)(br_object_container*, br_object*);
    br_error (*_find)(br_object_container*, br_object**, br_token, char*, br_token_value*);
    br_error (*_findMany)(br_object_container*, br_object**, br_int_32, br_int_32*, br_token, char*, br_token_value*);
    br_error (*_count)(br_object_container*, br_int_32*, br_token, char*, br_token_value*);
} br_object_container_dispatch;

typedef struct br_device_dispatch {
    void (*__reserved0)(br_object*);
    void (*__reserved1)(br_object*);
    void (*__reserved2)(br_object*);
    void (*__reserved3)(br_object*);
    void (*_free)(br_object*);
    char* (*_identifier)(br_object*);
    br_token (*_type)(br_object*);
    br_boolean (*_isType)(br_object*, br_token);
    br_device* (*_device)(br_object*);
    br_int_32 (*_space)(br_object*);
    br_tv_template* (*_templateQuery)(br_object*);
    br_error (*_query)(br_object*, br_uint_32*, br_token);
    br_error (*_queryBuffer)(br_object*, br_uint_32*, void*, br_size_t, br_token);
    br_error (*_queryMany)(br_object*, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_queryManySize)(br_object*, br_size_t*, br_token_value*);
    br_error (*_queryAll)(br_object*, br_token_value*, br_size_t);
    br_error (*_queryAllSize)(br_object*, br_size_t*);
    void* (*_listQuery)(br_object_container*);
    void* (*_tokensMatchBegin)(br_object_container*, br_token, br_token_value*);
    br_boolean (*_tokensMatch)(br_object_container*, br_object*, void*);
    void (*_tokensMatchEnd)(br_object_container*, void*);
    br_error (*_addFront)(br_object_container*, br_object*);
    br_error (*_removeFront)(br_object_container*, br_object**);
    br_error (*_remove)(br_object_container*, br_object*);
    br_error (*_find)(br_object_container*, br_object**, br_token, char*, br_token_value*);
    br_error (*_findMany)(br_object_container*, br_object**, br_int_32, br_int_32*, br_token, char*, br_token_value*);
    br_error (*_count)(br_object_container*, br_int_32*, br_token, char*, br_token_value*);
} br_device_dispatch;

// br_device. Not sure when this is used
typedef struct br_device_2 {
    br_device_dispatch* dispatch;
} br_device_2;

typedef struct br_object_dispatch {
    void (*__reserved0)(br_object*);
    void (*__reserved1)(br_object*);
    void (*__reserved2)(br_object*);
    void (*__reserved3)(br_object*);
    void (*_free)(br_object*);
    char* (*_identifier)(br_object*);
    br_token (*_type)(br_object*);
    br_boolean (*_isType)(br_object*, br_token);
    br_device* (*_device)(br_object*);
    br_int_32 (*_space)(br_object*);
    br_tv_template* (*_templateQuery)(br_object*);
    br_error (*_query)(br_object*, br_uint_32*, br_token);
    br_error (*_queryBuffer)(br_object*, br_uint_32*, void*, br_size_t, br_token);
    br_error (*_queryMany)(br_object*, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_queryManySize)(br_object*, br_size_t*, br_token_value*);
    br_error (*_queryAll)(br_object*, br_token_value*, br_size_t);
    br_error (*_queryAllSize)(br_object*, br_size_t*);
} br_object_dispatch;

typedef struct br_geometry_stored_dispatch br_geometry_stored_dispatch;
typedef struct br_geometry_stored {
    br_geometry_stored_dispatch* dispatch;
} br_geometry_stored;

typedef struct br_renderer_state_stored_dispatch {
    void (*__reserved0)(br_object*);
    void (*__reserved1)(br_object*);
    void (*__reserved2)(br_object*);
    void (*__reserved3)(br_object*);
    void (*_free)(br_object*);
    char* (*_identifier)(br_object*);
    br_token (*_type)(br_object*);
    br_boolean (*_isType)(br_object*, br_token);
    br_device* (*_device)(br_object*);
    br_int_32 (*_space)(br_object*);
    br_tv_template* (*_templateQuery)(br_object*);
    br_error (*_query)(br_object*, br_uint_32*, br_token);
    br_error (*_queryBuffer)(br_object*, br_uint_32*, void*, br_size_t, br_token);
    br_error (*_queryMany)(br_object*, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_queryManySize)(br_object*, br_size_t*, br_token_value*);
    br_error (*_queryAll)(br_object*, br_token_value*, br_size_t);
    br_error (*_queryAllSize)(br_object*, br_size_t*);
} br_renderer_state_stored_dispatch;

typedef struct br_renderer_state_stored {
    br_renderer_state_stored_dispatch* dispatch;
} br_renderer_state_stored;

typedef struct br_buffer_stored_dispatch br_buffer_stored_dispatch;
typedef struct br_buffer_stored {
    br_buffer_stored_dispatch* dispatch;
} br_buffer_stored;

typedef struct br_device_pixelmap_dispatch {
    void (*__reserved0)(br_object*);
    void (*__reserved1)(br_object*);
    void (*__reserved2)(br_object*);
    void (*__reserved3)(br_object*);
    void (*_free)(br_object*);
    char* (*_identifier)(br_object*);
    br_token (*_type)(br_object*);
    br_boolean (*_isType)(br_object*, br_token);
    br_device* (*_device)(br_object*);
    br_int_32 (*_space)(br_object*);
    br_tv_template* (*_templateQuery)(br_object*);
    br_error (*_query)(br_object*, br_uint_32*, br_token);
    br_error (*_queryBuffer)(br_object*, br_uint_32*, void*, br_size_t, br_token);
    br_error (*_queryMany)(br_object*, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_queryManySize)(br_object*, br_size_t*, br_token_value*);
    br_error (*_queryAll)(br_object*, br_token_value*, br_size_t);
    br_error (*_queryAllSize)(br_object*, br_size_t*);
    br_error (*_validSource)(br_device_pixelmap*, br_boolean*, br_object*);
    br_error (*_resize)(br_device_pixelmap*, br_int_32, br_int_32);
    br_error (*_match)(br_device_pixelmap*, br_device_pixelmap**, br_token_value*);
    br_error (*_allocateSub)(br_device_pixelmap*, br_device_pixelmap**, br_rectangle*);
    br_error (*_copy)(br_device_pixelmap*, br_device_pixelmap*);
    br_error (*_copyTo)(br_device_pixelmap*, br_device_pixelmap*);
    br_error (*_copyFrom)(br_device_pixelmap*, br_device_pixelmap*);
    br_error (*_fill)(br_device_pixelmap*, br_uint_32);
    br_error (*_doubleBuffer)(br_device_pixelmap*, br_device_pixelmap*);
    br_error (*_copyDirty)(br_device_pixelmap*, br_device_pixelmap*, br_rectangle*, br_int_32);
    br_error (*_copyToDirty)(br_device_pixelmap*, br_device_pixelmap*, br_rectangle*, br_int_32);
    br_error (*_copyFromDirty)(br_device_pixelmap*, br_device_pixelmap*, br_rectangle*, br_int_32);
    br_error (*_fillDirty)(br_device_pixelmap*, br_uint_32, br_rectangle*, br_int_32);
    br_error (*_doubleBufferDirty)(br_device_pixelmap*, br_device_pixelmap*, br_rectangle*, br_int_32);
    br_error (*_rectangle)(br_device_pixelmap*, br_rectangle*, br_uint_32);
    br_error (*_rectangle2)(br_device_pixelmap*, br_rectangle*, br_uint_32, br_uint_32);
    br_error (*_rectangleCopy)(br_device_pixelmap*, br_point*, br_device_pixelmap*, br_rectangle*);
    br_error (*_rectangleCopyTo)(br_device_pixelmap*, br_point*, br_device_pixelmap*, br_rectangle*);
    br_error (*_rectangleCopyFrom)(br_device_pixelmap*, br_point*, br_device_pixelmap*, br_rectangle*);
    br_error (*_rectangleStretchCopy)(br_device_pixelmap*, br_rectangle*, br_device_pixelmap*, br_rectangle*);
    br_error (*_rectangleStretchCopyTo)(br_device_pixelmap*, br_rectangle*, br_device_pixelmap*, br_rectangle*);
    br_error (*_rectangleStretchCopyFrom)(br_device_pixelmap*, br_rectangle*, br_device_pixelmap*, br_rectangle*);
    br_error (*_rectangleFill)(br_device_pixelmap*, br_rectangle*, br_uint_32);
    br_error (*_pixelSet)(br_device_pixelmap*, br_point*, br_uint_32);
    br_error (*_line)(br_device_pixelmap*, br_point*, br_point*, br_uint_32);
    br_error (*_copyBits)(br_device_pixelmap*, br_point*, br_uint_8*, br_uint_16, br_rectangle*, br_uint_32);
    br_error (*_text)(br_device_pixelmap*, br_point*, br_font*, char*, br_uint_32);
    br_error (*_textBounds)(br_device_pixelmap*, br_rectangle*, br_font*, char*);
    br_error (*_rowSize)(br_device_pixelmap*, br_size_t*);
    br_error (*_rowSet)(br_device_pixelmap*, void*, br_size_t, br_uint_32);
    br_error (*_rowQuery)(br_device_pixelmap*, void*, br_size_t, br_uint_32);
    br_error (*_pixelQuery)(br_device_pixelmap*, br_uint_32*, br_point*);
    br_error (*_pixelAddressQuery)(br_device_pixelmap*, void**, br_uint_32*, br_point*);
    br_error (*_pixelAddressSet)(br_device_pixelmap*, void*, br_uint_32*);
    br_error (*_originSet)(br_device_pixelmap*, br_point*);
    br_error (*_flush)(br_device_pixelmap*);
    br_error (*_synchronise)(br_device_pixelmap*, br_token, br_boolean);
    br_error (*_directLock)(br_device_pixelmap*, br_boolean);
    br_error (*_directUnlock)(br_device_pixelmap*);
} br_device_pixelmap_dispatch;

typedef struct br_buffer_stored_dispatch {
    void (*__reserved0)(br_object*);
    void (*__reserved1)(br_object*);
    void (*__reserved2)(br_object*);
    void (*__reserved3)(br_object*);
    void (*_free)(br_object*);
    char* (*_identifier)(br_object*);
    br_token (*_type)(br_object*);
    br_boolean (*_isType)(br_object*, br_token);
    br_device* (*_device)(br_object*);
    br_int_32 (*_space)(br_object*);
    br_tv_template* (*_templateQuery)(br_object*);
    br_error (*_query)(br_object*, br_uint_32*, br_token);
    br_error (*_queryBuffer)(br_object*, br_uint_32*, void*, br_size_t, br_token);
    br_error (*_queryMany)(br_object*, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_queryManySize)(br_object*, br_size_t*, br_token_value*);
    br_error (*_queryAll)(br_object*, br_token_value*, br_size_t);
    br_error (*_queryAllSize)(br_object*, br_size_t*);
    br_error (*_update)(br_buffer_stored*, br_device_pixelmap*, br_token_value*);
} br_buffer_stored_dispatch;

typedef struct brp_vertex {
} brp_vertex;

typedef struct br_renderer_dispatch {
    void (*__reserved0)(br_object*);
    void (*__reserved1)(br_object*);
    void (*__reserved2)(br_object*);
    void (*__reserved3)(br_object*);
    void (*_free)(br_object*);
    char* (*_identifier)(br_object*);
    br_token (*_type)(br_object*);
    br_boolean (*_isType)(br_object*, br_token);
    br_device* (*_device)(br_object*);
    br_int_32 (*_space)(br_object*);
    br_tv_template* (*_templateQuery)(br_object*);
    br_error (*_query)(br_object*, br_uint_32*, br_token);
    br_error (*_queryBuffer)(br_object*, br_uint_32*, void*, br_size_t, br_token);
    br_error (*_queryMany)(br_object*, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_queryManySize)(br_object*, br_size_t*, br_token_value*);
    br_error (*_queryAll)(br_object*, br_token_value*, br_size_t);
    br_error (*_queryAllSize)(br_object*, br_size_t*);
    void* (*_listQuery)(br_object_container*);
    void* (*_tokensMatchBegin)(br_object_container*, br_token, br_token_value*);
    br_boolean (*_tokensMatch)(br_object_container*, br_object*, void*);
    void (*_tokensMatchEnd)(br_object_container*, void*);
    br_error (*_addFront)(br_object_container*, br_object*);
    br_error (*_removeFront)(br_object_container*, br_object**);
    br_error (*_remove)(br_object_container*, br_object*);
    br_error (*_find)(br_object_container*, br_object**, br_token, char*, br_token_value*);
    br_error (*_findMany)(br_object_container*, br_object**, br_int_32, br_int_32*, br_token, char*, br_token_value*);
    br_error (*_count)(br_object_container*, br_int_32*, br_token, char*, br_token_value*);
    br_error (*_validDestination)(br_renderer*, br_boolean*, br_object*);
    br_error (*_stateStoredNew)(br_renderer*, br_renderer_state_stored**, br_uint_32, br_token_value*);
    br_error (*_stateStoredAvail)(br_renderer*, br_int_32*, br_uint_32, br_token_value*);
    br_error (*_bufferStoredNew)(br_renderer*, br_buffer_stored**, br_token, br_device_pixelmap*, br_token_value*);
    br_error (*_bufferStoredAvail)(br_renderer*, br_int_32*, br_token, br_token_value*);
    br_error (*_partSet)(br_renderer*, br_token, br_int_32, br_token, br_uint_32);
    br_error (*_partSetMany)(br_renderer*, br_token, br_int_32, br_token_value*, br_int_32*);
    br_error (*_partQuery)(br_renderer*, br_token, br_int_32, br_uint_32*, br_token);
    br_error (*_partQueryBuffer)(br_renderer*, br_token, br_int_32, br_uint_32*, br_uint_32*, br_size_t, br_token);
    br_error (*_partQueryMany)(br_renderer*, br_token, br_int_32, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_partQueryManySize)(br_renderer*, br_token, br_int_32, br_size_t*, br_token_value*);
    br_error (*_partQueryAll)(br_renderer*, br_token, br_int_32, br_token_value*, br_size_t);
    br_error (*_partQueryAllSize)(br_renderer*, br_token, br_int_32, br_size_t*);
    br_error (*_partIndexQuery)(br_renderer*, br_token, br_int_32*);
    br_error (*_modelMulF)(br_renderer*, br_matrix34_f*);
    br_error (*_modelMulX)(br_renderer*, br_matrix34_x*);
    br_error (*_modelPopPushMulF)(br_renderer*, br_matrix34_f*);
    br_error (*_modelPopPushMulX)(br_renderer*, br_matrix34_x*);
    br_error (*_modelInvert)(br_renderer*);
    br_error (*_statePush)(br_renderer*, br_uint_32);
    br_error (*_statePop)(br_renderer*, br_uint_32);
    br_error (*_stateSave)(br_renderer*, br_renderer_state_stored*, br_uint_32);
    br_error (*_stateRestore)(br_renderer*, br_renderer_state_stored*, br_uint_32);
    br_error (*_stateMask)(br_renderer*, br_uint_32*, br_token*, int);
    br_error (*_stateDefault)(br_renderer*, br_uint_32);
    br_error (*_boundsTestF)(br_renderer*, br_token*, br_bounds3_f*);
    br_error (*_boundsTestX)(br_renderer*, br_token*, br_bounds3_x*);
    br_error (*_coverageTestF)(br_renderer*, br_float*, br_bounds3_f*);
    br_error (*_coverageTestX)(br_renderer*, br_fixed_ls*, br_bounds3_x*);
    br_error (*_viewDistanceF)(br_renderer*, br_float*);
    br_error (*_viewDistanceX)(br_renderer*, br_fixed_ls*);
    br_error (*_commandModeSet)(br_renderer*, br_token);
    br_error (*_commandModeQuery)(br_renderer*, br_token*);
    br_error (*_commandModeDefault)(br_renderer*);
    br_error (*_commandModePush)(br_renderer*);
    br_error (*_commandModePop)(br_renderer*);
    br_error (*_flush)(br_renderer*, br_boolean);
    br_error (*_synchronise)(br_renderer*, br_token, br_boolean);
    br_error (*_testRender)(br_renderer*, br_token, brp_vertex*, brp_vertex*, brp_vertex*);
    br_error (*_partQueryCapability)(br_renderer*, br_token, br_int_32, br_token_value*, br_size_t);
    br_error (*_stateQueryPerformance)(br_renderer*, br_fixed_lu*);
} br_renderer_dispatch;

typedef struct br_geometry_stored_dispatch {
    void (*__reserved0)(br_object*);
    void (*__reserved1)(br_object*);
    void (*__reserved2)(br_object*);
    void (*__reserved3)(br_object*);
    void (*_free)(br_object*);
    char* (*_identifier)(br_object*);
    br_token (*_type)(br_object*);
    br_boolean (*_isType)(br_object*, br_token);
    br_device* (*_device)(br_object*);
    br_int_32 (*_space)(br_object*);
    br_tv_template* (*_templateQuery)(br_object*);
    br_error (*_query)(br_object*, br_uint_32*, br_token);
    br_error (*_queryBuffer)(br_object*, br_uint_32*, void*, br_size_t, br_token);
    br_error (*_queryMany)(br_object*, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_queryManySize)(br_object*, br_size_t*, br_token_value*);
    br_error (*_queryAll)(br_object*, br_token_value*, br_size_t);
    br_error (*_queryAllSize)(br_object*, br_size_t*);
    br_error (*_render)(br_geometry_stored*, br_renderer*);
    br_error (*_renderOnScreen)(br_geometry_stored*, br_renderer*);
} br_geometry_stored_dispatch;

typedef struct br_datafile br_datafile;
typedef struct br_file_struct_member {
    br_uint_16 type;
    br_uint_32 offset;
    char* name;
    void* extra;
} br_file_struct_member;

typedef struct br_file_struct {
    char* name;
    br_uint_32 nmembers;
    br_file_struct_member* members;
    int mem_size;
} br_file_struct;

typedef struct br_file_primitives {
    char* identifier;
    int (*skip)(br_datafile*, br_uint_32);
    int (*chunk_write)(br_datafile*, br_uint_32, br_uint_32);
    int (*chunk_read)(br_datafile*, br_uint_32*);
    void (*count_write)(br_datafile*, br_uint_32);
    br_uint_32 (*count_read)(br_datafile*);
    int (*count_size)(br_datafile*);
    br_uint_32 (*struct_write)(br_datafile*, br_file_struct*, void*);
    br_uint_32 (*struct_read)(br_datafile*, br_file_struct*, void*);
    int (*struct_size)(br_datafile*, br_file_struct*, void*);
    int (*block_write)(br_datafile*, void*, int, int, int, int);
    void* (*block_read)(br_datafile*, void*, int*, int, int);
    int (*block_size)(br_datafile*, void*, int, int, int, int);
    int (*name_write)(br_datafile*, char*);
    char* (*name_read)(br_datafile*, char*);
    int (*name_size)(br_datafile*, char*);
} br_file_primitives;

typedef struct br_chunks_table_entry {
    br_uint_32 id;
    br_uint_8 has_count;
    int (*handler)(br_datafile*, br_uint_32, br_uint_32, br_uint_32);
} br_chunks_table_entry;

typedef struct br_chunks_table {
    int nentries;
    br_chunks_table_entry* entries;
} br_chunks_table;

typedef struct br_file_enum_member {
    int value;
    char* name;
} br_file_enum_member;

typedef struct br_file_enum {
    br_uint_32 nmembers;
    br_file_enum_member* members;
} br_file_enum;

typedef struct br_device_clut_dispatch br_device_clut_dispatch;
typedef struct br_device_clut {
    br_device_clut_dispatch* dispatch;
} br_device_clut;

typedef struct br_device_clut_dispatch {
    void (*__reserved0)(br_object*);
    void (*__reserved1)(br_object*);
    void (*__reserved2)(br_object*);
    void (*__reserved3)(br_object*);
    void (*_free)(br_object*);
    char* (*_identifier)(br_object*);
    br_token (*_type)(br_object*);
    br_boolean (*_isType)(br_object*, br_token);
    br_device* (*_device)(br_object*);
    br_int_32 (*_space)(br_object*);
    br_tv_template* (*_templateQuery)(br_object*);
    br_error (*_query)(br_object*, br_uint_32*, br_token);
    br_error (*_queryBuffer)(br_object*, br_uint_32*, void*, br_size_t, br_token);
    br_error (*_queryMany)(br_object*, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_queryManySize)(br_object*, br_size_t*, br_token_value*);
    br_error (*_queryAll)(br_object*, br_token_value*, br_size_t);
    br_error (*_queryAllSize)(br_object*, br_size_t*);
    br_error (*_entrySet)(br_device_clut*, br_int_32, br_colour);
    br_error (*_entryQuery)(br_device_clut*, br_colour*, br_int_32);
    br_error (*_entrySetMany)(br_device_clut*, br_int_32, br_int_32, br_colour*);
    br_error (*_entryQueryMany)(br_device_clut*, br_colour*, br_int_32, br_int_32);
} br_device_clut_dispatch;

typedef struct br_geometry_dispatch {
    void (*__reserved0)(br_object*);
    void (*__reserved1)(br_object*);
    void (*__reserved2)(br_object*);
    void (*__reserved3)(br_object*);
    void (*_free)(br_object*);
    char* (*_identifier)(br_object*);
    br_token (*_type)(br_object*);
    br_boolean (*_isType)(br_object*, br_token);
    br_device* (*_device)(br_object*);
    br_int_32 (*_space)(br_object*);
    br_tv_template* (*_templateQuery)(br_object*);
    br_error (*_query)(br_object*, br_uint_32*, br_token);
    br_error (*_queryBuffer)(br_object*, br_uint_32*, void*, br_size_t, br_token);
    br_error (*_queryMany)(br_object*, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_queryManySize)(br_object*, br_size_t*, br_token_value*);
    br_error (*_queryAll)(br_object*, br_token_value*, br_size_t);
    br_error (*_queryAllSize)(br_object*, br_size_t*);
} br_geometry_dispatch;

typedef struct br_geometry_lighting_dispatch br_geometry_lighting_dispatch;
typedef struct br_geometry_lighting {
    br_geometry_lighting_dispatch* dispatch;
} br_geometry_lighting;

typedef struct br_geometry_lighting_dispatch {
    void (*__reserved0)(br_object*);
    void (*__reserved1)(br_object*);
    void (*__reserved2)(br_object*);
    void (*__reserved3)(br_object*);
    void (*_free)(br_object*);
    char* (*_identifier)(br_object*);
    br_token (*_type)(br_object*);
    br_boolean (*_isType)(br_object*, br_token);
    br_device* (*_device)(br_object*);
    br_int_32 (*_space)(br_object*);
    br_tv_template* (*_templateQuery)(br_object*);
    br_error (*_query)(br_object*, br_uint_32*, br_token);
    br_error (*_queryBuffer)(br_object*, br_uint_32*, void*, br_size_t, br_token);
    br_error (*_queryMany)(br_object*, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_queryManySize)(br_object*, br_size_t*, br_token_value*);
    br_error (*_queryAll)(br_object*, br_token_value*, br_size_t);
    br_error (*_queryAllSize)(br_object*, br_size_t*);
    br_error (*_renderF)(br_geometry_lighting*, br_renderer*, br_vector3_f*, br_vector3_f*, br_colour*, br_colour*, br_uint_16*, int, int, int, int, int);
    br_error (*_renderX)(br_geometry_lighting*, br_renderer*, br_vector3_x*, br_vector3_x*, br_colour*, br_colour*, br_uint_16*, int, int, int, int, int);
} br_geometry_lighting_dispatch;

typedef struct br_v1db_enable {
    br_int_32 max;
    br_int_32 count;
    br_int_32 type;
    char* name;
    br_actor** enabled;
} br_v1db_enable;

typedef struct br_v1db_state {
    br_boolean active;
    br_boolean zs_active;
    br_boolean zb_active;
    br_int_32 rendering;
    br_renderer* renderer;
    br_renderer* query_renderer;
    br_geometry* format_model;
    br_geometry* format_buckets;
    br_geometry_lighting* format_lighting;
    br_matrix4 model_to_screen;
    br_matrix34 model_to_view;
    br_boolean model_to_screen_valid;
    br_uint_32 ttype;
    br_actor* render_root;
    struct {
        br_matrix34 m;
        br_actor* a;
        br_uint_8 transform_type;
    };
    br_v1db_enable enabled_lights;
    br_v1db_enable enabled_clip_planes;
    br_v1db_enable enabled_horizon_planes;
    br_int_32 max_light;
    br_int_32 max_clip;
    br_actor* enabled_environment;
    br_registry reg_models;
    br_registry reg_materials;
    br_registry reg_textures;
    br_registry reg_tables;
    void* res;
    br_model* default_model;
    br_material* default_material;
    void* default_render_data;
    br_order_table* default_order_table;
    br_order_table* primary_order_table;
    br_order_table* order_table_list;
    br_primitive_heap heap;
    br_primitive_cbfn* primitive_call;
    br_renderbounds_cbfn* bounds_call;
    br_vector2 origin;
    br_scalar vp_ox;
    br_scalar vp_oy;
    br_scalar vp_width;
    br_scalar vp_height;
    br_pixelmap* colour_buffer;
} br_v1db_state;

typedef struct br_renderer_facility_dispatch {
    void (*__reserved0)(br_object*);
    void (*__reserved1)(br_object*);
    void (*__reserved2)(br_object*);
    void (*__reserved3)(br_object*);
    void (*_free)(br_object*);
    char* (*_identifier)(br_object*);
    br_token (*_type)(br_object*);
    br_boolean (*_isType)(br_object*, br_token);
    br_device* (*_device)(br_object*);
    br_int_32 (*_space)(br_object*);
    br_tv_template* (*_templateQuery)(br_object*);
    br_error (*_query)(br_object*, br_uint_32*, br_token);
    br_error (*_queryBuffer)(br_object*, br_uint_32*, void*, br_size_t, br_token);
    br_error (*_queryMany)(br_object*, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_queryManySize)(br_object*, br_size_t*, br_token_value*);
    br_error (*_queryAll)(br_object*, br_token_value*, br_size_t);
    br_error (*_queryAllSize)(br_object*, br_size_t*);
    void* (*_listQuery)(br_object_container*);
    void* (*_tokensMatchBegin)(br_object_container*, br_token, br_token_value*);
    br_boolean (*_tokensMatch)(br_object_container*, br_object*, void*);
    void (*_tokensMatchEnd)(br_object_container*, void*);
    br_error (*_addFront)(br_object_container*, br_object*);
    br_error (*_removeFront)(br_object_container*, br_object**);
    br_error (*_remove)(br_object_container*, br_object*);
    br_error (*_find)(br_object_container*, br_object**, br_token, char*, br_token_value*);
    br_error (*_findMany)(br_object_container*, br_object**, br_int_32, br_int_32*, br_token, char*, br_token_value*);
    br_error (*_count)(br_object_container*, br_int_32*, br_token, char*, br_token_value*);
    br_error (*_validDestination)(br_renderer_facility*, br_boolean*, br_object*);
    br_error (*_rendererNew)(br_renderer_facility*, br_renderer**, br_token_value*);
} br_renderer_facility_dispatch;

typedef struct br_output_facility_dispatch br_output_facility_dispatch;
typedef struct br_output_facility {
    br_output_facility_dispatch* dispatch;
} br_output_facility;

typedef struct br_output_facility_dispatch {
    void (*__reserved0)(br_object*);
    void (*__reserved1)(br_object*);
    void (*__reserved2)(br_object*);
    void (*__reserved3)(br_object*);
    void (*_free)(br_object*);
    char* (*_identifier)(br_object*);
    br_token (*_type)(br_object*);
    br_boolean (*_isType)(br_object*, br_token);
    br_device* (*_device)(br_object*);
    br_int_32 (*_space)(br_object*);
    br_tv_template* (*_templateQuery)(br_object*);
    br_error (*_query)(br_object*, br_uint_32*, br_token);
    br_error (*_queryBuffer)(br_object*, br_uint_32*, void*, br_size_t, br_token);
    br_error (*_queryMany)(br_object*, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_queryManySize)(br_object*, br_size_t*, br_token_value*);
    br_error (*_queryAll)(br_object*, br_token_value*, br_size_t);
    br_error (*_queryAllSize)(br_object*, br_size_t*);
    void* (*_listQuery)(br_object_container*);
    void* (*_tokensMatchBegin)(br_object_container*, br_token, br_token_value*);
    br_boolean (*_tokensMatch)(br_object_container*, br_object*, void*);
    void (*_tokensMatchEnd)(br_object_container*, void*);
    br_error (*_addFront)(br_object_container*, br_object*);
    br_error (*_removeFront)(br_object_container*, br_object**);
    br_error (*_remove)(br_object_container*, br_object*);
    br_error (*_find)(br_object_container*, br_object**, br_token, char*, br_token_value*);
    br_error (*_findMany)(br_object_container*, br_object**, br_int_32, br_int_32*, br_token, char*, br_token_value*);
    br_error (*_count)(br_object_container*, br_int_32*, br_token, char*, br_token_value*);
    br_error (*_validSource)(br_output_facility*, br_boolean*, br_object*);
    br_error (*_pixelmapNew)(br_output_facility*, br_device_pixelmap**, br_token_value*);
    br_error (*_clutNew)(br_output_facility*, br_device_clut**, br_token_value*);
    br_error (*_queryCapability)(br_output_facility*, br_token_value*, br_token_value*, br_size_t);
} br_output_facility_dispatch;

typedef struct br_primitive_state_dispatch br_primitive_state_dispatch;
typedef struct br_primitive_state {
    br_primitive_state_dispatch* dispatch;
} br_primitive_state;

typedef struct brp_block {
} brp_block;

typedef struct br_primitive_state_dispatch {
    void (*__reserved0)(br_object*);
    void (*__reserved1)(br_object*);
    void (*__reserved2)(br_object*);
    void (*__reserved3)(br_object*);
    void (*_free)(br_object*);
    char* (*_identifier)(br_object*);
    br_token (*_type)(br_object*);
    br_boolean (*_isType)(br_object*, br_token);
    br_device* (*_device)(br_object*);
    br_int_32 (*_space)(br_object*);
    br_tv_template* (*_templateQuery)(br_object*);
    br_error (*_query)(br_object*, br_uint_32*, br_token);
    br_error (*_queryBuffer)(br_object*, br_uint_32*, void*, br_size_t, br_token);
    br_error (*_queryMany)(br_object*, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_queryManySize)(br_object*, br_size_t*, br_token_value*);
    br_error (*_queryAll)(br_object*, br_token_value*, br_size_t);
    br_error (*_queryAllSize)(br_object*, br_size_t*);
    br_error (*_partSet)(br_primitive_state*, br_token, br_int_32, br_token, br_uint_32);
    br_error (*_partSetMany)(br_primitive_state*, br_token, br_int_32, br_token_value*, br_int_32*);
    br_error (*_partQuery)(br_primitive_state*, br_token, br_int_32, br_uint_32*, br_token);
    br_error (*_partQueryBuffer)(br_primitive_state*, br_token, br_int_32, br_uint_32*, br_uint_32*, br_size_t, br_token);
    br_error (*_partQueryMany)(br_primitive_state*, br_token, br_int_32, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_partQueryManySize)(br_primitive_state*, br_token, br_int_32, br_size_t*, br_token_value*);
    br_error (*_partQueryAll)(br_primitive_state*, br_token, br_int_32, br_token_value*, br_size_t);
    br_error (*_partQueryAllSize)(br_primitive_state*, br_token, br_int_32, br_size_t*);
    br_error (*_partIndexQuery)(br_primitive_state*, br_token, br_int_32*);
    br_error (*_stateDefault)(br_primitive_state*, br_uint_32);
    br_error (*_stateCopy)(br_primitive_state*, br_primitive_state*, br_uint_32);
    br_error (*_renderBegin)(br_primitive_state*, brp_block**, br_boolean*, br_boolean*, br_boolean, br_int_32);
    br_error (*_renderEnd)(br_primitive_state*, brp_block*);
    br_error (*_rangesQueryF)(br_primitive_state*, br_float*, br_float*, br_int_32);
    br_error (*_rangesQueryX)(br_primitive_state*, br_fixed_ls*, br_fixed_ls*, br_int_32);
    br_error (*_partQueryCapability)(br_primitive_state*, br_token, br_int_32, br_token_value*, br_size_t);
    br_error (*_stateQueryPerformance)(br_primitive_state*, br_fixed_lu*);
} br_primitive_state_dispatch;

typedef struct br_primitive_library_dispatch {
    void (*__reserved0)(br_object*);
    void (*__reserved1)(br_object*);
    void (*__reserved2)(br_object*);
    void (*__reserved3)(br_object*);
    void (*_free)(br_object*);
    char* (*_identifier)(br_object*);
    br_token (*_type)(br_object*);
    br_boolean (*_isType)(br_object*, br_token);
    br_device* (*_device)(br_object*);
    br_int_32 (*_space)(br_object*);
    br_tv_template* (*_templateQuery)(br_object*);
    br_error (*_query)(br_object*, br_uint_32*, br_token);
    br_error (*_queryBuffer)(br_object*, br_uint_32*, void*, br_size_t, br_token);
    br_error (*_queryMany)(br_object*, br_token_value*, void*, br_size_t, br_int_32*);
    br_error (*_queryManySize)(br_object*, br_size_t*, br_token_value*);
    br_error (*_queryAll)(br_object*, br_token_value*, br_size_t);
    br_error (*_queryAllSize)(br_object*, br_size_t*);
    void* (*_listQuery)(br_object_container*);
    void* (*_tokensMatchBegin)(br_object_container*, br_token, br_token_value*);
    br_boolean (*_tokensMatch)(br_object_container*, br_object*, void*);
    void (*_tokensMatchEnd)(br_object_container*, void*);
    br_error (*_addFront)(br_object_container*, br_object*);
    br_error (*_removeFront)(br_object_container*, br_object**);
    br_error (*_remove)(br_object_container*, br_object*);
    br_error (*_find)(br_object_container*, br_object**, br_token, char*, br_token_value*);
    br_error (*_findMany)(br_object_container*, br_object**, br_int_32, br_int_32*, br_token, char*, br_token_value*);
    br_error (*_count)(br_object_container*, br_int_32*, br_token, char*, br_token_value*);
    br_error (*_stateNew)(br_primitive_library*, br_primitive_state**);
    br_error (*_bufferStoredNew)(br_primitive_library*, br_buffer_stored**, br_token, br_device_pixelmap*, br_token_value*);
    br_error (*_bufferStoredAvail)(br_primitive_library*, br_int_32*, br_token, br_token_value*);
    br_error (*_flush)(br_primitive_library*, br_boolean);
    br_error (*_synchronise)(br_primitive_library*, br_token, br_boolean);
    br_error (*_mask)(br_primitive_library*, br_uint_32*, br_token*, int);
} br_primitive_library_dispatch;

typedef struct br_file {
    void* raw_file;
    br_boolean writing;
    int mode;
    char name[1];
} br_file;

typedef enum br_lexer_token_id {
    T_EOF = 0,
    T_ERROR = 1,
    T_IDENT = 2,
    T_STRING = 3,
    T_INTEGER = 4,
    T_REAL = 5,
    T_EXCLAM = 33,
    T_DQUOTE = 34,
    T_HASH = 35,
    T_DOLLAR = 36,
    T_PERCENT = 37,
    T_AMPERSAND = 38,
    T_RQUOTE = 39,
    T_LPAREN = 40,
    T_RPAREN = 41,
    T_ASTERISK = 42,
    T_PLUS = 43,
    T_COMMA = 44,
    T_DASH = 45,
    T_PERIOD = 46,
    T_SLASH = 47,
    T_COLON = 58,
    T_SEMICOLON = 59,
    T_LANGLE = 60,
    T_EQUAL = 61,
    T_RANGLE = 62,
    T_QUERY = 63,
    T_COMAT = 64,
    T_LSQUARE = 91,
    T_BSLASH = 92,
    T_RSQUARE = 93,
    T_CARET = 94,
    T_UNDERBAR = 95,
    T_LQUOTE = 96,
    T_LBRACE = 123,
    T_VBAR = 124,
    T_RBRACE = 125,
    T_TILDE = 126,
    T_KEYWORD = 128
} br_lexer_token_id;
typedef struct br_lexer_source br_lexer_source;
typedef void br_lexer_getchar_cbfn(br_lexer_source*);
typedef struct br_lexer br_lexer;
typedef void br_lexer_error_cbfn(br_lexer*, char*);
typedef struct br_lexer_source {
    br_lexer_source* prev;
    char* name;
    br_int_32 line;
    br_int_32 next;
    br_lexer_getchar_cbfn* getchar;
    void* ptr;
} br_lexer_source;

typedef struct br_lexer_token {
    br_lexer_token_id id;
    struct {
        br_int_32 integer;
        float real;
        char* string;
    };
} br_lexer_token;

typedef struct br_lexer_keyword {
    char* name;
    br_int_32 id;
} br_lexer_keyword;

typedef struct br_lexer {
    br_lexer_source* source;
    br_lexer_token current;
    br_lexer_keyword* keywords;
    br_int_32 nkeywords;
    char eol_comment;
    void (*advance)(br_lexer*);
    br_putline_cbfn* putline;
    void* putline_arg;
    char* string_buffer;
    br_int_32 string_buffer_size;
    br_lexer_error_cbfn* error;
} br_lexer;

typedef br_error br_tv_custom_query_cbfn(br_uint_32*, void**, br_size_t*, void*, br_tv_template_entry*);
typedef br_error br_tv_custom_set_cbfn(void*, br_uint_32*, br_tv_template_entry*);
typedef br_size_t br_tv_custom_extra_size_cbfn(void*, br_tv_template_entry*);
typedef struct br_tv_custom {
    br_tv_custom_query_cbfn* query;
    br_tv_custom_set_cbfn* set;
    br_tv_custom_extra_size_cbfn* extra_size;
} br_tv_custom;

typedef struct br_token_entry { // size: 24
    br_node node; // @0
    char* identifier; // @8
    br_int_32 type; // @12
    br_token token; // @16
    br_int_32 base_length; // @20
} br_token_entry;

typedef struct br_exception_handler br_exception_handler;
typedef struct br_exception_handler {
    br_exception_handler* prev;
    jmp_buf context;
} br_exception_handler;

typedef struct br_pixelmap_state {
    void* res;
    br_tv_template* device_pixelmap_template;
    br_tv_template* pixelmap_match_template;
} br_pixelmap_state;

typedef struct pm_type_info {
    br_uint_16 bits;
    br_uint_16 file_size;
    br_uint_16 align;
    br_uint_16 channels;
} pm_type_info;

typedef struct match_tokens {
    br_token use;
    br_uint_8 pixel_type;
    br_int_32 pixel_bits;
    br_int_32 width;
    br_int_32 height;
    br_object* renderer;
} match_tokens;

typedef struct host_info {
    br_uint_32 size;
    char identifier[40];
    br_uint_32 capabilities;
    br_token processor_family;
    br_token processor_type;
} host_info;

typedef struct host_interrupt_hook {
    br_uint_8 vector;
    br_boolean active;
    br_uint_32 old_offset;
    br_uint_16 old_sel;
} host_interrupt_hook;

typedef struct host_exception_hook {
    br_uint_8 exception;
    br_boolean active;
    br_uint_32 old_offset;
    br_uint_16 old_sel;
    br_uint_8 scratch[256];
} host_exception_hook;

typedef struct host_regs {
    struct {
        br_uint_32 edi;
        br_uint_32 esi;
        br_uint_32 ebp;
        br_uint_32 _res;
        br_uint_32 ebx;
        br_uint_32 edx;
        br_uint_32 ecx;
        br_uint_32 eax;
        br_uint_16 flags;
        br_uint_16 es;
        br_uint_16 ds;
        br_uint_16 fs;
        br_uint_16 gs;
        br_uint_16 ip;
        br_uint_16 cs;
        br_uint_16 sp;
        br_uint_16 ss;
    } a;
    struct {
        br_uint_16 di;
        br_uint_16 _pad0;
        br_uint_16 si;
        br_uint_16 _pad1;
        br_uint_16 bp;
        br_uint_16 _pad2;
        br_uint_16 _res;
        br_uint_16 _pad3;
        br_uint_16 bx;
        br_uint_16 _pad4;
        br_uint_16 dx;
        br_uint_16 _pad5;
        br_uint_16 cx;
        br_uint_16 _pad6;
        br_uint_16 ax;
        br_uint_16 _pad7;
        br_uint_16 flags;
        br_uint_16 es;
        br_uint_16 ds;
        br_uint_16 fs;
        br_uint_16 gs;
        br_uint_16 ip;
        br_uint_16 cs;
        br_uint_16 sp;
        br_uint_16 ss;
    } b;
    struct {
        br_uint_32 _pad0[4];
        br_uint_8 bl;
        br_uint_8 bh;
        br_uint_8 _pad1;
        br_uint_8 _pad2;
        br_uint_8 dl;
        br_uint_8 dh;
        br_uint_8 _pad3;
        br_uint_8 _pad4;
        br_uint_8 cl;
        br_uint_8 ch;
        br_uint_8 _pad5;
        br_uint_8 _pad6;
        br_uint_8 al;
        br_uint_8 ah;
        br_uint_8 _pad7;
        br_uint_8 _pad8;
    } c;
} host_regs;

typedef struct ldt {
    unsigned long limit_0 : 10;
    unsigned long base_0 : 10;
    unsigned long base_1 : 8;
    unsigned long type : 4;
    unsigned long application : 1;
    unsigned long dpl : 2;
    unsigned long present : 1;
    unsigned long limit_1 : 4;
    unsigned long system : 1;
    unsigned long reserved : 1;
    unsigned long use32 : 1;
    unsigned long granularity : 1;
    unsigned long base_2 : 8;
} ldt;

typedef struct msdos_header {
    br_uint_16 magic;
    br_uint_16 last_page_bytes;
    br_uint_16 pages;
    br_uint_16 n_relocations;
    br_uint_16 header_size;
    br_uint_16 minalloc;
    br_uint_16 maxalloc;
    br_uint_16 start_ss;
    br_uint_16 start_sp;
    br_uint_16 checksum;
    br_uint_16 start_ip;
    br_uint_16 start_cs;
    br_uint_16 reloc_offset;
    br_uint_16 overlay_number;
    br_uint_16 _reserved0[4];
    br_uint_16 oem_id;
    br_uint_16 oem_info;
    br_uint_16 _reserved1[10];
    br_uint_32 new_header_offset;
} msdos_header;

typedef struct coff_header {
    br_uint_16 machine;
    br_uint_16 n_sections;
    br_uint_32 time_date;
    br_uint_32 symbols_offset;
    br_uint_32 n_symbols;
    br_uint_16 opt_header_size;
    br_uint_16 flags;
} coff_header;

typedef struct nt_optional_header {
    br_uint_16 magic;
    br_uint_8 l_major;
    br_uint_8 l_minor;
    br_uint_32 code_size;
    br_uint_32 data_size;
    br_uint_32 bss_size;
    br_uint_32 entry_point;
    br_uint_32 code_base;
    br_uint_32 data_base;
    br_uint_32 image_base;
    br_uint_32 section_alignment;
    br_uint_32 file_alignment;
    br_uint_16 os_major;
    br_uint_16 os_minor;
    br_uint_16 user_major;
    br_uint_16 user_minor;
    br_uint_16 subsys_major;
    br_uint_16 subsys_minor;
    br_uint_32 _reserved;
    br_uint_32 image_size;
    br_uint_32 header_size;
    br_uint_32 file_checksum;
    br_uint_16 subsystem;
    br_uint_16 dll_flags;
    br_uint_32 stack_reserve_size;
    br_uint_32 stack_commit_size;
    br_uint_32 heap_reserve_size;
    br_uint_32 heap_commit_size;
    br_uint_32 loader_flags;
    br_uint_32 n_data_directories;
    struct {
        br_uint_32 rva;
        br_uint_32 size;
    };
} nt_optional_header;

typedef struct section_header {
    br_uint_8 section_name[8];
    br_uint_32 virtual_size;
    br_uint_32 rva;
    br_uint_32 data_size;
    br_uint_32 data_offset;
    br_uint_32 relocs_offset;
    br_uint_32 linenums_offset;
    br_uint_16 n_relocs;
    br_uint_16 n_linenums;
    br_uint_32 flags;
} section_header;

#pragma pack(push, 1)
typedef struct resource_header { // size: 24
    br_simple_node node; // @0
    br_simple_list children; // @8
    br_uint_8 size_l; // @12
    br_uint_8 size_m; // @13
    br_uint_8 size_h; // @14
    br_uint_8 class; // @15
    void* magic_ptr; // @16
    br_uint_32 magic_num; // @20
} resource_header;
#pragma pack(pop)

typedef struct host_real_memory {
    br_uint_32 pm_off;
    br_uint_16 pm_seg;
    br_uint_16 _reserved;
    br_uint_16 rm_off;
    br_uint_16 rm_seg;
} host_real_memory;

typedef struct file_info {
    br_uint_32 type;
    br_uint_32 version;
} file_info;

typedef struct object_list {
    br_simple_list l;
} object_list;

typedef struct object_list_entry {
    br_simple_node n;
    br_object* h;
} object_list_entry;

typedef struct token_match {
    br_token_value* original;
    br_token_value* query;
    br_int_32 n;
    void* extra;
    br_size_t extra_size;
} token_match;

typedef struct token_type {
    char* identifier;
    br_token type;
    int length;
} token_type;

typedef struct export_directory {
    br_uint_32 flags;
    br_uint_32 timestamp;
    br_uint_16 major_version;
    br_uint_16 minor_version;
    br_uint_32 name;
    br_uint_32 ordinal_base;
    br_uint_32 n_entries;
    br_uint_32 n_names;
    br_uint_32 export_table;
    br_uint_32 name_table;
    br_uint_32 ordinal_table;
} export_directory;

typedef struct import_directory {
    br_uint_32 lookup_table;
    br_uint_32 timestamp;
    br_uint_32 forwarder;
    br_uint_32 name;
    br_uint_32 address_table;
} import_directory;

typedef struct basereloc_header {
    br_uint_32 rva;
    br_uint_32 size;
} basereloc_header;

typedef struct order_info {
    br_uint_8 a0;
    br_uint_8 a1;
    br_uint_8 a2;
    br_uint_8 _pad[1];
} order_info;

typedef struct transform_type {
    br_uint_32 id;
    br_file_struct* fs;
} transform_type;

typedef enum dosio_event_type {
    DOSIO_EVENT_KEY_DOWN = 0,
    DOSIO_EVENT_KEY_UP = 1,
    DOSIO_EVENT_POINTER1_DOWN = 2,
    DOSIO_EVENT_POINTER1_UP = 3,
    DOSIO_EVENT_POINTER2_DOWN = 4,
    DOSIO_EVENT_POINTER2_UP = 5,
    DOSIO_EVENT_POINTER3_DOWN = 6,
    DOSIO_EVENT_POINTER3_UP = 7,
    DOSIO_EVENT_POINTER_MOVE = 8,
    DOSIO_EVENT_MAX = 9
} dosio_event_type;

typedef enum dosio_event_qual {
    DOSIO_QUAL_SHIFT = 1,
    DOSIO_QUAL_CONTROL = 2,
    DOSIO_QUAL_ALT = 4
} dosio_event_qual;

typedef struct dosio_event {
    br_uint_16 type;
    br_uint_16 qualifiers;
    br_uint_32 value_1;
    br_uint_32 value_2;
} dosio_event;

typedef struct dosio_event_queue {
    br_uint_16 total;
    br_uint_16 count;
    br_uint_16 head;
    br_uint_16 tail;
    dosio_event slots[1];
} dosio_event_queue;

typedef struct pm_temp_edge {
    struct pm_temp_edge* next;
    br_uint_16 first;
    br_uint_16 last;
    char other;
} pm_temp_edge;

typedef struct prep_vertex {
    br_vector3 n;
    br_uint_16 v;
    br_uint_16 f;
} prep_vertex;

typedef struct fmt_vertex {
    br_vector3 p;
    br_vector2 map;
    br_vector3 n;
} DR_VERTEX, fmt_vertex;

typedef struct v11face {
    br_uint_16 vertices[3];
    br_uint_16 edges[3];
    br_vector4 eqn;
} DR_FACE, v11face;

typedef struct v11group {
    void* stored;
    DR_FACE* faces;
    br_colour* face_colours;
    br_uint_16* face_user;
    DR_VERTEX* vertices;
    br_colour* vertex_colours;
    br_uint_16* vertex_user;
    br_uint_16 nfaces;
    br_uint_16 nvertices;
    br_uint_16 nedges;
} v11group;

typedef struct v11model {
    br_size_t size;
    br_uint_32 flags;
    br_uint_16 ngroups;
    br_vector3 pivot;
    v11group* groups;
} v11model;

#endif
