#include "hook.h"

// Offset: 18
// Size: 168
br_error HostInterruptHook(host_interrupt_hook* h, br_uint_8 vector, br_uint_32 off, br_uint_16 sel) {
    br_error r;
}

// Offset: 206
// Size: 124
br_error HostInterruptUnhook(host_interrupt_hook* h) {
    br_error r;
}

// Offset: 348
// Size: 168
br_error HostExceptionHook(host_exception_hook* h, br_uint_8 exception, br_uint_32 off, br_uint_16 sel) {
    br_error r;
}

// Offset: 536
// Size: 124
br_error HostExceptionUnhook(host_exception_hook* h) {
    br_error r;
}
