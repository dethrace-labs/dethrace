#include "datafile.h"
#include "CORE/FW/bswap.h"
#include "CORE/FW/diag.h"
#include "CORE/FW/file.h"
#include "CORE/FW/fwsetup.h"
#include "CORE/FW/resource.h"
#include "CORE/FW/scratch.h"
#include "CORE/MATH/fixed.h"
#include "CORE/STD/brstdlib.h"
#include "harness/trace.h"

#if BR_ENDIAN_BIG

#define BR_HTON_32(b) (b)
#define BR_HTON_16(b) (b)
#define BR_HTON_F(b) (b)
#define BR_HTON_D(b) (b)

#define BR_NTOH_32(b) (b)
#define BR_NTOH_16(b) (b)
#define BR_NTOH_F(b) (b)
#define BR_NTOH_D(b) (b)

#else

#define BR_HTON_32(b) (3-(b))
#define BR_HTON_16(b) (1-(b))
#define BR_HTON_F(b) (3-(b))
#define BR_HTON_D(b) (7-(b))

#define BR_NTOH_32(b) (3-(b))
#define BR_NTOH_16(b) (1-(b))
#define BR_NTOH_F(b) (3-(b))
#define BR_NTOH_D(b) (7-(b))

#endif

br_file_primitives _BrFilePrimsNull = {
    "NULL",
    (int (*)(br_datafile*, br_uint_32)) & BrNullOther,
    (int (*)(br_datafile*, br_uint_32, br_uint_32)) & BrNullOther,
    (int (*)(br_datafile*, br_uint_32*)) & BrNullOther,
    (void (*)(br_datafile*, br_uint_32)) & BrNullOther,
    (br_uint_32(*)(br_datafile*)) & BrNullOther,
    (int (*)(br_datafile*)) & BrNullOther,
    (br_uint_32(*)(br_datafile*, br_file_struct*, void*)) & BrNullOther,
    (br_uint_32(*)(br_datafile*, br_file_struct*, void*)) & BrNullOther,
    (int (*)(br_datafile*, br_file_struct*, void*)) & BrNullOther,
    (int (*)(br_datafile*, void*, int, int, int, int)) & BrNullOther,
    (void* (*)(br_datafile*, void*, int*, int, int)) & BrNullOther,
    (int (*)(br_datafile*, void*, int, int, int, int)) & BrNullOther,
    (int (*)(br_datafile*, char*)) & BrNullOther,
    (char* (*)(br_datafile*, char*)) & BrNullOther,
    (int (*)(br_datafile*, char*)) & BrNullOther,
};

br_file_primitives _BrFilePrimsReadBinary = {
    "Read Binary",
    &DfSkipBinary,
    (int (*)(br_datafile*, br_uint_32, br_uint_32)) & BrNullOther,
    &DfChunkReadBinary,
    (void (*)(br_datafile*, br_uint_32)) & BrNullOther,
    &DfCountReadBinary,
    &DfCountSizeBinary,
    (br_uint_32(*)(br_datafile*, br_file_struct*, void*)) & BrNullOther,
    &DfStructReadBinary,
    &DfStructSizeBinary,
    (int (*)(br_datafile*, void*, int, int, int, int)) & BrNullOther,
    &DfBlockReadBinary,
    &DfBlockSizeBinary,
    (int (*)(br_datafile*, char*)) & BrNullOther,
    &DfNameReadBinary,
    &DfNameSizeBinary,
};

br_file_primitives _BrFilePrimsWriteBinary = {
    "Write Binary",
    &DfSkipBinary,
    &DfChunkWriteBinary,
    (int (*)(br_datafile*, br_uint_32*)) & BrNullOther,
    &DfCountWriteBinary,
    (br_uint_32(*)(br_datafile*)) & BrNullOther,
    &DfCountSizeBinary,
    &DfStructWriteBinary,
    (br_uint_32(*)(br_datafile*, br_file_struct*, void*)) & BrNullOther,
    &DfStructSizeBinary,
    &DfBlockWriteBinary,
    (void* (*)(br_datafile*, void*, int*, int, int)) & BrNullOther,
    &DfBlockSizeBinary,
    &DfNameWriteBinary,
    (char* (*)(br_datafile*, char*)) & BrNullOther,
    &DfNameSizeBinary,
};

br_file_primitives _BrFilePrimsReadText = {
    "Read Text",
    &DfSkipText,
    (int (*)(br_datafile*, br_uint_32, br_uint_32)) & BrNullOther,
    &DfChunkReadText,
    (void (*)(br_datafile*, br_uint_32)) & BrNullOther,
    &DfCountReadText,
    &DfCountSizeText,
    (br_uint_32(*)(br_datafile*, br_file_struct*, void*)) & BrNullOther,
    &DfStructReadText,
    &DfStructSizeText,
    (int (*)(br_datafile*, void*, int, int, int, int)) & BrNullOther,
    &DfBlockReadText,
    &DfBlockSizeText,
    (int (*)(br_datafile*, char*)) & BrNullOther,
    &DfNameReadText,
    &DfNameSizeText,
};

br_file_primitives _BrFilePrimsWriteText = {
    "Write Text",
    &DfSkipText,
    &DfChunkWriteText,
    (int (*)(br_datafile*, br_uint_32*)) & BrNullOther,
    &DfCountWriteText,
    (br_uint_32(*)(br_datafile*)) & BrNullOther,
    &DfCountSizeText,
    &DfStructWriteText,
    (br_uint_32(*)(br_datafile*, br_file_struct*, void*)) & BrNullOther,
    &DfStructSizeText,
    &DfBlockWriteText,
    (void* (*)(br_datafile*, void*, int*, int, int)) & BrNullOther,
    &DfBlockSizeText,
    &DfNameWriteText,
    (char* (*)(br_datafile*, char*)) & BrNullOther,
    &DfNameSizeText,
};

char* member_type_names[32] = {
    "int_8",
    "uint_8",
    "int_16",
    "uint_16",
    "int_32",
    "uint_32",
    "fixed",
    "angle",
    "float",
    "double",
    "scalar",
    "fraction",
    "ufraction",
    "enum_8",
    "enum_16",
    "enum_32",
    "struct",
    "asciz",
    "colour",
    "vector2",
    "vector3",
    "vector4",
    "fixed_fraction",
    "fixed_ufraction",
    "float_fraction",
    "float_ufraction",
    "fixed_vector2",
    "fixed_vector3",
    "fixed_vector4",
    "float_vector2",
    "float_vector3",
    "float_vector4",
};

struct {
    int type;
    void* value;
    int count;
} DatafileStack[1024];

#ifdef BRENDER_FIX_BUGS
char* ChunkNames[62] = {
#else
char* ChunkNames[61] = {
#endif
    "END",
    "IMAGE_PLANE",
    "RLE_IMAGE_PLANE",
#ifdef BRENDER_FIX_BUGS
    "PIXELMAP_OLD",
#else
    "PIXELMAP",
#endif
    "MATERIAL_OLD",
    "ADD_MATERIAL",
    "OLD_ACTOR",
    "OLD_ADD_SIBLING",
    "OLD_ADD_CHILD",
    "OLD_MATERIAL_INDEX",
    "OLD_VERTICES",
    "OLD_VERTICES_UV",
    "OLD_FACES",
    "OLD_MODEL",
    "ADD_MODEL",
    "ANIM",
    "ANIM_TRANSFORM",
    "ANIM_RATE",
    "FILE_INFO",
    "OLD_LIGHT",
    "OLD_CAMERA",
    "PIVOT",
    "MATERIAL_INDEX",
    "VERTICES",
    "VERTEX_UV",
    "OLD_FACES_1",
    "FACE_MATERIAL",
    "OLD_MODEL_1",
    "COLOUR_MAP_REF",
    "OPACITY_MAP_REF",
    "INDEX_BLEND_REF",
    "INDEX_SHADE_REF",
    "SCREENDOOR_REF",
    "PIXELS",
    "ADD_MAP",
    "ACTOR",
    "ACTOR_MODEL",
    "ACTOR_TRANSFORM",
    "ACTOR_MATERIAL",
    "ACTOR_LIGHT",
    "ACTOR_CAMERA",
    "ACTOR_BOUNDS",
    "ACTOR_ADD_CHILD",
    "TRANSFORM_MATRIX34",
    "TRANSFORM_MATRIX34_LP",
    "TRANSFORM_QUAT",
    "TRANSFORM_EULER",
    "TRANSFORM_LOOK_UP",
    "TRANSFORM_TRANSLATION",
    "TRANSFORM_IDENTITY",
    "BOUNDS",
    "LIGHT",
    "CAMERA",
    "FACES",
    "MODEL",
    "ACTOR_CLIP_PLANE",
    "PLANE",
    "SATURN_FACES",
    "SATURN_MODEL",
    "INDEX_FOG_REF",
#ifdef BRENDER_FIX_BUGS
    "MATERIAL",
    "PIXELMAP",
#else
    "MATERIAL_OLD",
#endif
};

int DatafileStackTop;

// IDA: void __usercall DfPush(int type@<EAX>, void *value@<EDX>, int count@<EBX>)
void DfPush(int type, void* value, int count) {
    LOG_TRACE9("(%d, %p, %d)", type, value, count);

    if (DatafileStackTop >= BR_ASIZE(DatafileStack)) {
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
    if (countp != NULL) {
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
    if (countp != NULL)
        *countp = DatafileStack[DatafileStackTop - 1].count;
    return DatafileStack[DatafileStackTop - 1].value;
}

// IDA: int __cdecl DfTopType()
int DfTopType(void) {
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
    LOG_TRACE9("(%p, %p, %p)", df, ident, data);

    while (1) {
        int a;
        cp = BrScratchString();
        BrFileGetLine(cp, 256, df->h);
        if (BrFileEof(df->h) != 0) {
            return 0;
        }
        for (; (*cp == ' ') || (*cp == '\t'); cp++) {
        }
        if (*cp != '\0') {
            break;
        }
    }
    *ident = cp;
    while ((*cp != ' ') && (*cp != '\t') && (*cp != '\0')) {
        cp++;
    }
    *cp = '\0';
    cp++;
    while ((*cp == ' ') || (*cp == '\t')) {
        cp++;
    }
    *data = cp;
    if (*cp == '\"') {
        cp++;
        while ((*cp != '\0') && (*cp != '\"')) {
            cp++;
        }
    } else {
        while ((*cp != ' ') && (*cp != '\t') && (*cp != '\0')) {
            cp++;
        }
    }
    *cp = '\0';
    return 1;
}

// IDA: br_uint_16 __usercall scalarTypeConvert@<AX>(br_datafile *df@<EAX>, br_uint_16 t@<EDX>)
br_uint_16 scalarTypeConvert(br_datafile* df, br_uint_16 t) {
    LOG_TRACE9("(%p, %d)", df, t);

    if (df->scalar_type == BRT_FIXED) {
        switch (t) {
        case DF_TYPE_BR_SCALAR:
            return DF_TYPE_BR_FIXED;
        case DF_TYPE_BR_FRACTION:
            return DF_TYPE_BR_FRACTION_X;
        case DF_TYPE_BR_UFRACTION:
            return DF_TYPE_BR_UFRACTION_X;
        case DF_TYPE_BR_VECTOR2:
            return DF_TYPE_BR_VECTOR2_X;
        case DF_TYPE_BR_VECTOR3:
            return DF_TYPE_BR_VECTOR3_X;
        case DF_TYPE_BR_VECTOR4:
            return DF_TYPE_BR_VECTOR4_X;
        }
    } else if (df->scalar_type == BRT_FLOAT) {
        switch (t) {
        case DF_TYPE_BR_SCALAR:
            return DF_TYPE_FLOAT;
        case DF_TYPE_BR_UFRACTION:
            return DF_TYPE_BR_FRACTION_F;
        case DF_TYPE_BR_FRACTION:
            return DF_TYPE_BR_UFRACTION_F;
        case DF_TYPE_BR_VECTOR2:
            return DF_TYPE_BR_VECTOR2_F;
        case DF_TYPE_BR_VECTOR3:
            return DF_TYPE_BR_VECTOR3_F;
        case DF_TYPE_BR_VECTOR4:
            return DF_TYPE_BR_VECTOR4_F;
        }
    } else if ((t == DF_TYPE_BR_SCALAR) || ((DF_TYPE_BR_VECTOR2 <= t) && (t <= DF_TYPE_BR_VECTOR4))) {
        BrFailure("Incorrect scalar type");
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
    union { // size: 0x8
        unsigned char b[8]; // @0x0
        float f; // @0x0
        double d; // @0x0
    } conv;
    LOG_TRACE("(%p, %p, %p)", df, str, base);

    for (m = 0; m < str->nmembers; m++) {
        sm = &str->members[m];
        mp = ((unsigned char*)base) + sm->offset;

        switch (scalarTypeConvert(df, sm->type)) {
        case DF_TYPE_BR_INT_8:
        case DF_TYPE_BR_UINT_8:
        case DF_TYPE_ENUM_8:
            BrFilePutChar(((br_uint_8*)mp)[0], df->h);
            break;
        case DF_TYPE_BR_ANGLE:
        case DF_TYPE_BR_INT_16:
        case DF_TYPE_BR_UINT_16:
        case DF_TYPE_ENUM_16:
            BrFilePutChar(mp[BR_HTON_16(0)], df->h);
            BrFilePutChar(mp[BR_HTON_16(1)], df->h);
            break;
        case DF_TYPE_BR_INT_32:
        case DF_TYPE_BR_UINT_32:
        case DF_TYPE_ENUM_32:
            BrFilePutChar(mp[BR_HTON_32(0)], df->h);
            BrFilePutChar(mp[BR_HTON_32(1)], df->h);
            BrFilePutChar(mp[BR_HTON_32(2)], df->h);
            BrFilePutChar(mp[BR_HTON_32(3)], df->h);
            break;
        case DF_TYPE_FLOAT:
        case DF_TYPE_BR_FRACTION_F:
        case DF_TYPE_BR_UFRACTION_F:
            conv.f = *(float*)mp;
            BrFilePutChar(conv.b[BR_HTON_32(0)], df->h);
            BrFilePutChar(conv.b[BR_HTON_32(1)], df->h);
            BrFilePutChar(conv.b[BR_HTON_32(2)], df->h);
            BrFilePutChar(conv.b[BR_HTON_32(3)], df->h);
            break;
        case DF_TYPE_DOUBLE:
            conv.d = *(double*)mp;
            BrFilePutChar(conv.b[BR_HTON_D(0)], df->h);
            BrFilePutChar(conv.b[BR_HTON_D(1)], df->h);
            BrFilePutChar(conv.b[BR_HTON_D(2)], df->h);
            BrFilePutChar(conv.b[BR_HTON_D(3)], df->h);
            BrFilePutChar(conv.b[BR_HTON_D(4)], df->h);
            BrFilePutChar(conv.b[BR_HTON_D(5)], df->h);
            BrFilePutChar(conv.b[BR_HTON_D(6)], df->h);
            BrFilePutChar(conv.b[BR_HTON_D(7)], df->h);
            break;
        case DF_TYPE_STRUCT:
            DfStructWriteBinary(df, sm->extra, mp);
            break;
        case DF_TYPE_ASCIZ:
            if (*(char**)mp != NULL) {
                BrFileWrite(*(char**)mp, 1, BrStrLen(*(char**)mp), df->h);
            }
            BrFilePutChar('\0', df->h);
            break;
        case DF_TYPE_BR_COLOUR:
            BrFilePutChar(mp[BR_HTON_32(1)], df->h);
            BrFilePutChar(mp[BR_HTON_32(2)], df->h);
            BrFilePutChar(mp[BR_HTON_32(3)], df->h);
            break;
        case DF_TYPE_BR_FIXED:
            conv.f = BrFixedToFloat(*(br_fixed_ls*)mp);
            BrFilePutChar(conv.b[BR_HTON_32(0)], df->h);
            BrFilePutChar(conv.b[BR_HTON_32(1)], df->h);
            BrFilePutChar(conv.b[BR_HTON_32(2)], df->h);
            BrFilePutChar(conv.b[BR_HTON_32(3)], df->h);
            break;
        case DF_TYPE_BR_FRACTION_X:
            conv.f = BrFixedFractionToFloat(*(br_fraction_x*)mp);
            BrFilePutChar(conv.b[BR_HTON_32(0)], df->h);
            BrFilePutChar(conv.b[BR_HTON_32(1)], df->h);
            BrFilePutChar(conv.b[BR_HTON_32(2)], df->h);
            BrFilePutChar(conv.b[BR_HTON_32(3)], df->h);
            break;
        case DF_TYPE_BR_UFRACTION_X:
            conv.f = BrFixedUFractionToFloat(*(br_fraction_x*)mp);
            BrFilePutChar(conv.b[BR_HTON_32(0)], df->h);
            BrFilePutChar(conv.b[BR_HTON_32(1)], df->h);
            BrFilePutChar(conv.b[BR_HTON_32(2)], df->h);
            BrFilePutChar(conv.b[BR_HTON_32(3)], df->h);
            break;
        case DF_TYPE_BR_VECTOR2_X:
            n = 2;
            goto copy_fixed_vector;
        case DF_TYPE_BR_VECTOR3_X:
            n = 3;
            goto copy_fixed_vector;
        case DF_TYPE_BR_VECTOR4_X:
            n = 4;
        copy_fixed_vector:
            for (i = 0; i < n; i++) {
                conv.f = BrFixedToFloat(*(br_fixed_ls*)mp);
                BrFilePutChar(conv.b[BR_HTON_32(0)], df->h);
                BrFilePutChar(conv.b[BR_HTON_32(1)], df->h);
                BrFilePutChar(conv.b[BR_HTON_32(2)], df->h);
                BrFilePutChar(conv.b[BR_HTON_32(3)], df->h);
                mp += sizeof(br_fixed_ls);
            }
            break;
        case DF_TYPE_BR_VECTOR2_F:
            n = 2;
            goto copy_float_vector;
        case DF_TYPE_BR_VECTOR3_F:
            n = 3;
            goto copy_float_vector;
        case DF_TYPE_BR_VECTOR4_F:
            n = 4;
        copy_float_vector:
            for (i = 0; i < n; i++) {
                conv.f = *(float*)mp;
                BrFilePutChar(conv.b[BR_HTON_32(0)], df->h);
                BrFilePutChar(conv.b[BR_HTON_32(1)], df->h);
                BrFilePutChar(conv.b[BR_HTON_32(2)], df->h);
                BrFilePutChar(conv.b[BR_HTON_32(3)], df->h);
                mp += sizeof(br_fixed_ls);
            }
            break;
        }
    }
    return 1;
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
        double d;
    } conv;

    LOG_TRACE9("(%p, %p, %p)", df, str, base);

    for (m = 0; m < str->nmembers; m++) {
        sm = &str->members[m];

        c = scalarTypeConvert(df, sm->type);
        mp = ((unsigned char*)base) + sm->offset;

        switch (c) {
        case DF_TYPE_BR_INT_8:
        case DF_TYPE_BR_UINT_8:
        case DF_TYPE_ENUM_8:
            mp[0] = BrFileGetChar(df->h);
            break;
        case DF_TYPE_BR_INT_16:
        case DF_TYPE_BR_UINT_16:
        case DF_TYPE_BR_ANGLE:
        case DF_TYPE_ENUM_16:
            mp[BR_NTOH_16(0)] = BrFileGetChar(df->h);
            mp[BR_NTOH_16(1)] = BrFileGetChar(df->h);
            break;
        case DF_TYPE_BR_INT_32:
        case DF_TYPE_BR_UINT_32:
        case DF_TYPE_FLOAT:
        case DF_TYPE_ENUM_32:
        case DF_TYPE_BR_FRACTION_F:
        case DF_TYPE_BR_UFRACTION_F:
            mp[BR_NTOH_32(0)] = BrFileGetChar(df->h);
            mp[BR_NTOH_32(1)] = BrFileGetChar(df->h);
            mp[BR_NTOH_32(2)] = BrFileGetChar(df->h);
            mp[BR_NTOH_32(3)] = BrFileGetChar(df->h);
            break;
        case DF_TYPE_BR_FIXED:
            conv.b[BR_NTOH_F(0)] = BrFileGetChar(df->h);
            conv.b[BR_NTOH_F(1)] = BrFileGetChar(df->h);
            conv.b[BR_NTOH_F(2)] = BrFileGetChar(df->h);
            conv.b[BR_NTOH_F(3)] = BrFileGetChar(df->h);
            *(br_fixed_ls*)mp = BrFloatToFixed(conv.f);
            break;
        case DF_TYPE_DOUBLE:
            conv.b[BR_NTOH_D(0)] = BrFileGetChar(df->h);
            conv.b[BR_NTOH_D(1)] = BrFileGetChar(df->h);
            conv.b[BR_NTOH_D(2)] = BrFileGetChar(df->h);
            conv.b[BR_NTOH_D(3)] = BrFileGetChar(df->h);
            conv.b[BR_NTOH_D(4)] = BrFileGetChar(df->h);
            conv.b[BR_NTOH_D(5)] = BrFileGetChar(df->h);
            conv.b[BR_NTOH_D(6)] = BrFileGetChar(df->h);
            conv.b[BR_NTOH_D(7)] = BrFileGetChar(df->h);
            *(double*)mp = conv.d;
            break;
        case DF_TYPE_STRUCT:
            DfStructReadBinary(df, sm->extra, mp);
            break;
        case DF_TYPE_ASCIZ:
            for (n = 0; n < 255; n++) {
                c = BrFileGetChar(df->h);
                if (c == '\0' || c == -1)
                    break;
                tmp_string[n] = c;
            }
            tmp_string[n] = 0;
            *(char**)mp = (char*)BrResStrDup(df->res ? df->res : fw.res, tmp_string);
            break;
        case DF_TYPE_BR_COLOUR:
            mp[BR_NTOH_32(1)] = BrFileGetChar(df->h);
            mp[BR_NTOH_32(2)] = BrFileGetChar(df->h);
            mp[BR_NTOH_32(3)] = BrFileGetChar(df->h);
            break;
        case DF_TYPE_BR_FRACTION_X:
            conv.b[BR_NTOH_F(0)] = BrFileGetChar(df->h);
            conv.b[BR_NTOH_F(1)] = BrFileGetChar(df->h);
            conv.b[BR_NTOH_F(2)] = BrFileGetChar(df->h);
            conv.b[BR_NTOH_F(3)] = BrFileGetChar(df->h);
            *((br_fraction_x*)mp) = BrFloatToFixedFraction(conv.f);
            break;
        case DF_TYPE_BR_UFRACTION_X:
            conv.b[BR_NTOH_F(0)] = BrFileGetChar(df->h);
            conv.b[BR_NTOH_F(1)] = BrFileGetChar(df->h);
            conv.b[BR_NTOH_F(2)] = BrFileGetChar(df->h);
            conv.b[BR_NTOH_F(3)] = BrFileGetChar(df->h);
            *(br_ufraction_x*)mp = BrFloatToFixedUFraction(conv.f);
            break;
        case DF_TYPE_BR_VECTOR2_X:
            n = 2;
            goto copy_fixed_vector;
        case DF_TYPE_BR_VECTOR3_X:
            n = 3;
            goto copy_fixed_vector;
        case DF_TYPE_BR_VECTOR4_X:
            n = 4;
        copy_fixed_vector:
            for (i = 0; i < n; i++) {
                conv.b[BR_NTOH_F(0)] = BrFileGetChar(df->h);
                conv.b[BR_NTOH_F(1)] = BrFileGetChar(df->h);
                conv.b[BR_NTOH_F(2)] = BrFileGetChar(df->h);
                conv.b[BR_NTOH_F(3)] = BrFileGetChar(df->h);
                *(br_fixed_ls*)mp = BrFloatToFixed(conv.f);
                mp += sizeof(br_fixed_ls);
            }
            break;
        case DF_TYPE_BR_VECTOR2_F:
            n = 2;
            goto copy_float_vector;
            break;
        case DF_TYPE_BR_VECTOR3_F:
            n = 3;
            goto copy_float_vector;
            break;
        case DF_TYPE_BR_VECTOR4_F:
            n = 4;
        copy_float_vector:
            for (i = 0; i < n; i++) {
                conv.b[BR_NTOH_F(0)] = BrFileGetChar(df->h);
                conv.b[BR_NTOH_F(1)] = BrFileGetChar(df->h);
                conv.b[BR_NTOH_F(2)] = BrFileGetChar(df->h);
                conv.b[BR_NTOH_F(3)] = BrFileGetChar(df->h);
                *(float*)mp = conv.f;
                mp += sizeof(float);
            }
            break;
        }
    }

    return 1;
}
#include <stdio.h>
// IDA: int __usercall DfStructSizeBinary@<EAX>(br_datafile *df@<EAX>, br_file_struct *str@<EDX>, void *base@<EBX>)
int DfStructSizeBinary(br_datafile* df, br_file_struct* str, void* base) {
    unsigned char* mp;
    unsigned int m;
    br_file_struct_member* sm;
    int bytes;
    LOG_TRACE("(%p, %p, %p)", df, str, base);

    bytes = 0;
    for (m = 0; m < str->nmembers; m++) {
        sm = &str->members[m];

        mp = ((unsigned char*)base) + sm->offset;

        switch (scalarTypeConvert(df, sm->type)) {
        case DF_TYPE_BR_INT_8:
        case DF_TYPE_BR_UINT_8:
        case DF_TYPE_ENUM_8:
            bytes += 1;
            break;
        case DF_TYPE_BR_INT_16:
        case DF_TYPE_BR_UINT_16:
        case DF_TYPE_BR_ANGLE:
        case DF_TYPE_ENUM_16:
            bytes += 2;
            break;
        case DF_TYPE_BR_COLOUR:
            bytes += 4;
            break;
        case DF_TYPE_BR_INT_32:
        case DF_TYPE_BR_UINT_32:
        case DF_TYPE_FLOAT:
        case DF_TYPE_ENUM_32:
        case DF_TYPE_BR_FRACTION_F:
        case DF_TYPE_BR_UFRACTION_F:
        case DF_TYPE_BR_FIXED:
        case DF_TYPE_BR_FRACTION_X:
        case DF_TYPE_BR_UFRACTION_X:
            bytes += 4;
            break;
        case DF_TYPE_DOUBLE:
            bytes += 8;
            break;
        case DF_TYPE_STRUCT:
            bytes += DfStructSizeBinary(df, sm->extra, mp);
            break;
        case DF_TYPE_ASCIZ:
            if (*(char**)mp != NULL) {
                bytes += BrStrLen(*(char**)mp);
            }
            break;
        case DF_TYPE_BR_VECTOR2_X:
        case DF_TYPE_BR_VECTOR2_F:
            bytes += 8;
            break;
        case DF_TYPE_BR_VECTOR3_X:
        case DF_TYPE_BR_VECTOR3_F:
            bytes += 12;
            break;
        case DF_TYPE_BR_VECTOR4_X:
        case DF_TYPE_BR_VECTOR4_F:
            bytes += 16;
            break;
        }
    }

    return bytes;
}

// IDA: int __usercall EnumFromString@<EAX>(br_file_enum *e@<EAX>, char *str@<EDX>)
int EnumFromString(br_file_enum* e, char* str) {
    unsigned int m;
    LOG_TRACE9("(%p, \"%s\")", e, str);

    if (e == NULL) {
        BrFailure("Unknown enum string: %s", str);
    }
    for (m = 0; m < e->nmembers; m++) {
        if (BrStrCmp(str, e->members[m].name) == 0) {
            return e->members[m].value;
        }
    }
    BrFailure("Unknown enum string: %s", str);
}

// IDA: char* __usercall EnumToString@<EAX>(br_file_enum *e@<EAX>, int num@<EDX>)
char* EnumToString(br_file_enum* e, int num) {
    unsigned int m;
    LOG_TRACE9("(%p, %d)", e, num);

    if (e == NULL) {
        goto fail;
    }
    for (m = 0; m < e->nmembers; m++) {
        if (e->members[m].value == num) {
            return e->members[m].name;
        }
    }
fail:
    BrFailure("Unknown enum %d", num);
}

// IDA: br_uint_32 __usercall DfStructWriteText@<EAX>(br_datafile *df@<EAX>, br_file_struct *str@<EDX>, void *base@<EBX>)
br_uint_32 DfStructWriteText(br_datafile* df, br_file_struct* str, void* base) {
    LOG_TRACE("(%p, %p, %p)", df, str, base);

    BrFilePrintf(df->h, "  struct    %s\n", str->name);
    StructWriteTextSub(df, str, base, 4);
    return 1;
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

    for (m = 0; m < str->nmembers; m++) {
        sm = &str->members[m];
        mp = ((unsigned char*)base) + sm->offset;
        w = 0;
        add_comment = 1;

        for (i = 0; i < indent; i++) {
            BrFilePutChar(' ', df->h);
        }

        // Modified from "%-10s", to ensure space after long type names
        BrFilePrintf(df->h, "%-9s ", member_type_names[sm->type]);

        switch (scalarTypeConvert(df, sm->type)) {
        case DF_TYPE_BR_INT_8:
            w = BrFilePrintf(df->h, "%d", *(br_int_8*)mp);
            break;
        case DF_TYPE_BR_UINT_8:
            w = BrFilePrintf(df->h, "%u", *(br_uint_8*)mp);
            break;
        case DF_TYPE_BR_INT_16:
            w = BrFilePrintf(df->h, "%d", *(br_int_16*)mp);
            break;
        case DF_TYPE_BR_UINT_16:
            w = BrFilePrintf(df->h, "%u", *(br_uint_16*)mp);
            break;
        case DF_TYPE_BR_INT_32:
            w = BrFilePrintf(df->h, "%d", *(br_int_32*)mp);
            break;
        case DF_TYPE_BR_UINT_32:
            w = BrFilePrintf(df->h, "%u", *(br_uint_32*)mp);
            break;
        case DF_TYPE_BR_FIXED:
            w = BrFilePrintf(df->h, "%g", BrFixedToFloat(*(br_fixed_ls*)mp));
            break;
        case DF_TYPE_BR_ANGLE:
            w = BrFilePrintf(df->h, "%g", BrFixedToFloat(BrFixedMul(*(br_angle*)mp, BrIntToFixed(360))));
            break;
        case DF_TYPE_FLOAT:
        case DF_TYPE_BR_FRACTION_F:
        case DF_TYPE_BR_UFRACTION_F:
            w = BrFilePrintf(df->h, "%g", (double)*(float*)mp);
            break;
        case DF_TYPE_DOUBLE:
            w = BrFilePrintf(df->h, "%g", *(double*)mp);
            break;
        case DF_TYPE_ENUM_8:
            w = BrFilePrintf(df->h, "%s", EnumToString(sm->extra, *(br_uint_8*)mp));
            break;
        case DF_TYPE_ENUM_16:
            w = BrFilePrintf(df->h, "%s", EnumToString(sm->extra, *(br_uint_16*)mp));
            break;
        case DF_TYPE_ENUM_32:
            w = BrFilePrintf(df->h, "%s", EnumToString(sm->extra, *(br_uint_32*)mp));
            break;
        case DF_TYPE_STRUCT:
            add_comment = 0;
            w = BrFilePrintf(df->h, "%s", ((br_file_struct*)sm->extra)->name);
            if (sm->name != NULL) {
                if (w < 40) {
                    for (i = 40 - w; i != 0; i--) {
                        BrFilePutChar(' ', df->h);
                    }
                }
                BrFilePrintf(df->h, " # %s", sm->name);
            }
            BrFilePutChar('\n', df->h);
            StructWriteTextSub(df, sm->extra, mp, indent + 2);
            break;
        case DF_TYPE_ASCIZ:
            if (*(char**)mp == NULL) {
                w = BrFilePrintf(df->h, "NULL");
            } else {
                w = BrFilePrintf(df->h, "\"%s\"", *(char**)mp);
            }
            break;
        case DF_TYPE_BR_COLOUR:
            w = BrFilePrintf(df->h, "%d,%d,%d", (br_uint_8)((*(br_uint_32*)mp) >> 16), (br_uint_8)((*(br_uint_32*)mp) >> 8), (br_uint_8)((*(br_uint_32*)mp)));
            break;
        case DF_TYPE_BR_FRACTION_X:
            w = BrFilePrintf(df->h, "%g", (double)BrFixedFractionToFloat(*(br_fraction_x*)mp));
            break;
        case DF_TYPE_BR_UFRACTION_X:
            w = BrFilePrintf(df->h, "%g", (double)BrFixedUFractionToFloat(*(br_fraction_x*)mp));
            break;
        case DF_TYPE_BR_VECTOR2_X:
            w = BrFilePrintf(df->h, "%g,%g", BrFixedToFloat(((br_fixed_ls*)mp)[0]), BrFixedToFloat(((br_fixed_ls*)mp)[1]));
            break;
        case DF_TYPE_BR_VECTOR3_X:
            w = BrFilePrintf(df->h, "%g,%g,%g", BrFixedToFloat(((br_fixed_ls*)mp)[0]), BrFixedToFloat(((br_fixed_ls*)mp)[1]), BrFixedToFloat(((br_fixed_ls*)mp)[2]));
            break;
        case DF_TYPE_BR_VECTOR4_X:
            w = BrFilePrintf(df->h, "%g,%g,%g,%g", BrFixedToFloat(((br_fixed_ls*)mp)[0]), BrFixedToFloat(((br_fixed_ls*)mp)[1]), BrFixedToFloat(((br_fixed_ls*)mp)[2]), BrFixedToFloat(((br_fixed_ls*)mp)[3]));
            break;
        case DF_TYPE_BR_VECTOR2_F:
            w = BrFilePrintf(df->h, "%g,%g", ((float*)mp)[0], ((float*)mp)[1]);
            break;
        case DF_TYPE_BR_VECTOR3_F:
            w = BrFilePrintf(df->h, "%g,%g,%g", ((float*)mp)[0], ((float*)mp)[1], ((float*)mp)[2]);
            break;
        case DF_TYPE_BR_VECTOR4_F:
            w = BrFilePrintf(df->h, "%g,%g,%g,%g", ((float*)mp)[0], ((float*)mp)[1], ((float*)mp)[2], ((float*)mp)[3]);
            break;
        }
        if ((add_comment != 0) && (sm->name != NULL)) {
            if (w < 40) {
                for (i = 40 - w; i != 0; i--) {
                    BrFilePutChar(' ', df->h);
                }
            }
            BrFilePrintf(df->h, " # %s\n", sm->name);
        }
    }

    return 1;
}

// IDA: br_uint_32 __usercall DfStructReadText@<EAX>(br_datafile *df@<EAX>, br_file_struct *str@<EDX>, void *base@<EBX>)
br_uint_32 DfStructReadText(br_datafile* df, br_file_struct* str, void* base) {
    char* id;
    char* data;
    LOG_TRACE9("(%p, %p, %p)", df, str, base);

    TextReadLine(df, &id, &data);
    if (BrStrCmp(id, "struct") != 0) {
        BrFailure("Unknown text identifer \"%s\"", id);
    }
    if (BrStrCmp(data, str->name) != 0) {
        BrFailure("Incorrect structure name \"%s\"", data);
    }
    StructReadTextSub(df, str, base);
    return 1;
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
    LOG_TRACE9("(%p, %p, %p)", df, str, base);

    for (m = 0; m < str->nmembers; m++) {
        sm = &str->members[m];
        mp = ((unsigned char*)base) + sm->offset;

        if (TextReadLine(df, &id, &data) == 0) {
            BrFailure("Unexpected EOF in strructure");
        }
        if (BrStrCmp(id, member_type_names[sm->type]) != 0) {
            BrFailure("Unknown member identifer \"%s\"", id);
        }

        switch (scalarTypeConvert(df, sm->type)) {
        case DF_TYPE_BR_INT_8:
            *(br_int_8*)mp = (br_int_8)BrStrToL(data, NULL, 0);
            break;
        case DF_TYPE_BR_UINT_8:
            *(br_uint_8*)mp = (br_uint_8)BrStrToL(data, NULL, 0);
            break;
        case DF_TYPE_BR_INT_16:
            *(br_int_16*)mp = (br_int_16)BrStrToL(data, NULL, 0);
            break;
        case DF_TYPE_BR_UINT_16:
            *(br_uint_16*)mp = (br_uint_16)BrStrToL(data, NULL, 0);
            break;
        case DF_TYPE_BR_INT_32:
            *(br_int_32*)mp = (br_int_32)BrStrToL(data, NULL, 0);
            break;
        case DF_TYPE_BR_UINT_32:
            *(br_uint_32*)mp = (br_uint_32)BrStrToUL(data, NULL, 0);
            break;
        case DF_TYPE_BR_FIXED:
            *(br_fixed_ls*)mp = BrFloatToFixed(BrStrToD(data, NULL));
            break;
        case DF_TYPE_BR_ANGLE:
            *(br_angle*)mp = BrFixedMulDiv(BrFloatToFixed(BrStrToF(data, NULL)), BrIntToFixed(1), BrIntToFixed(360));
            break;
        case DF_TYPE_FLOAT:
        case DF_TYPE_BR_FRACTION_F:
        case DF_TYPE_BR_UFRACTION_F:
            *(float*)mp = BrStrToF(data, NULL);
            break;
        case DF_TYPE_DOUBLE:
            *(double*)mp = BrStrToD(data, NULL);
            break;
        case DF_TYPE_ENUM_8:
            *(br_uint_8*)mp = EnumFromString(sm->extra, data);
            break;
        case DF_TYPE_ENUM_16:
            *(br_uint_16*)mp = EnumFromString(sm->extra, data);
            break;
        case DF_TYPE_ENUM_32:
            *(br_uint_32*)mp = EnumFromString(sm->extra, data);
            break;
        case DF_TYPE_STRUCT:
            if (BrStrCmp(data, ((br_file_struct*)sm->extra)->name) != 0) {
                BrFailure("Incorrect structure name \"%s\"", data);
            }
            StructReadTextSub(df, sm->extra, mp);
            break;
        case DF_TYPE_ASCIZ:
            if (BrStrCmp(data, "NULL") == 0) {
                *(char**)mp = NULL;
            } else {
                *(char**)mp = BrResStrDup(df->res == NULL ? fw.res : df->res, data + 1);
            }
            break;
        case DF_TYPE_BR_COLOUR:
            r = BrStrToUL(data, &ep, 0);
            if (*ep != ',') {
                BrFailure("Incorrect colour");
            }
            g = BrStrToUL(ep + 1, &ep, 0);
            if (*ep != ',') {
                BrFailure("Incorrect colour");
            }
            b = BrStrToUL(ep + 1, &ep, 0);
            *(br_colour*)mp = BR_COLOUR_RGB(r, g, b);
            break;
        case DF_TYPE_BR_FRACTION_X:
            *(br_fixed_ss*)mp = BrFloatToFixedFraction(BrStrToD(data, NULL));
            break;
        case DF_TYPE_BR_UFRACTION_X:
            *(br_fixed_su*)mp = BrFloatToFixedUFraction(BrStrToD(data, NULL));
            break;
        case DF_TYPE_BR_VECTOR2_X:
            n = 2;
            goto copy_fixed_vector;
        case DF_TYPE_BR_VECTOR3_X:
            n = 3;
            goto copy_fixed_vector;
        case DF_TYPE_BR_VECTOR4_X:
            n = 4;
        copy_fixed_vector:
            for (i = 0; i < n; i++) {
                while (1) {
                    if ((*data != ',') && (*data != ' ') && (*data != '\t')) {
                        break;
                    }
                    data++;
                }
                if (*data == '\0') {
                    BrFailure("Incorrect vector");
                }
                *(br_fixed_ls*)mp = BrFloatToFixed(BrStrToF(data, NULL));
                while ((*data != ' ' && *data != ',' && *data != ' ' && *data != '\t')) {
                    data++;
                }
                mp = (char*)mp + sizeof(br_fixed_ls);
            }
            break;
        case DF_TYPE_BR_VECTOR2_F:
            n = 2;
            goto copy_float_vector;
        case DF_TYPE_BR_VECTOR3_F:
            n = 3;
            goto copy_float_vector;
        case DF_TYPE_BR_VECTOR4_F:
            n = 4;
        copy_float_vector:
            for (i = 0; i < n; i++) {
                while (1) {
                    if ((*data != ',') && (*data != ' ') && (*data != '\t')) {
                        break;
                    }
                    data++;
                }
                if (*data == '\0') {
                    BrFailure("Incorrect vector");
                }
                *(float*)mp = BrStrToF(data, NULL);
                while ((*data != ' ' && *data != ',' && *data != ' ' && *data != '\t')) {
                    data++;
                }
                mp = (char*)mp + sizeof(float);
            }
            break;
        }
    }

    return 1;
}

// IDA: int __usercall DfStructSizeText@<EAX>(br_datafile *df@<EAX>, br_file_struct *str@<EDX>, void *base@<EBX>)
int DfStructSizeText(br_datafile* df, br_file_struct* str, void* base) {
    unsigned int m;
    br_file_struct_member* sm;
    int lines;
    LOG_TRACE("(%p, %p, %p)", df, str, base);

    lines = 1;
    for (m = 0; m < str->nmembers; m++) {
        sm = &str->members[m];
        if (sm->type == DF_TYPE_STRUCT) {
            lines += DfStructSizeText(df, sm->extra, (br_uint_8*)base + sm->offset);
        } else {
            lines++;
        }
    }
    return lines;
}

// IDA: br_uint_32 __usercall DfStructWriteArray@<EAX>(br_datafile *df@<EAX>, br_file_struct *str@<EDX>, void *base@<EBX>, int n@<ECX>)
br_uint_32 DfStructWriteArray(br_datafile* df, br_file_struct* str, void* base, int n) {
    char* cp;
    int i;
    LOG_TRACE("(%p, %p, %p, %d)", df, str, base, n);

    cp = (char*)base;
    for (i = 0; i < n; i++) {
        df->prims->struct_write(df, str, cp);
        cp += str->mem_size;
    }
    return i;
}

// IDA: br_uint_32 __usercall DfStructReadArray@<EAX>(br_datafile *df@<EAX>, br_file_struct *str@<EDX>, void *base@<EBX>, int n@<ECX>)
br_uint_32 DfStructReadArray(br_datafile* df, br_file_struct* str, void* base, int n) {
    char* cp;
    int i;
    LOG_TRACE9("(%p, %p, %p, %d)", df, str, base, n);

    cp = (char*)base;
    for (i = 0; i < n; i++) {
        if (BrFileEof(df->h) != 0) {
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

    if (id < BR_ASIZE(ChunkNames)) {
        BrFilePrintf(df->h, "*%-16s %d\n", ChunkNames[id], length);
    } else {
        BrFilePrintf(df->h, "*0x%08x %d\n", id, length);
    }
}

// IDA: int __usercall DfChunkReadText@<EAX>(br_datafile *df@<EAX>, br_uint_32 *plength@<EDX>)
int DfChunkReadText(br_datafile* df, br_uint_32* plength) {
    int i;
    char* id;
    char* data;
    LOG_TRACE9("(%p, %p)", df, plength);

    if (TextReadLine(df, &id, &data) == 0) {
        return -1;
    }
    if (*id != '*') {
        BrFailure("Chunk ID not found");
    }
    id++;
    if (*id == '0') {
        i = BrStrToUL(id, NULL, 0);
    } else {
        for (i = 0; i < BR_ASIZE(ChunkNames); i++) {
            if (BrStrCmp(ChunkNames[i], id) == 0) {
                break;
            }
        }
        if (i >= BR_ASIZE(ChunkNames)) {
            BrFailure("Chunk ID not known: %s", id);
        }
    }
    if (plength != NULL) {
        *plength = BrStrToUL(data, NULL, 0);
    }
    return i;
}

// IDA: int __usercall DfChunkWriteBinary@<EAX>(br_datafile *df@<EAX>, br_uint_32 id@<EDX>, br_uint_32 length@<EBX>)
int DfChunkWriteBinary(br_datafile* df, br_uint_32 id, br_uint_32 length) {
    br_uint_32 l;
    LOG_TRACE("(%p, %d, %d)", df, id, length);

    l = BrHtoNL(id);
    BrFileWrite(&l, sizeof(br_uint_32), 1, df->h);
    l = BrHtoNL(length);
    BrFileWrite(&l, sizeof(br_uint_32), 1, df->h);
    return 0;
}

// IDA: int __usercall DfChunkReadBinary@<EAX>(br_datafile *df@<EAX>, br_uint_32 *plength@<EDX>)
int DfChunkReadBinary(br_datafile* df, br_uint_32* plength) {
    br_uint_32 id;
    br_uint_32 l;
    LOG_TRACE9("(%p, %p)", df, plength);

    if (BrFileEof(df->h) != 0) {
        return -1;
    }
    BrFileRead(&id, sizeof(br_uint_32), 1, df->h);
    if (BrFileEof(df->h) != 0) {
        return -1;
    }
    id = BrHtoNL(id);
    BrFileRead(&l, sizeof(br_uint_32), 1, df->h);
    if (BrFileEof(df->h) != 0) {
        return -1;
    }
    if (plength != NULL) {
        *plength = BrHtoNL(l);
    }
    return id;
}

// IDA: void __usercall DfCountWriteText(br_datafile *df@<EAX>, br_uint_32 count@<EDX>)
void DfCountWriteText(br_datafile* df, br_uint_32 count) {
    LOG_TRACE("(%p, %d)", df, count);

    BrFilePrintf(df->h, " count %d\n", count);
}

// IDA: br_uint_32 __usercall DfCountReadText@<EAX>(br_datafile *df@<EAX>)
br_uint_32 DfCountReadText(br_datafile* df) {
    char* id;
    char* data;
    LOG_TRACE("(%p)", df);

    TextReadLine(df, &id, &data);
    if (BrStrCmp(id, "count") != 0) {
        BrFailure("no element count for chunk");
    }
    return BrStrToUL(data, NULL, 0);
}

// IDA: void __usercall DfCountWriteBinary(br_datafile *df@<EAX>, br_uint_32 count@<EDX>)
void DfCountWriteBinary(br_datafile* df, br_uint_32 count) {
    br_uint_32 l;

    l = BrHtoNL(count);
    BrFileWrite(&l, sizeof(l), 1, df->h);
}

// IDA: br_uint_32 __usercall DfCountReadBinary@<EAX>(br_datafile *df@<EAX>)
br_uint_32 DfCountReadBinary(br_datafile* df) {
    br_uint_32 l;
    LOG_TRACE9("(%p)", df);

    BrFileRead(&l, sizeof(br_uint_32), 1, df->h);
    return BrHtoNL(l);
}

// IDA: int __usercall DfCountSizeText@<EAX>(br_datafile *df@<EAX>)
int DfCountSizeText(br_datafile* df) {
    LOG_TRACE("(%p)", df);

    return 1;
}

// IDA: int __usercall DfCountSizeBinary@<EAX>(br_datafile *df@<EAX>)
int DfCountSizeBinary(br_datafile* df) {
    LOG_TRACE("(%p)", df);

    return sizeof(br_uint_32);
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

    if (block_stride == block_size) {
        block_size = block_count * block_size;
        block_count = 1;
    }
    block = base;
#if !BR_ENDIAN_BIG
    if ((size != 1) || (block_count != 1)) {
#else
    if (block_count != 1) {
#endif
        block = BrScratchAllocate(block_count * block_size * size);
        for (count = 0; count < block_count; count++) {
            BrMemCpy(block + count * block_size * size, (br_uint_8*)base + count * block_stride * size, block_size * size);
        }
#if !BR_ENDIAN_BIG
        BrSwapBlock(block, block_count * block_size, size);
#endif
    }
    BrFilePrintf(df->h, "  block %d\n", block_count * block_size);
    BrFilePrintf(df->h, "  size %d\n", size);
    for (i = 0; i < block_count * block_size * size; i++) {
        if ((i & 0x1f) == 0) {
            BrFilePrintf(df->h, "    %08x: %02x", i, block[i]);
        } else {
            BrFilePrintf(df->h, "%02x", block[i]);
        }
        if ((i & 0x1f) == 0x1f) {
            BrFilePutChar('\n', df->h);
        }
    }
    if ((i & 0x1f) != 0x0) {
        BrFilePutChar('\n', df->h);
    }
    if (block != base) {
        BrScratchFree(block);
    }
    return 0;
}

// IDA: void* __usercall DfBlockReadText@<EAX>(br_datafile *df@<EAX>, void *base@<EDX>, int *count@<EBX>, int size@<ECX>, int mtype)
void* DfBlockReadText(br_datafile* df, void* base, int* count, int size, int mtype) {
    char* id;
    char* data;
    int l;
    int s;
    int a;
    char b[3];
    LOG_TRACE9("(%p, %p, %p, %d, %d)", df, base, count, size, mtype);

    TextReadLine(df, &id, &data);
    if (BrStrCmp(id, "block") != 0) {
        BrFailure("no block");
    }
    l = BrStrToUL(data, NULL, 0);
    TextReadLine(df, &id, &data);
    if (BrStrCmp(id, "size") != 0) {
        BrFailure("no size");
    }
    s = BrStrToUL(data, NULL, 0);
    if (s != size) {
        BrFailure("block size mismatch");
    }
    if (base == NULL) {
        base = BrResAllocate(df->res ? df->res : fw.res, l * size, mtype);
    } else {
        if (*count < l) {
            BrFailure("DfBlockReadText: block too long: %d", l);
        }
    }
    *count = l;
    for (a = 0; a < l * size;) {
        TextReadLine(df, &id, &data);
        if (a != BrStrToL(id, NULL, 16)) {
            BrFailure("block address mismatch");
        }
        while ((data[0] != '\0') && (data[1] != '\0')) {
            b[0] = data[0];
            b[1] = data[1];
            b[2] = '\0';
            ((br_uint_8*)base)[a] = (br_uint_8)BrStrToUL(b, NULL, 16);
            data += 2;
            a++;
        }
    }
#if !BR_ENDIAN_BIG
    BrSwapBlock(base, l, size);
#endif
    return base;
}

// IDA: int __usercall DfBlockWriteBinary@<EAX>(br_datafile *df@<EAX>, void *base@<EDX>, int block_size@<EBX>, int block_stride@<ECX>, int block_count, int size)
int DfBlockWriteBinary(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size) {
    int count;
    br_uint_32 l;
    br_uint_32 s;
    void* block;
    LOG_TRACE("(%p, %p, %d, %d, %d, %d)", df, base, block_size, block_stride, block_count, size);

    l = BrHtoNL(block_count * block_size);
    s = BrHtoNL(size);
    if (block_stride == block_size) {
        block_size = block_count * block_size;
        block_count = 1;
    }
    block = base;
#if !BR_ENDIAN_BIG
    if ((size != 1) || (block_count != 1)) {
#else
    if (block_count != 1) {
#endif
        block = BrScratchAllocate(block_count * block_size * size);
        for (count = 0; count < block_count; count++) {
            BrMemCpy((br_uint_8*)block + count * block_size * size, (br_uint_8*)base + count * block_stride * size, block_size * size);
        }
#if !BR_ENDIAN_BIG
        BrSwapBlock(block, block_count * block_size, size);
#endif
    }
    BrFileWrite(&l, sizeof(l), 1, df->h);
    BrFileWrite(&s, sizeof(s), 1, df->h);
    BrFileWrite(block, block_count * block_size, size, df->h);
    if (block != base) {
        BrScratchFree(block);
    }
    return 0;
}

// IDA: void* __usercall DfBlockReadBinary@<EAX>(br_datafile *df@<EAX>, void *base@<EDX>, int *count@<EBX>, int size@<ECX>, int mtype)
void* DfBlockReadBinary(br_datafile* df, void* base, int* count, int size, int mtype) {
    int l;
    int s;
    LOG_TRACE9("(%p, %p, %p, %d, %d)", df, base, count, size, mtype);

    BrFileRead(&l, 4, 1, df->h);
    l = BrHtoNL(l);
    BrFileRead(&s, 4, 1, df->h);
    s = BrHtoNL(s);
    if (s != size) {
        BrFailure("block size mismatch");
    }
    if (base != NULL) {
        if (l > *count) {
            BrFailure("DfBlockReadBinary: block too long: %d", l);
        }
    } else {
        base = BrResAllocate(df->res != NULL ? df->res : fw.res, size * l, mtype);
    }
    *count = l;
    BrFileRead(base, l, size, df->h);
#if !BR_ENDIAN_BIG
    BrSwapBlock(base, l, size);
#endif
    return base;
}

// IDA: int __usercall DfBlockSizeText@<EAX>(br_datafile *df@<EAX>, void *base@<EDX>, int block_size@<EBX>, int block_stride@<ECX>, int block_count, int size)
int DfBlockSizeText(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size) {
    LOG_TRACE("(%p, %p, %d, %d, %d, %d)", df, base, block_size, block_stride, block_count, size);

    return ((size * block_count * block_size + (32 - 1)) >> 5) + 2;
}

// IDA: int __usercall DfBlockSizeBinary@<EAX>(br_datafile *df@<EAX>, void *base@<EDX>, int block_size@<EBX>, int block_stride@<ECX>, int block_count, int size)
int DfBlockSizeBinary(br_datafile* df, void* base, int block_size, int block_stride, int block_count, int size) {
    LOG_TRACE("(%p, %p, %d, %d, %d, %d)", df, base, block_size, block_stride, block_count, size);

    return size * block_count * block_size + 8;
}

// IDA: char* __usercall DfNameReadText@<EAX>(br_datafile *df@<EAX>, char *name@<EDX>)
char* DfNameReadText(br_datafile* df, char* name) {
    char* id;
    char* data;
    LOG_TRACE("(%p, \"%s\")", df, name);

    TextReadLine(df, &id, &data);
    if (BrStrCmp(id, "name") != 0) {
        BrFailure("no name");
    }
    if ((data != NULL) && (*data == '\"')) {
        BrStrNCpy(name, data + 1, 0xff);
        name[0xff] = '\0';
        return data;
    }
    BrFailure("no name string");
}

// IDA: int __usercall DfNameWriteText@<EAX>(br_datafile *df@<EAX>, char *name@<EDX>)
int DfNameWriteText(br_datafile* df, char* name) {
    LOG_TRACE("(%p, \"%s\")", df, name);

    if (name == NULL) {
        name = "NULL";
    }
    BrFilePrintf(df->h, "  name \"%s\"\n", name);
    return 0;
}

// IDA: int __usercall DfNameSizeText@<EAX>(br_datafile *df@<EAX>, char *name@<EDX>)
int DfNameSizeText(br_datafile* df, char* name) {
    LOG_TRACE("(%p, \"%s\")", df, name);

    return 1;
}

// IDA: char* __usercall DfNameReadBinary@<EAX>(br_datafile *df@<EAX>, char *name@<EDX>)
char* DfNameReadBinary(br_datafile* df, char* name) {
    int c;
    int i;
    LOG_TRACE9("(%p, \"%s\")", df, name);

    for (i = 0; i < 255; i++) {
        c = BrFileGetChar(df->h);
        if ((c == 0) || (c == -1)) {
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

    if (name != NULL) {
        BrFileWrite(name, 1, BrStrLen(name), df->h);
    }
    BrFilePutChar('\0', df->h);
    return 0;
}

// IDA: int __usercall DfNameSizeBinary@<EAX>(br_datafile *df@<EAX>, char *name@<EDX>)
int DfNameSizeBinary(br_datafile* df, char* name) {
    LOG_TRACE("(%p, \"%s\")", df, name);

    if (name != NULL) {
        return BrStrLen(name) + 1;
    }
    return 1;
}

// IDA: int __usercall DfSkipText@<EAX>(br_datafile *df@<EAX>, br_uint_32 length@<EDX>)
int DfSkipText(br_datafile* df, br_uint_32 length) {
    char* id;
    char* data;
    LOG_TRACE("(%p, %d)", df, length);

    while ((BrFileEof(df->h) == 0) && (length != 0)) {
        TextReadLine(df, &data, &id);
        length--;
    }
    return 0;
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
        // LOG_DEBUG("chunk id=%d, len=%d", id, length);
        if (id == (br_uint_32)-1) {
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
            if (table->entries[i].has_count != 0) {
                count = df->prims->count_read(df);
            } else {
                count = 0;
            }
            r = table->entries[i].handler(df, id, length, count);
            if (r != 0) {
                return r;
            }
        }
    }
    return 0;
}

// IDA: void __cdecl BrNullOther()
void BrNullOther(void) {
    LOG_TRACE("()");

    // Yes, "invalid" is misspelled.
    BrFatal("datafile.c", 1825, "Invald file primitive call");
}

// IDA: int __cdecl DfFileIdentify(br_uint_8 *magics, br_size_t n_magics)
int DfFileIdentify(br_uint_8* magics, br_size_t n_magics) {
    static char text_magics[8] = { '*', 'F', 'I', 'L', 'E', '_', 'I', 'N' };
    static char binary_magics[8] = { '\0', '\0', '\0', '\x12', '\0', '\0', '\0', '\b' };

    if (BrMemCmp(magics, text_magics, sizeof(magics)) == 0) {
        return BR_FS_MODE_TEXT;
    }
    if (BrMemCmp(magics, binary_magics, sizeof(magics)) == 0) {
        return BR_FS_MODE_BINARY;
    }
    LOG_WARN("file does not match magics");
    return BR_FS_MODE_UNKNOWN;
}

// IDA: br_datafile* __usercall DfOpen@<EAX>(char *name@<EAX>, int write@<EDX>, br_token scalar_type@<EBX>)
br_datafile* DfOpen(char* name, int write, br_token scalar_type) {
    int mode;
    char c;
    br_datafile* df;
    void* h;
    LOG_TRACE9("(\"%s\", %d, %d)", name, write, scalar_type);

    mode = fw.open_mode;
    if (write != 0) {
        h = BrFileOpenWrite(name, fw.open_mode);
    } else {
        h = BrFileOpenRead(name, 8u, DfFileIdentify, &mode);
    }
    if (h == NULL) {
        return NULL;
    }
    df = BrResAllocate(fw.res, sizeof(br_datafile), BR_MEMORY_DATAFILE);
    df->prims = &_BrFilePrimsNull;
    df->h = h;
    df->scalar_type = scalar_type;

    if (mode == BR_FS_MODE_BINARY) {
        if (write != 0) {
            df->prims = &_BrFilePrimsWriteBinary;
        } else {
            df->prims = &_BrFilePrimsReadBinary;
        }
    } else {
        if (write != 0)
            df->prims = &_BrFilePrimsWriteText;
        else
            df->prims = &_BrFilePrimsReadText;
    }
    DfPush(BR_MEMORY_FILE, df, 1); // Wrong enum -> should be DF_STACK_FILE (see comment in src/BRSRC13/CORE/V1DB/v1dbfile.c)
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

    old = fw.open_mode;
    fw.open_mode = mode;
    return old;
}
