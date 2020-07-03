#include "devsetup.h"
#include "harness.h"

#include <stdarg.h>

br_pixelmap* last_begin_screen;
char rscid[51];

// Offset: 20
// Size: 45
br_pixelmap* BrDevLastBeginQuery() {
    NOT_IMPLEMENTED();
}

// Offset: 83
// Size: 45
// EAX: pm
void BrDevLastBeginSet(br_pixelmap* pm) {
    NOT_IMPLEMENTED();
}

// Offset: 142
// Size: 214
br_error BrDevBeginVar(br_pixelmap** ppm, char* setup_string, ...) {
    va_list vl;
    br_uint_32 i;
    br_uint_32 n;
    br_token_value tv[64];
    NOT_IMPLEMENTED();
}

// Offset: 367
// Size: 54
br_error BrDevBegin(br_pixelmap** ppm, char* setup_string) {
    NOT_IMPLEMENTED();
}

// Offset: 434
// Size: 1353
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
    NOT_IMPLEMENTED();
}

// Offset: 1801
// Size: 96
br_pixelmap* BrDevBeginOld(char* setup_string) {
    br_pixelmap* s;
    NOT_IMPLEMENTED();
}

// Offset: 1909
// Size: 60
void BrDevEndOld() {
    NOT_IMPLEMENTED();
}

// Offset: 1988
// Size: 57
void BrDevPaletteSetOld(br_pixelmap* pm) {
    Harness_Hook_SetPalette(pm);
}

// Offset: 2069
// Size: 61
void BrDevPaletteSetEntryOld(int i, br_colour colour) {
    NOT_IMPLEMENTED();
}

// Offset: 2153
// Size: 455
br_error BrRendererFacilityFind(br_renderer_facility** prf, br_device_pixelmap* destination, br_token scalar_type) {
    br_renderer_facility* renderer_facility;
    br_error r;
    br_output_facility* ot;
    char object_name[23];
    char image_name[9];
    br_boolean scalar_is_valid;
    NOT_IMPLEMENTED();
}

// Offset: 2631
// Size: 455
br_error BrPrimitiveLibraryFind(br_primitive_library** ppl, br_device_pixelmap* destination, br_token scalar_type) {
    br_primitive_library* primitive_library;
    br_error r;
    br_output_facility* ot;
    char object_name[25];
    char image_name[9];
    br_boolean scalar_is_valid;
    NOT_IMPLEMENTED();
}

// Offset: 3107
// Size: 547
br_error BrGeometryFormatFind(br_geometry** pgf, br_renderer* renderer, br_renderer_facility* renderer_facility, br_token scalar_type, br_token format_type) {
    br_error r;
    br_geometry* gf;
    char object_name[21];
    char image_name[9];
    br_boolean scalar_is_valid;
    NOT_IMPLEMENTED();
}
