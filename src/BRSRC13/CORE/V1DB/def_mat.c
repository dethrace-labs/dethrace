#include "def_mat.h"
#include "CORE/FW/resource.h"
#include "CORE/MATH/matrix23.h"
#include "dbsetup.h"
#include "harness/trace.h"

// IDA: br_material* __cdecl SetupDefaultMaterial()
br_material* SetupDefaultMaterial() {
    static br_material* default_mat;

    default_mat = BrResAllocate(v1db.res, sizeof(br_material), BR_MEMORY_MATERIAL);
    if (default_mat) {
        default_mat->identifier = "default_mat";
        default_mat->colour = BR_COLOUR_RGB(255, 255, 255);
        default_mat->ka = 0.1f;
        default_mat->kd = 0.699999f;
        default_mat->ks = 0.0f;
        default_mat->power = 20;
        default_mat->flags = BR_MATF_LIGHT;
        BrMatrix23Identity(&default_mat->map_transform);
        default_mat->index_base = 10;
        default_mat->index_range = 31;
    }
    return default_mat;
}
