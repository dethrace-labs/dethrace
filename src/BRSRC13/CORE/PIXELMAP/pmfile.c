#include "pmfile.h"

br_chunks_table PixelmapLoadTable;
br_file_struct br_old_pixelmap_F;
br_file_enum pixelmap_type_F;
br_file_struct br_pixelmap_F;
br_file_struct_member br_old_pixelmap_FM[7];
br_file_struct_member br_pixelmap_FM[8];
br_file_enum_member pixelmap_type_FM[15];
br_chunks_table_entry PixelmapLoadEntries[5];
char rscid[54];

// Offset: 18
// Size: 153
// EAX: df
// EDX: pixelmap
int FopWrite_PIXELMAP(br_datafile* df, br_pixelmap* pixelmap) {
    br_pixelmap pmap;
}

// Offset: 192
// Size: 181
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_PIXELMAP(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_pixelmap* pp;
}

// Offset: 390
// Size: 181
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_PIXELMAP(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_pixelmap* pp;
}

// Offset: 587
// Size: 542
// EAX: df
// EDX: pixelmap
int FopWrite_PIXELS(br_datafile* df, br_pixelmap* pixelmap) {
    int size;
    int bytes;
    int block_count;
    char* pixels;
}

// Offset: 1144
// Size: 144
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_PIXELS(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    int icount;
    br_pixelmap* pp;
    int size;
}

// Offset: 1305
// Size: 66
// EAX: df
int FopWrite_ADD_MAP(br_datafile* df) {
}

// Offset: 1387
// Size: 105
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ADD_MAP(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count) {
    br_pixelmap* pp;
    br_pixelmap* map;
}

// Offset: 1511
// Size: 162
br_uint_32 BrPixelmapLoadMany(char* filename, br_pixelmap** pixelmaps, br_uint_16 num) {
    br_datafile* df;
    int count;
    int r;
}

// Offset: 1687
// Size: 161
// EAX: pp
// EDX: df
int WritePixelmap(br_pixelmap* pp, br_datafile* df) {
}

// Offset: 1867
// Size: 218
br_uint_32 BrPixelmapSaveMany(char* filename, br_pixelmap** pixelmaps, br_uint_16 num) {
    br_datafile* df;
    int i;
}

// Offset: 2100
// Size: 81
br_pixelmap* BrPixelmapLoad(char* filename) {
    br_pixelmap* ptr;
}

// Offset: 2196
// Size: 58
br_uint_32 BrPixelmapSave(char* filename, br_pixelmap* ptr) {
}
