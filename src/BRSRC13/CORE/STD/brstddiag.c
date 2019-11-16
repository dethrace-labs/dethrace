#include "brstddiag.h"

br_diaghandler BrStdioDiagHandler;
br_diaghandler *_BrDefaultDiagHandler;
char rscid[49];

// Offset: 15
// Size: 78
void BrStdioWarning(char *message) {
}

// Offset: 108
// Size: 93
void BrStdioFailure(char *message) {
}

