#include "resource.h"

#include "CORE/FW/brlists.h"
#include "CORE/FW/diag.h"
#include "CORE/FW/fwsetup.h"
#include "CORE/FW/mem.h"
#include "CORE/FW/scratch.h"
#include "CORE/STD/brstdlib.h"
#include "harness/trace.h"
#include <stdio.h>
#include <stdlib.h>

#define RES_ALIGN 4
#define RESOURCE_SIZE(RES) ((RES)->size_h << 18)  | ((RES)->size_m << 10) | ((RES)->size_l << 2)

// IDA: void* __usercall ResToUser@<EAX>(resource_header *r@<EAX>)
void* ResToUser(resource_header* r) {
    br_int_32 align;
    LOG_TRACE9("(%p)", r);

    align = fw.resource_class_index[r->class]->alignment;
    if (align <= 0) {
        align = RES_ALIGN;
    }
    return ((br_size_t)r + sizeof(resource_header) + align - 1) & ~(align - 1);
}

// IDA: resource_header* __usercall UserToRes@<EAX>(void *r@<EAX>)
resource_header* UserToRes(void* r) {
    br_uint_8* p;
    LOG_TRACE9("(%p)", r);

    p = (br_uint_8*)r - 1;
    while (*p == 0) {
        p--;
    }
    if (((resource_header*)(p - (sizeof(resource_header) - 1)))->magic_num != 0xdeadbeef) {
        LOG_PANIC("Bad resource header from user at %p. Was %X", r, ((resource_header*)p)->magic_num);
    }
    return (resource_header*)(p - (sizeof(resource_header) - 1));
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

    size = (size + sizeof(resource_header) + RES_ALIGN - 1) & ~(RES_ALIGN - 1);

    malign = BrMemAlign(res_class);

    calign = fw.resource_class_index[res_class]->alignment;
    if (calign <= 0) {
        calign = RES_ALIGN;
    }

    pad = (~(malign - 1)) & (calign - 1);
    res = (resource_header*)BrMemAllocate(pad + size, res_class);
    actual_pad = ((((br_size_t)res + calign - 1) & (~(calign - 1))) - (br_size_t)res);
    if (actual_pad > pad) {
        BrFailure("Memory allocator broke alignment");
    }
    res->class = res_class;
    res->size_l = size >> 2;
    res->size_m = size >> 10;
    res->size_h = size >> 18;
    BrSimpleNewList(&res->children);
    res->magic_num = 0xdeadbeef;
    res->magic_ptr = res;

    if (vparent != NULL) {
        // vparent points to a resource body, we track backwards to obtain its resource_header
        parent = UserToRes(vparent);
        BrSimpleAddHead(&parent->children, &res->node);
    }

    return ResToUser(res);
}

// IDA: void __usercall BrResInternalFree(resource_header *res@<EAX>, br_boolean callback@<EDX>)
void BrResInternalFree(resource_header* res, br_boolean callback) {
    int c;
    void* r;
    LOG_TRACE10("(%p, %d)", res, callback);

    if (res->class == BR_MEMORY_FREE) {
        return;
    }
    if (callback != 0) {
        if (fw.resource_class_index[res->class]->free_cb != NULL) {
            fw.resource_class_index[res->class]->free_cb(ResToUser(res), res->class, RESOURCE_SIZE(res));
        }
    }

    while (res->children.head != NULL) {
        r = BrSimpleRemove(res->children.head);
        BrResInternalFree((resource_header*)r, 1);
    }
    if (res->node.prev != NULL) {
        BrSimpleRemove(&res->node);
    }
    res->magic_num = 1;
    res->magic_ptr = NULL;
    BrMemFree(res);
}

// IDA: void __cdecl BrResFree(void *vres)
void BrResFree(void* vres) {
    LOG_TRACE10("(%p)", vres);

    BrResInternalFree(UserToRes(vres), 1);
}

// IDA: void __cdecl BrResFreeNoCallback(void *vres)
void BrResFreeNoCallback(void* vres) {
    LOG_TRACE("(%p)", vres);

    BrResInternalFree(UserToRes(vres), 0);
}

// IDA: void* __cdecl BrResAdd(void *vparent, void *vres)
void* BrResAdd(void* vparent, void* vres) {
    resource_header* res;
    resource_header* parent;
    LOG_TRACE("(%p, %p)", vparent, vres);

    res = UserToRes(vres);
    parent = UserToRes(vparent);

    if (res->node.prev != NULL) {
        BrSimpleRemove(&res->node);
    }
    BrSimpleAddHead(&parent->children, &res->node);
    return vres;
}

// IDA: void* __cdecl BrResRemove(void *vres)
void* BrResRemove(void* vres) {
    resource_header* res;
    LOG_TRACE("(%p)", vres);

    res = UserToRes(vres);
    BrSimpleRemove(&res->node);
    return vres;
}

// IDA: br_uint_8 __cdecl BrResClass(void *vres)
br_uint_8 BrResClass(void* vres) {
    resource_header* res;
    LOG_TRACE("(%p)", vres);

    res = UserToRes(vres);
    return res->class;
}

// IDA: br_boolean __cdecl BrResIsChild(void *vparent, void *vchild)
br_boolean BrResIsChild(void* vparent, void* vchild) {
    resource_header* parent;
    resource_header* child;
    resource_header* cp;
    LOG_TRACE("(%p, %p)", vparent, vchild);

    parent = UserToRes(vparent);
    child = UserToRes(vchild);

    for (cp = (resource_header*)parent->children.head; cp != NULL; cp = (resource_header*)cp->node.next) {
        if (cp == child) {
            return 1;
        }
    }
    return 0;
}

// IDA: br_uint_32 __cdecl BrResSize(void *vres)
br_uint_32 BrResSize(void* vres) {
    resource_header* res;
    LOG_TRACE("(%p)", vres);

    res = UserToRes(vres);
    return RESOURCE_SIZE(res);
}

// IDA: br_uint_32 __cdecl ResSizeTotal(void *vres, br_uint_32 *ptotal)
br_uint_32 ResSizeTotal(void* vres, br_uint_32* ptotal) {
    LOG_TRACE("(%p, %p)", vres, ptotal);

    *ptotal += BrResSize(vres);
    BrResChildEnum(vres, (br_resenum_cbfn*)ResSizeTotal, ptotal);
    return 0;
}

// IDA: br_uint_32 __cdecl BrResSizeTotal(void *vres)
br_uint_32 BrResSizeTotal(void* vres) {
    br_uint_32 total;
    LOG_TRACE("(%p)", vres);

    total = BrResSize(vres);
    BrResChildEnum(vres, (br_resenum_cbfn*)ResSizeTotal, &total);
    return total;
}

// IDA: br_uint_32 __cdecl BrResChildEnum(void *vres, br_resenum_cbfn *callback, void *arg)
br_uint_32 BrResChildEnum(void* vres, br_resenum_cbfn* callback, void* arg) {
    resource_header* res;
    resource_header* rp;
    br_uint_32 r;
    LOG_TRACE("(%p, %p, %p)", vres, callback, arg);

    res = UserToRes(vres);
    for (rp = (resource_header*)res->children.head; rp != NULL; rp = (resource_header*)rp->node.next) {
        r = callback(ResToUser(rp), arg);
        if (r != 0) {
            break;
        }
    }
    return r;
}

// IDA: br_uint_32 __cdecl BrResCheck(void *vres, int no_tag)
br_uint_32 BrResCheck(void* vres, int no_tag) {
    resource_header* res;
    LOG_TRACE("(%p, %d)", vres, no_tag);

    res = UserToRes(vres);
    if ((res->magic_ptr ==  res) & (res->magic_num == 0xdeadbeef)) {
        return 1;
    }
    return 0;
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
    LOG_TRACE9("(%p, %p, %p, %d)", res, putline, arg, level);

    rclass = fw.resource_class_index[res->class];
    cp = BrScratchString();
    for (i = 0; i < level; i++) {
        *cp = ' ';
        cp++;
    }
    sprintf(cp, "%p [%s] size=%u", res, rclass->identifier, RESOURCE_SIZE(res));
    putline(BrScratchString(), arg);
    for (child = (resource_header*)res->children.head; child != NULL; child = (resource_header*)child->node.next) {
        InternalResourceDump(child, putline, arg, level + 1);
    }
}

// IDA: void __cdecl BrResDump(void *vres, br_putline_cbfn *putline, void *arg)
void BrResDump(void* vres, br_putline_cbfn* putline, void* arg) {
    resource_header* res;
    LOG_TRACE("(%p, %p, %p)", vres, putline, arg);

    res = UserToRes(vres);
    InternalResourceDump(res, putline, arg, 0);
}

// IDA: char* __cdecl BrResClassIdentifier(br_uint_8 res_class)
char* BrResClassIdentifier(br_uint_8 res_class) {
    br_resource_class* rclass;
    LOG_TRACE("(%d)", res_class);

    rclass = fw.resource_class_index[res_class];
    if (rclass == NULL) {
        return "<NULL>";
    }
    return rclass->identifier;
}
