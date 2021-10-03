#ifndef _REGSUPT_H_
#define _REGSUPT_H_

#include "brender/br_types.h"

br_model* BrModelAdd(br_model* model);

br_model* BrModelRemove(br_model* model);

br_model* BrModelFind(char* pattern);

br_model_find_cbfn* BrModelFindHook(br_model_find_cbfn* hook);

br_uint_32 BrModelAddMany(br_model** items, int n);

br_uint_32 BrModelRemoveMany(br_model** items, int n);

br_uint_32 BrModelFindMany(char* pattern, br_model** items, int max);

br_uint_32 BrModelCount(char* pattern);

br_uint_32 BrModelEnum(char* pattern, br_model_enum_cbfn* callback, void* arg);

br_material* BrMaterialAdd(br_material* material);

br_material* BrMaterialRemove(br_material* material);

br_material* BrMaterialFind(char* pattern);

br_material_find_cbfn* BrMaterialFindHook(br_material_find_cbfn* hook);

br_uint_32 BrMaterialAddMany(br_material** items, int n);

br_uint_32 BrMaterialRemoveMany(br_material** items, int n);

br_uint_32 BrMaterialFindMany(char* pattern, br_material** items, int max);

br_uint_32 BrMaterialCount(char* pattern);

br_uint_32 BrMaterialEnum(char* pattern, br_material_enum_cbfn* callback, void* arg);

br_pixelmap* BrMapAdd(br_pixelmap* pixelmap);

br_pixelmap* BrMapRemove(br_pixelmap* pixelmap);

br_pixelmap* BrMapFind(char* pattern);

br_map_find_cbfn* BrMapFindHook(br_map_find_cbfn* hook);

br_uint_32 BrMapAddMany(br_pixelmap** items, int n);

br_uint_32 BrMapRemoveMany(br_pixelmap** items, int n);

br_uint_32 BrMapFindMany(char* pattern, br_pixelmap** items, int max);

br_uint_32 BrMapCount(char* pattern);

br_uint_32 BrMapEnum(char* pattern, br_map_enum_cbfn* callback, void* arg);

br_pixelmap* BrTableAdd(br_pixelmap* pixelmap);

br_pixelmap* BrTableRemove(br_pixelmap* pixelmap);

br_pixelmap* BrTableFind(char* pattern);

br_table_find_cbfn* BrTableFindHook(br_table_find_cbfn* hook);

br_uint_32 BrTableAddMany(br_pixelmap** items, int n);

br_uint_32 BrTableRemoveMany(br_pixelmap** items, int n);

br_uint_32 BrTableFindMany(char* pattern, br_pixelmap** items, int max);

br_uint_32 BrTableCount(char* pattern);

br_uint_32 BrTableEnum(char* pattern, br_table_enum_cbfn* callback, void* arg);

#endif
