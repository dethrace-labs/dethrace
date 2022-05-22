#include "resource.h"

void* S3MemAllocate(br_size_t size, br_uint_8 type) {
    return BrMemAllocate(size, type);
}

void S3MemFree(void* p) {
    BrMemFree(p);
}

void S3CopyBrVector(tS3_vector3* a1, br_vector3* a2) {
    a1->x = a2->v[0];
    a1->y = a2->v[1];
    a1->z = a2->v[2];
}