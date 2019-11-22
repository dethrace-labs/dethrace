#ifndef _SYS_CONF_H_
#define _SYS_CONF_H_

#include "br_types.h"

// Offset: 26
// Size: 106
// EAX: t
br_boolean Is_Valid_Sys_Config_Token(br_token t);

// Offset: 145
// Size: 130
// EAX: ini_file
// EDX: section_name
// EBX: t
// ECX: Entry
br_boolean LoadIniEntry(char *ini_file, char *section_name, br_token t, char *Entry);

// Offset: 289
// Size: 213
// EAX: ini_file
// EDX: section_name
br_error LoadIniConfig(char *ini_file, char *section_name);

// Offset: 520
// Size: 130
// EAX: Reg_Path
// EDX: hKey
// EBX: t
// ECX: Entry
br_boolean LoadRegistryEntry(char *Reg_Path, void *hKey, br_token t, char *Entry);

// Offset: 669
// Size: 224
// EAX: Reg_Path
// EDX: hKey
br_error LoadRegistryConfig(char *Reg_Path, void *hKey);

// Offset: 912
// Size: 193
// EAX: t
// EDX: Entry
br_error BrSetDefaultConfig(br_token t, char *Entry);

// Offset: 1125
// Size: 328
br_error BrSystemConfigBegin();

// Offset: 1472
// Size: 188
br_error BrSystemConfigLoad(br_token t, char *Param1, void *Param2);

// Offset: 1684
// Size: 158
br_error BrSystemConfigSetString(br_token t, char *string);

// Offset: 1868
// Size: 175
br_error BrSystemConfigQueryString(br_token t, char *string, int max_size);

#endif
