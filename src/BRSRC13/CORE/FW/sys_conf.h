#ifndef _SYS_CONF_H_
#define _SYS_CONF_H_

#include "brender/br_types.h"

br_boolean Is_Valid_Sys_Config_Token(br_token t);

br_boolean LoadIniEntry(char* ini_file, char* section_name, br_token t, char* Entry);

br_error LoadIniConfig(char* ini_file, char* section_name);

br_boolean LoadRegistryEntry(char* Reg_Path, void* hKey, br_token t, char* Entry);

br_error LoadRegistryConfig(char* Reg_Path, void* hKey);

br_error BrSetDefaultConfig(br_token t, char* Entry);

br_error BrSystemConfigBegin();

br_error BrSystemConfigLoad(br_token t, char* Param1, void* Param2);

br_error BrSystemConfigSetString(br_token t, char* string);

br_error BrSystemConfigQueryString(br_token t, char* string, int max_size);

#endif
