#include "matsupt.h"
#include "CORE/FW/resource.h"
#include "CORE/V1DB/dbsetup.h"
#include "harness/trace.h"
#include <stddef.h>
#include <string.h>

// IDA: br_material* __cdecl BrMaterialAllocate(char *name)
br_material* BrMaterialAllocate(char* name) {
    br_material* m;
    LOG_TRACE("(\"%s\")", name);

    m = BrResAllocate(v1db.res, sizeof(br_material), BR_MEMORY_MATERIAL);
    memcpy(m, v1db.default_material, sizeof(br_material));
    m->stored = NULL;
    if (name) {
        m->identifier = BrResStrDup(m, name);
    } else {
        m->identifier = NULL;
    }
    return m;
}

// IDA: void __cdecl BrMaterialFree(br_material *m)
void BrMaterialFree(br_material* m) {
    LOG_TRACE("(%p)", m);
    BrResFree(m);
}
