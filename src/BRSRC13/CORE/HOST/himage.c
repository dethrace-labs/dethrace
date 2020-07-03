#include "himage.h"
#include "harness.h"

// Offset: 14
// Size: 40
void* HostImageLoad(char* name) {
    LOG_WARN("Not implemented");
}

// Offset: 70
// Size: 30
void HostImageUnload(void* image) {
    NOT_IMPLEMENTED();
}

// Offset: 120
// Size: 40
void* HostImageLookupName(void* img, char* name, br_uint_32 hint) {
    NOT_IMPLEMENTED();
}

// Offset: 183
// Size: 40
void* HostImageLookupOrdinal(void* img, br_uint_32 ordinal) {
    NOT_IMPLEMENTED();
}
