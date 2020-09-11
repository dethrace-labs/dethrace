#include "regsupt.h"
#include "CORE/FW/register.h"
#include "CORE/V1DB/dbsetup.h"
#include "CORE/V1DB/prepmap.h"
#include "CORE/V1DB/prepmatl.h"
#include "CORE/V1DB/prepmesh.h"
#include "CORE/V1DB/preptab.h"

#include "harness.h"

char rscid[51];

br_model* BrModelAdd(br_model* model) {
    BrRegistryAdd(&v1db.reg_models, model);
    BrModelUpdate(model, BR_MODU_ALL);
    return model;
}

br_model* BrModelRemove(br_model* model) {
    NOT_IMPLEMENTED();
}

br_model* BrModelFind(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);
    return BrRegistryFind(&v1db.reg_models, pattern);
}

br_model_find_cbfn* BrModelFindHook(br_model_find_cbfn* hook) {
    br_model_find_cbfn* old;
    NOT_IMPLEMENTED();
}

br_uint_32 BrModelAddMany(br_model** items, int n) {
    int i;
    int r;
    NOT_IMPLEMENTED();
}

br_uint_32 BrModelRemoveMany(br_model** items, int n) {
    int i;
    int r;
    NOT_IMPLEMENTED();
}

br_uint_32 BrModelFindMany(char* pattern, br_model** items, int max) {
    NOT_IMPLEMENTED();
}

br_uint_32 BrModelCount(char* pattern) {
    NOT_IMPLEMENTED();
}

br_uint_32 BrModelEnum(char* pattern, br_model_enum_cbfn* callback, void* arg) {
    NOT_IMPLEMENTED();
}

br_material* BrMaterialAdd(br_material* material) {
    BrRegistryAdd(&v1db.reg_materials, material);
    BrMaterialUpdate(material, BR_MATU_ALL);
}

br_material* BrMaterialRemove(br_material* material) {
    NOT_IMPLEMENTED();
}

br_material* BrMaterialFind(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);

    return BrRegistryFind(&v1db.reg_materials, pattern);
}

br_material_find_cbfn* BrMaterialFindHook(br_material_find_cbfn* hook) {
    br_material_find_cbfn* old;
    NOT_IMPLEMENTED();
}

br_uint_32 BrMaterialAddMany(br_material** items, int n) {
    int i;
    int r = 0;

    for (i = 0; i < n; i++) {
        BrRegistryAdd(&v1db.reg_materials, items[i]);
        BrMaterialUpdate(items[i], BR_MATU_ALL);
        if (items[i]) {
            r++;
        }
    }
    return r;
}

br_uint_32 BrMaterialRemoveMany(br_material** items, int n) {
    int i;
    int r;
    NOT_IMPLEMENTED();
}

br_uint_32 BrMaterialFindMany(char* pattern, br_material** items, int max) {
    NOT_IMPLEMENTED();
}

br_uint_32 BrMaterialCount(char* pattern) {
    NOT_IMPLEMENTED();
}

br_uint_32 BrMaterialEnum(char* pattern, br_material_enum_cbfn* callback, void* arg) {
    LOG_TRACE("(\"%s\", %p, %p)", pattern, callback, arg);
    return BrRegistryEnum(&v1db.reg_materials, pattern, (br_enum_cbfn*)callback, arg);
}

br_pixelmap* BrMapAdd(br_pixelmap* pixelmap) {
    LOG_TRACE("(%p)", pixelmap);
    BrRegistryAdd(&v1db.reg_textures, pixelmap);
    BrMapUpdate(pixelmap, 0xFFFu);
    return pixelmap;
}

br_pixelmap* BrMapRemove(br_pixelmap* pixelmap) {
    NOT_IMPLEMENTED();
}

br_pixelmap* BrMapFind(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);
    return BrRegistryFind(&v1db.reg_textures, pattern);
}

br_map_find_cbfn* BrMapFindHook(br_map_find_cbfn* hook) {
    br_map_find_cbfn* old;
    NOT_IMPLEMENTED();
}

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

br_uint_32 BrMapRemoveMany(br_pixelmap** items, int n) {
    int i;
    int r;
    NOT_IMPLEMENTED();
}

br_uint_32 BrMapFindMany(char* pattern, br_pixelmap** items, int max) {
    NOT_IMPLEMENTED();
}

br_uint_32 BrMapCount(char* pattern) {
    NOT_IMPLEMENTED();
}

br_uint_32 BrMapEnum(char* pattern, br_map_enum_cbfn* callback, void* arg) {
    NOT_IMPLEMENTED();
}

br_pixelmap* BrTableAdd(br_pixelmap* pixelmap) {
    NOT_IMPLEMENTED();
}

br_pixelmap* BrTableRemove(br_pixelmap* pixelmap) {
    NOT_IMPLEMENTED();
}

br_pixelmap* BrTableFind(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);
    return BrRegistryFind(&v1db.reg_tables, pattern);
}

br_table_find_cbfn* BrTableFindHook(br_table_find_cbfn* hook) {
    br_table_find_cbfn* old;
    NOT_IMPLEMENTED();
}

br_uint_32 BrTableAddMany(br_pixelmap** items, int n) {
    int i;
    int r;

    for (i = 0; i < n; i++) {
        BrRegistryAdd(&v1db.reg_tables, items[i]);
        BrTableUpdate(items[i], BR_TABU_ALL);
        if (items[i]) {
            ++r;
        }
    }
    return r;
}

br_uint_32 BrTableRemoveMany(br_pixelmap** items, int n) {
    int i;
    int r;
    NOT_IMPLEMENTED();
}

br_uint_32 BrTableFindMany(char* pattern, br_pixelmap** items, int max) {
    NOT_IMPLEMENTED();
}

br_uint_32 BrTableCount(char* pattern) {
    NOT_IMPLEMENTED();
}

br_uint_32 BrTableEnum(char* pattern, br_table_enum_cbfn* callback, void* arg) {
    NOT_IMPLEMENTED();
}
