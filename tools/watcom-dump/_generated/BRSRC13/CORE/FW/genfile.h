#ifndef _GENFILE_H_
#define _GENFILE_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 12
// Size: 50
// EAX: df
// EDX: id
// EBX: length
// ECX: count
int FopRead_END(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

// Offset: 75
// Size: 63
// EAX: df
int FopWrite_END(br_datafile *df);

// Offset: 157
// Size: 120
// EAX: df
// EDX: type
int FopWrite_FILE_INFO(br_datafile *df, br_uint_32 type);

#endif
