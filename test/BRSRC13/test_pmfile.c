#include "tests.h"

#include "CORE/PIXELMAP/pmfile.h"

void test_pmfile_BrPixelmapLoad() {
    REQUIRES_DATA_DIRECTORY();

    br_pixelmap* pm;
    pm = BrPixelmapLoad("DATA/PIXELMAP/LOADSCRN.PIX");
    TEST_ASSERT_NOT_NULL(pm);
    TEST_ASSERT_EQUAL_STRING("loadscrn.pix", pm->identifier);
    TEST_ASSERT_EQUAL_INT(320, pm->width);
    TEST_ASSERT_EQUAL_INT(320, pm->row_bytes);
    TEST_ASSERT_EQUAL_INT(200, pm->height);
}

void test_pmfile_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_pmfile_BrPixelmapLoad);
}
