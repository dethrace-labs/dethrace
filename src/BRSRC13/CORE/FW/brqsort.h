#ifndef _BRQSORT_H_
#define _BRQSORT_H_

#include "br_types.h"

// Offset: 8
// Size: 560
void BrQsort(void* basep, unsigned int nelems, unsigned int size, br_qsort_cbfn comp);

// Offset: 579
// Size: 81
// EAX: a
// EDX: b
// EBX: nbytes
void swap_chars(char* a, char* b, unsigned int nbytes);

// Offset: 670
// Size: 95
// EAX: ap
// EDX: bp
// EBX: nints
void swap_ints(char* ap, char* bp, unsigned int nints);

// Offset: 776
// Size: 85
// EAX: ap
// EDX: bp
// EBX: nints
void swap_int_1(char* ap, char* bp, unsigned int nints);

#endif
