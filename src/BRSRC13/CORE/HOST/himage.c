#include "himage.h"
#include "harness.h"

void* HostImageLoad(char* name) {
    STUB();
}

void HostImageUnload(void* image) {
    NOT_IMPLEMENTED();
}

void* HostImageLookupName(void* img, char* name, br_uint_32 hint) {
    NOT_IMPLEMENTED();
}

void* HostImageLookupOrdinal(void* img, br_uint_32 ordinal) {
    NOT_IMPLEMENTED();
}
