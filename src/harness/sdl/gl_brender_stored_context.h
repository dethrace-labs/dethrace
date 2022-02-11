#ifndef GL_BRENDER_STORED_CONTEXT
#define GL_BRENDER_STORED_CONTEXT

#include "brender/br_types.h"
#include <glad/glad.h>

typedef struct tStored_model_context {
    br_object_dispatch* dispatch;
    GLuint vao_id, ebo_id;
} tStored_model_context;

typedef struct tStored_pixelmap {
    GLuint id;
} tStored_pixelmap;

typedef struct tStored_material {
    tStored_pixelmap* texture;
} tStored_material;

tStored_model_context* NewStoredModelContext();

#endif