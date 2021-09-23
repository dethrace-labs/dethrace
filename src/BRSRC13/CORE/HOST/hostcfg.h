#ifndef _HOSTCFG_H_
#define _HOSTCFG_H_

#include "brender/br_types.h"

char* HostDefaultDevice();

br_boolean HostIniSectionExists(char* ini_file, char* section_name);

br_error HostIniQuery(char* ini_file, char* section_name, char* entry, char* Buffer, int max, br_uint_16* size);

br_error HostRegistryQuery(void* hKey, char* Path, char* entry, char* Buffer, br_uint_16 max, br_uint_16* size);

#endif
