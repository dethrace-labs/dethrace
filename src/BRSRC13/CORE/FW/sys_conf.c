#include "sys_conf.h"
#include "harness.h"

br_token valid_system_config_tokens[5];

br_boolean Is_Valid_Sys_Config_Token(br_token t) {
    br_boolean bOK;
    int i;
    NOT_IMPLEMENTED();
}

br_boolean LoadIniEntry(char* ini_file, char* section_name, br_token t, char* Entry) {
    char Temp[255];
    br_uint_16 size;
    br_value v;
    br_error r;
    NOT_IMPLEMENTED();
}

br_error LoadIniConfig(char* ini_file, char* section_name) {
    NOT_IMPLEMENTED();
}

br_boolean LoadRegistryEntry(char* Reg_Path, void* hKey, br_token t, char* Entry) {
    char Temp[255];
    br_uint_16 size;
    br_value v;
    br_error r;
    NOT_IMPLEMENTED();
}

br_error LoadRegistryConfig(char* Reg_Path, void* hKey) {
    NOT_IMPLEMENTED();
}

br_error BrSetDefaultConfig(br_token t, char* Entry) {
    char Reg_Path[255];
    int v0;
    int v1;
    int v2;
    br_value v;
    NOT_IMPLEMENTED();
}

br_error BrSystemConfigBegin() {
    char temp[255];
    br_value v;
    STUB();
}

br_error BrSystemConfigLoad(br_token t, char* Param1, void* Param2) {
    br_error r;
    NOT_IMPLEMENTED();
}

br_error BrSystemConfigSetString(br_token t, char* string) {
    br_value v;
    NOT_IMPLEMENTED();
}

br_error BrSystemConfigQueryString(br_token t, char* string, int max_size) {
    br_error r;
    br_value v;
    NOT_IMPLEMENTED();
}
