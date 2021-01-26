#include "tests.h"

#include "common/globvars.h"
#include "common/init.h"

void test_init_AllocateCamera() {
    AllocateCamera();
    TEST_ASSERT_NOT_NULL(gRearview_camera);
}

void test_init_suite() {
    RUN_TEST(test_init_AllocateCamera);
}