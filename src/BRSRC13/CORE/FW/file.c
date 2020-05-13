#include "file.h"
#include "CORE/FW/fwsetup.h"
#include "CORE/FW/resource.h"
#include "CORE/STD/brstdlib.h"
#include "debug.h"
#include <stdarg.h>
#include <stddef.h>

char rscid[49];

// Offset: 12
// Size: 86
void _BrFileFree(void* res, br_uint_8 res_class, br_size_t size) {
    br_file* file;
}

// Offset: 115
// Size: 77
br_uint_32 BrFileAttributes() {
}

// Offset: 207
// Size: 267
void* BrFileOpenRead(char* name, br_size_t n_magics, br_mode_test_cbfn* mode_test, int* mode_result) {
    void* raw_file;
    br_file* file;
    int bin_mode;
    LOG_TRACE("(\"%s\", %d, %p, %p)", name, n_magics, mode_test, mode_result);

    if (mode_result) {
        raw_file = fw.fsys->open_read(name, n_magics, mode_test, mode_result);
    } else {
        raw_file = fw.fsys->open_read(name, n_magics, mode_test, &bin_mode);
    }

    if (!raw_file) {
        return NULL;
    }
    file = BrResAllocate(fw.res, sizeof(br_file) + BrStrLen(name), BR_MEMORY_FILE);

    file->writing = 0;
    if (mode_result) {
        file->mode = *mode_result;
    }
    file->raw_file = raw_file;
    BrStrCpy(file->name, name);
    return file;
}

// Offset: 490
// Size: 230
void* BrFileOpenWrite(char* name, int mode) {
    void* raw_file;
    br_file* file;

    raw_file = fw.fsys->open_write(name, mode);
    if (!raw_file) {
        return NULL;
    }
    file = BrResAllocate(fw.res, sizeof(br_file) + BrStrLen(name), BR_MEMORY_FILE);
    file->writing = 1;
    file->mode = mode;
    file->raw_file = raw_file;
    BrStrCpy(file->name, name);
    return file;
}

// Offset: 732
// Size: 70
void BrFileClose(void* f) {
    LOG_TRACE("(%p)", f);
    BrResFree(f);
}

// Offset: 812
// Size: 117
int BrFileEof(void* f) {
    return fw.fsys->eof(((br_file*)f)->raw_file);
}

// Offset: 943
// Size: 120
int BrFileGetChar(void* f) {
    return fw.fsys->getchr(((br_file*)f)->raw_file);
}

// Offset: 1077
// Size: 118
void BrFilePutChar(int c, void* f) {
    return fw.fsys->putchr(c, ((br_file*)f)->raw_file);
}

// Offset: 1206
// Size: 163
int BrFileRead(void* buf, int size, int n, void* f) {
    return fw.fsys->read(buf, size, n, ((br_file*)f)->raw_file);
}

// Offset: 1381
// Size: 163
int BrFileWrite(void* buf, int size, int n, void* f) {
    return fw.fsys->write(buf, size, n, ((br_file*)f)->raw_file);
}

// Offset: 1558
// Size: 125
int BrFileGetLine(char* buf, br_size_t buf_len, void* f) {
    return fw.fsys->getline(buf, buf_len, ((br_file*)f)->raw_file);
}

// Offset: 1697
// Size: 118
void BrFilePutLine(char* buf, void* f) {
    fw.fsys->putline(buf, ((br_file*)f)->raw_file);
}

// Offset: 1829
// Size: 118
void BrFileAdvance(long count, void* f) {
    fw.fsys->advance(count, ((br_file*)f)->raw_file);
}

// Offset: 1960
// Size: 173
int BrFilePrintf(void* f, char* fmt, ...) {
    int n;
    va_list args;
}
