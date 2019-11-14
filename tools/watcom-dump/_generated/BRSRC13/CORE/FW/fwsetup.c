#include "fwsetup.h"

br_image Image_BRCORE1;
br_framework_state fw;
void *functionPointers_BRCORE1[185];
char *namePointers_BRCORE1[185];
br_resource_class resourceClasses[25];
br_uint_16 nameOrdinals_BRCORE1[185];
char rscid[53];

// Offset: 10
// Size: 324
br_error BrFwBegin() {
}

// Offset: 342
// Size: 107
br_error BrFwEnd() {
}

// Offset: 466
// Size: 73
br_diaghandler* BrDiagHandlerSet(br_diaghandler *newdh) {
    br_diaghandler *old;
}

// Offset: 555
// Size: 73
br_filesystem* BrFilesystemSet(br_filesystem *newfs) {
    br_filesystem *old;
}

// Offset: 643
// Size: 73
br_allocator* BrAllocatorSet(br_allocator *newal) {
    br_allocator *old;
}

