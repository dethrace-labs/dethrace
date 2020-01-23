#include "brender.h"
#include "framework/unity.h"
#include <stddef.h>

void test_actsupt_BrActorAllocate() {
    br_actor* a;
    a = BrActorAllocate(BR_ACTOR_LIGHT, NULL);
    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_EQUAL_INT(BR_ACTOR_LIGHT, a->type);
    a = BrActorAllocate(BR_ACTOR_NONE, NULL);
    TEST_ASSERT_NOT_NULL(a);

    a = BrActorAllocate(BR_ACTOR_CAMERA, NULL);
    TEST_ASSERT_NOT_NULL(a);

    a = BrActorAllocate(BR_ACTOR_BOUNDS, NULL);
    TEST_ASSERT_NOT_NULL(a);

    a = BrActorAllocate(BR_ACTOR_BOUNDS_CORRECT, NULL);
    TEST_ASSERT_NOT_NULL(a);

    a = BrActorAllocate(BR_ACTOR_CLIP_PLANE, NULL);
    TEST_ASSERT_NOT_NULL(a);
}

void test_actsupt_suite() {
    RUN_TEST(test_actsupt_BrActorAllocate);
}