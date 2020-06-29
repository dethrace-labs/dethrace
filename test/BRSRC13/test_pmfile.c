#include "framework/unity.h"

#include "CORE/PIXELMAP/pmfile.h"

void test_pmfile_BrPixelmapLoad() {
    br_pixelmap* pm;
    pm = BrPixelmapLoad("DATA/PIXELMAPS/LAVA.PIX");
    TEST_ASSERT_NOT_NULL(pm);
    TEST_ASSERT_EQUAL_STRING("LAVA.PIX", pm->identifier);
    TEST_ASSERT_EQUAL_INT(256, pm->width);
    TEST_ASSERT_EQUAL_INT(256, pm->height);
}

void test_pm_BrPixelmapLoad2() {
    br_pixelmap* pm;
    pm = BrPixelmapLoad("DATA/PIXELMAPS/LAVA.PIX");
    TEST_ASSERT_NOT_NULL(pm);
    TEST_ASSERT_EQUAL_STRING("LAVA.PIX", pm->identifier);
    TEST_ASSERT_EQUAL_INT(256, pm->width);
    TEST_ASSERT_EQUAL_INT(256, pm->height);
}

void test_pmfile_suite() {
    RUN_TEST(test_pmfile_BrPixelmapLoad);
    RUN_TEST(test_pm_BrPixelmapLoad2);
}