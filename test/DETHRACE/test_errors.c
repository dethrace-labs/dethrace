#include "tests.h"

#include "common/errors.h"
#include "pc-dos/dossys.h"
#include <string.h>

void test_errors_FatalError() {
    _unittest_do_not_exit = 1;
    FatalError(0x6b, "test_errors", "FATAL");
    TEST_ASSERT_EQUAL_STRING("Can't open 'test_errors'", _unittest_last_fatal_error);
}

void test_errors_suite() {
    RUN_TEST(test_errors_FatalError);
}
