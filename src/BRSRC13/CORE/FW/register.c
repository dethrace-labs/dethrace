#include "register.h"
#include "harness/trace.h"

#include "brlists.h"
#include "fwsetup.h"
#include "pattern.h"
#include "resource.h"

// IDA: void* __usercall BrRegistryNew@<EAX>(br_registry *reg@<EAX>)
void* BrRegistryNew(br_registry* reg) {
    LOG_TRACE10("(%p)", reg);

    BrNewList(&reg->list);
    reg->count = 0;
    return reg;
}

// IDA: void* __usercall BrRegistryClear@<EAX>(br_registry *reg@<EAX>)
void* BrRegistryClear(br_registry* reg) {
    br_registry_entry* e;
    LOG_TRACE10("(%p)", reg);

    e = (br_registry_entry*)reg->list.head;
    while (e->node.next != NULL) {
        BrRemove(&e->node);
        BrResFree(e);
        e = (br_registry_entry*)reg->list.head;
    }
    reg->count = 0;
    return reg;
}

// IDA: void* __usercall BrRegistryAdd@<EAX>(br_registry *reg@<EAX>, void *item@<EDX>)
void* BrRegistryAdd(br_registry* reg, void* item) {
    br_registry_entry* e;
    LOG_TRACE10("(%p, %p)", reg, item);

    e = (br_registry_entry*)BrResAllocate(fw.res, sizeof(br_registry_entry), BR_MEMORY_REGISTRY);
    e->item = (char**)item;
    BrAddHead(&reg->list, (br_node*)e);
    reg->count++;
    return item;
}

// IDA: int __usercall BrRegistryAddMany@<EAX>(br_registry *reg@<EAX>, void **items@<EDX>, int n@<EBX>)
int BrRegistryAddMany(br_registry* reg, void** items, int n) {
    br_registry_entry* e;
    int i;
    LOG_TRACE10("(%p, %p, %d)", reg, items, n);

    for(i = 0; i < n; i++) {
        e = BrResAllocate(fw.res, sizeof(br_registry_entry), BR_MEMORY_REGISTRY);
        e->item = ((char ***)items)[i];
        BrAddHead(&reg->list, (br_node *)&e->node);
        reg->count++;
    }
    return n;
}

// IDA: void* __usercall BrRegistryRemove@<EAX>(br_registry *reg@<EAX>, void *item@<EDX>)
void* BrRegistryRemove(br_registry* reg, void* item) {
    br_registry_entry* e;
    void* r;
    LOG_TRACE10("(%p, %p)", reg, item);

    e = (br_registry_entry*)reg->list.head;
    while ((e->node.next != NULL) && (e->item != item)) {
        e = (br_registry_entry*)e->node.next;
    }
    if (e->node.next == NULL) {
        return NULL;
    }
    BrRemove((br_node*)e);
    r = e->item;
    BrResFree(e);
    reg->count--;
    return r;
}

// IDA: int __usercall BrRegistryRemoveMany@<EAX>(br_registry *reg@<EAX>, void **items@<EDX>, int n@<EBX>)
int BrRegistryRemoveMany(br_registry* reg, void** items, int n) {
    int i;
    int r;
    LOG_TRACE("(%p, %p, %d)", reg, items, n);

    r = 0;
    for (; n != 0; n--) {
        if (BrRegistryRemove(reg, *items) != NULL) {
            r++;
        }
        items++;
    }
    return r;
}

// IDA: void* __usercall BrRegistryFind@<EAX>(br_registry *reg@<EAX>, char *pattern@<EDX>)
void* BrRegistryFind(br_registry* reg, char* pattern) {
    br_registry_entry* e;
    LOG_TRACE8("(%p, \"%s\")", reg, pattern);

    e = (br_registry_entry*)reg->list.head;
    while (e->node.next != NULL) {
        // 2nd element of item must be char pointer.
        if (BrNamePatternMatch(pattern, e->item[1]) != 0) {
            return e->item;
        }
        e = (br_registry_entry*)e->node.next;
    }
    if (reg->find_failed_hook != NULL) {
        return reg->find_failed_hook(pattern);
    }
    return NULL;
}

// IDA: int __usercall BrRegistryFindMany@<EAX>(br_registry *reg@<EAX>, char *pattern@<EDX>, void **items@<EBX>, int max@<ECX>)
int BrRegistryFindMany(br_registry* reg, char* pattern, void** items, int max) {
    br_registry_entry* e;
    int n;
    LOG_TRACE("(%p, \"%s\", %p, %d)", reg, pattern, items, max);

    n = 0;
    e = (br_registry_entry*)reg->list.head;
    while ((e->node.next != NULL) && (n < max)) {
        if (BrNamePatternMatch(pattern, e->item[1]) != 0) {
            *items = e->item;
            items++;
            n++;
        }
        e = (br_registry_entry*)e->node.next;
    }
    return n;
}

// IDA: int __usercall BrRegistryCount@<EAX>(br_registry *reg@<EAX>, char *pattern@<EDX>)
int BrRegistryCount(br_registry* reg, char* pattern) {
    br_registry_entry* e;
    int n;
    LOG_TRACE("(%p, \"%s\")", reg, pattern);

    if (pattern == NULL) {
        return reg->count;
    }
    n = 0;
    e = (br_registry_entry*)reg->list.head;
    while (e->node.next != NULL) {
        if (BrNamePatternMatch(pattern, e->item[1]) != 0) {
            n++;
        }
        e = (br_registry_entry*)e->node.next;
    }
    return n;
}

// IDA: int __usercall BrRegistryEnum@<EAX>(br_registry *reg@<EAX>, char *pattern@<EDX>, br_enum_cbfn *callback@<EBX>, void *arg@<ECX>)
int BrRegistryEnum(br_registry* reg, char* pattern, br_enum_cbfn* callback, void* arg) {
    br_registry_entry* e;
    int r;
    LOG_TRACE("(%p, \"%s\", %p, %p)", reg, pattern, callback, arg);

    if (pattern == NULL) {
        e = (br_registry_entry*)reg->list.tail;
        while (e->node.prev != NULL) {
            r = callback(e->item, arg);
            if (r != 0) {
                return r;
            }
            e = (br_registry_entry*)e->node.prev;
        }
    } else {
        e = (br_registry_entry*)reg->list.tail;
        while (e->node.prev != NULL) {
            // as a char**, e->item[1] actually points to `identifier` field in a br_* struct etc
            r = BrNamePatternMatch(pattern, e->item[1]);
            if (r != 0) {
                r = callback(e->item, arg);
                if (r != 0) {
                    return r;
                }
            }
            e = (br_registry_entry*)e->node.prev;
        }
    }
    return 0;
}

// IDA: void* __usercall BrRegistryNewStatic@<EAX>(br_registry *reg@<EAX>, br_registry_entry *base@<EDX>, int limit@<EBX>)
void* BrRegistryNewStatic(br_registry* reg, br_registry_entry* base, int limit) {
    LOG_TRACE("(%p, %p, %d)", reg, base, limit);
    return NULL;
}

// IDA: void* __usercall BrRegistryAddStatic@<EAX>(br_registry *reg@<EAX>, br_registry_entry *base@<EDX>, void *item@<EBX>)
void* BrRegistryAddStatic(br_registry* reg, br_registry_entry* base, void* item) {
    LOG_TRACE("(%p, %p, %p)", reg, base, item);
    return NULL;
}

// IDA: void* __usercall BrRegistryRemoveStatic@<EAX>(br_registry *reg@<EAX>, void *item@<EDX>)
void* BrRegistryRemoveStatic(br_registry* reg, void* item) {
    LOG_TRACE("(%p, %p)", reg, item);
    return NULL;
}
