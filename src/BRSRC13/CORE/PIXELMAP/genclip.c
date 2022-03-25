#include "genclip.h"
#include "harness/trace.h"

// IDA: br_clip_result __cdecl PixelmapPointClip(br_point *out, br_point *in, br_pixelmap *pm)
br_clip_result PixelmapPointClip(br_point* out, br_point* in, br_pixelmap* pm) {
    LOG_TRACE("(%p, %p, %p)", out, in, pm);

    out->x = in->x + pm->origin_x;
    out->y = in->y + pm->origin_y;
    if (out->x >= 0 && out->y >= 0 && out->x < pm->width && out->y < pm->height) {
        return BR_CLIP_ACCEPT;
    }
    return BR_CLIP_REJECT;
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

    x1 = s_in->x + pm->origin_x;
    x2 = e_in->x + pm->origin_x;
    y1 = s_in->y + pm->origin_y;
    y2 = e_in->y + pm->origin_y;
    w = pm->width - 1;
    h = pm->height - 1;
    if (x2 < x1) {
        temp = x2;
        x2 = x1;
        x1 = temp;
        temp = y2;
        y2 = y1;
        y1 = temp;
    }
    if (x2 >= 0 && x1 <= w) {
        if (y1 < y2) {
            if (y2 >= 0 && y1 <= h) {
                if (y1 < 0) {
                    x1 -= (x2 - x1) * y1 / (y2 - y1);
                    if (w < x1) {
                        return BR_CLIP_REJECT;
                    }
                    y1 = 0;
                }
                if (y2 <= h) {
incr_p2:
                    if (x1 < 0) {
                        y1 -= (y2 - y1) * x1 / (x2 - x1);
                        x1 = 0;
                    }
                    if (w < x2) {
                        y2 -= (x2 - w) * (y2 - y1) / (x2 - x1);
                        x2 = w;
                    }
                    s_out->x = x1;
                    s_out->y = y1;
                    e_out->x = x2;
                    e_out->y = y2;
                    return BR_CLIP_PARTIAL;
                } else {
                    x2 -= (y2 - h) * (x2 - x1) / (y2 - y1);
                    y2 = h;
                    if (0 <= x2) {
                        goto incr_p2;
                    }
                }
            }
        } else {
            if (0 <= y1 && y2 <= h) {
                if (y1 <= h) {
decr_p1:
                    if (y2 < 0) {
                        x2 -= (x2 - x1) * y2 / (y2 - y1);
                        if (x2 < 0) {
                            return BR_CLIP_REJECT;
                        }
                        y2 = 0;
                    }
                    if (x1 < 0) {
                        y1 -= (y2 - y1) * x1 / (x2 - x1);
                        x1 = 0;
                    }
                    if (w < x2) {
                        y2 -= (x2 - w) * (y2 - y1) / (x2 - x1);
                        x2 = w;
                    }
                    s_out->x = x1;
                    s_out->y = y1;
                    e_out->x = x2;
                    e_out->y = y2;
                    return BR_CLIP_PARTIAL;
                } else {
                    x1 += (h - y1) * (x2 - x1) / (y2 - y1);
                    y1 = h;
                    if (x1 <= w) {
                        goto decr_p1;
                    }
                }
            }
        }
    }
    return BR_CLIP_REJECT;
}

// IDA: br_clip_result __cdecl PixelmapRectangleClip(br_rectangle *out, br_rectangle *in, br_pixelmap *pm)
br_clip_result PixelmapRectangleClip(br_rectangle* out, br_rectangle* in, br_pixelmap* pm) {
    LOG_TRACE("(%p, %p, %p)", out, in, pm);

    out->x = pm->origin_x + in->x;
    out->y = pm->origin_y + in->y;
    out->w = in->w;
    out->h = in->h;
    if (pm->width > out->x && pm->height > out->y) {
        if ((out->x + out->w) > 0 && (out->y + out->h) > 0) {
            if (out->x < 0) {
                out->x = 0;
                out->w += out->x;
            }
            if (out->y < 0) {
                out->y = 0;
                out->h += out->y;
            }
            if ((out->x + out->w) > pm->width)
                out->w = pm->width - out->x;
            if ((out->y + out->h) > pm->height) {
                out->h = pm->height - out->y;
            }
            if (out->w != 0 && out->h != 0) {
                return BR_CLIP_PARTIAL;
            }
        }
    }
    return BR_CLIP_REJECT;
}

// IDA: br_clip_result __cdecl PixelmapRectangleClipTwo(br_rectangle *r_out, br_point *p_out, br_rectangle *r_in, br_point *p_in, br_pixelmap *pm_dst, br_pixelmap *pm_src)
br_clip_result PixelmapRectangleClipTwo(br_rectangle* r_out, br_point* p_out, br_rectangle* r_in, br_point* p_in, br_pixelmap* pm_dst, br_pixelmap* pm_src) {
    LOG_TRACE("(%p, %p, %p, %p, %p, %p)", r_out, p_out, r_in, p_in, pm_dst, pm_src);

    r_out->x = pm_src->origin_x + r_in->x;
    r_out->y = pm_src->origin_y + r_in->y;
    r_out->w = r_in->w;
    r_out->h = r_in->h;
    p_out->x = pm_dst->origin_x + p_in->x;
    p_out->y = pm_dst->origin_y + p_in->y;

    if (p_out->x >= pm_dst->width) {
        return BR_CLIP_REJECT;
    }
    if (p_out->y >= pm_dst->height) {
        return BR_CLIP_REJECT;
    }
    if (r_out->x >= pm_src->width) {
        return BR_CLIP_REJECT;
    }
    if (r_out->y >= pm_src->height) {
        return BR_CLIP_REJECT;
    }
    if (p_out->x + r_out->w <= 0) {
        return BR_CLIP_REJECT;
    }
    if (p_out->y + r_out->h <= 0) {
        return BR_CLIP_REJECT;
    }
    if (r_out->x + r_out->w <= 0) {
        return BR_CLIP_REJECT;
    }
    if (r_out->y + r_out->h <= 0) {
        return BR_CLIP_REJECT;
    }
    if (p_out->x < 0) {
        r_out->w += p_out->x;
        r_out->x -= p_out->x;
        p_out->x = 0;
    }
    if (p_out->y < 0) {
        r_out->h += p_out->y;
        r_out->y -= p_out->y;
        p_out->y = 0;
    }
    if (p_out->x + r_out->w > pm_dst->width) {
        r_out->w = pm_dst->width - p_out->x;
    }
    if (p_out->y + r_out->h > pm_dst->height) {
        r_out->h = pm_dst->height - p_out->y;
    }
    if (r_out->x < 0) {
        r_out->w += r_out->x;
        p_out->x -= r_out->x;
        r_out->x = 0;
    }
    if (r_out->y < 0) {
        r_out->h += r_out->y;
        p_out->y -= r_out->y;
        r_out->y = 0;
    }
    if (r_out->x + r_out->w > pm_src->width) {
        r_out->w = pm_src->width - r_out->x;
    }
    if (r_out->y + r_out->h > pm_src->height) {
        r_out->h = pm_src->height - r_out->y;
    }
    if (r_out->w == 0 || r_out->h == 0) {
        return BR_CLIP_REJECT;
    }
    return BR_CLIP_PARTIAL;
}

// IDA: br_clip_result __cdecl PixelmapCopyBitsClip(br_rectangle *r_out, br_point *p_out, br_rectangle *r_in, br_point *p_in, br_pixelmap *pm)
br_clip_result PixelmapCopyBitsClip(br_rectangle* r_out, br_point* p_out, br_rectangle* r_in, br_point* p_in, br_pixelmap* pm) {
    LOG_TRACE("(%p, %p, %p, %p, %p)", r_out, p_out, r_in, p_in, pm);

    r_out->x = r_in->x;
    r_out->y = r_in->y;
    r_out->w = r_in->w;
    r_out->h = r_in->h;
    p_out->x = pm->origin_x + p_in->x;
    p_out->y = pm->origin_y + p_in->y;

    if (p_out->x >= pm->width) {
        return BR_CLIP_REJECT;
    }
    if (p_out->y >= pm->height) {
        return BR_CLIP_REJECT;
    }
    if (p_out->x + r_out->w <= 0) {
        return BR_CLIP_REJECT;
    }
    if (p_out->y + r_out->h <= 0) {
        return BR_CLIP_REJECT;
    }
    if (p_out->x < 0) {
        r_out->w += p_out->x;
        r_out->x -= p_out->x;
        p_out->x = 0;
    }
    if (p_out->y < 0) {
        r_out->h += p_out->y;
        r_out->y -= p_out->y;
        p_out->y = 0;
    }
    if ((p_out->x + r_out->w) > pm->width) {
        r_out->w = pm->width - p_out->x;
    }
    if ((p_out->y + r_out->h) > pm->height) {
        r_out->h = pm->height - p_out->y;
    }
    if (r_out->w != 0 && r_out->h != 0) {
        return BR_CLIP_PARTIAL;
    }
    return BR_CLIP_REJECT;
}
