#include "pmdsptch.h"

#include "brstdlib.h"
#include "harness.h"
#include "pmmem.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

char rscid[65];

// IDA: br_pixelmap* __cdecl BrPixelmapAllocateSub(br_pixelmap *src, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h)
br_pixelmap* BrPixelmapAllocateSub(br_pixelmap* src, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h) {
    br_pixelmap* new;
    br_rectangle r;
    LOG_TRACE("(%p, %d, %d, %d, %d)", src, x, y, w, h);

    r.h = h;
    r.w = w;
    r.x = x;
    r.y = y;
    _M_br_device_pixelmap_mem_allocateSub((br_device_pixelmap*)src, (br_device_pixelmap**)&new, &r);
    return new;
}

// IDA: void __cdecl BrPixelmapFree(br_pixelmap *src)
void BrPixelmapFree(br_pixelmap* src) {
    LOG_TRACE10("(%p)", src);
    _M_br_device_pixelmap_mem_free((br_device_pixelmap*)src);
}

// IDA: br_pixelmap* __cdecl BrPixelmapResize(br_pixelmap *src, br_int_32 width, br_int_32 height)
br_pixelmap* BrPixelmapResize(br_pixelmap* src, br_int_32 width, br_int_32 height) {
    LOG_TRACE("(%p, %d, %d)", src, width, height);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __cdecl BrPixelmapMatch(br_pixelmap *src, br_uint_8 match_type)
br_pixelmap* BrPixelmapMatch(br_pixelmap* src, br_uint_8 match_type) {
    br_pixelmap* new;
    br_token_value tv[3];
    new = NULL;
    LOG_TRACE("(%p, %d)", src, match_type);

    CheckDispatch((br_device_pixelmap*)src);

    switch (match_type) {
    case 0u:
        tv[1].t = BRT_OFFSCREEN;
        break;
    case 1u:
        tv[2].t = BRT_VECTOR2_INTEGER;
        tv[1].t = BRT_DEPTH;
        tv[1].v.u32 = 76;
        break;
    default:
        LOG_PANIC("Case %d not implemented", match_type);
    }

    //((br_device_pixelmap*)src)->dispatch->_match((br_device_pixelmap*)src, (br_device_pixelmap**)&new, &tv[0]);
    if (_M_br_device_pixelmap_mem_match((br_device_pixelmap*)src, (br_device_pixelmap**)&new, &tv[0]) != 0) {
        LOG_WARN("_M_br_device_pixelmap_mem_match returned error");
        return NULL;
    }
    return new;
}

// IDA: br_pixelmap* __cdecl BrPixelmapMatchSized(br_pixelmap *src, br_uint_8 match_type, br_int_32 width, br_int_32 height)
br_pixelmap* BrPixelmapMatchSized(br_pixelmap* src, br_uint_8 match_type, br_int_32 width, br_int_32 height) {
    br_pixelmap* new;
    br_token_value tv[5];
    LOG_TRACE("(%p, %d, %d, %d)", src, match_type, width, height);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __cdecl BrPixelmapMatchTyped(br_pixelmap *src, br_uint_8 match_type, br_uint_8 pixelmap_type)
br_pixelmap* BrPixelmapMatchTyped(br_pixelmap* src, br_uint_8 match_type, br_uint_8 pixelmap_type) {
    br_pixelmap* new;
    br_token_value tv[4];
    LOG_TRACE("(%p, %d, %d)", src, match_type, pixelmap_type);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __cdecl BrPixelmapMatchTypedSized(br_pixelmap *src, br_uint_8 match_type, br_uint_8 pixelmap_type, br_int_32 width, br_int_32 height)
br_pixelmap* BrPixelmapMatchTypedSized(br_pixelmap* src, br_uint_8 match_type, br_uint_8 pixelmap_type, br_int_32 width, br_int_32 height) {
    br_pixelmap* new;
    br_token_value tv[6];
    LOG_TRACE("(%p, %d, %d, %d, %d)", src, match_type, pixelmap_type, width, height);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __cdecl BrPixelmapClone(br_pixelmap *src)
br_pixelmap* BrPixelmapClone(br_pixelmap* src) {
    br_pixelmap* new;
    br_token_value tv[2];
    LOG_TRACE("(%p)", src);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrPixelmapFill(br_pixelmap *dst, br_uint_32 colour)
void BrPixelmapFill(br_pixelmap* dst, br_uint_32 colour) {
    br_uint_8 linear_wholepixels; //added Jeff
    LOG_TRACE("(%p, %d)", dst, colour);

    linear_wholepixels = BR_PMF_LINEAR | BR_PMF_ROW_WHOLEPIXELS;
    if ((dst->flags & linear_wholepixels) == linear_wholepixels) {
        if (dst->row_bytes > 0) {
            memset(dst->pixels, colour, dst->row_bytes * dst->height);
        } else {
            TELL_ME_IF_WE_PASS_THIS_WAY();
        }
    } else {
        TELL_ME_IF_WE_PASS_THIS_WAY();
    }
}

// IDA: void __cdecl BrPixelmapRectangle(br_pixelmap *dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour)
void BrPixelmapRectangle(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour) {
    br_rectangle r;
    LOG_TRACE("(%p, %d, %d, %d, %d, %d)", dst, x, y, w, h, colour);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrPixelmapRectangle2(br_pixelmap *dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour_tl, br_uint_32 colour_br)
void BrPixelmapRectangle2(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour_tl, br_uint_32 colour_br) {
    br_rectangle r;
    LOG_TRACE("(%p, %d, %d, %d, %d, %d, %d)", dst, x, y, w, h, colour_tl, colour_br);
    NOT_IMPLEMENTED();
}

// IDA: br_error __usercall DispatchCopy@<EAX>(br_device_pixelmap *self@<EAX>, br_device_pixelmap *src@<EDX>)
br_error DispatchCopy(br_device_pixelmap* self, br_device_pixelmap* src) {
    LOG_TRACE("(%p, %p)", self, src);
    NOT_IMPLEMENTED();
}

// IDA: br_error __usercall DispatchRectangleCopy@<EAX>(br_device_pixelmap *self@<EAX>, br_point *p@<EDX>, br_device_pixelmap *src@<EBX>, br_rectangle *r@<ECX>)
br_error DispatchRectangleCopy(br_device_pixelmap* self, br_point* p, br_device_pixelmap* src, br_rectangle* r) {
    LOG_TRACE("(%p, %p, %p, %p)", self, p, src, r);
    NOT_IMPLEMENTED();
}

// IDA: br_error __usercall DispatchRectangleStretchCopy@<EAX>(br_device_pixelmap *self@<EAX>, br_rectangle *r@<EDX>, br_device_pixelmap *src@<EBX>, br_rectangle *s@<ECX>)
br_error DispatchRectangleStretchCopy(br_device_pixelmap* self, br_rectangle* r, br_device_pixelmap* src, br_rectangle* s) {
    LOG_TRACE("(%p, %p, %p, %p)", self, r, src, s);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrPixelmapRectangleCopy(br_pixelmap *dst, br_int_32 dx, br_int_32 dy, br_pixelmap *src, br_int_32 sx, br_int_32 sy, br_int_32 w, br_int_32 h)
void BrPixelmapRectangleCopy(br_pixelmap* dst, br_int_32 dx, br_int_32 dy, br_pixelmap* src, br_int_32 sx, br_int_32 sy, br_int_32 w, br_int_32 h) {
    br_rectangle r;
    br_point p;

    // Thanks Errol!

    br_uint_8* src_pix = (br_uint_8*)src->pixels;
    br_uint_8* dst_pix = (br_uint_8*)dst->pixels;

    dx += dst->origin_x;
    dy += dst->origin_y;

    sx += src->origin_x;
    sy += src->origin_y;

    if (src->type != dst->type) {
        LOG_PANIC("src and dst types don't match! src is %d and dst is %d", src->type, dst->type);
        return;
    }

    if (src->type != BR_PMT_INDEX_8) {
        LOG_PANIC("only 8 bit surfaces supported");
        return;
    }

    for (int x = 0; x < w; x++) {
        if (dx + x < 0 || dx + x >= dst->width) {
            continue;
        }
        if (sx + x >= src->width)
            continue;

        for (int y = 0; y < h; y++) {
            if (dy + y < 0 || dy + y >= dst->height) {
                continue;
            }
            if (sy + y >= src->height)
                continue;

            dst_pix[(y + dy) * dst->row_bytes + (x + dx)] = src_pix[(sy + y) * src->row_bytes + (x + sx)];
        }
    }
}

// IDA: void __cdecl BrPixelmapRectangleStretchCopy(br_pixelmap *dst, br_int_32 dx, br_int_32 dy, br_int_32 dw, br_int_32 dh, br_pixelmap *src, br_int_32 sx, br_int_32 sy, br_int_32 sw, br_int_32 sh)
void BrPixelmapRectangleStretchCopy(br_pixelmap* dst, br_int_32 dx, br_int_32 dy, br_int_32 dw, br_int_32 dh, br_pixelmap* src, br_int_32 sx, br_int_32 sy, br_int_32 sw, br_int_32 sh) {
    br_rectangle s;
    br_rectangle d;
    LOG_TRACE("(%p, %d, %d, %d, %d, %p, %d, %d, %d, %d)", dst, dx, dy, dw, dh, src, sx, sy, sw, sh);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrPixelmapRectangleFill(br_pixelmap *dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour)
void BrPixelmapRectangleFill(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour) {
    br_rectangle r;
    LOG_TRACE("(%p, %d, %d, %d, %d, %d)", dst, x, y, w, h, colour);
    br_uint_8* dst_pix = (br_uint_8*)dst->pixels;

    x += dst->origin_x;
    y += dst->origin_y;
    for (int i = 0; i < w; i++) {
        if (x + i < 0 || x + i >= dst->width) {
            continue;
        }
        for (int j = 0; j < h; j++) {
            if (y + j < 0 || y + j >= dst->height) {
                continue;
            }
            dst_pix[(y + j) * dst->row_bytes + (x + i)] = colour;
        }
    }
}

// IDA: void __cdecl BrPixelmapDirtyRectangleCopy(br_pixelmap *dst, br_pixelmap *src, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h)
void BrPixelmapDirtyRectangleCopy(br_pixelmap* dst, br_pixelmap* src, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h) {
    br_rectangle r;
    br_point p;
    LOG_TRACE("(%p, %p, %d, %d, %d, %d)", dst, src, x, y, w, h);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrPixelmapDirtyRectangleClear(br_pixelmap *dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour)
void BrPixelmapDirtyRectangleClear(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour) {
    br_rectangle r;
    LOG_TRACE("(%p, %d, %d, %d, %d, %d)", dst, x, y, w, h, colour);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrPixelmapDirtyRectangleDoubleBuffer(br_pixelmap *dst, br_pixelmap *src, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h)
void BrPixelmapDirtyRectangleDoubleBuffer(br_pixelmap* dst, br_pixelmap* src, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h) {
    br_rectangle r;
    LOG_TRACE("(%p, %p, %d, %d, %d, %d)", dst, src, x, y, w, h);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrPixelmapPixelSet(br_pixelmap *dst, br_int_32 x, br_int_32 y, br_uint_32 colour)
void BrPixelmapPixelSet(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_uint_32 colour) {
    br_point p;
    //LOG_TRACE("(%p, %d, %d, %d)", dst, x, y, colour);
    br_uint_8* dst_pix = (br_uint_8*)dst->pixels;
    dst_pix[(y * dst->row_bytes) + x] = (br_uint_8)colour;
}

// IDA: br_uint_32 __cdecl BrPixelmapPixelGet(br_pixelmap *dst, br_int_32 x, br_int_32 y)
br_uint_32 BrPixelmapPixelGet(br_pixelmap* dst, br_int_32 x, br_int_32 y) {
    br_point p;
    br_uint_32 colour;
    LOG_TRACE("(%p, %d, %d)", dst, x, y);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrPixelmapCopy(br_pixelmap *dst, br_pixelmap *src)
void BrPixelmapCopy(br_pixelmap* dst, br_pixelmap* src) {
    br_rectangle s;
    br_rectangle d;
    LOG_TRACE("(%p, %p)", dst, src);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrPixelmapLine(br_pixelmap *dst, br_int_32 x1, br_int_32 y1, br_int_32 x2, br_int_32 y2, br_uint_32 colour)
void BrPixelmapLine(br_pixelmap* dst, br_int_32 x1, br_int_32 y1, br_int_32 x2, br_int_32 y2, br_uint_32 colour) {
    //br_point s;
    //br_point e;
    LOG_TRACE("(%p, %d, %d, %d, %d, %d)", dst, x1, y1, x2, y2, colour);

    // Thanks Errol!
    br_int_16 dx, dy, inx, iny, e;
    dx = x2 - x1;
    dy = y2 - y1;
    inx = dx > 0 ? 1 : -1;
    iny = dy > 0 ? 1 : -1;
    dx = abs(dx);
    dy = abs(dy);
    if (dx >= dy) {
        dy <<= 1;
        e = dy - dx;
        dx <<= 1;
        while (x1 != x2) {
            BrPixelmapPixelSet(dst, x1, y1, colour);
            if (e >= 0) {
                y1 += iny;
                e -= dx;
            }
            e += dy;
            x1 += inx;
        }
    } else {
        dx <<= 1;
        e = dx - dy;
        dy <<= 1;
        while (y1 != y2) {
            BrPixelmapPixelSet(dst, x1, y1, colour);
            if (e >= 0) {
                x1 += inx;
                e -= dy;
            }
            e += dx;
            y1 += iny;
        }
    }
    BrPixelmapPixelSet(dst, x1, y1, colour);
}

// IDA: void __cdecl BrPixelmapDoubleBuffer(br_pixelmap *dst, br_pixelmap *src)
void BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src) {
    Harness_Hook_BrPixelmapDoubleBuffer(dst, src);
}

// IDA: void __cdecl BrPixelmapText(br_pixelmap *dst, br_int_32 x, br_int_32 y, br_uint_32 colour, br_font *font, char *text)
void BrPixelmapText(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_uint_32 colour, br_font* font, char* text) {
    LOG_TRACE("(%p, %d, %d, %d, %p, \"%s\")", dst, x, y, colour, font, text);

    // Thanks Errol!
    br_uint_8* dst_pix = (br_uint_8*)dst->pixels;

    if (font->flags & BR_FONTF_PROPORTIONAL) {
        br_int_8* widths = font->width;
        br_uint_8* glyphs = font->glyphs;
        br_uint_16 dst_offset = (y * dst->row_bytes) + x;

        for (int i = 0; i < strlen(text); i++) {
            br_uint_8 pw = widths[(br_uint_8)text[i]];
            br_uint_16 src_offset = font->encoding[(br_uint_8)text[i]];

            for (int fy = 0; fy < font->glyph_y; fy++) {
                br_uint_8 b;

                for (int fx = 0; fx < pw; fx++) {
                    if (fx % 8 == 0) {
                        b = glyphs[src_offset + fy + (fx / 8)];
                    }
                    dst_pix[dst_offset + (fy * dst->row_bytes) + fx + i] = (b & (1 << (7 - fx)) ? colour : 0);
                }
            }

            dst_offset += pw;
        }
    } else {
        TELL_ME_IF_WE_PASS_THIS_WAY();
    }
}

// IDA: void __cdecl BrPixelmapTextF(br_pixelmap *dst, br_int_32 x, br_int_32 y, br_uint_32 colour, br_font *font, char *fmt, ...)
void BrPixelmapTextF(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_uint_32 colour, br_font* font, char* fmt, ...) {
    char* ss;
    br_point p;
    va_list args;
    LOG_TRACE("(%p, %d, %d, %d, %p, \"%s\")", dst, x, y, colour, font, fmt);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_16 __cdecl BrPixelmapTextWidth(br_pixelmap *dst, br_font *font, char *text)
br_uint_16 BrPixelmapTextWidth(br_pixelmap* dst, br_font* font, char* text) {
    int i;
    int j;
    int w;
    LOG_TRACE("(%p, %p, \"%s\")", dst, font, text);

    if (!text) {
        return (font->glyph_x);
    }
    if (BrStrLen(text) == 0) {
        return 0;
    }

    if (font->flags & BR_FONTF_PROPORTIONAL) {
        for (i = 0, w = 0, j = BrStrLen(text); i < j; i++, text++)
            w += font->width[(int)*text] + 1;
        w -= 1;
        return w;
    } else
        return (font->glyph_x + 1) * BrStrLen(text) - 1;
}

// IDA: br_uint_16 __cdecl BrPixelmapTextHeight(br_pixelmap *dst, br_font *font)
br_uint_16 BrPixelmapTextHeight(br_pixelmap* dst, br_font* font) {
    LOG_TRACE("(%p, %p)", dst, font);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrPixelmapCopyBits(br_pixelmap *dst, br_int_32 x, br_int_32 y, br_uint_8 *src, br_int_32 s_stride, br_int_32 start_bit, br_int_32 end_bit, br_int_32 nrows, br_uint_32 colour)
void BrPixelmapCopyBits(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_uint_8* src, br_int_32 s_stride, br_int_32 start_bit, br_int_32 end_bit, br_int_32 nrows, br_uint_32 colour) {
    br_point p;
    br_rectangle r;
    LOG_TRACE("(%p, %d, %d, %p, %d, %d, %d, %d, %d)", dst, x, y, src, s_stride, start_bit, end_bit, nrows, colour);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_16 __cdecl BrPixelmapFileSize(br_pixelmap *pm)
br_uint_16 BrPixelmapFileSize(br_pixelmap* pm) {
    LOG_TRACE("(%p)", pm);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_16 __cdecl BrPixelmapPixelSize(br_pixelmap *pm)
br_uint_16 BrPixelmapPixelSize(br_pixelmap* pm) {
    LOG_TRACE("(%p)", pm);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_16 __cdecl BrPixelmapChannels(br_pixelmap *pm)
br_uint_16 BrPixelmapChannels(br_pixelmap* pm) {
    LOG_TRACE("(%p)", pm);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrPixelmapPaletteSet(br_pixelmap *pm, br_pixelmap *pal)
void BrPixelmapPaletteSet(br_pixelmap* pm, br_pixelmap* pal) {
    LOG_TRACE("(%p, %p)", pm, pal);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrPixelmapPaletteEntrySet(br_pixelmap *pm, br_int_32 index, br_colour colour)
void BrPixelmapPaletteEntrySet(br_pixelmap* pm, br_int_32 index, br_colour colour) {
    br_device_clut* clut;
    LOG_TRACE("(%p, %d, %d)", pm, index, colour);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrPixelmapPaletteEntrySetMany(br_pixelmap *pm, br_int_32 index, br_int_32 ncolours, br_colour *colours)
void BrPixelmapPaletteEntrySetMany(br_pixelmap* pm, br_int_32 index, br_int_32 ncolours, br_colour* colours) {
    br_device_clut* clut;
    LOG_TRACE("(%p, %d, %d, %p)", pm, index, ncolours, colours);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __cdecl BrPixelmapDirectLock(br_pixelmap *src, br_boolean block)
br_pixelmap* BrPixelmapDirectLock(br_pixelmap* src, br_boolean block) {
    LOG_TRACE("(%p, %d)", src, block);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __cdecl BrPixelmapDirectUnlock(br_pixelmap *src)
br_pixelmap* BrPixelmapDirectUnlock(br_pixelmap* src) {
    LOG_TRACE("(%p)", src);
    NOT_IMPLEMENTED();
}
