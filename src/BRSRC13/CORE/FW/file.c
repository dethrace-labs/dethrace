#include "file.h"

#include <stdarg.h>

char rscid[49];

// Offset: 12
// Size: 86
void _BrFileFree(void *res, br_uint_8 res_class, br_size_t size) {
    br_file *file;
}

// Offset: 115
// Size: 77
br_uint_32 BrFileAttributes() {
}

// Offset: 207
// Size: 267
void* BrFileOpenRead(char *name, br_size_t n_magics, br_mode_test_cbfn *mode_test, int *mode_result) {
    void *raw_file;
    br_file *file;
    int bin_mode;
}

// Offset: 490
// Size: 230
void* BrFileOpenWrite(char *name, int mode) {
    void *raw_file;
    br_file *file;
}

// Offset: 732
// Size: 70
void BrFileClose(void *f) {
}

// Offset: 812
// Size: 117
int BrFileEof(void *f) {
}

// Offset: 943
// Size: 120
int BrFileGetChar(void *f) {
}

// Offset: 1077
// Size: 118
void BrFilePutChar(int c, void *f) {
}

// Offset: 1206
// Size: 163
int BrFileRead(void *buf, int size, int n, void *f) {
}

// Offset: 1381
// Size: 163
int BrFileWrite(void *buf, int size, int n, void *f) {
}

// Offset: 1558
// Size: 125
int BrFileGetLine(char *buf, br_size_t buf_len, void *f) {
}

// Offset: 1697
// Size: 118
void BrFilePutLine(char *buf, void *f) {
}

// Offset: 1829
// Size: 118
void BrFileAdvance(long count, void *f) {
}

// Offset: 1960
// Size: 173
int BrFilePrintf(void *f, char *fmt, ...) {
    int n;
    va_list args;
}

