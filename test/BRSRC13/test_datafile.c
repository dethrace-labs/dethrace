#include "tests.h"

#include "CORE/FW/datafile.h"
#include "CORE/FW/file.h"
#include "CORE/MATH/fixed.h"

#include <string.h>

#define TEXT_MAGICS "*FILE_IN"
#define BINARY_MAGICS '\0', '\0', '\0', '\x12', '\0', '\0', '\0', '\b'

char *text_magics  = TEXT_MAGICS;
uint8_t binary_magics[] = {BINARY_MAGICS};


static void test_datafile_stack() {
    int dummy1;
    int dummy2;
    int val;
    void *ptr;
    int initialtoptype;

    initialtoptype = DfTopType();

    DfPush(0xcafebabe, &dummy1, 42);

    TEST_ASSERT_EQUAL_UINT32(0xcafebabe, DfTopType());

    val = 0;
    ptr = NULL;
    ptr = DfTop(0xcafebabe, &val);

    TEST_ASSERT_EQUAL_PTR(&dummy1, ptr);
    TEST_ASSERT_EQUAL_INT(42, val);
    TEST_ASSERT_EQUAL_UINT32(0xcafebabe, DfTopType());

    DfPush(0xf00dbabe, &dummy2, 9000);

    TEST_ASSERT_EQUAL_UINT32(0xf00dbabe, DfTopType());

    val = 0;
    ptr = NULL;
    ptr = DfTop(0xf00dbabe, &val);

    TEST_ASSERT_EQUAL_PTR(&dummy2, ptr);
    TEST_ASSERT_EQUAL_INT(9000, val);
    TEST_ASSERT_EQUAL_UINT32(0xf00dbabe, DfTopType());

    val = 0;
    ptr = NULL;
    ptr = DfPop(0xf00dbabe, &val);

    TEST_ASSERT_EQUAL_PTR(&dummy2, ptr);
    TEST_ASSERT_EQUAL_INT(9000, val);
    TEST_ASSERT_EQUAL_UINT32(0xcafebabe, DfTopType());

    val = 0;
    ptr = NULL;
    ptr = DfPop(0xcafebabe, &val);

    TEST_ASSERT_EQUAL_PTR(&dummy1, ptr);
    TEST_ASSERT_EQUAL_INT(42, val);
    TEST_ASSERT_EQUAL_UINT32(initialtoptype, DfTopType());
}

static void test_datafile_magics() {
    TEST_ASSERT_EQUAL_INT(BR_FS_MODE_TEXT, DfFileIdentify(text_magics, sizeof(text_magics)));
    TEST_ASSERT_EQUAL_INT(BR_FS_MODE_BINARY, DfFileIdentify(binary_magics, sizeof(binary_magics)));
    TEST_ASSERT_EQUAL_INT(BR_FS_MODE_UNKNOWN, DfFileIdentify("nonsense", sizeof("nonsense")));
}

typedef struct struct_br_int_8 {
    char dummy0[11];
    br_int_8 m1;
    char dummy1[13];
    br_int_8 m2;
    char dummy2[17];
    br_int_8 m3;
    char dummy3[23];
    br_int_8 m4;
    char dummy4[27];
} struct_br_int_8;
static br_file_struct_member struct_br_int_8_file_members[] = {
    { DF_TYPE_BR_INT_8, offsetof(struct_br_int_8, m1), "m1", NULL, },
    { DF_TYPE_BR_INT_8, offsetof(struct_br_int_8, m2), "m2", NULL, },
    { DF_TYPE_BR_INT_8, offsetof(struct_br_int_8, m3), "m3", NULL, },
    { DF_TYPE_BR_INT_8, offsetof(struct_br_int_8, m4), "m4", NULL, },
};
static br_file_struct struct_br_int_8_file = {
    "struct_br_int_8", BR_ASIZE(struct_br_int_8_file_members), struct_br_int_8_file_members, sizeof(struct_br_int_8),
};
static struct_br_int_8 br_int_8_ref = { { 0 }, 42, { 0 }, -42, { 0 }, -2, { 0 }, 87, { 0 }, };
static const uint8_t binary_br_int_8_data[] = { BINARY_MAGICS, 0x2a, 0xd6, 0xfe, 0x57, };
static const char *text_br_int_8_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_int_8" HOST_NL
    "    int_8     42                                       # m1" HOST_NL
    "    int_8     -42                                      # m2" HOST_NL
    "    int_8     -2                                       # m3" HOST_NL
    "    int_8     87                                       # m4" HOST_NL;
static void test_datafile_binary_br_int_8() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_int_8 read_struct;

    create_temp_file(tmpfilename, "br_int_8_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(4, DfStructSizeBinary(df_w, &struct_br_int_8_file, &br_int_8_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_int_8_file, &br_int_8_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_int_8_data, tmpfilename, sizeof(binary_br_int_8_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_int_8_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_int_8_ref, &read_struct, sizeof(br_int_8_ref));
}
static void test_datafile_text_br_int_8() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_int_8 read_struct;

    create_temp_file(tmpfilename, "br_int_8_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_int_8_file, &br_int_8_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_int_8_file, &br_int_8_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_int_8_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_int_8_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_int_8_ref, &read_struct, sizeof(br_int_8_ref));
}

typedef struct struct_br_uint_8 {
    char dummy0[11];
    br_uint_8 m1;
    char dummy1[13];
    br_uint_8 m2;
    char dummy2[17];
    br_uint_8 m3;
    char dummy3[23];
    br_uint_8 m4;
    char dummy4[27];
} struct_br_uint_8;
static br_file_struct_member struct_br_uint_8_file_members[] = {
    { DF_TYPE_BR_UINT_8, offsetof(struct_br_uint_8, m1), "m1", NULL, },
    { DF_TYPE_BR_UINT_8, offsetof(struct_br_uint_8, m2), "m2", NULL, },
    { DF_TYPE_BR_UINT_8, offsetof(struct_br_uint_8, m3), "m3", NULL, },
    { DF_TYPE_BR_UINT_8, offsetof(struct_br_uint_8, m4), "m4", NULL, },
};
static br_file_struct struct_br_uint_8_file = {
    "struct_br_uint_8", BR_ASIZE(struct_br_uint_8_file_members), struct_br_uint_8_file_members, sizeof(struct_br_uint_8),
};
static struct_br_uint_8 br_uint_8_ref = { { 0 }, 40, { 0 }, 250, { 0 }, 129, { 0 }, 127, { 0 }, };
static const uint8_t binary_br_uint_8_data[] = { BINARY_MAGICS, 0x28, 0xfa, 0x81, 0x7f, };
static const char *text_br_uint_8_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_uint_8" HOST_NL
    "    uint_8    40                                       # m1" HOST_NL
    "    uint_8    250                                      # m2" HOST_NL
    "    uint_8    129                                      # m3" HOST_NL
    "    uint_8    127                                      # m4" HOST_NL;
static void test_datafile_binary_br_uint_8() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_uint_8 read_struct;

    create_temp_file(tmpfilename, "br_uint_8_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(4, DfStructSizeBinary(df_w, &struct_br_uint_8_file, &br_uint_8_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_uint_8_file, &br_uint_8_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_uint_8_data, tmpfilename, sizeof(binary_br_uint_8_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_uint_8_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_uint_8_ref, &read_struct, sizeof(br_uint_8_ref));
}
static void test_datafile_text_br_uint_8() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_uint_8 read_struct;

    create_temp_file(tmpfilename, "br_uint_8_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_uint_8_file, &br_uint_8_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_uint_8_file, &br_uint_8_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_uint_8_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_uint_8_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_uint_8_ref, &read_struct, sizeof(br_uint_8_ref));
}

typedef struct struct_br_int_16 {
    char dummy0[11];
    br_int_16 m1;
    char dummy1[13];
    br_int_16 m2;
    char dummy2[17];
    br_int_16 m3;
    char dummy3[23];
    br_int_16 m4;
    char dummy4[27];
} struct_br_int_16;
static br_file_struct_member struct_br_int_16_file_members[] = {
    { DF_TYPE_BR_INT_16, offsetof(struct_br_int_16, m1), "m1", NULL, },
    { DF_TYPE_BR_INT_16, offsetof(struct_br_int_16, m2), "m2", NULL, },
    { DF_TYPE_BR_INT_16, offsetof(struct_br_int_16, m3), "m3", NULL, },
    { DF_TYPE_BR_INT_16, offsetof(struct_br_int_16, m4), "m4", NULL, },
};
static br_file_struct struct_br_int_16_file = {
    "struct_br_int_16", BR_ASIZE(struct_br_int_16_file_members), struct_br_int_16_file_members, sizeof(struct_br_int_16),
};
static struct_br_int_16 br_int_16_ref = { { 0 }, -32700, { 0 }, 32700, { 0 }, 255, { 0 }, -40, { 0 }, };
static const uint8_t binary_br_int_16_data[] = { BINARY_MAGICS, 0x80, 0x44, 0x7f, 0xbc, 0x00, 0xff, 0xff, 0xd8, };
static const char *text_br_int_16_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_int_16" HOST_NL
    "    int_16    -32700                                   # m1" HOST_NL
    "    int_16    32700                                    # m2" HOST_NL
    "    int_16    255                                      # m3" HOST_NL
    "    int_16    -40                                      # m4" HOST_NL;
static void test_datafile_binary_br_int_16() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_int_16 read_struct;

    create_temp_file(tmpfilename, "br_int_16_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(8, DfStructSizeBinary(df_w, &struct_br_int_16_file, &br_int_16_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_int_16_file, &br_int_16_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_int_16_data, tmpfilename, sizeof(binary_br_int_16_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_int_16_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_int_16_ref, &read_struct, sizeof(br_int_16_ref));
}
static void test_datafile_text_br_int_16() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_int_16 read_struct;

    create_temp_file(tmpfilename, "br_int_16_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_int_16_file, &br_int_16_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_int_16_file, &br_int_16_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_int_16_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_int_16_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_int_16_ref, &read_struct, sizeof(br_int_16_ref));
}

typedef struct struct_br_uint_16 {
    char dummy0[11];
    br_uint_16 m1;
    char dummy1[13];
    br_uint_16 m2;
    char dummy2[17];
    br_uint_16 m3;
    char dummy3[23];
    br_uint_16 m4;
    char dummy4[27];
} struct_br_uint_16;
static br_file_struct_member struct_br_uint_16_file_members[] = {
    { DF_TYPE_BR_UINT_16, offsetof(struct_br_uint_16, m1), "m1", NULL, },
    { DF_TYPE_BR_UINT_16, offsetof(struct_br_uint_16, m2), "m2", NULL, },
    { DF_TYPE_BR_UINT_16, offsetof(struct_br_uint_16, m3), "m3", NULL, },
    { DF_TYPE_BR_UINT_16, offsetof(struct_br_uint_16, m4), "m4", NULL, },
};
static br_file_struct struct_br_uint_16_file = {
    "struct_br_uint_16", BR_ASIZE(struct_br_uint_16_file_members), struct_br_uint_16_file_members, sizeof(struct_br_uint_16),
};
static struct_br_uint_16 br_uint_16_ref = { { 0 }, 65000, { 0 }, 255, { 0 }, 1000, { 0 }, 9000, { 0 }, };
static const uint8_t binary_br_uint_16_data[] = { BINARY_MAGICS, 0xfd, 0xe8, 0x00, 0xff, 0x03, 0xe8, 0x23, 0x28, };
static const char *text_br_uint_16_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_uint_16" HOST_NL
    "    uint_16   65000                                    # m1" HOST_NL
    "    uint_16   255                                      # m2" HOST_NL
    "    uint_16   1000                                     # m3" HOST_NL
    "    uint_16   9000                                     # m4" HOST_NL;
static void test_datafile_binary_br_uint_16() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_uint_16 read_struct;

    create_temp_file(tmpfilename, "br_uint_16_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(8, DfStructSizeBinary(df_w, &struct_br_uint_16_file, &br_uint_16_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_uint_16_file, &br_uint_16_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_uint_16_data, tmpfilename, sizeof(binary_br_uint_16_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_uint_16_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_uint_16_ref, &read_struct, sizeof(br_uint_16_ref));
}
static void test_datafile_text_br_uint_16() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_uint_16 read_struct;

    create_temp_file(tmpfilename, "br_uint_16_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_uint_16_file, &br_uint_16_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_uint_16_file, &br_uint_16_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_uint_16_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_uint_16_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_uint_16_ref, &read_struct, sizeof(br_uint_16_ref));
}

typedef struct struct_br_int_32 {
    char dummy0[11];
    br_int_32 m1;
    char dummy1[13];
    br_int_32 m2;
    char dummy2[17];
    br_int_32 m3;
    char dummy3[23];
    br_int_32 m4;
    char dummy4[27];
} struct_br_int_32;
static br_file_struct_member struct_br_int_32_file_members[] = {
    { DF_TYPE_BR_INT_32, offsetof(struct_br_int_32, m1), "m1", NULL, },
    { DF_TYPE_BR_INT_32, offsetof(struct_br_int_32, m2), "m2", NULL, },
    { DF_TYPE_BR_INT_32, offsetof(struct_br_int_32, m3), "m3", NULL, },
    { DF_TYPE_BR_INT_32, offsetof(struct_br_int_32, m4), "m4", NULL, },
};
static br_file_struct struct_br_int_32_file = {
    "struct_br_int_32", BR_ASIZE(struct_br_int_32_file_members), struct_br_int_32_file_members, sizeof(struct_br_int_32),
};
static struct_br_int_32 br_int_32_ref = { { 0 }, -2147483648, { 0 }, 2147483647, { 0 }, 255, { 0 }, -98765, { 0 }, };
static const uint8_t binary_br_int_32_data[] = { BINARY_MAGICS, 0x80, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xfe, 0x7e, 0x33, };
static const char *text_br_int_32_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_int_32" HOST_NL
    "    int_32    -2147483648                              # m1" HOST_NL
    "    int_32    2147483647                               # m2" HOST_NL
    "    int_32    255                                      # m3" HOST_NL
    "    int_32    -98765                                   # m4" HOST_NL;
static void test_datafile_binary_br_int_32() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_int_32 read_struct;

    create_temp_file(tmpfilename, "br_int_32_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(16, DfStructSizeBinary(df_w, &struct_br_int_32_file, &br_int_32_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_int_32_file, &br_int_32_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_int_32_data, tmpfilename, sizeof(binary_br_int_32_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_int_32_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_int_32_ref, &read_struct, sizeof(br_int_32_ref));
}
static void test_datafile_text_br_int_32() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_int_32 read_struct;

    create_temp_file(tmpfilename, "br_int_32_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_int_32_file, &br_int_32_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_int_32_file, &br_int_32_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_int_32_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_int_32_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_int_32_ref, &read_struct, sizeof(br_int_32_ref));
}

typedef struct struct_br_uint_32 {
    char dummy0[11];
    br_uint_32 m1;
    char dummy1[13];
    br_uint_32 m2;
    char dummy2[17];
    br_uint_32 m3;
    char dummy3[23];
    br_uint_32 m4;
    char dummy4[27];
} struct_br_uint_32;
static br_file_struct_member struct_br_uint_32_file_members[] = {
    { DF_TYPE_BR_UINT_32, offsetof(struct_br_uint_32, m1), "m1", NULL, },
    { DF_TYPE_BR_UINT_32, offsetof(struct_br_uint_32, m2), "m2", NULL, },
    { DF_TYPE_BR_UINT_32, offsetof(struct_br_uint_32, m3), "m3", NULL, },
    { DF_TYPE_BR_UINT_32, offsetof(struct_br_uint_32, m4), "m4", NULL, },
};
static br_file_struct struct_br_uint_32_file = {
    "struct_br_uint_32", BR_ASIZE(struct_br_uint_32_file_members), struct_br_uint_32_file_members, sizeof(struct_br_uint_32),
};
static struct_br_uint_32 br_uint_32_ref = { { 0 }, 0xffffffff, { 0 }, 1, { 0 }, 1000000, { 0 }, 90000, { 0 }, };
static const uint8_t binary_br_uint_32_data[] = { BINARY_MAGICS, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0f, 0x42, 0x40, 0x00, 0x01, 0x5f, 0x90, };
static const char *text_br_uint_32_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_uint_32" HOST_NL
    "    uint_32   4294967295                               # m1" HOST_NL
    "    uint_32   1                                        # m2" HOST_NL
    "    uint_32   1000000                                  # m3" HOST_NL
    "    uint_32   90000                                    # m4" HOST_NL;
static void test_datafile_binary_br_uint_32() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_uint_32 read_struct;

    create_temp_file(tmpfilename, "br_uint_32_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(16, DfStructSizeBinary(df_w, &struct_br_uint_32_file, &br_uint_32_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_uint_32_file, &br_uint_32_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_uint_32_data, tmpfilename, sizeof(binary_br_uint_32_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_uint_32_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_uint_32_ref, &read_struct, sizeof(br_uint_32_ref));
}
static void test_datafile_text_br_uint_32() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_uint_32 read_struct;

    create_temp_file(tmpfilename, "br_uint_32_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_uint_32_file, &br_uint_32_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_uint_32_file, &br_uint_32_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_uint_32_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_uint_32_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_uint_32_ref, &read_struct, sizeof(br_uint_32_ref));
}

typedef struct struct_br_fixed {
    char dummy0[11];
    br_fixed_ls m1;
    char dummy1[13];
    br_fixed_ls m2;
    char dummy2[17];
    br_fixed_ls m3;
    char dummy3[23];
    br_fixed_ls m4;
    char dummy4[27];
} struct_br_fixed;
static br_file_struct_member struct_br_fixed_file_members[] = {
    { DF_TYPE_BR_FIXED, offsetof(struct_br_fixed, m1), "m1", NULL, },
    { DF_TYPE_BR_FIXED, offsetof(struct_br_fixed, m2), "m2", NULL, },
    { DF_TYPE_BR_FIXED, offsetof(struct_br_fixed, m3), "m3", NULL, },
    { DF_TYPE_BR_FIXED, offsetof(struct_br_fixed, m4), "m4", NULL, },
};
static br_file_struct struct_br_fixed_file = {
    "struct_br_fixed", BR_ASIZE(struct_br_fixed_file_members), struct_br_fixed_file_members, sizeof(struct_br_fixed),
};
static struct_br_fixed br_fixed_ref = { { 0 }, 0x00008000, { 0 }, 0x00020000, { 0 }, 0x00101000, { 0 }, 0x01000000, { 0 }, };
static const uint8_t binary_br_fixed_data[] = { BINARY_MAGICS, 0x3f, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x41, 0x80, 0x80, 0x00, 0x43, 0x80, 0x00, 0x00, };
static const char *text_br_fixed_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_fixed" HOST_NL
    "    fixed     0.5                                      # m1" HOST_NL
    "    fixed     2                                        # m2" HOST_NL
    "    fixed     16.0625                                  # m3" HOST_NL
    "    fixed     256                                      # m4" HOST_NL;
static void test_datafile_binary_br_fixed() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_fixed read_struct;

    create_temp_file(tmpfilename, "br_fixed_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(16, DfStructSizeBinary(df_w, &struct_br_fixed_file, &br_fixed_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_fixed_file, &br_fixed_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_fixed_data, tmpfilename, sizeof(binary_br_fixed_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_fixed_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_INT32_WITHIN(BrFloatToFixed(1e-3), br_fixed_ref.m1, read_struct.m1);
    TEST_ASSERT_INT32_WITHIN(BrFloatToFixed(1e-3), br_fixed_ref.m2, read_struct.m2);
    TEST_ASSERT_INT32_WITHIN(BrFloatToFixed(1e-3), br_fixed_ref.m3, read_struct.m3);
    TEST_ASSERT_INT32_WITHIN(BrFloatToFixed(1e-3), br_fixed_ref.m4, read_struct.m4);
}
static void test_datafile_text_br_fixed() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_fixed read_struct;

    create_temp_file(tmpfilename, "br_fixed_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_fixed_file, &br_fixed_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_fixed_file, &br_fixed_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_fixed_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_fixed_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_fixed_ref, &read_struct, sizeof(br_fixed_ref));
}

typedef struct struct_br_angle {
    char dummy0[11];
    br_angle m1;
    char dummy1[13];
    br_angle m2;
    char dummy2[17];
    br_angle m3;
    char dummy3[23];
    br_angle m4;
    char dummy4[27];
} struct_br_angle;
static br_file_struct_member struct_br_angle_file_members[] = {
    { DF_TYPE_BR_ANGLE, offsetof(struct_br_angle, m1), "m1", NULL, },
    { DF_TYPE_BR_ANGLE, offsetof(struct_br_angle, m2), "m2", NULL, },
    { DF_TYPE_BR_ANGLE, offsetof(struct_br_angle, m3), "m3", NULL, },
    { DF_TYPE_BR_ANGLE, offsetof(struct_br_angle, m4), "m4", NULL, },
};
static br_file_struct struct_br_angle_file = {
    "struct_br_angle", BR_ASIZE(struct_br_angle_file_members), struct_br_angle_file_members, sizeof(struct_br_angle),
};
static struct_br_angle br_angle_ref = { { 0 }, 0x0080, { 0 }, 0x2000, { 0 }, 0x4000, { 0 }, 0xe000, { 0 }, };
static const uint8_t binary_br_angle_data[] = { BINARY_MAGICS, 0x00, 0x80, 0x20, 0x00, 0x40, 0x00, 0xe0, 0x00, };
static const char *text_br_angle_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_angle" HOST_NL
    "    angle     0.703125                                 # m1" HOST_NL
    "    angle     45                                       # m2" HOST_NL
    "    angle     90                                       # m3" HOST_NL
    "    angle     315                                      # m4" HOST_NL;
static void test_datafile_binary_br_angle() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_angle read_struct;

    create_temp_file(tmpfilename, "br_angle_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(8, DfStructSizeBinary(df_w, &struct_br_angle_file, &br_angle_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_angle_file, &br_angle_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_angle_data, tmpfilename, sizeof(binary_br_angle_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_angle_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_angle_ref, &read_struct, sizeof(br_angle_ref));
}
static void test_datafile_text_br_angle() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_angle read_struct;

    create_temp_file(tmpfilename, "br_angle_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_angle_file, &br_angle_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_angle_file, &br_angle_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_angle_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_angle_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_angle_ref, &read_struct, sizeof(br_angle_ref));
}

typedef struct struct_float {
    char dummy0[11];
    float m1;
    char dummy1[13];
    float m2;
    char dummy2[17];
    float m3;
    char dummy3[23];
    float m4;
    char dummy4[27];
} struct_float;
static br_file_struct_member struct_float_file_members[] = {
    { DF_TYPE_FLOAT, offsetof(struct_float, m1), "m1", NULL, },
    { DF_TYPE_FLOAT, offsetof(struct_float, m2), "m2", NULL, },
    { DF_TYPE_FLOAT, offsetof(struct_float, m3), "m3", NULL, },
    { DF_TYPE_FLOAT, offsetof(struct_float, m4), "m4", NULL, },
};
static br_file_struct struct_float_file = {
    "struct_float", BR_ASIZE(struct_float_file_members), struct_float_file_members, sizeof(struct_float),
};
static struct_float float_ref = { { 0 }, 0.5f, { 0 }, 256.f, { 0 }, 0.0625f, { 0 }, 3.14159f, { 0 }, };
static const uint8_t binary_float_data[] = { BINARY_MAGICS, 0x3f, 0x00, 0x00, 0x00, 0x43, 0x80, 0x00, 0x00, 0x3d, 0x80, 0x00, 0x00, 0x40, 0x49, 0x0f, 0xd0, };
static const char *text_float_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_float" HOST_NL
    "    float     0.5                                      # m1" HOST_NL
    "    float     256                                      # m2" HOST_NL
    "    float     0.0625                                   # m3" HOST_NL
    "    float     3.14159                                  # m4" HOST_NL;
static void test_datafile_binary_float() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_float read_struct;

    create_temp_file(tmpfilename, "float_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(16, DfStructSizeBinary(df_w, &struct_float_file, &float_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_float_file, &float_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_float_data, tmpfilename, sizeof(binary_float_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_float_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&float_ref, &read_struct, sizeof(float_ref));
}
static void test_datafile_text_float() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_float read_struct;

    create_temp_file(tmpfilename, "float_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_float_file, &float_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_float_file, &float_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_float_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_float_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&float_ref, &read_struct, sizeof(float_ref));
}

typedef struct struct_double {
    char dummy0[11];
    double m1;
    char dummy1[13];
    double m2;
    char dummy2[17];
    double m3;
    char dummy3[23];
    double m4;
    char dummy4[27];
} struct_double;
static br_file_struct_member struct_double_file_members[] = {
    { DF_TYPE_DOUBLE, offsetof(struct_double, m1), "m1", NULL, },
    { DF_TYPE_DOUBLE, offsetof(struct_double, m2), "m2", NULL, },
    { DF_TYPE_DOUBLE, offsetof(struct_double, m3), "m3", NULL, },
    { DF_TYPE_DOUBLE, offsetof(struct_double, m4), "m4", NULL, },
};
static br_file_struct struct_double_file = {
    "struct_double", BR_ASIZE(struct_double_file_members), struct_double_file_members, sizeof(struct_double),
};
static struct_double double_ref = { { 0 }, 0.5, { 0 }, 256., { 0 }, 0.0625, { 0 }, 3.14159, { 0 }, };
static const uint8_t binary_double_data[] = { BINARY_MAGICS, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x09, 0x21, 0xf9, 0xf0, 0x1b, 0x86, 0x6e, };
static const char *text_double_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_double" HOST_NL
    "    double    0.5                                      # m1" HOST_NL
    "    double    256                                      # m2" HOST_NL
    "    double    0.0625                                   # m3" HOST_NL
    "    double    3.14159                                  # m4" HOST_NL;
static void test_datafile_binary_double() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_double read_struct;

    create_temp_file(tmpfilename, "double_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(32, DfStructSizeBinary(df_w, &struct_double_file, &double_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_double_file, &double_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_double_data, tmpfilename, sizeof(binary_double_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_double_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&double_ref, &read_struct, sizeof(double_ref));
}
static void test_datafile_text_double() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_double read_struct;

    create_temp_file(tmpfilename, "double_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_double_file, &double_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_double_file, &double_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_double_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_double_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&double_ref, &read_struct, sizeof(double_ref));
}

enum {
    ENUM_8_TYPE1 = 0,
    ENUM_8_TYPE2 = 1,
    ENUM_8_TYPE3 = 2,
    ENUM_8_TYPE23 = ENUM_8_TYPE2 | ENUM_8_TYPE3,
    ENUM_8_TYPE4 = 0xff,
};
static br_file_enum_member file_test_enum_8_members[] = {
    { ENUM_8_TYPE1, "ENUM_8_TYPE1", },
    { ENUM_8_TYPE2, "ENUM_8_TYPE2", },
    { ENUM_8_TYPE3, "ENUM_8_TYPE3", },
    { ENUM_8_TYPE23, "ENUM_8_TYPE2|ENUM_8_TYPE3", },
    { ENUM_8_TYPE4, "ENUM_8_TYPE4", },
};
static br_file_enum file_test_enum_8 = {
    BR_ASIZE(file_test_enum_8_members),
    file_test_enum_8_members,
};
typedef struct struct_enum_8 {
    char dummy0[11];
    br_uint_8 m1;
    char dummy1[13];
    br_uint_8 m2;
    char dummy2[17];
    br_uint_8 m3;
    char dummy3[23];
    br_uint_8 m4;
    char dummy4[27];
} struct_enum_8;
static br_file_struct_member struct_enum_8_file_members[] = {
    { DF_TYPE_ENUM_8, offsetof(struct_enum_8, m1), "m1", &file_test_enum_8, },
    { DF_TYPE_ENUM_8, offsetof(struct_enum_8, m2), "m2", &file_test_enum_8, },
    { DF_TYPE_ENUM_8, offsetof(struct_enum_8, m3), "m3", &file_test_enum_8, },
    { DF_TYPE_ENUM_8, offsetof(struct_enum_8, m4), "m4", &file_test_enum_8, },
};
static br_file_struct struct_enum_8_file = {
    "struct_enum_8", BR_ASIZE(struct_enum_8_file_members), struct_enum_8_file_members, sizeof(struct_enum_8),
};
static struct_enum_8 enum_8_ref = { { 0 }, ENUM_8_TYPE1, { 0 }, ENUM_8_TYPE2, { 0 }, ENUM_8_TYPE23, { 0 }, ENUM_8_TYPE4, { 0 }, };
static const uint8_t binary_enum_8_data[] = { BINARY_MAGICS, 0x00, 0x01, 0x03, 0xff, };
static const char *text_enum_8_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_enum_8" HOST_NL
    "    enum_8    ENUM_8_TYPE1                             # m1" HOST_NL
    "    enum_8    ENUM_8_TYPE2                             # m2" HOST_NL
    "    enum_8    ENUM_8_TYPE2|ENUM_8_TYPE3                # m3" HOST_NL
    "    enum_8    ENUM_8_TYPE4                             # m4" HOST_NL;
static void test_datafile_binary_enum_8() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_enum_8 read_struct;

    create_temp_file(tmpfilename, "enum_8_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(4, DfStructSizeBinary(df_w, &struct_enum_8_file, &enum_8_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_enum_8_file, &enum_8_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_enum_8_data, tmpfilename, sizeof(binary_enum_8_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_enum_8_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&enum_8_ref, &read_struct, sizeof(enum_8_ref));
}
static void test_datafile_text_enum_8() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_enum_8 read_struct;

    create_temp_file(tmpfilename, "enum_8_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_enum_8_file, &enum_8_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_enum_8_file, &enum_8_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_enum_8_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_enum_8_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&enum_8_ref, &read_struct, sizeof(enum_8_ref));
}

enum {
    ENUM_16_TYPE1 = 0,
    ENUM_16_TYPE2 = 0x1000,
    ENUM_16_TYPE3 = 0x2000,
    ENUM_16_TYPE23 = ENUM_16_TYPE2 | ENUM_16_TYPE3,
    ENUM_16_TYPE4 = 0xffff,
};
static br_file_enum_member file_test_enum_16_members[] = {
    { ENUM_16_TYPE1, "ENUM_16_TYPE1", },
    { ENUM_16_TYPE2, "ENUM_16_TYPE2", },
    { ENUM_16_TYPE3, "TENUM_16_YPE3", },
    { ENUM_16_TYPE23, "ENUM_16_TYPE2|ENUM_16_TYPE3", },
    { ENUM_16_TYPE4, "ENUM_16_TYPE4", },
};
static br_file_enum file_test_enum_16 = {
    BR_ASIZE(file_test_enum_16_members),
    file_test_enum_16_members,
};
typedef struct struct_enum_16 {
    char dummy0[11];
    br_uint_16 m1;
    char dummy1[13];
    br_uint_16 m2;
    char dummy2[17];
    br_uint_16 m3;
    char dummy3[23];
    br_uint_16 m4;
    char dummy4[27];
} struct_enum_16;
static br_file_struct_member struct_enum_16_file_members[] = {
    { DF_TYPE_ENUM_16, offsetof(struct_enum_16, m1), "m1", &file_test_enum_16, },
    { DF_TYPE_ENUM_16, offsetof(struct_enum_16, m2), "m2", &file_test_enum_16, },
    { DF_TYPE_ENUM_16, offsetof(struct_enum_16, m3), "m3", &file_test_enum_16, },
    { DF_TYPE_ENUM_16, offsetof(struct_enum_16, m4), "m4", &file_test_enum_16, },
};
static br_file_struct struct_enum_16_file = {
    "struct_enum_16", BR_ASIZE(struct_enum_16_file_members), struct_enum_16_file_members, sizeof(struct_enum_16),
};
static struct_enum_16 enum_16_ref = { { 0 }, ENUM_16_TYPE1, { 0 }, ENUM_16_TYPE2, { 0 }, ENUM_16_TYPE23, { 0 }, ENUM_16_TYPE4, { 0 }, };
static const uint8_t binary_enum_16_data[] = { BINARY_MAGICS, 0x00, 0x00, 0x10, 0x00, 0x30, 0x00, 0xff, 0xff, };
static const char *text_enum_16_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_enum_16" HOST_NL
    "    enum_16   ENUM_16_TYPE1                            # m1" HOST_NL
    "    enum_16   ENUM_16_TYPE2                            # m2" HOST_NL
    "    enum_16   ENUM_16_TYPE2|ENUM_16_TYPE3              # m3" HOST_NL
    "    enum_16   ENUM_16_TYPE4                            # m4" HOST_NL;
static void test_datafile_binary_enum_16() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_enum_16 read_struct;

    create_temp_file(tmpfilename, "enum_16_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(8, DfStructSizeBinary(df_w, &struct_enum_16_file, &enum_16_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_enum_16_file, &enum_16_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_enum_16_data, tmpfilename, sizeof(binary_enum_16_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_enum_16_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&enum_16_ref, &read_struct, sizeof(enum_16_ref));
}
static void test_datafile_text_enum_16() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_enum_16 read_struct;

    create_temp_file(tmpfilename, "enum_16_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_enum_16_file, &enum_16_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_enum_16_file, &enum_16_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_enum_16_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_enum_16_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&enum_16_ref, &read_struct, sizeof(enum_16_ref));
}

enum {
    ENUM_32_TYPE1 = 0,
    ENUM_32_TYPE2 = 0x10000000,
    ENUM_32_TYPE3 = 0x20000000,
    ENUM_32_TYPE23 = ENUM_32_TYPE2 | ENUM_32_TYPE3,
    ENUM_32_TYPE4 = 0xffffffff,
};
static br_file_enum_member file_test_enum_32_members[] = {
    { ENUM_32_TYPE1, "ENUM_32_TYPE1", },
    { ENUM_32_TYPE2, "ENUM_32_TYPE2", },
    { ENUM_32_TYPE3, "TENUM_32_YPE3", },
    { ENUM_32_TYPE23, "ENUM_32_TYPE2|ENUM_32_TYPE3", },
    { ENUM_32_TYPE4, "ENUM_32_TYPE4", },
};
static br_file_enum file_test_enum_32 = {
    BR_ASIZE(file_test_enum_32_members),
    file_test_enum_32_members,
};
typedef struct struct_enum_32 {
    char dummy0[11];
    br_uint_32 m1;
    char dummy1[13];
    br_uint_32 m2;
    char dummy2[17];
    br_uint_32 m3;
    char dummy3[23];
    br_uint_32 m4;
    char dummy4[27];
} struct_enum_32;
static br_file_struct_member struct_enum_32_file_members[] = {
    { DF_TYPE_ENUM_32, offsetof(struct_enum_32, m1), "m1", &file_test_enum_32, },
    { DF_TYPE_ENUM_32, offsetof(struct_enum_32, m2), "m2", &file_test_enum_32, },
    { DF_TYPE_ENUM_32, offsetof(struct_enum_32, m3), "m3", &file_test_enum_32, },
    { DF_TYPE_ENUM_32, offsetof(struct_enum_32, m4), "m4", &file_test_enum_32, },
};
static br_file_struct struct_enum_32_file = {
    "struct_enum_32", BR_ASIZE(struct_enum_32_file_members), struct_enum_32_file_members, sizeof(struct_enum_32),
};
static struct_enum_32 enum_32_ref = { { 0 }, ENUM_32_TYPE1, { 0 }, ENUM_32_TYPE2, { 0 }, ENUM_32_TYPE23, { 0 }, ENUM_32_TYPE4, { 0 }, };
static const uint8_t binary_enum_32_data[] = { BINARY_MAGICS, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, };
static const char *text_enum_32_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_enum_32" HOST_NL
    "    enum_32   ENUM_32_TYPE1                            # m1" HOST_NL
    "    enum_32   ENUM_32_TYPE2                            # m2" HOST_NL
    "    enum_32   ENUM_32_TYPE2|ENUM_32_TYPE3              # m3" HOST_NL
    "    enum_32   ENUM_32_TYPE4                            # m4" HOST_NL;
static void test_datafile_binary_enum_32() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_enum_32 read_struct;

    create_temp_file(tmpfilename, "enum_32_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(16, DfStructSizeBinary(df_w, &struct_enum_32_file, &enum_32_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_enum_32_file, &enum_32_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_enum_32_data, tmpfilename, sizeof(binary_enum_32_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_enum_32_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&enum_32_ref, &read_struct, sizeof(enum_32_ref));
}
static void test_datafile_text_enum_32() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_enum_32 read_struct;

    create_temp_file(tmpfilename, "enum_32_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_enum_32_file, &enum_32_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_enum_32_file, &enum_32_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_enum_32_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_enum_32_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&enum_32_ref, &read_struct, sizeof(enum_32_ref));
}

typedef struct struct_asciz {
    char dummy0[11];
    char *m1;
    char dummy1[13];
    char *m2;
    char dummy2[17];
    char *m3;
    char dummy3[23];
    char *m4;
    char dummy4[27];
} struct_asciz;
static br_file_struct_member struct_asciz_file_members[] = {
    { DF_TYPE_ASCIZ, offsetof(struct_asciz, m1), "m1", NULL, },
    { DF_TYPE_ASCIZ, offsetof(struct_asciz, m2), "m2", NULL, },
    { DF_TYPE_ASCIZ, offsetof(struct_asciz, m3), "m3", NULL, },
    { DF_TYPE_ASCIZ, offsetof(struct_asciz, m4), "m4", NULL, },
};
static br_file_struct struct_asciz_file = {
    "struct_asciz", BR_ASIZE(struct_asciz_file_members), struct_asciz_file_members, sizeof(struct_asciz),
};
static struct_asciz asciz_ref = { { 0 }, "dethrace", { 0 }, NULL, { 0 }, "To be or not to be", { 0 }, "A very long string that is meant to test overspill", { 0 }, };
static const uint8_t binary_asciz_data[] = {
    BINARY_MAGICS,
    'd', 'e', 't', 'h', 'r', 'a', 'c', 'e', '\0', '\0', 'T', 'o', ' ', 'b', 'e', ' ', 'o', 'r', ' ', 'n',
    'o', 't', ' ', 't', 'o', ' ', 'b', 'e', '\0', 'A', ' ', 'v', 'e', 'r', 'y', ' ', 'l', 'o', 'n', 'g',
    ' ', 's', 't', 'r', 'i', 'n', 'g', ' ', 't', 'h', 'a', 't', ' ', 'i', 's', ' ', 'm', 'e', 'a', 'n',
    't', ' ', 't', 'o', ' ', 't', 'e', 's', 't', ' ', 'o', 'v', 'e', 'r', 's', 'p', 'i', 'l', 'l', '\0',
};
static const char *text_asciz_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_asciz" HOST_NL
    "    asciz     \"dethrace\"                               # m1" HOST_NL
    "    asciz     NULL                                     # m2" HOST_NL
    "    asciz     \"To be or not to be\"                     # m3" HOST_NL
    "    asciz     \"A very long string that is meant to test overspill\" # m4" HOST_NL;

static void test_datafile_binary_asciz() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_asciz read_struct;
    int sumStringLengths;

    create_temp_file(tmpfilename, "asciz_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    sumStringLengths = strlen(asciz_ref.m1) + strlen(asciz_ref.m3) + strlen(asciz_ref.m4);
    TEST_ASSERT_EQUAL_INT(sumStringLengths, DfStructSizeBinary(df_w, &struct_asciz_file, &asciz_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_asciz_file, &asciz_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_asciz_data, tmpfilename, sizeof(binary_asciz_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_asciz_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_STRING(asciz_ref.m1, read_struct.m1);
    TEST_ASSERT_EQUAL_STRING("", read_struct.m2);
    TEST_ASSERT_EQUAL_STRING(asciz_ref.m3, read_struct.m3);
    TEST_ASSERT_EQUAL_STRING(asciz_ref.m4, read_struct.m4);
}
static void test_datafile_text_asciz() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_asciz read_struct;

    create_temp_file(tmpfilename, "asciz_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_asciz_file, &asciz_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_asciz_file, &asciz_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_asciz_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_asciz_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_STRING(asciz_ref.m1, read_struct.m1);
    TEST_ASSERT_EQUAL_STRING(NULL, read_struct.m2);
    TEST_ASSERT_EQUAL_STRING(asciz_ref.m3, read_struct.m3);
    TEST_ASSERT_EQUAL_STRING(asciz_ref.m4, read_struct.m4);
}

typedef struct struct_br_colour {
    char dummy0[11];
    br_colour m1;
    char dummy1[13];
    br_colour m2;
    char dummy2[17];
    br_colour m3;
    char dummy3[23];
    br_colour m4;
    char dummy4[27];
} struct_br_colour;
static br_file_struct_member struct_br_colour_file_members[] = {
    { DF_TYPE_BR_COLOUR, offsetof(struct_br_colour, m1), "m1", NULL, },
    { DF_TYPE_BR_COLOUR, offsetof(struct_br_colour, m2), "m2", NULL, },
    { DF_TYPE_BR_COLOUR, offsetof(struct_br_colour, m3), "m3", NULL, },
    { DF_TYPE_BR_COLOUR, offsetof(struct_br_colour, m4), "m4", NULL, },
};
static br_file_struct struct_br_colour_file = {
    "struct_br_colour", BR_ASIZE(struct_br_colour_file_members), struct_br_colour_file_members, sizeof(struct_br_colour),
};
static struct_br_colour br_colour_ref = { { 0 }, 0xff0000, { 0 }, 0x00ff00, { 0 }, 0x0000ff, { 0 }, 0x4080c0, { 0 }, };
static const uint8_t binary_br_colour_data[] = { BINARY_MAGICS, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x40, 0x80, 0xc0, };
static const char *text_br_colour_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_colour" HOST_NL
    "    colour    255,0,0                                  # m1" HOST_NL
    "    colour    0,255,0                                  # m2" HOST_NL
    "    colour    0,0,255                                  # m3" HOST_NL
    "    colour    64,128,192                               # m4" HOST_NL;
static void test_datafile_binary_br_colour() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_colour read_struct;

    create_temp_file(tmpfilename, "br_colour_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(16, DfStructSizeBinary(df_w, &struct_br_colour_file, &br_colour_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_colour_file, &br_colour_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_colour_data, tmpfilename, sizeof(binary_br_colour_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_colour_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_colour_ref, &read_struct, sizeof(br_colour_ref));
}
static void test_datafile_text_br_colour() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_colour read_struct;

    create_temp_file(tmpfilename, "br_colour_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_colour_file, &br_colour_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_colour_file, &br_colour_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_colour_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_colour_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_colour_ref, &read_struct, sizeof(br_colour_ref));
}

typedef struct struct_br_fraction_x {
    char dummy0[11];
    br_fraction_x m1;
    char dummy1[13];
    br_fraction_x m2;
    char dummy2[17];
    br_fraction_x m3;
    char dummy3[23];
    br_fraction_x m4;
    char dummy4[27];
} struct_br_fraction_x;
static br_file_struct_member struct_br_fraction_x_file_members[] = {
    { DF_TYPE_BR_FRACTION_X, offsetof(struct_br_fraction_x, m1), "m1", NULL, },
    { DF_TYPE_BR_FRACTION_X, offsetof(struct_br_fraction_x, m2), "m2", NULL, },
    { DF_TYPE_BR_FRACTION_X, offsetof(struct_br_fraction_x, m3), "m3", NULL, },
    { DF_TYPE_BR_FRACTION_X, offsetof(struct_br_fraction_x, m4), "m4", NULL, },
};
static br_file_struct struct_br_fraction_x_file = {
    "struct_br_fraction_x", BR_ASIZE(struct_br_fraction_x_file_members), struct_br_fraction_x_file_members, sizeof(struct_br_fraction_x),
};
static struct_br_fraction_x br_fraction_x_ref = { { 0 }, 0x00008000, { 0 }, 0x00004000, { 0 }, 0x0000c000, { 0 }, 0x00000000, { 0 }, };
static const uint8_t binary_br_fraction_x_data[] = { BINARY_MAGICS, 0xbf, 0x80, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
static const char *text_br_fraction_x_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_fraction_x" HOST_NL
    "    fixed_fraction -1                                       # m1" HOST_NL
    "    fixed_fraction 0.5                                      # m2" HOST_NL
    "    fixed_fraction -0.5                                     # m3" HOST_NL
    "    fixed_fraction 0                                        # m4" HOST_NL;
static void test_datafile_binary_br_fraction_x() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_fraction_x read_struct;

    create_temp_file(tmpfilename, "br_fraction_x_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(16, DfStructSizeBinary(df_w, &struct_br_fraction_x_file, &br_fraction_x_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_fraction_x_file, &br_fraction_x_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_fraction_x_data, tmpfilename, sizeof(binary_br_fraction_x_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_fraction_x_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_INT16_WITHIN(BrFloatToFixed(1e-3), br_fraction_x_ref.m1, read_struct.m1);
    TEST_ASSERT_INT16_WITHIN(BrFloatToFixed(1e-3), br_fraction_x_ref.m2, read_struct.m2);
    TEST_ASSERT_INT16_WITHIN(BrFloatToFixed(1e-3), br_fraction_x_ref.m3, read_struct.m3);
    TEST_ASSERT_INT16_WITHIN(BrFloatToFixed(1e-3), br_fraction_x_ref.m4, read_struct.m4);
}
static void test_datafile_text_br_fraction_x() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_fraction_x read_struct;

    create_temp_file(tmpfilename, "br_fraction_x_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_fraction_x_file, &br_fraction_x_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_fraction_x_file, &br_fraction_x_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_fraction_x_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_fraction_x_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_fraction_x_ref, &read_struct, sizeof(br_fraction_x_ref));
}

typedef struct struct_br_ufraction_x {
    char dummy0[11];
    br_ufraction_x m1;
    char dummy1[13];
    br_ufraction_x m2;
    char dummy2[17];
    br_ufraction_x m3;
    char dummy3[23];
    br_ufraction_x m4;
    char dummy4[27];
} struct_br_ufraction_x;
static br_file_struct_member struct_br_ufraction_x_file_members[] = {
    { DF_TYPE_BR_UFRACTION_X, offsetof(struct_br_ufraction_x, m1), "m1", NULL, },
    { DF_TYPE_BR_UFRACTION_X, offsetof(struct_br_ufraction_x, m2), "m2", NULL, },
    { DF_TYPE_BR_UFRACTION_X, offsetof(struct_br_ufraction_x, m3), "m3", NULL, },
    { DF_TYPE_BR_UFRACTION_X, offsetof(struct_br_ufraction_x, m4), "m4", NULL, },
};
static br_file_struct struct_br_ufraction_x_file = {
    "struct_br_ufraction_x", BR_ASIZE(struct_br_ufraction_x_file_members), struct_br_ufraction_x_file_members, sizeof(struct_br_ufraction_x),
};
static struct_br_ufraction_x br_ufraction_x_ref = { { 0 }, 0x00008000, { 0 }, 0x00004000, { 0 }, 0x0000c000, { 0 }, 0x00000000, { 0 }, };
static const uint8_t binary_br_ufraction_x_data[] = { BINARY_MAGICS, 0x3f, 0x00, 0x00, 0x00, 0x3e, 0x80, 0x00, 0x00, 0x3f, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
static const char *text_br_ufraction_x_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_ufraction_x" HOST_NL
    "    fixed_ufraction 0.5                                      # m1" HOST_NL
    "    fixed_ufraction 0.25                                     # m2" HOST_NL
    "    fixed_ufraction 0.75                                     # m3" HOST_NL
    "    fixed_ufraction 0                                        # m4" HOST_NL;
static void test_datafile_binary_br_ufraction_x() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_ufraction_x read_struct;

    create_temp_file(tmpfilename, "br_ufraction_x_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(16, DfStructSizeBinary(df_w, &struct_br_ufraction_x_file, &br_ufraction_x_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_ufraction_x_file, &br_ufraction_x_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_ufraction_x_data, tmpfilename, sizeof(binary_br_ufraction_x_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_ufraction_x_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_INT16_WITHIN(BrFloatToFixed(1e-3), br_ufraction_x_ref.m1, read_struct.m1);
    TEST_ASSERT_INT16_WITHIN(BrFloatToFixed(1e-3), br_ufraction_x_ref.m2, read_struct.m2);
    TEST_ASSERT_INT16_WITHIN(BrFloatToFixed(1e-3), br_ufraction_x_ref.m3, read_struct.m3);
    TEST_ASSERT_INT16_WITHIN(BrFloatToFixed(1e-3), br_ufraction_x_ref.m4, read_struct.m4);
}
static void test_datafile_text_br_ufraction_x() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_ufraction_x read_struct;

    create_temp_file(tmpfilename, "br_ufraction_x_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_ufraction_x_file, &br_ufraction_x_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_ufraction_x_file, &br_ufraction_x_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_ufraction_x_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_ufraction_x_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_ufraction_x_ref, &read_struct, sizeof(br_ufraction_x_ref));
}

typedef struct struct_br_fraction_f {
    char dummy0[11];
    br_fraction_f m1;
    char dummy1[13];
    br_fraction_f m2;
    char dummy2[17];
    br_fraction_f m3;
    char dummy3[23];
    br_fraction_f m4;
    char dummy4[27];
} struct_br_fraction_f;
static br_file_struct_member struct_br_fraction_f_file_members[] = {
    { DF_TYPE_BR_FRACTION_F, offsetof(struct_br_fraction_f, m1), "m1", NULL, },
    { DF_TYPE_BR_FRACTION_F, offsetof(struct_br_fraction_f, m2), "m2", NULL, },
    { DF_TYPE_BR_FRACTION_F, offsetof(struct_br_fraction_f, m3), "m3", NULL, },
    { DF_TYPE_BR_FRACTION_F, offsetof(struct_br_fraction_f, m4), "m4", NULL, },
};
static br_file_struct struct_br_fraction_f_file = {
    "struct_br_fraction_f", BR_ASIZE(struct_br_fraction_f_file_members), struct_br_fraction_f_file_members, sizeof(struct_br_fraction_f),
};
static struct_br_fraction_f br_fraction_f_ref = { { 0 }, -1.f, { 0 }, 0.5f, { 0 }, -0.5f, { 0 }, 0.f, { 0 }, };
static const uint8_t binary_br_fraction_f_data[] = { BINARY_MAGICS, 0xbf, 0x80, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
static const char *text_br_fraction_f_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_fraction_f" HOST_NL
    "    float_fraction -1                                       # m1" HOST_NL
    "    float_fraction 0.5                                      # m2" HOST_NL
    "    float_fraction -0.5                                     # m3" HOST_NL
    "    float_fraction 0                                        # m4" HOST_NL;;
static void test_datafile_binary_br_fraction_f() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_fraction_f read_struct;

    create_temp_file(tmpfilename, "br_fraction_f_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(16, DfStructSizeBinary(df_w, &struct_br_fraction_f_file, &br_fraction_f_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_fraction_f_file, &br_fraction_f_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_fraction_f_data, tmpfilename, sizeof(binary_br_fraction_f_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_fraction_f_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_fraction_f_ref, &read_struct, sizeof(br_fraction_f_ref));
}
static void test_datafile_text_br_fraction_f() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_fraction_f read_struct;

    create_temp_file(tmpfilename, "br_fraction_f_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_fraction_f_file, &br_fraction_f_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_fraction_f_file, &br_fraction_f_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_fraction_f_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_fraction_f_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_fraction_f_ref, &read_struct, sizeof(br_fraction_f_ref));
}

typedef struct struct_br_ufraction_f {
    char dummy0[11];
    br_ufraction_f m1;
    char dummy1[13];
    br_ufraction_f m2;
    char dummy2[17];
    br_ufraction_f m3;
    char dummy3[23];
    br_ufraction_f m4;
    char dummy4[27];
} struct_br_ufraction_f;
static br_file_struct_member struct_br_ufraction_f_file_members[] = {
    { DF_TYPE_BR_UFRACTION_F, offsetof(struct_br_ufraction_f, m1), "m1", NULL, },
    { DF_TYPE_BR_UFRACTION_F, offsetof(struct_br_ufraction_f, m2), "m2", NULL, },
    { DF_TYPE_BR_UFRACTION_F, offsetof(struct_br_ufraction_f, m3), "m3", NULL, },
    { DF_TYPE_BR_UFRACTION_F, offsetof(struct_br_ufraction_f, m4), "m4", NULL, },
};
static br_file_struct struct_br_ufraction_f_file = {
    "struct_br_ufraction_f", BR_ASIZE(struct_br_ufraction_f_file_members), struct_br_ufraction_f_file_members, sizeof(struct_br_ufraction_f),
};
static struct_br_ufraction_f br_ufraction_f_ref = { { 0 }, 0.5, { 0 }, 0.25f, { 0 }, 0.75f, { 0 }, 0.f, { 0 }, };
static const uint8_t binary_br_ufraction_f_data[] = { BINARY_MAGICS, 0x3f, 0x00, 0x00, 0x00, 0x3e, 0x80, 0x00, 0x00, 0x3f, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
static const char *text_br_ufraction_f_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_ufraction_f" HOST_NL
    "    float_ufraction 0.5                                      # m1" HOST_NL
    "    float_ufraction 0.25                                     # m2" HOST_NL
    "    float_ufraction 0.75                                     # m3" HOST_NL
    "    float_ufraction 0                                        # m4" HOST_NL;;
static void test_datafile_binary_br_ufraction_f() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_ufraction_f read_struct;

    create_temp_file(tmpfilename, "br_ufraction_f_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(16, DfStructSizeBinary(df_w, &struct_br_ufraction_f_file, &br_ufraction_f_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_ufraction_f_file, &br_ufraction_f_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_ufraction_f_data, tmpfilename, sizeof(binary_br_ufraction_f_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_ufraction_f_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_ufraction_f_ref, &read_struct, sizeof(br_ufraction_f_ref));
}
static void test_datafile_text_br_ufraction_f() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_ufraction_f read_struct;

    create_temp_file(tmpfilename, "br_ufraction_f_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_ufraction_f_file, &br_ufraction_f_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_ufraction_f_file, &br_ufraction_f_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_ufraction_f_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_ufraction_f_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_ufraction_f_ref, &read_struct, sizeof(br_ufraction_f_ref));
}

typedef struct struct_br_vector2_x {
    char dummy0[11];
    br_vector2_x m1;
    char dummy1[13];
    br_vector2_x m2;
    char dummy2[17];
    br_vector2_x m3;
    char dummy3[23];
    br_vector2_x m4;
    char dummy4[27];
} struct_br_vector2_x;
static br_file_struct_member struct_br_vector2_x_file_members[] = {
    { DF_TYPE_BR_VECTOR2_X, offsetof(struct_br_vector2_x, m1), "m1", NULL, },
    { DF_TYPE_BR_VECTOR2_X, offsetof(struct_br_vector2_x, m2), "m2", NULL, },
    { DF_TYPE_BR_VECTOR2_X, offsetof(struct_br_vector2_x, m3), "m3", NULL, },
    { DF_TYPE_BR_VECTOR2_X, offsetof(struct_br_vector2_x, m4), "m4", NULL, },
};
static br_file_struct struct_br_vector2_x_file = {
    "struct_br_vector2_x", BR_ASIZE(struct_br_vector2_x_file_members), struct_br_vector2_x_file_members, sizeof(struct_br_vector2_x),
};
static struct_br_vector2_x br_vector2_x_ref = { { 0 }, {{0x00008000, 0x00004000}}, { 0 }, {{0x00020000, 0x00040000}}, { 0 }, {{0xffff0000, 0xfffe0000}}, { 0 }, {{0xffff8000, 0xffffc000}}, { 0 }, };
static const uint8_t binary_br_vector2_x_data[] = { BINARY_MAGICS, 0x3f, 0x00, 0x00, 0x00, 0x3e, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xbf, 0x00, 0x00, 0x00, 0xbe, 0x80, 0x00, 0x00, };
static const char *text_br_vector2_x_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_vector2_x" HOST_NL
    "    fixed_vector2 0.5,0.25                                 # m1" HOST_NL
    "    fixed_vector2 2,4                                      # m2" HOST_NL
    "    fixed_vector2 -1,-2                                    # m3" HOST_NL
    "    fixed_vector2 -0.5,-0.25                               # m4" HOST_NL;
static void test_datafile_binary_br_vector2_x() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_vector2_x read_struct;

    create_temp_file(tmpfilename, "br_vector2_x_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(32, DfStructSizeBinary(df_w, &struct_br_vector2_x_file, &br_vector2_x_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_vector2_x_file, &br_vector2_x_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_vector2_x_data, tmpfilename, sizeof(binary_br_vector2_x_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_vector2_x_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_vector2_x_ref, &read_struct, sizeof(br_vector2_x_ref));
}
static void test_datafile_text_br_vector2_x() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_vector2_x read_struct;

    create_temp_file(tmpfilename, "br_vector2_x_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_vector2_x_file, &br_vector2_x_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_vector2_x_file, &br_vector2_x_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_vector2_x_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_vector2_x_file, &read_struct);
    DfClose(df_r);

    printf("0x%08x 0x%08x   ", br_vector2_x_ref.m1.v[0], read_struct.m1.v[0]);printf("0x%08x 0x%08x\n", br_vector2_x_ref.m1.v[1], read_struct.m1.v[0]);
    printf("0x%08x 0x%08x   ", br_vector2_x_ref.m2.v[0], read_struct.m2.v[0]);printf("0x%08x 0x%08x\n", br_vector2_x_ref.m2.v[1], read_struct.m2.v[0]);
    printf("0x%08x 0x%08x   ", br_vector2_x_ref.m3.v[0], read_struct.m3.v[0]);printf("0x%08x 0x%08x\n", br_vector2_x_ref.m3.v[1], read_struct.m3.v[0]);
    printf("0x%08x 0x%08x   ", br_vector2_x_ref.m4.v[0], read_struct.m4.v[0]);printf("0x%08x 0x%08x\n", br_vector2_x_ref.m4.v[1], read_struct.m4.v[0]);
    TEST_ASSERT_EQUAL_MEMORY(&br_vector2_x_ref, &read_struct, sizeof(br_vector2_x_ref));
}

typedef struct struct_br_vector3_x {
    char dummy0[11];
    br_vector3_x m1;
    char dummy1[13];
    br_vector3_x m2;
    char dummy2[17];
    br_vector3_x m3;
    char dummy3[23];
    br_vector3_x m4;
    char dummy4[27];
} struct_br_vector3_x;
static br_file_struct_member struct_br_vector3_x_file_members[] = {
    { DF_TYPE_BR_VECTOR3_X, offsetof(struct_br_vector3_x, m1), "m1", NULL, },
    { DF_TYPE_BR_VECTOR3_X, offsetof(struct_br_vector3_x, m2), "m2", NULL, },
    { DF_TYPE_BR_VECTOR3_X, offsetof(struct_br_vector3_x, m3), "m3", NULL, },
    { DF_TYPE_BR_VECTOR3_X, offsetof(struct_br_vector3_x, m4), "m4", NULL, },
};
static br_file_struct struct_br_vector3_x_file = {
    "struct_br_vector3_x", BR_ASIZE(struct_br_vector3_x_file_members), struct_br_vector3_x_file_members, sizeof(struct_br_vector3_x),
};
static struct_br_vector3_x br_vector3_x_ref = { { 0 }, {{0x00008000, 0x00004000, 0x00002000}}, { 0 }, {{0x00020000, 0x00040000, 0x00080000}}, { 0 }, {{0xffff0000, 0xfffe0000, 0xfffc0000}}, { 0 }, {{0xffff8000, 0xffffc000, 0xffffe000}}, { 0 }, };
static const uint8_t binary_br_vector3_x_data[] = { BINARY_MAGICS, 0x3f, 0x00, 0x00, 0x00, 0x3e, 0x80, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0x80, 0x00, 0x00, 0xbf, 0x00, 0x00, 0x00, 0xbe, 0x80, 0x00, 0x00, 0xbe, 0x00, 0x00, 0x00, };
static const char *text_br_vector3_x_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_vector3_x" HOST_NL
    "    fixed_vector3 0.5,0.25,0.125                           # m1" HOST_NL
    "    fixed_vector3 2,4,8                                    # m2" HOST_NL
    "    fixed_vector3 -1,-2,-4                                 # m3" HOST_NL
    "    fixed_vector3 -0.5,-0.25,-0.125                        # m4" HOST_NL;
static void test_datafile_binary_br_vector3_x() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_vector3_x read_struct;

    create_temp_file(tmpfilename, "br_vector3_x_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(48, DfStructSizeBinary(df_w, &struct_br_vector3_x_file, &br_vector3_x_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_vector3_x_file, &br_vector3_x_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_vector3_x_data, tmpfilename, sizeof(binary_br_vector3_x_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_vector3_x_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_vector3_x_ref, &read_struct, sizeof(br_vector3_x_ref));
}
static void test_datafile_text_br_vector3_x() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_vector3_x read_struct;

    create_temp_file(tmpfilename, "br_vector3_x_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_vector3_x_file, &br_vector3_x_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_vector3_x_file, &br_vector3_x_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_vector3_x_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_vector3_x_file, &read_struct);
    DfClose(df_r);

    printf("0x%08x 0x%08x   ", br_vector3_x_ref.m1.v[0], read_struct.m1.v[0]);printf("0x%08x 0x%08x\n", br_vector3_x_ref.m1.v[1], read_struct.m1.v[0]);
    printf("0x%08x 0x%08x   ", br_vector3_x_ref.m2.v[0], read_struct.m2.v[0]);printf("0x%08x 0x%08x\n", br_vector3_x_ref.m2.v[1], read_struct.m2.v[0]);
    printf("0x%08x 0x%08x   ", br_vector3_x_ref.m3.v[0], read_struct.m3.v[0]);printf("0x%08x 0x%08x\n", br_vector3_x_ref.m3.v[1], read_struct.m3.v[0]);
    printf("0x%08x 0x%08x   ", br_vector3_x_ref.m4.v[0], read_struct.m4.v[0]);printf("0x%08x 0x%08x\n", br_vector3_x_ref.m4.v[1], read_struct.m4.v[0]);
    TEST_ASSERT_EQUAL_MEMORY(&br_vector3_x_ref, &read_struct, sizeof(br_vector3_x_ref));
}

typedef struct struct_br_vector4_x {
    char dummy0[11];
    br_vector4_x m1;
    char dummy1[13];
    br_vector4_x m2;
    char dummy2[17];
    br_vector4_x m3;
    char dummy3[23];
    br_vector4_x m4;
    char dummy4[27];
} struct_br_vector4_x;
static br_file_struct_member struct_br_vector4_x_file_members[] = {
    { DF_TYPE_BR_VECTOR4_X, offsetof(struct_br_vector4_x, m1), "m1", NULL, },
    { DF_TYPE_BR_VECTOR4_X, offsetof(struct_br_vector4_x, m2), "m2", NULL, },
    { DF_TYPE_BR_VECTOR4_X, offsetof(struct_br_vector4_x, m3), "m3", NULL, },
    { DF_TYPE_BR_VECTOR4_X, offsetof(struct_br_vector4_x, m4), "m4", NULL, },
};
static br_file_struct struct_br_vector4_x_file = {
    "struct_br_vector4_x", BR_ASIZE(struct_br_vector4_x_file_members), struct_br_vector4_x_file_members, sizeof(struct_br_vector4_x),
};
static struct_br_vector4_x br_vector4_x_ref = { { 0 }, {{0x00008000, 0x00004000, 0x00002000, 0x00001000}}, { 0 }, {{0x00020000, 0x00040000, 0x00080000, 0x00100000}}, { 0 }, {{0xffff0000, 0xfffe0000, 0xfffc0000, 0xfff80000}}, { 0 }, {{0xffff8000, 0xffffc000, 0xffffe000, 0xfffff000}}, { 0 }, };
static const uint8_t binary_br_vector4_x_data[] = { BINARY_MAGICS, 0x3f, 0x00, 0x00, 0x00, 0x3e, 0x80, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x3d, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x41, 0x80, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0x80, 0x00, 0x00, 0xc1, 0x00, 0x00, 0x00, 0xbf, 0x00, 0x00, 0x00, 0xbe, 0x80, 0x00, 0x00, 0xbe, 0x00, 0x00, 0x00, 0xbd, 0x80, 0x00, 0x00, };
static const char *text_br_vector4_x_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_vector4_x" HOST_NL
    "    fixed_vector4 0.5,0.25,0.125,0.0625                    # m1" HOST_NL
    "    fixed_vector4 2,4,8,16                                 # m2" HOST_NL
    "    fixed_vector4 -1,-2,-4,-8                              # m3" HOST_NL
    "    fixed_vector4 -0.5,-0.25,-0.125,-0.0625                # m4" HOST_NL;
static void test_datafile_binary_br_vector4_x() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_vector4_x read_struct;

    create_temp_file(tmpfilename, "br_vector4_x_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(64, DfStructSizeBinary(df_w, &struct_br_vector4_x_file, &br_vector4_x_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_vector4_x_file, &br_vector4_x_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_vector4_x_data, tmpfilename, sizeof(binary_br_vector4_x_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_vector4_x_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_vector4_x_ref, &read_struct, sizeof(br_vector4_x_ref));
}
static void test_datafile_text_br_vector4_x() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_vector4_x read_struct;

    create_temp_file(tmpfilename, "br_vector4_x_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_vector4_x_file, &br_vector4_x_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_vector4_x_file, &br_vector4_x_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_vector4_x_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_vector4_x_file, &read_struct);
    DfClose(df_r);

    printf("0x%08x 0x%08x   ", br_vector4_x_ref.m1.v[0], read_struct.m1.v[0]);printf("0x%08x 0x%08x\n", br_vector4_x_ref.m1.v[1], read_struct.m1.v[0]);
    printf("0x%08x 0x%08x   ", br_vector4_x_ref.m2.v[0], read_struct.m2.v[0]);printf("0x%08x 0x%08x\n", br_vector4_x_ref.m2.v[1], read_struct.m2.v[0]);
    printf("0x%08x 0x%08x   ", br_vector4_x_ref.m3.v[0], read_struct.m3.v[0]);printf("0x%08x 0x%08x\n", br_vector4_x_ref.m3.v[1], read_struct.m3.v[0]);
    printf("0x%08x 0x%08x   ", br_vector4_x_ref.m4.v[0], read_struct.m4.v[0]);printf("0x%08x 0x%08x\n", br_vector4_x_ref.m4.v[1], read_struct.m4.v[0]);
    TEST_ASSERT_EQUAL_MEMORY(&br_vector4_x_ref, &read_struct, sizeof(br_vector4_x_ref));
}

typedef struct struct_br_vector2_f {
    char dummy0[11];
    br_vector2_f m1;
    char dummy1[13];
    br_vector2_f m2;
    char dummy2[17];
    br_vector2_f m3;
    char dummy3[23];
    br_vector2_f m4;
    char dummy4[27];
} struct_br_vector2_f;
static br_file_struct_member struct_br_vector2_f_file_members[] = {
    { DF_TYPE_BR_VECTOR2_F, offsetof(struct_br_vector2_f, m1), "m1", NULL, },
    { DF_TYPE_BR_VECTOR2_F, offsetof(struct_br_vector2_f, m2), "m2", NULL, },
    { DF_TYPE_BR_VECTOR2_F, offsetof(struct_br_vector2_f, m3), "m3", NULL, },
    { DF_TYPE_BR_VECTOR2_F, offsetof(struct_br_vector2_f, m4), "m4", NULL, },
};
static br_file_struct struct_br_vector2_f_file = {
    "struct_br_vector2_f", BR_ASIZE(struct_br_vector2_f_file_members), struct_br_vector2_f_file_members, sizeof(struct_br_vector2_f),
};
static struct_br_vector2_f br_vector2_f_ref = { { 0 }, {{0.5f, 0.25f}}, { 0 }, {{2.f, 4.f}}, { 0 }, {{-1.f, -2.f}}, { 0 }, {{-0.5f, -0.25f}}, { 0 }, };
static const uint8_t binary_br_vector2_f_data[] = { BINARY_MAGICS, 0x3f, 0x00, 0x00, 0x00, 0x3e, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xbf, 0x00, 0x00, 0x00, 0xbe, 0x80, 0x00, 0x00, };
static const char *text_br_vector2_f_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_vector2_f" HOST_NL
    "    float_vector2 0.5,0.25                                 # m1" HOST_NL
    "    float_vector2 2,4                                      # m2" HOST_NL
    "    float_vector2 -1,-2                                    # m3" HOST_NL
    "    float_vector2 -0.5,-0.25                               # m4" HOST_NL;
static void test_datafile_binary_br_vector2_f() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_vector2_f read_struct;

    create_temp_file(tmpfilename, "br_vector2_f_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(32, DfStructSizeBinary(df_w, &struct_br_vector2_f_file, &br_vector2_f_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_vector2_f_file, &br_vector2_f_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_vector2_f_data, tmpfilename, sizeof(binary_br_vector2_f_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_vector2_f_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_vector2_f_ref, &read_struct, sizeof(br_vector2_f_ref));
}
static void test_datafile_text_br_vector2_f() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_vector2_f read_struct;

    create_temp_file(tmpfilename, "br_vector2_f_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_vector2_f_file, &br_vector2_f_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_vector2_f_file, &br_vector2_f_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_vector2_f_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_vector2_f_file, &read_struct);
    DfClose(df_r);

    printf("0x%08x 0x%08x   ", br_vector2_f_ref.m1.v[0], read_struct.m1.v[0]);printf("0x%08x 0x%08x\n", br_vector2_f_ref.m1.v[1], read_struct.m1.v[0]);
    printf("0x%08x 0x%08x   ", br_vector2_f_ref.m2.v[0], read_struct.m2.v[0]);printf("0x%08x 0x%08x\n", br_vector2_f_ref.m2.v[1], read_struct.m2.v[0]);
    printf("0x%08x 0x%08x   ", br_vector2_f_ref.m3.v[0], read_struct.m3.v[0]);printf("0x%08x 0x%08x\n", br_vector2_f_ref.m3.v[1], read_struct.m3.v[0]);
    printf("0x%08x 0x%08x   ", br_vector2_f_ref.m4.v[0], read_struct.m4.v[0]);printf("0x%08x 0x%08x\n", br_vector2_f_ref.m4.v[1], read_struct.m4.v[0]);
    TEST_ASSERT_EQUAL_MEMORY(&br_vector2_f_ref, &read_struct, sizeof(br_vector2_f_ref));
}

typedef struct struct_br_vector3_f {
    char dummy0[11];
    br_vector3_f m1;
    char dummy1[13];
    br_vector3_f m2;
    char dummy2[17];
    br_vector3_f m3;
    char dummy3[23];
    br_vector3_f m4;
    char dummy4[27];
} struct_br_vector3_f;
static br_file_struct_member struct_br_vector3_f_file_members[] = {
    { DF_TYPE_BR_VECTOR3_F, offsetof(struct_br_vector3_f, m1), "m1", NULL, },
    { DF_TYPE_BR_VECTOR3_F, offsetof(struct_br_vector3_f, m2), "m2", NULL, },
    { DF_TYPE_BR_VECTOR3_F, offsetof(struct_br_vector3_f, m3), "m3", NULL, },
    { DF_TYPE_BR_VECTOR3_F, offsetof(struct_br_vector3_f, m4), "m4", NULL, },
};
static br_file_struct struct_br_vector3_f_file = {
    "struct_br_vector3_f", BR_ASIZE(struct_br_vector3_f_file_members), struct_br_vector3_f_file_members, sizeof(struct_br_vector3_f),
};
static struct_br_vector3_f br_vector3_f_ref = { { 0 }, {{0.5f, 0.25f, 0.125f}}, { 0 }, {{2.f, 4.f, 8.f}}, { 0 }, {{-1.f, -2.f, -4.f}}, { 0 }, {{-0.5f, -0.25f, -0.125f}}, { 0 }, };
static const uint8_t binary_br_vector3_f_data[] = { BINARY_MAGICS, 0x3f, 0x00, 0x00, 0x00, 0x3e, 0x80, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0x80, 0x00, 0x00, 0xbf, 0x00, 0x00, 0x00, 0xbe, 0x80, 0x00, 0x00, 0xbe, 0x00, 0x00, 0x00, };
static const char *text_br_vector3_f_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_vector3_f" HOST_NL
    "    float_vector3 0.5,0.25,0.125                           # m1" HOST_NL
    "    float_vector3 2,4,8                                    # m2" HOST_NL
    "    float_vector3 -1,-2,-4                                 # m3" HOST_NL
    "    float_vector3 -0.5,-0.25,-0.125                        # m4" HOST_NL;
static void test_datafile_binary_br_vector3_f() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_vector3_f read_struct;

    create_temp_file(tmpfilename, "br_vector3_f_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(48, DfStructSizeBinary(df_w, &struct_br_vector3_f_file, &br_vector3_f_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_vector3_f_file, &br_vector3_f_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_vector3_f_data, tmpfilename, sizeof(binary_br_vector3_f_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_vector3_f_file, &read_struct);
    DfClose(df_r);

    printf("%f %f", br_vector3_f_ref.m1.v[0], read_struct.m1.v[0]); printf("%f %f", br_vector3_f_ref.m1.v[1], read_struct.m1.v[1]);

    TEST_ASSERT_EQUAL_MEMORY(&br_vector3_f_ref, &read_struct, sizeof(br_vector3_f_ref));
}
static void test_datafile_text_br_vector3_f() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_vector3_f read_struct;

    create_temp_file(tmpfilename, "br_vector3_f_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_vector3_f_file, &br_vector3_f_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_vector3_f_file, &br_vector3_f_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_vector3_f_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_vector3_f_file, &read_struct);
    DfClose(df_r);

    printf("0x%08x 0x%08x   ", br_vector3_f_ref.m1.v[0], read_struct.m1.v[0]);printf("0x%08x 0x%08x\n", br_vector3_f_ref.m1.v[1], read_struct.m1.v[0]);
    printf("0x%08x 0x%08x   ", br_vector3_f_ref.m2.v[0], read_struct.m2.v[0]);printf("0x%08x 0x%08x\n", br_vector3_f_ref.m2.v[1], read_struct.m2.v[0]);
    printf("0x%08x 0x%08x   ", br_vector3_f_ref.m3.v[0], read_struct.m3.v[0]);printf("0x%08x 0x%08x\n", br_vector3_f_ref.m3.v[1], read_struct.m3.v[0]);
    printf("0x%08x 0x%08x   ", br_vector3_f_ref.m4.v[0], read_struct.m4.v[0]);printf("0x%08x 0x%08x\n", br_vector3_f_ref.m4.v[1], read_struct.m4.v[0]);
    TEST_ASSERT_EQUAL_MEMORY(&br_vector3_f_ref, &read_struct, sizeof(br_vector3_f_ref));
}

typedef struct struct_br_vector4_f {
    char dummy0[11];
    br_vector4_f m1;
    char dummy1[13];
    br_vector4_f m2;
    char dummy2[17];
    br_vector4_f m3;
    char dummy3[23];
    br_vector4_f m4;
    char dummy4[27];
} struct_br_vector4_f;
static br_file_struct_member struct_br_vector4_f_file_members[] = {
    { DF_TYPE_BR_VECTOR4_F, offsetof(struct_br_vector4_f, m1), "m1", NULL, },
    { DF_TYPE_BR_VECTOR4_F, offsetof(struct_br_vector4_f, m2), "m2", NULL, },
    { DF_TYPE_BR_VECTOR4_F, offsetof(struct_br_vector4_f, m3), "m3", NULL, },
    { DF_TYPE_BR_VECTOR4_F, offsetof(struct_br_vector4_f, m4), "m4", NULL, },
};
static br_file_struct struct_br_vector4_f_file = {
    "struct_br_vector4_f", BR_ASIZE(struct_br_vector4_f_file_members), struct_br_vector4_f_file_members, sizeof(struct_br_vector4_f),
};
static struct_br_vector4_f br_vector4_f_ref = { { 0 }, {{0.5f, 0.25f, 0.125f, 0.0625f}}, { 0 }, {{2.f, 4.f, 8.f, 16.f}}, { 0 }, {{-1.f, -2.f, -4.f, -8.f}}, { 0 }, {{-0.5f, -0.25f, -0.125f, -0.0625f}}, { 0 }, };
static const uint8_t binary_br_vector4_f_data[] = { BINARY_MAGICS, 0x3f, 0x00, 0x00, 0x00, 0x3e, 0x80, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x3d, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x41, 0x80, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0x80, 0x00, 0x00, 0xc1, 0x00, 0x00, 0x00, 0xbf, 0x00, 0x00, 0x00, 0xbe, 0x80, 0x00, 0x00, 0xbe, 0x00, 0x00, 0x00, 0xbd, 0x80, 0x00, 0x00, };
static const char *text_br_vector4_f_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_br_vector4_f" HOST_NL
    "    float_vector4 0.5,0.25,0.125,0.0625                    # m1" HOST_NL
    "    float_vector4 2,4,8,16                                 # m2" HOST_NL
    "    float_vector4 -1,-2,-4,-8                              # m3" HOST_NL
    "    float_vector4 -0.5,-0.25,-0.125,-0.0625                # m4" HOST_NL;
static void test_datafile_binary_br_vector4_f() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_vector4_f read_struct;

    create_temp_file(tmpfilename, "br_vector4_f_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(64, DfStructSizeBinary(df_w, &struct_br_vector4_f_file, &br_vector4_f_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_br_vector4_f_file, &br_vector4_f_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_br_vector4_f_data, tmpfilename, sizeof(binary_br_vector4_f_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_br_vector4_f_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_vector4_f_ref, &read_struct, sizeof(br_vector4_f_ref));
}
static void test_datafile_text_br_vector4_f() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_br_vector4_f read_struct;

    create_temp_file(tmpfilename, "br_vector4_f_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(5, DfStructSizeText(df_w, &struct_br_vector4_f_file, &br_vector4_f_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_br_vector4_f_file, &br_vector4_f_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_br_vector4_f_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_br_vector4_f_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_MEMORY(&br_vector4_f_ref, &read_struct, sizeof(br_vector4_f_ref));
}

typedef struct {
    float float1;
    br_fixed_ls fixed1;
    br_uint_32 u32_1;
    char *s;
} struct_substruct1;
static br_file_struct_member struct_struct_substruct1_members[] = {
    { DF_TYPE_FLOAT, offsetof(struct_substruct1, float1), "float1", NULL, },
    { DF_TYPE_BR_FIXED, offsetof(struct_substruct1, fixed1), "fixed1", NULL, },
    { DF_TYPE_BR_UINT_32, offsetof(struct_substruct1, u32_1), "u32_1", NULL, },
};
static br_file_struct struct_substruct1_file = {
    "struct_substruct1", BR_ASIZE(struct_struct_substruct1_members), struct_struct_substruct1_members, sizeof(struct_substruct1),
};
typedef struct {
    br_vector3_f vf2;
    br_angle a2;
    br_uint_8 e8_2;
    struct_substruct1 s_2;
} struct_substruct2;
static br_file_struct_member struct_struct_substruct2_members[] = {
    { DF_TYPE_BR_VECTOR2_F, offsetof(struct_substruct2, vf2), "vf2", NULL, },
    { DF_TYPE_BR_ANGLE, offsetof(struct_substruct2, a2), "a2", NULL, },
    { DF_TYPE_ENUM_8, offsetof(struct_substruct2, e8_2), "e8_2", &file_test_enum_8, },
    { DF_TYPE_STRUCT, offsetof(struct_substruct2, s_2), "s_2", &struct_substruct1_file, },
};
static br_file_struct struct_substruct2_file = {
    "struct_substruct2", BR_ASIZE(struct_struct_substruct2_members), struct_struct_substruct2_members, sizeof(struct_substruct2),
};
typedef struct {
    char *s;
    struct_substruct1 sub1;
    struct_substruct2 sub2;
} struct_substruct;
static br_file_struct_member struct_struct_substruct_members[] = {
    { DF_TYPE_ASCIZ, offsetof(struct_substruct, s), "s", NULL, },
    { DF_TYPE_STRUCT, offsetof(struct_substruct, sub1), "sub1", &struct_substruct1_file, },
    { DF_TYPE_STRUCT, offsetof(struct_substruct, sub2), "sub2", &struct_substruct2_file, },
};
static br_file_struct struct_substruct_file = {
    "struct_substruct", BR_ASIZE(struct_struct_substruct_members), struct_struct_substruct_members, sizeof(struct_substruct),
};
static struct_substruct struct_substruct_ref = { "Hello world", {3.14f, 0x00144000, 0x12345678, }, { { 1.4f, -0.5f}, 0x2000, ENUM_8_TYPE23, { 2.71f, 0xffff8000, 0x10101010, }}};
static const uint8_t binary_struct_data[] = { BINARY_MAGICS, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x00, 0x40, 0x48, 0xf5, 0xc3, 0x41, 0xa2, 0x00, 0x00, 0x12, 0x34, 0x56, 0x78, 0x3f, 0xb3, 0x33, 0x33, 0xbf, 0x00, 0x00, 0x00, 0x20, 0x00, 0x03, 0x40, 0x2d, 0x70, 0xa4, 0xbf, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, };
static const char *text_struct_data =
    TEXT_MAGICS HOST_NL
    "  struct    struct_substruct" HOST_NL
    "    asciz     \"Hello world\"                            # s" HOST_NL
    "    struct    struct_substruct1                        # sub1" HOST_NL
    "      float     3.14                                     # float1" HOST_NL
    "      fixed     20.25                                    # fixed1" HOST_NL
    "      uint_32   305419896                                # u32_1" HOST_NL
    "    struct    struct_substruct2                        # sub2" HOST_NL
    "      float_vector2 1.4,-0.5                                 # vf2" HOST_NL
    "      angle     45                                       # a2" HOST_NL
    "      enum_8    ENUM_8_TYPE2|ENUM_8_TYPE3                # e8_2" HOST_NL
    "      struct    struct_substruct1                        # s_2" HOST_NL
    "        float     2.71                                     # float1" HOST_NL
    "        fixed     -0.5                                     # fixed1" HOST_NL
    "        uint_32   269488144                                # u32_1" HOST_NL;
static void test_datafile_binary_substruct() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_substruct read_struct;

    create_temp_file(tmpfilename, "struct_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(46, DfStructSizeBinary(df_w, &struct_substruct_file, &struct_substruct_ref));

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfStructWriteBinary(df_w, &struct_substruct_file, &struct_substruct_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_struct_data, tmpfilename, sizeof(binary_struct_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfStructReadBinary(df_r, &struct_substruct_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_STRING(struct_substruct_ref.s, read_struct.s);
    TEST_ASSERT_EQUAL_MEMORY(&struct_substruct_ref.sub1, &read_struct.sub1, sizeof(struct_substruct_ref.sub1));
    TEST_ASSERT_EQUAL_MEMORY(&struct_substruct_ref.sub2, &read_struct.sub2, sizeof(struct_substruct_ref.sub2));
}
static void test_datafile_text_substruct() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    struct_substruct read_struct;

    create_temp_file(tmpfilename, "struct_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);
    TEST_ASSERT_EQUAL_INT(14, DfStructSizeText(df_w, &struct_substruct_file, &struct_substruct_ref));

    BrFilePutLine(text_magics, df_w->h);
    DfStructWriteText(df_w, &struct_substruct_file, &struct_substruct_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_struct_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    memset(&read_struct, 0, sizeof(read_struct));
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfStructReadText(df_r, &struct_substruct_file, &read_struct);
    DfClose(df_r);

    TEST_ASSERT_EQUAL_STRING(struct_substruct_ref.s, read_struct.s);
    TEST_ASSERT_EQUAL_MEMORY(&struct_substruct_ref.sub1, &read_struct.sub1, sizeof(struct_substruct_ref.sub1));
    TEST_ASSERT_EQUAL_MEMORY(&struct_substruct_ref.sub2, &read_struct.sub2, sizeof(struct_substruct_ref.sub2));
}

static const br_uint_32 chunk_id_ref = DF_CHUNKID_PIXELMAP;
static const br_uint_32 chunk_length_ref = 0x100;
static const uint8_t binary_chunk_data[] = { BINARY_MAGICS, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x01, 0x00, };
static const char *text_chunk_data =
    TEXT_MAGICS HOST_NL
    "*PIXELMAP_OLD     256" HOST_NL;
static void test_datafile_binary_chunk() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    int read_id;
    br_uint_32 read_length;

    create_temp_file(tmpfilename, "chunk_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfChunkWriteBinary(df_w, chunk_id_ref, chunk_length_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_chunk_data, tmpfilename, sizeof(binary_chunk_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    BrFileAdvance(sizeof(binary_magics), df_r->h);
    read_length = 0;
    read_id = DfChunkReadBinary(df_r, &read_length);

    TEST_ASSERT_EQUAL_UINT32(chunk_id_ref, read_id);
    TEST_ASSERT_EQUAL_INT(chunk_length_ref, read_length);

    DfClose(df_r);
}
static void test_datafile_text_chunk() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    int read_id;
    br_uint_32 read_length;

    create_temp_file(tmpfilename, "chunk_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);

    BrFilePutLine(text_magics, df_w->h);
    DfChunkWriteText(df_w, chunk_id_ref, chunk_length_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_chunk_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    BrFileAdvance(strlen(text_magics), df_r->h);
    read_length = 0;
    read_id = DfChunkReadText(df_r, &read_length);

    TEST_ASSERT_EQUAL_UINT32(chunk_id_ref, read_id);
    TEST_ASSERT_EQUAL_INT(chunk_length_ref, read_length);

    DfClose(df_r);
}

static const br_uint_32 chunk_unknown_id_ref = 0x9000;
static const br_uint_32 chunk_unknown_length_ref = 0x123456;
static const uint8_t binary_chunk_unknown_data[] = { BINARY_MAGICS, 0x00, 0x00, 0x90, 0x00, 0x00, 0x12, 0x34, 0x56, };
static const char *text_chunk_unknown_data =
    TEXT_MAGICS HOST_NL
    "*0x00009000 1193046" HOST_NL;
static void test_datafile_binary_chunk_unknown() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    int read_id;
    br_uint_32 read_length;

    create_temp_file(tmpfilename, "chunk_unknown_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfChunkWriteBinary(df_w, chunk_unknown_id_ref, chunk_unknown_length_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_chunk_unknown_data, tmpfilename, sizeof(binary_chunk_unknown_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    BrFileAdvance(sizeof(binary_magics), df_r->h);
    read_length = 0;
    read_id = DfChunkReadBinary(df_r, &read_length);

    TEST_ASSERT_EQUAL_UINT32(chunk_unknown_id_ref, read_id);
    TEST_ASSERT_EQUAL_INT(chunk_unknown_length_ref, read_length);

    DfClose(df_r);
}
static void test_datafile_text_chunk_unknown() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    int read_id;
    br_uint_32 read_length;

    create_temp_file(tmpfilename, "chunk_unknown_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);

    BrFilePutLine(text_magics, df_w->h);
    DfChunkWriteText(df_w, chunk_unknown_id_ref, chunk_unknown_length_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_chunk_unknown_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    BrFileAdvance(strlen(text_magics), df_r->h);
    read_length = 0;
    read_id = DfChunkReadText(df_r, &read_length);

    TEST_ASSERT_EQUAL_UINT32(chunk_unknown_id_ref, read_id);
    TEST_ASSERT_EQUAL_INT(chunk_unknown_length_ref, read_length);

    DfClose(df_r);
}

static const br_uint_32 count_ref = 0x1234567;
static const uint8_t binary_count_data[] = { BINARY_MAGICS, 0x01, 0x23, 0x45, 0x67, };
static const char *text_count_data =
    TEXT_MAGICS HOST_NL
    " count 19088743" HOST_NL;
static void test_datafile_binary_count() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    br_uint_32 read_count;

    create_temp_file(tmpfilename, "count_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_EQUAL_INT(4, DfCountSizeBinary(df_w));

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfCountWriteBinary(df_w, count_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_count_data, tmpfilename, sizeof(binary_count_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    BrFileAdvance(sizeof(binary_magics), df_r->h);
    read_count = DfCountReadBinary(df_r);

    TEST_ASSERT_EQUAL_UINT32(count_ref, read_count);

    DfClose(df_r);
}
static void test_datafile_text_count() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    br_uint_32 read_count;

    create_temp_file(tmpfilename, "count_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_EQUAL_INT(1, DfCountSizeText(df_w));

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);

    BrFilePutLine(text_magics, df_w->h);
    DfCountWriteText(df_w, count_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_count_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    BrFileAdvance(strlen(text_magics), df_r->h);
    read_count = DfCountReadText(df_r);

    TEST_ASSERT_EQUAL_UINT32(count_ref, read_count);

    DfClose(df_r);
}

static char* name_ref = "DETHRACE_NAME";
static const uint8_t binary_name_data[] = { BINARY_MAGICS, 'D', 'E', 'T', 'H', 'R', 'A','C', 'E', '_', 'N', 'A', 'M', 'E', '\0', };
static const char *text_name_data =
    TEXT_MAGICS HOST_NL
    "  name \"DETHRACE_NAME\"" HOST_NL;
static void test_datafile_binary_name() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    char nameBuffer[256];

    create_temp_file(tmpfilename, "name_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_EQUAL_INT(strlen(name_ref) + 1, DfNameSizeBinary(df_w, name_ref));

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfNameWriteBinary(df_w, name_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_name_data, tmpfilename, sizeof(binary_name_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    nameBuffer[0] = '\0';
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    DfNameReadBinary(df_r, nameBuffer);

    TEST_ASSERT_EQUAL_STRING(name_ref, nameBuffer);;

    DfClose(df_r);
}
static void test_datafile_text_name() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    char nameBuffer[256];

    create_temp_file(tmpfilename, "name_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_EQUAL_INT(1, DfNameSizeText(df_w, name_ref));

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);

    BrFilePutLine(text_magics, df_w->h);
    DfNameWriteText(df_w, name_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_name_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    nameBuffer[0] = '\0';
    BrFileAdvance(strlen(text_magics), df_r->h);
    DfNameReadText(df_r, nameBuffer);
   
    TEST_ASSERT_EQUAL_STRING(name_ref, nameBuffer);

    DfClose(df_r);
}

static const int block_continguous_block_size_ref = 8;
static const int block_continguous_block_stride_ref = 8;
static const int block_continguous_block_count_ref = 7;
static const int block_continguous_size_ref = 2;
static br_uint_16 block_continguous_ref[7][8] = {
    { 0x4fd2, 0x73c4, 0xdb2b, 0x81d7, 0xa39f, 0x4b0e, 0x9238, 0xa263, },
    { 0x5ca8, 0xa44b, 0x6ef6, 0x5670, 0x7645, 0x78a9, 0x2ade, 0x7e1b, },
    { 0x846a, 0x96b3, 0x6c19, 0xeed2, 0x0d5c, 0x1339, 0x7ab1, 0x8844, },
    { 0xb820, 0xa176, 0xf77a, 0xc1cb, 0x6098, 0xfb5a, 0x1793, 0xcc1f, },
    { 0x4c24, 0x86bd, 0xf802, 0x6517, 0x2a97, 0x819c, 0x9ccf, 0x57e2, },
    { 0x6d30, 0xda19, 0xfbfb, 0xf64e, 0x7867, 0x4693, 0xcfb5, 0x1960, },
    { 0xd4a5, 0x3514, 0x0158, 0xe78e, 0xe109, 0xcf80, 0x5394, 0xe367, },
};
static const uint8_t binary_block_continguous_data[] = { BINARY_MAGICS,
    0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x02,
    0x4f, 0xd2, 0x73, 0xc4, 0xdb, 0x2b, 0x81, 0xd7, 0xa3, 0x9f, 0x4b, 0x0e, 0x92, 0x38, 0xa2, 0x63,
    0x5c, 0xa8, 0xa4, 0x4b, 0x6e, 0xf6, 0x56, 0x70, 0x76, 0x45, 0x78, 0xa9, 0x2a, 0xde, 0x7e, 0x1b,
    0x84, 0x6a, 0x96, 0xb3, 0x6c, 0x19, 0xee, 0xd2, 0x0d, 0x5c, 0x13, 0x39, 0x7a, 0xb1, 0x88, 0x44,
    0xb8, 0x20, 0xa1, 0x76, 0xf7, 0x7a, 0xc1, 0xcb, 0x60, 0x98, 0xfb, 0x5a, 0x17, 0x93, 0xcc, 0x1f,
    0x4c, 0x24, 0x86, 0xbd, 0xf8, 0x02, 0x65, 0x17, 0x2a, 0x97, 0x81, 0x9c, 0x9c, 0xcf, 0x57, 0xe2,
    0x6d, 0x30, 0xda, 0x19, 0xfb, 0xfb, 0xf6, 0x4e, 0x78, 0x67, 0x46, 0x93, 0xcf, 0xb5, 0x19, 0x60,
    0xd4, 0xa5, 0x35, 0x14, 0x01, 0x58, 0xe7, 0x8e, 0xe1, 0x09, 0xcf, 0x80, 0x53, 0x94, 0xe3, 0x67,
};
static const char *text_block_continguous_data =
    TEXT_MAGICS HOST_NL
    "  block 56" HOST_NL
    "  size 2" HOST_NL
    "    00000000: 4fd273c4db2b81d7a39f4b0e9238a2635ca8a44b6ef65670764578a92ade7e1b" HOST_NL
    "    00000020: 846a96b36c19eed20d5c13397ab18844b820a176f77ac1cb6098fb5a1793cc1f" HOST_NL
    "    00000040: 4c2486bdf80265172a97819c9ccf57e26d30da19fbfbf64e78674693cfb51960" HOST_NL
    "    00000060: d4a535140158e78ee109cf805394e367" HOST_NL;
static void test_datafile_binary_block_continguous() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    br_uint_32 read_count;
    br_uint_16 read_block_continguous[20][8];
    br_uint_8 *read_blocks;

    create_temp_file(tmpfilename, "block_continguous_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_EQUAL_INT(120, DfBlockSizeBinary(df_w, block_continguous_ref, block_continguous_block_size_ref, block_continguous_block_stride_ref, block_continguous_block_count_ref, block_continguous_size_ref));

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfBlockWriteBinary(df_w, block_continguous_ref, block_continguous_block_size_ref, block_continguous_block_stride_ref, block_continguous_block_count_ref, block_continguous_size_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_block_continguous_data, tmpfilename, sizeof(binary_block_continguous_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    read_count = sizeof(read_block_continguous) / sizeof(br_uint_16);
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    read_blocks = DfBlockReadBinary(df_r, read_block_continguous, &read_count, block_continguous_size_ref, BR_MEMORY_APPLICATION);

    TEST_ASSERT_EQUAL_PTR(read_block_continguous, read_blocks);
    TEST_ASSERT_EQUAL_INT(block_continguous_block_size_ref * block_continguous_block_count_ref, read_count);
    TEST_ASSERT_EQUAL_MEMORY(block_continguous_ref, read_block_continguous, sizeof(block_continguous_ref));

    DfClose(df_r);
}
static void test_datafile_text_block_continguous() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    br_uint_32 read_count;
    br_uint_16 read_block_continguous[20][8];
    br_uint_8 *read_blocks;

    create_temp_file(tmpfilename, "block_continguous_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_EQUAL_INT(6, DfBlockSizeText(df_w, block_continguous_ref, block_continguous_block_size_ref, block_continguous_block_stride_ref, block_continguous_block_count_ref, block_continguous_size_ref));

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);

    BrFilePutLine(text_magics, df_w->h);
    DfBlockWriteText(df_w, block_continguous_ref, block_continguous_block_size_ref, block_continguous_block_stride_ref, block_continguous_block_count_ref, block_continguous_size_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_block_continguous_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    read_count = sizeof(read_block_continguous) / sizeof(br_uint_16);
    BrFileAdvance(strlen(text_magics), df_r->h);
    read_blocks = DfBlockReadText(df_r, read_block_continguous, &read_count, block_continguous_size_ref, BR_MEMORY_APPLICATION);

    TEST_ASSERT_EQUAL_PTR(read_block_continguous, read_blocks);
    TEST_ASSERT_EQUAL_INT(block_continguous_block_size_ref * block_continguous_block_count_ref, read_count);
    TEST_ASSERT_EQUAL_MEMORY(block_continguous_ref, read_block_continguous, sizeof(block_continguous_ref));

    DfClose(df_r);
}

static const int block_striped_block_size_ref = 8;
    static const int block_striped_block_stride_ref = 16;
static const int block_striped_block_count_ref = 3;
static const int block_striped_size_ref = 2;
static br_uint_16 block_striped_input_ref[6][8] = {
    { 0x4fd2, 0x73c4, 0xdb2b, 0x81d7, 0xa39f, 0x4b0e, 0x9238, 0xa263, },
    { 0x5ca8, 0xa44b, 0x6ef6, 0x5670, 0x7645, 0x78a9, 0x2ade, 0x7e1b, },
    { 0x846a, 0x96b3, 0x6c19, 0xeed2, 0x0d5c, 0x1339, 0x7ab1, 0x8844, },
    { 0xb820, 0xa176, 0xf77a, 0xc1cb, 0x6098, 0xfb5a, 0x1793, 0xcc1f, },
    { 0x4c24, 0x86bd, 0xf802, 0x6517, 0x2a97, 0x819c, 0x9ccf, 0x57e2, },
    { 0x6d30, 0xda19, 0xfbfb, 0xf64e, 0x7867, 0x4693, 0xcfb5, 0x1960, },
};
static br_uint_16 block_striped_output_ref[3][8] = {
    { 0x4fd2, 0x73c4, 0xdb2b, 0x81d7, 0xa39f, 0x4b0e, 0x9238, 0xa263, },
    { 0x846a, 0x96b3, 0x6c19, 0xeed2, 0x0d5c, 0x1339, 0x7ab1, 0x8844, },
    { 0x4c24, 0x86bd, 0xf802, 0x6517, 0x2a97, 0x819c, 0x9ccf, 0x57e2, },
};
static const uint8_t binary_block_striped_data[] = { BINARY_MAGICS,
    0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x02,
    0x4f, 0xd2, 0x73, 0xc4, 0xdb, 0x2b, 0x81, 0xd7, 0xa3, 0x9f, 0x4b, 0x0e, 0x92, 0x38, 0xa2, 0x63,
    0x84, 0x6a, 0x96, 0xb3, 0x6c, 0x19, 0xee, 0xd2, 0x0d, 0x5c, 0x13, 0x39, 0x7a, 0xb1, 0x88, 0x44,
    0x4c, 0x24, 0x86, 0xbd, 0xf8, 0x02, 0x65, 0x17, 0x2a, 0x97, 0x81, 0x9c, 0x9c, 0xcf, 0x57, 0xe2,
};
static const char *text_block_striped_data =
    TEXT_MAGICS HOST_NL
    "  block 24" HOST_NL
    "  size 2" HOST_NL
    "    00000000: 4fd273c4db2b81d7a39f4b0e9238a263846a96b36c19eed20d5c13397ab18844" HOST_NL
    "    00000020: 4c2486bdf80265172a97819c9ccf57e2" HOST_NL;
static void test_datafile_binary_block_striped() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    br_uint_32 read_count;
    br_uint_16 read_block_striped[20][8];
    br_uint_8 *read_blocks;

    create_temp_file(tmpfilename, "block_striped_binary");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_EQUAL_INT(56, DfBlockSizeBinary(df_w, block_striped_input_ref, block_striped_block_size_ref, block_striped_block_stride_ref, block_striped_block_count_ref, block_striped_size_ref));

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);

    BrFileWrite(binary_magics, sizeof(binary_magics), 1, df_w->h);
    DfBlockWriteBinary(df_w, block_striped_input_ref, block_striped_block_size_ref, block_striped_block_stride_ref, block_striped_block_count_ref, block_striped_size_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(binary_block_striped_data, tmpfilename, sizeof(binary_block_striped_data));

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    read_count = sizeof(read_block_striped) / sizeof(br_uint_16);
    BrFileAdvance(sizeof(binary_magics), df_r->h);
    read_blocks = DfBlockReadBinary(df_r, read_block_striped, &read_count, block_striped_size_ref, BR_MEMORY_APPLICATION);

    TEST_ASSERT_EQUAL_PTR(read_block_striped, read_blocks);
    TEST_ASSERT_EQUAL_INT(block_striped_block_size_ref * block_striped_block_count_ref, read_count);
    TEST_ASSERT_EQUAL_MEMORY(block_striped_output_ref, read_block_striped, sizeof(block_striped_output_ref));

    DfClose(df_r);
}
static void test_datafile_text_block_striped() {
    br_datafile* df_w;
    br_datafile* df_r;
    char tmpfilename[PATH_MAX+1];
    br_uint_32 read_count;
    br_uint_16 read_block_striped[20][8];
    br_uint_8 *read_blocks;

    create_temp_file(tmpfilename, "block_striped_text");

    df_w = DfOpen(tmpfilename, 1, BRT_FLOAT);

    TEST_ASSERT_EQUAL_INT(4, DfBlockSizeText(df_w, block_striped_input_ref, block_striped_block_size_ref, block_striped_block_stride_ref, block_striped_block_count_ref, block_striped_size_ref));

    TEST_ASSERT_NOT_NULL(df_w);
    TEST_ASSERT_NOT_NULL(df_w->h);

    BrFilePutLine(text_magics, df_w->h);
    DfBlockWriteText(df_w, block_striped_input_ref, block_striped_block_size_ref, block_striped_block_stride_ref, block_striped_block_count_ref, block_striped_size_ref);
    DfClose(df_w);

    TEST_ASSERT_EQUAL_FILE_TEXT(text_block_striped_data, tmpfilename);

    df_r = DfOpen(tmpfilename, 0, BRT_FLOAT);

    TEST_ASSERT_NOT_NULL(df_r);
    TEST_ASSERT_NOT_NULL(df_r->h);

    read_count = sizeof(read_block_striped) / sizeof(br_uint_16);
    BrFileAdvance(strlen(text_magics), df_r->h);
    read_blocks = DfBlockReadText(df_r, read_block_striped, &read_count, block_striped_size_ref, BR_MEMORY_APPLICATION);

    TEST_ASSERT_EQUAL_PTR(read_block_striped, read_blocks);
    TEST_ASSERT_EQUAL_INT(block_striped_block_size_ref * block_striped_block_count_ref, read_count);
    TEST_ASSERT_EQUAL_MEMORY(block_striped_output_ref, read_block_striped, sizeof(block_striped_output_ref));

    DfClose(df_r);
}

static void test_datafile_OpenFile() {
    br_datafile* df;

    REQUIRES_DATA_DIRECTORY();
    df = DfOpen("DATA/MODELS/CPOINT.DAT", 0, BRT_FLOAT);
    TEST_ASSERT_NOT_NULL(df);
    TEST_ASSERT_NOT_NULL(df->h);
    DfClose(df);
}

void test_datafile_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_datafile_stack);
    RUN_TEST(test_datafile_magics);
    RUN_TEST(test_datafile_binary_br_int_8);
    RUN_TEST(test_datafile_text_br_int_8);
    RUN_TEST(test_datafile_binary_br_uint_8);
    RUN_TEST(test_datafile_text_br_int_16);
    RUN_TEST(test_datafile_binary_br_int_16);
    RUN_TEST(test_datafile_text_br_int_16);
    RUN_TEST(test_datafile_binary_br_uint_16);
    RUN_TEST(test_datafile_text_br_uint_8);
    RUN_TEST(test_datafile_binary_br_int_32);
    RUN_TEST(test_datafile_text_br_int_32);
    RUN_TEST(test_datafile_binary_br_uint_32);
    RUN_TEST(test_datafile_text_br_uint_32);
    RUN_TEST(test_datafile_binary_br_fixed);
    RUN_TEST(test_datafile_text_br_fixed);
    RUN_TEST(test_datafile_binary_br_angle);
    RUN_TEST(test_datafile_text_br_angle);
    RUN_TEST(test_datafile_binary_float);
    RUN_TEST(test_datafile_text_float);
    RUN_TEST(test_datafile_binary_double);
    RUN_TEST(test_datafile_text_double);
    RUN_TEST(test_datafile_binary_enum_8);
    RUN_TEST(test_datafile_text_enum_8);
    RUN_TEST(test_datafile_binary_enum_16);
    RUN_TEST(test_datafile_text_enum_16);
    RUN_TEST(test_datafile_binary_enum_32);
    RUN_TEST(test_datafile_text_enum_32);
    RUN_TEST(test_datafile_binary_asciz);
    RUN_TEST(test_datafile_text_asciz);
    RUN_TEST(test_datafile_binary_br_colour);
    RUN_TEST(test_datafile_text_br_colour);
    RUN_TEST(test_datafile_binary_br_fraction_x);
    RUN_TEST(test_datafile_text_br_fraction_x);
    RUN_TEST(test_datafile_binary_br_ufraction_x);
    RUN_TEST(test_datafile_text_br_ufraction_x);
    RUN_TEST(test_datafile_binary_br_fraction_f);
    RUN_TEST(test_datafile_text_br_fraction_f);
    RUN_TEST(test_datafile_binary_br_ufraction_f);
    RUN_TEST(test_datafile_text_br_ufraction_f);
    RUN_TEST(test_datafile_binary_br_vector2_x);
    RUN_TEST(test_datafile_text_br_vector2_x);
    RUN_TEST(test_datafile_binary_br_vector3_x);
    RUN_TEST(test_datafile_text_br_vector3_x);
    RUN_TEST(test_datafile_binary_br_vector4_x);
    RUN_TEST(test_datafile_text_br_vector4_x);
    RUN_TEST(test_datafile_binary_br_vector2_f);
    RUN_TEST(test_datafile_text_br_vector2_f);
    RUN_TEST(test_datafile_binary_br_vector3_f);
    RUN_TEST(test_datafile_text_br_vector3_f);
    RUN_TEST(test_datafile_binary_br_vector4_f);
    RUN_TEST(test_datafile_text_br_vector4_f);
    RUN_TEST(test_datafile_binary_substruct);
    RUN_TEST(test_datafile_text_substruct);
    RUN_TEST(test_datafile_binary_chunk);
    RUN_TEST(test_datafile_text_chunk);
    RUN_TEST(test_datafile_binary_chunk_unknown);
    RUN_TEST(test_datafile_text_chunk_unknown);
    RUN_TEST(test_datafile_binary_count);
    RUN_TEST(test_datafile_text_count);
    RUN_TEST(test_datafile_binary_name);
    RUN_TEST(test_datafile_text_name);
    RUN_TEST(test_datafile_binary_block_continguous);
    RUN_TEST(test_datafile_text_block_continguous);
    RUN_TEST(test_datafile_binary_block_striped);
    RUN_TEST(test_datafile_text_block_striped);
    RUN_TEST(test_datafile_OpenFile);
}
