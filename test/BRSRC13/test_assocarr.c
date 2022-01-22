#include "CORE/FW/assocarr.h"
#include "CORE/FW/resource.h"
#include "tests.h"

static void test_assocarr_BrAssociativeArrayAllocate() {
    br_associative_array *arr;
    br_error err;
    br_value val;

    arr = BrAssociativeArrayAllocate();

    TEST_ASSERT_NOT_NULL(arr);
    TEST_ASSERT_NOT_NULL(arr->tv);
    TEST_ASSERT_EQUAL(0, arr->num_elements);
    TEST_ASSERT_GREATER_THAN(0, arr->max_elements);

    err = BrAssociativeArrayQuery(arr, BRT_INTEL, &val);

    TEST_ASSERT_EQUAL_INT(0x1002, err);

    err = BrAssociativeArrayRemoveEntry(arr, BRT_INTEL);

    TEST_ASSERT_EQUAL_INT(0x1002, err);
}

static void test_assocarr_BrAssociativeArraySetEntry_u32() {
    br_associative_array *arr;
    br_error err;
    br_value valin;
    br_value valout;

    arr = BrAssociativeArrayAllocate();

    TEST_ASSERT_EQUAL(0, arr->num_elements);

    valin.u32 = 487;
    BrAssociativeArraySetEntry(arr, BRT_INTEL, valin);

    TEST_ASSERT_EQUAL(1, arr->num_elements);

    valout.u32 = 0;
    err = BrAssociativeArrayQuery(arr, BRT_INTEL, &valout);

    TEST_ASSERT_EQUAL_INT(0, err);
    TEST_ASSERT_EQUAL_INT(487, valout.u32);

    BrResFree(arr);
}
static void test_assocarr_BrAssociativeArraySetEntry_str() {
    br_associative_array *arr;
    br_error err;
    br_value valin;
    br_value valout;
    char *str;

    arr = BrAssociativeArrayAllocate();

    TEST_ASSERT_EQUAL(0, arr->num_elements);

    str = BrResStrDup(NULL, "dummy string");
    valin.str = str;
    BrAssociativeArraySetEntry(arr, BRT_DEFAULT_DEVICE_STR, valin);
    BrResFree(str);
    str = NULL;

    TEST_ASSERT_EQUAL(1, arr->num_elements);

    valout.str = NULL;
    err = BrAssociativeArrayQuery(arr, BRT_DEFAULT_DEVICE_STR, &valout);

    TEST_ASSERT_EQUAL_INT(0, err);
    TEST_ASSERT_EQUAL(BR_MEMORY_STRING, BrResClass(valout.str));
    TEST_ASSERT_EQUAL_STRING("dummy string", valout.str);

    valout.u32 = 0;
    err = BrAssociativeArrayQuery(arr, BRT_INTEL, &valout);

    TEST_ASSERT_EQUAL_INT(0x1002, err);

    err = BrAssociativeArrayRemoveEntry(arr, BRT_DEFAULT_DEVICE_STR);
    
    TEST_ASSERT_EQUAL_INT(0, err);

    err = BrAssociativeArrayRemoveEntry(arr, BRT_DEFAULT_DEVICE_STR);
    
    TEST_ASSERT_EQUAL_INT(0x1002, err);

    BrResFree(arr);
}

static void test_assocarr_overflow() {
    br_associative_array *arr;
    br_error err;
    br_value valin;
    br_value valout;
    br_uint_32 initialCapacity;

    arr = BrAssociativeArrayAllocate();

    initialCapacity = arr->max_elements;

    valin.b = 1;
    BrAssociativeArraySetEntry(arr, BRT_INTEL, valin);
    valin.b = 1;
    BrAssociativeArraySetEntry(arr, BRT_LINEAR, valin);
    valin.u32 = 3;
    BrAssociativeArraySetEntry(arr, BRT_VERSION_U32, valin);
    valin.f = 3.14f;
    BrAssociativeArraySetEntry(arr, BRT_VERTEX_DBL, valin);
    valin.i32 = 5;
    BrAssociativeArraySetEntry(arr, BRT_WIDTH_I32, valin);

    TEST_ASSERT_EQUAL(5, arr->num_elements);

    valin.i32 = 6;
    BrAssociativeArraySetEntry(arr, BRT_VIDEO_MEMORY_U32, valin);
    valin.i32 = 7;
    BrAssociativeArraySetEntry(arr, BRT_WRITABLE_BASE_I32, valin);
    valin.i32 = 8;
    BrAssociativeArraySetEntry(arr, BRT_WRITABLE_RANGE_I32, valin);
    valin.i32 = 9;
    BrAssociativeArraySetEntry(arr, BRT_SYCHRONISE_I32, valin);
    valin.i32 = 10;
    BrAssociativeArraySetEntry(arr, BRT_CACHED_SIZE_U32, valin);

    TEST_ASSERT_EQUAL(10, arr->num_elements);

    valin.i32 = 11;
    BrAssociativeArraySetEntry(arr, BRT_RENDERER_MAX_I32, valin);
    valin.i32 = 12;
    BrAssociativeArraySetEntry(arr, BRT_WORD_BYTES_I32, valin);

    TEST_ASSERT_EQUAL(12, arr->num_elements);
    if (arr->num_elements > initialCapacity) {
        TEST_ASSERT_GREATER_THAN(initialCapacity, arr->max_elements);
    }

    err = BrAssociativeArrayRemoveEntry(arr, BRT_SYCHRONISE_I32);

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(11, arr->num_elements);

    valout.i32 = 0;
    err = BrAssociativeArrayQuery(arr, BRT_WRITABLE_RANGE_I32, &valout);

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL_INT32(8, valout.i32);

    valout.i32 = 0;
    err = BrAssociativeArrayQuery(arr, BRT_SYCHRONISE_I32, &valout);

    TEST_ASSERT_EQUAL(0x1002, err);

    BrResFree(arr);
}

void test_assocarr_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_assocarr_BrAssociativeArrayAllocate);
    RUN_TEST(test_assocarr_BrAssociativeArraySetEntry_u32);
    RUN_TEST(test_assocarr_BrAssociativeArraySetEntry_str);
    RUN_TEST(test_assocarr_overflow);
}
