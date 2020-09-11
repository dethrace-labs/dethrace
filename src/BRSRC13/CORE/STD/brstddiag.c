#include "brstddiag.h"
#include "harness.h"

br_diaghandler BrStdioDiagHandler;
br_diaghandler* _BrDefaultDiagHandler;
char rscid[49];

void BrStdioWarning(char* message) {
    NOT_IMPLEMENTED();
}

void BrStdioFailure(char* message) {
    NOT_IMPLEMENTED();
}
