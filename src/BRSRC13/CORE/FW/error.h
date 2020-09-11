#ifndef _ERROR_H_
#define _ERROR_H_

#include "br_types.h"

br_error BrLastErrorGet(void** valuep);

void BrLastErrorSet(br_error type, void* value);

#endif
