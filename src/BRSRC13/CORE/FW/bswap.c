#include "bswap.h"
#include "debug.h"

char rscid[47];

// Offset: 9
// Size: 81
br_uint_32 BrSwap32(br_uint_32 l) {
    // union {
    //     unsigned long l;
    //     unsigned char c[4];
    // } u;
    return ((l << 24) | ((l << 8) & 0x00FF0000) | ((l >> 8) & 0x0000FF00) | (l >> 24));
    NOT_IMPLEMENTED();
}

// Offset: 99
// Size: 64
br_uint_16 BrSwap16(br_uint_16 s) {
    struct {
        unsigned short s;
        unsigned char c[2];
    } u;
    NOT_IMPLEMENTED();
}

// Offset: 175
// Size: 92
br_float BrSwapFloat(br_float f) {
    struct {
        br_float f;
        unsigned char c[4];
    } u;
    NOT_IMPLEMENTED();
}

// Offset: 279
// Size: 781
void* BrSwapBlock(void* block, int count, int size) {
    br_uint_8* cp;
    int i;
    int k;
    NOT_IMPLEMENTED();
}
