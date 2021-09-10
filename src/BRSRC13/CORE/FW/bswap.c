#include "bswap.h"
#include "harness_trace.h"


// IDA: br_uint_32 __cdecl BrSwap32(br_uint_32 l)
br_uint_32 BrSwap32(br_uint_32 l) {
    // struct { // size: 0x4
    //     unsigned long l; // @0x0
    //     unsigned char c[4]; // @0x0
    // } u;
    LOG_TRACE10("(%d)", l);
    return ((l << 24) | ((l << 8) & 0x00FF0000) | ((l >> 8) & 0x0000FF00) | (l >> 24));
}

// IDA: br_uint_16 __cdecl BrSwap16(br_uint_16 s)
br_uint_16 BrSwap16(br_uint_16 s) {
    struct { // size: 0x2
        unsigned short s; // @0x0
        unsigned char c[2]; // @0x0
    } u;
    LOG_TRACE("(%d)", s);
    NOT_IMPLEMENTED();
}

// IDA: br_float __cdecl BrSwapFloat(br_float f)
br_float BrSwapFloat(br_float f) {
    struct { // size: 0x4
        br_float f; // @0x0
        unsigned char c[4]; // @0x0
    } u;
    LOG_TRACE("(%f)", f);
    NOT_IMPLEMENTED();
}

// IDA: void* __cdecl BrSwapBlock(void *block, int count, int size)
void* BrSwapBlock(void* block, int count, int size) {
    br_uint_8* cp;
    int i;
    int k;
    LOG_TRACE9("(%p, %d, %d)", block, count, size);

    cp = (br_uint_8*)block;
    switch (size) {
    case 1:
        break;
    case 2:
        for (i = 0; i < count; i++) {
            k = cp[1];
            cp[1] = cp[0];
            cp[0] = k;
            cp += 2;
        }
        break;
    case 4:
        for (i = 0; i < count; i++) {
            k = cp[0];
            cp[0] = cp[3];
            cp[3] = k;
            k = cp[1];
            cp[1] = cp[2];
            cp[2] = k;
            cp += 4;
        }
        break;

    default:
        LOG_PANIC("size not implemented");
    }
    return block;
}
