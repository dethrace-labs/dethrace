#include "sys_conf.h"
#include "harness/trace.h"

br_token valid_system_config_tokens[5];

// IDA: br_boolean __usercall Is_Valid_Sys_Config_Token@<EAX>(br_token t@<EAX>)
br_boolean Is_Valid_Sys_Config_Token(br_token t) {
    br_boolean bOK;
    int i;
    LOG_TRACE("(%d)", t);
    NOT_IMPLEMENTED();
}

// IDA: br_boolean __usercall LoadIniEntry@<EAX>(char *ini_file@<EAX>, char *section_name@<EDX>, br_token t@<EBX>, char *Entry@<ECX>)
br_boolean LoadIniEntry(char* ini_file, char* section_name, br_token t, char* Entry) {
    char Temp[255];
    br_uint_16 size;
    br_value v;
    br_error r;
    LOG_TRACE("(\"%s\", \"%s\", %d, \"%s\")", ini_file, section_name, t, Entry);
    NOT_IMPLEMENTED();
}

// IDA: br_error __usercall LoadIniConfig@<EAX>(char *ini_file@<EAX>, char *section_name@<EDX>)
br_error LoadIniConfig(char* ini_file, char* section_name) {
    LOG_TRACE("(\"%s\", \"%s\")", ini_file, section_name);
    NOT_IMPLEMENTED();
}

// IDA: br_boolean __usercall LoadRegistryEntry@<EAX>(char *Reg_Path@<EAX>, void *hKey@<EDX>, br_token t@<EBX>, char *Entry@<ECX>)
br_boolean LoadRegistryEntry(char* Reg_Path, void* hKey, br_token t, char* Entry) {
    char Temp[255];
    br_uint_16 size;
    br_value v;
    br_error r;
    LOG_TRACE("(\"%s\", %p, %d, \"%s\")", Reg_Path, hKey, t, Entry);
    NOT_IMPLEMENTED();
}

// IDA: br_error __usercall LoadRegistryConfig@<EAX>(char *Reg_Path@<EAX>, void *hKey@<EDX>)
br_error LoadRegistryConfig(char* Reg_Path, void* hKey) {
    LOG_TRACE("(\"%s\", %p)", Reg_Path, hKey);
    NOT_IMPLEMENTED();
}

// IDA: br_error __usercall BrSetDefaultConfig@<EAX>(br_token t@<EAX>, char *Entry@<EDX>)
br_error BrSetDefaultConfig(br_token t, char* Entry) {
    char Reg_Path[255];
    int v0;
    int v1;
    int v2;
    br_value v;
    LOG_TRACE("(%d, \"%s\")", t, Entry);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrSystemConfigBegin()
br_error BrSystemConfigBegin() {
    char temp[255];
    br_value v;
    STUB();
}

// IDA: br_error __cdecl BrSystemConfigLoad(br_token t, char *Param1, void *Param2)
br_error BrSystemConfigLoad(br_token t, char* Param1, void* Param2) {
    br_error r;
    LOG_TRACE("(%d, \"%s\", %p)", t, Param1, Param2);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrSystemConfigSetString(br_token t, char *string)
br_error BrSystemConfigSetString(br_token t, char* string) {
    br_value v;
    LOG_TRACE("(%d, \"%s\")", t, string);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl BrSystemConfigQueryString(br_token t, char *string, int max_size)
br_error BrSystemConfigQueryString(br_token t, char* string, int max_size) {
    br_error r;
    br_value v;
    LOG_TRACE("(%d, \"%s\", %d)", t, string, max_size);
    NOT_IMPLEMENTED();
}
