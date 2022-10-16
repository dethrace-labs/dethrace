#ifndef _SCRATCH_H_
#define _SCRATCH_H_

#include "brender/br_types.h"

void* BrScratchAllocate(br_size_t size);

void BrScratchFree(void* scratch);

void BrScratchFlush(void);

br_size_t BrScratchInquire(void);

char* BrScratchString(void);

br_size_t BrScratchStringSize(void);

#endif
