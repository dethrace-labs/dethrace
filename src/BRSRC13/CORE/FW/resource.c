#include "resource.h"

#include "brlists.h"
#include "brstdlib.h"
#include "file.h"
#include "fwsetup.h"
#include "harness.h"
#include "mem.h"
#include <stdio.h>
#include <stdlib.h>

char rscid[51];

// Offset: 10
// Size: 188
// EAX: r
void* ResToUser(resource_header* r) {
    br_int_32 align;
    NOT_IMPLEMENTED();
}

// Offset: 208
// Size: 72
// EAX: r
resource_header* UserToRes(void* r) {
    br_uint_8* p;
    NOT_IMPLEMENTED();
}

// Offset: 294
// Size: 459
void* BrResAllocate(void* vparent, br_size_t size, br_uint_8 res_class) {
    resource_header* res;
    resource_header* parent;
    br_int_32 malign;
    br_int_32 calign;
    br_int_32 pad;
    br_int_32 actual_pad;
    LOG_TRACE("(%p, %d, %d)", vparent, size, res_class);

    char* tmp;

    malign = BrMemAlign(res_class) - 1;
    calign = fw.resource_class_index[res_class]->alignment;
    if (calign <= 0) {
        calign = 4;
    }
    malign--;
    calign--;

    pad = (~malign & calign) + 3;
    actual_pad = (sizeof(resource_header) + pad) & 0xFFFC;

    // JeffH ignore calculated padding for simplicity. We dont care too much about
    // optimizing data alignment at this point ;)
    pad = 0;
    actual_pad = sizeof(resource_header) + pad;

    res = (resource_header*)BrMemAllocate(size + actual_pad, res_class);

    res->class = res_class;
    res->size_l = size >> 2;
    res->size_m = size >> 10;
    res->size_h = size >> 18;

    BrSimpleNewList(&res->children);
    res->magic_ptr = res;
    res->magic_num = 0xDEADBEEF;

    if (vparent) {
        // vparent points to a resource body, we track backwards to obtain its resource_header
        parent = (resource_header*)((char*)vparent - sizeof(resource_header));
        BrSimpleAddHead(&parent->children, &res->node);
    }
    return ((char*)res) + actual_pad;
}

// Offset: 771
// Size: 367
// EAX: res
// EDX: callback
void BrResInternalFree(resource_header* res, br_boolean callback) {
    int c;
    void* r;
    LOG_TRACE("(%p, %d)", res, callback);

    if (res->magic_num != 0xDEADBEEF) {
        LOG_PANIC("Bad resource header")
    }

    c = res->class;
    if (c != 127) {
        res->class = 127;
        if (callback && fw.resource_class_index[c]->free_cb) {
            // res+1 means we jump over the header to get to the resource data
            fw.resource_class_index[c]->free_cb(res + 1, c, (res->size_h << 18) | 4 * res->size_l | (res->size_m << 10));
        }

        while (res->children.head) {
            r = BrSimpleRemove(res->children.head);
            BrResInternalFree((resource_header*)r, 1);
        }
        if (res->node.prev) {
            BrSimpleRemove(&res->node);
        }
        res->magic_num = 1;
        res->magic_ptr = NULL;
        BrMemFree(res);
    }
}

// Offset: 1148
// Size: 79
void BrResFree(void* vres) {
    LOG_TRACE("(%p)", vres);

    vres = vres - sizeof(resource_header);
    if (((resource_header*)vres)->magic_num != 0xDEADBEEF) {
        LOG_PANIC("Bad resource header at %p", vres);
    }
    BrResInternalFree(vres, 1);
}

void BrResAssert(void* vres) {
    LOG_TRACE("(%p)", vres);

    vres = vres - sizeof(resource_header);

    if (((resource_header*)vres)->magic_num != 0xDEADBEEF) {
        LOG_PANIC("Bad resource header at %p. Was %X", vres, ((resource_header*)vres)->magic_num);
    }
}

// Offset: 1247
// Size: 76
void BrResFreeNoCallback(void* vres) {
    NOT_IMPLEMENTED();
}

// Offset: 1332
// Size: 259
void* BrResAdd(void* vparent, void* vres) {
    resource_header* res;
    resource_header* parent;
    NOT_IMPLEMENTED();
}

// Offset: 1603
// Size: 141
void* BrResRemove(void* vres) {
    resource_header* res;
    NOT_IMPLEMENTED();
}

// Offset: 1755
// Size: 132
br_uint_8 BrResClass(void* vres) {
    resource_header* res;
    NOT_IMPLEMENTED();
}

// Offset: 1900
// Size: 266
br_boolean BrResIsChild(void* vparent, void* vchild) {
    resource_header* parent;
    resource_header* child;
    resource_header* cp;
    NOT_IMPLEMENTED();
}

// Offset: 2176
// Size: 177
br_uint_32 BrResSize(void* vres) {
    resource_header* res;
    NOT_IMPLEMENTED();
}

// Offset: 2366
// Size: 80
br_uint_32 ResSizeTotal(void* vres, br_uint_32* ptotal) {
    NOT_IMPLEMENTED();
}

// Offset: 2461
// Size: 62
br_uint_32 BrResSizeTotal(void* vres) {
    br_uint_32 total;
    NOT_IMPLEMENTED();
}

// Offset: 2538
// Size: 226
br_uint_32 BrResChildEnum(void* vres, br_resenum_cbfn* callback, void* arg) {
    resource_header* res;
    resource_header* rp;
    br_uint_32 r;
    NOT_IMPLEMENTED();
}

// Offset: 2775
// Size: 91
br_uint_32 BrResCheck(void* vres, int no_tag) {
    resource_header* res;
    NOT_IMPLEMENTED();
}

// Offset: 2878
// Size: 127
char* BrResStrDup(void* vparent, char* str) {
    int l;
    char* nstr;

    l = BrStrLen(str);
    nstr = (char*)BrResAllocate(vparent, l + 1, BR_MEMORY_STRING);
    BrStrCpy(nstr, str);
    return nstr;
}

// Offset: 3026
// Size: 313
// EAX: res
// EDX: putline
// EBX: arg
// ECX: level
void InternalResourceDump(resource_header* res, br_putline_cbfn* putline, void* arg, int level) {
    int i;
    char* cp;
    resource_header* child;
    br_resource_class* rclass;
    NOT_IMPLEMENTED();
}

// Offset: 3349
// Size: 57
void BrResDump(void* vres, br_putline_cbfn* putline, void* arg) {
    resource_header* res;
    NOT_IMPLEMENTED();
}

// Offset: 3427
// Size: 80
char* BrResClassIdentifier(br_uint_8 res_class) {
    br_resource_class* rclass;
    NOT_IMPLEMENTED();
}
