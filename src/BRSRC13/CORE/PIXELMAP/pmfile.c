#include "pmfile.h"
#include "harness.h"

#include "CORE/FW/datafile.h"
#include "CORE/FW/genfile.h"
#include "CORE/PIXELMAP/pmmem.h"

#include <stddef.h>

br_file_struct br_old_pixelmap_F;
br_file_enum pixelmap_type_F;

br_file_struct_member br_old_pixelmap_FM[7];

br_file_struct_member br_pixelmap_FM[8] = {
    { 13u, offsetof(br_pixelmap, type), "type", NULL },
    { 3u, offsetof(br_pixelmap, row_bytes), "row_bytes", NULL },
    { 3u, offsetof(br_pixelmap, width), "width", NULL },
    { 3u, offsetof(br_pixelmap, height), "height", NULL },
    { 3u, offsetof(br_pixelmap, origin_x), "origin_x", NULL },
    { 3u, offsetof(br_pixelmap, origin_y), "origin_y", NULL },
    { 17u, offsetof(br_pixelmap, identifier), "identifier", NULL },
    //not sure about this one...
    { 31436u, 7u, &br_pixelmap_FM, (void*)0x44 }
};
br_file_struct br_pixelmap_F = { "br_pixelmap", 7u, br_pixelmap_FM, sizeof(br_pixelmap) };

br_file_enum_member pixelmap_type_FM[15];
br_chunks_table_entry PixelmapLoadEntries[5] = {
    { 0u, 0u, &FopRead_END },
    { 3u, 0u, &FopRead_PIXELMAP },
    { 33u, 0u, &FopRead_PIXELS },
    { 34u, 0u, &FopRead_ADD_MAP },
    { 4u, 168u, NULL }
};
br_chunks_table PixelmapLoadTable = { 4, PixelmapLoadEntries };

char rscid[54];

#define DF_PIXELMAP 2

int FopWrite_PIXELMAP(br_datafile* df, br_pixelmap* pixelmap) {
    br_pixelmap pmap;
    NOT_IMPLEMENTED();
}

int FopRead_OLD_PIXELMAP(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_pixelmap* pp;
    NOT_IMPLEMENTED();
}

int FopRead_PIXELMAP(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_pixelmap* pp;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    pp = (br_pixelmap*)DevicePixelmapMemAllocate(3u, 0, 0, 0, 2);
    df->res = pp;
    df->prims->struct_read(df, &br_pixelmap_F, pp);
    df->res = NULL;
    pp->row_bytes = (pmTypeInfo[pp->type].bits >> 3) * pp->width;
    DfPush(DF_PIXELMAP, pp, 1);
    return 0;
}

int FopWrite_PIXELS(br_datafile* df, br_pixelmap* pixelmap) {
    int size;
    int bytes;
    int block_count;
    char* pixels;
    NOT_IMPLEMENTED();
}

int FopRead_PIXELS(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    int icount = 0;
    br_pixelmap* pp;
    int size;
    LOG_TRACE("(%p, %d, %d, %d)", df, id, length, count);

    pp = DfTop(DF_PIXELMAP, NULL);
    size = pmTypeInfo[pp->type].file_size;
    df->res = pp;
    pp->pixels = df->prims->block_read(df, NULL, &icount, size, 3);
    pp->flags |= BR_PMF_LINEAR;
    df->res = NULL;
    return 0;
}

int FopWrite_ADD_MAP(br_datafile* df) {
    NOT_IMPLEMENTED();
}

int FopRead_ADD_MAP(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_pixelmap* pp;
    br_pixelmap* map;
    NOT_IMPLEMENTED();
}

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

int WritePixelmap(br_pixelmap* pp, br_datafile* df) {
    NOT_IMPLEMENTED();
}

br_uint_32 BrPixelmapSaveMany(char* filename, br_pixelmap** pixelmaps, br_uint_16 num) {
    br_datafile* df;
    int i;
    NOT_IMPLEMENTED();
}

br_pixelmap* BrPixelmapLoad(char* filename) {
    br_pixelmap* ptr;

    if (BrPixelmapLoadMany(filename, &ptr, 1) == 1) {
        return ptr;
    }
    return NULL;
}

br_uint_32 BrPixelmapSave(char* filename, br_pixelmap* ptr) {
    NOT_IMPLEMENTED();
}
