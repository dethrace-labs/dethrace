#include "brexcept.h"
#include "harness.h"

void* exceptionValue;
br_exception_handler* _BrExceptionHandler;
char rscid[50];

br_exception_handler* _BrExceptionBegin() {
    NOT_IMPLEMENTED();
}

void _BrExceptionEnd() {
    NOT_IMPLEMENTED();
}

void _BrExceptionThrow(br_int_32 type, void* value) {
    br_exception_handler h;
    br_exception_handler* old;
    NOT_IMPLEMENTED();
}

br_exception _BrExceptionValueFetch(br_exception type, void** evp) {
    NOT_IMPLEMENTED();
}

void* _BrExceptionResource() {
    NOT_IMPLEMENTED();
}
