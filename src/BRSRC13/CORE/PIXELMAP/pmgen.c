#include "pmgen.h"

char rscid[48];

// Offset: 32
// Size: 40
br_error _M_br_device_pixelmap_gen_match(br_device_pixelmap* self, br_device_pixelmap** newpm, br_token_value* tv) {
}

// Offset: 103
// Size: 150
br_error _M_br_device_pixelmap_gen_copy(br_device_pixelmap* self, br_device_pixelmap* src) {
    br_rectangle r;
    br_point p;
}

// Offset: 286
// Size: 150
br_error _M_br_device_pixelmap_gen_copyTo(br_device_pixelmap* self, br_device_pixelmap* src) {
    br_rectangle r;
    br_point p;
}

// Offset: 471
// Size: 150
br_error _M_br_device_pixelmap_gen_copyFrom(br_device_pixelmap* self, br_device_pixelmap* src) {
    br_rectangle r;
    br_point p;
}

// Offset: 652
// Size: 116
br_error _M_br_device_pixelmap_gen_fill(br_device_pixelmap* self, br_uint_32 colour) {
    br_rectangle r;
}

// Offset: 807
// Size: 213
br_error _M_br_device_pixelmap_gen_doubleBuffer(br_device_pixelmap* self, br_device_pixelmap* src) {
    br_rectangle s;
    br_rectangle d;
}

// Offset: 1056
// Size: 208
br_error _M_br_device_pixelmap_gen_copyDirty(br_device_pixelmap* self, br_device_pixelmap* src, br_rectangle* dirty, br_int_32 num_rects) {
    int i;
    br_point p;
    br_error e;
}

// Offset: 1302
// Size: 208
br_error _M_br_device_pixelmap_gen_copyToDirty(br_device_pixelmap* self, br_device_pixelmap* src, br_rectangle* dirty, br_int_32 num_rects) {
    int i;
    br_point p;
    br_error e;
}

// Offset: 1550
// Size: 208
br_error _M_br_device_pixelmap_gen_copyFromDirty(br_device_pixelmap* self, br_device_pixelmap* src, br_rectangle* dirty, br_int_32 num_rects) {
    int i;
    br_point p;
    br_error e;
}

// Offset: 1794
// Size: 116
br_error _M_br_device_pixelmap_gen_fillDirty(br_device_pixelmap* self, br_uint_32 colour, br_rectangle* dirty, br_int_32 num_rects) {
    int i;
    br_error e;
}

// Offset: 1954
// Size: 208
br_error _M_br_device_pixelmap_gen_doubleBufferDirty(br_device_pixelmap* self, br_device_pixelmap* src, br_rectangle* dirty, br_int_32 num_rects) {
    int i;
    br_point p;
    br_error e;
}

// Offset: 2198
// Size: 303
br_error _M_br_device_pixelmap_gen_rectangle(br_device_pixelmap* self, br_rectangle* rect, br_uint_32 colour) {
    br_point tl;
    br_point tr;
    br_point bl;
    br_point br;
    br_error e;
}

// Offset: 2538
// Size: 303
br_error _M_br_device_pixelmap_gen_rectangle2(br_device_pixelmap* self, br_rectangle* rect, br_uint_32 colour_tl, br_uint_32 colour_br) {
    br_point tl;
    br_point tr;
    br_point bl;
    br_point br;
    br_error e;
}

// Offset: 2872
// Size: 3973
br_error _M_br_device_pixelmap_gen_line(br_device_pixelmap* self, br_point* s, br_point* e, br_uint_32 colour) {
    int dx;
    int dy;
    int incr1;
    int incr2;
    int D;
    int x;
    int y;
    int xend;
    int c;
    int pixels_left;
    int x1;
    int y1;
    int sign_x;
    int sign_y;
    int step;
    int reverse;
    int i;
    br_point as;
    br_point ae;
    br_point p;
}

// Offset: 6876
// Size: 548
br_error _M_br_device_pixelmap_gen_text(br_device_pixelmap* self, br_point* point, br_font* font, char* text, br_uint_32 colour) {
    br_rectangle r;
    br_point p;
    br_int_32 x;
    br_int_32 y;
    br_int_32 s_stride;
}

// Offset: 7461
// Size: 217
br_error _M_br_device_pixelmap_gen_textBounds(br_device_pixelmap* self, br_rectangle* rect, br_font* font, char* text) {
    int i;
    int j;
}

// Offset: 7713
// Size: 238
br_error _M_br_device_pixelmap_gen_copyBits(br_device_pixelmap* self, br_point* point, br_uint_8* src, br_uint_16 s_stride, br_rectangle* bit_rect, br_uint_32 colour) {
    br_int_32 x;
    br_int_32 y;
    br_point p;
}

// Offset: 7983
// Size: 40
br_error _M_br_device_pixelmap_gen_flush(br_device_pixelmap* self) {
}

// Offset: 8061
// Size: 40
br_error _M_br_device_pixelmap_gen_synchronise(br_device_pixelmap* self, br_token sync_type, br_boolean block) {
}

// Offset: 8138
// Size: 40
br_error _M_br_device_pixelmap_gen_directLock(br_device_pixelmap* self, br_boolean block) {
}

// Offset: 8217
// Size: 40
br_error _M_br_device_pixelmap_gen_directUnlock(br_device_pixelmap* self) {
}
