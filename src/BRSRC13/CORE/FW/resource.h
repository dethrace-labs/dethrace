#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 10
// Size: 188
// EAX: r
void* ResToUser(resource_header *r);

// Offset: 208
// Size: 72
// EAX: r
resource_header* UserToRes(void *r);

// Offset: 294
// Size: 459
void* BrResAllocate(void *vparent, br_size_t size, br_uint_8 res_class);

// Offset: 771
// Size: 367
// EAX: res
// EDX: callback
void BrResInternalFree(resource_header *res, br_boolean callback);

// Offset: 1148
// Size: 79
void BrResFree(void *vres);

// Offset: 1247
// Size: 76
void BrResFreeNoCallback(void *vres);

// Offset: 1332
// Size: 259
void* BrResAdd(void *vparent, void *vres);

// Offset: 1603
// Size: 141
void* BrResRemove(void *vres);

// Offset: 1755
// Size: 132
br_uint_8 BrResClass(void *vres);

// Offset: 1900
// Size: 266
br_boolean BrResIsChild(void *vparent, void *vchild);

// Offset: 2176
// Size: 177
br_uint_32 BrResSize(void *vres);

// Offset: 2366
// Size: 80
br_uint_32 ResSizeTotal(void *vres, br_uint_32 *ptotal);

// Offset: 2461
// Size: 62
br_uint_32 BrResSizeTotal(void *vres);

// Offset: 2538
// Size: 226
br_uint_32 BrResChildEnum(void *vres, br_resenum_cbfn *callback, void *arg);

// Offset: 2775
// Size: 91
br_uint_32 BrResCheck(void *vres, int no_tag);

// Offset: 2878
// Size: 127
char* BrResStrDup(void *vparent, char *str);

// Offset: 3026
// Size: 313
// EAX: res
// EDX: putline
// EBX: arg
// ECX: level
void InternalResourceDump(resource_header *res, br_putline_cbfn *putline, void *arg, int level);

// Offset: 3349
// Size: 57
void BrResDump(void *vres, br_putline_cbfn *putline, void *arg);

// Offset: 3427
// Size: 80
char* BrResClassIdentifier(br_uint_8 res_class);

#endif
