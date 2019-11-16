#include "diag.h"
#include "fwsetup.h"
#include "brprintf.h"
#include "brstdlib.h"
#include <stdarg.h>

// Global variables
char _diag_scratch[128];
char rscid[49];

// Offset: 10
// Size: 118
void BrFailure(const char *s, ...) {
    va_list args;
    const char failure_header[10] = "Failure: ";

    va_start(args, s);
    BrStrCpy(_diag_scratch, failure_header);
    BrVSprintf(&_diag_scratch[sizeof(failure_header) - 1], s, args);

    if (fw.err->error == NULL) {
        BrAbort();
    }

    fw.err->error(_diag_scratch);
    va_end(args);
}

// Offset: 138
// Size: 118
void BrWarning(const char *s, ...) {
    va_list args;
    const char warning_header[10] = "Warning: ";

    va_start(args, s);
    BrStrCpy(_diag_scratch, warning_header);
    BrVSprintf(&_diag_scratch[sizeof(warning_header) - 1], s, args);

    if (fw.err->message == NULL) {
        BrAbort();
    }

    fw.err->message(_diag_scratch);
    va_end(args);
}

// Offset: 264
// Size: 132
void BrFatal(const char *name, int line, const char *s, ...) {
    va_list args;
    int n;

    va_start(args, s);
    n = BrSprintf(_diag_scratch, "FATAL %s:%d\n", name, line);
    BrVSprintf(&_diag_scratch[n], s, args);
    if (fw.err->error == NULL) {
        BrAbort();
    }

    fw.err->error(_diag_scratch);
    va_end(args);
}

// Offset: 406
// Size: 95
void _BrAssert(const char *condition, const char *file, unsigned int line) {
    if (fw.err->error == NULL) {
        BrAbort();
    }

    BrSprintf(_diag_scratch, "ASSERTION FAILED %s:%d: \"%s\"\n", file, line, condition);
    fw.err->error(_diag_scratch);
}

// Offset: 512
// Size: 95
void _BrUAssert(const char *condition, const char *file, unsigned int line) {
    if (fw.err->error == NULL) {
        BrAbort();
    }

    BrSprintf(_diag_scratch, "ASSERTION FAILED %s:%d: \"%s\"\n", file, line, condition);
    fw.err->error(_diag_scratch);
}

