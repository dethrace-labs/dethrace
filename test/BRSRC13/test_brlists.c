#include "framework/unity.h"

#include "CORE/FW/brlists.h"
#include <string.h>

void test_brlists_BrSimpleList() {
    br_simple_list list;
    br_simple_node one;
    br_simple_node two;
    br_simple_node three;

    BrSimpleNewList(&list);
    TEST_ASSERT_NULL(list.head);

    BrSimpleAddHead(&list, &one);
    TEST_ASSERT_EQUAL_PTR(&one, list.head);
    TEST_ASSERT_NULL(list.head->next);

    //TODO: should be null?
    TEST_ASSERT_EQUAL_PTR(&one, *list.head->prev);

    BrSimpleAddHead(&list, &two);
    TEST_ASSERT_EQUAL_PTR(&two, list.head);
    TEST_ASSERT_EQUAL_PTR(&one, list.head->next);

    //TODO: this should be "2" IMO
    TEST_ASSERT_EQUAL_PTR(&one, *list.head->next->prev);

    BrSimpleAddHead(&list, &three);
    TEST_ASSERT_EQUAL_PTR(&three, list.head);
    TEST_ASSERT_EQUAL_PTR(&two, list.head->next);
    TEST_ASSERT_EQUAL_PTR(&one, list.head->next->next);

    //TODO: this doesnt look right
    //TEST_ASSERT_EQUAL_PTR(&three, *list.head->prev);
    TEST_ASSERT_EQUAL_PTR(&one, *list.head->next->prev);
    TEST_ASSERT_EQUAL_PTR(&one, *list.head->next->next->prev);
}

void test_brlists_suite() {
    RUN_TEST(test_brlists_BrSimpleList);
}