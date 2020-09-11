#include "mem.h"

#include "CORE/FW/fwsetup.h"
#include "CORE/STD/brstdlib.h"
#include "harness.h"

char rscid[45];

void* BrMemAllocate(br_size_t size, br_uint_8 type) {
    void* b;
    LOG_TRACE10("(%d, %d)", size, type);

    b = fw.mem->allocate(size, type);
    BrMemSet(b, 0, size);
    return b;
}

void BrMemFree(void* block) {
    LOG_TRACE10("(%p)", block);
    fw.mem->free(block);
}

br_size_t BrMemInquire(br_uint_8 type) {
    br_size_t i;
    i = fw.mem->inquire(type);
    return i;
}

br_int_32 BrMemAlign(br_uint_8 type) {
    br_int_32 i = 0;
    if (fw.mem->align) {
        i = fw.mem->align(type);
    }
    return i;
}

void* BrMemCalloc(int nelems, br_size_t size, br_uint_8 type) {
    void* b;
    b = fw.mem->allocate(nelems * size, type);
    BrMemSet(b, 0, nelems * size);
    return b;
}

char* BrMemStrDup(char* str) {
    int l;
    char* nstr;
    NOT_IMPLEMENTED();
}
