#include "readmse.h"
#include "harness.h"

host_real_memory mouseMemory;
host_regs regs;
br_boolean mouseActive;
char rscid[49];
unsigned char mouseHandler[245];

br_error DOSMouseBegin() {
    NOT_IMPLEMENTED();
}

void DOSMouseEnd() {
    NOT_IMPLEMENTED();
}

br_error DOSMouseRead(br_int_32* mouse_x, br_int_32* mouse_y, br_uint_32* mouse_buttons) {
    br_int_16 mx;
    br_int_16 my;
    static br_int_16 ox;
    static br_int_16 oy;
    NOT_IMPLEMENTED();
}
