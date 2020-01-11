#include "hostcfg.h"

// Offset: 18
// Size: 40
char* HostDefaultDevice() {
}

// Offset: 79
// Size: 40
br_boolean HostIniSectionExists(char* ini_file, char* section_name) {
}

// Offset: 132
// Size: 54
br_error HostIniQuery(char* ini_file, char* section_name, char* entry, char* Buffer, int max, br_uint_16* size) {
}

// Offset: 204
// Size: 54
br_error HostRegistryQuery(void* hKey, char* Path, char* entry, char* Buffer, br_uint_16 max, br_uint_16* size) {
}
