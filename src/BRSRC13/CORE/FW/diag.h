#ifndef _DIAG_H_
#define _DIAG_H_

#include "brender/br_types.h"
void BrFailure(const char* s, ...);

void BrWarning(const char* s, ...);

void BrFatal(const char* name, int line, const char* s, ...);

void _BrAssert(const char* condition, const char* file, unsigned int line);

void _BrUAssert(const char* condition, const char* file, unsigned int line);

#endif
