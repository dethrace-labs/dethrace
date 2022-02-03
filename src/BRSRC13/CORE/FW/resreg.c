#include "resreg.h"
#include "harness/trace.h"

#include "CORE/FW/fwsetup.h"
#include "CORE/FW/register.h"
#include <stddef.h>
#include <stdio.h>

// IDA: br_resource_class* __cdecl BrResClassAdd(br_resource_class *rclass)
br_resource_class* BrResClassAdd(br_resource_class* rclass) {
    br_resource_class* r;

    r = BrRegistryAdd(&fw.reg_resource_classes, rclass);
    if (r != NULL) {
        fw.resource_class_index[rclass->res_class] = r;
    }
    return r;
}

// IDA: br_resource_class* __cdecl BrResClassRemove(br_resource_class *rclass)
br_resource_class* BrResClassRemove(br_resource_class* rclass) {
    br_resource_class* r;
    LOG_TRACE("(%p)", rclass);

    r = BrRegistryRemove(&fw.reg_resource_classes, rclass);
    if (r != NULL) {
        fw.resource_class_index[rclass->res_class] = NULL;
    }
    return r;
}

// IDA: br_resource_class* __cdecl BrResClassFind(char *pattern)
br_resource_class* BrResClassFind(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);

    return BrRegistryFind(&fw.reg_resource_classes, pattern);
}

// IDA: br_resclass_find_cbfn* __cdecl BrResClassFindHook(br_resclass_find_cbfn *hook)
br_resclass_find_cbfn* BrResClassFindHook(br_resclass_find_cbfn* hook) {
    br_resclass_find_cbfn* old;
    LOG_TRACE("(%p)", hook);

    old = (br_resclass_find_cbfn*)fw.reg_resource_classes.find_failed_hook;
    fw.reg_resource_classes.find_failed_hook = (br_find_failed_cbfn*)hook;
    return old;
}

// IDA: br_uint_32 __cdecl BrResClassAddMany(br_resource_class **items, int n)
br_uint_32 BrResClassAddMany(br_resource_class** items, int n) {
    int i;
    LOG_TRACE("(%p, %d)", items, n);

    for (i = 0; i < n; i++) {
        if (BrRegistryAdd(&fw.reg_resource_classes, items[i]) != NULL) {
            fw.resource_class_index[items[i]->res_class] = items[i];
        }
    }
    return n;
}

// IDA: br_uint_32 __cdecl BrResClassRemoveMany(br_resource_class **items, int n)
br_uint_32 BrResClassRemoveMany(br_resource_class** items, int n) {
    int i;
    int r;
    LOG_TRACE("(%p, %d)", items, n);

    r = 0;
    for (i = 0; i < n; i++) {
        if (BrRegistryRemove(&fw.reg_resource_classes, items[i]) != NULL) {
            fw.resource_class_index[items[i]->res_class] = NULL;
            r++;
        }
    }
    return r;
}

// IDA: br_uint_32 __cdecl BrResClassFindMany(char *pattern, br_resource_class **items, int max)
br_uint_32 BrResClassFindMany(char* pattern, br_resource_class** items, int max) {
    LOG_TRACE("(\"%s\", %p, %d)", pattern, items, max);

    return BrRegistryFindMany(&fw.reg_resource_classes, pattern, (void**)items, max);
}

// IDA: br_uint_32 __cdecl BrResClassCount(char *pattern)
br_uint_32 BrResClassCount(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);

    return BrRegistryCount(&fw.reg_resource_classes, pattern);
}

// IDA: br_uint_32 __cdecl BrResClassEnum(char *pattern, br_resclass_enum_cbfn *callback, void *arg)
br_uint_32 BrResClassEnum(char* pattern, br_resclass_enum_cbfn* callback, void* arg) {
    LOG_TRACE("(\"%s\", %p, %p)", pattern, callback, arg);

    return BrRegistryEnum(&fw.reg_resource_classes, pattern, (br_enum_cbfn*)callback, arg);
}
