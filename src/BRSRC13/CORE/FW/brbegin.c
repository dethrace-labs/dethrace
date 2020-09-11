#include "brbegin.h"
#include "harness.h"

#include "CORE/HOST/himage.h"
#include "CORE/PIXELMAP/pmsetup.h"
#include "fwsetup.h"

br_boolean active;
char rscid[54];

br_error BrBegin() {
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
    NOT_IMPLEMENTED();
}

br_error BrEnd() {
    NOT_IMPLEMENTED();
}
