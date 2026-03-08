#include "resource.h"

// FUNCTION: CARM95 0x0049CC05
void* S3MemAllocate(br_size_t size, br_uint_8 type) {
    return BrMemAllocate(size, type);
}

// FUNCTION: CARM95 0x0049CC2E
void S3MemFree(void* p) {
    BrMemFree(p);
}
