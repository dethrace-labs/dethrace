#include "objectc.h"
#include "harness.h"

char rscid[49];

void* BrObjectListAllocate(void* res) {
    object_list* hl;
    NOT_IMPLEMENTED();
}

br_error _M_br_object_container_addFront(br_object_container* self, br_object* ph) {
    object_list* hl;
    object_list_entry* he;
    NOT_IMPLEMENTED();
}

br_error _M_br_object_container_remove(br_object_container* self, br_object* h) {
    object_list* hl;
    object_list_entry* he;
    NOT_IMPLEMENTED();
}

br_error _M_br_object_container_removeFront(br_object_container* self, br_object** ph) {
    object_list* hl;
    object_list_entry* he;
    NOT_IMPLEMENTED();
}

br_error _M_br_object_container_find(br_object_container* self, br_object** ph, br_token type, char* pattern, br_token_value* tv) {
    object_list* hl;
    object_list_entry* he;
    void* tvarg;
    br_error r;
    NOT_IMPLEMENTED();
}

br_error _M_br_object_container_findMany(br_object_container* self, br_object** objects, br_int_32 max_objects, br_int_32* num_objects, br_token type, char* pattern, br_token_value* tv) {
    object_list* hl;
    object_list_entry* he;
    void* tvarg;
    int n;
    NOT_IMPLEMENTED();
}

br_error _M_br_object_container_count(br_object_container* self, br_uint_32* pcount, br_token type, char* pattern, br_token_value* tv) {
    object_list* hl;
    object_list_entry* he;
    void* tvarg;
    int n;
    NOT_IMPLEMENTED();
}

void* _M_br_object_container_tokensMatchBegin(br_object_container* self, br_token t, br_token_value* tv) {
    token_match* tm;
    br_int_32 i;
    NOT_IMPLEMENTED();
}

br_boolean _M_br_object_container_tokensMatch(br_object_container* self, br_object* h, void* arg) {
    token_match* tm;
    br_size_t s;
    br_int_32 n;
    NOT_IMPLEMENTED();
}

void _M_br_object_container_tokensMatchEnd(br_object_container* self, void* arg) {
    NOT_IMPLEMENTED();
}

br_error BrObjectContainerFree(br_object_container* self, br_token type, char* pattern, br_token_value* tv) {
    br_error r;
    br_object** handles;
    br_int_32 count;
    br_int_32 n;
    br_int_32 i;
    NOT_IMPLEMENTED();
}
