#include "stdmem.h"

#include <stdlib.h>

// Global variables
char rscid[48];
br_allocator BrStdlibAllocator = {"malloc", BrStdlibAllocate, BrStdlibFree, BrStdlibInquire, BrStdlibAlign};
br_allocator *_BrDefaultAllocator = &BrStdlibAllocator;

// Offset: 17
// Size: 80
void* BrStdlibAllocate(br_size_t size, br_uint_8 type) {
    void *m = malloc(size);

    if (m == NULL) {
        /* TODO BrFailure(); call*/
    }

    return m;
}

// Offset: 110
// Size: 38
void BrStdlibFree(void *mem) {
    free(mem);
}

// Offset: 164
// Size: 40
br_size_t BrStdlibInquire(br_uint_8 type) {
    return 0;
}

// Offset: 218
// Size: 40
br_uint_32 BrStdlibAlign(br_uint_8 type) {
    return sizeof(void *);
}

