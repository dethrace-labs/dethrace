#ifndef _HOOK_H_
#define _HOOK_H_

#include "brender/br_types.h"

br_error HostInterruptHook(host_interrupt_hook* h, br_uint_8 vector, br_uint_32 off, br_uint_16 sel);

br_error HostInterruptUnhook(host_interrupt_hook* h);

br_error HostExceptionHook(host_exception_hook* h, br_uint_8 exception, br_uint_32 off, br_uint_16 sel);

br_error HostExceptionUnhook(host_exception_hook* h);

#endif
