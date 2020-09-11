#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include "br_types.h"

void* ResToUser(resource_header* r);

resource_header* UserToRes(void* r);

void* BrResAllocate(void* vparent, br_size_t size, br_uint_8 res_class);

void BrResInternalFree(resource_header* res, br_boolean callback);

void BrResFree(void* vres);

void BrResAssert(void* vres);

void BrResFreeNoCallback(void* vres);

void* BrResAdd(void* vparent, void* vres);

void* BrResRemove(void* vres);

br_uint_8 BrResClass(void* vres);

br_boolean BrResIsChild(void* vparent, void* vchild);

br_uint_32 BrResSize(void* vres);

br_uint_32 ResSizeTotal(void* vres, br_uint_32* ptotal);

br_uint_32 BrResSizeTotal(void* vres);

br_uint_32 BrResChildEnum(void* vres, br_resenum_cbfn* callback, void* arg);

br_uint_32 BrResCheck(void* vres, int no_tag);

char* BrResStrDup(void* vparent, char* str);

void InternalResourceDump(resource_header* res, br_putline_cbfn* putline, void* arg, int level);

void BrResDump(void* vres, br_putline_cbfn* putline, void* arg);

char* BrResClassIdentifier(br_uint_8 res_class);

#endif
