#include "CORE/V1DB/matsupt.h"
#include "CORE/V1DB/regsupt.h"
#include "tests.h"

int nbr_callbacks;

br_uint_32 callback(br_material* mat, void* arg) {
    nbr_callbacks++;
    return 0;
}

void test_regsupt_BrMaterialEnum() {
    int result;
    br_material *m, *m2;

    m = BrMaterialAllocate("mat1");
    m2 = BrMaterialAllocate("mat2");
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_NOT_NULL(m2);
    BrMaterialAdd(m);
    BrMaterialAdd(m2);

    nbr_callbacks = 0;
    result = BrMaterialEnum("*", callback, NULL);
    TEST_ASSERT_EQUAL_INT(2, nbr_callbacks);

    nbr_callbacks = 0;
    result = BrMaterialEnum(NULL, callback, NULL);
    TEST_ASSERT_EQUAL_INT(2, nbr_callbacks);

    nbr_callbacks = 0;
    result = BrMaterialEnum("mat1", callback, NULL);
    TEST_ASSERT_EQUAL_INT(1, nbr_callbacks);
}

void test_regsupt_suite() {
    RUN_TEST(test_regsupt_BrMaterialEnum);
}