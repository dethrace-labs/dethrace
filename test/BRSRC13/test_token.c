#include "CORE/FW/token.h"
#include "CORE/FW/fwsetup.h"
#include "CORE/FW/brlists.h"
#include "tests.h"

static void test_token_BrTokenIdentifier() {
    char* ident;

    ident = BrTokenIdentifier(BRT_CULL);
    TEST_ASSERT_EQUAL_STRING("CULL", ident);
    ident = BrTokenIdentifier(BRT_CULL_B);
    TEST_ASSERT_EQUAL_STRING("CULL_B", ident);
    ident = BrTokenIdentifier(0x66666);
    TEST_ASSERT_NULL(ident);
}

static void test_token_BrTokenType() {
    br_token typ;

    typ = BrTokenType(BRT_CULL);
    TEST_ASSERT_EQUAL_UINT32(BRT_NONE, typ);
    typ = BrTokenType(BRT_CULL_B);
    TEST_ASSERT_EQUAL_UINT32(BRT_BOOLEAN, typ);
    typ = BrTokenType(BRT_D_15);
    TEST_ASSERT_EQUAL_UINT32(BRT_NONE, typ);
    typ = BrTokenType(BRT_DDI_VERSION_U32);
    TEST_ASSERT_EQUAL_UINT32(BRT_UINT_32, typ);
    typ = BrTokenType(BRT_DEFAULT_DEVICE_STR);
    TEST_ASSERT_EQUAL_UINT32(BRT_STRING, typ);
    typ = BrTokenType(BRT_SYCHRONISE_I32);
    TEST_ASSERT_EQUAL_UINT32(BRT_INT_32, typ);
    typ = BrTokenType(BRT_OPACITY_X);
    TEST_ASSERT_EQUAL_UINT32(BRT_FIXED, typ);
    typ = BrTokenType(BRT_TITLE_CSTR);
    TEST_ASSERT_EQUAL_UINT32(BRT_CONSTANT_STRING, typ);
    typ = BrTokenType(BRT_COPY_BITS_F);
    TEST_ASSERT_EQUAL_UINT32(BRT_FLOAT, typ);
    typ = BrTokenType(BRT_HEIGHT_TL);
    TEST_ASSERT_EQUAL_UINT32(BRT_TOKEN_LIST, typ);
    typ = BrTokenType(BRT_SCREEN_DOOR_TVL);
    TEST_ASSERT_EQUAL_UINT32(BRT_TOKEN_VALUE_LIST, typ);
    typ = BrTokenType(0x66666);
    TEST_ASSERT_EQUAL_UINT32(0, typ);
}

static void test_token_BrTokenFind() {
    br_token typ;

    typ = BrTokenFind("ANTIALIASING_TL");
    TEST_ASSERT_EQUAL_UINT32(BRT_ANTIALIASING_TL, typ);
    typ = BrTokenFind("STEREO_B");
    TEST_ASSERT_EQUAL_UINT32(BRT_STEREO_B, typ);
    typ = BrTokenFind("NO_WAY_THIS_TOKEN_EXISTS");
    TEST_ASSERT_EQUAL_UINT32(0, typ);
}

static int array_contains_token(br_token* array, size_t array_size, br_token element) {
    int i;
    for (i = 0; i < array_size; i++) {
        if (array[i] == element) {
            return 1;
        }
    }
    return 0;
}

static void test_token_BrTokenFindMany() {
    br_token typ;
    br_int_32 nb;
    br_token token_buffer[16];

    nb = BrTokenFindMany("*ANTIALIASING*", token_buffer, BR_ASIZE(token_buffer));
    TEST_ASSERT_GREATER_OR_EQUAL_INT32(4, nb);
    TEST_ASSERT_TRUE(array_contains_token(token_buffer, BR_ASIZE(token_buffer), BRT_ANTIALIASING_T));
    TEST_ASSERT_TRUE(array_contains_token(token_buffer, BR_ASIZE(token_buffer), BRT_ANTIALIASING_TL));
    TEST_ASSERT_TRUE(array_contains_token(token_buffer, BR_ASIZE(token_buffer), BRT_MAP_ANTIALIASING_T));
    TEST_ASSERT_TRUE(array_contains_token(token_buffer, BR_ASIZE(token_buffer), BRT_MAP_ANTIALIASING_TL));
}

static br_token test_vector3_type_tokens[] = {
    BRT_VECTOR3_FIXED,
    BRT_VECTOR3_FLOAT,
};
static br_token test_token_type_tokens[] = {
    BRT_TOKEN_LIST,
    BRT_TOKEN_VALUE_LIST,
};
static br_token test_antialiasing_tokens[] = {
    BRT_ANTIALIASING_T,
    BRT_ANTIALIASING_TL,
    BRT_MAP_ANTIALIASING_T,
    BRT_MAP_ANTIALIASING_TL,
};
static void test_token_BrTokenFindType() {
    br_token typ;
    br_token tok;

    typ = 0xdeadbabe;

    tok = BrTokenFindType(&typ, "NONSENSICAL", test_vector3_type_tokens, BR_ASIZE(test_vector3_type_tokens));
    TEST_ASSERT_EQUAL_INT32(0, tok);
    TEST_ASSERT_EQUAL_INT32(0xdeadbabe, typ);


    tok = BrTokenFindType(&typ, "ANTIALIASING", test_token_type_tokens, BR_ASIZE(test_token_type_tokens));
    TEST_ASSERT_TRUE(array_contains_token(test_antialiasing_tokens, BR_ASIZE(test_antialiasing_tokens), tok));
    TEST_ASSERT_TRUE(array_contains_token(test_token_type_tokens, BR_ASIZE(test_token_type_tokens), typ));
}

static void test_token_BrTokenCreate() {
    br_token typ;
    br_token tok;
    br_token new_tok;

    tok = BrTokenFind("TEST_TOKEN");
    TEST_ASSERT_EQUAL_INT32(0, tok);

    new_tok = BrTokenCreate("TOKEN_CREATE", BRT_MATRIX34_FLOAT);
    TEST_ASSERT_EQUAL_INT32(0, new_tok);

    new_tok = BrTokenCreate("TOKEN_CREATE_M34_F", BRT_MATRIX34_FLOAT);
    TEST_ASSERT_NOT_EQUAL(0, new_tok);

    tok = BrTokenFind("TOKEN_CREATE_M34_F");
    TEST_ASSERT_EQUAL_INT32(new_tok, tok);

    typ = BrTokenType(new_tok);
    TEST_ASSERT_EQUAL_INT32(BRT_MATRIX34_FLOAT, typ);

    tok = BrTokenCreate("ANTIALIASING_TL", BRT_TOKEN_LIST);
    TEST_ASSERT_EQUAL_INT32(BRT_ANTIALIASING_TL, tok);
}

void test_token_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_token_BrTokenIdentifier);
    RUN_TEST(test_token_BrTokenType);
    RUN_TEST(test_token_BrTokenFind);
    RUN_TEST(test_token_BrTokenFindMany);
    RUN_TEST(test_token_BrTokenFindType);
    RUN_TEST(test_token_BrTokenCreate);
}
