#include "renderer_state.h"
#include "harness/trace.h"
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
