#include "dosio.h"
#include "CORE/PIXELMAP/pixelmap.h"
#include "harness_hooks.h"
#include "harness_trace.h"
#include <stddef.h>

br_pixelmap* DOSGfxBegin(char* setup_string) {
    LOG_TRACE("(\"%s\")", setup_string);

    Harness_Hook_DOSGfxBegin();
    return BrPixelmapAllocate(BR_PMT_INDEX_8, 320, 240, NULL, BR_PMAF_NORMAL);
}
