#include "pmfile.h"
#include "harness/trace.h"

#include "CORE/FW/datafile.h"
#include "CORE/FW/diag.h"
#include "CORE/FW/genfile.h"
#include "CORE/FW/resource.h"
#include "CORE/PIXELMAP/pmmem.h"
#include "CORE/PIXELMAP/pmdsptch.h"
#include "CORE/V1DB/chunkids.h"
#include "CORE/V1DB/stackids.h"

#include <stddef.h>
#include <string.h>

br_file_enum_member pixelmap_type_FM[15] = {
    { BR_PMT_INDEX_1, "BR_PMT_INDEX_1", },
    { BR_PMT_INDEX_2, "BR_PMT_INDEX_2", },
    { BR_PMT_INDEX_4, "BR_PMT_INDEX_4", },
    { BR_PMT_INDEX_8, "BR_PMT_INDEX_8", },
    { BR_PMT_RGB_555, "BR_PMT_RGB_555", },
    { BR_PMT_RGB_565, "BR_PMT_RGB_565", },
    { BR_PMT_RGB_888, "BR_PMT_RGB_888", },
    { BR_PMT_RGBX_888, "BR_PMT_RGBX_888", },
    { BR_PMT_RGBA_8888, "BR_PMT_RGBA_8888", },
    { BR_PMT_YUYV_8888, "BR_PMT_YUYV_8888", },
    { BR_PMT_YUV_888, "BR_PMT_YUV_888", },
    { BR_PMT_DEPTH_16, "BR_PMT_DEPTH_16", },
    { BR_PMT_DEPTH_32, "BR_PMT_DEPTH_32", },
    { BR_PMT_ALPHA_8, "BR_PMT_ALPHA_8", },
    { BR_PMT_INDEXA_88, "BR_PMT_INDEXA_88", },
};
br_file_enum pixelmap_type_F = { BR_ASIZE(pixelmap_type_FM), pixelmap_type_FM };

br_file_struct_member br_old_pixelmap_FM[7] = {
    { DF_TYPE_ENUM_8, offsetof(br_pixelmap, type), "type", &pixelmap_type_F },
    { DF_TYPE_BR_UINT_16, offsetof(br_pixelmap, row_bytes), "row_bytes", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_pixelmap, width), "width", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_pixelmap, height), "height", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_pixelmap, origin_x), "origin_x", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_pixelmap, origin_y), "origin_y", NULL },
    { DF_TYPE_ASCIZ, offsetof(br_pixelmap, identifier), "identifier", NULL },
};

br_file_struct_member br_pixelmap_FM[8] = {
    { DF_TYPE_ENUM_8, offsetof(br_pixelmap, type), "type", &pixelmap_type_F },
    { DF_TYPE_BR_UINT_16, offsetof(br_pixelmap, row_bytes), "row_bytes", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_pixelmap, width), "width", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_pixelmap, height), "height", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_pixelmap, origin_x), "origin_x", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_pixelmap, origin_y), "origin_y", NULL },
    { DF_TYPE_BR_UINT_16, offsetof(br_pixelmap, mip_offset), "mip_offset", NULL },
    { DF_TYPE_ASCIZ, offsetof(br_pixelmap, identifier), "identifier", NULL },
};

br_file_struct br_pixelmap_F = { "br_pixelmap", BR_ASIZE(br_pixelmap_FM), br_pixelmap_FM, sizeof(br_pixelmap) };
br_file_struct br_old_pixelmap_F = { "br_old_pixelmap", BR_ASIZE(br_old_pixelmap_FM), br_old_pixelmap_FM, sizeof(br_pixelmap) };

br_chunks_table_entry PixelmapLoadEntries[5] = {
    { CHUNKID_END, 0u, FopRead_END },
    { CHUNKID_PIXELMAP, 0u, FopRead_PIXELMAP },
    { CHUNKID_PIXELMAP_PIXELS, 0u, FopRead_PIXELS },
    { CHUNKID_PIXELMAP_ADD_MAP, 0u, FopRead_ADD_MAP },
    { CHUNKID_OLD_PIXELMAP, 0u, FopRead_OLD_PIXELMAP },
};
br_chunks_table PixelmapLoadTable = { BR_ASIZE(PixelmapLoadEntries), PixelmapLoadEntries };

// IDA: int __usercall FopWrite_PIXELMAP@<EAX>(br_datafile *df@<EAX>, br_pixelmap *pixelmap@<EDX>)
int FopWrite_PIXELMAP(br_datafile* df, br_pixelmap* pixelmap) {
    br_pixelmap pmap;
    LOG_TRACE("(%p, %p)", df, pixelmap);

    memcpy(&pmap, pixelmap, sizeof(br_pixelmap));
    pmap.row_bytes = (pmTypeInfo[pmap.type].bits >> 3) * pmap.width;
    df->prims->chunk_write(df, CHUNKID_PIXELMAP, df->prims->struct_size(df, &br_pixelmap_F, &pmap));
    df->prims->struct_write(df, &br_pixelmap_F, &pmap);
    return 0;
}

// IDA: int __usercall FopRead_OLD_PIXELMAP@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_OLD_PIXELMAP(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_pixelmap* pp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    pp = (br_pixelmap*)DevicePixelmapMemAllocate(BR_PMAF_NO_PIXELS | BR_PMAF_INVERTED, 0, 0, 0, 2);
    df->res = pp;
    df->prims->struct_read(df, &br_old_pixelmap_F, pp);
    df->res = NULL;
    pp->row_bytes = (pmTypeInfo[pp->type].bits >> 3) * pp->width;
    DfPush(DF_PIXELMAP, pp, 1);
    return 0;
}

// IDA: int __usercall FopRead_PIXELMAP@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_PIXELMAP(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_pixelmap* pp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    pp = (br_pixelmap*)DevicePixelmapMemAllocate(BR_PMAF_NO_PIXELS | BR_PMAF_INVERTED, 0, 0, 0, 2);
    df->res = pp;
    df->prims->struct_read(df, &br_pixelmap_F, pp);
    df->res = NULL;
    pp->row_bytes = (pmTypeInfo[pp->type].bits >> 3) * pp->width;
    DfPush(DF_PIXELMAP, pp, 1);
    return 0;
}

// IDA: int __usercall FopWrite_PIXELS@<EAX>(br_datafile *df@<EAX>, br_pixelmap *pixelmap@<EDX>)
int FopWrite_PIXELS(br_datafile* df, br_pixelmap* pixelmap) {
    int size;
    int bytes;
    int block_count;
    char* pixels;
    LOG_TRACE("(%p, %p)", df, pixelmap);

    pixels = (char*)pixelmap->pixels + pixelmap->base_x * pixelmap->width + pixelmap->base_y;
    if (pixelmap->mip_offset == 0) {
        bytes = df->prims->block_size(df, pixels, (pmTypeInfo[pixelmap->type].bits >> 3) * pixelmap->width / pmTypeInfo[pixelmap->type].file_size, pixelmap->row_bytes, pixelmap->height, pmTypeInfo[pixelmap->type].file_size);
        df->prims->chunk_write(df, CHUNKID_PIXELMAP_PIXELS, bytes);
        bytes = (pmTypeInfo[pixelmap->type].bits >> 3) * pixelmap->width / pmTypeInfo[pixelmap->type].file_size;
        size = pixelmap->row_bytes / pmTypeInfo[pixelmap->type].file_size;
        block_count = pixelmap->height;
    } else {
        size = 0;
        for (bytes = pixelmap->width; bytes != 0; bytes = bytes >> 1) {
            size += bytes * bytes * (pmTypeInfo[pixelmap->type].bits >> 3);
        }
        df->prims->chunk_write(df, CHUNKID_PIXELMAP_PIXELS, df-> prims->block_size(df, pixels, size / pmTypeInfo[pixelmap->type].file_size, size, 1, pmTypeInfo[pixelmap->type].file_size));
        bytes = size;
        block_count = 1;
    }
    df->prims->block_write(df, pixels, bytes, size, block_count, pmTypeInfo[pixelmap->type].file_size);
    return 0;
}

// IDA: int __usercall FopRead_PIXELS@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_PIXELS(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    int icount = 0;
    br_pixelmap* pp;
    int size;
    LOG_TRACE9("(%p, %d, %d, %d)", df, id, length, count);

    pp = DfTop(DF_PIXELMAP, NULL);
    size = pmTypeInfo[pp->type].file_size;
    df->res = pp;
    pp->pixels = df->prims->block_read(df, NULL, &icount, size, BR_MEMORY_PIXELS);
    pp->flags |= BR_PMF_LINEAR;
    df->res = NULL;
    return 0;
}

// IDA: int __usercall FopWrite_ADD_MAP@<EAX>(br_datafile *df@<EAX>)
int FopWrite_ADD_MAP(br_datafile* df) {
    LOG_TRACE("(%p)", df);

    df->prims->chunk_write(df, CHUNKID_PIXELMAP_ADD_MAP, 0);
    return 0;
}

// IDA: int __usercall FopRead_ADD_MAP@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_ADD_MAP(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_pixelmap* pp;
    br_pixelmap* map;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    map = DfPop(DF_PIXELMAP, 0);
    pp = DfTop(DF_PIXELMAP, 0);
    pp->map = map;
    BrResAdd(pp, map);
    return 0;
}

// IDA: br_uint_32 __cdecl BrPixelmapLoadMany(char *filename, br_pixelmap **pixelmaps, br_uint_16 num)
br_uint_32 BrPixelmapLoadMany(char* filename, br_pixelmap** pixelmaps, br_uint_16 num) {
    br_datafile* df;
    int count;
    int r;
    LOG_TRACE("(\"%s\", %p, %d)", filename, pixelmaps, num);

    df = DfOpen(filename, 0, BRT_FIXED);
    if (df == NULL) {
        return 0;
    }
    count = 0;
    do {
        if (count >= num) {
            break;
        }
        r = DfChunksInterpret(df, &PixelmapLoadTable);
        if (DfTopType() == DF_PIXELMAP) {
            pixelmaps[count] = DfPop(DF_PIXELMAP, 0);
            count++;
        }
    } while (r != 0);
    DfClose(df);
    return count;
}

// IDA: int __usercall WritePixelmap@<EAX>(br_pixelmap *pp@<EAX>, br_datafile *df@<EDX>)
int WritePixelmap(br_pixelmap* pp, br_datafile* df) {
    LOG_TRACE("(%p, %p)", pp, df);

    BrPixelmapDirectLock(pp, 1);
    if (pp->pixels == NULL) {
        _BrAssert("pp->pixels", "pmfile.c", 291);
    }
    FopWrite_PIXELMAP(df, pp);
    if (pp->map != NULL) {
        WritePixelmap(pp->map, df);
        FopWrite_ADD_MAP(df);
    }
    FopWrite_PIXELS(df, pp);
    BrPixelmapDirectUnlock(pp);
    return 0;
}

// IDA: br_uint_32 __cdecl BrPixelmapSaveMany(char *filename, br_pixelmap **pixelmaps, br_uint_16 num)
br_uint_32 BrPixelmapSaveMany(char* filename, br_pixelmap** pixelmaps, br_uint_16 num) {
    br_datafile* df;
    int i;
    LOG_TRACE("(\"%s\", %p, %d)", filename, pixelmaps, num);

    if (filename == NULL) {
        _BrAssert("filename != NULL", "pmfile.c", 324);
    }
    if (pixelmaps == NULL) {
        _BrAssert("pixelmaps != NULL", "pmfile.c", 325);
    }
    df = DfOpen(filename, 1, BRT_FIXED);
    if (df == NULL) {
        return 0;
    }
    FopWrite_FILE_INFO(df, 2);
    for (i = 0; i < num; i++) {
        WritePixelmap(pixelmaps[i], df);
        FopWrite_END(df);
    }
    DfClose(df);
    return num;
}

// IDA: br_pixelmap* __cdecl BrPixelmapLoad(char *filename)
br_pixelmap* BrPixelmapLoad(char* filename) {
    br_pixelmap* ptr;

    if (BrPixelmapLoadMany(filename, &ptr, 1) == 1) {
        return ptr;
    }
    return NULL;
}

// IDA: br_uint_32 __cdecl BrPixelmapSave(char *filename, br_pixelmap *ptr)
br_uint_32 BrPixelmapSave(char* filename, br_pixelmap* ptr) {
    LOG_TRACE("(\"%s\", %p)", filename, ptr);

    return BrPixelmapSaveMany(filename, &ptr, 1);
}
