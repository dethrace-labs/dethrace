#include "diag.h"

char _diag_scratch[128];
char rscid[49];

// Offset: 10
// Size: 118
void BrFailure(char *s, ...) {
    va_list args;
    char failure_header[10];
}

// Offset: 138
// Size: 118
void BrWarning(char *s, ...) {
    va_list args;
    char warning_header[10];
}

// Offset: 264
// Size: 132
void BrFatal(char *name, int line, char *s, ...) {
    va_list args;
    int n;
}

// Offset: 406
// Size: 95
void _BrAssert(char *condition, char *file, unsigned int line) {
}

// Offset: 512
// Size: 95
void _BrUAssert(char *condition, char *file, unsigned int line) {
}

