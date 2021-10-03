#ifndef _DRMEM_H_
#define _DRMEM_H_

#include "brender/br_types.h"
#include "dr_types.h"

extern br_allocator gAllocator;
extern int gNon_fatal_allocation_errors;
extern char* gMem_names[247];
extern br_resource_class gStainless_classes[118];

void SetNonFatalAllocationErrors();

void ResetNonFatalAllocationErrors();

int AllocationErrorsAreFatal();

void MAMSInitMem();

void PrintMemoryDump(int pFlags, char* pTitle);

void* DRStdlibAllocate(br_size_t size, br_uint_8 type);

void DRStdlibFree(void* mem);

br_size_t DRStdlibInquire(br_uint_8 type);

br_uint_32 Claim4ByteAlignment(br_uint_8 type);

void InstallDRMemCalls();

void MAMSUnlock(void** pPtr);

void MAMSLock(void** pPtr);

void CreateStainlessClasses();

void CheckMemory();

#endif
