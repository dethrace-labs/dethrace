#ifndef _PMMEM_H_
#define _PMMEM_H_

#include "br_types.h"

extern pm_type_info pmTypeInfo[30];

br_device_pixelmap* DevicePixelmapMemAllocate(br_uint_8 type, br_uint_16 w, br_uint_16 h, void* pixels, int flags);

void CheckDispatch(br_device_pixelmap* pm);

br_error _M_br_device_pixelmap_mem_allocateSub(br_device_pixelmap* self, br_device_pixelmap** newpm, br_rectangle* rect);

void _M_br_device_pixelmap_mem_free(br_device_pixelmap* self);

char* _M_br_device_pixelmap_mem_identifier(br_device_pixelmap* self);

br_token _M_br_device_pixelmap_mem_type(br_device_pixelmap* self);

br_boolean _M_br_device_pixelmap_mem_isType(br_device_pixelmap* self, br_token t);

br_device* _M_br_device_pixelmap_mem_device(br_device_pixelmap* self);

br_int_32 _M_br_device_pixelmap_mem_space(br_device_pixelmap* self);

br_tv_template* _M_br_device_pixelmap_mem_queryTemplate(br_device_pixelmap* self);

br_error _M_br_device_pixelmap_mem_validSource(br_device_pixelmap* self, br_object* h);

br_error _M_br_device_pixelmap_mem_resize(br_device_pixelmap* self, br_int_32 width, br_int_32 height);

br_error _M_br_device_pixelmap_mem_match(br_device_pixelmap* self, br_device_pixelmap** newpm, br_token_value* tv);

br_error _M_br_device_pixelmap_mem_copyTo(br_device_pixelmap* self, br_device_pixelmap* src);

br_error _M_br_device_pixelmap_mem_copyFrom(br_device_pixelmap* self, br_device_pixelmap* dest);

br_error _M_br_device_pixelmap_mem_fill(br_device_pixelmap* self, br_uint_32 colour);

br_error _M_br_device_pixelmap_mem_doubleBuffer(br_device_pixelmap* self, br_device_pixelmap* src);

br_error _M_br_device_pixelmap_mem_copyDirty(br_device_pixelmap* self, br_device_pixelmap* src, br_rectangle* dirty, br_int_32 num_rects);

br_error _M_br_device_pixelmap_mem_copyToDirty(br_device_pixelmap* self, br_device_pixelmap* src, br_rectangle* dirty, br_int_32 num_rects);

br_error _M_br_device_pixelmap_mem_copyFromDirty(br_device_pixelmap* self, br_device_pixelmap* src, br_rectangle* dirty, br_int_32 num_rects);

br_error _M_br_device_pixelmap_mem_fillDirty(br_device_pixelmap* self, br_uint_32 colour, br_rectangle* dirty, br_int_32 num_rects);

br_error _M_br_device_pixelmap_mem_doubleBufferDirty(br_device_pixelmap* self, br_device_pixelmap* src, br_rectangle* dirty, br_int_32 num_rects);

br_error _M_br_device_pixelmap_mem_rectangleCopyTo(br_device_pixelmap* self, br_point* p, br_device_pixelmap* src, br_rectangle* r);

br_error _M_br_device_pixelmap_mem_rectangleCopyFrom(br_device_pixelmap* self, br_point* p, br_device_pixelmap* dest, br_rectangle* r);

br_error _M_br_device_pixelmap_mem_rectangleStretchCopyTo(br_device_pixelmap* self, br_rectangle* destinationRectangle, br_device_pixelmap* src, br_rectangle* sourceRectangle);

br_error _M_br_device_pixelmap_mem_rectangleStretchCopyFrom(br_device_pixelmap* self, br_rectangle* d, br_device_pixelmap* src, br_rectangle* s);

br_error _M_br_device_pixelmap_mem_rectangleFill(br_device_pixelmap* self, br_rectangle* rect, br_uint_32 colour);

br_error _M_br_device_pixelmap_mem_pixelSet(br_device_pixelmap* self, br_point* p, br_uint_32 colour);

br_error _M_br_device_pixelmap_mem_line(br_device_pixelmap* self, br_point* s, br_point* e, br_uint_32 colour);

br_error _M_br_device_pixelmap_mem_copyBits(br_device_pixelmap* self, br_point* point, br_uint_8* src, br_uint_16 s_stride, br_rectangle* bit_rect, br_uint_32 colour);

br_error _M_br_device_pixelmap_mem_pixelQuery(br_device_pixelmap* self, br_uint_32* pcolour, br_point* p);

br_error _M_br_device_pixelmap_mem_pixelAddressQuery(br_device_pixelmap* self, void** pptr, br_uint_32* pqual, br_point* p);

br_error _M_br_device_pixelmap_mem_pixelAddressSet(br_device_pixelmap* self, void* ptr, br_uint_32* qual);

br_error _M_br_device_pixelmap_mem_originSet(br_device_pixelmap* self, br_point* p);

br_error _M_br_device_pixelmap_mem_rowSize(br_device_pixelmap* self, br_size_t* sizep);

br_error _M_br_device_pixelmap_mem_rowQuery(br_device_pixelmap* self, void* buffer, br_size_t buffer_size, br_int_32 row);

br_error _M_br_device_pixelmap_mem_rowSet(br_device_pixelmap* self, void* buffer, br_size_t buffer_size, br_int_32 row);

br_error _M_br_device_pixelmap_mem_flush(br_device_pixelmap* self);

br_error _M_br_device_pixelmap_mem_synchronise(br_device_pixelmap* self, br_token sync_type, br_boolean block);

br_error _M_br_device_pixelmap_mem_directLock(br_device_pixelmap* self, br_boolean block);

br_error _M_br_device_pixelmap_mem_directUnlock(br_device_pixelmap* self);

#endif
