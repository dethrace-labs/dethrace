#include "brexcept.h"

#include "CORE/FW/resource.h"
#include "CORE/FW/diag.h"
#include "harness/trace.h"

#include <string.h>

void* exceptionValue;
br_exception_handler* _BrExceptionHandler;

// IDA: br_exception_handler* __cdecl _BrExceptionBegin()
br_exception_handler* _BrExceptionBegin() {
    br_exception_handler* h;
    LOG_TRACE("()");

    h = BrResAllocate(NULL, sizeof(br_exception_handler), BR_MEMORY_EXCEPTION_HANDLER);
    h->prev = _BrExceptionHandler;
    _BrExceptionHandler = h;
    return h;
}

// IDA: void __cdecl _BrExceptionEnd()
void _BrExceptionEnd() {
    br_exception_handler* old;
    LOG_TRACE("()");

    if (_BrExceptionHandler == NULL) {
        BrFailure("Unhandled exception: %d");
    }
    old = _BrExceptionHandler;
    _BrExceptionHandler = _BrExceptionHandler->prev;
    BrResFree(old);
}

// IDA: void __cdecl _BrExceptionThrow(br_int_32 type, void *value)
void _BrExceptionThrow(br_int_32 type, void* value) {
    br_exception_handler h;
    br_exception_handler* old;
    LOG_TRACE("(%d, %p)", type, value);

    if (_BrExceptionHandler == NULL) {
        BrFailure("Unhandled exception: %d");
    }
    memcpy(&h, _BrExceptionHandler, sizeof(br_exception_handler));
    old = _BrExceptionHandler;
    _BrExceptionHandler = _BrExceptionHandler->prev;
    BrResFree(old);
    exceptionValue = value;
    longjmp(h.context, type);
}

// IDA: br_exception __cdecl _BrExceptionValueFetch(br_exception type, void **evp)
br_exception _BrExceptionValueFetch(br_exception type, void** evp) {
    LOG_TRACE("(%d, %p)", type, evp);

    if (type != 0 && evp != NULL) {
        *evp = exceptionValue;
    }
    return type;
}

// IDA: void* __cdecl _BrExceptionResource()
void* _BrExceptionResource() {
    LOG_TRACE("()");

    return _BrExceptionHandler;
}
