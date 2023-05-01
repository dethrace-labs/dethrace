#include "fwsetup.h"

#include "CORE/FW/brlists.h"
#include "CORE/FW/file.h"
#include "CORE/FW/image.h"
#include "CORE/FW/object.h"
#include "CORE/FW/register.h"
#include "CORE/FW/resource.h"
#include "CORE/FW/resreg.h"
#include "CORE/FW/sys_conf.h"
#include "CORE/FW/token.h"
#include "CORE/STD/brstddiag.h"
#include "CORE/STD/brstdfile.h"
#include "CORE/STD/brstdlib.h"
#include "CORE/STD/brstdmem.h"
#include "harness/trace.h"
#include <stddef.h>
#include <stdio.h>

br_framework_state fw;
void* functionPointers_BRCORE1[185];
char* namePointers_BRCORE1[185];
br_resource_class fw_resourceClasses[25] = {
    { 0u, "REGISTRY", BR_MEMORY_REGISTRY, NULL, 0u },
    { 0u, "ANCHOR", BR_MEMORY_ANCHOR, NULL, 0u },
    { 0u, "RESOURCE_CLASS", BR_MEMORY_RESOURCE_CLASS, NULL, 0u },
    { 0u, "SCRATCH,", BR_MEMORY_SCRATCH, NULL, 16u },
    { 0u, "STRING", BR_MEMORY_STRING, NULL, 1u },
    { 0u, "FILE", BR_MEMORY_FILE, &_BrFileFree, 0u },
    { 0u, "POOL", BR_MEMORY_POOL, NULL, 16u },
    { 0u, "EXCEPTION_HANDLER", BR_MEMORY_EXCEPTION_HANDLER, NULL, 0u },
    { 0u, "TOKEN", BR_MEMORY_TOKEN, NULL, 0u },
    { 0u, "TOKEN_MAP", BR_MEMORY_TOKEN_MAP, NULL, 8u },
    { 0u, "DATAFILE", BR_MEMORY_DATAFILE, NULL, 0u },
    { 0u, "LEXER", BR_MEMORY_LEXER, NULL, 0u },
    { 0u, "OBJECT_LIST", BR_MEMORY_OBJECT_LIST, NULL, 0u },
    { 0u, "OBJECT_LIST_ENTRY", BR_MEMORY_OBJECT_LIST_ENTRY, NULL, 0u },
    { 0u, "APPLICATION", BR_MEMORY_APPLICATION, NULL, 0u },
    { 0u, "IMAGE", BR_MEMORY_IMAGE, &_BrImageFree, 0u },
    { 0u, "IMAGE_ARENA", BR_MEMORY_IMAGE_ARENA, NULL, 0u },
    { 0u, "IMAGE_SECTIONS", BR_MEMORY_IMAGE_SECTIONS, NULL, 0u },
    { 0u, "IMAGE_NAMES", BR_MEMORY_IMAGE_NAMES, NULL, 0u },
    { 0u, "OBJECT", BR_MEMORY_OBJECT, &_BrObjectFree, 16u },
    { 0u, "OBJECT_DATA", BR_MEMORY_OBJECT_DATA, NULL, 16u },
    { 0u, "DRIVER", BR_MEMORY_DRIVER, NULL, 0u },
    { 0u, "FMT_RESULTS", BR_MEMORY_FMT_RESULTS, NULL, 0u },
    { 0u, "TOKEN_VALUE", BR_MEMORY_TOKEN_VALUE, NULL, 0u },
    { 0u, "TOKEN_TEMPLATE", BR_MEMORY_TOKEN_TEMPLATE, NULL, 0u }
};

br_uint_16 nameOrdinals_BRCORE1[185];

br_image Image_BRCORE1 = {
    { 0 },
    "BRCORE1",
    2,
    0,
    1,
    185,
    functionPointers_BRCORE1,
    185,
    namePointers_BRCORE1,
    nameOrdinals_BRCORE1,
    0,
    NULL,
    0,
    NULL,
    NULL
};

#define NBR_DEV_SLOTS 16

// IDA: br_error __cdecl BrFwBegin()
br_error BrFwBegin(void) {
    int i;

    if (fw.active) {
        return 4103;
    }

    if (fw.diag == NULL)
        fw.diag = _BrDefaultDiagHandler;
    if (fw.fsys == NULL)
        fw.fsys = _BrDefaultFilesystem;
    if (fw.mem == NULL)
        fw.mem = _BrDefaultAllocator;
    fw.open_mode = 0;
    BrRegistryNew(&fw.reg_resource_classes);

    fw.resource_class_index[BR_MEMORY_REGISTRY] = &fw_resourceClasses[0];
    fw.resource_class_index[BR_MEMORY_ANCHOR] = &fw_resourceClasses[1];
    fw.res = BrResAllocate(NULL, 0, BR_MEMORY_ANCHOR);
    for (i = 0; i < BR_ASIZE(fw_resourceClasses); i++) {
        BrResClassAdd(&fw_resourceClasses[i]);
    }
    BrNewList(&fw.images);
    fw.dev_slots = (br_open_device*)BrResAllocate(fw.res, sizeof(br_open_device) * NBR_DEV_SLOTS, BR_MEMORY_OBJECT_DATA);
    fw.ndev_slots = NBR_DEV_SLOTS;
    fw.active = 1;
    BrTokenBegin();
    BrSystemConfigBegin();
    fw.bAlreadyLoadedDrivers = 0;
    BrImageAdd(&Image_BRCORE1);

    return 0;
}

// IDA: br_error __cdecl BrFwEnd()
br_error BrFwEnd(void) {
    LOG_TRACE("()");

    if (fw.active == 0) {
        return 4102;
    }
    BrImageRemove(&Image_BRCORE1);
    BrResFree(fw.res);
    BrMemSet(&fw, 0, sizeof(fw));
    return 0;
}

// IDA: br_diaghandler* __cdecl BrDiagHandlerSet(br_diaghandler *newdh)
br_diaghandler* BrDiagHandlerSet(br_diaghandler* newdh) {
    br_diaghandler* old;
    LOG_TRACE("(%p)", newdh);

    old = fw.diag;
    if (newdh == NULL) {
        newdh = _BrDefaultDiagHandler;
    }
    fw.diag = newdh;
    return old;
}

// IDA: br_filesystem* __cdecl BrFilesystemSet(br_filesystem *newfs)
br_filesystem* BrFilesystemSet(br_filesystem* newfs) {
    br_filesystem* old;

    old = fw.fsys;
    if (newfs == NULL) {
        newfs = _BrDefaultFilesystem;
    }
    fw.fsys = newfs;
    return old;
}

// IDA: br_allocator* __cdecl BrAllocatorSet(br_allocator *newal)
br_allocator* BrAllocatorSet(br_allocator* newal) {
    br_allocator* old;
    LOG_TRACE("(%p)", newal);

    old = fw.mem;
    if (newal == NULL) {
        newal = _BrDefaultAllocator;
    }
    fw.mem = newal;

    return old;
}
