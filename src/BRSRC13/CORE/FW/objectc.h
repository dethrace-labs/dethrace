#ifndef _OBJECTC_H_
#define _OBJECTC_H_

#include "br_types.h"

// Offset: 21
// Size: 74
void* BrObjectListAllocate(void* res);

// Offset: 127
// Size: 136
br_error _M_br_object_container_addFront(br_object_container* self, br_object* ph);

// Offset: 293
// Size: 160
br_error _M_br_object_container_remove(br_object_container* self, br_object* h);

// Offset: 488
// Size: 135
br_error _M_br_object_container_removeFront(br_object_container* self, br_object** ph);

// Offset: 651
// Size: 300
br_error _M_br_object_container_find(br_object_container* self, br_object** ph, br_token type, char* pattern, br_token_value* tv);

// Offset: 983
// Size: 337
br_error _M_br_object_container_findMany(br_object_container* self, br_object** objects, br_int_32 max_objects, br_int_32* num_objects, br_token type, char* pattern, br_token_value* tv);

// Offset: 1349
// Size: 301
br_error _M_br_object_container_count(br_object_container* self, br_uint_32* pcount, br_token type, char* pattern, br_token_value* tv);

// Offset: 1690
// Size: 194
void* _M_br_object_container_tokensMatchBegin(br_object_container* self, br_token t, br_token_value* tv);

// Offset: 1919
// Size: 214
br_boolean _M_br_object_container_tokensMatch(br_object_container* self, br_object* h, void* arg);

// Offset: 2171
// Size: 48
void _M_br_object_container_tokensMatchEnd(br_object_container* self, void* arg);

// Offset: 2241
// Size: 270
br_error BrObjectContainerFree(br_object_container* self, br_token type, char* pattern, br_token_value* tv);

#endif
