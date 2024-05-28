#include "tests.h"

#include "common/graphics.h"

void test_graphics_loadfont() {
    REQUIRES_DATA_DIRECTORY();

    TEST_ASSERT_EQUAL_INT(0, gFonts[kFont_TYPEABLE].file_read_once);
    LoadFont(kFont_TYPEABLE);
    TEST_ASSERT_EQUAL_INT(1, gFonts[kFont_TYPEABLE].file_read_once);
    TEST_ASSERT_NOT_NULL(gFonts[kFont_TYPEABLE].images);
}

void test_graphics_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_graphics_loadfont);
}
