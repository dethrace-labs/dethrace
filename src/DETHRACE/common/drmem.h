#ifndef _DRMEM_H_
#define _DRMEM_H_

#include "br_types.h"
#include "dr_types.h"

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
