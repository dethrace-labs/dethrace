#ifndef _BRPRINTF_H_
#define _BRPRINTF_H_

#include "br_types.h"

#include <stdlib.h>

// Offset: 10
// Size: 75
br_int_32 BrSprintf(char *buf, char *fmt, ...);

// Offset: 96
// Size: 75
br_int_32 BrSprintfN(char *buf, br_size_t buf_size, char *fmt, ...);

// Offset: 183
// Size: 97
int BrLogPrintf(char *fmt, ...);

// Offset: 289
// Size: 75
br_int_32 BrSScanf(char *str, char *fmt, ...);

#endif
