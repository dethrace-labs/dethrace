#include "eventq.h"
#include "harness.h"

char rscid[48];
int _DOSEventQEnabled;
host_real_memory _DOSEventMemory;

br_error DOSEventBegin() {
    int s;
    NOT_IMPLEMENTED();
}

void DOSEventEnd() {
    NOT_IMPLEMENTED();
}

br_boolean DOSEventWait(dosio_event* event, br_boolean block) {
    int t;
    NOT_IMPLEMENTED();
}
