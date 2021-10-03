#ifndef _GENCOPY_H_
#define _GENCOPY_H_

#include "brender/br_types.h"

br_error GeneralRectangleCopy(br_device_pixelmap* dest, br_point* p, br_device_pixelmap* src, br_rectangle* r);

br_error GeneralCopy(br_device_pixelmap* dst, br_device_pixelmap* src);

br_error GeneralRectangleStretchCopy(br_device_pixelmap* dst, br_rectangle* d, br_device_pixelmap* src, br_rectangle* s);

#endif
