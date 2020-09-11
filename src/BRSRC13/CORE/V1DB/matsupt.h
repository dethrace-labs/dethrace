#ifndef _MATSUPT_H_
#define _MATSUPT_H_

#include "br_types.h"

br_material* BrMaterialAllocate(char* name);

void BrMaterialFree(br_material* m);

#endif
