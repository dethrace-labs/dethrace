#ifndef _SCRATCH_H_
#define _SCRATCH_H_

#include "brender/br_types.h"

void* BrScratchAllocate(br_size_t size);

void BrScratchFree(void* scratch);

void BrScratchFlush();

br_size_t BrScratchInquire();

char* BrScratchString();

br_size_t BrScratchStringSize();

#endif
