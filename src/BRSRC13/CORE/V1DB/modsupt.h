#ifndef _MODSUPT_H_
#define _MODSUPT_H_

#include "br_types.h"

void BrModelApplyMap(br_model* model, int map_type, br_matrix34* xform);

br_matrix34* BrModelFitMap(br_model* model, int axis_0, int axis_1, br_matrix34* transform);

void BrModelFree(br_model* m);

br_model* BrModelAllocate(char* name, int nvertices, int nfaces);

br_primitive_list* BrPrimitiveListAllocate(br_uint_32 prim_type, br_uint_16 num_prims);

br_uint_32 BrModelAddPrimitiveList(br_model* model, br_primitive_list* primitive_list);

#endif
