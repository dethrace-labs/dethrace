#include "pmsetup.h"
#include "CORE/FW/resource.h"
#include "CORE/FW/resreg.h"
#include "CORE/STD/brstdlib.h"
#include "debug.h"
#include "image.h"
#include <stddef.h>
#include <stdio.h>

br_image Image_BRPMAP1;
br_resource_class pm_resourceClasses[2] = {
    { 0u, "PIXELMAP", BR_MEMORY_PIXELMAP, NULL, 0u },
    { 0u, "PIXELS", BR_MEMORY_PIXELS, NULL, 32u }
};
void* functionPointers_BRPMAP1[77];
char* namePointers_BRPMAP1[77];
br_pixelmap_state _pixelmap;
br_uint_16 nameOrdinals_BRPMAP1[77];

// Offset: 16
// Size: 130
void BrPixelmapBegin() {
    int i;

    BrMemSet(&_pixelmap, 0, sizeof(br_pixelmap_state));
    _pixelmap.res = BrResAllocate(NULL, 0, BR_MEMORY_ANCHOR);
    for (i = 0; i < 2; i++) {
        BrResClassAdd(&pm_resourceClasses[i]);
    }
    BrImageAdd(&Image_BRPMAP1);
}

// Offset: 160
// Size: 76
void BrPixelmapEnd() {
    NOT_IMPLEMENTED();
}
