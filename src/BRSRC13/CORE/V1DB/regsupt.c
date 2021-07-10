#include "regsupt.h"
#include "CORE/FW/register.h"
#include "CORE/V1DB/dbsetup.h"
#include "CORE/V1DB/prepmap.h"
#include "CORE/V1DB/prepmatl.h"
#include "CORE/V1DB/prepmesh.h"
#include "CORE/V1DB/preptab.h"

#include "harness_trace.h"

char rscid[51];

// IDA: br_model* __cdecl BrModelAdd(br_model *model)
br_model* BrModelAdd(br_model* model) {
    BrRegistryAdd(&v1db.reg_models, model);
    BrModelUpdate(model, BR_MODU_ALL);
    return model;
}

// IDA: br_model* __cdecl BrModelRemove(br_model *model)
br_model* BrModelRemove(br_model* model) {
    LOG_TRACE("(%p)", model);

    BrModelClear(model);
    return BrRegistryRemove(&v1db.reg_models, model);
}

// IDA: br_model* __cdecl BrModelFind(char *pattern)
br_model* BrModelFind(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);
    return BrRegistryFind(&v1db.reg_models, pattern);
}

// IDA: br_model_find_cbfn* __cdecl BrModelFindHook(br_model_find_cbfn *hook)
br_model_find_cbfn* BrModelFindHook(br_model_find_cbfn* hook) {
    br_model_find_cbfn* old;
    LOG_TRACE("(%p)", hook);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrModelAddMany(br_model **items, int n)
br_uint_32 BrModelAddMany(br_model** items, int n) {
    int i;
    int r;
    LOG_TRACE("(%p, %d)", items, n);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrModelRemoveMany(br_model **items, int n)
br_uint_32 BrModelRemoveMany(br_model** items, int n) {
    int i;
    int r;
    LOG_TRACE("(%p, %d)", items, n);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrModelFindMany(char *pattern, br_model **items, int max)
br_uint_32 BrModelFindMany(char* pattern, br_model** items, int max) {
    LOG_TRACE("(\"%s\", %p, %d)", pattern, items, max);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrModelCount(char *pattern)
br_uint_32 BrModelCount(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrModelEnum(char *pattern, br_model_enum_cbfn *callback, void *arg)
br_uint_32 BrModelEnum(char* pattern, br_model_enum_cbfn* callback, void* arg) {
    LOG_TRACE("(\"%s\", %p, %p)", pattern, callback, arg);
    NOT_IMPLEMENTED();
}

// IDA: br_material* __cdecl BrMaterialAdd(br_material *material)
br_material* BrMaterialAdd(br_material* material) {
    BrRegistryAdd(&v1db.reg_materials, material);
    BrMaterialUpdate(material, BR_MATU_ALL);
    return material;
}

// IDA: br_material* __cdecl BrMaterialRemove(br_material *material)
br_material* BrMaterialRemove(br_material* material) {
    LOG_TRACE("(%p)", material);

    return (br_material*)BrRegistryRemove(&v1db.reg_materials, material);
}

// IDA: br_material* __cdecl BrMaterialFind(char *pattern)
br_material* BrMaterialFind(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);

    return BrRegistryFind(&v1db.reg_materials, pattern);
}

// IDA: br_material_find_cbfn* __cdecl BrMaterialFindHook(br_material_find_cbfn *hook)
br_material_find_cbfn* BrMaterialFindHook(br_material_find_cbfn* hook) {
    br_material_find_cbfn* old;
    LOG_TRACE("(%p)", hook);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrMaterialAddMany(br_material **items, int n)
br_uint_32 BrMaterialAddMany(br_material** items, int n) {
    int i;
    int r;

    r = 0;
    for (i = 0; i < n; i++) {
        BrRegistryAdd(&v1db.reg_materials, items[i]);
        BrMaterialUpdate(items[i], BR_MATU_ALL);
        if (items[i]) {
            r++;
        }
    }
    return r;
}

// IDA: br_uint_32 __cdecl BrMaterialRemoveMany(br_material **items, int n)
br_uint_32 BrMaterialRemoveMany(br_material** items, int n) {
    int i;
    int r;
    LOG_TRACE("(%p, %d)", items, n);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrMaterialFindMany(char *pattern, br_material **items, int max)
br_uint_32 BrMaterialFindMany(char* pattern, br_material** items, int max) {
    LOG_TRACE("(\"%s\", %p, %d)", pattern, items, max);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrMaterialCount(char *pattern)
br_uint_32 BrMaterialCount(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrMaterialEnum(char *pattern, br_material_enum_cbfn *callback, void *arg)
br_uint_32 BrMaterialEnum(char* pattern, br_material_enum_cbfn* callback, void* arg) {
    LOG_TRACE("(\"%s\", %p, %p)", pattern, callback, arg);
    return BrRegistryEnum(&v1db.reg_materials, pattern, (br_enum_cbfn*)callback, arg);
}

// IDA: br_pixelmap* __cdecl BrMapAdd(br_pixelmap *pixelmap)
br_pixelmap* BrMapAdd(br_pixelmap* pixelmap) {
    LOG_TRACE("(%p)", pixelmap);
    BrRegistryAdd(&v1db.reg_textures, pixelmap);
    BrMapUpdate(pixelmap, 0xFFFu);
    return pixelmap;
}

// IDA: br_pixelmap* __cdecl BrMapRemove(br_pixelmap *pixelmap)
br_pixelmap* BrMapRemove(br_pixelmap* pixelmap) {
    LOG_TRACE("(%p)", pixelmap);

    BrBufferClear(pixelmap);
    return BrRegistryRemove(&v1db.reg_textures, pixelmap);
}

// IDA: br_pixelmap* __cdecl BrMapFind(char *pattern)
br_pixelmap* BrMapFind(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);
    return BrRegistryFind(&v1db.reg_textures, pattern);
}

// IDA: br_map_find_cbfn* __cdecl BrMapFindHook(br_map_find_cbfn *hook)
br_map_find_cbfn* BrMapFindHook(br_map_find_cbfn* hook) {
    br_map_find_cbfn* old;
    LOG_TRACE("(%p)", hook);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrMapAddMany(br_pixelmap **items, int n)
br_uint_32 BrMapAddMany(br_pixelmap** items, int n) {
    int i;
    int r;

    r = 0;
    for (i = 0; i < n; i++) {
        BrRegistryAdd(&v1db.reg_textures, items[i]);
        BrMapUpdate(items[i], 4095);
        if (items[i]) {
            r++;
        }
    }
    return r++;
}

// IDA: br_uint_32 __cdecl BrMapRemoveMany(br_pixelmap **items, int n)
br_uint_32 BrMapRemoveMany(br_pixelmap** items, int n) {
    int i;
    int r;
    LOG_TRACE("(%p, %d)", items, n);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrMapFindMany(char *pattern, br_pixelmap **items, int max)
br_uint_32 BrMapFindMany(char* pattern, br_pixelmap** items, int max) {
    LOG_TRACE("(\"%s\", %p, %d)", pattern, items, max);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrMapCount(char *pattern)
br_uint_32 BrMapCount(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrMapEnum(char *pattern, br_map_enum_cbfn *callback, void *arg)
br_uint_32 BrMapEnum(char* pattern, br_map_enum_cbfn* callback, void* arg) {
    LOG_TRACE("(\"%s\", %p, %p)", pattern, callback, arg);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __cdecl BrTableAdd(br_pixelmap *pixelmap)
br_pixelmap* BrTableAdd(br_pixelmap* pixelmap) {
    LOG_TRACE("(%p)", pixelmap);

    BrRegistryAdd(&v1db.reg_tables, pixelmap);
    BrTableUpdate(pixelmap, BR_TABU_ALL);
    return pixelmap;
}

// IDA: br_pixelmap* __cdecl BrTableRemove(br_pixelmap *pixelmap)
br_pixelmap* BrTableRemove(br_pixelmap* pixelmap) {
    LOG_TRACE("(%p)", pixelmap);
    NOT_IMPLEMENTED();
}

// IDA: br_pixelmap* __cdecl BrTableFind(char *pattern)
br_pixelmap* BrTableFind(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);
    return BrRegistryFind(&v1db.reg_tables, pattern);
}

// IDA: br_table_find_cbfn* __cdecl BrTableFindHook(br_table_find_cbfn *hook)
br_table_find_cbfn* BrTableFindHook(br_table_find_cbfn* hook) {
    br_table_find_cbfn* old;
    LOG_TRACE("(%p)", hook);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrTableAddMany(br_pixelmap **items, int n)
br_uint_32 BrTableAddMany(br_pixelmap** items, int n) {
    int i;
    int r = 0;

    for (i = 0; i < n; i++) {
        BrRegistryAdd(&v1db.reg_tables, items[i]);
        BrTableUpdate(items[i], BR_TABU_ALL);
        if (items[i]) {
            ++r;
        }
    }
    return r;
}

// IDA: br_uint_32 __cdecl BrTableRemoveMany(br_pixelmap **items, int n)
br_uint_32 BrTableRemoveMany(br_pixelmap** items, int n) {
    int i;
    int r;
    LOG_TRACE("(%p, %d)", items, n);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrTableFindMany(char *pattern, br_pixelmap **items, int max)
br_uint_32 BrTableFindMany(char* pattern, br_pixelmap** items, int max) {
    LOG_TRACE("(\"%s\", %p, %d)", pattern, items, max);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrTableCount(char *pattern)
br_uint_32 BrTableCount(char* pattern) {
    LOG_TRACE("(\"%s\")", pattern);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl BrTableEnum(char *pattern, br_table_enum_cbfn *callback, void *arg)
br_uint_32 BrTableEnum(char* pattern, br_table_enum_cbfn* callback, void* arg) {
    LOG_TRACE("(\"%s\", %p, %p)", pattern, callback, arg);
    NOT_IMPLEMENTED();
}
