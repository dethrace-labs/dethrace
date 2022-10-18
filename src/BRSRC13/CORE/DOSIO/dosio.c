#include "dosio.h"
#include "CORE/PIXELMAP/pixelmap.h"
#include "harness/hooks.h"
#include "harness/trace.h"
#include <stddef.h>

br_pixelmap* DOSGfxBegin(char* setup_string) {
    LOG_TRACE("(\"%s\")", setup_string);

    return BrPixelmapAllocate(BR_PMT_INDEX_8, 320, 200, NULL, BR_PMAF_NORMAL);
}
