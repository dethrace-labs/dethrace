#include "framework/unity.h"

#include "CORE/V1DB/actsupt.h"
#include "common/globvars.h"
#include "common/init.h"

void test_init_AllocateCamera() {
    gSelf = BrActorAllocate(BR_ACTOR_NONE, NULL);
    gUniverse_actor = BrActorAllocate(BR_ACTOR_NONE, NULL);
    AllocateCamera();
    TEST_ASSERT_NOT_NULL(gRearview_camera);
}

void test_init_suite() {
    RUN_TEST(test_init_AllocateCamera);
}