#include "framework/unity.h"

#include "CORE/FW/resource.h"
#include <string.h>

void test_resource_BrResAllocate() {
    void* parent;
    void* child;
    resource_header* parent_header;
    resource_header* child_header;

    parent = BrResAllocate(NULL, 0, BR_MEMORY_ANCHOR);
    TEST_ASSERT_NOT_NULL(parent);

    // A brender resource consists of a header and a body. The pointer to the body is what is returned to the caller,
    // so we have to jump the pointer backwards to inspect the header.
    parent_header = (resource_header*)(((char*)parent) - sizeof(resource_header) - 4);
    TEST_ASSERT_EQUAL_UINT32(0xDEADBEEF, parent_header->magic_num);
    TEST_ASSERT_NULL(parent_header->children.head);
    TEST_ASSERT_EQUAL_CHAR(BR_MEMORY_ANCHOR, parent_header->class);
    TEST_ASSERT_EQUAL_PTR(parent_header, parent_header->magic_ptr);

    child = BrResAllocate(parent, 0, BR_MEMORY_ANCHOR);
    child_header = (resource_header*)(((char*)child) - sizeof(resource_header) - 4);
    TEST_ASSERT_EQUAL_PTR(&child_header->node, parent_header->children.head);
    TEST_ASSERT_NULL(parent_header->children.head->next);
}

void test_resource_BrResFree() {
    void* r;
    br_file* child;
    void* child2;

    // simple case
    r = BrResAllocate(NULL, 0, BR_MEMORY_ANCHOR);
    TEST_ASSERT_NOT_NULL(r);
    printf("Got res allocated at %p\n", r);
    BrResFree(r);
}

void test_resource_BrResFree2() {
    br_file* child;
    child = BrResAllocate(NULL, sizeof(br_file), BR_MEMORY_FILE);
    child->raw_file = 0x1;
    TEST_ASSERT_NOT_NULL(child);
    BrResAssert(child);
}

void test_resource_BrResFree_Child() {
    void* r;
    void* child;
    resource_header* header;

    r = BrResAllocate(NULL, 0, BR_MEMORY_ANCHOR);
    TEST_ASSERT_NOT_NULL(r);
    child = BrResAllocate(r, 0, BR_MEMORY_ANCHOR);
    TEST_ASSERT_NOT_NULL(child);

    header = (resource_header*)(((char*)child) - sizeof(resource_header) - 4);
    TEST_ASSERT_EQUAL_INT(0xDEADBEEF, header->magic_num);
    BrResFree(r);
    // when the res is free'd, magic_num is set to 1. We make sure the child was free'd when the parent was
    TEST_ASSERT_EQUAL_INT(1, header->magic_num);

    // And the parent should have the child unlinked from its list of children
    header = (resource_header*)(((char*)r) - sizeof(resource_header) - 4);
    TEST_ASSERT_NULL(header->children.head);
}

void test_resource_suite() {
    RUN_TEST(test_resource_BrResAllocate);
    RUN_TEST(test_resource_BrResFree);
    RUN_TEST(test_resource_BrResFree2);
    RUN_TEST(test_resource_BrResFree_Child);
}