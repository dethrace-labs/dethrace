#include "dosio.h"
#include "debug.h"
#include "pixelmap.h"
#include <stddef.h>

br_pixelmap* DOSGfxBegin(char* setup_string) {
    LOG_TRACE("(\"%s\")", setup_string);
    // Original implementation replaced with SDL...

    return BrPixelmapAllocate(BR_PMT_INDEX_8, 640, 480, NULL, BR_PMAF_NORMAL);
}
