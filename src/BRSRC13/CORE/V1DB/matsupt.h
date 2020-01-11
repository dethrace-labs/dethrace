#ifndef _MATSUPT_H_
#define _MATSUPT_H_

#include "br_types.h"

// Offset: 19
// Size: 138
br_material* BrMaterialAllocate(char* name);

// Offset: 172
// Size: 70
void BrMaterialFree(br_material* m);

#endif
