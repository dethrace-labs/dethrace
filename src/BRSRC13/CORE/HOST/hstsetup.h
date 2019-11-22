#ifndef _HSTSETUP_H_
#define _HSTSETUP_H_

#include "br_types.h"

// Offset: 10
// Size: 49
void HostBegin();

// Offset: 67
// Size: 49
void HostEnd();

// Offset: 125
// Size: 125
br_error HostInfo(host_info *buffer, br_size_t buffersize);

#endif
