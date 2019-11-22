#ifndef _BSWAP_H_
#define _BSWAP_H_

#include "br_types.h"

// Offset: 9
// Size: 81
br_uint_32 BrSwap32(br_uint_32 l);

// Offset: 99
// Size: 64
br_uint_16 BrSwap16(br_uint_16 s);

// Offset: 175
// Size: 92
br_float BrSwapFloat(br_float f);

// Offset: 279
// Size: 781
void* BrSwapBlock(void *block, int count, int size);

#endif
