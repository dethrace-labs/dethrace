#include "mem.h"

#include "CORE/FW/fwsetup.h"
#include "CORE/STD/brstdlib.h"
#include "debug.h"

char rscid[45];

// Offset: 14
// Size: 153
void* BrMemAllocate(br_size_t size, br_uint_8 type) {
    void* b;
    LOG_TRACE("(%d, %d)", size, type);

    b = fw.mem->allocate(size, type);
    BrMemSet(b, 0, size);
    return b;
}

// Offset: 177
// Size: 106
void BrMemFree(void* block) {
    fw.mem->free(block);
}

// Offset: 296
// Size: 131
br_size_t BrMemInquire(br_uint_8 type) {
    br_size_t i;
    i = fw.mem->inquire(type);
    return i;
}

// Offset: 438
// Size: 116
br_int_32 BrMemAlign(br_uint_8 type) {
    br_int_32 i = 0;
    if (fw.mem->align) {
        i = fw.mem->align(type);
    }
    return i;
}

// Offset: 566
// Size: 161
void* BrMemCalloc(int nelems, br_size_t size, br_uint_8 type) {
    void* b;
    b = fw.mem->allocate(nelems * size, type);
    BrMemSet(b, 0, nelems * size);
    return b;
}

// Offset: 739
// Size: 120
char* BrMemStrDup(char* str) {
    int l;
    char* nstr;
}
