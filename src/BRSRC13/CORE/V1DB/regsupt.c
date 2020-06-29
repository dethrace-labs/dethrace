#include "regsupt.h"
#include "CORE/FW/register.h"
#include "CORE/V1DB/dbsetup.h"
#include "CORE/V1DB/prepmap.h"
#include "CORE/V1DB/prepmatl.h"
#include "CORE/V1DB/prepmesh.h"
#include "CORE/V1DB/preptab.h"

#include "debug.h"

char rscid[51];

// Offset: 11
// Size: 148
br_model* BrModelAdd(br_model* model) {
    BrRegistryAdd(&v1db.reg_models, model);
    BrModelUpdate(model, 0x7FFF);
    return model;
}

// Offset: 173
// Size: 85
br_model* BrModelRemove(br_model* model) {
    NOT_IMPLEMENTED();
}

// Offset: 270
// Size: 49
br_model* BrModelFind(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);
    return BrRegistryFind(&v1db.reg_models, pattern);
}

// Offset: 335
// Size: 83
br_model_find_cbfn* BrModelFindHook(br_model_find_cbfn* hook) {
    br_model_find_cbfn* old;
    NOT_IMPLEMENTED();
}

// Offset: 433
// Size: 158
br_uint_32 BrModelAddMany(br_model** items, int n) {
    int i;
    int r;
    NOT_IMPLEMENTED();
}

// Offset: 609
// Size: 158
br_uint_32 BrModelRemoveMany(br_model** items, int n) {
    int i;
    int r;
    NOT_IMPLEMENTED();
}

// Offset: 783
// Size: 55
br_uint_32 BrModelFindMany(char* pattern, br_model** items, int max) {
    NOT_IMPLEMENTED();
}

// Offset: 851
// Size: 49
br_uint_32 BrModelCount(char* pattern) {
    NOT_IMPLEMENTED();
}

// Offset: 912
// Size: 55
br_uint_32 BrModelEnum(char* pattern, br_model_enum_cbfn* callback, void* arg) {
    NOT_IMPLEMENTED();
}

// Offset: 981
// Size: 185
br_material* BrMaterialAdd(br_material* material) {
    NOT_IMPLEMENTED();
}

// Offset: 1183
// Size: 88
br_material* BrMaterialRemove(br_material* material) {
    NOT_IMPLEMENTED();
}

// Offset: 1286
// Size: 49
br_material* BrMaterialFind(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);

    return BrRegistryFind(&v1db.reg_materials, pattern);
}

// Offset: 1354
// Size: 55
br_material_find_cbfn* BrMaterialFindHook(br_material_find_cbfn* hook) {
    br_material_find_cbfn* old;
    NOT_IMPLEMENTED();
}

// Offset: 1427
// Size: 164
br_uint_32 BrMaterialAddMany(br_material** items, int n) {
    int i;
    int r = 0;

    for (i = 0; i < n; i++) {
        BrRegistryAdd(&v1db.reg_materials, items[i]);
        BrMaterialUpdate(items[i], 0x7FFFu);
        if (items[i]) {
            r++;
        }
    }
    return r;
}

// Offset: 1612
// Size: 164
br_uint_32 BrMaterialRemoveMany(br_material** items, int n) {
    int i;
    int r;
    NOT_IMPLEMENTED();
}

// Offset: 1795
// Size: 55
br_uint_32 BrMaterialFindMany(char* pattern, br_material** items, int max) {
    NOT_IMPLEMENTED();
}

// Offset: 1866
// Size: 49
br_uint_32 BrMaterialCount(char* pattern) {
    NOT_IMPLEMENTED();
}

// Offset: 1930
// Size: 55
br_uint_32 BrMaterialEnum(char* pattern, br_material_enum_cbfn* callback, void* arg) {
    NOT_IMPLEMENTED();
}

// Offset: 1994
// Size: 101
br_pixelmap* BrMapAdd(br_pixelmap* pixelmap) {
    LOG_TRACE("(%p)", pixelmap);
    BrRegistryAdd(&v1db.reg_textures, pixelmap);
    BrMapUpdate(pixelmap, 0xFFFu);
    return pixelmap;
}

// Offset: 2107
// Size: 88
br_pixelmap* BrMapRemove(br_pixelmap* pixelmap) {
    NOT_IMPLEMENTED();
}

// Offset: 2205
// Size: 49
br_pixelmap* BrMapFind(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);
    return BrRegistryFind(&v1db.reg_textures, pattern);
}

// Offset: 2268
// Size: 86
br_map_find_cbfn* BrMapFindHook(br_map_find_cbfn* hook) {
    br_map_find_cbfn* old;
    NOT_IMPLEMENTED();
}

// Offset: 2367
// Size: 164
br_uint_32 BrMapAddMany(br_pixelmap** items, int n) {
    int i;
    int r;

    for (i = 0; i < n; i++) {
        BrRegistryAdd(&v1db.reg_textures, items[i]);
        BrMapUpdate(items[i], 4095);
        if (items[i]) {
            r++;
        }
    }
    return r++;
}

// Offset: 2547
// Size: 164
br_uint_32 BrMapRemoveMany(br_pixelmap** items, int n) {
    int i;
    int r;
    NOT_IMPLEMENTED();
}

// Offset: 2725
// Size: 55
br_uint_32 BrMapFindMany(char* pattern, br_pixelmap** items, int max) {
    NOT_IMPLEMENTED();
}

// Offset: 2791
// Size: 49
br_uint_32 BrMapCount(char* pattern) {
    NOT_IMPLEMENTED();
}

// Offset: 2850
// Size: 55
br_uint_32 BrMapEnum(char* pattern, br_map_enum_cbfn* callback, void* arg) {
    NOT_IMPLEMENTED();
}

// Offset: 2916
// Size: 101
br_pixelmap* BrTableAdd(br_pixelmap* pixelmap) {
    NOT_IMPLEMENTED();
}

// Offset: 3031
// Size: 88
br_pixelmap* BrTableRemove(br_pixelmap* pixelmap) {
    NOT_IMPLEMENTED();
}

// Offset: 3131
// Size: 49
br_pixelmap* BrTableFind(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);
    return BrRegistryFind(&v1db.reg_tables, pattern);
}

// Offset: 3196
// Size: 86
br_table_find_cbfn* BrTableFindHook(br_table_find_cbfn* hook) {
    br_table_find_cbfn* old;
    NOT_IMPLEMENTED();
}

// Offset: 3297
// Size: 164
br_uint_32 BrTableAddMany(br_pixelmap** items, int n) {
    int i;
    int r;

    for (i = 0; i < n; i++) {
        BrRegistryAdd(&v1db.reg_tables, items[i]);
        BrTableUpdate(items[i], 0x7FFFu);
        if (items[i]) {
            ++r;
        }
    }
    return r;
}

// Offset: 3479
// Size: 164
br_uint_32 BrTableRemoveMany(br_pixelmap** items, int n) {
    int i;
    int r;
    NOT_IMPLEMENTED();
}

// Offset: 3659
// Size: 55
br_uint_32 BrTableFindMany(char* pattern, br_pixelmap** items, int max) {
    NOT_IMPLEMENTED();
}

// Offset: 3727
// Size: 49
br_uint_32 BrTableCount(char* pattern) {
    NOT_IMPLEMENTED();
}

// Offset: 3788
// Size: 55
br_uint_32 BrTableEnum(char* pattern, br_table_enum_cbfn* callback, void* arg) {
    NOT_IMPLEMENTED();
}
