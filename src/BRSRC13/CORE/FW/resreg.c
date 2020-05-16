#include "resreg.h"
#include "debug.h"

#include "CORE/FW/fwsetup.h"
#include "CORE/FW/register.h"
#include <stddef.h>
#include <stdio.h>

char rscid[48];

// Offset: 14
// Size: 176
br_resource_class* BrResClassAdd(br_resource_class* rclass) {
    br_resource_class* r;

    //printf("BrResClassAdd: id: %s, %d = %p\n", rclass->identifier, rclass->res_class, fw.resource_class_index[rclass->res_class]);

    r = BrRegistryAdd(&fw.reg_resource_classes, rclass);
    if (!r) {
        return NULL;
    }
    fw.resource_class_index[rclass->res_class] = r;
    return r;
}

// Offset: 207
// Size: 158
br_resource_class* BrResClassRemove(br_resource_class* rclass) {
    br_resource_class* r;
    NOT_IMPLEMENTED();
}

// Offset: 380
// Size: 49
br_resource_class* BrResClassFind(char* pattern) {
    NOT_IMPLEMENTED();
}

// Offset: 448
// Size: 55
br_resclass_find_cbfn* BrResClassFindHook(br_resclass_find_cbfn* hook) {
    br_resclass_find_cbfn* old;
    NOT_IMPLEMENTED();
}

// Offset: 521
// Size: 83
br_uint_32 BrResClassAddMany(br_resource_class** items, int n) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 625
// Size: 100
br_uint_32 BrResClassRemoveMany(br_resource_class** items, int n) {
    int i;
    int r;
    NOT_IMPLEMENTED();
}

// Offset: 744
// Size: 55
br_uint_32 BrResClassFindMany(char* pattern, br_resource_class** items, int max) {
    NOT_IMPLEMENTED();
}

// Offset: 815
// Size: 49
br_uint_32 BrResClassCount(char* pattern) {
    NOT_IMPLEMENTED();
}

// Offset: 879
// Size: 55
br_uint_32 BrResClassEnum(char* pattern, br_resclass_enum_cbfn* callback, void* arg) {
    NOT_IMPLEMENTED();
}
