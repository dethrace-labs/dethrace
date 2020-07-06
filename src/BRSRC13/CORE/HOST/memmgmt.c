#include "memmgmt.h"
#include "harness.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

host_regs regs;

// Offset: 9
// Size: 223
br_error HostLock(br_uint_32 offset, br_uint_16 sel, br_size_t size) {
    NOT_IMPLEMENTED();
}

// Offset: 243
// Size: 223
br_error HostUnlock(br_uint_32 offset, br_uint_16 sel, br_size_t size) {
    NOT_IMPLEMENTED();
}

// Offset: 487
// Size: 109
br_error HostSelectorAllocate(br_uint_16* selp) {
    NOT_IMPLEMENTED();
}

// Offset: 623
// Size: 151
br_error HostSelectorAllocateLinear(br_uint_16* selp, br_uint_32 base, br_size_t size) {
    br_uint_16 sel;
    br_error r;
    NOT_IMPLEMENTED();
}

// Offset: 800
// Size: 159
br_error HostSelectorAllocateAlias(br_uint_16* aliasp, br_uint_16 sel) {
    br_uint_16 alias;
    br_uint_32 base;
    br_uint_32 limit;
    br_error r;
    NOT_IMPLEMENTED();
}

// Offset: 988
// Size: 174
br_error HostSelectorAllocatePhysical(br_uint_16* selp, br_uint_32 phys_addr, br_size_t size) {
    NOT_IMPLEMENTED();
}

// Offset: 1179
// Size: 135
br_error HostSelectorFree(br_uint_16 sel) {
    br_uint_16 rsel;
    NOT_IMPLEMENTED();
}

// Offset: 1334
// Size: 122
br_error HostSelectorBaseSet(br_uint_16 sel, br_uint_32 base) {
    NOT_IMPLEMENTED();
}

// Offset: 1477
// Size: 138
br_error HostSelectorLimitSet(br_uint_16 sel, br_size_t limit) {
    NOT_IMPLEMENTED();
}

// Offset: 1637
// Size: 130
br_error HostSelectorBaseQuery(br_uint_32* basep, br_uint_16 sel) {
    br_uint_32 base;
    NOT_IMPLEMENTED();
}

// Offset: 1790
// Size: 175
br_error HostSelectorLimitQuery(br_uint_32* limitp, br_uint_16 sel) {
    br_uint_32 limit;
    ldt ldta;
    NOT_IMPLEMENTED();
}
