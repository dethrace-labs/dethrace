#include "tests.h"

#include <string.h>
#include <unistd.h>

#include "common/globvars.h"
#include "common/powerup.h"

void test_loading_powerups() {
    REQUIRES_DATA_DIRECTORY();

    LoadPowerups();
    TEST_ASSERT_EQUAL_INT(20, gnumber_of)
}

void test_loading_suite() {
    RUN_TEST(test_loading_powerups);
}
