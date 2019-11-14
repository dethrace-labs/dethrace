#include "mem.h"

char rscid[45];

// Offset: 14
// Size: 153
void* BrMemAllocate(br_size_t size, br_uint_8 type) {
    void *b;
}

// Offset: 177
// Size: 106
void BrMemFree(void *block) {
}

// Offset: 296
// Size: 131
br_size_t BrMemInquire(br_uint_8 type) {
    br_size_t i;
}

// Offset: 438
// Size: 116
br_int_32 BrMemAlign(br_uint_8 type) {
    br_int_32 i;
}

// Offset: 566
// Size: 161
void* BrMemCalloc(int nelems, br_size_t size, br_uint_8 type) {
    void *b;
}

// Offset: 739
// Size: 120
char* BrMemStrDup(char *str) {
    int l;
    char *nstr;
}

