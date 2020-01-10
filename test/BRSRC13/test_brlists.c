#include "framework/unity.h"

#include "CORE/FW/brlists.h"
#include <stdlib.h>
#include <string.h>

void test_brlists_BrSimpleList() {
    br_simple_list* list = calloc(1, sizeof(br_simple_list));
    br_simple_node* one = calloc(1, sizeof(br_simple_node));
    br_simple_node* two = calloc(1, sizeof(br_simple_node));
    br_simple_node* three = calloc(1, sizeof(br_simple_node));

    BrSimpleNewList(list);
    TEST_ASSERT_NULL(list->head);

    BrSimpleAddHead(list, one);
    // expected 1->null
    TEST_ASSERT_EQUAL_PTR(one, list->head);
    TEST_ASSERT_NULL(one->next);
    TEST_ASSERT_EQUAL_PTR(list, one->prev);

    BrSimpleAddHead(list, two);
    // expected 2->1->null
    TEST_ASSERT_EQUAL_PTR(two, list->head);
    TEST_ASSERT_EQUAL_PTR(one, two->next);
    TEST_ASSERT_EQUAL_PTR(list, two->prev);

    TEST_ASSERT_EQUAL_PTR(two, one->prev);
    TEST_ASSERT_NULL(one->next);

    BrSimpleAddHead(list, three);
    // expected 3->2->1->null
    TEST_ASSERT_EQUAL_PTR(two, one->prev);

    TEST_ASSERT_EQUAL_PTR(three, list->head);
    TEST_ASSERT_EQUAL_PTR(two, three->next);
    TEST_ASSERT_EQUAL_PTR(list, three->prev);

    TEST_ASSERT_EQUAL_PTR(three, two->prev);
    TEST_ASSERT_EQUAL_PTR(one, two->next);

    TEST_ASSERT_EQUAL_PTR(two, one->prev);
    TEST_ASSERT_NULL(one->next);

    free(list);
    free(one);
    free(two);
    free(three);
}

void test_brlists_suite() {
    RUN_TEST(test_brlists_BrSimpleList);
}