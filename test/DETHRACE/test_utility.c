#include "framework/unity.h"

#include <string.h>
#include "common/utility.h"

void test_utility_DecodeLine2() {
    char buf[50];
    // first line of GENERAL.TXT, "@" prefix and line ending stripped
    char input[] = "\x29\x2a\x9c\x22\x61\x4d\x5e\x5f\x60\x34\x64\x57\x8d\x2b\x82\x7b\x33\x4c";
    strcpy(buf, input);
    DecodeLine2(buf);
    char expected[] = "0.01\t\t\t\t\t// Hither";
    TEST_ASSERT_EQUAL_STRING(expected, buf);
}

void test_utility_EncodeLine2() {
    char buf[50];
    strcpy(buf, "0.01\t\t\t\t\t// Hither");
    EncodeLine2(buf);
    char expected[] = "\x29\x2a\x9c\x22\x61\x4d\x5e\x5f\x60\x34\x64\x57\x8d\x2b\x82\x7b\x33\x4c";
    TEST_ASSERT_EQUAL_STRING(expected, buf);
}

void test_utility_StripCR() {
    char buf[50];
    strcpy(buf, "new\nline");
    StripCR(buf);
    TEST_ASSERT_EQUAL_STRING("new", buf);
    strcpy(buf, "line");
    StripCR(buf);
    TEST_ASSERT_EQUAL_STRING("line", buf);
}

void test_loop() {
    int i = 0;
    if (i) {
        printf("got i\n");
    }
    if (!i) {
        printf("got not i\n");
    }
    i = 100;
    if (i) {
        printf("got i=1\n");
    }
    if (!i) {
        printf("got not i=1\n");
    }

}

void test_utility_GetALineWithNoPossibleService() {
    FILE *file = fopen("/tmp/testfile","wt");
    fprintf(file,"hello world\r\n  space_prefixed\r\n\r\n\ttab_prefixed\r\n$ignored_prefix\r\nlast_line");
    fclose(file);

    file = fopen("/tmp/testfile","rt");
    char s[256];

    char *result = GetALineWithNoPossibleService(file, s);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("hello world", s);

    result = GetALineWithNoPossibleService(file, s);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("space_prefixed", s);

    result = GetALineWithNoPossibleService(file, s);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("tab_prefixed", s);

    result = GetALineWithNoPossibleService(file, s);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("last_line", s);

    result = GetALineWithNoPossibleService(file, s);
    TEST_ASSERT_NULL(result);
}

void test_utility_suite() {
    RUN_TEST(test_utility_DecodeLine2);
    RUN_TEST(test_utility_EncodeLine2);
    RUN_TEST(test_utility_StripCR);
    RUN_TEST(test_utility_GetALineWithNoPossibleService);
}
