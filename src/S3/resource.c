#include "resource.h"

void* S3MemAllocate(br_size_t size, br_uint_8 type) {
    return BrMemAllocate(size, type);
}

void S3MemFree(void* p) {
    BrMemFree(p);
}