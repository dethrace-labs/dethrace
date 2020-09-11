#include "memmgmt.h"
#include "harness.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

host_regs regs;

br_error HostLock(br_uint_32 offset, br_uint_16 sel, br_size_t size) {
    NOT_IMPLEMENTED();
}

br_error HostUnlock(br_uint_32 offset, br_uint_16 sel, br_size_t size) {
    NOT_IMPLEMENTED();
}

br_error HostSelectorAllocate(br_uint_16* selp) {
    NOT_IMPLEMENTED();
}

br_error HostSelectorAllocateLinear(br_uint_16* selp, br_uint_32 base, br_size_t size) {
    br_uint_16 sel;
    br_error r;
    NOT_IMPLEMENTED();
}

br_error HostSelectorAllocateAlias(br_uint_16* aliasp, br_uint_16 sel) {
    br_uint_16 alias;
    br_uint_32 base;
    br_uint_32 limit;
    br_error r;
    NOT_IMPLEMENTED();
}

br_error HostSelectorAllocatePhysical(br_uint_16* selp, br_uint_32 phys_addr, br_size_t size) {
    NOT_IMPLEMENTED();
}

br_error HostSelectorFree(br_uint_16 sel) {
    br_uint_16 rsel;
    NOT_IMPLEMENTED();
}

br_error HostSelectorBaseSet(br_uint_16 sel, br_uint_32 base) {
    NOT_IMPLEMENTED();
}

br_error HostSelectorLimitSet(br_uint_16 sel, br_size_t limit) {
    NOT_IMPLEMENTED();
}

br_error HostSelectorBaseQuery(br_uint_32* basep, br_uint_16 sel) {
    br_uint_32 base;
    NOT_IMPLEMENTED();
}

br_error HostSelectorLimitQuery(br_uint_32* limitp, br_uint_16 sel) {
    br_uint_32 limit;
    ldt ldta;
    NOT_IMPLEMENTED();
}
