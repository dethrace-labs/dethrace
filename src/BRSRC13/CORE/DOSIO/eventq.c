#include "eventq.h"
#include "harness/trace.h"

int _DOSEventQEnabled;
host_real_memory _DOSEventMemory;

// IDA: br_error __cdecl DOSEventBegin()
br_error DOSEventBegin(void) {
    host_info hi;
    int s;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DOSEventEnd()
void DOSEventEnd(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_boolean __cdecl DOSEventWait(dosio_event *event, br_boolean block)
br_boolean DOSEventWait(dosio_event* event, br_boolean block) {
    int t;
    LOG_TRACE("(%p, %d)", event, block);
    NOT_IMPLEMENTED();
}
