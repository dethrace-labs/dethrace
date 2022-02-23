#include "brprintf.h"
#include "harness/trace.h"
#include "CORE/FW/scratch.h"
#include "CORE/STD/brstdlib.h"
#include "CORE/STD/logwrite.h"

#include <stdarg.h>
#include <stdio.h>

br_int_32 BrSprintf(char* buf, char* fmt, ...) {
    int n;
    va_list args;

    va_start(args, fmt);
    n = vsprintf(buf, fmt, args);
    va_end(args);
    return n;
}

br_int_32 BrSprintfN(char* buf, br_size_t buf_size, char* fmt, ...) {
    int n;
    va_list args;

    va_start(args, fmt);
    n = vsnprintf(buf, buf_size, fmt, args);
    va_end(args);
    return n;
}

int BrLogPrintf(char* fmt, ...) {
    int n;
    va_list args;

    va_start(args, fmt);
    n = BrVSprintf(BrScratchString(), fmt, args);
    va_end(args);
    BrLogWrite(BrScratchString(), 1, n);

    return n;
}

br_int_32 BrSScanf(char* str, char* fmt, ...) {
    int n;
    va_list args;

    va_start(args, fmt);
    n = vsscanf(str, fmt, args);
    va_end(args);
    return n;
}
