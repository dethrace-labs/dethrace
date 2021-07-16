#include "memmgmt.h"
#include "harness_trace.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

host_regs regs;

// IDA: br_error __cdecl HostLock(br_uint_32 offset, br_uint_16 sel, br_size_t size)
br_error HostLock(br_uint_32 offset, br_uint_16 sel, br_size_t size) {
    LOG_TRACE("(%d, %d, %d)", offset, sel, size);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl HostUnlock(br_uint_32 offset, br_uint_16 sel, br_size_t size)
br_error HostUnlock(br_uint_32 offset, br_uint_16 sel, br_size_t size) {
    LOG_TRACE("(%d, %d, %d)", offset, sel, size);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl HostSelectorAllocate(br_uint_16 *selp)
br_error HostSelectorAllocate(br_uint_16* selp) {
    LOG_TRACE("(%p)", selp);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl HostSelectorAllocateLinear(br_uint_16 *selp, br_uint_32 base, br_size_t size)
br_error HostSelectorAllocateLinear(br_uint_16* selp, br_uint_32 base, br_size_t size) {
    br_uint_16 sel;
    br_error r;
    LOG_TRACE("(%p, %d, %d)", selp, base, size);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl HostSelectorAllocateAlias(br_uint_16 *aliasp, br_uint_16 sel)
br_error HostSelectorAllocateAlias(br_uint_16* aliasp, br_uint_16 sel) {
    br_uint_16 alias;
    br_uint_32 base;
    br_uint_32 limit;
    br_error r;
    LOG_TRACE("(%p, %d)", aliasp, sel);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl HostSelectorAllocatePhysical(br_uint_16 *selp, br_uint_32 phys_addr, br_size_t size)
br_error HostSelectorAllocatePhysical(br_uint_16* selp, br_uint_32 phys_addr, br_size_t size) {
    LOG_TRACE("(%p, %d, %d)", selp, phys_addr, size);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl HostSelectorFree(br_uint_16 sel)
br_error HostSelectorFree(br_uint_16 sel) {
    br_uint_16 rsel;
    LOG_TRACE("(%d)", sel);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl HostSelectorBaseSet(br_uint_16 sel, br_uint_32 base)
br_error HostSelectorBaseSet(br_uint_16 sel, br_uint_32 base) {
    LOG_TRACE("(%d, %d)", sel, base);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl HostSelectorLimitSet(br_uint_16 sel, br_size_t limit)
br_error HostSelectorLimitSet(br_uint_16 sel, br_size_t limit) {
    LOG_TRACE("(%d, %d)", sel, limit);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl HostSelectorBaseQuery(br_uint_32 *basep, br_uint_16 sel)
br_error HostSelectorBaseQuery(br_uint_32* basep, br_uint_16 sel) {
    br_uint_32 base;
    LOG_TRACE("(%p, %d)", basep, sel);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl HostSelectorLimitQuery(br_uint_32 *limitp, br_uint_16 sel)
br_error HostSelectorLimitQuery(br_uint_32* limitp, br_uint_16 sel) {
    br_uint_32 limit;
    ldt ldt;
    LOG_TRACE("(%p, %d)", limitp, sel);
    NOT_IMPLEMENTED();
}
