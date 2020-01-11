#include "objectc.h"

char rscid[49];

// Offset: 21
// Size: 74
void* BrObjectListAllocate(void* res) {
    object_list* hl;
}

// Offset: 127
// Size: 136
br_error _M_br_object_container_addFront(br_object_container* self, br_object* ph) {
    object_list* hl;
    object_list_entry* he;
}

// Offset: 293
// Size: 160
br_error _M_br_object_container_remove(br_object_container* self, br_object* h) {
    object_list* hl;
    object_list_entry* he;
}

// Offset: 488
// Size: 135
br_error _M_br_object_container_removeFront(br_object_container* self, br_object** ph) {
    object_list* hl;
    object_list_entry* he;
}

// Offset: 651
// Size: 300
br_error _M_br_object_container_find(br_object_container* self, br_object** ph, br_token type, char* pattern, br_token_value* tv) {
    object_list* hl;
    object_list_entry* he;
    void* tvarg;
    br_error r;
}

// Offset: 983
// Size: 337
br_error _M_br_object_container_findMany(br_object_container* self, br_object** objects, br_int_32 max_objects, br_int_32* num_objects, br_token type, char* pattern, br_token_value* tv) {
    object_list* hl;
    object_list_entry* he;
    void* tvarg;
    int n;
}

// Offset: 1349
// Size: 301
br_error _M_br_object_container_count(br_object_container* self, br_uint_32* pcount, br_token type, char* pattern, br_token_value* tv) {
    object_list* hl;
    object_list_entry* he;
    void* tvarg;
    int n;
}

// Offset: 1690
// Size: 194
void* _M_br_object_container_tokensMatchBegin(br_object_container* self, br_token t, br_token_value* tv) {
    token_match* tm;
    br_int_32 i;
}

// Offset: 1919
// Size: 214
br_boolean _M_br_object_container_tokensMatch(br_object_container* self, br_object* h, void* arg) {
    token_match* tm;
    br_size_t s;
    br_int_32 n;
}

// Offset: 2171
// Size: 48
void _M_br_object_container_tokensMatchEnd(br_object_container* self, void* arg) {
}

// Offset: 2241
// Size: 270
br_error BrObjectContainerFree(br_object_container* self, br_token type, char* pattern, br_token_value* tv) {
    br_error r;
    br_object** handles;
    br_int_32 count;
    br_int_32 n;
    br_int_32 i;
}
