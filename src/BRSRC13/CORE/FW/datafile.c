#include "datafile.h"

br_file_primitives _BrFilePrimsNull;
br_file_primitives _BrFilePrimsReadBinary;
br_file_primitives _BrFilePrimsWriteText;
br_file_primitives _BrFilePrimsWriteBinary;
br_file_primitives _BrFilePrimsReadText;
char *member_type_names[32];
struct {
        int type;
        void *value;
        int count;
    };
char *ChunkNames[61];
char rscid[52];
int DatafileStackTop;

// Offset: 7
// Size: 209
// EAX: type
// EDX: value
// EBX: count
void DfPush(int type, void *value, int count) {
}

// Offset: 222
// Size: 193
// EAX: type
// EDX: countp
void* DfPop(int type, int *countp) {
}

// Offset: 421
// Size: 193
// EAX: type
// EDX: countp
void* DfTop(int type, int *countp) {
}

// Offset: 624
// Size: 73
int DfTopType() {
}

// Offset: 710
// Size: 355
// EAX: df
// EDX: ident
// EBX: data
int TextReadLine(br_datafile *df, char **ident, char **data) {
    char *cp;
}

// Offset: 1083
// Size: 436
// EAX: df
// EDX: t
br_uint_16 scalarTypeConvert(br_datafile *df, br_uint_16 t) {
}

// Offset: 1539
// Size: 1211
// EAX: df
// EDX: str
// EBX: base
br_uint_32 DfStructWriteBinary(br_datafile *df, br_file_struct *str, void *base) {
    unsigned int m;
    int i;
    int n;
    unsigned char *mp;
    br_file_struct_member *sm;
    struct {
        unsigned char b[8];
        float f;
    };
}

// Offset: 2769
// Size: 1394
// EAX: df
// EDX: str
// EBX: base
br_uint_32 DfStructReadBinary(br_datafile *df, br_file_struct *str, void *base) {
    char tmp_string[256];
    unsigned int m;
    int i;
    int c;
    int n;
    unsigned char *mp;
    br_file_struct_member *sm;
    struct {
        unsigned char b[8];
        float f;
    };
}

// Offset: 4182
// Size: 427
// EAX: df
// EDX: str
// EBX: base
int DfStructSizeBinary(br_datafile *df, br_file_struct *str, void *base) {
    unsigned char *mp;
    unsigned int m;
    br_file_struct_member *sm;
    int bytes;
}

// Offset: 4624
// Size: 177
// EAX: e
// EDX: str
int EnumFromString(br_file_enum *e, char *str) {
    unsigned int m;
}

// Offset: 4814
// Size: 138
// EAX: e
// EDX: num
char* EnumToString(br_file_enum *e, int num) {
    unsigned int m;
}

// Offset: 4970
// Size: 92
// EAX: df
// EDX: str
// EBX: base
br_uint_32 DfStructWriteText(br_datafile *df, br_file_struct *str, void *base) {
}

// Offset: 5081
// Size: 1827
// EAX: df
// EDX: str
// EBX: base
// ECX: indent
br_uint_32 StructWriteTextSub(br_datafile *df, br_file_struct *str, void *base, int indent) {
    unsigned int m;
    int i;
    int w;
    int add_comment;
    void *mp;
    br_file_struct_member *sm;
}

// Offset: 6925
// Size: 156
// EAX: df
// EDX: str
// EBX: base
br_uint_32 DfStructReadText(br_datafile *df, br_file_struct *str, void *base) {
    char *id;
    char *data;
}

// Offset: 7099
// Size: 1609
// EAX: df
// EDX: str
// EBX: base
br_uint_32 StructReadTextSub(br_datafile *df, br_file_struct *str, void *base) {
    unsigned int m;
    unsigned int r;
    unsigned int g;
    unsigned int b;
    int i;
    int n;
    void *mp;
    br_file_struct_member *sm;
    char *id;
    char *data;
    char *ep;
}

// Offset: 8725
// Size: 144
// EAX: df
// EDX: str
// EBX: base
int DfStructSizeText(br_datafile *df, br_file_struct *str, void *base) {
    unsigned int m;
    br_file_struct_member *sm;
    int lines;
}

// Offset: 8888
// Size: 109
// EAX: df
// EDX: str
// EBX: base
// ECX: n
br_uint_32 DfStructWriteArray(br_datafile *df, br_file_struct *str, void *base, int n) {
    char *cp;
    int i;
}

// Offset: 9015
// Size: 126
// EAX: df
// EDX: str
// EBX: base
// ECX: n
br_uint_32 DfStructReadArray(br_datafile *df, br_file_struct *str, void *base, int n) {
    char *cp;
    int i;
}

// Offset: 9158
// Size: 150
// EAX: df
// EDX: id
// EBX: length
int DfChunkWriteText(br_datafile *df, br_uint_32 id, br_uint_32 length) {
}

// Offset: 9324
// Size: 238
// EAX: df
// EDX: plength
int DfChunkReadText(br_datafile *df, br_uint_32 *plength) {
    int i;
    char *id;
    char *data;
}

// Offset: 9581
// Size: 121
// EAX: df
// EDX: id
// EBX: length
int DfChunkWriteBinary(br_datafile *df, br_uint_32 id, br_uint_32 length) {
    br_uint_32 l;
}

// Offset: 9720
// Size: 210
// EAX: df
// EDX: plength
int DfChunkReadBinary(br_datafile *df, br_uint_32 *plength) {
    br_uint_32 id;
    br_uint_32 l;
}

// Offset: 9947
// Size: 61
// EAX: df
// EDX: count
void DfCountWriteText(br_datafile *df, br_uint_32 count) {
}

// Offset: 10024
// Size: 109
// EAX: df
br_uint_32 DfCountReadText(br_datafile *df) {
    char *id;
    char *data;
}

// Offset: 10152
// Size: 74
// EAX: df
// EDX: count
void DfCountWriteBinary(br_datafile *df, br_uint_32 count) {
    br_uint_32 l;
}

// Offset: 10244
// Size: 76
// EAX: df
br_uint_32 DfCountReadBinary(br_datafile *df) {
    br_uint_32 l;
}

// Offset: 10336
// Size: 47
// EAX: df
int DfCountSizeText(br_datafile *df) {
}

// Offset: 10401
// Size: 47
// EAX: df
int DfCountSizeBinary(br_datafile *df) {
}

// Offset: 10464
// Size: 227
// EAX: base
// EDX: block_size
// EBX: block_stride
// ECX: block_count
br_uint_8* BlockWriteSetup(void *base, int block_size, int block_stride, int block_count, int size) {
    int b;
    br_uint_8 *block;
    br_uint_8 *sp;
    br_uint_8 *dp;
}

// Offset: 10708
// Size: 358
// EAX: df
// EDX: base
// EBX: block_size
// ECX: block_stride
int DfBlockWriteText(br_datafile *df, void *base, int block_size, int block_stride, int block_count, int size) {
    int i;
    br_uint_8 *cp;
    br_uint_8 *block;
    int count;
}

// Offset: 11082
// Size: 498
// EAX: df
// EDX: base
// EBX: count
// ECX: size
void* DfBlockReadText(br_datafile *df, void *base, int *count, int size, int mtype) {
    char *id;
    char *data;
    int l;
    int s;
    int a;
    char b[3];
}

// Offset: 11599
// Size: 203
// EAX: df
// EDX: base
// EBX: block_size
// ECX: block_stride
int DfBlockWriteBinary(br_datafile *df, void *base, int block_size, int block_stride, int block_count, int size) {
    int count;
    br_uint_32 l;
    br_uint_32 s;
    void *block;
}

// Offset: 11820
// Size: 300
// EAX: df
// EDX: base
// EBX: count
// ECX: size
void* DfBlockReadBinary(br_datafile *df, void *base, int *count, int size, int mtype) {
    int l;
    int s;
}

// Offset: 12136
// Size: 78
// EAX: df
// EDX: base
// EBX: block_size
// ECX: block_stride
int DfBlockSizeText(br_datafile *df, void *base, int block_size, int block_stride, int block_count, int size) {
}

// Offset: 12232
// Size: 62
// EAX: df
// EDX: base
// EBX: block_size
// ECX: block_stride
int DfBlockSizeBinary(br_datafile *df, void *base, int block_size, int block_stride, int block_count, int size) {
}

// Offset: 12309
// Size: 157
// EAX: df
// EDX: name
char* DfNameReadText(br_datafile *df, char *name) {
    char *id;
    char *data;
}

// Offset: 12482
// Size: 91
// EAX: df
// EDX: name
int DfNameWriteText(br_datafile *df, char *name) {
}

// Offset: 12588
// Size: 48
// EAX: df
// EDX: name
int DfNameSizeText(br_datafile *df, char *name) {
}

// Offset: 12653
// Size: 125
// EAX: df
// EDX: name
char* DfNameReadBinary(br_datafile *df, char *name) {
    int c;
    int i;
}

// Offset: 12796
// Size: 101
// EAX: df
// EDX: name
int DfNameWriteBinary(br_datafile *df, char *name) {
}

// Offset: 12914
// Size: 72
// EAX: df
// EDX: name
int DfNameSizeBinary(br_datafile *df, char *name) {
}

// Offset: 12997
// Size: 92
// EAX: df
// EDX: length
int DfSkipText(br_datafile *df, br_uint_32 length) {
    char *id;
    char *data;
}

// Offset: 13102
// Size: 65
// EAX: df
// EDX: length
int DfSkipBinary(br_datafile *df, br_uint_32 length) {
}

// Offset: 13185
// Size: 241
// EAX: df
// EDX: table
int DfChunksInterpret(br_datafile *df, br_chunks_table *table) {
    br_uint_32 length;
    br_uint_32 count;
    br_uint_32 id;
    int r;
    int i;
}

// Offset: 13438
// Size: 59
void BrNullOther() {
}

// Offset: 13512
// Size: 168
int DfFileIdentify(br_uint_8 *magics, br_size_t n_magics) {
    static char text_magics[8];
    static char binary_magics[8];
}

// Offset: 13687
// Size: 278
// EAX: name
// EDX: write
// EBX: scalar_type
br_datafile* DfOpen(char *name, int write, br_token scalar_type) {
    int mode;
    br_datafile *df;
    void *h;
}

// Offset: 13973
// Size: 198
// EAX: df
void DfClose(br_datafile *df) {
    br_datafile *dfp;
}

// Offset: 14186
// Size: 94
int BrWriteModeSet(int mode) {
    int old;
}

