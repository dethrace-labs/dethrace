#include "brstddiag.h"
#include "harness/trace.h"

br_diaghandler BrStdioDiagHandler;
br_diaghandler* _BrDefaultDiagHandler = NULL;

void BrStdioWarning(char* message) {
    NOT_IMPLEMENTED();
}

void BrStdioFailure(char* message) {
    NOT_IMPLEMENTED();
}
