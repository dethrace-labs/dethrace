#include "bswap.h"

char rscid[47];

// Offset: 9
// Size: 81
br_uint_32 BrSwap32(br_uint_32 l) {
    struct {
        unsigned long l;
        unsigned char c[4];
    } u;
}

// Offset: 99
// Size: 64
br_uint_16 BrSwap16(br_uint_16 s) {
    struct {
        unsigned short s;
        unsigned char c[2];
    } u;
}

// Offset: 175
// Size: 92
br_float BrSwapFloat(br_float f) {
    struct {
        br_float f;
        unsigned char c[4];
    } u;
}

// Offset: 279
// Size: 781
void* BrSwapBlock(void* block, int count, int size) {
    br_uint_8* cp;
    int i;
    int k;
}
