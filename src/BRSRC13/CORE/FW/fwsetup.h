#ifndef _FWSETUP_H_
#define _FWSETUP_H_

#include "br_types.h"

extern br_framework_state fw;

br_error BrFwBegin();

br_error BrFwEnd();

br_diaghandler* BrDiagHandlerSet(br_diaghandler* newdh);

br_filesystem* BrFilesystemSet(br_filesystem* newfs);

br_allocator* BrAllocatorSet(br_allocator* newal);

#endif
