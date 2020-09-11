#include "datafile.h"
#include "CORE/FW/bswap.h"
#include "CORE/FW/diag.h"
#include "CORE/FW/fwsetup.h"
#include "CORE/FW/resource.h"
#include "CORE/STD/brstdlib.h"
#include "file.h"
#include "harness.h"

#include <stdint.h>
#include <stdio.h>

br_file_primitives _BrFilePrimsNull = {
    "NULL",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

br_file_primitives _BrFilePrimsReadBinary = {
    "Read Binary",
    &DfSkipBinary,
    NULL,
    &DfChunkReadBinary,
    NULL,
    &DfCountReadBinary,
    &DfCountSizeBinary,
    NULL,
    &DfStructReadBinary,
    &DfStructSizeBinary,
    NULL,
    &DfBlockReadBinary,
    &DfBlockSizeBinary,
    NULL,
    &DfNameReadBinary,
    &DfNameSizeBinary
};

br_file_primitives _BrFilePrimsWriteBinary = {
    "Write Binary",
    &DfSkipBinary,
    &DfChunkWriteBinary,
    NULL,
    &DfCountWriteBinary,
    NULL,
    &DfCountSizeBinary,
    &DfStructWriteBinary,
    NULL,
    &DfStructSizeBinary,
    &DfBlockWriteBinary,
    NULL,
    &DfBlockSizeBinary,
    &DfNameWriteBinary,
    NULL,
    &DfNameSizeBinary
};

br_file_primitives _BrFilePrimsReadText = {
    "Read Text",
    &DfSkipText,
    NULL,
    &DfChunkReadText,
    NULL,
    &DfCountReadText,
    (int (*)(br_datafile*))(&DfNameSizeText),
    NULL,
    &DfStructReadText,
    &DfStructSizeText,
    NULL,
    &DfBlockReadText,
    &DfBlockSizeText,
    NULL,
    &DfNameReadText,
    &DfNameSizeText
};

br_file_primitives _BrFilePrimsWriteText = {
    "Write Text",
    &DfSkipText,
    &DfChunkWriteText,
    NULL,
    &DfCountWriteText,
    NULL,
    (int (*)(br_datafile*))(&DfNameSizeText),
    &DfStructWriteText,
    NULL,
    &DfStructSizeText,
    &DfBlockWriteText,
    NULL,
    &DfBlockSizeText,
    &DfNameWriteText,
    NULL,
    &DfNameSizeText
};

char* member_type_names[32];
struct {
    int type;
    void* value;
    int count;
} DatafileStack[1024];
char* ChunkNames[61];
char rscid[52];
int DatafileStackTop;

void DfPush(int type, void* value, int count) {
    LOG_TRACE9("(%d, %p, %d)", type, value, count);

    if (DatafileStackTop >= 1024) {
        BrFailure("DatafileStack Overflow");
    }
    DatafileStack[DatafileStackTop].type = type;
    DatafileStack[DatafileStackTop].count = count;
    DatafileStack[DatafileStackTop].value = value;
    DatafileStackTop++;
}

void* DfPop(int type, int* countp) {
    LOG_TRACE9("(%d, %p)", type, countp);

    if (DatafileStackTop <= 0) {
        BrFailure("DatafileStack Underflow");
    }
    DatafileStackTop--;
    if (type != DatafileStack[DatafileStackTop].type) {
        BrFailure("DatafileStack type mismatch, wanted %d, got %d", type, DatafileStack[DatafileStackTop].type);
    }
    if (countp) {
        *countp = DatafileStack[DatafileStackTop].count;
    }
    return DatafileStack[DatafileStackTop].value;
}

void* DfTop(int type, int* countp) {
    if (DatafileStackTop <= 0)
        BrFailure("DatafileStack Underflow");
    if (type != DatafileStack[DatafileStackTop - 1].type)
        BrFailure("DatafileStack type mismatch, wanted %d, got %d", type, DatafileStack[DatafileStackTop].type);
    if (countp)
        *countp = DatafileStack[DatafileStackTop - 1].count;
    return DatafileStack[DatafileStackTop - 1].value;
    NOT_IMPLEMENTED();
}

int DfTopType() {
    if (DatafileStackTop > 0) {
        return DatafileStack[DatafileStackTop - 1].type;
    } else {
        return 0;
    }
    NOT_IMPLEMENTED();
}

int TextReadLine(br_datafile* df, char** ident, char** data) {
    char* cp;
    NOT_IMPLEMENTED();
}

br_uint_16 scalarTypeConvert(br_datafile* df, br_uint_16 t) {
    LOG_TRACE9("(%p, %d)", df, t);

    if (df->scalar_type < BRT_FIXED) {
        goto LABEL_20;
    }

    if (df->scalar_type > BRT_FIXED) {
        if (df->scalar_type == BRT_FLOAT) {
            if (t - 10 <= BRT_FIXED) {
                switch (t) {
                case 10:
                    return 8;
                case 11:
                    return 24;
                case 12:
                    return 25;
                case 13:
                case 14:
                case 15:
                case 16:
                case 17:
                case 18:
                    return t;
                case 19:
                    return 29;
                    break;
                case 20:
                    return 30;
                    break;
                case 21:
                    return 31;
                    break;
                }
                return t;
            }
            NOT_IMPLEMENTED();
            return t - 10;
        }
    LABEL_20:
        if (t >= 0xAu
            && (t <= 0xAu || (t >= 0x13u && t <= 0x15u))) {
            BrFailure("Incorrect scalar type");
        }
        return t;
    }
    if (t - 10 > BRT_FIXED) {
        return t;
    }
    switch (t) {
    case 10:
        return 6;
    case 11:
        return 22;
    case 12:
        return 23;
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
        return t;
    case 19:
        return 26;
    case 20:
        return 27;
    case 21:
        return 28;
    }

    return t;
}

br_uint_32 DfStructWriteBinary(br_datafile* df, br_file_struct* str, void* base) {
    unsigned int m;
    int i;
    int n;
    unsigned char* mp;
    br_file_struct_member* sm;
    struct {
        unsigned char b[8];
        float f;
    } conv;
    NOT_IMPLEMENTED();
}

br_uint_32 DfStructReadBinary(br_datafile* df, br_file_struct* str, void* base) {
    char tmp_string[256];
    unsigned int m;
    int i;
    int c;
    int n;
    unsigned char* mp;
    br_file_struct_member* sm;
    union {
        unsigned char b[8];
        float f;
    } conv;

    LOG_TRACE9("(%p, %p, %p)", df, str, base);

    for (m = 0; m < str->nmembers; m++) {

        sm = &str->members[m];

        c = scalarTypeConvert(df, sm->type);
        mp = ((unsigned char*)base) + sm->offset;
        if (c > 0x1f) {
            continue;
        }

        switch (c) {
        case 0:
        case 1:
        case 13:
            *mp = BrFileGetChar(df->h);
            goto LABEL_33;
        case 2:
        case 3:
        case 7:
        case 14:
            mp[1] = BrFileGetChar(df->h);
            *mp = BrFileGetChar(df->h);
            goto LABEL_33;
        case 4:
        case 5:
        case 8:
        case 15:
        case 24:
        case 25:
            mp[3] = BrFileGetChar(df->h);
            mp[2] = BrFileGetChar(df->h);
            mp[1] = BrFileGetChar(df->h);
            *mp = BrFileGetChar(df->h);
            goto LABEL_33;
        case 6:
            LOG_PANIC("Not implemented?");
            conv.b[3] = BrFileGetChar(df->h);
            conv.b[2] = BrFileGetChar(df->h);
            conv.b[1] = BrFileGetChar(df->h);
            conv.b[0] = BrFileGetChar(df->h);
            *mp = (int)(conv.f * 65536.0f);
            goto LABEL_33;

        case 9:
            mp[7] = BrFileGetChar(df->h);
            mp[6] = BrFileGetChar(df->h);
            mp[5] = BrFileGetChar(df->h);
            mp[4] = BrFileGetChar(df->h);
            mp[3] = BrFileGetChar(df->h);
            mp[2] = BrFileGetChar(df->h);
            mp[1] = BrFileGetChar(df->h);
            *mp = BrFileGetChar(df->h);
            goto LABEL_33;
        case 10:
        case 11:
        case 12:
        case 19:
        case 20:
        case 21:
            LOG_WARN("Ignoring member type");
            goto LABEL_33;
        case 16:
            LOG_PANIC("Not implemented?");
            DfStructReadBinary(df, str, mp);
            goto LABEL_33;
        case 17:
            for (n = 0; n < 255; n++) {
                c = BrFileGetChar(df->h);
                if (!c || c == -1)
                    break;
                tmp_string[n] = c;
            }
            tmp_string[n] = 0;
            *(intptr_t*)mp = (intptr_t*)BrResStrDup(df->res ? df->res : fw.res, tmp_string);
            goto LABEL_33;
        case 18:
            mp[2] = BrFileGetChar(df->h);
            mp[1] = BrFileGetChar(df->h);
            *mp = BrFileGetChar(df->h);
            goto LABEL_33;
        case 22:
            LOG_PANIC("Not implemented?");
            conv.b[3] = BrFileGetChar(df->h);
            conv.b[2] = BrFileGetChar(df->h);
            conv.b[1] = BrFileGetChar(df->h);
            conv.b[0] = BrFileGetChar(df->h);
            *mp = (int)(conv.f * 32768.0f);
            goto LABEL_33;
        case 23:
            LOG_PANIC("Not implemented?");
            conv.b[3] = BrFileGetChar(df->h);
            conv.b[2] = BrFileGetChar(df->h);
            conv.b[1] = BrFileGetChar(df->h);
            conv.b[0] = BrFileGetChar(df->h);
            *mp = (int)(conv.f * 65536.0f);
            goto LABEL_33;
        case 26:
            n = 2;
            goto LABEL_15;
        case 27:
            n = 3;
            goto LABEL_15;
        case 28:
            n = 4;
        LABEL_15:
            LOG_PANIC("Not implemented?");
            i = 0;
            for (i = 0; i < n; i++) {
                conv.b[3] = BrFileGetChar(df->h);
                conv.b[2] = BrFileGetChar(df->h);
                conv.b[1] = BrFileGetChar(df->h);
                conv.b[0] = BrFileGetChar(df->h);
                *mp = (int)(conv.f * 65536.0f);
                mp += sizeof(int);
            }
            goto LABEL_33;
        case 29:
            n = 2;
            break;
        case 30:
            n = 3;
            break;
        case 31:
            n = 4;
            break;
        }
        for (i = 0; i < n; i++) {
            conv.b[3] = BrFileGetChar(df->h);
            conv.b[2] = BrFileGetChar(df->h);
            conv.b[1] = BrFileGetChar(df->h);
            conv.b[0] = BrFileGetChar(df->h);
            *mp = conv.f;
            mp += 4;
        }

    LABEL_33:
        i = 0;
    }

    return 1;
}

int DfStructSizeBinary(br_datafile* df, br_file_struct* str, void* base) {
    unsigned char* mp;
    unsigned int m;
    br_file_struct_member* sm;
    int bytes;
    NOT_IMPLEMENTED();
}

int EnumFromString(br_file_enum* e, char* str) {
    unsigned int m;
    NOT_IMPLEMENTED();
}

char* EnumToString(br_file_enum* e, int num) {
    unsigned int m;
    NOT_IMPLEMENTED();
}

br_uint_32 DfStructWriteText(br_datafile* df, br_file_struct* str, void* base) {
    NOT_IMPLEMENTED();
}

br_uint_32 StructWriteTextSub(br_datafile* df, br_file_struct* str, void* base, int indent) {
    unsigned int m;
    int i;
    int w;
    int add_comment;
    void* mp;
    br_file_struct_member* sm;
    NOT_IMPLEMENTED();
}

br_uint_32 DfStructReadText(br_datafile* df, br_file_struct* str, void* base) {
    char* id;
    char* data;
    NOT_IMPLEMENTED();
}

br_uint_32 StructReadTextSub(br_datafile* df, br_file_struct* str, void* base) {
    unsigned int m;
    unsigned int r;
    unsigned int g;
    unsigned int b;
    int i;
    int n;
    void* mp;
    br_file_struct_member* sm;
    char* id;
    char* data;
    char* ep;
    NOT_IMPLEMENTED();
}

int DfStructSizeText(br_datafile* df, br_file_struct* str, void* base) {
    unsigned int m;
    br_file_struct_member* sm;
    int lines;
    NOT_IMPLEMENTED();
}

br_uint_32 DfStructWriteArray(br_datafile* df, br_file_struct* str, void* base, int n) {
    char* cp;
    int i;
    NOT_IMPLEMENTED();
}

br_uint_32 DfStructReadArray(br_datafile* df, br_file_struct* str, void* base, int n) {
    char* cp;
    int i;
    LOG_TRACE("(%p, %p, %p, %d)", df, str, base, n);

    cp = (char*)base;
    for (i = 0; i < n; i++) {
        if (BrFileEof(df->h)) {
            break;
        }
        df->prims->struct_read(df, str, cp);
        cp += str->mem_size;
    }
    return i;
}

int DfChunkWriteText(br_datafile* df, br_uint_32 id, br_uint_32 length) {
    NOT_IMPLEMENTED();
}

int DfChunkReadText(br_datafile* df, br_uint_32* plength) {
    int i;
    char* id;
    char* data;
    LOG_TRACE("(%p, %p)", df, plength);
    NOT_IMPLEMENTED();
}

int DfChunkWriteBinary(br_datafile* df, br_uint_32 id, br_uint_32 length) {
    br_uint_32 l;
    NOT_IMPLEMENTED();
}

int DfChunkReadBinary(br_datafile* df, br_uint_32* plength) {
    br_uint_32 id;
    br_uint_32 l;
    LOG_TRACE9("(%p, %p)", df, plength);

    if (BrFileEof(df->h)) {
        return -1;
    }
    BrFileRead(&id, 4, 1, df->h);
    if (BrFileEof(df->h)) {
        return -1;
    }
    id = BrSwap32(id);
    BrFileRead(&l, 4, 1, df->h);
    if (BrFileEof(df->h)) {
        return -1;
    }
    if (plength) {
        *plength = BrSwap32(l);
    }
    return id;
}

void DfCountWriteText(br_datafile* df, br_uint_32 count) {
    NOT_IMPLEMENTED();
}

br_uint_32 DfCountReadText(br_datafile* df) {
    char* id;
    char* data;
    NOT_IMPLEMENTED();
}

void DfCountWriteBinary(br_datafile* df, br_uint_32 count) {
    br_uint_32 l;
    NOT_IMPLEMENTED();
}

br_uint_32 DfCountReadBinary(br_datafile* df) {
    br_uint_32 l;
    BrFileRead(&l, 4, 1, df->h);
    return BrSwap32(l);
    NOT_IMPLEMENTED();
}

int DfCountSizeText(br_datafile* df) {
    NOT_IMPLEMENTED();
}

int DfCountSizeBinary(br_datafile* df) {
    return 4;
    NOT_IMPLEMENTED();
}

br_uint_8* BlockWriteSetup(void* base, int block_size, int block_stride, int block_count, int size) {
    int b;
    br_uint_8* block;
    br_uint_8* sp;
    br_uint_8* dp;
    NOT_IMPLEMENTED();
}

int DfBlockWriteText(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size) {
    int i;
    br_uint_8* cp;
    br_uint_8* block;
    int count;
    NOT_IMPLEMENTED();
}

void* DfBlockReadText(br_datafile* df, void* base, int* count, int size, int mtype) {
    char* id;
    char* data;
    int l;
    int s;
    int a;
    char b[3];
    NOT_IMPLEMENTED();
}

int DfBlockWriteBinary(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size) {
    int count;
    br_uint_32 l;
    br_uint_32 s;
    void* block;
    NOT_IMPLEMENTED();
}

void* DfBlockReadBinary(br_datafile* df, void* base, int* count, int size, int mtype) {
    int l;
    int s;
    LOG_TRACE("(%p, %p, %p, %d, %d)", df, base, count, size, mtype);

    BrFileRead(&l, 4, 1, df->h);
    l = BrSwap32(l);
    BrFileRead(&s, 4, 1, df->h);
    s = BrSwap32(s);
    if (s != size) {
        BrFailure("block size mismatch");
    }
    if (base) {
        if (l > *count) {
            BrFailure("DfBlockReadBinary: block too long: %d", l);
        }
    } else {
        base = BrResAllocate(df->res ? df->res : fw.res, size * l, mtype);
    }
    *count = l;
    BrFileRead(base, l, size, df->h);
    BrSwapBlock(base, l, size);
    return base;
}

int DfBlockSizeText(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size) {
    NOT_IMPLEMENTED();
}

int DfBlockSizeBinary(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size) {
    NOT_IMPLEMENTED();
}

char* DfNameReadText(br_datafile* df, char* name) {
    char* id;
    char* data;
    NOT_IMPLEMENTED();
}

int DfNameWriteText(br_datafile* df, char* name) {
    NOT_IMPLEMENTED();
}

int DfNameSizeText(br_datafile* df, char* name) {
    NOT_IMPLEMENTED();
}

char* DfNameReadBinary(br_datafile* df, char* name) {
    int c;
    int i;
    LOG_TRACE("(%p, \"%s\")", df, name);

    for (i = 0; i < 255; i++) {
        c = BrFileGetChar(df->h);
        if (!c) {
            break;
        }
        if (c == -1) {
            break;
        }
        name[i] = c;
    }
    name[i] = '\0';
    return name;
}

int DfNameWriteBinary(br_datafile* df, char* name) {
    NOT_IMPLEMENTED();
}

int DfNameSizeBinary(br_datafile* df, char* name) {
    NOT_IMPLEMENTED();
}

int DfSkipText(br_datafile* df, br_uint_32 length) {
    char* id;
    char* data;
    NOT_IMPLEMENTED();
}

int DfSkipBinary(br_datafile* df, br_uint_32 length) {
    LOG_TRACE9("(%p, %d)", df, length);
    BrFileAdvance(length, df->h);
}

int DfChunksInterpret(br_datafile* df, br_chunks_table* table) {
    br_uint_32 length;
    br_uint_32 count;
    br_uint_32 id;
    int r;
    int i;
    LOG_TRACE9("(%p, %p)", df, table);

    while (1) {
        id = df->prims->chunk_read(df, &length);
        LOG_DEBUG("chunk id=%d, len=%d", id, length);
        if (id == -1) {
            break;
        }
        for (i = 0; i < table->nentries; i++) {
            if (table->entries[i].id == id) {
                break;
            }
        }
        if (i >= table->nentries) {
            df->prims->skip(df, length);
        } else {
            if (table->entries[i].has_count) {
                count = df->prims->count_read(df);
            } else {
                count = 0;
            }
            r = table->entries[i].handler(df, id, length, count);
            if (r) {
                return r;
            }
        }
    }
    return 0;
}

void BrNullOther() {
    NOT_IMPLEMENTED();
}

int DfFileIdentify(br_uint_8* magics, br_size_t n_magics) {
    static char text_magics[8] = { '*', 'F', 'I', 'L', 'E', '_', 'I', 'N' };
    static char binary_magics[8] = { '\0', '\0', '\0', '\x12', '\0', '\0', '\0', '\b' };

    if (BrMemCmp(magics, text_magics, sizeof(magics)) == 0) {
        return 1;
    }
    if (BrMemCmp(magics, binary_magics, sizeof(magics)) == 0) {
        return 0;
    }
    LOG_WARN("file does not match magics");
    return 2;
}

br_datafile* DfOpen(char* name, int write, br_token scalar_type) {
    int mode;
    char c;
    br_datafile* df;
    void* h;
    LOG_TRACE9("(\"%s\", %d, %d)", name, write, scalar_type);

    if (write) {
        h = BrFileOpenWrite(name, fw.open_mode);
    } else {
        h = BrFileOpenRead(name, 8u, DfFileIdentify, &mode);
    }
    if (!h) {
        LOG_WARN("returning 0");
        return 0;
    }
    df = BrResAllocate(fw.res, sizeof(br_datafile), BR_MEMORY_DATAFILE);
    df->prims = &_BrFilePrimsNull;
    df->h = h;
    df->scalar_type = scalar_type;

    if (mode == 0) {
        if (write) {
            df->prims = &_BrFilePrimsWriteBinary;
        } else {
            df->prims = &_BrFilePrimsReadBinary;
        }
    } else {
        if (write)
            df->prims = &_BrFilePrimsWriteText;
        else
            df->prims = &_BrFilePrimsReadText;
    }
    DfPush(BR_MEMORY_FILE, df, 1);
    return df;
}

void DfClose(br_datafile* df) {
    br_datafile* dfp;
    LOG_TRACE("(%p)", df);

    // TODO: this is not quite right according to IDA

    while (1) {
        if (DatafileStackTop > 0) {
            if (DatafileStack[DatafileStackTop - 1].type == BR_MEMORY_FILE) {
                break;
            }
            DfPop(DatafileStack[DatafileStackTop - 1].type, NULL);
        }
    }
    dfp = DfPop(BR_MEMORY_FILE, NULL);
    LOG_DEBUG("popped h=%p, raw_file=%p", dfp->h, ((br_file*)dfp->h)->raw_file);
    BrFileClose(dfp->h);
    BrResFree(dfp);
}

int BrWriteModeSet(int mode) {
    int old;
    NOT_IMPLEMENTED();
}
