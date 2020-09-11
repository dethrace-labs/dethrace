#include "prepmap.h"
#include "harness.h"

char rscid[51];

void BrMapUpdate(br_pixelmap* map, br_uint_16 flags) {
    BrBufferUpdate(map, BRT_COLOUR_MAP_O, flags);
}

void BrBufferUpdate(br_pixelmap* pm, br_token use, br_uint_16 flags) {
    br_token_value tv[3];

    // renderer->dispatch->bufferStoredNew ...
}

void BrBufferClear(br_pixelmap* pm) {
    NOT_IMPLEMENTED();
}
