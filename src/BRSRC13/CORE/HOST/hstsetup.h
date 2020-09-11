#ifndef _HSTSETUP_H_
#define _HSTSETUP_H_

#include "br_types.h"

void HostBegin();

void HostEnd();

br_error HostInfo(host_info* buffer, br_size_t buffersize);

#endif
