#ifndef _EVENTQ_H_
#define _EVENTQ_H_

#include "br_types.h"

// Offset: 14
// Size: 200
br_error DOSEventBegin();

// Offset: 226
// Size: 63
void DOSEventEnd();

// Offset: 302
// Size: 407
br_boolean DOSEventWait(dosio_event *event, br_boolean block);

#endif
