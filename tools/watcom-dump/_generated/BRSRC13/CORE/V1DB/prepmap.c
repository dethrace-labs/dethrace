#include "prepmap.h"

char rscid[51];

// Offset: 12
// Size: 77
void BrMapUpdate(br_pixelmap *map, br_uint_16 flags) {
}

// Offset: 104
// Size: 183
// EAX: pm
// EDX: use
// EBX: flags
void BrBufferUpdate(br_pixelmap *pm, br_token use, br_uint_16 flags) {
    br_token_value tv[3];
}

// Offset: 301
// Size: 76
// EAX: pm
void BrBufferClear(br_pixelmap *pm) {
}

