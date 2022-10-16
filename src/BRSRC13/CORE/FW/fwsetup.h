#ifndef _FWSETUP_H_
#define _FWSETUP_H_

#include "brender/br_types.h"

extern br_framework_state fw;

br_error BrFwBegin(void);

br_error BrFwEnd(void);

br_diaghandler* BrDiagHandlerSet(br_diaghandler* newdh);

br_filesystem* BrFilesystemSet(br_filesystem* newfs);

br_allocator* BrAllocatorSet(br_allocator* newal);

#endif
