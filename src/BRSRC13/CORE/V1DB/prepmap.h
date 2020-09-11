#ifndef _PREPMAP_H_
#define _PREPMAP_H_

#include "br_types.h"

void BrMapUpdate(br_pixelmap* map, br_uint_16 flags);

void BrBufferUpdate(br_pixelmap* pm, br_token use, br_uint_16 flags);

void BrBufferClear(br_pixelmap* pm);

#endif
