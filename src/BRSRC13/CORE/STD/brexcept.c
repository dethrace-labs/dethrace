#include "brexcept.h"
#include "harness_trace.h"

void* exceptionValue;
br_exception_handler* _BrExceptionHandler;
char rscid[50];

// IDA: br_exception_handler* __cdecl _BrExceptionBegin()
br_exception_handler* _BrExceptionBegin() {
    br_exception_handler* h;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl _BrExceptionEnd()
void _BrExceptionEnd() {
    br_exception_handler* old;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl _BrExceptionThrow(br_int_32 type, void *value)
void _BrExceptionThrow(br_int_32 type, void* value) {
    br_exception_handler h;
    br_exception_handler* old;
    LOG_TRACE("(%d, %p)", type, value);
    NOT_IMPLEMENTED();
}

// IDA: br_exception __cdecl _BrExceptionValueFetch(br_exception type, void **evp)
br_exception _BrExceptionValueFetch(br_exception type, void** evp) {
    LOG_TRACE("(%d, %p)", type, evp);
    NOT_IMPLEMENTED();
}

// IDA: void* __cdecl _BrExceptionResource()
void* _BrExceptionResource() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
