#ifndef _PMMEM_H_
#define _PMMEM_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 26
// Size: 498
// EAX: type
// EDX: w
// EBX: h
// ECX: pixels
br_device_pixelmap* DevicePixelmapMemAllocate(br_uint_8 type, br_uint_16 w, br_uint_16 h, void *pixels, int flags);

// Offset: 539
// Size: 54
// EAX: pm
void _CheckDispatch(br_device_pixelmap *pm);

// Offset: 631
// Size: 222
br_error _M_br_device_pixelmap_mem_allocateSub(br_device_pixelmap *self, br_device_pixelmap **newpm, br_rectangle *rect);

// Offset: 884
// Size: 42
void _M_br_device_pixelmap_mem_free(br_device_pixelmap *self);

// Offset: 963
// Size: 42
char* _M_br_device_pixelmap_mem_identifier(br_device_pixelmap *self);

// Offset: 1036
// Size: 40
br_token _M_br_device_pixelmap_mem_type(br_device_pixelmap *self);

// Offset: 1109
// Size: 67
br_boolean _M_br_device_pixelmap_mem_isType(br_device_pixelmap *self, br_token t);

// Offset: 1209
// Size: 40
br_device* _M_br_device_pixelmap_mem_device(br_device_pixelmap *self);

// Offset: 1281
// Size: 48
br_int_32 _M_br_device_pixelmap_mem_space(br_device_pixelmap *self);

// Offset: 1369
// Size: 77
br_tv_template* _M_br_device_pixelmap_mem_queryTemplate(br_device_pixelmap *self);

// Offset: 1484
// Size: 40
br_error _M_br_device_pixelmap_mem_validSource(br_device_pixelmap *self, br_object *h);

// Offset: 1557
// Size: 411
br_error _M_br_device_pixelmap_mem_resize(br_device_pixelmap *self, br_int_32 width, br_int_32 height);

// Offset: 2000
// Size: 771
br_error _M_br_device_pixelmap_mem_match(br_device_pixelmap *self, br_device_pixelmap **newpm, br_token_value *tv);

// Offset: 2804
// Size: 759
br_error _M_br_device_pixelmap_mem_copyTo(br_device_pixelmap *self, br_device_pixelmap *src);

// Offset: 3598
// Size: 759
br_error _M_br_device_pixelmap_mem_copyFrom(br_device_pixelmap *self, br_device_pixelmap *dest);

// Offset: 4388
// Size: 530
br_error _M_br_device_pixelmap_mem_fill(br_device_pixelmap *self, br_uint_32 colour);

// Offset: 4957
// Size: 40
br_error _M_br_device_pixelmap_mem_doubleBuffer(br_device_pixelmap *self, br_device_pixelmap *src);

// Offset: 5033
// Size: 40
br_error _M_br_device_pixelmap_mem_copyDirty(br_device_pixelmap *self, br_device_pixelmap *src, br_rectangle *dirty, br_int_32 num_rects);

// Offset: 5111
// Size: 40
br_error _M_br_device_pixelmap_mem_copyToDirty(br_device_pixelmap *self, br_device_pixelmap *src, br_rectangle *dirty, br_int_32 num_rects);

// Offset: 5191
// Size: 40
br_error _M_br_device_pixelmap_mem_copyFromDirty(br_device_pixelmap *self, br_device_pixelmap *src, br_rectangle *dirty, br_int_32 num_rects);

// Offset: 5267
// Size: 40
br_error _M_br_device_pixelmap_mem_fillDirty(br_device_pixelmap *self, br_uint_32 colour, br_rectangle *dirty, br_int_32 num_rects);

// Offset: 5351
// Size: 40
br_error _M_br_device_pixelmap_mem_doubleBufferDirty(br_device_pixelmap *self, br_device_pixelmap *src, br_rectangle *dirty, br_int_32 num_rects);

// Offset: 5433
// Size: 935
br_error _M_br_device_pixelmap_mem_rectangleCopyTo(br_device_pixelmap *self, br_point *p, br_device_pixelmap *src, br_rectangle *r);

// Offset: 6412
// Size: 935
br_error _M_br_device_pixelmap_mem_rectangleCopyFrom(br_device_pixelmap *self, br_point *p, br_device_pixelmap *dest, br_rectangle *r);

// Offset: 7396
// Size: 40
br_error _M_br_device_pixelmap_mem_rectangleStretchCopyTo(br_device_pixelmap *self, br_rectangle *destinationRectangle, br_device_pixelmap *src, br_rectangle *sourceRectangle);

// Offset: 7487
// Size: 40
br_error _M_br_device_pixelmap_mem_rectangleStretchCopyFrom(br_device_pixelmap *self, br_rectangle *d, br_device_pixelmap *src, br_rectangle *s);

// Offset: 7567
// Size: 596
br_error _M_br_device_pixelmap_mem_rectangleFill(br_device_pixelmap *self, br_rectangle *rect, br_uint_32 colour);

// Offset: 8198
// Size: 196
br_error _M_br_device_pixelmap_mem_pixelSet(br_device_pixelmap *self, br_point *p, br_uint_32 colour);

// Offset: 8425
// Size: 8332
br_error _M_br_device_pixelmap_mem_line(br_device_pixelmap *self, br_point *s, br_point *e, br_uint_32 colour);

// Offset: 16792
// Size: 260
br_error _M_br_device_pixelmap_mem_copyBits(br_device_pixelmap *self, br_point *point, br_uint_8 *src, br_uint_16 s_stride, br_rectangle *bit_rect, br_uint_32 colour);

// Offset: 17089
// Size: 199
br_error _M_br_device_pixelmap_mem_pixelQuery(br_device_pixelmap *self, br_uint_32 *pcolour, br_point *p);

// Offset: 17332
// Size: 200
br_error _M_br_device_pixelmap_mem_pixelAddressQuery(br_device_pixelmap *self, void **pptr, br_uint_32 *pqual, br_point *p);

// Offset: 17574
// Size: 72
br_error _M_br_device_pixelmap_mem_pixelAddressSet(br_device_pixelmap *self, void *ptr, br_uint_32 *qual);

// Offset: 17682
// Size: 67
br_error _M_br_device_pixelmap_mem_originSet(br_device_pixelmap *self, br_point *p);

// Offset: 17783
// Size: 40
br_error _M_br_device_pixelmap_mem_rowSize(br_device_pixelmap *self, br_size_t *sizep);

// Offset: 17858
// Size: 40
br_error _M_br_device_pixelmap_mem_rowQuery(br_device_pixelmap *self, void *buffer, br_size_t buffer_size, br_int_32 row);

// Offset: 17931
// Size: 40
br_error _M_br_device_pixelmap_mem_rowSet(br_device_pixelmap *self, void *buffer, br_size_t buffer_size, br_int_32 row);

// Offset: 18003
// Size: 40
br_error _M_br_device_pixelmap_mem_flush(br_device_pixelmap *self);

// Offset: 18081
// Size: 40
br_error _M_br_device_pixelmap_mem_synchronise(br_device_pixelmap *self, br_token sync_type, br_boolean block);

// Offset: 18158
// Size: 40
br_error _M_br_device_pixelmap_mem_directLock(br_device_pixelmap *self, br_boolean block);

// Offset: 18237
// Size: 40
br_error _M_br_device_pixelmap_mem_directUnlock(br_device_pixelmap *self);

#endif
