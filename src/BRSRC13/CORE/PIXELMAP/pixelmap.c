#include "pixelmap.h"
#include "pmmem.h"

char rscid[50];

// Offset: 19
// Size: 65
br_pixelmap* BrPixelmapAllocate(br_uint_8 type, br_int_32 w, br_int_32 h, void* pixels, int flags) {
    return (br_pixelmap*)DevicePixelmapMemAllocate(type, w, h, pixels, flags);
}
