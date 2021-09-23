#ifndef _GENFILE_H_
#define _GENFILE_H_

#include "brender/br_types.h"

int FopRead_END(br_datafile* df, br_uint_32 id, br_uint_32 length, br_uint_32 count);

int FopWrite_END(br_datafile* df);

int FopWrite_FILE_INFO(br_datafile* df, br_uint_32 type);

#endif
