#include "framework/unity.h"

#include "CORE/PIXELMAP/pmfile.h"

void test_pmfile_BrPixelmapLoad() {
    br_pixelmap* pm;
    pm = BrPixelmapLoad("DATA/PIXELMAP/LOADSCRN.PIX");
    TEST_ASSERT_NOT_NULL(pm);
    TEST_ASSERT_EQUAL_STRING("loadscrn.pix", pm->identifier);
    TEST_ASSERT_EQUAL_INT(320, pm->width);
    TEST_ASSERT_EQUAL_INT(320, pm->row_bytes);
    TEST_ASSERT_EQUAL_INT(200, pm->height);
}

void test_pmfile_suite() {
    RUN_TEST(test_pmfile_BrPixelmapLoad);
}