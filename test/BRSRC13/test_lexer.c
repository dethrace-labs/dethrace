#include "CORE/FW/lexer.h"
#include "CORE/MATH/fixed.h"
#include "tests.h"
#include <brender.h>
#include <stddef.h>
#include <string.h>

size_t lexer_error_msg_stack_size;
static char lexer_error_msg_stack[16][256];

static void lexer_error_stack_reset() {
    lexer_error_msg_stack_size = 0;
}

static void lexer_error_cbfn(br_lexer* l, char* msg) {
    TEST_ASSERT_LESS_THAN_size_t(BR_ASIZE(lexer_error_msg_stack), lexer_error_msg_stack_size);
    strncpy(lexer_error_msg_stack[lexer_error_msg_stack_size], msg, 255);
    lexer_error_msg_stack[lexer_error_msg_stack_size][255] = '\0';
    lexer_error_msg_stack_size++;
}

static void test_lexer_BrLexerAllocate_BrLexerFree() {
    br_lexer* l;

    l = BrLexerAllocate(NULL, 0);
    TEST_ASSERT_NOT_NULL(l);
    BrLexerFree(l);
}

static void test_lexer_BrLexer_BrLexerPushString() {
    br_lexer* l;
    br_int_32 v;
    char buf[256];

    l = BrLexerAllocate(NULL, 0);
    TEST_ASSERT_NOT_NULL(l);

    BrLexerPushString(l, "a=4\nb=\"something\"\nc=2.5\n", "BrLexerPushString_test_function");

    BrLexerPosition(l, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("BrLexerPushString_test_function:1 ", buf);

    l->advance(l);
    BrLexerPosition(l, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(T_IDENT, l->current.id);
    TEST_ASSERT_EQUAL_STRING("a", l->current.v.string);
    TEST_ASSERT_EQUAL_STRING("BrLexerPushString_test_function:1 ", buf);
    l->advance(l);
    BrLexerPosition(l, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT('=', l->current.id);
    TEST_ASSERT_EQUAL_STRING("BrLexerPushString_test_function:1 ", buf);
    l->advance(l);
    BrLexerPosition(l, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(T_INTEGER, l->current.id);
    TEST_ASSERT_EQUAL_INT(4, l->current.v.integer);
    TEST_ASSERT_EQUAL_STRING("BrLexerPushString_test_function:1 ", buf);

    l->advance(l);
    BrLexerPosition(l, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(T_IDENT, l->current.id);
    TEST_ASSERT_EQUAL_STRING("b", l->current.v.string);
    TEST_ASSERT_EQUAL_STRING("BrLexerPushString_test_function:2 ", buf);
    l->advance(l);
    BrLexerPosition(l, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT('=', l->current.id);
    TEST_ASSERT_EQUAL_STRING("BrLexerPushString_test_function:2 ", buf);
    l->advance(l);
    BrLexerPosition(l, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(T_STRING, l->current.id);
    TEST_ASSERT_EQUAL_STRING("something", l->current.v.string);
    TEST_ASSERT_EQUAL_STRING("BrLexerPushString_test_function:2 ", buf);

    l->advance(l);
    BrLexerPosition(l, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(T_IDENT, l->current.id);
    TEST_ASSERT_EQUAL_STRING("c", l->current.v.string);
    TEST_ASSERT_EQUAL_STRING("BrLexerPushString_test_function:3 ", buf);
    l->advance(l);
    BrLexerPosition(l, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT('=', l->current.id);
    TEST_ASSERT_EQUAL_STRING("BrLexerPushString_test_function:3 ", buf);
    l->advance(l);
    BrLexerPosition(l, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(T_REAL, l->current.id);
    TEST_ASSERT_EQUAL_FLOAT(2.5f, l->current.v.real);
    TEST_ASSERT_EQUAL_STRING("BrLexerPushString_test_function:3 ", buf);

    BrLexerFree(l);
}

static void test_lexer_comments() {
    br_lexer* l;
    char prev;

    l = BrLexerAllocate(NULL, 0);
    TEST_ASSERT_NOT_NULL(l);

    prev = BrLexerCommentSet(l, '#');
    TEST_ASSERT_EQUAL_CHAR('\0', prev);

    BrLexerPushString(l, "#comment1\n4#comment2\n1.25#comment3\n", (char*)__func__);

    TEST_ASSERT_EQUAL_INT(1, l->source->line);

    l->advance(l);
    TEST_ASSERT_EQUAL_INT(T_INTEGER, l->current.id);
    TEST_ASSERT_EQUAL_INT(4, l->current.v.integer);
    TEST_ASSERT_EQUAL_INT(2, l->source->line);

    l->advance(l);
    TEST_ASSERT_EQUAL_INT(T_REAL, l->current.id);
    TEST_ASSERT_EQUAL_FLOAT(1.25f, l->current.v.real);
    TEST_ASSERT_EQUAL_INT(3, l->source->line);

    l->advance(l);
    TEST_ASSERT_EQUAL_INT(T_EOF, l->current.id);

    BrLexerFree(l);
}

struct {
    char* str;
    int val;
} integer_test_values[] = {
    { "0", 0 },
    { "-0", 0 },
    { "4", 4 },
    { "-4", -4 },
    { "1337", 1337 },
    { "-1337", -1337 },
    { "0x12345678", 0x12345678 },
    { "-0x1337", -0x1337 },
    { "0777", 0777 },
};

static void test_lexer_BrParseInteger() {
    br_lexer* l;
    int i;
    br_int_32 v;

    l = BrLexerAllocate(NULL, 0);
    TEST_ASSERT_NOT_NULL(l);

    for (i = 0; i < BR_ASIZE(integer_test_values); i++) {
        LOG_DEBUG("Testing input \"%s\", expecting %d", integer_test_values[i].str, integer_test_values[i].val);

        BrLexerPushString(l, integer_test_values[i].str, (char*)__func__);
        l->advance(l);
        v = BrParseInteger(l);
        TEST_ASSERT_NOT_EQUAL(T_ERROR, l->current.id);
        TEST_ASSERT_EQUAL_INT(integer_test_values[i].val, v);
        TEST_ASSERT_NULL(l->source);
    }

    BrLexerPushString(l, "0 -1 3 -7 15 -31 63 -127 255 -511 1023 -2047 4095 -8191", (char*)__func__);
    l->advance(l);
    for (i = 0; i < 14; i++) {
        v = BrParseInteger(l);
        TEST_ASSERT_NOT_EQUAL(T_ERROR, l->current.id);
        TEST_ASSERT_EQUAL_INT((i & 1 ? -1 : 1) * ((1 << i) - 1), v);
    }
    TEST_ASSERT_NULL(l->source);

    BrLexerFree(l);
}

struct {
    char* str;
    float val;
} float_test_values[] = {
    { "0.", 0.f },
    { "-0.", 0.f },
    { "  4.5", 4.5f },
    { "-4.5", -4.5f },
    { "0.0625", 0.0625f },
    { "-4.0625", -4.0625f },
    { "1337.125", 1337.125 },
    { "-1337.125", -1337.125 },
};

static void test_lexer_BrParseFloat() {
    br_lexer* l;
    br_float v;
    int i;

    l = BrLexerAllocate(NULL, 0);
    TEST_ASSERT_NOT_NULL(l);

    for (i = 0; i < BR_ASIZE(float_test_values); i++) {
        LOG_DEBUG("Testing input \"%s\", expecting %f", float_test_values[i].str, float_test_values[i].val);

        BrLexerPushString(l, float_test_values[i].str, (char*)__func__);
        l->advance(l);
        v = BrParseFloat(l);
        TEST_ASSERT_NOT_EQUAL(T_ERROR, l->current.id);
        TEST_ASSERT_EQUAL_FLOAT(float_test_values[i].val, v);
        TEST_ASSERT_NULL(l->source);
    }

    BrLexerPushString(l, "0 4.25 6.25 -7.5 4.125 -7.375 63.4375", (char*)__func__);
    l->advance(l);

    TEST_ASSERT_EQUAL_FLOAT(0.f, BrParseFloat(l));
    TEST_ASSERT_EQUAL_FLOAT(4.25f, BrParseFloat(l));
    TEST_ASSERT_EQUAL_FLOAT(6.25f, BrParseFloat(l));
    TEST_ASSERT_EQUAL_FLOAT(-7.5f, BrParseFloat(l));
    TEST_ASSERT_EQUAL_FLOAT(4.125f, BrParseFloat(l));
    TEST_ASSERT_EQUAL_FLOAT(-7.375f, BrParseFloat(l));
    TEST_ASSERT_EQUAL_FLOAT(63.4375f, BrParseFloat(l));
    TEST_ASSERT_NULL(l->source);

    BrLexerFree(l);
}

struct {
    char* str;
    br_fixed_ls val;
} fixed_test_values[] = {
    { "0.", BrFloatToFixed(0.f) },
    { "-0.", BrFloatToFixed(-0.f) },
    { "  4.5", BrFloatToFixed(4.5f) },
    { "  9", BrIntToFixed(9) },
    { "-4.5", BrFloatToFixed(-4.5f) },
    { "0.0625", BrFloatToFixed(0.0625f) },
    { "-4.0625", BrFloatToFixed(-4.0625f) },
    { "1337.125", BrFloatToFixed(1337.125f) },
    { "-1337.125", BrFloatToFixed(-1337.125f) },
};

static void test_lexer_BrParseFixed() {
    br_lexer* l;
    br_fixed_ls v;
    int i;

    l = BrLexerAllocate(NULL, 0);
    TEST_ASSERT_NOT_NULL(l);

    for (i = 0; i < BR_ASIZE(float_test_values); i++) {
        LOG_DEBUG("Testing input \"%s\", expecting 0x%08x", fixed_test_values[i].str, fixed_test_values[i].val);

        BrLexerPushString(l, fixed_test_values[i].str, (char*)__func__);
        l->advance(l);
        v = BrParseFixed(l);
        TEST_ASSERT_NOT_EQUAL(T_ERROR, l->current.id);
        TEST_ASSERT_EQUAL_INT32(fixed_test_values[i].val, v);
        TEST_ASSERT_NULL(l->source);
    }

    BrLexerPushString(l, "0 4.25 6.25 -7.5 4.125 -7.375 63.4375", (char*)__func__);
    l->advance(l);

    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(0.f), BrParseFixed(l));
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(4.25f), BrParseFixed(l));
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(6.25f), BrParseFixed(l));
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(-7.5f), BrParseFixed(l));
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(4.125f), BrParseFixed(l));
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(-7.375f), BrParseFixed(l));
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(63.4375f), BrParseFixed(l));
    TEST_ASSERT_NULL(l->source);

    BrLexerFree(l);
}

struct {
    char* str;
    char* val;
} string_test_values[] = {
    { "\"\"", "" },
    { "  \"a\"", "a" },
    { "\"shortstring\"", "shortstring" },
    { "  \"a_snake_string\"", "a_snake_string" },
    { "\"string with spaces\"", "string with spaces" },
    { "  \"extremely supercalifragilisticexpialidocious long string\"", "extremely supercalifragilisticexpialidocious long string" },
};

static void test_lexer_string() {
    br_lexer* l;
    int i;

    l = BrLexerAllocate(NULL, 0);
    TEST_ASSERT_NOT_NULL(l);

    BrLexerPushString(l, "\"a string\"", (char*)__func__);
    l->advance(l);

    TEST_ASSERT_EQUAL(T_STRING, l->current.id);
    TEST_ASSERT_EQUAL_STRING("a string", l->current.v.string);

    BrLexerFree(l);

    l = BrLexerAllocate(NULL, 0);

    for (i = 0; i < BR_ASIZE(string_test_values); i++) {
        LOG_DEBUG("Testing input \"%s\", expecting \"%s\"", string_test_values[i].str, string_test_values[i].val);

        BrLexerPushString(l, string_test_values[i].str, (char*)__func__);
        l->advance(l);

        TEST_ASSERT_EQUAL(T_STRING, l->current.id);
        TEST_ASSERT_EQUAL_STRING(string_test_values[i].val, l->current.v.string);
        TEST_ASSERT_NOT_NULL(l->source);
        l->advance(l);
        TEST_ASSERT_NULL(l->source);
    }

    BrLexerPushString(l, "\"\" \"abc\" \"def\" \"super nice\" \"another long string\"", (char*)__func__);

    l->advance(l);
    TEST_ASSERT_EQUAL(T_STRING, l->current.id);
    TEST_ASSERT_EQUAL_STRING("", l->current.v.string);

    l->advance(l);
    TEST_ASSERT_EQUAL(T_STRING, l->current.id);
    TEST_ASSERT_EQUAL_STRING("abc", l->current.v.string);

    l->advance(l);
    TEST_ASSERT_EQUAL(T_STRING, l->current.id);
    TEST_ASSERT_EQUAL_STRING("def", l->current.v.string);

    l->advance(l);
    TEST_ASSERT_EQUAL(T_STRING, l->current.id);
    TEST_ASSERT_EQUAL_STRING("super nice", l->current.v.string);

    l->advance(l);
    TEST_ASSERT_EQUAL(T_STRING, l->current.id);
    TEST_ASSERT_EQUAL_STRING("another long string", l->current.v.string);

    l->advance(l);
    TEST_ASSERT_NULL(l->source);

    BrLexerFree(l);
}

struct {
    char* str;
} ident_test_values[] = {
    { "_" },
    { "a" },
    { "_a" },
    { "_a1" },
    { "a1" },
    { "a_1" },
    { "abcdef_dfef" },
    { "whatever1337_____" },
};

static void test_lexer_ident() {
    br_lexer* l;
    int i;

    l = BrLexerAllocate(NULL, 0);
    TEST_ASSERT_NOT_NULL(l);

    BrLexerPushString(l, "abc", (char*)__func__);
    l->advance(l);

    TEST_ASSERT_EQUAL(T_IDENT, l->current.id);
    TEST_ASSERT_EQUAL_STRING("abc", l->current.v.string);

    BrLexerFree(l);

    l = BrLexerAllocate(NULL, 0);

    for (i = 0; i < BR_ASIZE(ident_test_values); i++) {
        LOG_DEBUG("Testing input \"%s\", expecting \"%s\"", ident_test_values[i].str, ident_test_values[i].str);

        BrLexerPushString(l, ident_test_values[i].str, (char*)__func__);
        l->advance(l);

        TEST_ASSERT_EQUAL(T_IDENT, l->current.id);
        TEST_ASSERT_EQUAL_STRING(ident_test_values[i].str, l->current.v.string);
        TEST_ASSERT_NOT_NULL(l->source);
        l->advance(l);
        TEST_ASSERT_NULL(l->source);
    }

    BrLexerPushString(l, "_ abc _something ___dd", (char*)__func__);

    l->advance(l);
    TEST_ASSERT_EQUAL(T_IDENT, l->current.id);
    TEST_ASSERT_EQUAL_STRING("_", l->current.v.string);

    l->advance(l);
    TEST_ASSERT_EQUAL(T_IDENT, l->current.id);
    TEST_ASSERT_EQUAL_STRING("abc", l->current.v.string);

    l->advance(l);
    TEST_ASSERT_EQUAL(T_IDENT, l->current.id);
    TEST_ASSERT_EQUAL_STRING("_something", l->current.v.string);

    l->advance(l);
    TEST_ASSERT_EQUAL(T_IDENT, l->current.id);
    TEST_ASSERT_EQUAL_STRING("___dd", l->current.v.string);

    l->advance(l);
    TEST_ASSERT_NULL(l->source);

    BrLexerFree(l);
}

enum {
    T_TEST_CUSTOM_UINT8_T = 0x200,
    T_TEST_CUSTOM_UINT16_T = 0x201,
    T_TEST_CUSTOM_UINT32_T = 0x202,
    T_TEST_CUSTOM_INT8_T = 0x203,
    T_TEST_CUSTOM_INT16_T = 0x204,
    T_TEST_CUSTOM_INT32_T = 0x205,
    T_TEST_CUSTOM_INT32_TT = 0x206,
};
br_lexer_keyword lexer_ident_custom_keywords[] = {
    { "uint8_t", T_TEST_CUSTOM_UINT8_T },
    { "uint16_t", T_TEST_CUSTOM_UINT16_T },
    { "uint32_t", T_TEST_CUSTOM_UINT32_T },
    { "int8_t", T_TEST_CUSTOM_INT8_T },
    { "int16_t", T_TEST_CUSTOM_INT16_T },
    { "int32_tt", T_TEST_CUSTOM_INT32_TT },
    { "int32_t", T_TEST_CUSTOM_INT32_T },
};
struct {
    char* str;
    int token;
} ident_custom_test_values[] = {
    { "_", T_IDENT },
    { "uint32_t", T_TEST_CUSTOM_UINT32_T },
    { "int32_t", T_TEST_CUSTOM_INT32_T },
    { "int32_t_", T_IDENT },
    { "int32_t", T_TEST_CUSTOM_INT32_T },
    { "int32_tt", T_TEST_CUSTOM_INT32_TT },
    { "int8_t", T_TEST_CUSTOM_INT8_T },
    { "_int8_t", T_IDENT },
};

static void test_lexer_ident_custom() {
    br_lexer* l;
    int i;

    l = BrLexerAllocate(lexer_ident_custom_keywords, BR_ASIZE(lexer_ident_custom_keywords));
    TEST_ASSERT_NOT_NULL(l);

    BrLexerPushString(l, "abc", (char*)__func__);
    l->advance(l);
    TEST_ASSERT_EQUAL(T_IDENT, l->current.id);
    TEST_ASSERT_EQUAL_STRING("abc", l->current.v.string);

    BrLexerPushString(l, "uint8_t", (char*)__func__);
    l->advance(l);
    TEST_ASSERT_EQUAL(T_TEST_CUSTOM_UINT8_T, l->current.id);
    TEST_ASSERT_EQUAL_STRING("uint8_t", l->current.v.string);

    BrLexerFree(l);

    l = BrLexerAllocate(lexer_ident_custom_keywords, BR_ASIZE(lexer_ident_custom_keywords));

    for (i = 0; i < BR_ASIZE(ident_custom_test_values); i++) {
        LOG_DEBUG("Testing input \"%s\", expecting \"%s\"", ident_custom_test_values[i].str, ident_custom_test_values[i].str);

        BrLexerPushString(l, ident_custom_test_values[i].str, (char*)__func__);
        l->advance(l);

        TEST_ASSERT_EQUAL(ident_custom_test_values[i].token, l->current.id);
        TEST_ASSERT_EQUAL_STRING(ident_custom_test_values[i].str, l->current.v.string);
        TEST_ASSERT_NOT_NULL(l->source);
        l->advance(l);
        TEST_ASSERT_NULL(l->source);
    }

    BrLexerPushString(l, "_ uint8_t _something int8_t int32_t int32_tt", (char*)__func__);

    l->advance(l);
    TEST_ASSERT_EQUAL(T_IDENT, l->current.id);
    TEST_ASSERT_EQUAL_STRING("_", l->current.v.string);

    l->advance(l);
    TEST_ASSERT_EQUAL(T_TEST_CUSTOM_UINT8_T, l->current.id);
    TEST_ASSERT_EQUAL_STRING("uint8_t", l->current.v.string);

    l->advance(l);
    TEST_ASSERT_EQUAL(T_IDENT, l->current.id);
    TEST_ASSERT_EQUAL_STRING("_something", l->current.v.string);

    l->advance(l);
    TEST_ASSERT_EQUAL(T_TEST_CUSTOM_INT8_T, l->current.id);
    TEST_ASSERT_EQUAL_STRING("int8_t", l->current.v.string);

    l->advance(l);
    TEST_ASSERT_EQUAL(T_TEST_CUSTOM_INT32_T, l->current.id);
    TEST_ASSERT_EQUAL_STRING("int32_t", l->current.v.string);

    l->advance(l);
    TEST_ASSERT_EQUAL(T_TEST_CUSTOM_INT32_TT, l->current.id);
    TEST_ASSERT_EQUAL_STRING("int32_tt", l->current.v.string);

    l->advance(l);
    TEST_ASSERT_NULL(l->source);

    BrLexerFree(l);
}

static void test_lexer_unterminated_string() {
    br_lexer* l;

    l = BrLexerAllocate(NULL, 0);
    TEST_ASSERT_NOT_NULL(l);

    BrLexerPushString(l, "\"unterminated", (char*)__func__);
    l->advance(l);

    TEST_ASSERT_EQUAL(T_ERROR, l->current.id);
    TEST_ASSERT_EQUAL_STRING("Unterminated string", l->current.v.string);

    BrLexerFree(l);
}

static void test_lexer_BrLexerParseVectorFloat() {
    br_lexer* l;
    float values[16];
    int n;

    l = BrLexerAllocate(NULL, 0);
    TEST_ASSERT_NOT_NULL(l);
    BrLexerErrorSet(l, lexer_error_cbfn);

    BrLexerPushString(l, "[5.25]", (char*)__func__);
    l->advance(l);
    n = BrParseVectorFloat(l, values, BR_ASIZE(values));
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_FLOAT(5.25f, values[0]);
    TEST_ASSERT_NULL(l->source);

    BrLexerPushString(l, "[1.5,      2,\t-4.5]", (char*)__func__);
    l->advance(l);
    n = BrParseVectorFloat(l, values, BR_ASIZE(values));
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(3, n);
    TEST_ASSERT_EQUAL_FLOAT(1.5f, values[0]);
    TEST_ASSERT_EQUAL_FLOAT(2.f, values[1]);
    TEST_ASSERT_EQUAL_FLOAT(-4.5f, values[2]);
    TEST_ASSERT_NULL(l->source);

    BrLexerPushString(l, "[\t  ]", (char*)__func__);
    l->advance(l);
    n = BrParseVectorFloat(l, values, BR_ASIZE(values));
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "[1.5,1.25,1.125]", (char*)__func__);
    l->advance(l);
    n = BrParseVectorFloat(l, values, 2);
    TEST_ASSERT_EQUAL_STRING("expected ']'", lexer_error_msg_stack[0]);
    TEST_ASSERT_EQUAL_INT(2, n);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "1.5", (char*)__func__);
    l->advance(l);
    n = BrParseVectorFloat(l, values, 2);
    TEST_ASSERT_EQUAL_STRING("expected '['", lexer_error_msg_stack[0]);
    TEST_ASSERT_EQUAL_INT(2, n);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "[1.5,1.25", (char*)__func__);
    l->advance(l);
    n = BrParseVectorFloat(l, values, 2);
    TEST_ASSERT_EQUAL_STRING("expected ']'", lexer_error_msg_stack[0]);
    TEST_ASSERT_EQUAL_INT(2, n);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "[1.5 1.25", (char*)__func__);
    l->advance(l);
    n = BrParseVectorFloat(l, values, 2);
    TEST_ASSERT_EQUAL_STRING("expected ','", lexer_error_msg_stack[0]);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    BrLexerFree(l);
}

static void test_lexer_BrLexerParseVectorFixed() {
    br_lexer* l;
    br_fixed_ls values[16];
    int n;

    l = BrLexerAllocate(NULL, 0);
    TEST_ASSERT_NOT_NULL(l);
    BrLexerErrorSet(l, lexer_error_cbfn);

    BrLexerPushString(l, "[5.25]", (char*)__func__);
    l->advance(l);
    n = BrParseVectorFixed(l, values, BR_ASIZE(values));
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(5.25f), values[0]);
    TEST_ASSERT_NULL(l->source);

    BrLexerPushString(l, "[1.5,      2,\t-4.5]", (char*)__func__);
    l->advance(l);
    n = BrParseVectorFixed(l, values, BR_ASIZE(values));
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(3, n);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(1.5f), values[0]);
    TEST_ASSERT_EQUAL_INT32(BrIntToFixed(2), values[1]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(-4.5f), values[2]);
    TEST_ASSERT_NULL(l->source);

    BrLexerPushString(l, "[\t  ]", (char*)__func__);
    l->advance(l);
    n = BrParseVectorFixed(l, values, BR_ASIZE(values));
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "[1.5,1.25,1.125]", (char*)__func__);
    l->advance(l);
    n = BrParseVectorFixed(l, values, 2);
    TEST_ASSERT_EQUAL_STRING("expected ']'", lexer_error_msg_stack[0]);
    TEST_ASSERT_EQUAL_INT(2, n);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "1.5", (char*)__func__);
    l->advance(l);
    n = BrParseVectorFixed(l, values, 2);
    TEST_ASSERT_EQUAL_STRING("expected '['", lexer_error_msg_stack[0]);
    TEST_ASSERT_EQUAL_INT(2, n);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "[1.5,1.25", (char*)__func__);
    l->advance(l);
    n = BrParseVectorFixed(l, values, 2);
    TEST_ASSERT_EQUAL_STRING("expected ']'", lexer_error_msg_stack[0]);
    TEST_ASSERT_EQUAL_INT(2, n);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "[1.5 1.25", (char*)__func__);
    l->advance(l);
    n = BrParseVectorFixed(l, values, 2);
    TEST_ASSERT_EQUAL_STRING("expected ','", lexer_error_msg_stack[0]);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    BrLexerFree(l);
}

static void test_lexer_BrLexerParseMatrixFloat() {
    br_lexer* l;
    float values[16];
    int n;

    l = BrLexerAllocate(NULL, 0);
    TEST_ASSERT_NOT_NULL(l);
    BrLexerErrorSet(l, lexer_error_cbfn);

    BrLexerPushString(l, "[[5.25]]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFloat(l, values, 4, 4);
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_FLOAT(5.25f, values[0]);
    TEST_ASSERT_NULL(l->source);

    BrLexerPushString(l, "[[1.5,      2,\t-4.5]]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFloat(l, values, 3, 1);
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_FLOAT(1.5f, values[0]);
    TEST_ASSERT_EQUAL_FLOAT(2.f, values[1]);
    TEST_ASSERT_EQUAL_FLOAT(-4.5f, values[2]);
    TEST_ASSERT_NULL(l->source);

    BrLexerPushString(l, "[[1.5,      2,\t-4.5],[1, 3, 5.5]]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFloat(l, values, 4, 2);
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(2, n);
    TEST_ASSERT_EQUAL_FLOAT(1.5f, values[0]);
    TEST_ASSERT_EQUAL_FLOAT(2.f, values[1]);
    TEST_ASSERT_EQUAL_FLOAT(-4.5f, values[2]);
    TEST_ASSERT_EQUAL_FLOAT(1.f, values[4]);
    TEST_ASSERT_EQUAL_FLOAT(3.f, values[5]);
    TEST_ASSERT_EQUAL_FLOAT(5.5f, values[6]);
    TEST_ASSERT_NULL(l->source);

    BrLexerPushString(l, "[[1.5,      2,\t-4.5],]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFloat(l, values, 3, 2);
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_FLOAT(1.5f, values[0]);
    TEST_ASSERT_EQUAL_FLOAT(2.f, values[1]);
    TEST_ASSERT_EQUAL_FLOAT(-4.5f, values[2]);
    TEST_ASSERT_NULL(l->source);

    BrLexerPushString(l, "[[1.25, 2.5, 3.75, 5.],[6.25,7.5,8.75,10],[11.25,12.5,13.75,15],[16.25,17.5,18.75,20]]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFloat(l, values, 4, 4);
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(4, n);
    TEST_ASSERT_EQUAL_FLOAT(1.25f, values[0]);
    TEST_ASSERT_EQUAL_FLOAT(2.5f, values[1]);
    TEST_ASSERT_EQUAL_FLOAT(3.75f, values[2]);
    TEST_ASSERT_EQUAL_FLOAT(5.f, values[3]);
    TEST_ASSERT_EQUAL_FLOAT(6.25f, values[4]);
    TEST_ASSERT_EQUAL_FLOAT(7.5f, values[5]);
    TEST_ASSERT_EQUAL_FLOAT(8.75f, values[6]);
    TEST_ASSERT_EQUAL_FLOAT(10.f, values[7]);
    TEST_ASSERT_EQUAL_FLOAT(11.25f, values[8]);
    TEST_ASSERT_EQUAL_FLOAT(12.5f, values[9]);
    TEST_ASSERT_EQUAL_FLOAT(13.75f, values[10]);
    TEST_ASSERT_EQUAL_FLOAT(15.f, values[11]);
    TEST_ASSERT_EQUAL_FLOAT(16.25f, values[12]);
    TEST_ASSERT_EQUAL_FLOAT(17.5f, values[13]);
    TEST_ASSERT_EQUAL_FLOAT(18.75f, values[14]);
    TEST_ASSERT_EQUAL_FLOAT(20.f, values[15]);
    TEST_ASSERT_NULL(l->source);

    BrLexerPushString(l, "[\t  ]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFloat(l, values, 3, 1);
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "[1.5,1.25,1.125]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFloat(l, values, 3, 1);
    TEST_ASSERT_EQUAL_STRING("expected '['", lexer_error_msg_stack[0]);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "[[1.5,1.25,1.125]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFloat(l, values, 3, 1);
    TEST_ASSERT_EQUAL_STRING("expected ']'", lexer_error_msg_stack[0]);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "1.5", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFloat(l, values, 3, 1);
    TEST_ASSERT_EQUAL_STRING("expected '['", lexer_error_msg_stack[0]);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "[1.5,1.25", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFloat(l, values, 2, 1);
    TEST_ASSERT_EQUAL_STRING("expected '['", lexer_error_msg_stack[0]);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "[[1.5 1.25],[1.5,1.4]]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFloat(l, values, 2, 2);
    TEST_ASSERT_EQUAL_STRING("expected ','", lexer_error_msg_stack[0]);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "[[1.5,1.25] [1.5,1.4]]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFloat(l, values, 2, 2);
    TEST_ASSERT_EQUAL_STRING("expected ','", lexer_error_msg_stack[0]);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    BrLexerFree(l);
}

static void test_lexer_BrLexerParseMatrixFixed() {
    br_lexer* l;
    br_fixed_ls values[16];
    int n;

    l = BrLexerAllocate(NULL, 0);
    TEST_ASSERT_NOT_NULL(l);
    BrLexerErrorSet(l, lexer_error_cbfn);

    BrLexerPushString(l, "[[5.25]]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFixed(l, values, 4, 4);
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(5.25f), values[0]);
    TEST_ASSERT_NULL(l->source);

    BrLexerPushString(l, "[[1.5,      2,\t-4.5]]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFixed(l, values, 3, 1);
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(1.5f), values[0]);
    TEST_ASSERT_EQUAL_INT32(BrIntToFixed(2), values[1]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(-4.5f), values[2]);
    TEST_ASSERT_NULL(l->source);

    BrLexerPushString(l, "[[1.5,      2,\t-4.5],[1, 3, 5.5]]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFixed(l, values, 4, 2);
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(2, n);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(1.5f), values[0]);
    TEST_ASSERT_EQUAL_INT32(BrIntToFixed(2), values[1]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(-4.5f), values[2]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(1.f), values[4]);
    TEST_ASSERT_EQUAL_INT32(BrIntToFixed(3), values[5]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(5.5f), values[6]);
    TEST_ASSERT_NULL(l->source);

    BrLexerPushString(l, "[[1.5,      2,\t-4.5],]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFixed(l, values, 3, 2);
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(1.5f), values[0]);
    TEST_ASSERT_EQUAL_INT32(BrIntToFixed(2), values[1]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(-4.5f), values[2]);
    TEST_ASSERT_NULL(l->source);

    BrLexerPushString(l, "[[1.25, 2.5, 3.75, 5.],[6.25,7.5,8.75,10],[11.25,12.5,13.75,15],[16.25,17.5,18.75,20]]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFixed(l, values, 4, 4);
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(4, n);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(1.25f), values[0]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(2.5f), values[1]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(3.75f), values[2]);
    TEST_ASSERT_EQUAL_INT32(BrIntToFixed(5), values[3]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(6.25f), values[4]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(7.5f), values[5]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(8.75f), values[6]);
    TEST_ASSERT_EQUAL_INT32(BrIntToFixed(10), values[7]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(11.25f), values[8]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(12.5f), values[9]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(13.75f), values[10]);
    TEST_ASSERT_EQUAL_INT32(BrIntToFixed(15), values[11]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(16.25f), values[12]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(17.5f), values[13]);
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(18.75f), values[14]);
    TEST_ASSERT_EQUAL_INT32(BrIntToFixed(20), values[15]);
    TEST_ASSERT_NULL(l->source);

    BrLexerPushString(l, "[\t  ]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFixed(l, values, 3, 1);
    TEST_ASSERT_EQUAL(T_EOF, l->current.id);
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "[1.5,1.25,1.125]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFixed(l, values, 3, 1);
    TEST_ASSERT_EQUAL_STRING("expected '['", lexer_error_msg_stack[0]);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "[[1.5,1.25,1.125]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFixed(l, values, 3, 1);
    TEST_ASSERT_EQUAL_STRING("expected ']'", lexer_error_msg_stack[0]);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "1.5", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFixed(l, values, 3, 1);
    TEST_ASSERT_EQUAL_STRING("expected '['", lexer_error_msg_stack[0]);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "[1.5,1.25", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFixed(l, values, 2, 1);
    TEST_ASSERT_EQUAL_STRING("expected '['", lexer_error_msg_stack[0]);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "[[1.5 1.25],[1.5,1.4]]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFixed(l, values, 2, 2);
    TEST_ASSERT_EQUAL_STRING("expected ','", lexer_error_msg_stack[0]);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    lexer_error_stack_reset();
    BrLexerPushString(l, "[[1.5,1.25] [1.5,1.4]]", (char*)__func__);
    l->advance(l);
    n = BrParseMatrixFixed(l, values, 2, 2);
    TEST_ASSERT_EQUAL_STRING("expected ','", lexer_error_msg_stack[0]);
    while (l->current.id != T_EOF) {
        l->advance(l);
    }
    TEST_ASSERT_NULL(l->source);

    BrLexerFree(l);
}

void test_lexer_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_lexer_BrLexerAllocate_BrLexerFree);
    RUN_TEST(test_lexer_BrLexer_BrLexerPushString);
    RUN_TEST(test_lexer_comments);
    RUN_TEST(test_lexer_BrParseInteger);
    RUN_TEST(test_lexer_BrParseFloat);
    RUN_TEST(test_lexer_BrParseFixed);
    RUN_TEST(test_lexer_string);
    RUN_TEST(test_lexer_ident);
    RUN_TEST(test_lexer_ident_custom);
    RUN_TEST(test_lexer_unterminated_string);
    RUN_TEST(test_lexer_BrLexerParseVectorFloat);
    RUN_TEST(test_lexer_BrLexerParseVectorFixed);
    RUN_TEST(test_lexer_BrLexerParseMatrixFloat);
    RUN_TEST(test_lexer_BrLexerParseMatrixFixed);
}
