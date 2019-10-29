#include "stdmem.h"

// Global variables
char rscid[48];
br_allocator BrStdlibAllocator;
br_allocator *_BrDefaultAllocator;

// Offset: 17
// Size: 80
void* BrStdlibAllocate(br_size_t size, br_uint_8 type) {
  void *m;
}

// Offset: 110
// Size: 38
void BrStdlibFree(void *mem) {
}

// Offset: 164
// Size: 40
br_size_t BrStdlibInquire(br_uint_8 type) {
}

// Offset: 218
// Size: 40
br_uint_32 BrStdlibAlign(br_uint_8 type) {
}

