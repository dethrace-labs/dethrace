#include "brexcept.h"
#include "harness.h"

void* exceptionValue;
br_exception_handler* _BrExceptionHandler;
char rscid[50];

// Offset: 18
// Size: 81
br_exception_handler* _BrExceptionBegin() {
    NOT_IMPLEMENTED();
}

// Offset: 115
// Size: 85
void _BrExceptionEnd() {
    NOT_IMPLEMENTED();
}

// Offset: 218
// Size: 107
void _BrExceptionThrow(br_int_32 type, void* value) {
    br_exception_handler h;
    br_exception_handler* old;
    NOT_IMPLEMENTED();
}

// Offset: 348
// Size: 64
br_exception _BrExceptionValueFetch(br_exception type, void** evp) {
    NOT_IMPLEMENTED();
}

// Offset: 433
// Size: 41
void* _BrExceptionResource() {
    NOT_IMPLEMENTED();
}
