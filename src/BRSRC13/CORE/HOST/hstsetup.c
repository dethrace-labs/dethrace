#include "hstsetup.h"
#include "harness.h"

host_info hostInfo;
void* functionPointers_BRHOST1[50];
br_image Image_BRHOST1;
char* namePointers_BRHOST1[50];
br_uint_16 nameOrdinals_BRHOST1[50];

// IDA: void __cdecl HostBegin()
void HostBegin() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl HostEnd()
void HostEnd() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: br_error __cdecl HostInfo(host_info *buffer, br_size_t buffersize)
br_error HostInfo(host_info* buffer, br_size_t buffersize) {
    br_int_32 features;
    LOG_TRACE("(%p, %d)", buffer, buffersize);
    NOT_IMPLEMENTED();
}
