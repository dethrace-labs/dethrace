#include "CORE/FW/scratch.h"
#include "CORE/FW/fwsetup.h"
#include "tests.h"
#include <stddef.h>

static void test_scratch_BrScratchAllocate_BrScratchFree_BrScratchInquire() {
    void *block = NULL;

    TEST_ASSERT_FALSE(fw.scratch_inuse);

    block = BrScratchAllocate(0x100);

    TEST_ASSERT_NOT_NULL(block);
    TEST_ASSERT_TRUE(fw.scratch_inuse);
    TEST_ASSERT_EQUAL(0x100, fw.scratch_last);
    TEST_ASSERT_EQUAL(block, fw.scratch_ptr);
    TEST_ASSERT_LESS_OR_EQUAL_size_t(0x100, BrScratchInquire());

    BrScratchFree(block);

    TEST_ASSERT_EQUAL(block, fw.scratch_ptr);
    TEST_ASSERT_FALSE(fw.scratch_inuse);
}

static void test_scratch_BrScratchFree_NULL() {;
    br_size_t scratch_size;
    void *block = NULL;

    TEST_ASSERT_FALSE(fw.scratch_inuse);

    block = BrScratchAllocate(0x100);

    TEST_ASSERT_TRUE(fw.scratch_inuse);

    BrScratchFree(block);
    block = NULL;

    TEST_ASSERT_FALSE(fw.scratch_inuse);
}

static void test_scratch_BrScratchFlush() {;
    br_size_t scratch_size;
    void *block = NULL;

    TEST_ASSERT_FALSE(fw.scratch_inuse);

    block = BrScratchAllocate(0x100);

    TEST_ASSERT_TRUE(fw.scratch_inuse);
    TEST_ASSERT_EQUAL(block, fw.scratch_ptr);

    BrScratchFree(block);

    TEST_ASSERT_FALSE(fw.scratch_inuse);
    TEST_ASSERT_EQUAL(block, fw.scratch_ptr);

    BrScratchFlush();

    TEST_ASSERT_FALSE(fw.scratch_inuse);
    TEST_ASSERT_EQUAL(NULL, fw.scratch_ptr);
}

static void test_scratch_BrScratchSize() {
    TEST_ASSERT_EQUAL(0x200, BrScratchStringSize());
}

static void test_scratch_BrScratchString() {
    int i;
    char *text1;
    char *text2;

    text1 = NULL;
    text2 = NULL;

    text1 = BrScratchString();

    TEST_ASSERT_NOT_NULL(text1);

    for (int i = 0; i < 0x200; i++) {
        text1[i] = (char)i;
    }

    text2 = BrScratchString();

    TEST_ASSERT_NOT_NULL(text2);
    TEST_ASSERT_EQUAL(text1, text2);

    for (int i = 0; i < 0x200; i++) {
        TEST_ASSERT_EQUAL_UINT8(i, text2[i]);
    }
}

void test_scratch_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_scratch_BrScratchAllocate_BrScratchFree_BrScratchInquire);
    RUN_TEST(test_scratch_BrScratchFree_NULL);
    RUN_TEST(test_scratch_BrScratchFlush);
    RUN_TEST(test_scratch_BrScratchSize);
    RUN_TEST(test_scratch_BrScratchString);
}
