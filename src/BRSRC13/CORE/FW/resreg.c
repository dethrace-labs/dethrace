#include "resreg.h"
#include "harness.h"

#include "CORE/FW/fwsetup.h"
#include "CORE/FW/register.h"
#include <stddef.h>
#include <stdio.h>

char rscid[48];

br_resource_class* BrResClassAdd(br_resource_class* rclass) {
    br_resource_class* r;

    r = BrRegistryAdd(&fw.reg_resource_classes, rclass);
    if (!r) {
        return NULL;
    }
    fw.resource_class_index[rclass->res_class] = r;
    return r;
}

br_resource_class* BrResClassRemove(br_resource_class* rclass) {
    br_resource_class* r;
    NOT_IMPLEMENTED();
}

br_resource_class* BrResClassFind(char* pattern) {
    NOT_IMPLEMENTED();
}

br_resclass_find_cbfn* BrResClassFindHook(br_resclass_find_cbfn* hook) {
    br_resclass_find_cbfn* old;
    NOT_IMPLEMENTED();
}

br_uint_32 BrResClassAddMany(br_resource_class** items, int n) {
    int i;
    NOT_IMPLEMENTED();
}

br_uint_32 BrResClassRemoveMany(br_resource_class** items, int n) {
    int i;
    int r;
    NOT_IMPLEMENTED();
}

br_uint_32 BrResClassFindMany(char* pattern, br_resource_class** items, int max) {
    NOT_IMPLEMENTED();
}

br_uint_32 BrResClassCount(char* pattern) {
    NOT_IMPLEMENTED();
}

br_uint_32 BrResClassEnum(char* pattern, br_resclass_enum_cbfn* callback, void* arg) {
    NOT_IMPLEMENTED();
}
