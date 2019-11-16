#include "brprintf.h"

char rscid[48];

// Offset: 10
// Size: 75
br_int_32 BrSprintf(char *buf, char *fmt, ...) {
    int n;
    va_list args;
}

// Offset: 96
// Size: 75
br_int_32 BrSprintfN(char *buf, br_size_t buf_size, char *fmt, ...) {
    int n;
    va_list args;
}

// Offset: 183
// Size: 97
int BrLogPrintf(char *fmt, ...) {
    int n;
    va_list args;
}

// Offset: 289
// Size: 75
br_int_32 BrSScanf(char *str, char *fmt, ...) {
    int n;
    va_list args;
}

