#include "harness.h"

#include <stdarg.h>
#include <stdio.h>

void debug_printf(const char* fmt, const char* fn, const char* fmt2, ...) {
    va_list ap;

    fprintf(stderr, fmt, fn);

    va_start(ap, fmt2);
    vfprintf(stderr, fmt2, ap);
    va_end(ap);

    fputs("\033[0m\n", stderr);
}