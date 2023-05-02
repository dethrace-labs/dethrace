#include "scratch.h"
#include "diag.h"
#include "fwsetup.h"
#include "harness/trace.h"
#include "resource.h"

char scratchString[512] = "SCRATCH";

// IDA: void* __cdecl BrScratchAllocate(br_size_t size)
void* BrScratchAllocate(br_size_t size) {
    LOG_TRACE("(%d)", size);

    if (fw.scratch_inuse != 0) {
        BrFailure("Scratchpad not available");
    }

    fw.scratch_last = size;

    if (size > fw.scratch_size) {
        if (fw.scratch_ptr != NULL) {
            BrResFree(fw.scratch_ptr);
        }
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
void BrScratchFlush(void) {
    LOG_TRACE("()");

    if (fw.scratch_inuse != 0) {
        BrFailure("Scratchpad cannot be flushed while in use");
    }
    if (fw.scratch_ptr != NULL) {
        BrResFree(fw.scratch_ptr);
    }
    fw.scratch_ptr = NULL;
    fw.scratch_size = 0;
}

// IDA: br_size_t __cdecl BrScratchInquire()
br_size_t BrScratchInquire(void) {
    LOG_TRACE("()");

    return fw.scratch_size;
}

// IDA: char* __cdecl BrScratchString()
char* BrScratchString(void) {
    LOG_TRACE9("()");

    return scratchString;
}

// IDA: br_size_t __cdecl BrScratchStringSize()
br_size_t BrScratchStringSize(void) {
    LOG_TRACE9("()");

    return sizeof(scratchString);
}
