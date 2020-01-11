#include "hstsetup.h"

host_info hostInfo;
void* functionPointers_BRHOST1[50];
br_image Image_BRHOST1;
char* namePointers_BRHOST1[50];
br_uint_16 nameOrdinals_BRHOST1[50];

// Offset: 10
// Size: 49
void HostBegin() {
}

// Offset: 67
// Size: 49
void HostEnd() {
}

// Offset: 125
// Size: 125
br_error HostInfo(host_info* buffer, br_size_t buffersize) {
    br_int_32 features;
}
