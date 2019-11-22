#ifndef _PREPMAP_H_
#define _PREPMAP_H_

#include "br_types.h"

// Offset: 12
// Size: 77
void BrMapUpdate(br_pixelmap *map, br_uint_16 flags);

// Offset: 104
// Size: 183
// EAX: pm
// EDX: use
// EBX: flags
void BrBufferUpdate(br_pixelmap *pm, br_token use, br_uint_16 flags);

// Offset: 301
// Size: 76
// EAX: pm
void BrBufferClear(br_pixelmap *pm);

#endif
