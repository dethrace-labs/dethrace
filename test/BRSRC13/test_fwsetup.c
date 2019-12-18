#include "framework/unity.h"

#include "CORE/FW/fwsetup.h"
#include <string.h>

void test_fwsetup_BrFwBegin() {
    TEST_ASSERT_TRUE(fw.active);
}

void test_fwsetup_suite() {
    RUN_TEST(test_fwsetup_BrFwBegin);
}