#include "assocarr.h"
#include "CORE/FW/fwsetup.h"
#include "CORE/FW/mem.h"
#include "CORE/FW/resource.h"
#include "CORE/FW/token.h"
#include "CORE/STD/brstdlib.h"
#include "harness/trace.h"

// IDA: br_associative_array* __cdecl BrAssociativeArrayAllocate()
br_associative_array* BrAssociativeArrayAllocate(void) {
    br_associative_array* pArray;
    LOG_TRACE("()");

    pArray = BrResAllocate(fw.res, sizeof(br_associative_array), BR_MEMORY_SCRATCH);
    if (pArray == NULL) {
        return NULL;
    }
    pArray->tv = BrResAllocate(pArray, 10 * sizeof(br_token_value), BR_MEMORY_SCRATCH);
    if (pArray->tv == NULL) {
        return NULL;
    }
    pArray->max_elements = 10;
    pArray->num_elements = 0;
    return pArray;
}

// IDA: br_error __usercall Set_Associative_Array_Value@<EAX>(br_associative_array *pArray@<EAX>, int index@<EDX>, br_value v@<EBX>)
br_error Set_Associative_Array_Value(br_associative_array* pArray, int index, br_value v) {
    LOG_TRACE("(%p, %d, %d)", pArray, index, v);

    if (BrTokenType(pArray->tv[index].t) == BRT_STRING) {
        if (pArray->tv[index].v.str != NULL) {
            BrResFree(pArray->tv[index].v.str);
        }
        if (v.str != NULL) {
            pArray->tv[index].v.str = BrResStrDup(pArray, v.str);
        } else {
            pArray->tv[index].v.str = NULL;
        }
    } else {
        pArray->tv[index].v = v;
    }
    return 0;
}

// IDA: br_error __usercall BrAssociativeArraySetEntry@<EAX>(br_associative_array *pArray@<EAX>, br_token t@<EDX>, br_value v@<EBX>)
br_error BrAssociativeArraySetEntry(br_associative_array* pArray, br_token t, br_value v) {
    br_uint_16 i;
    br_token_value* temp;
    LOG_TRACE("(%p, %d, %d)", pArray, t, v);

    for (i = 0; i < pArray->num_elements; i++) {
        if (pArray->tv[i].t == t) {
            return Set_Associative_Array_Value(pArray, i, v);
        }
    }
    if (pArray->num_elements < pArray->max_elements) {
        pArray->tv[pArray->num_elements].t = t;
        Set_Associative_Array_Value(pArray, pArray->num_elements, v);
        pArray->num_elements += 1;
        return 0;
    }
    temp = BrResAllocate(pArray, (pArray->max_elements + 10) * sizeof(br_token_value), BR_MEMORY_APPLICATION);
    BrMemCpy(temp, pArray->tv, pArray->max_elements * sizeof(br_token_value));
    BrResFree(pArray->tv);
    pArray->tv = temp;
    if (temp == NULL) {
        return 0x1002;
    }
    pArray->max_elements += 10;
    Set_Associative_Array_Value(pArray, pArray->num_elements, v);
    pArray->num_elements += 1;
    return 0;
}

// IDA: br_error __usercall BrAssociativeArrayRemoveEntry@<EAX>(br_associative_array *pArray@<EAX>, br_token t@<EDX>)
br_error BrAssociativeArrayRemoveEntry(br_associative_array* pArray, br_token t) {
    br_uint_16 i;
    br_boolean bFound;
    LOG_TRACE("(%p, %d)", pArray, t);

    bFound = 0;
    for (i = 0; i < pArray->num_elements; i++) {
        if (pArray->tv[i].t == t) {
            bFound = 1;
            break;
        }
    }
    if (bFound != 0) {
        if ((BrTokenType(t) == BRT_STRING) && (pArray->tv[i].v.str != NULL)) {
            BrResFree(pArray->tv[i].v.str);
        }
        for (; i < pArray->num_elements - 1; i++) {
            BrMemCpy(&pArray->tv[i], &pArray->tv[i+1], sizeof(br_token_value));
        }
        pArray->num_elements--;
        return 0; // BRE_OK
    }
    return 0x1002; // BRE_NOTFOUND
}

// IDA: br_error __usercall BrAssociativeArrayQuery@<EAX>(br_associative_array *pArray@<EAX>, br_token t@<EDX>, br_value *pValue@<EBX>)
br_error BrAssociativeArrayQuery(br_associative_array* pArray, br_token t, br_value* pValue) {
    br_uint_16 i;
    LOG_TRACE("(%p, %d, %p)", pArray, t, pValue);

    for (i = 0; i < pArray->num_elements; i++) {
        if (pArray->tv[i].t == t) {
            *pValue = pArray->tv[i].v;
            return 0;
        }
    }
    return 0x1002;
}
