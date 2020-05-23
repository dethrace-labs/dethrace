#include "drfile.h"
#include "brender.h"
#include "loading.h"
#include <stdlib.h>

br_filesystem* gOld_file_system;
br_filesystem gFilesystem = {
    "Carmageddon",
    NULL,
    &DRStdioOpenRead,
    &DRStdioOpenWrite,
    &DRStdioClose,
    NULL,
    NULL,
    NULL,
    &DRStdioRead,
    &DRStdioWrite,
    NULL,
    NULL,
    NULL
};

// Added by JeffH
#define DR_MEMORY_FILESYSTEM 140

// Offset: 0
// Size: 64
void* DRStdioOpenRead(char* name, br_size_t n_magics, br_mode_test_cbfn* identify, int* mode_result) {
    if (mode_result) {
        *mode_result = 0;
    }
    return DRfopen(name, "rb");
}

// Offset: 64
// Size: 56
void* DRStdioOpenWrite(char* name, int mode) {
    return gOld_file_system->open_write(name, mode);
}

// Offset: 120
// Size: 46
void DRStdioClose(void* f) {
    gOld_file_system->close(f);
}

// Offset: 168
// Size: 70
br_size_t DRStdioRead(void* buf, br_size_t size, unsigned int n, void* f) {
    br_size_t result;
    return gOld_file_system->read(buf, size, n, f);
}

// Offset: 240
// Size: 70
br_size_t DRStdioWrite(void* buf, br_size_t size, unsigned int n, void* f) {
    br_size_t result;
    return gOld_file_system->write(buf, size, n, f);
}

// Offset: 312
// Size: 187
void InstallDRFileCalls() {
    br_filesystem* temp_system;
    LOG_TRACE("()");
    temp_system = BrMemAllocate(sizeof(br_filesystem), DR_MEMORY_FILESYSTEM);
    gOld_file_system = BrFilesystemSet(temp_system);
    gFilesystem.attributes = gOld_file_system->attributes;
    gFilesystem.eof = gOld_file_system->eof;
    gFilesystem.getchr = gOld_file_system->getchr;
    gFilesystem.putchr = gOld_file_system->putchr;
    gFilesystem.getline = gOld_file_system->getline;
    gFilesystem.putline = gOld_file_system->putline;
    gFilesystem.advance = gOld_file_system->advance;
    BrFilesystemSet(&gFilesystem);
    BrMemFree(temp_system);
}
