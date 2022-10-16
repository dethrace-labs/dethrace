#ifndef _EVENTQ_H_
#define _EVENTQ_H_

#include "brender/br_types.h"

br_error DOSEventBegin(void);

void DOSEventEnd(void);

br_boolean DOSEventWait(dosio_event* event, br_boolean block);

#endif
