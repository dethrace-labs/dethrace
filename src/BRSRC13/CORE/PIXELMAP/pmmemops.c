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

void pm_mem_copy_bits(br_uint_8* dst, br_uint_32 dst_qual, br_uint_32 dst_stride, br_uint_8* src, br_uint_32 src_stride,
        br_uint_32 start_bit, br_uint_32 end_bit, br_uint_32 height, br_uint_32 bpp, br_uint_32 colour) {
    static br_uint_8 start_mask_lut[9] = { 0xff, 0x7f, 0x3f, 0x1f, 0xf, 0x7, 0x3, 0x1, 0x0 };
    static br_uint_8 end_mask_lut[9] = { 0x0, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff };
    br_uint_8 start_mask;
    br_uint_8 end_mask;
    br_uint_32 end_bit_byte;
    br_uint_8 mask;
    br_uint_8 colour_byte1;
    br_uint_8 colour_byte2;
    br_uint_8 colour_byte3;
    br_uint_8* tmp_dst;
    br_uint_8* tmp_src;
    LOG_TRACE("(%p, %d, %d, %p, %d, %d, %d, %d, %d)", dst, dst_qual, dst_stride, src, src_stride, start_bit, end_bit, height, bpp, colour);


    start_mask = start_mask_lut[start_bit];
    end_mask = end_mask_lut[end_bit % 8];
    end_bit_byte = end_bit / 8;

    if (end_bit_byte == 0) {
        if (bpp == 1) {
            colour_byte1 = colour;
            for (; height != 0; height--, dst += dst_stride, src += src_stride) {
                mask = *src & start_mask & end_mask;
                if (mask & 0x80) {
                    dst[0] = colour_byte1;
                }
                if (mask & 0x40) {
                    dst[1] = colour_byte1;
                }
                if (mask & 0x20) {
                    dst[2] = colour_byte1;
                }
                if (mask & 0x10) {
                    dst[3] = colour_byte1;
                }
                if (mask & 0x08) {
                    dst[4] = colour_byte1;
                }
                if (mask & 0x04) {
                    dst[5] = colour_byte1;
                }
                if (mask & 0x02) {
                    dst[6] = colour_byte1;
                }
                if (mask & 0x01) {
                    dst[7] = colour_byte1;
                }
            }
            return;
        }
        else if (bpp == 2) {
            mask = *src & start_mask & end_mask;
            colour_byte1 = colour;
            colour_byte2 = colour >> 8;
            for (; height != 0; height--, dst += dst_stride, src += src_stride) {
                if (mask & 0x80) {
                    dst[0] = colour_byte1;
                    dst[1] = colour_byte2;
                }
                if (mask & 0x40) {
                    dst[2] = colour_byte1;
                    dst[3] = colour_byte2;
                }
                if (mask & 0x20) {
                    dst[4] = colour_byte1;
                    dst[5] = colour_byte2;
                }
                if (mask & 0x10) {
                    dst[6] = colour_byte1;
                    dst[7] = colour_byte2;
                }
                if (mask & 0x08) {
                    dst[8] = colour_byte1;
                    dst[9] = colour_byte2;
                }
                if (mask & 0x04) {
                    dst[10] = colour_byte1;
                    dst[11] = colour_byte2;
                }
                if (mask & 0x02) {
                    dst[12] = colour_byte1;
                    dst[13] = colour_byte2;
                }
                if (mask & 0x01) {
                    dst[14] = colour_byte1;
                    dst[15] = colour_byte2;
                }
            }
            return;
        }
        else if (bpp == 3) {
            mask = *src & start_mask & end_mask;
            colour_byte1 = colour;
            colour_byte2 = colour >> 8;
            colour_byte3 = colour >> 16;
            for (; height != 0; height--, dst += dst_stride, src += src_stride) {
                if (mask & 0x80) {
                    dst[0] = colour_byte1;
                    dst[1] = colour_byte2;
                    dst[2] = colour_byte3;
                }
                if (mask & 0x40) {
                    dst[3] = colour_byte1;
                    dst[4] = colour_byte2;
                    dst[5] = colour_byte3;
                }
                if (mask & 0x20) {
                    dst[6] = colour_byte1;
                    dst[7] = colour_byte2;
                    dst[8] = colour_byte3;
                }
                if (mask & 0x10) {
                    dst[9] = colour_byte1;
                    dst[10] = colour_byte2;
                    dst[11] = colour_byte3;
                }
                if (mask & 0x08) {
                    dst[12] = colour_byte1;
                    dst[13] = colour_byte2;
                    dst[14] = colour_byte3;
                }
                if (mask & 0x04) {
                    dst[15] = colour_byte1;
                    dst[16] = colour_byte2;
                    dst[17] = colour_byte3;
                }
                if (mask & 0x02) {
                    dst[18] = colour_byte1;
                    dst[19] = colour_byte2;
                    dst[20] = colour_byte3;
                }
                if (mask & 0x01) {
                    dst[21] = colour_byte1;
                    dst[22] = colour_byte2;
                    dst[23] = colour_byte3;
                }
            }
            return;
        }
        else if (bpp == 4) {
            mask = *src & start_mask & end_mask;
            for (; height != 0; height--, dst += dst_stride, src += src_stride) {
                if (mask & 0x80) {
                    ((br_uint_32*)dst)[0] = colour;
                }
                if (mask & 0x40) {
                    ((br_uint_32*)dst)[1] = colour;
                }
                if (mask & 0x20) {
                    ((br_uint_32*)dst)[2] = colour;
                }
                if (mask & 0x10) {
                    ((br_uint_32*)dst)[3] = colour;
                }
                if (mask & 0x08) {
                    ((br_uint_32*)dst)[4] = colour;
                }
                if (mask & 0x04) {
                    ((br_uint_32*)dst)[5] = colour;
                }
                if (mask & 0x02) {
                    ((br_uint_32*)dst)[6] = colour;
                }
                if (mask & 0x01) {
                    ((br_uint_32*)dst)[7] = colour;
                }
            }
            return;
        }
        else {
            return;
        }
    }
    if (bpp == 1) {
        colour_byte1 = colour;

        for (; height != 0; height--, src += src_stride, dst += dst_stride) {
            mask = *src & start_mask;
            tmp_dst = dst;
            tmp_src = src;
            br_uint_8 row_end_bit_byte = end_bit_byte;
            while (row_end_bit_byte != 0) {
                if (mask & 0x80) {
                    tmp_dst[0] = colour_byte1;
                }
                if (mask & 0x40) {
                    tmp_dst[1] = colour_byte1;
                }
                if (mask & 0x20) {
                    tmp_dst[2] = colour_byte1;
                }
                if (mask & 0x10) {
                    tmp_dst[3] = colour_byte1;
                }
                if (mask & 0x08) {
                    tmp_dst[4] = colour_byte1;
                }
                if (mask & 0x04) {
                    tmp_dst[5] = colour_byte1;
                }
                if (mask & 0x02) {
                    tmp_dst[6] = colour_byte1;
                }
                if (mask & 0x01) {
                    tmp_dst[7] = colour_byte1;
                }
                tmp_dst += 8;
                tmp_src += 1;
                row_end_bit_byte -= 1;
                mask = *tmp_src;
            }
            mask = end_mask;
            if (mask & 0x80) {
                tmp_dst[0] = colour_byte1;
            }
            if (mask & 0x40) {
                tmp_dst[1] = colour_byte1;
            }
            if (mask & 0x20) {
                tmp_dst[2] = colour_byte1;
            }
            if (mask & 0x10) {
                tmp_dst[3] = colour_byte1;
            }
            if (mask & 0x08) {
                tmp_dst[4] = colour_byte1;
            }
            if (mask & 0x04) {
                tmp_dst[5] = colour_byte1;
            }
            if (mask & 0x02) {
                tmp_dst[6] = colour_byte1;
            }
            if (mask & 0x01) {
                tmp_dst[7] = colour_byte1;
            }
        }
        return;
    } else {
        TELL_ME_IF_WE_PASS_THIS_WAY();
    }
}
