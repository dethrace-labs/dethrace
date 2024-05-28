#include "tests.h"

#include "CORE/FW/resource.h"
#include <string.h>

static void test_resource_BrResAllocate() {
    void* parent;
    void* child;
    resource_header* parent_header;
    resource_header* child_header;

    parent = BrResAllocate(NULL, 0, BR_MEMORY_ANCHOR);
    TEST_ASSERT_NOT_NULL(parent);

    // A brender resource consists of a header and a body. The pointer to the body is what is returned to the caller,
    // so we have to jump the pointer backwards to inspect the header.
    parent_header = UserToRes(parent);
    TEST_ASSERT_EQUAL_UINT32(0xdeadbeef, parent_header->magic_num);
    TEST_ASSERT_NULL(parent_header->children.head);
    TEST_ASSERT_EQUAL_CHAR(BR_MEMORY_ANCHOR, parent_header->class);
    TEST_ASSERT_EQUAL_PTR(parent_header, parent_header->magic_ptr);
    TEST_ASSERT_EQUAL_PTR(parent, ResToUser(parent_header));
    TEST_ASSERT_TRUE(BrResCheck(parent, 0));

    child = BrResAllocate(parent, 0, BR_MEMORY_ANCHOR);
    child_header = UserToRes(child);
    TEST_ASSERT_EQUAL_PTR(&child_header->node, parent_header->children.head);
    TEST_ASSERT_NULL(parent_header->children.head->next);
    TEST_ASSERT_EQUAL_PTR(child, ResToUser(child_header));
    TEST_ASSERT_TRUE(BrResCheck(child, 0));
}

static void test_resource_BrResFree() {
    void* r;
    br_file* child;
    void* child2;

    // simple case
    r = BrResAllocate(NULL, 0, BR_MEMORY_ANCHOR);
    TEST_ASSERT_NOT_NULL(r);
    printf("Got res allocated at %p\n", r);
    TEST_ASSERT_TRUE(BrResCheck(r, 0));
    BrResFree(r);
}

static void test_resource_BrResFree2() {
    br_file* child;
    resource_header* childhdr;
    child = BrResAllocate(NULL, sizeof(br_file), BR_MEMORY_FILE);
    TEST_ASSERT_NOT_NULL(child);
    child->raw_file = (void*)0x1;
    childhdr = UserToRes(child);
    if (childhdr->magic_num != 0xdeadbeef) {
        LOG_PANIC("Bad resource header at %p. Was %X", childhdr, ((resource_header*)childhdr)->magic_num);
    }
    TEST_ASSERT_TRUE(BrResCheck(child, 0));
}

static void test_resource_BrResFree_Child() {
    void* r;
    void* child;
    resource_header* header;

    r = BrResAllocate(NULL, 0, BR_MEMORY_ANCHOR);
    TEST_ASSERT_NOT_NULL(r);
    child = BrResAllocate(r, 0, BR_MEMORY_ANCHOR);
    TEST_ASSERT_NOT_NULL(child);

    TEST_ASSERT_TRUE(BrResIsChild(r, child));

    header = UserToRes(child);
    TEST_ASSERT_EQUAL_INT(0xdeadbeef, header->magic_num);

    header = UserToRes(r);
    BrResFree(r);

    /* Accessing freed memory is undefined behavior. So only do this on platforms that don't use an MMU. */
#if defined(__DOS__)
    // when the res is free'd, magic_num is set to 1. We make sure the child was free'd when the parent was
    TEST_ASSERT_EQUAL_INT(1, header->magic_num);

    // And the parent should have the child unlinked from its list of children
    TEST_ASSERT_NULL(header->children.head);
#endif
}

static void test_resource_BrResStrDup() {
    char* str;
    
    str = BrResStrDup(NULL, "some input string");
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_EQUAL_STRING("some input string", str);

    BrResFree(str);
}

static void test_resource_BrResClassIdentifier() {
    TEST_ASSERT_EQUAL_STRING("CAMERA", BrResClassIdentifier(BR_MEMORY_CAMERA));
    TEST_ASSERT_EQUAL_STRING("DATAFILE", BrResClassIdentifier(BR_MEMORY_DATAFILE));
}

#define RESDUMP_DATA_LINE_CAPACITY 16
struct btest_resdump_data {
    char buffer[RESDUMP_DATA_LINE_CAPACITY][128];
    int size;
};

static void test_brresdump_putline_func(char* line, void* data) {
    struct btest_resdump_data *rdata = data;

    LOG_DEBUG(line);
    TEST_ASSERT_LESS_THAN(RESDUMP_DATA_LINE_CAPACITY, rdata->size);
    strcpy(rdata->buffer[rdata->size], line);
    rdata->size++;
}

static void test_resource_BrResDump() {
    void* vparent;
    void* child1;
    void* child2;
    struct btest_resdump_data data;

    vparent = BrResAllocate(NULL, 0, BR_MEMORY_ANCHOR);
    child1 = BrResAllocate(vparent, 10, BR_MEMORY_DATAFILE);
    child2 = BrResAllocate(vparent, 10, BR_MEMORY_FACES);
    BrResAllocate(child1, 20, BR_MEMORY_PIXELMAP);
    BrResAllocate(child1, 5, BR_MEMORY_VERTICES);
    BrResAllocate(child2, 13, BR_MEMORY_REGISTRY);
    BrResAllocate(child2, 17, BR_MEMORY_LEXER);

    memset(&data, 0, sizeof(data));
    BrResDump(vparent, test_brresdump_putline_func, &data);

    TEST_ASSERT_EQUAL_INT(7, data.size);
    TEST_ASSERT_NOT_NULL(strstr(data.buffer[0], "[ANCHOR]"));
    TEST_ASSERT_NOT_NULL(strstr(data.buffer[1], "[FACES]"));
    TEST_ASSERT_NOT_NULL(strstr(data.buffer[2], "[LEXER]"));
    TEST_ASSERT_NOT_NULL(strstr(data.buffer[3], "[REGISTRY]"));
    TEST_ASSERT_NOT_NULL(strstr(data.buffer[4], "[DATAFILE]"));
    TEST_ASSERT_NOT_NULL(strstr(data.buffer[5], "[VERTICES]"));
    TEST_ASSERT_NOT_NULL(strstr(data.buffer[6], "[PIXELMAP]"));

    BrResFree(vparent);
}

static void test_resource_BrResSize_BrResSizeTotal() {
    void* vparent;
    void* child1;
    void* child2;
    br_uint_32 size_parent;
    br_uint_32 size_child1;
    br_uint_32 size_child2;

    vparent = BrResAllocate(NULL, 0, BR_MEMORY_ANCHOR);
    TEST_ASSERT_NOT_NULL(vparent);
    size_parent = BrResSize(vparent);
    TEST_ASSERT_GREATER_THAN_UINT32(0, size_parent);
    TEST_ASSERT_EQUAL_UINT32(size_parent, BrResSizeTotal(vparent));

    child1 = BrResAllocate(vparent, 10, BR_MEMORY_DATAFILE);
    TEST_ASSERT_NOT_NULL(child1);
    size_child1 = BrResSize(child1);
    TEST_ASSERT_GREATER_THAN_UINT32(0, size_child1);
    TEST_ASSERT_EQUAL(size_parent, BrResSize(vparent));
    TEST_ASSERT_EQUAL(size_parent + size_child1, BrResSizeTotal(vparent));

    child2 = BrResAllocate(child1, 10, BR_MEMORY_DATAFILE);
    TEST_ASSERT_NOT_NULL(child2);
    size_child2 = BrResSize(child2);
    TEST_ASSERT_GREATER_THAN_UINT32(0, size_child2);
    TEST_ASSERT_EQUAL(size_parent, BrResSize(vparent));
    TEST_ASSERT_EQUAL(size_child1, BrResSize(child1));
    TEST_ASSERT_EQUAL(size_parent + size_child1 + size_child2, BrResSizeTotal(vparent));

    BrResFree(vparent);
}

void test_resource_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_resource_BrResAllocate);
    RUN_TEST(test_resource_BrResFree);
    RUN_TEST(test_resource_BrResFree2);
    RUN_TEST(test_resource_BrResFree_Child);
    RUN_TEST(test_resource_BrResStrDup);
    RUN_TEST(test_resource_BrResClassIdentifier);
    RUN_TEST(test_resource_BrResDump);
    RUN_TEST(test_resource_BrResSize_BrResSizeTotal);
}
