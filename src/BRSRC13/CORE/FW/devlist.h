#ifndef _DEVLIST_H_
#define _DEVLIST_H_

#include "br_types.h"

// Offset: 20
// Size: 112
br_error AddRequestedDrivers();

// Offset: 139
// Size: 393
// EAX: pdev
// EDX: dev_begin
// EBX: args
// ECX: image
br_error devAdd(br_device **pdev, br_device_begin_fn *dev_begin, char *args, br_image *image);

// Offset: 541
// Size: 161
br_error BrDevAdd(br_device **pdev, char *image, char *args);

// Offset: 717
// Size: 52
br_error BrDevAddStatic(br_device **pdev, br_device_begin_fn *dev_begin, char *args);

// Offset: 783
// Size: 85
br_error BrDevCheckAdd(br_device **pdev, char *name, char *args);

// Offset: 883
// Size: 342
br_error BrDevAddConfig(char *config);

// Offset: 1237
// Size: 196
br_error BrDevRemove(br_device *dev);

// Offset: 1443
// Size: 195
br_error BrDevFind(br_device **pdev, char *pattern);

// Offset: 1652
// Size: 225
br_error BrDevFindMany(br_device **devices, br_int_32 *ndevices, br_int_32 max_devices, char *pattern);

// Offset: 1888
// Size: 179
br_error BrDevCount(br_int_32 *ndevices, char *pattern);

// Offset: 2086
// Size: 164
br_error BrDevContainedFind(br_object **ph, br_token type, char *pattern, br_token_value *tv);

// Offset: 2273
// Size: 222
br_error BrDevContainedFindMany(br_object **objects, br_int_32 max_objects, br_int_32 *pnum_objects, br_token type, char *pattern, br_token_value *tv);

// Offset: 2515
// Size: 193
br_error BrDevContainedCount(br_int_32 *pcount, br_token type, char *pattern, br_token_value *tv);

#endif
