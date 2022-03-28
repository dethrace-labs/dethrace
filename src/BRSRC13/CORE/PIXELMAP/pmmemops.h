#ifndef _PMMEMOPS_H_
#define _PMMEMOPS_H_
#include "brender/br_types.h"

void pm_mem_fill_colour(br_uint_8 *dest, br_uint_32 qual, br_uint_32 nbbytes, br_uint_32 bpp, br_uint_32 colour);

void pm_mem_fill_colour_rect(br_uint_8* dest, br_uint_32 qual, br_uint_32 width, br_uint_32 height, br_uint_32 stride, br_uint_32 bpp, br_uint_32 colour);

void pm_mem_copy_colour(br_uint_8* dest, br_uint_32 dest_qual, br_uint_8* src, br_uint_32 src_qual, br_uint_32 nbpixels, br_uint_32 bpp);

void pm_mem_copy_colour_rect(br_uint_8* dest, br_uint_32 dest_qual, br_uint_8* src, br_uint_32 src_qual,
    br_uint_32 width, br_uint_32 height, br_uint_32 dest_stride, br_uint_32 src_stride, br_uint_32 bbp);

void pm_mem_set_colour(br_uint_8* dest, br_uint_32 dest_qual, br_uint_32 bbp, br_uint_32 colour);

void pm_mem_copy_bits(br_uint_8* dst, br_uint_32 dst_qual, br_uint_32 dst_stride, br_uint_8* src, br_uint_32 src_stride,
        br_uint_32 start_bit, br_uint_32 end_bit, br_uint_32 height, br_uint_32 bpp, br_uint_32 colour);

#endif // _MEMOPS_H_
