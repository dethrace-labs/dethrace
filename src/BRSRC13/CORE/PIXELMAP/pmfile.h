#ifndef _PMFILE_H_
#define _PMFILE_H_

#include "br_types.h"

// Offset: 18
// Size: 153
// EAX: df
// EDX: pixelmap
int FopWrite_PIXELMAP(br_datafile* df, br_pixelmap* pixelmap);

// Offset: 192
// Size: 181
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_OLD_PIXELMAP(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 390
// Size: 181
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_PIXELMAP(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 587
// Size: 542
// EAX: df
// EDX: pixelmap
int FopWrite_PIXELS(br_datafile* df, br_pixelmap* pixelmap);

// Offset: 1144
// Size: 144
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_PIXELS(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 1305
// Size: 66
// EAX: df
int FopWrite_ADD_MAP(br_datafile* df);

// Offset: 1387
// Size: 105
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_ADD_MAP(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 1511
// Size: 162
br_uint_32 BrPixelmapLoadMany(char* filename, br_pixelmap** pixelmaps, br_uint_16 num);

// Offset: 1687
// Size: 161
// EAX: pp
// EDX: df
int WritePixelmap(br_pixelmap* pp, br_datafile* df);

// Offset: 1867
// Size: 218
br_uint_32 BrPixelmapSaveMany(char* filename, br_pixelmap** pixelmaps, br_uint_16 num);

// Offset: 2100
// Size: 81
br_pixelmap* BrPixelmapLoad(char* filename);

// Offset: 2196
// Size: 58
br_uint_32 BrPixelmapSave(char* filename, br_pixelmap* ptr);

#endif
