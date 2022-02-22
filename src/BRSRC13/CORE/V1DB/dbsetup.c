#include "dbsetup.h"

#include "CORE/FW/brbegin.h"
#include "CORE/FW/devsetup.h"
#include "CORE/FW/diag.h"
#include "CORE/FW/register.h"
#include "CORE/FW/resource.h"
#include "CORE/FW/resreg.h"
#include "CORE/STD/brstdlib.h"
#include "CORE/V1DB/def_mat.h"
#include "CORE/V1DB/def_mdl.h"
#include "CORE/V1DB/def_otab.h"
#include "CORE/V1DB/prepmap.h"
#include "CORE/V1DB/prepmatl.h"
#include "CORE/V1DB/prepmesh.h"
#include "CORE/V1DB/preptab.h"
#include "CORE/V1DB/regsupt.h"
#include "harness/hooks.h"
#include "harness/trace.h"
#include <stdio.h>
#include <string.h>

br_resource_class v1db_resourceClasses[14] = {
    { 0u, "VERTICES", BR_MEMORY_VERTICES, NULL, 0u },
    { 0u, "FACES", BR_MEMORY_FACES, NULL, 0u },
    { 0u, "MODEL", BR_MEMORY_MODEL, NULL, 0u },
    { 0u, "MATERIAL", BR_MEMORY_MATERIAL, NULL, 0u },
    { 0u, "MATERIAL_INDEX", BR_MEMORY_MATERIAL_INDEX, NULL, 0u },
    { 0u, "ACTOR", BR_MEMORY_ACTOR, NULL, 0u },
    { 0u, "LIGHT", BR_MEMORY_LIGHT, NULL, 0u },
    { 0u, "CAMERA", BR_MEMORY_CAMERA, NULL, 0u },
    { 0u, "BOUNDS", BR_MEMORY_BOUNDS, NULL, 0u },
    { 0u, "CLIP_PLANE", BR_MEMORY_CLIP_PLANE, NULL, 0u },
    { 0u, "TRANSFORM", BR_MEMORY_TRANSFORM, NULL, 0u },
    { 0u, "ENABLED_ACTORS", BR_MEMORY_ENABLED_ACTORS, NULL, 0u },
    { 0u, "PREPARED_MODEL", BR_MEMORY_PREPARED_MODEL, NULL, 32u },
    { 0u, "ORDER_TABLE", BR_MEMORY_ORDER_TABLE, NULL, 32u }
};

br_v1db_state v1db;

// IDA: br_error __cdecl BrV1dbBegin()
br_error BrV1dbBegin() {
    char* devstr;
    br_size_t s;
    int i;

    if (v1db.active) {
        return 4103;
    }

    BrMemSet(&v1db, 0, sizeof(br_v1db_state));
    v1db.active = 1;
    BrRegistryNew(&v1db.reg_models);
    BrRegistryNew(&v1db.reg_materials);
    BrRegistryNew(&v1db.reg_textures);
    BrRegistryNew(&v1db.reg_tables);
    v1db.res = BrResAllocate(NULL, 0, BR_MEMORY_ANCHOR);

    for (i = 0; i < BR_ASIZE(v1db_resourceClasses); i++) {
        BrResClassAdd(&v1db_resourceClasses[i]);
    }

    v1db.default_model = BrResAllocate(v1db.res, sizeof(br_model), BR_MEMORY_MODEL);
    memcpy(v1db.default_model, &_BrDefaultModel, sizeof(br_model));
    v1db.default_material = SetupDefaultMaterial();
    v1db.enabled_lights.max = 16;
    v1db.enabled_lights.type = BR_ACTOR_LIGHT;
    v1db.enabled_lights.name = "light";
    v1db.enabled_clip_planes.max = 6;
    v1db.enabled_clip_planes.type = BR_ACTOR_CLIP_PLANE;
    v1db.enabled_clip_planes.name = "clip plane";
    v1db.enabled_horizon_planes.max = 6;
    v1db.enabled_horizon_planes.type = BR_ACTOR_HORIZONTAL_PLANE;
    v1db.enabled_horizon_planes.name = "horizon plane";
    return 0;
}

// IDA: br_error __cdecl BrV1dbEnd()
br_error BrV1dbEnd() {
    br_device* dev;
    LOG_TRACE("()");

    if (v1db.active == 0) {
        return 4102;
    }
    v1db.active = 0;
    BrResFree(v1db.res);
    BrMemSet(&v1db, 0, sizeof(v1db));
    return 0;
}

// IDA: br_uint_32 __cdecl updateTable(br_pixelmap *item, void *arg)
br_uint_32 updateTable(br_pixelmap* item, void* arg) {
    LOG_TRACE("(%p, %p)", item, arg);

    BrTableUpdate(item, BR_TABU_ALL);
}

// IDA: br_uint_32 __cdecl updateMap(br_pixelmap *item, void *arg)
br_uint_32 updateMap(br_pixelmap* item, void* arg) {
    LOG_TRACE("(%p, %p)", item, arg);
    
    BrMapUpdate(item, BR_MAPU_ALL);
}

// IDA: br_uint_32 __cdecl updateMaterial(br_material *item, void *arg)
br_uint_32 updateMaterial(br_material* item, void* arg) {
    LOG_TRACE("(%p, %p)", item, arg);

    BrMaterialUpdate(item, BR_MATU_ALL);
}

// IDA: br_uint_32 __cdecl updateModel(br_model *item, void *arg)
br_uint_32 updateModel(br_model* item, void* arg) {
    LOG_TRACE("(%p, %p)", item, arg);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __cdecl clearTable(br_pixelmap *item, void *arg)
br_uint_32 clearTable(br_pixelmap* item, void* arg) {
    LOG_TRACE("(%p, %p)", item, arg);

    BrBufferClear(item);
}

// IDA: br_uint_32 __cdecl clearMap(br_pixelmap *item, void *arg)
br_uint_32 clearMap(br_pixelmap* item, void* arg) {
    LOG_TRACE("(%p, %p)", item, arg);

    BrBufferClear(item);
}

// IDA: br_uint_32 __cdecl clearMaterial(br_material *item, void *arg)
br_uint_32 clearMaterial(br_material* item, void* arg) {
    LOG_TRACE("(%p, %p)", item, arg);

    BrMaterialClear(item);
}

// IDA: br_uint_32 __cdecl clearModel(br_model *item, void *arg)
br_uint_32 clearModel(br_model* item, void* arg) {
    LOG_TRACE("(%p, %p)", item, arg);

    BrModelClear(item);
}

// IDA: br_error __cdecl BrV1dbRendererBegin(br_device_pixelmap *destination, br_renderer *renderer)
br_error BrV1dbRendererBegin(br_device_pixelmap* destination, br_renderer* renderer) {
    br_renderer_facility* renderer_facility;
    br_error r;
    br_token_value tv[2];
    LOG_TRACE("(%p, %p)", destination, renderer);

    Harness_Hook_BrV1dbRendererBegin(&v1db);
    return 0;

    // FIXME: use this logic once the clouds clear up
    renderer_facility = NULL;
    tv[0].t = 0;
    tv[0].v.u32 = 0;
    tv[1].t = 0;
    tv[1].v.u32 = 0;

    if (renderer == NULL) {
        r = BrRendererFacilityFind(&renderer_facility, destination, BRT_FLOAT);
        if (r != 0) {
            return r;
        }
        if (destination != NULL) {
            tv[0].t = BRT_DESTINATION_O;
            tv[0].v.o = (br_object*)destination;
        }
        r = (*(br_renderer_facility_dispatch**)renderer_facility)->_rendererNew(renderer_facility, &renderer, tv);
        if (r != 0) {
            return r;
        }
    }
    v1db.renderer = renderer;
    r = BrGeometryFormatFind(&v1db.format_model, renderer, renderer_facility, BRT_FLOAT, BRT_GEOMETRY_V1_MODEL);
    if (r != 0) {
        return r;
    }
    r = BrGeometryFormatFind(&v1db.format_buckets, renderer, renderer_facility, BRT_FLOAT, BRT_GEOMETRY_V1_BUCKETS);
    if (r != 0) {
        return r;
    }
    r= BrGeometryFormatFind((br_geometry**)&v1db.format_lighting, renderer, renderer_facility, BRT_FLOAT, BRT_GEOMETRY_LIGHTING);
    if (r != 0) {
        return r;
    }
    BrModelUpdate(v1db.default_model, BR_MODU_ALL);
    v1db.default_order_table = &_BrDefaultOrderTable;
    v1db.primary_order_table = NULL;
    BrTableEnum(NULL, updateTable, NULL);
    BrMapEnum(NULL, updateMap, NULL);
    BrMaterialEnum(NULL, updateMaterial, NULL);
    BrMaterialUpdate(v1db.default_material, BR_MATU_ALL);
    BrMaterialUpdate(v1db.default_material, BR_MATU_ALL);
    return 0;
}

// IDA: br_renderer* __cdecl BrV1dbRendererQuery()
br_renderer* BrV1dbRendererQuery() {
    LOG_TRACE("()");

    return v1db.renderer;
}

// IDA: br_error __cdecl BrV1dbRendererEnd()
br_error BrV1dbRendererEnd() {
    LOG_TRACE("()");

    BrTableEnum(NULL, clearTable, NULL);
    BrMapEnum(NULL, clearMap, NULL);
    BrMaterialEnum(NULL, clearMaterial, NULL);
    BrModelEnum(NULL, clearModel, NULL);
    BrMaterialClear(v1db.default_material);
    BrModelClear(v1db.default_model);
    v1db.default_order_table = NULL;
    v1db.default_render_data = NULL;
    v1db.primary_order_table = NULL;
    v1db.format_model = NULL;
    v1db.renderer->dispatch->_free((br_object*)v1db.renderer);
    v1db.renderer = NULL;
}

// IDA: void __cdecl BrZbBegin(br_uint_8 colour_type, br_uint_8 depth_type)
void BrZbBegin(br_uint_8 colour_type, br_uint_8 depth_type) {
    LOG_TRACE("(%d, %d)", colour_type, depth_type);

    if (v1db.zs_active == 0 && v1db.zb_active == 0) {
        if (BrV1dbRendererBegin((br_device_pixelmap*)BrDevLastBeginQuery(), NULL) != 0) {
            BrFailure("Failed to load renderer\n");
        }
    }
    v1db.zb_active = 1;
}

// IDA: void __cdecl BrZsBegin(br_uint_8 colour_type, void *primitive, br_uint_32 size)
void BrZsBegin(br_uint_8 colour_type, void* primitive, br_uint_32 size) {
    LOG_TRACE("(%d, %p, %d)", colour_type, primitive, size);

    if (v1db.zs_active == 0 && v1db.zb_active == 0) {
        if (BrV1dbRendererBegin((br_device_pixelmap*)BrDevLastBeginQuery(), NULL) != 0) {
            BrFailure("Failed to load renderer\n");
        }
    }
    v1db.zs_active = 1;
    v1db.heap.base = primitive;
    v1db.heap.size = size;
}

// IDA: void __cdecl BrZbEnd()
void BrZbEnd() {
    LOG_TRACE("()");

    v1db.zb_active = 0;
    if (v1db.zs_active == 0 && v1db.renderer != NULL) {
        BrV1dbRendererEnd();
    }
}

// IDA: void __cdecl BrZsEnd()
void BrZsEnd() {
    LOG_TRACE("()");

    v1db.zs_active = 0;
    if (v1db.zb_active == 0 && v1db.renderer != NULL) {
        BrV1dbRendererEnd();
    }
}

// IDA: void __cdecl BrV1dbBeginWrapper_Float()
void BrV1dbBeginWrapper_Float() {
    BrBegin();
    BrV1dbBegin();
}

// IDA: void __cdecl BrV1dbEndWrapper()
void BrV1dbEndWrapper() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}
