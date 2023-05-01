#include "hostcfg.h"
#include "harness/trace.h"

// IDA: char* __cdecl HostDefaultDevice()
char* HostDefaultDevice(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_boolean __cdecl HostIniSectionExists(char *ini_file, char *section_name)
br_boolean HostIniSectionExists(char* ini_file, char* section_name) {
    LOG_TRACE("(\"%s\", \"%s\")", ini_file, section_name);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl HostIniQuery(char *ini_file, char *section_name, char *entry, char *Buffer, int max, br_uint_16 *size)
br_error HostIniQuery(char* ini_file, char* section_name, char* entry, char* Buffer, int max, br_uint_16* size) {
    LOG_TRACE("(\"%s\", \"%s\", \"%s\", \"%s\", %d, %p)", ini_file, section_name, entry, Buffer, max, size);
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl HostRegistryQuery(void *hKey, char *Path, char *entry, char *Buffer, br_uint_16 max, br_uint_16 *size)
br_error HostRegistryQuery(void* hKey, char* Path, char* entry, char* Buffer, br_uint_16 max, br_uint_16* size) {
    LOG_TRACE("(%p, \"%s\", \"%s\", \"%s\", %d, %p)", hKey, Path, entry, Buffer, max, size);
    NOT_IMPLEMENTED();
}
