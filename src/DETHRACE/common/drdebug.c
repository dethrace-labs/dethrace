#include "drdebug.h"

// These functions were compiled out of the release executable. Implementation is just a guess

void DrDebugLog(int unk, char *pStr) {
    // maybe this is actually "errors::dprintf"?
    printf("%d: %s\n", unk, pStr);
}

void DrDebugMessage(char *pStr, ...) {
    va_list args;
    va_start(args, pStr);
    vprintf(pStr, args);
    va_end(args);
}