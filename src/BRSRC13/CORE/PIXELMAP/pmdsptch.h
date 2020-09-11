#ifndef _PMDSPTCH_H_
#define _PMDSPTCH_H_

#include "br_types.h"

br_pixelmap* BrPixelmapAllocateSub(br_pixelmap* src, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h);

void BrPixelmapFree(br_pixelmap* src);

br_pixelmap* BrPixelmapResize(br_pixelmap* src, br_int_32 width, br_int_32 height);

br_pixelmap* BrPixelmapMatch(br_pixelmap* src, br_uint_8 match_type);

br_pixelmap* BrPixelmapMatchSized(br_pixelmap* src, br_uint_8 match_type, br_int_32 width, br_int_32 height);

br_pixelmap* BrPixelmapMatchTyped(br_pixelmap* src, br_uint_8 match_type, br_uint_8 pixelmap_type);

br_pixelmap* BrPixelmapMatchTypedSized(br_pixelmap* src, br_uint_8 match_type, br_uint_8 pixelmap_type, br_int_32 width, br_int_32 height);

br_pixelmap* BrPixelmapClone(br_pixelmap* src);

void BrPixelmapFill(br_pixelmap* dst, br_uint_32 colour);

void BrPixelmapRectangle(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour);

void BrPixelmapRectangle2(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour_tl, br_uint_32 colour_br);

br_error DispatchCopy(br_device_pixelmap* self, br_device_pixelmap* src);

br_error DispatchRectangleCopy(br_device_pixelmap* self, br_point* p, br_device_pixelmap* src, br_rectangle* r);

br_error DispatchRectangleStretchCopy(br_device_pixelmap* self, br_rectangle* r, br_device_pixelmap* src, br_rectangle* s);

void BrPixelmapRectangleCopy(br_pixelmap* dst, br_int_32 dx, br_int_32 dy, br_pixelmap* src, br_int_32 sx, br_int_32 sy, br_int_32 w, br_int_32 h);

void BrPixelmapRectangleStretchCopy(br_pixelmap* dst, br_int_32 dx, br_int_32 dy, br_int_32 dw, br_int_32 dh, br_pixelmap* src, br_int_32 sx, br_int_32 sy, br_int_32 sw, br_int_32 sh);

void BrPixelmapRectangleFill(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour);

void BrPixelmapDirtyRectangleCopy(br_pixelmap* dst, br_pixelmap* src, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h);

void BrPixelmapDirtyRectangleClear(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour);

void BrPixelmapDirtyRectangleDoubleBuffer(br_pixelmap* dst, br_pixelmap* src, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h);

void BrPixelmapPixelSet(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_uint_32 colour);

br_uint_32 BrPixelmapPixelGet(br_pixelmap* dst, br_int_32 x, br_int_32 y);

void BrPixelmapCopy(br_pixelmap* dst, br_pixelmap* src);

void BrPixelmapLine(br_pixelmap* dst, br_int_32 x1, br_int_32 y1, br_int_32 x2, br_int_32 y2, br_uint_32 colour);

void BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src);

void BrPixelmapText(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_uint_32 colour, br_font* font, char* text);

void BrPixelmapTextF(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_uint_32 colour, br_font* font, char* fmt, ...);

br_uint_16 BrPixelmapTextWidth(br_pixelmap* dst, br_font* font, char* text);

br_uint_16 BrPixelmapTextHeight(br_pixelmap* dst, br_font* font);

void BrPixelmapCopyBits(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_uint_8* src, br_int_32 s_stride, br_int_32 start_bit, br_int_32 end_bit, br_int_32 nrows, br_uint_32 colour);

br_uint_16 BrPixelmapFileSize(br_pixelmap* pm);

br_uint_16 BrPixelmapPixelSize(br_pixelmap* pm);

br_uint_16 BrPixelmapChannels(br_pixelmap* pm);

void BrPixelmapPaletteSet(br_pixelmap* pm, br_pixelmap* pal);

void BrPixelmapPaletteEntrySet(br_pixelmap* pm, br_int_32 index, br_colour colour);

void BrPixelmapPaletteEntrySetMany(br_pixelmap* pm, br_int_32 index, br_int_32 ncolours, br_colour* colours);

br_pixelmap* BrPixelmapDirectLock(br_pixelmap* src, br_boolean block);

br_pixelmap* BrPixelmapDirectUnlock(br_pixelmap* src);

#endif
