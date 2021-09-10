#include "preptab.h"

#include "harness_trace.h"
#include "prepmap.h"

// IDA: void __cdecl BrTableUpdate(br_pixelmap *table, br_uint_16 flags)
void BrTableUpdate(br_pixelmap* table, br_uint_16 flags) {
    BrBufferUpdate(table, BRT_UNKNOWN, flags);
}
