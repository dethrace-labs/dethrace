#include "register.h"
#include "harness.h"

#include "brlists.h"
#include "fwsetup.h"
#include "pattern.h"
#include "resource.h"

char rscid[50];

// Offset: 14
// Size: 96
void* BrRegistryNew(br_registry* reg) {
    BrNewList(&reg->list);
    reg->count = 0;
}

// Offset: 126
// Size: 126
void* BrRegistryClear(br_registry* reg) {
    br_registry_entry* e;
    NOT_IMPLEMENTED();
}

// Offset: 266
// Size: 159
void* BrRegistryAdd(br_registry* reg, void* item) {
    br_registry_entry* e;

    e = (br_registry_entry*)BrResAllocate(fw.res, sizeof(br_registry_entry), BR_MEMORY_REGISTRY);
    e->item = (char**)item;
    BrAddHead(&reg->list, (br_node*)e);
    reg->count++;
    return item;
}

// Offset: 443
// Size: 148
int BrRegistryAddMany(br_registry* reg, void** items, int n) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 608
// Size: 198
void* BrRegistryRemove(br_registry* reg, void* item) {
    br_registry_entry* e;
    void* r;
    NOT_IMPLEMENTED();
}

// Offset: 827
// Size: 171
int BrRegistryRemoveMany(br_registry* reg, void** items, int n) {
    int i;
    int r;
    NOT_IMPLEMENTED();
}

// Offset: 1013
// Size: 172
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

// Offset: 1204
// Size: 140
int BrRegistryFindMany(br_registry* reg, char* pattern, void** items, int max) {
    br_registry_entry* e;
    int n;
    NOT_IMPLEMENTED();
}

// Offset: 1360
// Size: 163
int BrRegistryCount(br_registry* reg, char* pattern) {
    br_registry_entry* e;
    int n;
    NOT_IMPLEMENTED();
}

// Offset: 1538
// Size: 280
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

// Offset: 1838
// Size: 49
void* BrRegistryNewStatic(br_registry* reg, br_registry_entry* base, int limit) {
    NOT_IMPLEMENTED();
}

// Offset: 1907
// Size: 49
void* BrRegistryAddStatic(br_registry* reg, br_registry_entry* base, void* item) {
    NOT_IMPLEMENTED();
}

// Offset: 1979
// Size: 48
void* BrRegistryRemoveStatic(br_registry* reg, void* item) {
    NOT_IMPLEMENTED();
}
