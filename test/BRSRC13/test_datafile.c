#include "framework/unity.h"

#include "CORE/FW/datafile.h"

void test_datafile_ReadBinary() {
    br_datafile* f;
    f = DfOpen("DATA/MODELS/DEFAULT.DAT", 0, BRT_FLOAT);
    TEST_ASSERT_NOT_NULL(f);
    TEST_ASSERT_NOT_NULL(f->h);
    DfClose(f);
}

void test_datafile_suite() {
    RUN_TEST(test_datafile_ReadBinary);
}