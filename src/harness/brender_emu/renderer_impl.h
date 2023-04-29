#ifndef HARNESS_BRENDER_RENDERER_DISPATCH
#define HARNESS_BRENDER_RENDERER_DISPATCH

#include "brender/br_types.h"

#define MAX_STATE_STACK 32

struct state_matrix {
    br_matrix34 model_to_view;
    br_matrix4 view_to_screen;
    br_matrix34 view_to_environment;

    br_scalar hither_z;
    br_scalar yon_z;
};

typedef struct state_all {
    struct br_renderer* renderer;
    struct state_matrix matrix;

} state_all;

// Same structure as br_renderer_state
typedef struct harness_br_renderer {
    struct br_renderer_dispatch* dispatch;
    char* identifier;
    struct br_device* device;

    void* object_list;
    struct state_all state;
    struct state_all* default_state;
    struct br_renderer_state_stored* last_restored;
    struct br_primitive_library* plib;

    struct state_all state_stack[MAX_STATE_STACK];
    br_uint_32 stack_top;
    br_renderer_facility* renderer_facility;

} harness_br_renderer;

harness_br_renderer* NewHarnessBrRenderer(void);

#endif
