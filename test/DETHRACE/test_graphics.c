#include "tests.h"

#include "common/graphics.h"

void test_graphics_loadfont() {
    REQUIRES_DATA_DIRECTORY();

    TEST_ASSERT_EQUAL_INT(0, gFonts[0].file_read_once);
    LoadFont(0);
    TEST_ASSERT_EQUAL_INT(1, gFonts[0].file_read_once);
    TEST_ASSERT_NOT_NULL(gFonts[0].images);
}

void test_graphics_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_graphics_loadfont);
}
