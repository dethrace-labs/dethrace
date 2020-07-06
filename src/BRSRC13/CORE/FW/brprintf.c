#include "brprintf.h"
#include "harness.h"

char rscid[48];

#include <stdarg.h>

// Offset: 10
// Size: 75
br_int_32 BrSprintf(char* buf, char* fmt, ...) {
    int n;
    va_list args;
    NOT_IMPLEMENTED();
}

// Offset: 96
// Size: 75
br_int_32 BrSprintfN(char* buf, br_size_t buf_size, char* fmt, ...) {
    int n;
    va_list args;
    NOT_IMPLEMENTED();
}

// Offset: 183
// Size: 97
int BrLogPrintf(char* fmt, ...) {
    int n;
    va_list args;
    NOT_IMPLEMENTED();
}

// Offset: 289
// Size: 75
br_int_32 BrSScanf(char* str, char* fmt, ...) {
    int n;
    va_list args;
    NOT_IMPLEMENTED();
}
