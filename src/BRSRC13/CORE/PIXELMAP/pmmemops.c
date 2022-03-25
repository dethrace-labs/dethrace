#include "pmmemops.h"
#include "harness/trace.h"

#include <string.h>

// FIXME: optimize filling memory, eventually using builtins
//        this file should contain a general arch-independent implementation,
//        other files can contain arch-specific implementations, e.g. one for SSE4

void pm_mem_fill_colour(br_uint_8* dest, br_uint_32 qual, br_uint_32 nbpixels, br_uint_32 bpp, br_uint_32 colour) {
    LOG_TRACE("(%p, %d, %d, %d, %d)", dest, qual, nbpixels, bpp, colour);

    switch (bpp) {
        case 1:
            memset(dest, colour, nbpixels);
            break;
        case 2: {
            br_uint_16 p = colour;
            int i;
            for (i = nbpixels; i != 0; i--, dest+=2) {
                *(br_uint_16*)dest = colour;
            }
            break;
        }
        break;
        case 3:
        case 4:
            NOT_IMPLEMENTED();
        default:
            TELL_ME_IF_WE_PASS_THIS_WAY();
            return;
    }
}

void pm_mem_fill_colour_rect(br_uint_8* dest, br_uint_32 qual, br_uint_32 width, br_uint_32 height, br_uint_32 stride, br_uint_32 bpp, br_uint_32 colour) {
    int x;
    int y;
    LOG_TRACE("(%p, %d, %d, %d, %d, %d, %d)", dest, qual, width, height, stride, bpp, colour);

    switch (bpp) {
    case 1:
        for (y = 0; y < height; y++) {
            memset(dest + y * stride, colour, width);
        }
        break;
    case 2:
        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x+=1) {
                *(br_uint_16*)(dest + y * stride + 2 * x) = colour;
            }
        }
        break;
    case 3:
    case 4:
        NOT_IMPLEMENTED();
    default:
        TELL_ME_IF_WE_PASS_THIS_WAY();
        return;
    }
}

void pm_mem_copy_colour(br_uint_8* dest, br_uint_32 dest_qual, br_uint_8* src, br_uint_32 src_qual, br_uint_32 nbpixels, br_uint_32 bpp) {
    LOG_TRACE("(%p, %d, %p, %d, %d, %d)", dest, dest_qual, src, src_qual, nbpixels, bpp);

    memcpy(dest, src, nbpixels * bpp);
}

void pm_mem_copy_colour_rect(br_uint_8* dest, br_uint_32 dest_qual, br_uint_8* src ,br_uint_32 src_qual,
                 br_uint_32 width, br_uint_32 height, br_uint_32 dest_stride, br_uint_32 src_stride, br_uint_32 bpp) {
    int i;
    LOG_TRACE("(%p, %d, %p, %d, %d, %d, %d, %d, %d)", dest, dest_qual, src, src_qual, width, height, dest_stride, src_stride, bpp);

    for (i = 0; i < height; i++) {
        memcpy(dest + i * dest_stride, src + i * src_stride, bpp * width);
    }
}

void pm_mem_set_colour(br_uint_8* dest, br_uint_32 dest_qual, br_uint_32 bpp, br_uint_32 colour) {
    switch (bpp) {
    case 1:
        *(br_uint_8*)dest = colour;
        break;
    case 2:
        *(br_uint_16*)dest = colour;
        break;
    case 3:
        ((br_uint_8*)dest)[0] = colour;
        ((br_uint_8*)dest)[1] = colour >> 8;
        ((br_uint_8*)dest)[2] = colour >> 16;
        break;
    case 4:
        *(br_uint_32*)dest = colour;
        break;
    default:
        TELL_ME_IF_WE_PASS_THIS_WAY();
    }
}
