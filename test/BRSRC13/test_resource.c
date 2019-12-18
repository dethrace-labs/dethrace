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
    parent_header = (resource_header*)(((char*)parent) - sizeof(resource_header) - 4);
    TEST_ASSERT_EQUAL_UINT32(0xDEADBEEF, parent_header->magic_num);
    TEST_ASSERT_NULL(parent_header->children.head);
    TEST_ASSERT_EQUAL_CHAR(BR_MEMORY_ANCHOR, parent_header->class);
    TEST_ASSERT_EQUAL_PTR(parent_header, parent_header->magic_ptr);

    child = BrResAllocate(parent, 0, BR_MEMORY_ANCHOR);
    child_header = (resource_header*)(((char*)child) - sizeof(resource_header) - 4);
    TEST_ASSERT_EQUAL_PTR(&child_header->node, parent_header->children.head);
    TEST_ASSERT_NULL(parent_header->children.head->next);
    // ? TEST_ASSERT_NULL(parent_header->children.head->prev);
}

void test_resource_suite() {
    RUN_TEST(test_resource_BrResAllocate);
}