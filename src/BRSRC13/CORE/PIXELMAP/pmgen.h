#ifndef _PMGEN_H_
#define _PMGEN_H_

#include "br_types.h"

br_error _M_br_device_pixelmap_gen_match(br_device_pixelmap* self, br_device_pixelmap** newpm, br_token_value* tv);

br_error _M_br_device_pixelmap_gen_copy(br_device_pixelmap* self, br_device_pixelmap* src);

br_error _M_br_device_pixelmap_gen_copyTo(br_device_pixelmap* self, br_device_pixelmap* src);

br_error _M_br_device_pixelmap_gen_copyFrom(br_device_pixelmap* self, br_device_pixelmap* src);

br_error _M_br_device_pixelmap_gen_fill(br_device_pixelmap* self, br_uint_32 colour);

br_error _M_br_device_pixelmap_gen_doubleBuffer(br_device_pixelmap* self, br_device_pixelmap* src);

br_error _M_br_device_pixelmap_gen_copyDirty(br_device_pixelmap* self, br_device_pixelmap* src, br_rectangle* dirty, br_int_32 num_rects);

br_error _M_br_device_pixelmap_gen_copyToDirty(br_device_pixelmap* self, br_device_pixelmap* src, br_rectangle* dirty, br_int_32 num_rects);

br_error _M_br_device_pixelmap_gen_copyFromDirty(br_device_pixelmap* self, br_device_pixelmap* src, br_rectangle* dirty, br_int_32 num_rects);

br_error _M_br_device_pixelmap_gen_fillDirty(br_device_pixelmap* self, br_uint_32 colour, br_rectangle* dirty, br_int_32 num_rects);

br_error _M_br_device_pixelmap_gen_doubleBufferDirty(br_device_pixelmap* self, br_device_pixelmap* src, br_rectangle* dirty, br_int_32 num_rects);

br_error _M_br_device_pixelmap_gen_rectangle(br_device_pixelmap* self, br_rectangle* rect, br_uint_32 colour);

br_error _M_br_device_pixelmap_gen_rectangle2(br_device_pixelmap* self, br_rectangle* rect, br_uint_32 colour_tl, br_uint_32 colour_br);

br_error _M_br_device_pixelmap_gen_line(br_device_pixelmap* self, br_point* s, br_point* e, br_uint_32 colour);

br_error _M_br_device_pixelmap_gen_text(br_device_pixelmap* self, br_point* point, br_font* font, char* text, br_uint_32 colour);

br_error _M_br_device_pixelmap_gen_textBounds(br_device_pixelmap* self, br_rectangle* rect, br_font* font, char* text);

br_error _M_br_device_pixelmap_gen_copyBits(br_device_pixelmap* self, br_point* point, br_uint_8* src, br_uint_16 s_stride, br_rectangle* bit_rect, br_uint_32 colour);

br_error _M_br_device_pixelmap_gen_flush(br_device_pixelmap* self);

br_error _M_br_device_pixelmap_gen_synchronise(br_device_pixelmap* self, br_token sync_type, br_boolean block);

br_error _M_br_device_pixelmap_gen_directLock(br_device_pixelmap* self, br_boolean block);

br_error _M_br_device_pixelmap_gen_directUnlock(br_device_pixelmap* self);

#endif
