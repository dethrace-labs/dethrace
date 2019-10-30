#include "drmem.h"

// Global variables
br_resource_class gStainless_classes[118];
char *gMem_names[247];
int gNon_fatal_allocation_errors;
br_allocator gAllocator;

// Offset: 0
// Size: 44
void SetNonFatalAllocationErrors() {
}

// Offset: 44
// Size: 44
void ResetNonFatalAllocationErrors() {
}

// Offset: 88
// Size: 68
int AllocationErrorsAreFatal() {
}

// Offset: 156
// Size: 34
void MAMSInitMem() {
  FILE *f;
  tPath_name the_path;
}

// Offset: 192
// Size: 38
// EAX: pFlags
// EDX: pTitle
void PrintMemoryDump(int pFlags, char *pTitle) {
}

// Offset: 232
// Size: 169
void* DRStdlibAllocate(br_size_t size, br_uint_8 type) {
  void *p;
  int i;
  char s[256];
}

// Offset: 404
// Size: 38
void DRStdlibFree(void *mem) {
  int i;
}

// Offset: 444
// Size: 40
br_size_t DRStdlibInquire(br_uint_8 type) {
}

// Offset: 484
// Size: 40
br_uint_32 Claim4ByteAlignment(br_uint_8 type) {
}

// Offset: 524
// Size: 48
void InstallDRMemCalls() {
}

// Offset: 572
// Size: 59
// EAX: pPtr
void MAMSUnlock(void **pPtr) {
}

// Offset: 632
// Size: 37
// EAX: pPtr
void MAMSLock(void **pPtr) {
}

// Offset: 672
// Size: 124
void CreateStainlessClasses() {
}

// Offset: 796
// Size: 34
void CheckMemory() {
}

