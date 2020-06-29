#include "preptab.h"

#include "debug.h"
#include "prepmap.h"

char rscid[51];

// Offset: 14
// Size: 77
void BrTableUpdate(br_pixelmap* table, br_uint_16 flags) {
    BrBufferUpdate(table, BRT_UNKNOWN, flags);
}
