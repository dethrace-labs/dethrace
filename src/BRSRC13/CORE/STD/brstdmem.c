#include "brstdmem.h"
#include "harness_trace.h"

#include <stdlib.h>

// Global variables
char rscid[48];
br_allocator BrStdlibAllocator = { "malloc", BrStdlibAllocate, BrStdlibFree, BrStdlibInquire, BrStdlibAlign };
br_allocator* _BrDefaultAllocator = &BrStdlibAllocator;

void* BrStdlibAllocate(br_size_t size, br_uint_8 type) {
    void* m = malloc(size);

    if (m == NULL) {
        /* TODO BrFailure(); call*/
    }

    return m;
}

void BrStdlibFree(void* mem) {
    free(mem);
}

br_size_t BrStdlibInquire(br_uint_8 type) {
    return 0;
}

br_uint_32 BrStdlibAlign(br_uint_8 type) {
    return sizeof(void*);
}
