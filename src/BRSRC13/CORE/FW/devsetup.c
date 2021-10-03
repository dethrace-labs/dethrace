#include "devsetup.h"
#include "harness/hooks.h"
#include "harness/trace.h"

#include <stdarg.h>

br_pixelmap* last_begin_screen;

// IDA: br_pixelmap* __cdecl BrDevLastBeginQuery()
br_pixelmap* BrDevLastBeginQuery() {
    LOG_TRACE("()");

    STUB();
    return NULL;
}

// IDA: void __usercall BrDevLastBeginSet(br_pixelmap *pm@<EAX>)
void BrDevLastBeginSet(br_pixelmap* pm) {
    LOG_TRACE("(%p)", pm);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrDevBeginVar(br_pixelmap **ppm, char *setup_string, ...)
br_error BrDevBeginVar(br_pixelmap** ppm, char* setup_string, ...) {
    va_list vl;
    br_uint_32 i;
    br_uint_32 n;
    br_token_value tv[64];
    LOG_TRACE("(%p, \"%s\")", ppm, setup_string);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrDevBegin(br_pixelmap **ppm, char *setup_string)
br_error BrDevBegin(br_pixelmap** ppm, char* setup_string) {
    LOG_TRACE("(%p, \"%s\")", ppm, setup_string);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrDevBeginTV(br_pixelmap **ppm, char *setup_string, br_token_value *setup_tv)
br_error BrDevBeginTV(br_pixelmap** ppm, char* setup_string, br_token_value* setup_tv) {
    char str[512];
    char* args;
    char* devname;
    char* devargs;
    char devices_str[512];
    char* device_name;
    char* device_args;
    char* next_device;
    br_token_value args_tv[64];
    br_token_value* tv;
    br_output_facility* output_facility;
    br_device* device;
    br_pixelmap* screen;
    int i;
    int n;
    br_error r;
    LOG_TRACE("(%p, \"%s\", %p)", ppm, setup_string, setup_tv);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __cdecl BrDevBeginOld(char *setup_string)
br_pixelmap* BrDevBeginOld(char* setup_string) {
    br_pixelmap* s;
    LOG_TRACE("(\"%s\")", setup_string);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrDevEndOld()
void BrDevEndOld() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl BrDevPaletteSetOld(br_pixelmap *pm)
void BrDevPaletteSetOld(br_pixelmap* pm) {
    Harness_Hook_BrDevPaletteSetOld(pm);
}

// IDA: void __cdecl BrDevPaletteSetEntryOld(int i, br_colour colour)
void BrDevPaletteSetEntryOld(int i, br_colour colour) {
    Harness_Hook_BrDevPaletteSetEntryOld(i, colour);
}

// IDA: br_error __cdecl BrRendererFacilityFind(br_renderer_facility **prf, br_device_pixelmap *destination, br_token scalar_type)
br_error BrRendererFacilityFind(br_renderer_facility** prf, br_device_pixelmap* destination, br_token scalar_type) {
    br_renderer_facility* renderer_facility;
    br_error r;
    br_output_facility* ot;
    char object_name[23];
    char image_name[9];
    br_boolean scalar_is_valid;
    LOG_TRACE("(%p, %p, %d)", prf, destination, scalar_type);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrPrimitiveLibraryFind(br_primitive_library **ppl, br_device_pixelmap *destination, br_token scalar_type)
br_error BrPrimitiveLibraryFind(br_primitive_library** ppl, br_device_pixelmap* destination, br_token scalar_type) {
    br_primitive_library* primitive_library;
    br_error r;
    br_output_facility* ot;
    char object_name[25];
    char image_name[9];
    br_boolean scalar_is_valid;
    LOG_TRACE("(%p, %p, %d)", ppl, destination, scalar_type);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrGeometryFormatFind(br_geometry **pgf, br_renderer *renderer, br_renderer_facility *renderer_facility, br_token scalar_type, br_token format_type)
br_error BrGeometryFormatFind(br_geometry** pgf, br_renderer* renderer, br_renderer_facility* renderer_facility, br_token scalar_type, br_token format_type) {
    br_error r;
    br_geometry* gf;
    char object_name[21];
    char image_name[9];
    br_boolean scalar_is_valid;
    LOG_TRACE("(%p, %p, %p, %d, %d)", pgf, renderer, renderer_facility, scalar_type, format_type);
    NOT_IMPLEMENTED();
}
