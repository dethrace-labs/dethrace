#include "matsupt.h"
#include "dbsetup.h"
#include "harness.h"
#include "resource.h"
#include <stddef.h>
#include <string.h>

char rscid[49];

// Offset: 19
// Size: 138
br_material* BrMaterialAllocate(char* name) {
    br_material* m;
    LOG_TRACE("(\"%s\")", name);

    m = BrResAllocate(v1db.res, sizeof(br_material), BR_MEMORY_MATERIAL);
    memcpy(m, v1db.default_material, sizeof(br_material));
    m->map_transform_1.m[0][0] = 0;
    if (name) {
        m->identifier = BrResStrDup(m, name);
    } else {
        m->identifier = NULL;
    }
    return m;
}

// Offset: 172
// Size: 70
void BrMaterialFree(br_material* m) {
    NOT_IMPLEMENTED();
}
