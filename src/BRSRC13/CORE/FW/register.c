#include "register.h"
#include "harness.h"

#include "brlists.h"
#include "fwsetup.h"
#include "pattern.h"
#include "resource.h"

char rscid[50];

void* BrRegistryNew(br_registry* reg) {
    BrNewList(&reg->list);
    reg->count = 0;
}

void* BrRegistryClear(br_registry* reg) {
    br_registry_entry* e;
    NOT_IMPLEMENTED();
}

void* BrRegistryAdd(br_registry* reg, void* item) {
    br_registry_entry* e;

    e = (br_registry_entry*)BrResAllocate(fw.res, sizeof(br_registry_entry), BR_MEMORY_REGISTRY);
    e->item = (char**)item;
    BrAddHead(&reg->list, (br_node*)e);
    reg->count++;
    return item;
}

int BrRegistryAddMany(br_registry* reg, void** items, int n) {
    int i;
    NOT_IMPLEMENTED();
}

void* BrRegistryRemove(br_registry* reg, void* item) {
    br_registry_entry* e;
    void* r;
    NOT_IMPLEMENTED();
}

int BrRegistryRemoveMany(br_registry* reg, void** items, int n) {
    int i;
    int r;
    NOT_IMPLEMENTED();
}

void* BrRegistryFind(br_registry* reg, char* pattern) {
    br_registry_entry* e;
    br_pixelmap* pm;
    LOG_TRACE8("(%p, \"%s\")", reg, pattern);

    e = (br_registry_entry*)reg->list.head;

    pm = (br_pixelmap*)e->item;

    if (e->item) {
        // as a char**, e->item[1] actually points to `identifier` field in a br_* struct etc
        while (!BrNamePatternMatch(pattern, e->item[1])) {
            e = (br_registry_entry*)e->node.next;
            if (!e->node.next) {
                if (reg->find_failed_hook) {
                    return reg->find_failed_hook(pattern);
                }
                return NULL;
            }
        }
        return e->item;
    }
    if (reg->find_failed_hook) {
        return reg->find_failed_hook(pattern);
    }
    return NULL;
}

int BrRegistryFindMany(br_registry* reg, char* pattern, void** items, int max) {
    br_registry_entry* e;
    int n;
    NOT_IMPLEMENTED();
}

int BrRegistryCount(br_registry* reg, char* pattern) {
    br_registry_entry* e;
    int n;
    NOT_IMPLEMENTED();
}

int BrRegistryEnum(br_registry* reg, char* pattern, br_enum_cbfn* callback, void* arg) {
    br_registry_entry* e;
    int r;

    e = (br_registry_entry*)reg->list.tail;
    if (!pattern) {
        if (e->node.prev) {
            while (1) {
                r = callback(e->item, arg);
                if (r) {
                    break;
                }
                e = (br_registry_entry*)e->node.prev;
                if (!e->node.prev) {
                    return 0;
                }
            }
            return 0;
        }
        return 0;
    }
    if (!e->node.prev) {
        return 0;
    }
    while (1) {
        // as a char**, e->item[1] actually points to `identifier` field in a br_* struct etc
        if (BrNamePatternMatch(pattern, e->item[1])) {
            r = callback(e->item, arg);
            if (r) {
                break;
            }
        }
        e = (br_registry_entry*)e->node.prev;
        if (!e->node.prev) {
            return 0;
        }
    }
    return 0;
}

void* BrRegistryNewStatic(br_registry* reg, br_registry_entry* base, int limit) {
    NOT_IMPLEMENTED();
}

void* BrRegistryAddStatic(br_registry* reg, br_registry_entry* base, void* item) {
    NOT_IMPLEMENTED();
}

void* BrRegistryRemoveStatic(br_registry* reg, void* item) {
    NOT_IMPLEMENTED();
}
