#include "resreg.h"

char rscid[48];

// Offset: 14
// Size: 176
br_resource_class* BrResClassAdd(br_resource_class *rclass) {
    br_resource_class *r;
}

// Offset: 207
// Size: 158
br_resource_class* BrResClassRemove(br_resource_class *rclass) {
    br_resource_class *r;
}

// Offset: 380
// Size: 49
br_resource_class* BrResClassFind(char *pattern) {
}

// Offset: 448
// Size: 55
br_resclass_find_cbfn* BrResClassFindHook(br_resclass_find_cbfn *hook) {
    br_resclass_find_cbfn *old;
}

// Offset: 521
// Size: 83
br_uint_32 BrResClassAddMany(br_resource_class **items, int n) {
    int i;
}

// Offset: 625
// Size: 100
br_uint_32 BrResClassRemoveMany(br_resource_class **items, int n) {
    int i;
    int r;
}

// Offset: 744
// Size: 55
br_uint_32 BrResClassFindMany(char *pattern, br_resource_class **items, int max) {
}

// Offset: 815
// Size: 49
br_uint_32 BrResClassCount(char *pattern) {
}

// Offset: 879
// Size: 55
br_uint_32 BrResClassEnum(char *pattern, br_resclass_enum_cbfn *callback, void *arg) {
}

