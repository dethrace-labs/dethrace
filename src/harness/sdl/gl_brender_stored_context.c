#include "gl_brender_stored_context.h"
#include "../include/harness/trace.h"
#include <stdlib.h>

void _free(br_object* o) {
    tStored_model_context* ctx = (tStored_model_context*)o;
    LOG_DEBUG("free called!");

    glDeleteVertexArrays(1, &ctx->vao_id);
    glDeleteBuffers(1, &ctx->ebo_id);
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