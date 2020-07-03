#include "dosio.h"
#include "harness.h"
#include "pixelmap.h"
#include <stddef.h>

br_pixelmap* DOSGfxBegin(char* setup_string) {
    LOG_TRACE("(\"%s\")", setup_string);

    Harness_Hook_ActivateRenderer();
    // Original implementation replaced with SDL...

    return BrPixelmapAllocate(BR_PMT_INDEX_8, 640, 480, NULL, BR_PMAF_NORMAL);
}
