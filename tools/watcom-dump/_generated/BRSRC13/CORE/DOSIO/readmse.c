#include "readmse.h"

host_real_memory mouseMemory;
host_regs regs;
br_boolean mouseActive;
char rscid[49];
unsigned char mouseHandler[245];

// Offset: 14
// Size: 358
br_error DOSMouseBegin() {
}

// Offset: 384
// Size: 92
void DOSMouseEnd() {
}

// Offset: 489
// Size: 217
br_error DOSMouseRead(br_int_32 *mouse_x, br_int_32 *mouse_y, br_uint_32 *mouse_buttons) {
    br_int_16 mx;
    br_int_16 my;
    br_int_16 ox;
    br_int_16 oy;
}

