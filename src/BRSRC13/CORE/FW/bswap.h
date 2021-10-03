#ifndef _BSWAP_H_
#define _BSWAP_H_

#include "brender/br_types.h"

br_uint_32 BrSwap32(br_uint_32 l);

br_uint_16 BrSwap16(br_uint_16 s);

br_float BrSwapFloat(br_float f);

void* BrSwapBlock(void* block, int count, int size);

#endif
