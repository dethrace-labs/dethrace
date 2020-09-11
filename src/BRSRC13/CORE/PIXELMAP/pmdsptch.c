#include "pmdsptch.h"

#include "harness.h"
#include "pmmem.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

char rscid[65];

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

void BrPixelmapFree(br_pixelmap* src) {
    LOG_TRACE10("(%p)", src);
    _M_br_device_pixelmap_mem_free((br_device_pixelmap*)src);
}

br_pixelmap* BrPixelmapResize(br_pixelmap* src, br_int_32 width, br_int_32 height) {
    NOT_IMPLEMENTED();
}

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

br_pixelmap* BrPixelmapMatchSized(br_pixelmap* src, br_uint_8 match_type, br_int_32 width, br_int_32 height) {
    br_pixelmap* new;
    br_token_value tv[5];
    NOT_IMPLEMENTED();
}

br_pixelmap* BrPixelmapMatchTyped(br_pixelmap* src, br_uint_8 match_type, br_uint_8 pixelmap_type) {
    br_pixelmap* new;
    br_token_value tv[4];
    NOT_IMPLEMENTED();
}

br_pixelmap* BrPixelmapMatchTypedSized(br_pixelmap* src, br_uint_8 match_type, br_uint_8 pixelmap_type, br_int_32 width, br_int_32 height) {
    br_pixelmap* new;
    br_token_value tv[6];
    NOT_IMPLEMENTED();
}

br_pixelmap* BrPixelmapClone(br_pixelmap* src) {
    br_pixelmap* new;
    br_token_value tv[2];
    NOT_IMPLEMENTED();
}

void BrPixelmapFill(br_pixelmap* dst, br_uint_32 colour) {
    br_uint_8 linear_wholepixels; //added JeffH
    LOG_TRACE("(%p, %d)", dst, colour);

    linear_wholepixels = BR_PMF_LINEAR | BR_PMF_ROW_WHOLEPIXELS;
    if ((dst->flags & linear_wholepixels) == linear_wholepixels) {
        if (dst->row_bytes > 0) {
            memset(dst->pixels, colour, dst->row_bytes * dst->height);
        } else {
            LOG_PANIC("Not implemented");
        }
    }
}

void BrPixelmapRectangle(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour) {
    br_rectangle r;
    NOT_IMPLEMENTED();
}

void BrPixelmapRectangle2(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour_tl, br_uint_32 colour_br) {
    br_rectangle r;
    NOT_IMPLEMENTED();
}

br_error DispatchCopy(br_device_pixelmap* self, br_device_pixelmap* src) {
    NOT_IMPLEMENTED();
}

br_error DispatchRectangleCopy(br_device_pixelmap* self, br_point* p, br_device_pixelmap* src, br_rectangle* r) {
    NOT_IMPLEMENTED();
}

br_error DispatchRectangleStretchCopy(br_device_pixelmap* self, br_rectangle* r, br_device_pixelmap* src, br_rectangle* s) {
    NOT_IMPLEMENTED();
}

void BrPixelmapRectangleCopy(br_pixelmap* dst, br_int_32 dx, br_int_32 dy, br_pixelmap* src, br_int_32 sx, br_int_32 sy, br_int_32 w, br_int_32 h) {
    br_rectangle r;
    br_point p;

    // Taken from Errol's brender

    br_uint_8* src_pix = (br_uint_8*)src->pixels;
    br_uint_8* dst_pix = (br_uint_8*)dst->pixels;

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

void BrPixelmapRectangleStretchCopy(br_pixelmap* dst, br_int_32 dx, br_int_32 dy, br_int_32 dw, br_int_32 dh, br_pixelmap* src, br_int_32 sx, br_int_32 sy, br_int_32 sw, br_int_32 sh) {
    br_rectangle s;
    br_rectangle d;
    NOT_IMPLEMENTED();
}

void BrPixelmapRectangleFill(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour) {
    br_rectangle r;
    NOT_IMPLEMENTED();
}

void BrPixelmapDirtyRectangleCopy(br_pixelmap* dst, br_pixelmap* src, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h) {
    br_rectangle r;
    br_point p;
    NOT_IMPLEMENTED();
}

void BrPixelmapDirtyRectangleClear(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h, br_uint_32 colour) {
    br_rectangle r;
    NOT_IMPLEMENTED();
}

void BrPixelmapDirtyRectangleDoubleBuffer(br_pixelmap* dst, br_pixelmap* src, br_int_32 x, br_int_32 y, br_int_32 w, br_int_32 h) {
    br_rectangle r;
    NOT_IMPLEMENTED();
}

void BrPixelmapPixelSet(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_uint_32 colour) {
    br_point p;
    NOT_IMPLEMENTED();
}

br_uint_32 BrPixelmapPixelGet(br_pixelmap* dst, br_int_32 x, br_int_32 y) {
    br_point p;
    br_uint_32 colour;
    NOT_IMPLEMENTED();
}

void BrPixelmapCopy(br_pixelmap* dst, br_pixelmap* src) {
    br_rectangle s;
    br_rectangle d;
    NOT_IMPLEMENTED();
}

void BrPixelmapLine(br_pixelmap* dst, br_int_32 x1, br_int_32 y1, br_int_32 x2, br_int_32 y2, br_uint_32 colour) {
    br_point s;
    br_point e;
    NOT_IMPLEMENTED();
}

void BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src) {
    Harness_Hook_BrPixelmapDoubleBuffer(dst, src);
}

void BrPixelmapText(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_uint_32 colour, br_font* font, char* text) {
    br_point p;
    NOT_IMPLEMENTED();
}

void BrPixelmapTextF(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_uint_32 colour, br_font* font, char* fmt, ...) {
    char* ss;
    br_point p;
    va_list args;
    NOT_IMPLEMENTED();
}

br_uint_16 BrPixelmapTextWidth(br_pixelmap* dst, br_font* font, char* text) {
    int i;
    int j;
    int w;
    NOT_IMPLEMENTED();
}

br_uint_16 BrPixelmapTextHeight(br_pixelmap* dst, br_font* font) {
    NOT_IMPLEMENTED();
}

void BrPixelmapCopyBits(br_pixelmap* dst, br_int_32 x, br_int_32 y, br_uint_8* src, br_int_32 s_stride, br_int_32 start_bit, br_int_32 end_bit, br_int_32 nrows, br_uint_32 colour) {
    br_point p;
    br_rectangle r;
    NOT_IMPLEMENTED();
}

br_uint_16 BrPixelmapFileSize(br_pixelmap* pm) {
    NOT_IMPLEMENTED();
}

br_uint_16 BrPixelmapPixelSize(br_pixelmap* pm) {
    NOT_IMPLEMENTED();
}

br_uint_16 BrPixelmapChannels(br_pixelmap* pm) {
    NOT_IMPLEMENTED();
}

void BrPixelmapPaletteSet(br_pixelmap* pm, br_pixelmap* pal) {
    NOT_IMPLEMENTED();
}

void BrPixelmapPaletteEntrySet(br_pixelmap* pm, br_int_32 index, br_colour colour) {
    br_device_clut* clut;
    NOT_IMPLEMENTED();
}

void BrPixelmapPaletteEntrySetMany(br_pixelmap* pm, br_int_32 index, br_int_32 ncolours, br_colour* colours) {
    br_device_clut* clut;
    NOT_IMPLEMENTED();
}

br_pixelmap* BrPixelmapDirectLock(br_pixelmap* src, br_boolean block) {
    NOT_IMPLEMENTED();
}

br_pixelmap* BrPixelmapDirectUnlock(br_pixelmap* src) {
    NOT_IMPLEMENTED();
}
