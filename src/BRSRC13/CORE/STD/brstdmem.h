#ifndef _BRSTDMEM_H_
#define _BRSTDMEM_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 17
// Size: 80
void* BrStdlibAllocate(br_size_t size, br_uint_8 type);

// Offset: 110
// Size: 38
void BrStdlibFree(void *mem);

// Offset: 164
// Size: 40
br_size_t BrStdlibInquire(br_uint_8 type);

// Offset: 218
// Size: 40
br_uint_32 BrStdlibAlign(br_uint_8 type);

#endif
