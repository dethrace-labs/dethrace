#include "framework/unity.h"

#include <string.h>
#include "common/errors.h"
#include "pc-dos/dossys.h"


void test_errors_FatalError() {
    _unittest_do_not_exit = 1;
    FatalError(0x6b, "hello", "world");
}

void test_errors_suite() {
    RUN_TEST(test_errors_FatalError);
}
