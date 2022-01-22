#include "brstddiag.h"
#include "CORE/FW/brbegin.h"

#include "harness/trace.h"

#include <stdio.h>

br_diaghandler BrStdioDiagHandler = {
    "Stdio DiagHandler",
    BrStdioWarning,
    BrStdioFailure,
};

br_diaghandler* _BrDefaultDiagHandler = &BrStdioDiagHandler;

void BrStdioWarning(char* message) {
    fflush(stdout);
    fputs(message, stderr);
    fputc('\n', stderr);
    fflush(stderr);
}

void BrStdioFailure(char* message) {
    LOG_TRACE("(%s)", message);

#if 0
    // FIXME: 'real' implementation ends BRender
    BrEnd();
#endif
    fflush(stdout);
    fputs(message, stderr);
    fputc('\n', stderr);
    fflush(stderr);
#if 0
    // FIXME: 'real' implementation unconditionally exits.
    exit(10);
#else
    abort();
#endif
}
