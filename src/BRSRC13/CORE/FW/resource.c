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

// IDA: void* __usercall ResToUser@<EAX>(resource_header *r@<EAX>)
void* ResToUser(resource_header* r) {
    br_int_32 align;
    LOG_TRACE("(%p)", r);
    NOT_IMPLEMENTED();
}

// IDA: resource_header* __usercall UserToRes@<EAX>(void *r@<EAX>)
resource_header* UserToRes(void* r) {
    br_uint_8* p;
    LOG_TRACE("(%p)", r);
    NOT_IMPLEMENTED();
}

// IDA: void* __cdecl BrResAllocate(void *vparent, br_size_t size, br_uint_8 res_class)
void* BrResAllocate(void* vparent, br_size_t size, br_uint_8 res_class) {
    resource_header* res;
    resource_header* parent;
    br_int_32 malign;
    br_int_32 calign;
    br_int_32 pad;
    br_int_32 actual_pad;
    LOG_TRACE9("(%p, %d, %d)", vparent, size, res_class);

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

    // Jeff ignore calculated padding for simplicity. We dont care too much about
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

// IDA: void __usercall BrResInternalFree(resource_header *res@<EAX>, br_boolean callback@<EDX>)
void BrResInternalFree(resource_header* res, br_boolean callback) {
    int c;
    void* r;
    LOG_TRACE10("(%p, %d)", res, callback);

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

// IDA: void __cdecl BrResFree(void *vres)
void BrResFree(void* vres) {
    LOG_TRACE10("(%p)", vres);

    vres = (char*)vres - sizeof(resource_header);
    if (((resource_header*)vres)->magic_num != 0xDEADBEEF) {
        LOG_PANIC("Bad resource header at %p", vres);
    }
    BrResInternalFree(vres, 1);
}

void BrResAssert(void* vres) {
    LOG_TRACE("(%p)", vres);

    vres = (char*)vres - sizeof(resource_header);

    if (((resource_header*)vres)->magic_num != 0xDEADBEEF) {
        LOG_PANIC("Bad resource header at %p. Was %X", vres, ((resource_header*)vres)->magic_num);
    }
}

// IDA: void __cdecl BrResFreeNoCallback(void *vres)
void BrResFreeNoCallback(void* vres) {
    LOG_TRACE("(%p)", vres);
    NOT_IMPLEMENTED();
}

// IDA: void* __cdecl BrResAdd(void *vparent, void *vres)
void* BrResAdd(void* vparent, void* vres) {
    resource_header* res;
    resource_header* parent;
    LOG_TRACE("(%p, %p)", vparent, vres);
    NOT_IMPLEMENTED();
}

// IDA: void* __cdecl BrResRemove(void *vres)
void* BrResRemove(void* vres) {
    resource_header* res;
    LOG_TRACE("(%p)", vres);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_8 __cdecl BrResClass(void *vres)
br_uint_8 BrResClass(void* vres) {
    resource_header* res;
    LOG_TRACE("(%p)", vres);
    NOT_IMPLEMENTED();
}

// IDA: br_boolean __cdecl BrResIsChild(void *vparent, void *vchild)
br_boolean BrResIsChild(void* vparent, void* vchild) {
    resource_header* parent;
    resource_header* child;
    resource_header* cp;
    LOG_TRACE("(%p, %p)", vparent, vchild);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrResSize(void *vres)
br_uint_32 BrResSize(void* vres) {
    resource_header* res;
    LOG_TRACE("(%p)", vres);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl ResSizeTotal(void *vres, br_uint_32 *ptotal)
br_uint_32 ResSizeTotal(void* vres, br_uint_32* ptotal) {
    LOG_TRACE("(%p, %p)", vres, ptotal);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrResSizeTotal(void *vres)
br_uint_32 BrResSizeTotal(void* vres) {
    br_uint_32 total;
    LOG_TRACE("(%p)", vres);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrResChildEnum(void *vres, br_resenum_cbfn *callback, void *arg)
br_uint_32 BrResChildEnum(void* vres, br_resenum_cbfn* callback, void* arg) {
    resource_header* res;
    resource_header* rp;
    br_uint_32 r;
    LOG_TRACE("(%p, %p, %p)", vres, callback, arg);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrResCheck(void *vres, int no_tag)
br_uint_32 BrResCheck(void* vres, int no_tag) {
    resource_header* res;
    LOG_TRACE("(%p, %d)", vres, no_tag);
    NOT_IMPLEMENTED();
}

// IDA: char* __cdecl BrResStrDup(void *vparent, char *str)
char* BrResStrDup(void* vparent, char* str) {
    int l;
    char* nstr;

    l = BrStrLen(str);
    nstr = (char*)BrResAllocate(vparent, l + 1, BR_MEMORY_STRING);
    BrStrCpy(nstr, str);
    return nstr;
}

// IDA: void __usercall InternalResourceDump(resource_header *res@<EAX>, br_putline_cbfn *putline@<EDX>, void *arg@<EBX>, int level@<ECX>)
void InternalResourceDump(resource_header* res, br_putline_cbfn* putline, void* arg, int level) {
    int i;
    char* cp;
    resource_header* child;
    br_resource_class* rclass;
    LOG_TRACE("(%p, %p, %p, %d)", res, putline, arg, level);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrResDump(void *vres, br_putline_cbfn *putline, void *arg)
void BrResDump(void* vres, br_putline_cbfn* putline, void* arg) {
    resource_header* res;
    LOG_TRACE("(%p, %p, %p)", vres, putline, arg);
    NOT_IMPLEMENTED();
}

// IDA: char* __cdecl BrResClassIdentifier(br_uint_8 res_class)
char* BrResClassIdentifier(br_uint_8 res_class) {
    br_resource_class* rclass;
    LOG_TRACE("(%d)", res_class);
    NOT_IMPLEMENTED();
}
