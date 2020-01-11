#ifndef _ERROR_H_
#define _ERROR_H_

#include "br_types.h"

// Offset: 15
// Size: 55
br_error BrLastErrorGet(void** valuep);

// Offset: 85
// Size: 46
void BrLastErrorSet(br_error type, void* value);

#endif
