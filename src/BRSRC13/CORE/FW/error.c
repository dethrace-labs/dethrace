#include "error.h"
#include "harness/trace.h"

// IDA: br_error __cdecl BrLastErrorGet(void **valuep)
br_error BrLastErrorGet(void** valuep) {
    LOG_TRACE("(%p)", valuep);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrLastErrorSet(br_error type, void *value)
void BrLastErrorSet(br_error type, void* value) {
    LOG_TRACE("(%d, %p)", type, value);
    NOT_IMPLEMENTED();
}
