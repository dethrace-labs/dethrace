#include "brstdfile.h"

#include "CORE/FW/diag.h"
#include "debug.h"
#include <stdio.h>
#include <string.h>

// Global variables
br_filesystem BrStdioFilesystem = {
    "Standard IO",
    BrStdioAttributes,
    BrStdioOpenRead,
    BrStdioOpenWrite,
    BrStdioClose,
    BrStdioEof,
    BrStdioGetChar,
    BrStdioPutChar,
    BrStdioRead,
    BrStdioWrite,
    BrStdioGetLine,
    BrStdioPutLine,
    BrStdioAdvance
};
br_filesystem* _BrDefaultFilesystem = &BrStdioFilesystem;
char rscid[52]; // $Id: stdfile.c 2.7 1996/11/20 18:07:52 STEVEW Exp $

// Offset: 18
// Size: 40
br_uint_32 BrStdioAttributes() {
    return BR_FS_ATTR_READABLE | BR_FS_ATTR_WRITEABLE | BR_FS_ATTR_HAS_TEXT | BR_FS_ATTR_HAS_BINARY | BR_FS_ATTR_HAS_ADVANCE;
    NOT_IMPLEMENTED();
}

// Offset: 74
// Size: 595
void* BrStdioOpenRead(char* name, br_size_t n_magics, br_mode_test_cbfn* identify, int* mode_result) {
    FILE* fh;
    char* br_path;
    char config_path[512];
    char try_name[512];
    char* cp;
    br_uint_8 magics[16];
    int open_mode;
    LOG_TRACE("(\"%s\", %d, %p, %p)", name, n_magics, identify, mode_result);

    fh = fopen(name, "rb");
    if (!fh) {
        // skip logic around getting BRENDER_PATH from ini files etc
        return NULL;
    }

    if (n_magics) {
        if (fread(magics, 1u, n_magics, fh) != n_magics) {
            fclose(fh);
            return 0;
        }
        if (identify) {
            open_mode = identify(magics, n_magics);
        }
        if (mode_result) {
            *mode_result = open_mode;
        }
    }

    fclose(fh);
    if (open_mode == 0) {
        fh = fopen(name, "rb");
    } else if (open_mode == 1) {
        fh = fopen(name, "rt");
    } else {
        BrFailure("BrStdFileOpenRead: invalid open_mode %d", open_mode);
        return NULL;
    }
    LOG_DEBUG("returning pointer %p", &(*fh));
    return fh;
}

// Offset: 686
// Size: 82
void* BrStdioOpenWrite(char* name, int mode) {
    FILE* fh;

    if (mode == BR_FS_MODE_TEXT) {
        fh = fopen(name, "w");
    } else {
        fh = fopen(name, "wb");
    }

    return fh;
}

// Offset: 781
// Size: 38
void BrStdioClose(void* f) {
    LOG_TRACE("(%p)", f);
    fclose(f);
}

// Offset: 830
// Size: 45
int BrStdioEof(void* f) {
    return feof(f);
}

// Offset: 890
// Size: 117
int BrStdioGetChar(void* f) {
    int c;
    c = fgetc(f);
    //LOG_DEBUG("file pos: %d, char: %d", ftell(f) - 1, c);
    return c;
}

// Offset: 1022
// Size: 41
void BrStdioPutChar(int c, void* f) {
    fputc(c, f);
}

// Offset: 1075
// Size: 53
br_size_t BrStdioRead(void* buf, br_size_t size, unsigned int n, void* f) {
    int i;
    LOG_TRACE("(%p, %d, %d, %p)", buf, size, n, f);
    LOG_DEBUG("file pos: %d", ftell(f));
    i = fread(buf, size, n, f);
    return i;
}

// Offset: 1141
// Size: 53
br_size_t BrStdioWrite(void* buf, br_size_t size, unsigned int n, void* f) {
    return fwrite(buf, size, n, f);
}

// Offset: 1209
// Size: 141
br_size_t BrStdioGetLine(char* buf, br_size_t buf_len, void* f) {
    br_size_t l;

    if (fgets(buf, buf_len, f) == NULL) {
        return 0;
    }

    l = strlen(buf);

    if (l != 0 && buf[l - 1] == '\n') {
        buf[--l] = '\0';
    }

    return l;
}

// Offset: 1365
// Size: 54
void BrStdioPutLine(char* buf, void* f) {
    fputs(buf, f);
    fputc('\n', f);
}

// Offset: 1434
// Size: 46
void BrStdioAdvance(br_size_t count, void* f) {
    fseek(f, count, SEEK_CUR);
}
