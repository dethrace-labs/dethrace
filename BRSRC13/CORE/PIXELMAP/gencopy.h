#include "dr_types.h"
#include "br_types.h"
// Offset: 21
// Size: 50
// EAX: dest
// EDX: p
// EBX: src
// ECX: r
br_error GeneralRectangleCopy(br_device_pixelmap *dest, br_point *p, br_device_pixelmap *src, br_rectangle *r);

// Offset: 83
// Size: 48
// EAX: dst
// EDX: src
br_error GeneralCopy(br_device_pixelmap *dst, br_device_pixelmap *src);

// Offset: 159
// Size: 50
// EAX: dst
// EDX: d
// EBX: src
// ECX: s
br_error GeneralRectangleStretchCopy(br_device_pixelmap *dst, br_rectangle *d, br_device_pixelmap *src, br_rectangle *s);

