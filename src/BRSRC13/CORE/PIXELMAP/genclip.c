#include "genclip.h"
#include "harness.h"

char rscid[49];

// IDA: br_clip_result __cdecl PixelmapPointClip(br_point *out, br_point *in, br_pixelmap *pm)
br_clip_result PixelmapPointClip(br_point* out, br_point* in, br_pixelmap* pm) {
    LOG_TRACE("(%p, %p, %p)", out, in, pm);
    NOT_IMPLEMENTED();
}

// IDA: br_clip_result __cdecl PixelmapLineClip(br_point *s_out, br_point *e_out, br_point *s_in, br_point *e_in, br_pixelmap *pm)
br_clip_result PixelmapLineClip(br_point* s_out, br_point* e_out, br_point* s_in, br_point* e_in, br_pixelmap* pm) {
    int temp;
    br_int_32 w;
    br_int_32 h;
    br_int_32 x1;
    br_int_32 x2;
    br_int_32 y1;
    br_int_32 y2;
    LOG_TRACE("(%p, %p, %p, %p, %p)", s_out, e_out, s_in, e_in, pm);
    NOT_IMPLEMENTED();
}

// IDA: br_clip_result __cdecl PixelmapRectangleClip(br_rectangle *out, br_rectangle *in, br_pixelmap *pm)
br_clip_result PixelmapRectangleClip(br_rectangle* out, br_rectangle* in, br_pixelmap* pm) {
    LOG_TRACE("(%p, %p, %p)", out, in, pm);

    out->x = pm->origin_x + in->x;
    out->y = pm->origin_y + in->y;
    out->w = in->w;
    out->h = in->h;
    if (pm->width > out->x && pm->height > out->y) {
        if (out->w + out->x > 0 && out->y + out->h > 0) {
            if (out->x < 0) {
                out->x = 0;
                out->w = out->w + out->x;
            }
            if (out->y < 0) {
                out->y = 0;
                out->h = out->y + out->h;
            }
            if (out->w + out->x > pm->width)
                out->w = pm->width - out->x;
            if (out->h + out->y > pm->height)
                out->h = pm->height - out->y;
            return out->w && out->h;
        }
    }
    return 0;
}

// IDA: br_clip_result __cdecl PixelmapRectangleClipTwo(br_rectangle *r_out, br_point *p_out, br_rectangle *r_in, br_point *p_in, br_pixelmap *pm_dst, br_pixelmap *pm_src)
br_clip_result PixelmapRectangleClipTwo(br_rectangle* r_out, br_point* p_out, br_rectangle* r_in, br_point* p_in, br_pixelmap* pm_dst, br_pixelmap* pm_src) {
    LOG_TRACE("(%p, %p, %p, %p, %p, %p)", r_out, p_out, r_in, p_in, pm_dst, pm_src);
    NOT_IMPLEMENTED();
}

// IDA: br_clip_result __cdecl PixelmapCopyBitsClip(br_rectangle *r_out, br_point *p_out, br_rectangle *r_in, br_point *p_in, br_pixelmap *pm)
br_clip_result PixelmapCopyBitsClip(br_rectangle* r_out, br_point* p_out, br_rectangle* r_in, br_point* p_in, br_pixelmap* pm) {
    LOG_TRACE("(%p, %p, %p, %p, %p)", r_out, p_out, r_in, p_in, pm);
    NOT_IMPLEMENTED();
}
