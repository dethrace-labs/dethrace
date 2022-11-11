#ifndef GL_BRENDER_STORED_CONTEXT
#define GL_BRENDER_STORED_CONTEXT

#include "brender/br_types.h"
#include <glad/glad.h>

typedef struct tStored_model_context {
    br_object_dispatch* dispatch;
    GLuint vao_id, vbo_id, ebo_id;
} tStored_model_context;

typedef struct tStored_pixelmap {
    GLuint id;
} tStored_pixelmap;

typedef struct tStored_material {
    int index_base;
    br_uint_32 flags;
    br_pixelmap* shade_table;
    br_pixelmap* pixelmap;
    br_matrix23 map_transform;
    char identifier[200];
} tStored_material;

tStored_model_context* NewStoredModelContext();

tStored_material* NewStoredMaterial();

tStored_pixelmap* NewStoredPixelmap();

#endif
