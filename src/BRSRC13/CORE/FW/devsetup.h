#ifndef _DEVSETUP_H_
#define _DEVSETUP_H_

#include "br_types.h"

br_pixelmap* BrDevLastBeginQuery();

void BrDevLastBeginSet(br_pixelmap* pm);

br_error BrDevBeginVar(br_pixelmap** ppm, char* setup_string, ...);

br_error BrDevBegin(br_pixelmap** ppm, char* setup_string);

br_error BrDevBeginTV(br_pixelmap** ppm, char* setup_string, br_token_value* setup_tv);

br_pixelmap* BrDevBeginOld(char* setup_string);

void BrDevEndOld();

void BrDevPaletteSetOld(br_pixelmap* pm);

void BrDevPaletteSetEntryOld(int i, br_colour colour);

br_error BrRendererFacilityFind(br_renderer_facility** prf, br_device_pixelmap* destination, br_token scalar_type);

br_error BrPrimitiveLibraryFind(br_primitive_library** ppl, br_device_pixelmap* destination, br_token scalar_type);

br_error BrGeometryFormatFind(br_geometry** pgf, br_renderer* renderer, br_renderer_facility* renderer_facility, br_token scalar_type, br_token format_type);

#endif
