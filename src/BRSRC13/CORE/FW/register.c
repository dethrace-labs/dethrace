#include "register.h"
#include "debug.h"

#include "brlists.h"
#include "fwsetup.h"
#include "pattern.h"
#include "resource.h"

char rscid[50];

// Offset: 14
// Size: 96
// EAX: reg
void* BrRegistryNew(br_registry* reg) {
    BrNewList(&reg->list);
    reg->count = 0;
}

// Offset: 126
// Size: 126
// EAX: reg
void* BrRegistryClear(br_registry* reg) {
    br_registry_entry* e;
    NOT_IMPLEMENTED();
}

// Offset: 266
// Size: 159
// EAX: reg
// EDX: item
void* BrRegistryAdd(br_registry* reg, void* item) {
    br_registry_entry* e;

    e = (br_registry_entry*)BrResAllocate(fw.res, sizeof(br_registry_entry), BR_MEMORY_REGISTRY);
    e->item = (char**)item;
    BrAddHead(&reg->list, &e->node);
    reg->count++;
    return item;
}

// Offset: 443
// Size: 148
// EAX: reg
// EDX: items
// EBX: n
int BrRegistryAddMany(br_registry* reg, void** items, int n) {
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 608
// Size: 198
// EAX: reg
// EDX: item
void* BrRegistryRemove(br_registry* reg, void* item) {
    br_registry_entry* e;
    void* r;
    NOT_IMPLEMENTED();
}

// Offset: 827
// Size: 171
// EAX: reg
// EDX: items
// EBX: n
int BrRegistryRemoveMany(br_registry* reg, void** items, int n) {
    int i;
    int r;
    NOT_IMPLEMENTED();
}

// Offset: 1013
// Size: 172
// EAX: reg
// EDX: pattern
void* BrRegistryFind(br_registry* reg, char* pattern) {
    br_registry_entry* e;

    e = (br_registry_entry*)reg->list.head;
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
// EAX: reg
// EDX: pattern
// EBX: items
// ECX: max
int BrRegistryFindMany(br_registry* reg, char* pattern, void** items, int max) {
    br_registry_entry* e;
    int n;
    NOT_IMPLEMENTED();
}

// Offset: 1360
// Size: 163
// EAX: reg
// EDX: pattern
int BrRegistryCount(br_registry* reg, char* pattern) {
    br_registry_entry* e;
    int n;
    NOT_IMPLEMENTED();
}

// Offset: 1538
// Size: 280
// EAX: reg
// EDX: pattern
// EBX: callback
// ECX: arg
int BrRegistryEnum(br_registry* reg, char* pattern, br_enum_cbfn* callback, void* arg) {
    br_registry_entry* e;
    int r;
    NOT_IMPLEMENTED();
}

// Offset: 1838
// Size: 49
// EAX: reg
// EDX: base
// EBX: limit
void* BrRegistryNewStatic(br_registry* reg, br_registry_entry* base, int limit) {
    NOT_IMPLEMENTED();
}

// Offset: 1907
// Size: 49
// EAX: reg
// EDX: base
// EBX: item
void* BrRegistryAddStatic(br_registry* reg, br_registry_entry* base, void* item) {
    NOT_IMPLEMENTED();
}

// Offset: 1979
// Size: 48
// EAX: reg
// EDX: item
void* BrRegistryRemoveStatic(br_registry* reg, void* item) {
    NOT_IMPLEMENTED();
}
