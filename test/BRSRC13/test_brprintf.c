#include "tests.h"
#include "CORE/FW/brprintf.h"
#include <string.h>

static void test_brprintf_BrSPrintf() {
    char buf[256];

    memset(buf, 0xaa, sizeof(buf));
    BrSprintf(buf, "My name is %s, am %d years old, loves 0x%08x and %g.", "Bond, James Bond", 47, 0xdeadbabe, 3.14159);
    TEST_ASSERT_EQUAL_STRING("My name is Bond, James Bond, am 47 years old, loves 0xdeadbabe and 3.14159.", buf);
}

static void test_brprintf_BrSPrintfN() {
    char buf[256];

    memset(buf, 0xaa, sizeof(buf));
    BrSprintfN(buf, 20, "My name is %s, am %d years old, loves 0x%08x and %g.", "Bond, James Bond", 15, 0xdeadbabe, 3.14159);
    TEST_ASSERT_EQUAL_STRING_LEN("My name is Bond, Ja", buf, 20);
}

static void test_brprintf_BrLogPrintf() {
    char buf[256];

    BrLogPrintf("My name is %s, am %d years old, loves 0x%08x and %g.", "Bond, James Bond", 15, 0xdeadbabe, 3.14159);
}


static void test_brprintf_BrSScanf() {
    char buf[256];
    float f;
    br_uint_32 v1;
    int v2;
    int n;

    memset(buf, 0xaa, sizeof(buf));
    f = 42.3f;
    v1 = 0xaaaaaaaa;
    v2 = 0x55555555;

    n = BrSScanf("abcdef 3.14159 0xdeadbabe 21", "%s %f 0x%08x %d", buf, &f, &v1, &v2);
    TEST_ASSERT_EQUAL_INT(4, n);
    TEST_ASSERT_EQUAL_STRING("abcdef", buf);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 3.14159f, f);
    TEST_ASSERT_EQUAL_UINT32(0xdeadbabe, v1);
    TEST_ASSERT_EQUAL_INT(21, v2);
}

void test_brprintf_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_brprintf_BrSPrintf);
    RUN_TEST(test_brprintf_BrSPrintfN);
    RUN_TEST(test_brprintf_BrLogPrintf);
    RUN_TEST(test_brprintf_BrSScanf);
}
