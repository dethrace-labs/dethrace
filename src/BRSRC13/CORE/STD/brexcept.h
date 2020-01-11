#ifndef _BREXCEPT_H_
#define _BREXCEPT_H_

#include "br_types.h"

// Offset: 18
// Size: 81
br_exception_handler* _BrExceptionBegin();

// Offset: 115
// Size: 85
void _BrExceptionEnd();

// Offset: 218
// Size: 107
void _BrExceptionThrow(br_int_32 type, void* value);

// Offset: 348
// Size: 64
br_exception _BrExceptionValueFetch(br_exception type, void** evp);

// Offset: 433
// Size: 41
void* _BrExceptionResource();

#endif
