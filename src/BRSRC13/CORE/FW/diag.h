#ifndef _DIAG_H_
#define _DIAG_H_

#include "br_types.h"
// Offset: 10
// Size: 118
void BrFailure(const char* s, ...);

// Offset: 138
// Size: 118
void BrWarning(const char* s, ...);

// Offset: 264
// Size: 132
void BrFatal(const char* name, int line, const char* s, ...);

// Offset: 406
// Size: 95
void _BrAssert(const char* condition, const char* file, unsigned int line);

// Offset: 512
// Size: 95
void _BrUAssert(const char* condition, const char* file, unsigned int line);

#endif
