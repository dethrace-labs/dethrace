#include "tests.h"

#include "CORE/FW/brlists.h"
#include <stdlib.h>
#include <string.h>

static void test_brlists_BrNewList() {
    br_list* list = calloc(1, sizeof(br_list));

    BrNewList(list);
    TEST_ASSERT_NOT_NULL(list->head);
    TEST_ASSERT_NULL(list->_null);
    TEST_ASSERT_NOT_NULL(list->tail);

    free(list);
}

static void test_brlists_BrAddHead() {
    br_list* list = calloc(1, sizeof(br_list));
    br_node* one = calloc(1, sizeof(br_node));
    br_node* two = calloc(1, sizeof(br_node));

    BrNewList(list);

    BrAddHead(list, one);
    TEST_ASSERT_EQUAL_PTR(one, list->head);
    TEST_ASSERT_EQUAL_PTR(one, list->tail);
    TEST_ASSERT_EQUAL_PTR(one->prev, (br_node*)&list->head);
    TEST_ASSERT_EQUAL_PTR(one->next, (br_node*)&list->_null);

    BrAddHead(list, two);
    TEST_ASSERT_EQUAL_PTR(two, list->head);
    TEST_ASSERT_EQUAL_PTR(one, list->tail);
    TEST_ASSERT_EQUAL_PTR(two->prev, (br_node*)&list->head);
    TEST_ASSERT_EQUAL_PTR(two->next, one);
    TEST_ASSERT_EQUAL_PTR(one->prev, two);
    TEST_ASSERT_EQUAL_PTR(one->next, (br_node*)&list->_null);

    free(list);
    free(one);
    free(two);
}

static void test_brlists_BrAddTail() {
    br_list* list = calloc(1, sizeof(br_list));
    br_node* one = calloc(1, sizeof(br_node));
    br_node* two = calloc(1, sizeof(br_node));

    BrNewList(list);

    BrAddTail(list, one);
    TEST_ASSERT_EQUAL_PTR(one, list->head);
    TEST_ASSERT_EQUAL_PTR(one, list->tail);
    TEST_ASSERT_EQUAL_PTR(one->prev, (br_node*)&list->head);
    TEST_ASSERT_EQUAL_PTR(one->next, (br_node*)&list->_null);

    BrAddTail(list, two);
    TEST_ASSERT_EQUAL_PTR(one, list->head);
    TEST_ASSERT_EQUAL_PTR(two, list->tail);
    TEST_ASSERT_EQUAL_PTR(two->prev, one);
    TEST_ASSERT_EQUAL_PTR(two->next, (br_node*)&list->_null);
    TEST_ASSERT_EQUAL_PTR(one->prev, (br_node*)&list->head);
    TEST_ASSERT_EQUAL_PTR(one->next, two);

    free(list);
    free(one);
    free(two);
}

static void test_brlists_BrRemHead() {
    br_list* list = calloc(1, sizeof(br_list));
    br_node* one = calloc(1, sizeof(br_node));
    br_node* two = calloc(1, sizeof(br_node));
    br_node* removed;

    BrNewList(list);

    removed = BrRemHead(list);
    TEST_ASSERT_NULL(removed);

    BrAddTail(list, one);
    BrAddTail(list, two);
    TEST_ASSERT_EQUAL_PTR(one, list->head);

    removed = BrRemHead(list);
    TEST_ASSERT_EQUAL_PTR(one, removed);
    TEST_ASSERT_EQUAL_PTR(two, list->head);
    TEST_ASSERT_EQUAL_PTR(two, list->tail);
    TEST_ASSERT_EQUAL_PTR((br_node*)&list->head, two->prev);
    TEST_ASSERT_EQUAL_PTR((br_node*)&list->_null, two->next);

    removed = BrRemHead(list);
    TEST_ASSERT_EQUAL_PTR(two, removed);
    TEST_ASSERT_EQUAL_PTR((br_node*)&list->_null, list->head);
    TEST_ASSERT_EQUAL_PTR((br_node*)&list->head, list->tail);

    free(list);
    free(one);
    free(two);
}

static void test_brlists_BrRemTail() {
    br_list* list = calloc(1, sizeof(br_list));
    br_node* one = calloc(1, sizeof(br_node));
    br_node* two = calloc(1, sizeof(br_node));
    br_node* removed;

    BrNewList(list);

    removed = BrRemTail(list);
    TEST_ASSERT_NULL(removed);

    BrAddTail(list, one);
    BrAddTail(list, two);
    TEST_ASSERT_EQUAL_PTR(two, list->tail);

    removed = BrRemTail(list);
    TEST_ASSERT_EQUAL_PTR(two, removed);
    TEST_ASSERT_EQUAL_PTR(one, list->head);
    TEST_ASSERT_EQUAL_PTR(one, list->tail);
    TEST_ASSERT_EQUAL_PTR((br_node*)&list->head, one->prev);
    TEST_ASSERT_EQUAL_PTR((br_node*)&list->_null, one->next);

    removed = BrRemHead(list);
    TEST_ASSERT_EQUAL_PTR(one, removed);
    TEST_ASSERT_EQUAL_PTR((br_node*)&list->_null, list->head);
    TEST_ASSERT_EQUAL_PTR((br_node*)&list->head, list->tail);

    free(list);
    free(one);
    free(two);
}

static void test_brlists_BrInsert() {
    br_list* list = calloc(1, sizeof(br_list));
    br_node* one = calloc(1, sizeof(br_node));
    br_node* two = calloc(1, sizeof(br_node));
    br_node* three = calloc(1, sizeof(br_node));

    BrNewList(list);

    BrInsert(list, (br_node*)&list->head, one);
    TEST_ASSERT_EQUAL_PTR(one, list->head);
    TEST_ASSERT_EQUAL_PTR(one, list->tail);
    TEST_ASSERT_EQUAL_PTR((br_node*)&list->head, one->prev);
    TEST_ASSERT_EQUAL_PTR((br_node*)&list->_null, one->next);

    BrInsert(list, one, two);
    TEST_ASSERT_EQUAL_PTR(two, one->next);
    TEST_ASSERT_EQUAL_PTR(two, list->tail);
    TEST_ASSERT_EQUAL_PTR(one, two->prev);
    TEST_ASSERT_EQUAL_PTR(two, one->next);

    BrInsert(list, one, three);
    TEST_ASSERT_EQUAL_PTR(three, one->next);
    TEST_ASSERT_EQUAL_PTR(three, two->prev);
    TEST_ASSERT_EQUAL_PTR(one, three->prev);
    TEST_ASSERT_EQUAL_PTR(two, three->next);

    free(list);
    free(one);
    free(two);
    free(three);
}

static void test_brlists_BrRemove() {
    br_list* list = calloc(1, sizeof(br_list));
    br_node* one = calloc(1, sizeof(br_node));
    br_node* two = calloc(1, sizeof(br_node));
    br_node* three = calloc(1, sizeof(br_node));
    br_node *removed;

    BrNewList(list);
    BrAddTail(list, one);
    TEST_ASSERT_EQUAL_PTR(one, list->head);
    TEST_ASSERT_EQUAL_PTR(one, list->tail);

    removed = BrRemove(one);
    TEST_ASSERT_EQUAL_PTR(one, removed);
    TEST_ASSERT_EQUAL_PTR((br_node*)&list->_null, list->head);
    TEST_ASSERT_EQUAL_PTR((br_node*)&list->head, list->tail);

    BrAddTail(list, one);
    BrAddTail(list, two);
    BrAddTail(list, three);

    removed = BrRemove(two);
    TEST_ASSERT_EQUAL_PTR(two, removed);
    TEST_ASSERT_EQUAL_PTR(three, one->next);
    TEST_ASSERT_EQUAL_PTR(one, three->prev);

    removed = BrRemove(three);
    TEST_ASSERT_EQUAL_PTR(three, removed);
    TEST_ASSERT_EQUAL_PTR((br_node*)&list->_null, one->next);
    TEST_ASSERT_EQUAL_PTR(one, list->tail);

    removed = BrRemove(one);
    TEST_ASSERT_EQUAL_PTR(one, removed);
    TEST_ASSERT_EQUAL_PTR((br_node*)&list->_null, list->head);
    TEST_ASSERT_EQUAL_PTR((br_node*)&list->head, list->tail);

    free(list);
    free(one);
    free(two);
    free(three);
}

static void test_brlists_BrSimpleNewList() {
    br_simple_list* list = calloc(1, sizeof(br_simple_list));

    BrSimpleNewList(list);
    TEST_ASSERT_NULL(list->head);

    free(list);
}

static void test_brlists_BrSimpleAddHead() {
    br_simple_list* list = calloc(1, sizeof(br_simple_list));
    br_simple_node* one = calloc(1, sizeof(br_simple_node));
    br_simple_node* two = calloc(1, sizeof(br_simple_node));

    BrSimpleNewList(list);

    BrSimpleAddHead(list, one);
    TEST_ASSERT_EQUAL_PTR(one, list->head);
    TEST_ASSERT_EQUAL_PTR(&list->head, one->prev);
    TEST_ASSERT_NULL(one->next);

    BrSimpleAddHead(list, two);
    TEST_ASSERT_EQUAL_PTR(two, list->head);
    TEST_ASSERT_EQUAL_PTR(&list->head, two->prev);
    TEST_ASSERT_EQUAL_PTR(one, two->next);
    TEST_ASSERT_EQUAL_PTR(two, one->prev);

    free(list);
    free(one);
    free(two);
}

static void test_brlists_BrSimpleRemHead() {
    br_simple_list* list = calloc(1, sizeof(br_simple_list));
    br_simple_node* one = calloc(1, sizeof(br_simple_node));
    br_simple_node* two = calloc(1, sizeof(br_simple_node));
    br_simple_node* removed;

    BrSimpleNewList(list);
    BrSimpleAddHead(list, one);
    BrSimpleAddHead(list, two);

    TEST_ASSERT_EQUAL_PTR(two, list->head);
    TEST_ASSERT_EQUAL_PTR(two, one->prev);
    TEST_ASSERT_EQUAL_PTR((br_simple_node**)&list->head, two->prev);
    TEST_ASSERT_EQUAL_PTR(one, two->next);

    removed = BrSimpleRemHead(list);
    TEST_ASSERT_EQUAL(two, removed);
    TEST_ASSERT_EQUAL_PTR(one, list->head);
    TEST_ASSERT_EQUAL_PTR((br_simple_node**)&list->head, one->prev);
    TEST_ASSERT_NULL(two->prev);
    TEST_ASSERT_NULL(two->next);

    removed = BrSimpleRemHead(list);
    TEST_ASSERT_EQUAL(one, removed);
    TEST_ASSERT_NULL(list->head);
    TEST_ASSERT_NULL(one->prev);
    TEST_ASSERT_NULL(one->next);

    free(list);
    free(one);
    free(two);
}

static void test_brlists_BrSimpleInsert() {
    br_simple_list* list = calloc(1, sizeof(br_simple_list));
    br_simple_node* one = calloc(1, sizeof(br_simple_node));
    br_simple_node* two = calloc(1, sizeof(br_simple_node));
    br_simple_node* three = calloc(1, sizeof(br_simple_node));

    BrSimpleNewList(list);
    TEST_ASSERT_NULL(list->head);

    BrSimpleAddHead(list, one);
    TEST_ASSERT_EQUAL_PTR(one, list->head);
    TEST_ASSERT_EQUAL_PTR((br_simple_node**)&list->head, one->prev);
    TEST_ASSERT_NULL(one->next);

    BrSimpleInsert(list, one, two);
    TEST_ASSERT_EQUAL_PTR(one, list->head);
    TEST_ASSERT_EQUAL_PTR((br_simple_node**)&list->head, one->prev);
    TEST_ASSERT_EQUAL_PTR(two, one->next);
    TEST_ASSERT_EQUAL_PTR(one, two->prev);
    TEST_ASSERT_NULL(two->next);

    BrSimpleInsert(list, one, three);
    TEST_ASSERT_EQUAL_PTR(three, one->next);
    TEST_ASSERT_EQUAL_PTR(three, two->prev);
    TEST_ASSERT_EQUAL_PTR(one, three->prev);
    TEST_ASSERT_EQUAL_PTR(two, three->next);

    free(list);
    free(one);
    free(two);
    free(three);
}

static void test_brlists_BrSimpleRemove() {
    br_simple_list* list = calloc(1, sizeof(br_simple_list));
    br_simple_node* one = calloc(1, sizeof(br_simple_node));
    br_simple_node* two = calloc(1, sizeof(br_simple_node));
    br_simple_node* three = calloc(1, sizeof(br_simple_node));
    br_simple_node *removed;

    BrSimpleNewList(list);
    BrSimpleAddHead(list, one);
    TEST_ASSERT_EQUAL_PTR(one, list->head);
    TEST_ASSERT_EQUAL_PTR((br_simple_node**)&list->head, one->prev);
    TEST_ASSERT_NULL(one->next);

    removed = BrSimpleRemove(one);
    TEST_ASSERT_EQUAL_PTR(one, removed);
    TEST_ASSERT_NULL(one->next);
    TEST_ASSERT_NULL(one->prev);
    TEST_ASSERT_NULL(list->head);

    BrSimpleAddHead(list, one);
    BrSimpleAddHead(list, two);

    removed = BrSimpleRemove(two);
    TEST_ASSERT_EQUAL_PTR(two, removed);
    TEST_ASSERT_NULL(two->next);
    TEST_ASSERT_NULL(two->prev);
    TEST_ASSERT_EQUAL_PTR(one, list->head);
    TEST_ASSERT_EQUAL_PTR((br_simple_node**)&list->head, one->prev);

    BrSimpleAddHead(list, two);
    BrSimpleAddHead(list, three);

    removed = BrSimpleRemove(two);
    TEST_ASSERT_EQUAL_PTR(two, removed);
    TEST_ASSERT_NULL(two->next);
    TEST_ASSERT_NULL(two->prev);
    TEST_ASSERT_EQUAL_PTR(three, list->head);
    TEST_ASSERT_EQUAL_PTR(one, three->next);
    TEST_ASSERT_EQUAL_PTR((br_simple_node*)&list->head, three->prev);
    TEST_ASSERT_EQUAL_PTR(three, one->prev);
    TEST_ASSERT_NULL(one->next);

    free(list);
    free(one);
    free(two);
    free(three);
}

void test_brlists_suite() {
    RUN_TEST(test_brlists_BrNewList);
    RUN_TEST(test_brlists_BrAddHead);
    RUN_TEST(test_brlists_BrAddTail);
    RUN_TEST(test_brlists_BrRemHead);
    RUN_TEST(test_brlists_BrRemTail);
    RUN_TEST(test_brlists_BrInsert);
    RUN_TEST(test_brlists_BrRemove);
    RUN_TEST(test_brlists_BrSimpleNewList);
    RUN_TEST(test_brlists_BrSimpleAddHead);
    RUN_TEST(test_brlists_BrSimpleRemHead);
    RUN_TEST(test_brlists_BrSimpleInsert);
    RUN_TEST(test_brlists_BrSimpleRemove);
}
