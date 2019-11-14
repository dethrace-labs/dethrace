#ifndef _SCRATCH_H_
#define _SCRATCH_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 18
// Size: 153
void* BrScratchAllocate(br_size_t size);

// Offset: 185
// Size: 72
void BrScratchFree(void *scratch);

// Offset: 272
// Size: 96
void BrScratchFlush();

// Offset: 385
// Size: 41
br_size_t BrScratchInquire();

// Offset: 442
// Size: 40
char* BrScratchString();

// Offset: 502
// Size: 40
br_size_t BrScratchStringSize();

#endif
