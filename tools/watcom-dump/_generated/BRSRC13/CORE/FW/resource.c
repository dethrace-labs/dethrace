#include "resource.h"

char rscid[51];

// Offset: 10
// Size: 188
// EAX: r
void* ResToUser(resource_header *r) {
    br_int_32 align;
}

// Offset: 208
// Size: 72
// EAX: r
resource_header* UserToRes(void *r) {
    br_uint_8 *p;
}

// Offset: 294
// Size: 459
void* BrResAllocate(void *vparent, br_size_t size, br_uint_8 res_class) {
    resource_header *res;
    resource_header *parent;
    br_int_32 malign;
    br_int_32 calign;
    br_int_32 pad;
    br_int_32 actual_pad;
}

// Offset: 771
// Size: 367
// EAX: res
// EDX: callback
void BrResInternalFree(resource_header *res, br_boolean callback) {
    int c;
    void *r;
}

// Offset: 1148
// Size: 79
void BrResFree(void *vres) {
}

// Offset: 1247
// Size: 76
void BrResFreeNoCallback(void *vres) {
}

// Offset: 1332
// Size: 259
void* BrResAdd(void *vparent, void *vres) {
    resource_header *res;
    resource_header *parent;
}

// Offset: 1603
// Size: 141
void* BrResRemove(void *vres) {
    resource_header *res;
}

// Offset: 1755
// Size: 132
br_uint_8 BrResClass(void *vres) {
    resource_header *res;
}

// Offset: 1900
// Size: 266
br_boolean BrResIsChild(void *vparent, void *vchild) {
    resource_header *parent;
    resource_header *child;
    resource_header *cp;
}

// Offset: 2176
// Size: 177
br_uint_32 BrResSize(void *vres) {
    resource_header *res;
}

// Offset: 2366
// Size: 80
br_uint_32 ResSizeTotal(void *vres, br_uint_32 *ptotal) {
}

// Offset: 2461
// Size: 62
br_uint_32 BrResSizeTotal(void *vres) {
    br_uint_32 total;
}

// Offset: 2538
// Size: 226
br_uint_32 BrResChildEnum(void *vres, br_resenum_cbfn *callback, void *arg) {
    resource_header *res;
    resource_header *rp;
    br_uint_32 r;
}

// Offset: 2775
// Size: 91
br_uint_32 BrResCheck(void *vres, int no_tag) {
    resource_header *res;
}

// Offset: 2878
// Size: 127
char* BrResStrDup(void *vparent, char *str) {
    int l;
    char *nstr;
}

// Offset: 3026
// Size: 313
// EAX: res
// EDX: putline
// EBX: arg
// ECX: level
void InternalResourceDump(resource_header *res, br_putline_cbfn *putline, void *arg, int level) {
    int i;
    char *cp;
    resource_header *child;
    br_resource_class *rclass;
}

// Offset: 3349
// Size: 57
void BrResDump(void *vres, br_putline_cbfn *putline, void *arg) {
    resource_header *res;
}

// Offset: 3427
// Size: 80
char* BrResClassIdentifier(br_uint_8 res_class) {
    br_resource_class *rclass;
}

