#ifndef _BRSTDMEM_H_
#define _BRSTDMEM_H_

#include "br_types.h"

extern br_allocator BrStdlibAllocator;
extern br_allocator* _BrDefaultAllocator;

void* BrStdlibAllocate(br_size_t size, br_uint_8 type);

void BrStdlibFree(void* mem);

br_size_t BrStdlibInquire(br_uint_8 type);

br_uint_32 BrStdlibAlign(br_uint_8 type);

#endif
