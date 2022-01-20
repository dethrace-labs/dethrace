#include "CORE/MATH/quat.h"
#include "CORE/MATH/matrix34.h"
#include "tests.h"
#include <math.h>
#include <string.h>

static void test_quat_BrQuatMul() {
    br_quat q;
    br_quat l;
    br_quat r;
    br_quat *res;

    l.x = sqrtf(0.5f);
    l.y = 0.f;
    l.z = 0.f;
    l.w = sqrt(0.5f);

    r.x = 0.f;
    r.y = sqrt(0.5f);
    r.z = 0.f;
    r.w = sqrt(0.5f);

    res = BrQuatMul(&q, &l, &r);
    TEST_ASSERT_EQUAL_PTR(&q, res);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, q.x);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, q.y);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, q.z);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, q.w);

    l.x = 0.f;
    l.y = 0.f;
    l.z = 0.f;
    l.w = 1.f;

    res = BrQuatMul(&q, &l, &r);
    TEST_ASSERT_EQUAL_PTR(&q, res);
    TEST_ASSERT_EQUAL_FLOAT(r.x, q.x);
    TEST_ASSERT_EQUAL_FLOAT(r.y, q.y);
    TEST_ASSERT_EQUAL_FLOAT(r.z, q.z);
    TEST_ASSERT_EQUAL_FLOAT(r.w, q.w);

    res = BrQuatMul(&q, &r, &l);
    TEST_ASSERT_EQUAL_PTR(&q, res);
    TEST_ASSERT_EQUAL_FLOAT(r.x, q.x);
    TEST_ASSERT_EQUAL_FLOAT(r.y, q.y);
    TEST_ASSERT_EQUAL_FLOAT(r.z, q.z);
    TEST_ASSERT_EQUAL_FLOAT(r.w, q.w);
}

static void test_quat_BrQuatNormalise() {
    br_quat q;
    br_quat qq;
    br_quat *res;

    qq.x = 1.f;
    qq.y = 1.f;
    qq.z = 1.f;
    qq.w = 1.f;

    res = BrQuatNormalise(&q, &qq);
    TEST_ASSERT_EQUAL_PTR(&q, res);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, q.x);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, q.y);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, q.z);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, q.w);

    qq.x = 4.f;
    qq.y = 3.f;
    qq.z = 0.f;
    qq.w = 0.f;

    res = BrQuatNormalise(&q, &qq);
    TEST_ASSERT_EQUAL_PTR(&q, res);
    TEST_ASSERT_EQUAL_FLOAT(0.8f, q.x);
    TEST_ASSERT_EQUAL_FLOAT(0.6f, q.y);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, q.z);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, q.w);
}

static void test_quat_BrQuatInvert() {
    br_quat q;
    br_quat qq;
    br_quat *res;

    qq.x = 1.f;
    qq.y = 1.f;
    qq.z = 1.f;
    qq.w = 1.f;

    res = BrQuatInvert(&q, &qq);
    TEST_ASSERT_EQUAL_PTR(&q, res);
    TEST_ASSERT_EQUAL_FLOAT(-1.f, q.x);
    TEST_ASSERT_EQUAL_FLOAT(-1.f, q.y);
    TEST_ASSERT_EQUAL_FLOAT(-1.f, q.z);
    TEST_ASSERT_EQUAL_FLOAT(1.f, q.w);

    qq.x = 4.f;
    qq.y = 3.f;
    qq.z = 0.f;
    qq.w = 0.f;

    res = BrQuatInvert(&q, &qq);
    TEST_ASSERT_EQUAL_PTR(&q, res);
    TEST_ASSERT_EQUAL_FLOAT(-4.f, q.x);
    TEST_ASSERT_EQUAL_FLOAT(-3.f, q.y);
    TEST_ASSERT_EQUAL_FLOAT(-0.0f, q.z);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, q.w);

    qq.x = 0.f;
    qq.y = 3.f;
    qq.z = 0.f;
    qq.w = 4.f;

    res = BrQuatInvert(&q, &qq);
    TEST_ASSERT_EQUAL_PTR(&q, res);
    TEST_ASSERT_EQUAL_FLOAT(-0.f, q.x);
    TEST_ASSERT_EQUAL_FLOAT(-3.f, q.y);
    TEST_ASSERT_EQUAL_FLOAT(-0.0f, q.z);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, q.w);
}

static void test_quat_BrQuatSlerp() {
    br_quat q;
    br_quat l;
    br_quat r;
    br_scalar a;
    br_int_16 spins;
    br_quat *res;

    l.x = sqrtf(0.5f);
    l.y = 0.f;
    l.z = 0.f;
    l.w = sqrtf(0.5f);

    r.x = 0.f;
    r.y = sqrtf(0.5f);
    r.z = 0.f;
    r.w = sqrtf(0.5f);

    a = 0.f;
    spins = 0;

    res = BrQuatSlerp(&q, &l, &r, a, spins);
    TEST_ASSERT_EQUAL_PTR(&q, res);
    TEST_ASSERT_EQUAL_FLOAT(sqrt(0.5f), q.x);
    TEST_ASSERT_EQUAL_FLOAT(0.f, q.y);
    TEST_ASSERT_EQUAL_FLOAT(0.f, q.z);
    TEST_ASSERT_EQUAL_FLOAT(sqrt(0.5f), q.w);

    a = 1.f;
    spins = 0;

    res = BrQuatSlerp(&q, &l, &r, a, spins);
    TEST_ASSERT_EQUAL_PTR(&q, res);
    TEST_ASSERT_EQUAL_FLOAT(0.f, q.x);
    TEST_ASSERT_EQUAL_FLOAT(sqrt(0.5f), q.y);
    TEST_ASSERT_EQUAL_FLOAT(0.f, q.z);
    TEST_ASSERT_EQUAL_FLOAT(sqrt(0.5f), q.w);

    a = 0.5f;
    spins = 0;

    res = BrQuatSlerp(&q, &l, &r, a, spins);
    TEST_ASSERT_EQUAL_PTR(&q, res);
    TEST_ASSERT_FLOAT_WITHIN(1e-4, 0.40824829f, q.x);
    TEST_ASSERT_FLOAT_WITHIN(1e-4, 0.40824829f, q.y);
    TEST_ASSERT_FLOAT_WITHIN(1e-4, 0.f, q.z);
    TEST_ASSERT_FLOAT_WITHIN(1e-4, 0.81649658f, q.w);

    a = 0.5f;
    spins = -1;

    res = BrQuatSlerp(&q, &l, &r, a, spins);
    TEST_ASSERT_EQUAL_PTR(&q, res);
    TEST_ASSERT_FLOAT_WITHIN(1e-4, 0.707185f, q.x);
    TEST_ASSERT_FLOAT_WITHIN(1e-4, -0.707185f, q.y);
    TEST_ASSERT_FLOAT_WITHIN(1e-4, 0.f, q.z);
    TEST_ASSERT_FLOAT_WITHIN(1e-4, 0.f, q.w);
}

static void test_quat_BrQuatToMatrix34() {
    br_matrix34 mat;
    br_quat q;
    br_matrix34 *res;
    br_matrix34 expected;

    q.x = 0.f;
    q.y = 0.f;
    q.z = 0.f;
    q.w = 1.f;
    memset(&expected, 0, sizeof(expected));
    expected.m[0][0] = 1.f;
    expected.m[1][1] = 1.f;
    expected.m[2][2] = 1.f;
    res = BrQuatToMatrix34(&mat, &q);
    TEST_ASSERT_EQUAL_PTR(&mat, res);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, mat.m, 12);

    q.x = sqrtf(0.5f);
    q.y = 0.f;
    q.z = 0.f;
    q.w = sqrtf(0.5f);
    expected.m[0][0] = 1.f;
    expected.m[0][1] = 0.f;
    expected.m[0][2] = 0.f;
    expected.m[1][0] = 0.f;
    expected.m[1][1] = 0.f;
    expected.m[1][2] = 1.f;
    expected.m[2][0] = 0.f;
    expected.m[2][1] = -1.f;
    expected.m[2][2] = 0.f;
    expected.m[3][0] = 0.f;
    expected.m[3][1] = 0.f;
    expected.m[3][2] = 0.f;
    res = BrQuatToMatrix34(&mat, &q);
    TEST_ASSERT_EQUAL_PTR(&mat, res);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-5, expected.m, mat.m, 12);
}

static void test_quat_BrMatrix34ToQuat() {
    br_quat q;
    br_matrix34 mat;
    br_quat *res;
    br_quat expected;

    memset(&mat, 0, sizeof(mat));
    mat.m[0][0] = 1.f;
    mat.m[1][1] = 1.f;
    mat.m[2][2] = 1.f;
    expected.x = 0.f;
    expected.y = 0.f;
    expected.z = 0.f;
    expected.w = 1.f;
    res = BrMatrix34ToQuat(&q, &mat);
    TEST_ASSERT_EQUAL_PTR(&q, res);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, expected.x, q.x);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, expected.y, q.y);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, expected.z, q.z);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, expected.w, q.w);

    mat.m[0][0] = 1.f;
    mat.m[0][1] = 0.f;
    mat.m[0][2] = 0.f;
    mat.m[1][0] = 0.f;
    mat.m[1][1] = 0.f;
    mat.m[1][2] = 1.f;
    mat.m[2][0] = 0.f;
    mat.m[2][1] = -1.f;
    mat.m[2][2] = 0.f;
    mat.m[3][0] = 0.f;
    mat.m[3][1] = 0.f;
    mat.m[3][2] = 0.f;
    expected.x = sqrtf(0.5f);
    expected.y = 0.f;
    expected.z = 0.f;
    expected.w = sqrtf(0.5f);
    res = BrMatrix34ToQuat(&q, &mat);
    TEST_ASSERT_EQUAL_PTR(&q, res);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, expected.x, q.x);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, expected.y, q.y);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, expected.z, q.z);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, expected.w, q.w);
}

static void test_quat_BrQuatToMatrix4() {
    br_matrix4 mat;
    br_quat q;
    br_matrix4 *res;
    br_matrix4 expected;

    q.x = 0.f;
    q.y = 0.f;
    q.z = 0.f;
    q.w = 1.f;
    memset(&expected, 0, sizeof(expected));
    expected.m[0][0] = 1.f;
    expected.m[1][1] = 1.f;
    expected.m[2][2] = 1.f;
    expected.m[3][3] = 1.f;
    res = BrQuatToMatrix4(&mat, &q);
    TEST_ASSERT_EQUAL_PTR(&mat, res);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, mat.m, 16);

    q.x = sqrtf(0.5f);
    q.y = 0.f;
    q.z = 0.f;
    q.w = sqrtf(0.5f);
    expected.m[0][0] = 1.f;
    expected.m[0][1] = 0.f;
    expected.m[0][2] = 0.f;
    expected.m[0][3] = 0.f;
    expected.m[1][0] = 0.f;
    expected.m[1][1] = 0.f;
    expected.m[1][2] = 1.f;
    expected.m[1][3] = 0.f;
    expected.m[2][0] = 0.f;
    expected.m[2][1] = -1.f;
    expected.m[2][2] = 0.f;
    expected.m[2][3] = 0.f;
    expected.m[3][0] = 0.f;
    expected.m[3][1] = 0.f;
    expected.m[3][2] = 0.f;
    expected.m[3][3] = 1.f;
    res = BrQuatToMatrix4(&mat, &q);
    TEST_ASSERT_EQUAL_PTR(&mat, res);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-5, expected.m, mat.m, 16);
}

static void test_quat_BrMatrix4ToQuat() {
    br_quat q;
    br_matrix4 mat;
    br_quat *res;
    br_quat expected;

    memset(&mat, 0, sizeof(mat));
    mat.m[0][0] = 1.f;
    mat.m[1][1] = 1.f;
    mat.m[2][2] = 1.f;
    mat.m[3][3] = 1.f;
    expected.x = 0.f;
    expected.y = 0.f;
    expected.z = 0.f;
    expected.w = 1.f;
    res = BrMatrix4ToQuat(&q, &mat);
    TEST_ASSERT_EQUAL_PTR(&q, res);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, expected.x, q.x);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, expected.y, q.y);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, expected.z, q.z);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, expected.w, q.w);

    mat.m[0][0] = 1.f;
    mat.m[0][1] = 0.f;
    mat.m[0][2] = 0.f;
    mat.m[0][3] = 0.f;
    mat.m[1][0] = 0.f;
    mat.m[1][1] = 0.f;
    mat.m[1][2] = 1.f;
    mat.m[1][3] = 0.f;
    mat.m[2][0] = 0.f;
    mat.m[2][1] = -1.f;
    mat.m[2][2] = 0.f;
    mat.m[2][3] = 0.f;
    mat.m[3][0] = 0.f;
    mat.m[3][1] = 0.f;
    mat.m[3][2] = 0.f;
    mat.m[3][3] = 1.f;
    expected.x = sqrtf(0.5f);
    expected.y = 0.f;
    expected.z = 0.f;
    expected.w = sqrtf(0.5f);
    res = BrMatrix4ToQuat(&q, &mat);
    TEST_ASSERT_EQUAL_PTR(&q, res);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, expected.x, q.x);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, expected.y, q.y);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, expected.z, q.z);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, expected.w, q.w);
}

void test_quat_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_quat_BrQuatMul);
    RUN_TEST(test_quat_BrQuatNormalise);
    RUN_TEST(test_quat_BrQuatInvert);
    RUN_TEST(test_quat_BrQuatSlerp);
    RUN_TEST(test_quat_BrQuatToMatrix34);
    RUN_TEST(test_quat_BrMatrix34ToQuat);
    RUN_TEST(test_quat_BrQuatToMatrix4);
    RUN_TEST(test_quat_BrMatrix4ToQuat);
}
