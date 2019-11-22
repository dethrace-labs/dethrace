#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "br_types.h"

// Offset: 11
// Size: 58
br_boolean BrImageAdd(br_image *img);

// Offset: 83
// Size: 52
br_boolean BrImageRemove(br_image *img);

// Offset: 147
// Size: 201
br_image* BrImageFind(char *pattern);

// Offset: 362
// Size: 140
// EAX: name
br_image* imageLoadHost(char *name);

// Offset: 519
// Size: 454
br_image* BrImageReference(char *name);

// Offset: 989
// Size: 332
// EAX: img
// EDX: name
// EBX: hint
void* imageLookupName(br_image *img, char *name, br_uint_32 hint);

// Offset: 1339
// Size: 231
void* BrImageLookupName(br_image *img, char *name, br_uint_32 hint);

// Offset: 1591
// Size: 147
void* BrImageLookupOrdinal(br_image *img, br_uint_32 ordinal);

// Offset: 1757
// Size: 122
void BrImageDereference(br_image *image);

// Offset: 1891
// Size: 90
void BrImageFree(br_image *image);

// Offset: 1994
// Size: 42
void _BrImageFree(void *res, br_uint_8 res_class, br_size_t size);

#endif
