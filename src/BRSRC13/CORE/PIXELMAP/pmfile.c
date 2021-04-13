#include "pmfile.h"
#include "harness.h"

#include "CORE/FW/datafile.h"
#include "CORE/FW/genfile.h"
#include "CORE/FW/resource.h"
#include "CORE/PIXELMAP/pmmem.h"

#include <stddef.h>

br_file_enum pixelmap_type_F;

br_file_struct_member br_old_pixelmap_FM[7] = {
    { 13u, offsetof(br_pixelmap, type), "type", NULL },
    { 3u, offsetof(br_pixelmap, row_bytes), "row_bytes", NULL },
    { 3u, offsetof(br_pixelmap, width), "width", NULL },
    { 3u, offsetof(br_pixelmap, height), "height", NULL },
    { 3u, offsetof(br_pixelmap, origin_x), "origin_x", NULL },
    { 3u, offsetof(br_pixelmap, origin_y), "origin_y", NULL },
    { 17u, offsetof(br_pixelmap, identifier), "identifier", NULL }
};

br_file_struct_member br_pixelmap_FM[8] = {
    { 13u, offsetof(br_pixelmap, type), "type", NULL },
    { 3u, offsetof(br_pixelmap, row_bytes), "row_bytes", NULL },
    { 3u, offsetof(br_pixelmap, width), "width", NULL },
    { 3u, offsetof(br_pixelmap, height), "height", NULL },
    { 3u, offsetof(br_pixelmap, origin_x), "origin_x", NULL },
    { 3u, offsetof(br_pixelmap, origin_y), "origin_y", NULL },
    { 3u, offsetof(br_pixelmap, mip_offset), "mip_offset", NULL },
    { 17u, offsetof(br_pixelmap, identifier), "identifier", NULL },

};
br_file_struct br_old_pixelmap_F = { "br_old_pixelmap", 7u, br_old_pixelmap_FM, sizeof(br_pixelmap) };
br_file_struct br_pixelmap_F = { "br_pixelmap", 8u, br_pixelmap_FM, sizeof(br_pixelmap) };

br_file_enum_member pixelmap_type_FM[15];
br_chunks_table_entry PixelmapLoadEntries[5] = {
    { 0u, 0u, &FopRead_END },
    { 3u, 0u, &FopRead_OLD_PIXELMAP },
    { 33u, 0u, &FopRead_PIXELS },
    { 34u, 0u, &FopRead_ADD_MAP }
};
br_chunks_table PixelmapLoadTable = { 4, PixelmapLoadEntries };

char rscid[54];

#define DF_PIXELMAP 2

// IDA: int __usercall FopWrite_PIXELMAP@<EAX>(br_datafile *df@<EAX>, br_pixelmap *pixelmap@<EDX>)
int FopWrite_PIXELMAP(br_datafile* df, br_pixelmap* pixelmap) {
    br_pixelmap pmap;
    LOG_TRACE("(%p, %p)", df, pixelmap);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopRead_OLD_PIXELMAP@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>, br_uint_32 count@<ECX>)
int FopRead_OLD_PIXELMAP(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_pixelmap* pp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    pp = (br_pixelmap*)DevicePixelmapMemAllocate(3u, 0, 0, 0, 2);
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
    LOG_TRACE9("(%p, %d, %d, %d)", df, id, length, count);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FopWrite_PIXELS@<EAX>(br_datafile *df@<EAX>, br_pixelmap *pixelmap@<EDX>)
int FopWrite_PIXELS(br_datafile* df, br_pixelmap* pixelmap) {
    int size;
    int bytes;
    int block_count;
    char* pixels;
    LOG_TRACE("(%p, %p)", df, pixelmap);
    NOT_IMPLEMENTED();
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
    pp->pixels = df->prims->block_read(df, NULL, &icount, size, 3);
    pp->flags |= BR_PMF_LINEAR;
    df->res = NULL;
    return 0;
}

// IDA: int __usercall FopWrite_ADD_MAP@<EAX>(br_datafile *df@<EAX>)
int FopWrite_ADD_MAP(br_datafile* df) {
    LOG_TRACE("(%p)", df);
    NOT_IMPLEMENTED();
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
    int count = 0;
    int r;
    LOG_TRACE("(\"%s\", %p, %d)", filename, pixelmaps, num);

    df = DfOpen(filename, 0, BRT_FIXED);
    if (df) {
        do {
            if (count >= num) {
                break;
            }
            r = DfChunksInterpret(df, &PixelmapLoadTable);
            if (DfTopType() == DF_PIXELMAP) {
                pixelmaps[count] = DfPop(DF_PIXELMAP, NULL);
                count++;
            }
        } while (r);
        DfClose(df);
    }
    return count;
}

// IDA: int __usercall WritePixelmap@<EAX>(br_pixelmap *pp@<EAX>, br_datafile *df@<EDX>)
int WritePixelmap(br_pixelmap* pp, br_datafile* df) {
    LOG_TRACE("(%p, %p)", pp, df);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrPixelmapSaveMany(char *filename, br_pixelmap **pixelmaps, br_uint_16 num)
br_uint_32 BrPixelmapSaveMany(char* filename, br_pixelmap** pixelmaps, br_uint_16 num) {
    br_datafile* df;
    int i;
    LOG_TRACE("(\"%s\", %p, %d)", filename, pixelmaps, num);
    NOT_IMPLEMENTED();
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
    NOT_IMPLEMENTED();
}
