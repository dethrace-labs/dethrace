#include "stored_context.h"
#include "../include/harness/trace.h"
#include <stdlib.h>

void _free(br_object* o) {
    tStored_model_context* ctx = (tStored_model_context*)o;
    glDeleteVertexArrays(1, &ctx->vao_id);
    glDeleteBuffers(1, &ctx->ebo_id);
    free(o);
}

br_object_dispatch dispatch_funcs = {
    NULL,
    NULL,
    NULL,
    NULL,
    _free,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

tStored_model_context* NewStoredModelContext() {
    tStored_model_context* ctx = malloc(sizeof(tStored_model_context));
    ctx->dispatch = &dispatch_funcs;
    return ctx;
}

tStored_material* NewStoredMaterial() {
    tStored_material* ctx = malloc(sizeof(tStored_material));
    ctx->index_base = -1;
    ctx->pixelmap = NULL;
    ctx->shade_table = NULL;
    ctx->identifier[0] = '\0';
    return ctx;
}

tStored_pixelmap* NewStoredPixelmap() {
    tStored_pixelmap* ctx = malloc(sizeof(tStored_pixelmap));
    ctx->id = 0;
    return ctx;
}