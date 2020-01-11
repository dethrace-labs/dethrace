#ifndef _FWSETUP_H_
#define _FWSETUP_H_

#include "br_types.h"

extern br_framework_state fw;

// Offset: 10
// Size: 324
br_error BrFwBegin();

// Offset: 342
// Size: 107
br_error BrFwEnd();

// Offset: 466
// Size: 73
br_diaghandler* BrDiagHandlerSet(br_diaghandler* newdh);

// Offset: 555
// Size: 73
br_filesystem* BrFilesystemSet(br_filesystem* newfs);

// Offset: 643
// Size: 73
br_allocator* BrAllocatorSet(br_allocator* newal);

#endif
