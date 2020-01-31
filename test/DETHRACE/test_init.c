#include "framework/unity.h"

#include "common/globvars.h"
#include "common/init.h"

void test_init_AllocateCamera() {
    AllocateCamera();
}

void test_init_suite() {
    RUN_TEST(test_init_AllocateCamera);
    TEST_ASSERT_NOT_NULL(gRearview_camera);
}