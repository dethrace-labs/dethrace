#ifndef _ASSOCARR_H_
#define _ASSOCARR_H_

#include "br_types.h"

// Offset: 27
// Size: 145
br_associative_array* BrAssociativeArrayAllocate();

// Offset: 200
// Size: 207
// EAX: pArray
// EDX: index
// EBX: v
br_error Set_Associative_Array_Value(br_associative_array *pArray, int index, br_value v);

// Offset: 434
// Size: 535
// EAX: pArray
// EDX: t
// EBX: v
br_error BrAssociativeArraySetEntry(br_associative_array *pArray, br_token t, br_value v);

// Offset: 999
// Size: 363
// EAX: pArray
// EDX: t
br_error BrAssociativeArrayRemoveEntry(br_associative_array *pArray, br_token t);

// Offset: 1386
// Size: 202
// EAX: pArray
// EDX: t
// EBX: pValue
br_error BrAssociativeArrayQuery(br_associative_array *pArray, br_token t, br_value *pValue);

#endif
