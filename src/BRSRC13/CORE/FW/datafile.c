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

// IDA: void __usercall DfPush(int type@<EAX>, void *value@<EDX>, int count@<EBX>)
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

// IDA: void* __usercall DfPop@<EAX>(int type@<EAX>, int *countp@<EDX>)
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

// IDA: void* __usercall DfTop@<EAX>(int type@<EAX>, int *countp@<EDX>)
void* DfTop(int type, int* countp) {
    LOG_TRACE9("(%d, %p)", type, countp);
    if (DatafileStackTop <= 0)
        BrFailure("DatafileStack Underflow");
    if (type != DatafileStack[DatafileStackTop - 1].type)
        BrFailure("DatafileStack type mismatch, wanted %d, got %d", type, DatafileStack[DatafileStackTop].type);
    if (countp)
        *countp = DatafileStack[DatafileStackTop - 1].count;
    return DatafileStack[DatafileStackTop - 1].value;
}

// IDA: int __cdecl DfTopType()
int DfTopType() {
    LOG_TRACE9("()");
    if (DatafileStackTop > 0) {
        return DatafileStack[DatafileStackTop - 1].type;
    } else {
        return 0;
    }
}

// IDA: int __usercall TextReadLine@<EAX>(br_datafile *df@<EAX>, char **ident@<EDX>, char **data@<EBX>)
int TextReadLine(br_datafile* df, char** ident, char** data) {
    char* cp;
    LOG_TRACE("(%p, %p, %p)", df, ident, data);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_16 __usercall scalarTypeConvert@<AX>(br_datafile *df@<EAX>, br_uint_16 t@<EDX>)
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

// IDA: br_uint_32 __usercall DfStructWriteBinary@<EAX>(br_datafile *df@<EAX>, br_file_struct *str@<EDX>, void *base@<EBX>)
br_uint_32 DfStructWriteBinary(br_datafile* df, br_file_struct* str, void* base) {
    unsigned int m;
    int i;
    int n;
    unsigned char* mp;
    br_file_struct_member* sm;
    struct { // size: 0x8
        unsigned char b[8]; // @0x0
        float f; // @0x0
    } conv;
    LOG_TRACE("(%p, %p, %p)", df, str, base);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __usercall DfStructReadBinary@<EAX>(br_datafile *df@<EAX>, br_file_struct *str@<EDX>, void *base@<EBX>)
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
            ((br_fvector4_f*)mp)->v[i] = conv.f;
        }

    LABEL_33:
        i = 0;
    }

    return 1;
}

// IDA: int __usercall DfStructSizeBinary@<EAX>(br_datafile *df@<EAX>, br_file_struct *str@<EDX>, void *base@<EBX>)
int DfStructSizeBinary(br_datafile* df, br_file_struct* str, void* base) {
    unsigned char* mp;
    unsigned int m;
    br_file_struct_member* sm;
    int bytes;
    LOG_TRACE("(%p, %p, %p)", df, str, base);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall EnumFromString@<EAX>(br_file_enum *e@<EAX>, char *str@<EDX>)
int EnumFromString(br_file_enum* e, char* str) {
    unsigned int m;
    LOG_TRACE("(%p, \"%s\")", e, str);
    NOT_IMPLEMENTED();
}

// IDA: char* __usercall EnumToString@<EAX>(br_file_enum *e@<EAX>, int num@<EDX>)
char* EnumToString(br_file_enum* e, int num) {
    unsigned int m;
    LOG_TRACE("(%p, %d)", e, num);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __usercall DfStructWriteText@<EAX>(br_datafile *df@<EAX>, br_file_struct *str@<EDX>, void *base@<EBX>)
br_uint_32 DfStructWriteText(br_datafile* df, br_file_struct* str, void* base) {
    LOG_TRACE("(%p, %p, %p)", df, str, base);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __usercall StructWriteTextSub@<EAX>(br_datafile *df@<EAX>, br_file_struct *str@<EDX>, void *base@<EBX>, int indent@<ECX>)
br_uint_32 StructWriteTextSub(br_datafile* df, br_file_struct* str, void* base, int indent) {
    unsigned int m;
    int i;
    int w;
    int add_comment;
    void* mp;
    br_file_struct_member* sm;
    LOG_TRACE("(%p, %p, %p, %d)", df, str, base, indent);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __usercall DfStructReadText@<EAX>(br_datafile *df@<EAX>, br_file_struct *str@<EDX>, void *base@<EBX>)
br_uint_32 DfStructReadText(br_datafile* df, br_file_struct* str, void* base) {
    char* id;
    char* data;
    LOG_TRACE("(%p, %p, %p)", df, str, base);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __usercall StructReadTextSub@<EAX>(br_datafile *df@<EAX>, br_file_struct *str@<EDX>, void *base@<EBX>)
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
    LOG_TRACE("(%p, %p, %p)", df, str, base);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DfStructSizeText@<EAX>(br_datafile *df@<EAX>, br_file_struct *str@<EDX>, void *base@<EBX>)
int DfStructSizeText(br_datafile* df, br_file_struct* str, void* base) {
    unsigned int m;
    br_file_struct_member* sm;
    int lines;
    LOG_TRACE("(%p, %p, %p)", df, str, base);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __usercall DfStructWriteArray@<EAX>(br_datafile *df@<EAX>, br_file_struct *str@<EDX>, void *base@<EBX>, int n@<ECX>)
br_uint_32 DfStructWriteArray(br_datafile* df, br_file_struct* str, void* base, int n) {
    char* cp;
    int i;
    LOG_TRACE("(%p, %p, %p, %d)", df, str, base, n);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __usercall DfStructReadArray@<EAX>(br_datafile *df@<EAX>, br_file_struct *str@<EDX>, void *base@<EBX>, int n@<ECX>)
br_uint_32 DfStructReadArray(br_datafile* df, br_file_struct* str, void* base, int n) {
    char* cp;
    int i;
    LOG_TRACE9("(%p, %p, %p, %d)", df, str, base, n);

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

// IDA: int __usercall DfChunkWriteText@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>)
int DfChunkWriteText(br_datafile* df, br_uint_32 id, br_uint_32 length) {
    LOG_TRACE("(%p, %d, %d)", df, id, length);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DfChunkReadText@<EAX>(br_datafile *df@<EAX>, br_uint_32 *plength@<EDX>)
int DfChunkReadText(br_datafile* df, br_uint_32* plength) {
    int i;
    char* id;
    char* data;
    LOG_TRACE("(%p, %p)", df, plength);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DfChunkWriteBinary@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>)
int DfChunkWriteBinary(br_datafile* df, br_uint_32 id, br_uint_32 length) {
    br_uint_32 l;
    LOG_TRACE("(%p, %d, %d)", df, id, length);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DfChunkReadBinary@<EAX>(br_datafile *df@<EAX>, br_uint_32 *plength@<EDX>)
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

// IDA: void __usercall DfCountWriteText(br_datafile *df@<EAX>, br_uint_32 count@<EDX>)
void DfCountWriteText(br_datafile* df, br_uint_32 count) {
    LOG_TRACE("(%p, %d)", df, count);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __usercall DfCountReadText@<EAX>(br_datafile *df@<EAX>)
br_uint_32 DfCountReadText(br_datafile* df) {
    char* id;
    char* data;
    LOG_TRACE("(%p)", df);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DfCountWriteBinary(br_datafile *df@<EAX>, br_uint_32 count@<EDX>)
void DfCountWriteBinary(br_datafile* df, br_uint_32 count) {
    br_uint_32 l;
    LOG_TRACE("(%p, %d)", df, count);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_32 __usercall DfCountReadBinary@<EAX>(br_datafile *df@<EAX>)
br_uint_32 DfCountReadBinary(br_datafile* df) {
    br_uint_32 l;
    LOG_TRACE9("(%p)", df);
    BrFileRead(&l, 4, 1, df->h);
    return BrSwap32(l);
}

// IDA: int __usercall DfCountSizeText@<EAX>(br_datafile *df@<EAX>)
int DfCountSizeText(br_datafile* df) {
    LOG_TRACE("(%p)", df);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DfCountSizeBinary@<EAX>(br_datafile *df@<EAX>)
int DfCountSizeBinary(br_datafile* df) {
    LOG_TRACE("(%p)", df);
    NOT_IMPLEMENTED();
}

// IDA: br_uint_8* __usercall BlockWriteSetup@<EAX>(void *base@<EAX>, int block_size@<EDX>, int block_stride@<EBX>, int block_count@<ECX>, int size)
br_uint_8* BlockWriteSetup(void* base, int block_size, int block_stride, int block_count, int size) {
    int b;
    br_uint_8* block;
    br_uint_8* sp;
    br_uint_8* dp;
    LOG_TRACE("(%p, %d, %d, %d, %d)", base, block_size, block_stride, block_count, size);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DfBlockWriteText@<EAX>(br_datafile *df@<EAX>, void *base@<EDX>, int block_size@<EBX>, int block_stride@<ECX>, int block_count, int size)
int DfBlockWriteText(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size) {
    int i;
    br_uint_8* cp;
    br_uint_8* block;
    int count;
    LOG_TRACE("(%p, %p, %d, %d, %d, %d)", df, base, block_size, block_stride, block_count, size);
    NOT_IMPLEMENTED();
}

// IDA: void* __usercall DfBlockReadText@<EAX>(br_datafile *df@<EAX>, void *base@<EDX>, int *count@<EBX>, int size@<ECX>, int mtype)
void* DfBlockReadText(br_datafile* df, void* base, int* count, int size, int mtype) {
    char* id;
    char* data;
    int l;
    int s;
    int a;
    char b[3];
    LOG_TRACE("(%p, %p, %p, %d, %d)", df, base, count, size, mtype);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DfBlockWriteBinary@<EAX>(br_datafile *df@<EAX>, void *base@<EDX>, int block_size@<EBX>, int block_stride@<ECX>, int block_count, int size)
int DfBlockWriteBinary(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size) {
    int count;
    br_uint_32 l;
    br_uint_32 s;
    void* block;
    LOG_TRACE("(%p, %p, %d, %d, %d, %d)", df, base, block_size, block_stride, block_count, size);
    NOT_IMPLEMENTED();
}

// IDA: void* __usercall DfBlockReadBinary@<EAX>(br_datafile *df@<EAX>, void *base@<EDX>, int *count@<EBX>, int size@<ECX>, int mtype)
void* DfBlockReadBinary(br_datafile* df, void* base, int* count, int size, int mtype) {
    int l;
    int s;
    LOG_TRACE9("(%p, %p, %p, %d, %d)", df, base, count, size, mtype);

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

// IDA: int __usercall DfBlockSizeText@<EAX>(br_datafile *df@<EAX>, void *base@<EDX>, int block_size@<EBX>, int block_stride@<ECX>, int block_count, int size)
int DfBlockSizeText(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size) {
    LOG_TRACE("(%p, %p, %d, %d, %d, %d)", df, base, block_size, block_stride, block_count, size);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DfBlockSizeBinary@<EAX>(br_datafile *df@<EAX>, void *base@<EDX>, int block_size@<EBX>, int block_stride@<ECX>, int block_count, int size)
int DfBlockSizeBinary(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size) {
    LOG_TRACE("(%p, %p, %d, %d, %d, %d)", df, base, block_size, block_stride, block_count, size);
    NOT_IMPLEMENTED();
}

// IDA: char* __usercall DfNameReadText@<EAX>(br_datafile *df@<EAX>, char *name@<EDX>)
char* DfNameReadText(br_datafile* df, char* name) {
    char* id;
    char* data;
    LOG_TRACE("(%p, \"%s\")", df, name);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DfNameWriteText@<EAX>(br_datafile *df@<EAX>, char *name@<EDX>)
int DfNameWriteText(br_datafile* df, char* name) {
    LOG_TRACE("(%p, \"%s\")", df, name);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DfNameSizeText@<EAX>(br_datafile *df@<EAX>, char *name@<EDX>)
int DfNameSizeText(br_datafile* df, char* name) {
    LOG_TRACE("(%p, \"%s\")", df, name);
    NOT_IMPLEMENTED();
}

// IDA: char* __usercall DfNameReadBinary@<EAX>(br_datafile *df@<EAX>, char *name@<EDX>)
char* DfNameReadBinary(br_datafile* df, char* name) {
    int c;
    int i;
    LOG_TRACE9("(%p, \"%s\")", df, name);

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

// IDA: int __usercall DfNameWriteBinary@<EAX>(br_datafile *df@<EAX>, char *name@<EDX>)
int DfNameWriteBinary(br_datafile* df, char* name) {
    LOG_TRACE("(%p, \"%s\")", df, name);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DfNameSizeBinary@<EAX>(br_datafile *df@<EAX>, char *name@<EDX>)
int DfNameSizeBinary(br_datafile* df, char* name) {
    LOG_TRACE("(%p, \"%s\")", df, name);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DfSkipText@<EAX>(br_datafile *df@<EAX>, br_uint_32 length@<EDX>)
int DfSkipText(br_datafile* df, br_uint_32 length) {
    char* id;
    char* data;
    LOG_TRACE("(%p, %d)", df, length);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall DfSkipBinary@<EAX>(br_datafile *df@<EAX>, br_uint_32 length@<EDX>)
int DfSkipBinary(br_datafile* df, br_uint_32 length) {
    LOG_TRACE9("(%p, %d)", df, length);
    BrFileAdvance(length, df->h);
    return 0;
}

// IDA: int __usercall DfChunksInterpret@<EAX>(br_datafile *df@<EAX>, br_chunks_table *table@<EDX>)
int DfChunksInterpret(br_datafile* df, br_chunks_table* table) {
    br_uint_32 length;
    br_uint_32 count;
    br_uint_32 id;
    int r;
    int i;
    LOG_TRACE9("(%p, %p)", df, table);

    while (1) {
        id = df->prims->chunk_read(df, &length);
        //LOG_DEBUG("chunk id=%d, len=%d", id, length);
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

// IDA: void __cdecl BrNullOther()
void BrNullOther() {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: int __cdecl DfFileIdentify(br_uint_8 *magics, br_size_t n_magics)
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

// IDA: br_datafile* __usercall DfOpen@<EAX>(char *name@<EAX>, int write@<EDX>, br_token scalar_type@<EBX>)
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

// IDA: void __usercall DfClose(br_datafile *df@<EAX>)
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
    BrFileClose(dfp->h);
    BrResFree(dfp);
}

// IDA: int __cdecl BrWriteModeSet(int mode)
int BrWriteModeSet(int mode) {
    int old;
    LOG_TRACE("(%d)", mode);
    NOT_IMPLEMENTED();
}
