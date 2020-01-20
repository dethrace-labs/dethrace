#ifndef _DBSETUP_H_
#define _DBSETUP_H_

#include "br_types.h"

extern br_v1db_state v1db;

// Offset: 12
// Size: 345
br_error BrV1dbBegin();

// Offset: 367
// Size: 103
br_error BrV1dbEnd();

// Offset: 482
// Size: 89
br_uint_32 updateTable(br_pixelmap* item, void* arg);

// Offset: 581
// Size: 89
br_uint_32 updateMap(br_pixelmap* item, void* arg);

// Offset: 685
// Size: 89
br_uint_32 updateMaterial(br_material* item, void* arg);

// Offset: 786
// Size: 89
br_uint_32 updateModel(br_model* item, void* arg);

// Offset: 886
// Size: 79
br_uint_32 clearTable(br_pixelmap* item, void* arg);

// Offset: 974
// Size: 79
br_uint_32 clearMap(br_pixelmap* item, void* arg);

// Offset: 1067
// Size: 79
br_uint_32 clearMaterial(br_material* item, void* arg);

// Offset: 1157
// Size: 79
br_uint_32 clearModel(br_model* item, void* arg);

// Offset: 1256
// Size: 462
br_error BrV1dbRendererBegin(br_device_pixelmap* destination, br_renderer* renderer);

// Offset: 1738
// Size: 41
br_renderer* BrV1dbRendererQuery();

// Offset: 1797
// Size: 222
br_error BrV1dbRendererEnd();

// Offset: 2029
// Size: 94
void BrZbBegin(br_uint_8 colour_type, br_uint_8 depth_type);

// Offset: 2133
// Size: 141
void BrZsBegin(br_uint_8 colour_type, void* primitive, br_uint_32 size);

// Offset: 2282
// Size: 65
void BrZbEnd();

// Offset: 2355
// Size: 65
void BrZsEnd();

// Offset: 2445
// Size: 40
void BrV1dbBeginWrapper_Float();

// Offset: 2502
// Size: 40
void BrV1dbEndWrapper();

#endif
