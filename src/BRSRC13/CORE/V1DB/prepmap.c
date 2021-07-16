#include "prepmap.h"
#include "harness_trace.h"

char rscid[51];

// IDA: void __cdecl BrMapUpdate(br_pixelmap *map, br_uint_16 flags)
void BrMapUpdate(br_pixelmap* map, br_uint_16 flags) {
    BrBufferUpdate(map, BRT_COLOUR_MAP_O, flags);
}

// IDA: void __usercall BrBufferUpdate(br_pixelmap *pm@<EAX>, br_token use@<EDX>, br_uint_16 flags@<EBX>)
void BrBufferUpdate(br_pixelmap* pm, br_token use, br_uint_16 flags) {
    br_token_value tv[3];

    // renderer->dispatch->bufferStoredNew ...
}

// IDA: void __usercall BrBufferClear(br_pixelmap *pm@<EAX>)
void BrBufferClear(br_pixelmap* pm) {
    LOG_TRACE("(%p)", pm);

    SILENT_STUB();
}
