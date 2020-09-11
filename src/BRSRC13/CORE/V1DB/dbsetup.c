#include "dbsetup.h"

#include "CORE/FW/brbegin.h"
#include "CORE/FW/register.h"
#include "CORE/FW/resource.h"
#include "CORE/FW/resreg.h"
#include "CORE/STD/brstdlib.h"
#include "CORE/V1DB/def_mat.h"
#include "CORE/V1DB/def_mdl.h"
#include "harness.h"
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

    for (i = 0; i < 14; i++) {
        BrResClassAdd(&v1db_resourceClasses[i]);
    }

    v1db.default_model = BrResAllocate(v1db.res, sizeof(br_model), BR_MEMORY_MODEL);
    memcpy(v1db.default_model, &_BrDefaultModel, sizeof(br_model));
    v1db.default_material = SetupDefaultMaterial();
    v1db.enabled_lights.type = 2;
    v1db.enabled_clip_planes.name = "clip plane";
    v1db.enabled_clip_planes.type = 7;
    v1db.enabled_lights.max = 16;
    v1db.enabled_lights.name = "light";
    v1db.enabled_clip_planes.max = 6;
    v1db.enabled_horizon_planes.max = 6;
    v1db.enabled_horizon_planes.name = "horizon plane";
    v1db.enabled_horizon_planes.type = 8;
    return 0;
}

br_error BrV1dbEnd() {
    NOT_IMPLEMENTED();
}

br_uint_32 updateTable(br_pixelmap* item, void* arg) {
    NOT_IMPLEMENTED();
}

br_uint_32 updateMap(br_pixelmap* item, void* arg) {
    NOT_IMPLEMENTED();
}

br_uint_32 updateMaterial(br_material* item, void* arg) {
    NOT_IMPLEMENTED();
}

br_uint_32 updateModel(br_model* item, void* arg) {
    NOT_IMPLEMENTED();
}

br_uint_32 clearTable(br_pixelmap* item, void* arg) {
    NOT_IMPLEMENTED();
}

br_uint_32 clearMap(br_pixelmap* item, void* arg) {
    NOT_IMPLEMENTED();
}

br_uint_32 clearMaterial(br_material* item, void* arg) {
    NOT_IMPLEMENTED();
}

br_uint_32 clearModel(br_model* item, void* arg) {
    NOT_IMPLEMENTED();
}

br_error BrV1dbRendererBegin(br_device_pixelmap* destination, br_renderer* renderer) {
    br_renderer_facility* renderer_facility;
    br_error r;
    br_token_value tv[2];
    NOT_IMPLEMENTED();
}

br_renderer* BrV1dbRendererQuery() {
    NOT_IMPLEMENTED();
}

br_error BrV1dbRendererEnd() {
    NOT_IMPLEMENTED();
}

void BrZbBegin(br_uint_8 colour_type, br_uint_8 depth_type) {
    LOG_TRACE("(%d, %d)", colour_type, depth_type);
    STUB();
}

void BrZsBegin(br_uint_8 colour_type, void* primitive, br_uint_32 size) {
    NOT_IMPLEMENTED();
}

void BrZbEnd() {
    NOT_IMPLEMENTED();
}

void BrZsEnd() {
    NOT_IMPLEMENTED();
}

void BrV1dbBeginWrapper_Float() {
    BrBegin();
    BrV1dbBegin();
}

void BrV1dbEndWrapper() {
    NOT_IMPLEMENTED();
}
