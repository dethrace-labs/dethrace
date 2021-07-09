#include "renderer_state.h"
#include "harness_trace.h"
#include <stddef.h>

extern void* BrResAllocate(void* vparent, br_size_t size, br_uint_8 res_class);
extern void BrMatrix34Mul(br_matrix34* A, br_matrix34* B, br_matrix34* C);

struct state_all default_state = {
    NULL,
    { { { BR_VECTOR3(1, 0, 0), /* model_to_view */
          BR_VECTOR3(0, 1, 0),
          BR_VECTOR3(0, 0, 1),
          BR_VECTOR3(0, 0, 0) } },

        { { BR_VECTOR4(1, 0, 0, 0), /* view_to_screen */
            BR_VECTOR4(0, 1, 0, 0),
            BR_VECTOR4(0, 0, 1, 0),
            BR_VECTOR4(0, 0, 0, 1) } },

        { { BR_VECTOR3(1, 0, 0), /* view_to_environment */
            BR_VECTOR3(0, 1, 0),
            BR_VECTOR3(0, 0, 1),
            BR_VECTOR3(0, 0, 0) } },
        0,
        0 },
};

static struct br_renderer_dispatch dispatch_table;

br_error StateCopy(struct state_all* dest, struct state_all* src, br_uint_32 copy_mask, void* res) {
    if (copy_mask & BR_STATE_MATRIX) {
        dest->matrix = src->matrix;
    }
    return 0;
}

br_error modelMulF(struct br_renderer* r, br_matrix34_f* m) {
    harness_br_renderer* self = (harness_br_renderer*)r;
    br_matrix34 om = self->state.matrix.model_to_view;

    BrMatrix34Mul(&self->state.matrix.model_to_view, (br_matrix34*)m, &om);

    //TouchModelToView(self);
    return 0;
}

br_error statePush(struct br_renderer* r, br_uint_32 mask) {
    harness_br_renderer* self = (harness_br_renderer*)r;
    struct state_all* sp;

    if (self->stack_top >= MAX_STATE_STACK) {
        LOG_PANIC("state stack overflow!");
    }

    sp = self->state_stack + self->stack_top;
    self->stack_top++;
    return StateCopy(sp, &self->state, mask, self);
}

br_error statePop(struct br_renderer* r, br_uint_32 mask) {
    harness_br_renderer* self = (harness_br_renderer*)r;
    br_error result;
    struct state_all* sp;

    if (self->stack_top <= 0) {
        LOG_PANIC("state stack underflow!");
    }

    self->stack_top--;
    sp = self->state_stack + self->stack_top;
    result = StateCopy(&self->state, sp, mask, self);
    return result;
}

br_error stateDefault(struct br_renderer* r, br_uint_32 mask) {
    harness_br_renderer* self = (harness_br_renderer*)r;
    return StateCopy(&self->state, self->default_state, mask, self);
}

harness_br_renderer* NewRendererState() {

    harness_br_renderer* self;

    self = BrResAllocate(NULL, sizeof(*self), BR_MEMORY_OBJECT);

    if (self == NULL) {
        return NULL;
    }

    dispatch_table._modelMulF = &modelMulF;
    dispatch_table._statePush = &statePush;
    dispatch_table._statePop = &statePop;

    self->default_state = &default_state;
    stateDefault((br_renderer*)self, BR_STATE_ALL);

    self->dispatch = &dispatch_table;
    self->identifier = "harness_renderer_state";
    return self;
    //self->device = device;
    //self->renderer_facility = facility;
}

#define BR_CMETHOD_REF(t, m) ((void*)_M_##t##_##m)

//static struct br_renderer_dispatch rendererDispatch; // = {
//     BR_CMETHOD_REF(br_renderer_d3d, sceneBegin),
//     BR_CMETHOD_REF(br_renderer_d3d, sceneEnd),
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,
//     NULL,

//     BR_CMETHOD_REF(br_renderer_d3d, validDestination),
//     BR_CMETHOD_REF(br_renderer_d3d, stateStoredNew),
//     BR_CMETHOD_REF(br_renderer_d3d, stateStoredAvail),
//     BR_CMETHOD_REF(br_renderer_d3d, bufferStoredNew),
//     BR_CMETHOD_REF(br_renderer_d3d, bufferStoredAvail),
//     BR_CMETHOD_REF(br_renderer_d3d, partSet),
//     BR_CMETHOD_REF(br_renderer_d3d, partSetMany),
//     BR_CMETHOD_REF(br_renderer_d3d, partQuery),
//     BR_CMETHOD_REF(br_renderer_d3d, partQueryBuffer),
//     BR_CMETHOD_REF(br_renderer_d3d, partQueryMany),
//     BR_CMETHOD_REF(br_renderer_d3d, partQueryManySize),
//     BR_CMETHOD_REF(br_renderer_d3d, partQueryAll),
//     BR_CMETHOD_REF(br_renderer_d3d, partQueryAllSize),
//     BR_CMETHOD_REF(br_renderer_d3d, partIndexQuery),
//     BR_CMETHOD_REF(br_renderer_d3d, modelMulF),
//     BR_CMETHOD_REF(br_renderer_d3d, modelMulX),
//     BR_CMETHOD_REF(br_renderer_d3d, modelPopPushMulF),
//     BR_CMETHOD_REF(br_renderer_d3d, modelPopPushMulX),
//     BR_CMETHOD_REF(br_renderer_d3d, modelInvert),
//     BR_CMETHOD_REF(br_renderer_d3d, statePush),
//     BR_CMETHOD_REF(br_renderer_d3d, statePop),
//     BR_CMETHOD_REF(br_renderer_d3d, stateSave),
//     BR_CMETHOD_REF(br_renderer_d3d, stateRestore),
//     BR_CMETHOD_REF(br_renderer_d3d, stateMask),
//     BR_CMETHOD_REF(br_renderer_d3d, stateDefault),
//     BR_CMETHOD_REF(br_renderer_d3d, boundsTestF),
//     BR_CMETHOD_REF(br_renderer_d3d, boundsTestX),
//     BR_CMETHOD_REF(br_renderer_d3d, coverageTestF),
//     BR_CMETHOD_REF(br_renderer_d3d, coverageTestX),
//     BR_CMETHOD_REF(br_renderer_d3d, viewDistanceF),
//     BR_CMETHOD_REF(br_renderer_d3d, viewDistanceX),
//     BR_CMETHOD_REF(br_renderer_d3d, commandModeSet),
//     BR_CMETHOD_REF(br_renderer_d3d, commandModeQuery),
//     BR_CMETHOD_REF(br_renderer_d3d, commandModeDefault),
//     BR_CMETHOD_REF(br_renderer_d3d, commandModePush),
//     BR_CMETHOD_REF(br_renderer_d3d, commandModePop),
//     BR_CMETHOD_REF(br_renderer_d3d, flush),
//     BR_CMETHOD_REF(br_renderer_d3d, synchronise),
//     BR_CMETHOD_REF(br_renderer_d3d, partQueryCapability),
//     BR_CMETHOD_REF(br_renderer_d3d, stateQueryPerformance),
// };