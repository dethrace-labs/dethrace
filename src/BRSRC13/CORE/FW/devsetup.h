#ifndef _DEVSETUP_H_
#define _DEVSETUP_H_

#include "br_types.h"

// Offset: 20
// Size: 45
br_pixelmap* BrDevLastBeginQuery();

// Offset: 83
// Size: 45
// EAX: pm
void BrDevLastBeginSet(br_pixelmap *pm);

// Offset: 142
// Size: 214
br_error BrDevBeginVar(br_pixelmap **ppm, char *setup_string, ...);

// Offset: 367
// Size: 54
br_error BrDevBegin(br_pixelmap **ppm, char *setup_string);

// Offset: 434
// Size: 1353
br_error BrDevBeginTV(br_pixelmap **ppm, char *setup_string, br_token_value *setup_tv);

// Offset: 1801
// Size: 96
br_pixelmap* BrDevBeginOld(char *setup_string);

// Offset: 1909
// Size: 60
void BrDevEndOld();

// Offset: 1988
// Size: 57
void BrDevPaletteSetOld(br_pixelmap *pm);

// Offset: 2069
// Size: 61
void BrDevPaletteSetEntryOld(int i, br_colour colour);

// Offset: 2153
// Size: 455
br_error BrRendererFacilityFind(br_renderer_facility **prf, br_device_pixelmap *destination, br_token scalar_type);

// Offset: 2631
// Size: 455
br_error BrPrimitiveLibraryFind(br_primitive_library **ppl, br_device_pixelmap *destination, br_token scalar_type);

// Offset: 3107
// Size: 547
br_error BrGeometryFormatFind(br_geometry **pgf, br_renderer *renderer, br_renderer_facility *renderer_facility, br_token scalar_type, br_token format_type);

#endif
