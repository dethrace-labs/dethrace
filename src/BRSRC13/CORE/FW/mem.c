#include "mem.h"

#include "CORE/FW/fwsetup.h"
#include "CORE/STD/brstdlib.h"
#include "harness/trace.h"

// IDA: void* __cdecl BrMemAllocate(br_size_t size, br_uint_8 type)
void* BrMemAllocate(br_size_t size, br_uint_8 type) {
    void* b;
    LOG_TRACE10("(%d, %d)", size, type);

    b = fw.mem->allocate(size, type);
    BrMemSet(b, 0, size);
    return b;
}

// IDA: void __cdecl BrMemFree(void *block)
void BrMemFree(void* block) {
    LOG_TRACE10("(%p)", block);
    fw.mem->free(block);
}

// IDA: br_size_t __cdecl BrMemInquire(br_uint_8 type)
br_size_t BrMemInquire(br_uint_8 type) {
    br_size_t i;
    i = fw.mem->inquire(type);
    return i;
}

// IDA: br_int_32 __cdecl BrMemAlign(br_uint_8 type)
br_int_32 BrMemAlign(br_uint_8 type) {
    br_int_32 i = 0;
    if (fw.mem->align) {
        i = fw.mem->align(type);
    }
    return i;
}

// IDA: void* __cdecl BrMemCalloc(int nelems, br_size_t size, br_uint_8 type)
void* BrMemCalloc(int nelems, br_size_t size, br_uint_8 type) {
    void* b;
    b = fw.mem->allocate(nelems * size, type);
    BrMemSet(b, 0, nelems * size);
    return b;
}

// IDA: char* __cdecl BrMemStrDup(char *str)
char* BrMemStrDup(char* str) {
    int l;
    char* nstr;
    LOG_TRACE("(\"%s\")", str);
    NOT_IMPLEMENTED();
}
