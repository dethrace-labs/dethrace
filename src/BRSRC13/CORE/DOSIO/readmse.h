#ifndef _READMSE_H_
#define _READMSE_H_

#include "br_types.h"

br_error DOSMouseBegin();

void DOSMouseEnd();

br_error DOSMouseRead(br_int_32* mouse_x, br_int_32* mouse_y, br_uint_32* mouse_buttons);

#endif
