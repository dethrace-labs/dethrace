#include "dr_types.h"
#include "br_types.h"
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

