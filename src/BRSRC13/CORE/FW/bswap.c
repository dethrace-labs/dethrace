#include "bswap.h"
#include "harness/trace.h"

#define SWAP(A, B) do { (A) ^= (B); (B) ^= (A); (A) ^= (B); } while (0)


// IDA: br_uint_32 __cdecl BrSwap32(br_uint_32 l)
br_uint_32 BrSwap32(br_uint_32 l) {
    union { // size: 0x4
        unsigned long l; // @0x0
        unsigned char c[4]; // @0x0
    } u;
    LOG_TRACE10("(%d)", l);

    u.l = l;
    SWAP(u.c[0], u.c[3]);
    SWAP(u.c[1], u.c[2]);
    return u.l;
}

// IDA: br_uint_16 __cdecl BrSwap16(br_uint_16 s)
br_uint_16 BrSwap16(br_uint_16 s) {
    union { // size: 0x2
        unsigned short s; // @0x0
        unsigned char c[2]; // @0x0
    } u;
    LOG_TRACE("(%d)", s);

    u.s = s;
    SWAP(u.c[0], u.c[1]);
    return u.s;
}

// IDA: br_float __cdecl BrSwapFloat(br_float f)
br_float BrSwapFloat(br_float f) {
    union { // size: 0x4
        br_float f; // @0x0
        unsigned char c[4]; // @0x0
    } u;
    LOG_TRACE("(%f)", f);

    u.f = f;
    SWAP(u.c[0], u.c[3]);
    SWAP(u.c[1], u.c[2]);
    return u.f;
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
            SWAP(cp[0], cp[1]);
            cp += 2;
        }
        break;
    case 4:
        for (i = 0; i < count; i++) {
            SWAP(cp[0], cp[3]);
            SWAP(cp[1], cp[2]);
            cp += 4;
        }
        break;

    default:
        for (i = 0; i < count; i++) {
            for (k = 0; k < size / 2; k++) {
                SWAP(cp[k], cp[size - k - 1]);
            }
            cp += size;
        }
        break;
    }
    return block;
}
