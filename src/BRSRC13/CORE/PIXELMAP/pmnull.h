#ifndef _PMNULL_H_
#define _PMNULL_H_

#include "br_types.h"

// Offset: 34
// Size: 40
br_error _M_br_device_pixelmap_null_resize(br_device_pixelmap* self, br_int_32 width, br_int_32 height);

// Offset: 108
// Size: 40
br_error _M_br_device_pixelmap_null_copyTo(br_device_pixelmap* self, br_device_pixelmap* src);

// Offset: 184
// Size: 40
br_error _M_br_device_pixelmap_null_copyFrom(br_device_pixelmap* self, br_device_pixelmap* src);

// Offset: 256
// Size: 40
br_error _M_br_device_pixelmap_null_fill(br_device_pixelmap* self, br_uint_32 colour);

// Offset: 336
// Size: 40
br_error _M_br_device_pixelmap_null_doubleBuffer(br_device_pixelmap* self, br_device_pixelmap* src);

// Offset: 413
// Size: 40
br_error _M_br_device_pixelmap_null_copyDirty(br_device_pixelmap* self, br_device_pixelmap* src, br_rectangle* dirty, br_int_32 num_rects);

// Offset: 492
// Size: 40
br_error _M_br_device_pixelmap_null_copyToDirty(br_device_pixelmap* self, br_device_pixelmap* src, br_rectangle* dirty, br_int_32 num_rects);

// Offset: 573
// Size: 40
br_error _M_br_device_pixelmap_null_copyFromDirty(br_device_pixelmap* self, br_device_pixelmap* src, br_rectangle* dirty, br_int_32 num_rects);

// Offset: 650
// Size: 40
br_error _M_br_device_pixelmap_null_fillDirty(br_device_pixelmap* self, br_uint_32 colour, br_rectangle* dirty, br_int_32 num_rects);

// Offset: 735
// Size: 40
br_error _M_br_device_pixelmap_null_doubleBufferDirty(br_device_pixelmap* self, br_device_pixelmap* src, br_rectangle* dirty, br_int_32 num_rects);

// Offset: 818
// Size: 40
br_error _M_br_device_pixelmap_null_rectangleCopyTo(br_device_pixelmap* self, br_point* p, br_device_pixelmap* src, br_rectangle* r);

// Offset: 903
// Size: 40
br_error _M_br_device_pixelmap_null_rectangleCopyFrom(br_device_pixelmap* self, br_point* p, br_device_pixelmap* mem, br_rectangle* r);

// Offset: 993
// Size: 40
br_error _M_br_device_pixelmap_null_rectangleStretchCopyTo(br_device_pixelmap* self, br_rectangle* d, br_device_pixelmap* src, br_rectangle* s);

// Offset: 1085
// Size: 40
br_error _M_br_device_pixelmap_null_rectangleStretchCopyFrom(br_device_pixelmap* self, br_rectangle* d, br_device_pixelmap* src, br_rectangle* s);

// Offset: 1166
// Size: 40
br_error _M_br_device_pixelmap_null_rectangleFill(br_device_pixelmap* self, br_rectangle* rect, br_uint_32 colour);

// Offset: 1242
// Size: 40
br_error _M_br_device_pixelmap_null_pixelSet(br_device_pixelmap* self, br_point* p, br_uint_32 colour);

// Offset: 1318
// Size: 40
br_error _M_br_device_pixelmap_null_copyBits(br_device_pixelmap* self, br_point* point, br_uint_8* src, br_uint_16 s_stride, br_rectangle* bit_rect, br_uint_32 colour);

// Offset: 1396
// Size: 40
br_error _M_br_device_pixelmap_null_pixelQuery(br_device_pixelmap* self, br_uint_32* pcolour, br_point* p);

// Offset: 1481
// Size: 40
br_error _M_br_device_pixelmap_null_pixelAddressQuery(br_device_pixelmap* self, void** pptr, br_uint_32* pqual, br_point* p);

// Offset: 1564
// Size: 40
br_error _M_br_device_pixelmap_null_pixelAddressSet(br_device_pixelmap* self, void* ptr, br_uint_32* qual);

#endif
