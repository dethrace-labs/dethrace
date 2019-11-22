#ifndef _HOOK_H_
#define _HOOK_H_

#include "br_types.h"

// Offset: 18
// Size: 168
br_error HostInterruptHook(host_interrupt_hook *h, br_uint_8 vector, br_uint_32 off, br_uint_16 sel);

// Offset: 206
// Size: 124
br_error HostInterruptUnhook(host_interrupt_hook *h);

// Offset: 348
// Size: 168
br_error HostExceptionHook(host_exception_hook *h, br_uint_8 exception, br_uint_32 off, br_uint_16 sel);

// Offset: 536
// Size: 124
br_error HostExceptionUnhook(host_exception_hook *h);

#endif
