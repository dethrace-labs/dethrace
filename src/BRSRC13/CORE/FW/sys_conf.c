#include "sys_conf.h"
#include "debug.h"

br_token valid_system_config_tokens[5];

// Offset: 26
// Size: 106
// EAX: t
br_boolean Is_Valid_Sys_Config_Token(br_token t) {
    br_boolean bOK;
    int i;
    NOT_IMPLEMENTED();
}

// Offset: 145
// Size: 130
// EAX: ini_file
// EDX: section_name
// EBX: t
// ECX: Entry
br_boolean LoadIniEntry(char* ini_file, char* section_name, br_token t, char* Entry) {
    char Temp[255];
    br_uint_16 size;
    br_value v;
    br_error r;
    NOT_IMPLEMENTED();
}

// Offset: 289
// Size: 213
// EAX: ini_file
// EDX: section_name
br_error LoadIniConfig(char* ini_file, char* section_name) {
    NOT_IMPLEMENTED();
}

// Offset: 520
// Size: 130
// EAX: Reg_Path
// EDX: hKey
// EBX: t
// ECX: Entry
br_boolean LoadRegistryEntry(char* Reg_Path, void* hKey, br_token t, char* Entry) {
    char Temp[255];
    br_uint_16 size;
    br_value v;
    br_error r;
    NOT_IMPLEMENTED();
}

// Offset: 669
// Size: 224
// EAX: Reg_Path
// EDX: hKey
br_error LoadRegistryConfig(char* Reg_Path, void* hKey) {
    NOT_IMPLEMENTED();
}

// Offset: 912
// Size: 193
// EAX: t
// EDX: Entry
br_error BrSetDefaultConfig(br_token t, char* Entry) {
    char Reg_Path[255];
    int v0;
    int v1;
    int v2;
    br_value v;
    NOT_IMPLEMENTED();
}

// Offset: 1125
// Size: 328
br_error BrSystemConfigBegin() {
    char temp[255];
    br_value v;
    LOG_WARN("Not implemented");
}

// Offset: 1472
// Size: 188
br_error BrSystemConfigLoad(br_token t, char* Param1, void* Param2) {
    br_error r;
    NOT_IMPLEMENTED();
}

// Offset: 1684
// Size: 158
br_error BrSystemConfigSetString(br_token t, char* string) {
    br_value v;
    NOT_IMPLEMENTED();
}

// Offset: 1868
// Size: 175
br_error BrSystemConfigQueryString(br_token t, char* string, int max_size) {
    br_error r;
    br_value v;
    NOT_IMPLEMENTED();
}
