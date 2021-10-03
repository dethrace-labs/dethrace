#include "hook.h"
#include "harness/trace.h"

// IDA: br_error __cdecl HostInterruptHook(host_interrupt_hook *h, br_uint_8 vector, br_uint_32 off, br_uint_16 sel)
br_error HostInterruptHook(host_interrupt_hook* h, br_uint_8 vector, br_uint_32 off, br_uint_16 sel) {
    br_error r;
    LOG_TRACE("(%p, %d, %d, %d)", h, vector, off, sel);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl HostInterruptUnhook(host_interrupt_hook *h)
br_error HostInterruptUnhook(host_interrupt_hook* h) {
    br_error r;
    LOG_TRACE("(%p)", h);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl HostExceptionHook(host_exception_hook *h, br_uint_8 exception, br_uint_32 off, br_uint_16 sel)
br_error HostExceptionHook(host_exception_hook* h, br_uint_8 exception, br_uint_32 off, br_uint_16 sel) {
    br_error r;
    LOG_TRACE("(%p, %d, %d, %d)", h, exception, off, sel);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl HostExceptionUnhook(host_exception_hook *h)
br_error HostExceptionUnhook(host_exception_hook* h) {
    br_error r;
    LOG_TRACE("(%p)", h);
    NOT_IMPLEMENTED();
}
