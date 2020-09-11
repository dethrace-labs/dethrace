#include "scratch.h"
#include "harness.h"

char scratchString[512];
char rscid[49];

// IDA: void* __cdecl BrScratchAllocate(br_size_t size)
void* BrScratchAllocate(br_size_t size) {
    LOG_TRACE("(%d)", size);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrScratchFree(void *scratch)
void BrScratchFree(void* scratch) {
    LOG_TRACE("(%p)", scratch);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrScratchFlush()
void BrScratchFlush() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_size_t __cdecl BrScratchInquire()
br_size_t BrScratchInquire() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: char* __cdecl BrScratchString()
char* BrScratchString() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_size_t __cdecl BrScratchStringSize()
br_size_t BrScratchStringSize() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
