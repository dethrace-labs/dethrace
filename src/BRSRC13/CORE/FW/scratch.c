#include "scratch.h"
#include "diag.h"
#include "fwsetup.h"
#include "harness_trace.h"
#include "resource.h"

char scratchString[512];

// IDA: void* __cdecl BrScratchAllocate(br_size_t size)
void* BrScratchAllocate(br_size_t size) {
    LOG_TRACE("(%d)", size);

    if (fw.scratch_inuse)
        BrFailure("Scratchpad not available");

    fw.scratch_last = size;

    if (size > fw.scratch_size) {

        if (fw.scratch_ptr)
            BrResFree(fw.scratch_ptr);

        fw.scratch_ptr = BrResAllocate(fw.res, size, BR_MEMORY_SCRATCH);
        fw.scratch_size = size;
    }
    fw.scratch_inuse = 1;
    return fw.scratch_ptr;
}

// IDA: void __cdecl BrScratchFree(void *scratch)
void BrScratchFree(void* scratch) {
    LOG_TRACE("(%p)", scratch);
    fw.scratch_inuse = 0;
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
