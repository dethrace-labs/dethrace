#include "diag.h"
#include "CORE/STD/brstdlib.h"
#include "brprintf.h"
#include "fwsetup.h"
#include "harness_trace.h"
#include <stdarg.h>

// Global variables
char _diag_scratch[128];

// IDA: void __cdecl BrFailure(char *s, ...)
void BrFailure(const char* s, ...) {
    va_list args;
    const char failure_header[10] = "Failure: ";
    LOG_TRACE("(\"%s\")", s);

    va_start(args, s);
    BrStrCpy(_diag_scratch, failure_header);
    BrVSprintf(&_diag_scratch[sizeof(failure_header) - 1], s, args);

    if (fw.diag->failure == NULL) {
        BrAbort();
    }

    fw.diag->failure(_diag_scratch);
    va_end(args);
}

// IDA: void __cdecl BrWarning(char *s, ...)
void BrWarning(const char* s, ...) {
    va_list args;
    const char warning_header[10] = "Warning: ";

    va_start(args, s);
    BrStrCpy(_diag_scratch, warning_header);
    BrVSprintf(&_diag_scratch[sizeof(warning_header) - 1], s, args);

    if (fw.diag->warning == NULL) {
        BrAbort();
    }

    fw.diag->warning(_diag_scratch);
    va_end(args);
}

// IDA: void __cdecl BrFatal(char *name, int line, char *s, ...)
void BrFatal(const char* name, int line, const char* s, ...) {
    va_list args;
    int n;

    va_start(args, s);
    n = BrSprintf(_diag_scratch, "FATAL %s:%d\n", name, line);
    BrVSprintf(&_diag_scratch[n], s, args);
    if (fw.diag->failure == NULL) {
        BrAbort();
    }

    fw.diag->failure(_diag_scratch);
    va_end(args);
}

// IDA: void __cdecl _BrAssert(char *condition, char *file, unsigned int line)
void _BrAssert(const char* condition, const char* file, unsigned int line) {
    if (fw.diag->failure == NULL) {
        BrAbort();
    }

    BrSprintf(_diag_scratch, "ASSERTION FAILED %s:%d: \"%s\"\n", file, line, condition);
    fw.diag->failure(_diag_scratch);
}

// IDA: void __cdecl _BrUAssert(char *condition, char *file, unsigned int line)
void _BrUAssert(const char* condition, const char* file, unsigned int line) {
    if (fw.diag->failure == NULL) {
        BrAbort();
    }

    BrSprintf(_diag_scratch, "ASSERTION FAILED %s:%d: \"%s\"\n", file, line, condition);
    fw.diag->failure(_diag_scratch);
}
