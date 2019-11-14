#ifndef _PMDSPTCH_H_
#define _PMDSPTCH_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 22
// Size: 107
br_pixelmap* BrPixelmapAllocateSub(br_pixelmap *src, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h);

// Offset: 144
// Size: 98
void BrPixelmapFree(br_pixelmap *src);

// Offset: 259
// Size: 111
br_pixelmap* BrPixelmapResize(br_pixelmap *src, br_int_32 width, br_int_32 height);

// Offset: 386
// Size: 432
br_pixelmap* BrPixelmapMatch(br_pixelmap *src, br_uint_8 match_type);

// Offset: 839
// Size: 308
br_pixelmap* BrPixelmapMatchSized(br_pixelmap *src, br_uint_8 match_type, br_int_32 width, br_int_32 height);

// Offset: 1168
// Size: 389
br_pixelmap* BrPixelmapMatchTyped(br_pixelmap *src, br_uint_8 match_type, br_uint_8 pixelmap_type);

// Offset: 1583
// Size: 438
br_pixelmap* BrPixelmapMatchTypedSized(br_pixelmap *src, br_uint_8 match_type, br_uint_8 pixelmap_type, br_int_32 width, br_int_32 height);

// Offset: 2037
// Size: 95
br_pixelmap* BrPixelmapClone(br_pixelmap *src);

// Offset: 2147
// Size: 57
void BrPixelmapFill(br_pixelmap *dst, br_uint_32 colour);

// Offset: 2224
// Size: 85
void BrPixelmapRectangle(br_pixelmap *dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour);

// Offset: 2330
// Size: 92
void BrPixelmapRectangle2(br_pixelmap *dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour_tl, br_uint_32 colour_br);

// Offset: 2435
// Size: 197
// EAX: self
// EDX: src
br_error DispatchCopy(br_device_pixelmap *self, br_device_pixelmap *src);

// Offset: 2654
// Size: 238
// EAX: self
// EDX: p
// EBX: src
// ECX: r
br_error DispatchRectangleCopy(br_device_pixelmap *self, br_point *p, br_device_pixelmap *src, br_rectangle *r);

// Offset: 2921
// Size: 238
// EAX: self
// EDX: r
// EBX: src
// ECX: s
br_error DispatchRectangleStretchCopy(br_device_pixelmap *self, br_rectangle *r, br_device_pixelmap *src, br_rectangle *s);

// Offset: 3183
// Size: 83
void BrPixelmapRectangleCopy(br_pixelmap *dst, br_int_32 dx, br_int_32 dy, br_pixelmap *src, br_int_32 sx, br_int_32 sy, br_int_32 w, br_int_32 h);

// Offset: 3297
// Size: 95
void BrPixelmapRectangleStretchCopy(br_pixelmap *dst, br_int_32 dx, br_int_32 dy, br_int_32 dw, br_int_32 dh, br_pixelmap *src, br_int_32 sx, br_int_32 sy, br_int_32 sw, br_int_32 sh);

// Offset: 3416
// Size: 88
void BrPixelmapRectangleFill(br_pixelmap *dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour);

// Offset: 3533
// Size: 291
void BrPixelmapDirtyRectangleCopy(br_pixelmap *dst, br_pixelmap *src, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h);

// Offset: 3854
// Size: 87
void BrPixelmapDirtyRectangleClear(br_pixelmap *dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour);

// Offset: 3978
// Size: 87
void BrPixelmapDirtyRectangleDoubleBuffer(br_pixelmap *dst, br_pixelmap *src, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h);

// Offset: 4084
// Size: 76
void BrPixelmapPixelSet(br_pixelmap *dst, br_int_32 x, br_int_32 y, br_uint_32 colour);

// Offset: 4179
// Size: 85
br_uint_32 BrPixelmapPixelGet(br_pixelmap *dst, br_int_32 x, br_int_32 y);

// Offset: 4279
// Size: 220
void BrPixelmapCopy(br_pixelmap *dst, br_pixelmap *src);

// Offset: 4514
// Size: 92
void BrPixelmapLine(br_pixelmap *dst, br_int_32 x1, br_int_32 y1, br_int_32 x2, br_int_32 y2, br_uint_32 colour);

// Offset: 4629
// Size: 65
void BrPixelmapDoubleBuffer(br_pixelmap *dst, br_pixelmap *src);

// Offset: 4709
// Size: 98
void BrPixelmapText(br_pixelmap *dst, br_int_32 x, br_int_32 y, br_uint_32 colour, br_font *font, char *text);

// Offset: 4823
// Size: 145
void BrPixelmapTextF(br_pixelmap *dst, br_int_32 x, br_int_32 y, br_uint_32 colour, br_font *font, char *fmt, ...);

// Offset: 4988
// Size: 214
br_uint_16 BrPixelmapTextWidth(br_pixelmap *dst, br_font *font, char *text);

// Offset: 5223
// Size: 51
br_uint_16 BrPixelmapTextHeight(br_pixelmap *dst, br_font *font);

// Offset: 5293
// Size: 119
void BrPixelmapCopyBits(br_pixelmap *dst, br_int_32 x, br_int_32 y, br_uint_8 *src, br_int_32 s_stride, br_int_32 start_bit, br_int_32 end_bit, br_int_32 nrows, br_uint_32 colour);

// Offset: 5431
// Size: 57
br_uint_16 BrPixelmapFileSize(br_pixelmap *pm);

// Offset: 5508
// Size: 57
br_uint_16 BrPixelmapPixelSize(br_pixelmap *pm);

// Offset: 5584
// Size: 57
br_uint_16 BrPixelmapChannels(br_pixelmap *pm);

// Offset: 5662
// Size: 133
void BrPixelmapPaletteSet(br_pixelmap *pm, br_pixelmap *pal);

// Offset: 5821
// Size: 92
void BrPixelmapPaletteEntrySet(br_pixelmap *pm, br_int_32 index, br_colour colour);

// Offset: 5943
// Size: 96
void BrPixelmapPaletteEntrySetMany(br_pixelmap *pm, br_int_32 index, br_int_32 ncolours, br_colour *colours);

// Offset: 6060
// Size: 113
br_pixelmap* BrPixelmapDirectLock(br_pixelmap *src, br_boolean block);

// Offset: 6196
// Size: 109
br_pixelmap* BrPixelmapDirectUnlock(br_pixelmap *src);

#endif
