#include "tests.h"

#include "common/loading.h"
#include "common/utility.h"
#include <string.h>

void test_utility_EncodeLinex() {
    char buf[50];
    gEncryption_method = 1;
    // first line of GENERAL.TXT, "@" prefix and line ending stripped
    char input[] = "\x29\x2a\x9c\x22\x61\x4d\x5e\x5f\x60\x34\x64\x57\x8d\x2b\x82\x7b\x33\x4c";
    strcpy(buf, input);
    EncodeLine(buf);
    //TEST_ASSERT_EQUAL_INT(2, gEncryption_method);
    char expected[] = "0.01\t\t\t\t\t// Hither";
    TEST_ASSERT_EQUAL_STRING(expected, buf);
}

void test_utility_DecodeLine2() {
    char buf[50];
    gEncryption_method = 1;
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

static void get_system_temp_folder(char *buffer, size_t bufferSize) {
#ifdef _WIN32
    GetTempPathA(bufferSize, buffer);
#else
    strcpy(buffer, "/tmp/");
#endif
}

void test_utility_GetALineWithNoPossibleService() {
    char tmpPath[256];
    get_system_temp_folder(tmpPath, sizeof(tmpPath));
    strcat(tmpPath, "testfile");
    FILE* file = fopen(tmpPath, "wt");
    fprintf(file, "hello world\r\n  space_prefixed\r\n\r\n\ttab_prefixed\r\n$ignored_prefix\r\nlast_line");
    fclose(file);

    file = fopen(tmpPath, "rt");
    char s[256];

    char* result = GetALineWithNoPossibleService(file, s);
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

    fclose(file);
}

void test_utility_PathCat() {
    char buf[256];
    PathCat(buf, "a", "b");
    TEST_ASSERT_EQUAL_STRING("a/b", buf);

    PathCat(buf, "a", "");
    TEST_ASSERT_EQUAL_STRING("a", buf);
}

void test_utility_IRandomBetween() {
    int r;
    int i;
    int j;
    int actual_min;
    int actual_max;
    struct {
        int min;
        int max;
    } ranges[] = {
        { -2, 1 },
        { 1, -2 },
        { 2, -1 },
        { -1, 2 },
        { 0, 3 },
        { 3, 0 },
        { 0, 10000 },
        { 10000, 0 },
    };

    for (i = 0; i < BR_ASIZE(ranges); i++) {
        LOG_INFO("Testing min=%d max=%d", ranges[i].min, ranges[i].max);
        actual_min = MIN(ranges[i].min, ranges[i].max);
        actual_max = MAX(ranges[i].min, ranges[i].max);
        for (j = 0; j < 1000; j++) {
            r = IRandomBetween(ranges[i].min, ranges[i].max);

            TEST_ASSERT_GREATER_OR_EQUAL(actual_min, r);
            TEST_ASSERT_LESS_OR_EQUAL(actual_max, r);
        }
    }
}

void test_utility_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_utility_EncodeLinex);
    RUN_TEST(test_utility_DecodeLine2);
    RUN_TEST(test_utility_EncodeLine2);
    RUN_TEST(test_utility_StripCR);
    RUN_TEST(test_utility_GetALineWithNoPossibleService);
    RUN_TEST(test_utility_PathCat);
    RUN_TEST(test_utility_IRandomBetween);
}
