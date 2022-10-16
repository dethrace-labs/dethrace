#ifndef _DEVLIST_H_
#define _DEVLIST_H_

#include "brender/br_types.h"

br_error AddRequestedDrivers(void);

br_error devAdd(br_device** pdev, br_device_begin_fn* dev_begin, char* args, br_image* image);

br_error BrDevAdd(br_device** pdev, char* image, char* args);

br_error BrDevAddStatic(br_device** pdev, br_device_begin_fn* dev_begin, char* args);

br_error BrDevCheckAdd(br_device** pdev, char* name, char* args);

br_error BrDevAddConfig(char* config);

br_error BrDevRemove(br_device* dev);

br_error BrDevFind(br_device** pdev, char* pattern);

br_error BrDevFindMany(br_device** devices, br_int_32* ndevices, br_int_32 max_devices, char* pattern);

br_error BrDevCount(br_int_32* ndevices, char* pattern);

br_error BrDevContainedFind(br_object** ph, br_token type, char* pattern, br_token_value* tv);

br_error BrDevContainedFindMany(br_object** objects, br_int_32 max_objects, br_int_32* pnum_objects, br_token type, char* pattern, br_token_value* tv);

br_error BrDevContainedCount(br_int_32* pcount, br_token type, char* pattern, br_token_value* tv);

#endif
