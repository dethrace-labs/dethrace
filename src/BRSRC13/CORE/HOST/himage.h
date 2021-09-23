#ifndef _HIMAGE_H_
#define _HIMAGE_H_

#include "brender/br_types.h"

void* HostImageLoad(char* name);

void HostImageUnload(void* image);

void* HostImageLookupName(void* img, char* name, br_uint_32 hint);

void* HostImageLookupOrdinal(void* img, br_uint_32 ordinal);

#endif
