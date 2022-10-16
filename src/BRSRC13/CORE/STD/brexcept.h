#ifndef _BREXCEPT_H_
#define _BREXCEPT_H_

#include "brender/br_types.h"

br_exception_handler* _BrExceptionBegin(void);

void _BrExceptionEnd(void);

void _BrExceptionThrow(br_int_32 type, void* value);

br_exception _BrExceptionValueFetch(br_exception type, void** evp);

void* _BrExceptionResource(void);

#endif
