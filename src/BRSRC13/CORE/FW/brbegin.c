#include "brbegin.h"
#include "harness/trace.h"

#include "CORE/HOST/himage.h"
#include "CORE/PIXELMAP/pmsetup.h"
#include "fwsetup.h"

br_boolean active;

// IDA: br_error __cdecl BrBegin()
br_error BrBegin(void) {
    LOG_TRACE("()");

    if (active) {
        return 4103;
    } else {
        BrFwBegin();
        HostImageLoad("unk?");
        BrPixelmapBegin();
        active = 1;
        //nullsub_28();
        return 0;
    }
}

// IDA: br_error __cdecl BrEnd()
br_error BrEnd(void) {
    br_device* dev;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
