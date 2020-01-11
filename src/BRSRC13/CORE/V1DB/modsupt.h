#ifndef _MODSUPT_H_
#define _MODSUPT_H_

#include "br_types.h"

// Offset: 16
// Size: 703
void BrModelApplyMap(br_model* model, int map_type, br_matrix34* xform);

// Offset: 733
// Size: 697
br_matrix34* BrModelFitMap(br_model* model, int axis_0, int axis_1, br_matrix34* transform);

// Offset: 1442
// Size: 73
void BrModelFree(br_model* m);

// Offset: 1531
// Size: 250
br_model* BrModelAllocate(char* name, int nvertices, int nfaces);

// Offset: 1805
// Size: 40
br_primitive_list* BrPrimitiveListAllocate(br_uint_32 prim_type, br_uint_16 num_prims);

// Offset: 1869
// Size: 40
br_uint_32 BrModelAddPrimitiveList(br_model* model, br_primitive_list* primitive_list);

#endif
