#ifndef _RESREG_H_
#define _RESREG_H_

#include "brender/br_types.h"

br_resource_class* BrResClassAdd(br_resource_class* rclass);

br_resource_class* BrResClassRemove(br_resource_class* rclass);

br_resource_class* BrResClassFind(char* pattern);

br_resclass_find_cbfn* BrResClassFindHook(br_resclass_find_cbfn* hook);

br_uint_32 BrResClassAddMany(br_resource_class** items, int n);

br_uint_32 BrResClassRemoveMany(br_resource_class** items, int n);

br_uint_32 BrResClassFindMany(char* pattern, br_resource_class** items, int max);

br_uint_32 BrResClassCount(char* pattern);

br_uint_32 BrResClassEnum(char* pattern, br_resclass_enum_cbfn* callback, void* arg);

#endif
