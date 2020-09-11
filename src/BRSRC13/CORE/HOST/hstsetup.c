#include "hstsetup.h"
#include "harness.h"

host_info hostInfo;
void* functionPointers_BRHOST1[50];
br_image Image_BRHOST1;
char* namePointers_BRHOST1[50];
br_uint_16 nameOrdinals_BRHOST1[50];

void HostBegin() {
    NOT_IMPLEMENTED();
}

void HostEnd() {
    NOT_IMPLEMENTED();
}

br_error HostInfo(host_info* buffer, br_size_t buffersize) {
    br_int_32 features;
    NOT_IMPLEMENTED();
}
