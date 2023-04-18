#include "prepmatl.h"
#include "harness/hooks.h"
#include "harness/trace.h"

// IDA: void __cdecl BrMaterialUpdate(br_material *mat, br_uint_16 flags)
void BrMaterialUpdate(br_material* mat, br_uint_16 flags) {
    br_token_value tva[32];
    br_token_value* tvp;
    br_token t;
    br_int_32 c;

    // TODO: ?

    // Added by dethrace
    gHarness_platform.Renderer_BufferMaterial(mat);
}

// IDA: void __usercall BrMaterialClear(br_material *mat@<EAX>)
void BrMaterialClear(br_material* mat) {
    LOG_TRACE("(%p)", mat);
    NOT_IMPLEMENTED();
}
