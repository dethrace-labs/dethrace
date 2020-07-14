// This is a temporary solution to avoid userland (dethrace) code from having to understand brender internal paths.
// Probably should end up just being a bunch of function prototypes.

#ifndef BRENDER_H
#define BRENDER_H

#include "CORE/DOSIO/dosio.h"
#include "CORE/FW/bswap.h"
#include "CORE/FW/fwsetup.h"
#include "CORE/FW/mem.h"
#include "CORE/FW/resource.h"
#include "CORE/FW/resreg.h"
#include "CORE/MATH/matrix34.h"
#include "CORE/PIXELMAP/pixelmap.h"
#include "CORE/PIXELMAP/pmdsptch.h"
#include "CORE/V1DB/actsupt.h"
#include "CORE/V1DB/dbsetup.h"
#include "CORE/V1DB/enables.h"
#include "CORE/V1DB/matsupt.h"
#include "CORE/V1DB/prepmatl.h"
#include "CORE/V1DB/regsupt.h"
#include "CORE/V1DB/v1dbfile.h"

extern struct br_font* BrFontFixed3x5;
extern struct br_font* BrFontProp4x6;
extern struct br_font* BrFontProp7x9;

br_pixelmap* BrPixelmapLoad(char* filename);
br_uint_32 BrPixelmapLoadMany(char* filename, br_pixelmap** pixelmaps, br_uint_16 num);
void BrDevPaletteSetOld(br_pixelmap* pm);
void* BrMemAllocate(br_size_t size, br_uint_8 type);

#endif