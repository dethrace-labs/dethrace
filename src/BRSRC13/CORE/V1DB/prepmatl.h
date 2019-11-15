#ifndef _PREPMATL_H_
#define _PREPMATL_H_

#include "dr_types.h"
#include "br_types.h"

// Offset: 17
// Size: 2997
void BrMaterialUpdate(br_material *mat, br_uint_16 flags);

// Offset: 3030
// Size: 88
// EAX: mat
void BrMaterialClear(br_material *mat);

#endif
