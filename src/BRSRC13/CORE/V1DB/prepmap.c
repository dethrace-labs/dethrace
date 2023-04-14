#include "prepmap.h"
#include "harness/hooks.h"
#include "harness/trace.h"

// IDA: void __cdecl BrMapUpdate(br_pixelmap *map, br_uint_16 flags)
void BrMapUpdate(br_pixelmap* map, br_uint_16 flags) {
    BrBufferUpdate(map, BRT_COLOUR_MAP_O, flags);
}

// IDA: void __usercall BrBufferUpdate(br_pixelmap *pm@<EAX>, br_token use@<EDX>, br_uint_16 flags@<EBX>)
void BrBufferUpdate(br_pixelmap* pm, br_token use, br_uint_16 flags) {
    br_token_value tv[3];

    // renderer->dispatch->bufferStoredNew ...

    // Added by dethrace
    if (use != BRT_COLOUR_MAP_O && use != BRT_UNKNOWN) {
        LOG_PANIC("use %d", use);
    }
    gHarness_platform.Renderer_BufferTexture(pm);
}

// IDA: void __usercall BrBufferClear(br_pixelmap *pm@<EAX>)
void BrBufferClear(br_pixelmap* pm) {
    LOG_TRACE("(%p)", pm);

    STUB_ONCE();
    return;

    // FIXME: use a proper br_buffer_storage object with br_buffer_stored_dispatch dispatch table
    if (pm->stored != NULL) {
        (*(br_object_dispatch**)pm->stored)->_free(pm->stored);
        pm->stored = NULL;
    }
}
