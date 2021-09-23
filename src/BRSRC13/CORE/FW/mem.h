#ifndef _MEM_H_
#define _MEM_H_

#include "brender/br_types.h"

void* BrMemAllocate(br_size_t size, br_uint_8 type);

void BrMemFree(void* block);

br_size_t BrMemInquire(br_uint_8 type);

br_int_32 BrMemAlign(br_uint_8 type);

void* BrMemCalloc(int nelems, br_size_t size, br_uint_8 type);

char* BrMemStrDup(char* str);

#endif
