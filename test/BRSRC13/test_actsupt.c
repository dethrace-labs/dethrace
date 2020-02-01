#include "brender.h"
#include "framework/unity.h"
#include <stddef.h>

void test_actsupt_BrActorAllocateAndFree() {
    br_actor* a;
    a = BrActorAllocate(BR_ACTOR_LIGHT, NULL);
    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_EQUAL_INT(BR_ACTOR_LIGHT, a->type);
    TEST_ASSERT_NULL(a->children);
    BrActorFree(a);

    a = BrActorAllocate(BR_ACTOR_NONE, NULL);
    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_NULL(a->type_data)

    a = BrActorAllocate(BR_ACTOR_CAMERA, NULL);
    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_EQUAL_FLOAT(((br_camera*)a->type_data)->yon_z, 10.f);
    BrActorFree(a);

    a = BrActorAllocate(BR_ACTOR_BOUNDS, NULL);
    BrActorFree(a);

    a = BrActorAllocate(BR_ACTOR_BOUNDS_CORRECT, NULL);
    BrActorFree(a);

    a = BrActorAllocate(BR_ACTOR_CLIP_PLANE, NULL);
    BrActorFree(a);
}

void test_actsupt_BrActorAllocateAndFreeChild() {
    br_actor* a;
    a = BrActorAllocate(BR_ACTOR_CAMERA, NULL);
    a->identifier = BrResStrDup(a, "actor_name");
    BrActorFree(a);
}

void test_actsupt_BrActorAdd() {
    br_actor* a;
    br_actor* b;
    a = BrActorAllocate(BR_ACTOR_NONE, NULL);
    b = BrActorAllocate(BR_ACTOR_NONE, NULL);
    BrActorAdd(a, b);
    TEST_ASSERT_EQUAL_INT(0, a->depth);
    TEST_ASSERT_EQUAL_INT(1, b->depth);
    TEST_ASSERT_EQUAL_PTR(b, a->children);
    TEST_ASSERT_EQUAL_PTR(a, b->parent);
    TEST_ASSERT_NULL(a->children->next);
    TEST_ASSERT_NULL(a->children->children);
}

void test_actsupt_suite() {
    RUN_TEST(test_actsupt_BrActorAllocateAndFree);
    RUN_TEST(test_actsupt_BrActorAllocateAndFreeChild);
    RUN_TEST(test_actsupt_BrActorAdd);
}