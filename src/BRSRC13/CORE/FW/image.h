#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "br_types.h"

br_boolean BrImageAdd(br_image* img);

br_boolean BrImageRemove(br_image* img);

br_image* BrImageFind(char* pattern);

br_image* imageLoadHost(char* name);

br_image* BrImageReference(char* name);

void* imageLookupName(br_image* img, char* name, br_uint_32 hint);

void* BrImageLookupName(br_image* img, char* name, br_uint_32 hint);

void* BrImageLookupOrdinal(br_image* img, br_uint_32 ordinal);

void BrImageDereference(br_image* image);

void BrImageFree(br_image* image);

void _BrImageFree(void* res, br_uint_8 res_class, br_size_t size);

#endif
