#ifndef _MEMMGMT_H_
#define _MEMMGMT_H_

#include "br_types.h"

br_error HostLock(br_uint_32 offset, br_uint_16 sel, br_size_t size);

br_error HostUnlock(br_uint_32 offset, br_uint_16 sel, br_size_t size);

br_error HostSelectorAllocate(br_uint_16* selp);

br_error HostSelectorAllocateLinear(br_uint_16* selp, br_uint_32 base, br_size_t size);

br_error HostSelectorAllocateAlias(br_uint_16* aliasp, br_uint_16 sel);

br_error HostSelectorAllocatePhysical(br_uint_16* selp, br_uint_32 phys_addr, br_size_t size);

br_error HostSelectorFree(br_uint_16 sel);

br_error HostSelectorBaseSet(br_uint_16 sel, br_uint_32 base);

br_error HostSelectorLimitSet(br_uint_16 sel, br_size_t limit);

br_error HostSelectorBaseQuery(br_uint_32* basep, br_uint_16 sel);

br_error HostSelectorLimitQuery(br_uint_32* limitp, br_uint_16 sel);

#endif
