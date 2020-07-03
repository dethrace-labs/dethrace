#include "assocarr.h"
#include "harness.h"

// Offset: 27
// Size: 145
br_associative_array* BrAssociativeArrayAllocate() {
    br_associative_array* pArray;
    NOT_IMPLEMENTED();
}

// Offset: 200
// Size: 207
// EAX: pArray
// EDX: index
// EBX: v
br_error Set_Associative_Array_Value(br_associative_array* pArray, int index, br_value v) {
    NOT_IMPLEMENTED();
}

// Offset: 434
// Size: 535
// EAX: pArray
// EDX: t
// EBX: v
br_error BrAssociativeArraySetEntry(br_associative_array* pArray, br_token t, br_value v) {
    br_uint_16 i;
    br_token_value* temp;
    NOT_IMPLEMENTED();
}

// Offset: 999
// Size: 363
// EAX: pArray
// EDX: t
br_error BrAssociativeArrayRemoveEntry(br_associative_array* pArray, br_token t) {
    br_uint_16 i;
    br_boolean bFound;
    NOT_IMPLEMENTED();
}

// Offset: 1386
// Size: 202
// EAX: pArray
// EDX: t
// EBX: pValue
br_error BrAssociativeArrayQuery(br_associative_array* pArray, br_token t, br_value* pValue) {
    br_uint_16 i;
    NOT_IMPLEMENTED();
}
