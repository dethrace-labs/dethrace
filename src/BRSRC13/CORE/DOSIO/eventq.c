#include "eventq.h"
#include "harness.h"

char rscid[48];
int _DOSEventQEnabled;
host_real_memory _DOSEventMemory;

// Offset: 14
// Size: 200
br_error DOSEventBegin() {
    int s;
    NOT_IMPLEMENTED();
}

// Offset: 226
// Size: 63
void DOSEventEnd() {
    NOT_IMPLEMENTED();
}

// Offset: 302
// Size: 407
br_boolean DOSEventWait(dosio_event* event, br_boolean block) {
    int t;
    NOT_IMPLEMENTED();
}
