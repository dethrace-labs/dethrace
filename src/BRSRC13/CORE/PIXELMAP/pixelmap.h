#ifndef _PIXELMAP_H_
#define _PIXELMAP_H_

#include "brender/br_types.h"

br_pixelmap* BrPixelmapAllocate(br_uint_8 type, br_int_32 w, br_int_32 h, void* pixels, int flags);

#endif
