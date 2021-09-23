#ifndef _BRPRINTF_H_
#define _BRPRINTF_H_

#include "brender/br_types.h"

#include <stdlib.h>

br_int_32 BrSprintf(char* buf, char* fmt, ...);

br_int_32 BrSprintfN(char* buf, br_size_t buf_size, char* fmt, ...);

int BrLogPrintf(char* fmt, ...);

br_int_32 BrSScanf(char* str, char* fmt, ...);

#endif
