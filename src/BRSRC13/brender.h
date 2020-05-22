// This is a temporary solution to avoid userland (dethrace) code from having to understand brender internal paths.
// Probably should end up just being a bunch of function prototypes.

#include "CORE/DOSIO/dosio.h"
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