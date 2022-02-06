#include "CORE/V1DB/matsupt.h"
#include "CORE/V1DB/regsupt.h"
#include "tests.h"

struct callback_data {
    int nb;
};

br_uint_32 callback(br_material* mat, void* arg) {
    ((struct callback_data*)arg)->nb++;
    return 0;
}

void test_regsupt_BrMaterialEnum() {
    int nb_initial;
    int result;
    struct callback_data callback_data;
    br_material *m, *m2;

    callback_data.nb = 0;
    BrMaterialEnum(NULL, callback, &callback_data);
    nb_initial = callback_data.nb;

    m = BrMaterialAllocate("regsupt_BrMaterialEnum_mat1");
    m2 = BrMaterialAllocate("regsupt_BrMaterialEnum_mat2");
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_NOT_NULL(m2);
    BrMaterialAdd(m);
    BrMaterialAdd(m2);

    callback_data.nb = 0;
    result = BrMaterialEnum("regsupt_BrMaterialEnum_*", callback, &callback_data);
    TEST_ASSERT_EQUAL_INT(2, callback_data.nb);

    callback_data.nb = 0;
    result = BrMaterialEnum(NULL, callback, &callback_data);
    TEST_ASSERT_EQUAL_INT(2, callback_data.nb - nb_initial);

    callback_data.nb = 0;
    result = BrMaterialEnum("regsupt_BrMaterialEnum_mat1", callback, &callback_data);
    TEST_ASSERT_EQUAL_INT(1, callback_data.nb);
}

void test_regsupt_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_regsupt_BrMaterialEnum);
}
