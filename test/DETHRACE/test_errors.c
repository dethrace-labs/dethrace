#include "tests.h"

#include "common/errors.h"
#include "pd/sys.h"
#include <string.h>

void test_errors_FatalError() {
    FatalError(107, "test_errors", "FATAL");
    TEST_ASSERT_EQUAL_STRING("Can't open 'test_errors'", _unittest_last_fatal_error);
}

void test_errors_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_errors_FatalError);
}
