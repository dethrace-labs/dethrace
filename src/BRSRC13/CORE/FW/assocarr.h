#ifndef _ASSOCARR_H_
#define _ASSOCARR_H_

#include "br_types.h"

br_associative_array* BrAssociativeArrayAllocate();

br_error Set_Associative_Array_Value(br_associative_array* pArray, int index, br_value v);

br_error BrAssociativeArraySetEntry(br_associative_array* pArray, br_token t, br_value v);

br_error BrAssociativeArrayRemoveEntry(br_associative_array* pArray, br_token t);

br_error BrAssociativeArrayQuery(br_associative_array* pArray, br_token t, br_value* pValue);

#endif
