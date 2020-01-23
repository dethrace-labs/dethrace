#include "resource.h"

#include "brlists.h"
#include "debug.h"
#include "fwsetup.h"
#include "mem.h"
#include <stdio.h>
#include <stdlib.h>

char rscid[51];

// Offset: 10
// Size: 188
// EAX: r
void* ResToUser(resource_header* r) {
    br_int_32 align;
}

// Offset: 208
// Size: 72
// EAX: r
resource_header* UserToRes(void* r) {
    br_uint_8* p;
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

    br_int_32 actual_pad_2;
    char* tmp;

    malign = BrMemAlign(res_class) - 1;
    //printf("BrResAllocate: parent: %p, size: %d, class: %d, class_index: %p\n", vparent, size, res_class, fw.resource_class_index[res_class]);
    calign = fw.resource_class_index[res_class]->alignment;
    if (calign <= 0) {
        calign = 4;
    }
    malign--;
    calign--;

    pad = (~malign & calign) + 3;
    actual_pad = (sizeof(resource_header) + pad) & 0xFFFC;
    actual_pad_2 = (size + sizeof(resource_header) + 3) & 0xFFFC;

    res = (resource_header*)BrMemAllocate(size + actual_pad, res_class);
    LOG_DEBUG("allocated r %p\n", res);
    // TOOD: ?
    // if ((signed int)(((unsigned int)((char*)allocated + res_align_1) & ~res_align_1) - (_DWORD)allocated) > v8)
    //     BrFailure((int)"Memory allocator broke alignment", v14);
    res->class = res_class;
    res->size_l = actual_pad_2 >> 2;
    res->size_m = actual_pad_2 >> 10;
    res->size_h = actual_pad_2 >> 18;

    BrSimpleNewList(&res->children);
    res->magic_ptr = res;
    res->magic_num = 0xDEADBEEF;

    if (vparent) {
        tmp = vparent;
        while (*(tmp - 1) == 0) {
            tmp--;
        }
        parent = (resource_header*)(tmp - sizeof(resource_header));
        BrSimpleAddHead(&parent->children, &res->node);
    }
    //printf("BrResAllocate returning res: %p, pad: %d, result: %p\n", res, pad, ((char*)res) + actual_pad);
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

    c = res->class;
    if (c != 127) {
        //v5 = fw.resource_class_index[c]->alignment;
        //if ((signed int)v5 <= 0)
        //    v5 = 4;
        res->class = 127;
        if (callback && fw.resource_class_index[c]->free_cb) {
            // TODO: we should return address of the user-facing data, not the resource_header
            //~(v5 - 1) & (unsigned int)((char*)&res->magic_num + v5 + 3),
            fw.resource_class_index[c]->free_cb(res, c, (res->size_h << 18) | 4 * res->size_l | (res->size_m << 10));
        }

        while (res->children.head) {
            r = BrSimpleRemove(res->children.head);
            BrResInternalFree((resource_header*)r, 1);
        }
        if (res->node.prev) {
            BrSimpleRemove(&res->node);
        }
        res->magic_num = 1;
        res->magic_ptr = 0;
        BrMemFree(res);
    }
}

// Offset: 1148
// Size: 79
void BrResFree(void* vres) {
    LOG_TRACE("(%p)", vres);

    // go backwards through padding until we hit the end of resource_header
    while (*(char*)vres == 0) {
        vres = (char*)vres - 1;
        LOG_DEBUG("jump");
    }
    // jump one more step backwards to start of resource_header
    vres = (char*)vres + 1;
    vres = ((resource_header*)vres - 1);
    //TODO assert magic_num
    BrResInternalFree(vres, 1);
}

// Offset: 1247
// Size: 76
void BrResFreeNoCallback(void* vres) {
}

// Offset: 1332
// Size: 259
void* BrResAdd(void* vparent, void* vres) {
    resource_header* res;
    resource_header* parent;
}

// Offset: 1603
// Size: 141
void* BrResRemove(void* vres) {
    resource_header* res;
}

// Offset: 1755
// Size: 132
br_uint_8 BrResClass(void* vres) {
    resource_header* res;
}

// Offset: 1900
// Size: 266
br_boolean BrResIsChild(void* vparent, void* vchild) {
    resource_header* parent;
    resource_header* child;
    resource_header* cp;
}

// Offset: 2176
// Size: 177
br_uint_32 BrResSize(void* vres) {
    resource_header* res;
}

// Offset: 2366
// Size: 80
br_uint_32 ResSizeTotal(void* vres, br_uint_32* ptotal) {
}

// Offset: 2461
// Size: 62
br_uint_32 BrResSizeTotal(void* vres) {
    br_uint_32 total;
}

// Offset: 2538
// Size: 226
br_uint_32 BrResChildEnum(void* vres, br_resenum_cbfn* callback, void* arg) {
    resource_header* res;
    resource_header* rp;
    br_uint_32 r;
}

// Offset: 2775
// Size: 91
br_uint_32 BrResCheck(void* vres, int no_tag) {
    resource_header* res;
}

// Offset: 2878
// Size: 127
char* BrResStrDup(void* vparent, char* str) {
    int l;
    char* nstr;
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
}

// Offset: 3349
// Size: 57
void BrResDump(void* vres, br_putline_cbfn* putline, void* arg) {
    resource_header* res;
}

// Offset: 3427
// Size: 80
char* BrResClassIdentifier(br_uint_8 res_class) {
    br_resource_class* rclass;
}
