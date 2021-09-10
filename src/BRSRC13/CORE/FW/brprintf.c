#include "brprintf.h"
#include "harness_trace.h"

#include <stdarg.h>

br_int_32 BrSprintf(char* buf, char* fmt, ...) {
    int n;
    va_list args;
    NOT_IMPLEMENTED();
}

br_int_32 BrSprintfN(char* buf, br_size_t buf_size, char* fmt, ...) {
    int n;
    va_list args;
    NOT_IMPLEMENTED();
}

int BrLogPrintf(char* fmt, ...) {
    int n;
    va_list args;
    NOT_IMPLEMENTED();
}

br_int_32 BrSScanf(char* str, char* fmt, ...) {
    int n;
    va_list args;
    NOT_IMPLEMENTED();
}
