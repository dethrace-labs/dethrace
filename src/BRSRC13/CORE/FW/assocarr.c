#include "assocarr.h"
#include "harness.h"

br_associative_array* BrAssociativeArrayAllocate() {
    br_associative_array* pArray;
    NOT_IMPLEMENTED();
}

br_error Set_Associative_Array_Value(br_associative_array* pArray, int index, br_value v) {
    NOT_IMPLEMENTED();
}

br_error BrAssociativeArraySetEntry(br_associative_array* pArray, br_token t, br_value v) {
    br_uint_16 i;
    br_token_value* temp;
    NOT_IMPLEMENTED();
}

br_error BrAssociativeArrayRemoveEntry(br_associative_array* pArray, br_token t) {
    br_uint_16 i;
    br_boolean bFound;
    NOT_IMPLEMENTED();
}

br_error BrAssociativeArrayQuery(br_associative_array* pArray, br_token t, br_value* pValue) {
    br_uint_16 i;
    NOT_IMPLEMENTED();
}
