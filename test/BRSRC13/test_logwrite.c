#include "tests.h"
#include "CORE/STD/logwrite.h"

static void test_logwrite_BrLogWrite() {
    char* msg;
    int n;
#define MSG "A message to print\n"
    msg = MSG;
    n = BrLogWrite(msg, 1, sizeof(MSG));
    TEST_ASSERT_EQUAL_INT(sizeof(MSG), n);
}

void test_logwrite_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_logwrite_BrLogWrite);
}
