#ifndef _READMSE_H_
#define _READMSE_H_

#include "br_types.h"

// Offset: 14
// Size: 358
br_error DOSMouseBegin();

// Offset: 384
// Size: 92
void DOSMouseEnd();

// Offset: 489
// Size: 217
br_error DOSMouseRead(br_int_32 *mouse_x, br_int_32 *mouse_y, br_uint_32 *mouse_buttons);

#endif
