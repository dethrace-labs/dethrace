#include "tests.h"

#include "CORE/MATH/matrix34.h"
#include <stdlib.h>
#include <string.h>

static void test_matrix34_BrMatrix34Copy() {
    br_matrix34 matin = {{
        {  1.f,  2.f,  3.f, },
        {  4.f,  5.f,  6.f, },
        {  7.f,  8.f,  9.f, },
        { 10.f, 11.f, 12.f, },
    }};
    br_matrix34 matout;
    BrMatrix34Copy(&matout, &matin);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(matin.m, matout.m, 12);
}

static void test_matrix34_BrMatrix34Mul() {
    br_matrix34 a;
    br_matrix34 b = {{
        {  1.f,  2.f,  3.f, },
        {  4.f,  5.f,  6.f, },
        {  7.f,  8.f,  9.f, },
        { 10.f, 11.f, 12.f, },
    }};
    br_matrix34 c = {{
        { 13.f, 14.f, 15.f, },
        { 16.f, 17.f, 18.f, },
        { 19.f, 20.f, 21.f, },
        { 22.f, 23.f, 24.f, },
    }};
    br_matrix34 expected = {{
        { 102.f, 108.f, 114.f, },
        { 246.f, 261.f, 276.f, },
        { 390.f, 414.f, 438.f, },
        { 556.f, 590.f, 624.f, },
    }};
    BrMatrix34Mul(&a, &b, &c);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, a.m, 12);
}

static void test_matrix34_BrMatrix34Identity() {
    br_matrix34 a;
    br_matrix34 expected = {{
        { 1.f, 0.f, 0.f, },
        { 0.f, 1.f, 0.f, },
        { 0.f, 0.f, 1.f, },
        { 0.f, 0.f, 0.f, },
    }};
    BrMatrix34Identity(&a);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, a.m, 12);
}

static void test_matrix34_BrMatrix34RotateX() {
    br_matrix34 a;
    br_matrix34 expected = {{
        { 1.f, .0f,         .0f, },
        { 0.f, .7071067f,   .7071067f, },
        { 0.f, -0.7071067f, .7071067f, },
        { 0.f, .0f,         .0f, },
    }};
    BrMatrix34RotateX(&a, BR_ANGLE_DEG(45));
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, a.m, 12);
}

static void test_matrix34_BrMatrix34RotateY() {
    br_matrix34 a;
    br_matrix34 expected = {{
        { .7071067f, 0.f, -.7071067f, },
        { .0f,       1.f,  .0f, },
        { .7071067f, 0.f,  .7071067f, },
        { .0f,       0.f,  .0f,       },
    }};
    BrMatrix34RotateY(&a, BR_ANGLE_DEG(45));
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, a.m, 12);
}

static void test_matrix34_BrMatrix34RotateZ() {
    br_matrix34 a;
    br_matrix34 expected = {{
        {  .7071067f, .7071067f, 0.f, },
        { -.7071067f, .7071067f, 0.f, },
        {  .0f,       .0f,       1.f, },
        {  .0f,       .0f,       0.f, },
    }};
    BrMatrix34RotateZ(&a, BR_ANGLE_DEG(45));
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, a.m, 12);
}

static void test_matrix34_BrMatrix34Rotate() {
    br_matrix34 mat;
    // scipy.spatial.transform.Rotation.from_rotvec(numpy.matrix([1,1,1]) * -numpy.pi/4/math.sqrt(3)).as_matrix()
    br_matrix34 expected = {{
        {  .80473785f,  .50587936f, -.31061722f, },
        { -.31061722f,  .80473785f,  .50587936f, },
        {  .50587936f, -.31061722f,  .80473785f, },
        {  .0f,         .0f,         .0f,        },
    }};
    br_vector3 a = {{ 0.577350269f, 0.577350269f, 0.577350269f, }};
    BrMatrix34Rotate(&mat, BR_ANGLE_DEG(45), &a);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, mat.m, 12);
}

static void test_matrix34_BrMatrix34Translate() {
    br_matrix34 mat;
    br_matrix34 expected = {{
        { 1.f, 0.f, 0.f, },
        { 0.f, 1.f, 0.f, },
        { 0.f, 0.f, 1.f, },
        { 3.f, 5.f, 7.f, },
    }};
    BrMatrix34Translate(&mat, 3.f, 5.f, 7.f);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, mat.m, 12);
}

static void test_matrix34_BrMatrix34Scale() {
    br_matrix34 mat;
    br_matrix34 expected = {{
        { 3.f, 0.f, 0.f, },
        { 0.f, 5.f, 0.f, },
        { 0.f, 0.f, 7.f, },
        { 0.f, 0.f, 0.f, },
    }};
    BrMatrix34Scale(&mat, 3.f, 5.f, 7.f);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, mat.m, 12);
}

static void test_matrix34_BrMatrix34ShearX() {
    br_matrix34 mat;
    br_matrix34 expected = {{
        { 1.f, 3.f, 5.f, },
        { 0.f, 1.f, 0.f, },
        { 0.f, 0.f, 1.f, },
        { 0.f, 0.f, 0.f, },
    }};
    BrMatrix34ShearX(&mat, 3.f, 5.f);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, mat.m, 12);
}

static void test_matrix34_BrMatrix34ShearY() {
    br_matrix34 mat;
    br_matrix34 expected = {{
        { 1.f, 0.f, 0.f, },
        { 3.f, 1.f, 5.f, },
        { 0.f, 0.f, 1.f, },
        { 0.f, 0.f, 0.f, },
    }};
    BrMatrix34ShearY(&mat, 3.f, 5.f);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, mat.m, 12);
}

static void test_matrix34_BrMatrix34ShearZ() {
    br_matrix34 mat;
    br_matrix34 expected = {{
        { 1.f, 0.f, 0.f, },
        { 0.f, 1.f, 0.f, },
        { 3.f, 5.f, 1.f, },
        { 0.f, 0.f, 0.f, },
    }};
    BrMatrix34ShearZ(&mat, 3.f, 5.f);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, mat.m, 12);
}

static void test_matrix34_BrMatrix34Inverse() {
    br_matrix34 matout;
    br_matrix34 matin = {{
        { 1.f, 4.f, 2.f, },
        { 3.f, 5.f, 1.f, },
        { 2.f, 5.f, 3.f, },
        { 4.f, 8.f, 2.f, },
    }};
    br_matrix34 expected = {{
        { -1.25f,   0.25f,   0.75f,  },
        {  0.875f,  0.125f, -0.625f, },
        { -0.625f, -0.375f,  0.875f, },
        { -0.75f, -1.25f,    0.25f,  },
    }};
    BrMatrix34Inverse(&matout, &matin);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-5, expected.m, matout.m, 12);
}

static void test_matrix34_BrMatrix34LPInverse() {
    br_matrix34 matout;
    br_matrix34 matin = {{
        {  0.f, 1.f, 0.f, },
        { -1.f, 0.f, 0.f, },
        {  0.f, 0.f, 1.f, },
        {  0.f, 0.f, 0.f, },
    }};
    br_matrix34 expected = {{
        { 0.f, -1.f, 0.f, },
        { 1.f,  0.f, 0.f, },
        { 0.f,  0.f, 1.f, },
        { 0.f,  0.f, 0.f, },
    }};
    BrMatrix34Inverse(&matout, &matin);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-5, expected.m, matout.m, 12);
}

static void test_matrix34_BrMatrix34LPNormalise() {
    br_matrix34 matout;
    br_matrix34 matin = {{
        { 1.f,    0.f, 0.f,    },
        { 1.e-3f, 1.f, 2.e-3f, },
        { 0.f,    0.f, 1.f,    },
        { 0.f,    0.f, 0.f,    },
    }};
    br_matrix34 expected = {{
        { 1.f, 0.f, 0.f, },
        { 0.f, 1.f, 0.f, },
        { 0.f, 0.f, 1.f, },
        { 0.f, 0.f, 0.f, },
    }};
    BrMatrix34LPNormalise(&matout, &matin);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-3, expected.m, matout.m, 12);
}

static void test_matrix34_BrMatrix34RollingBall() {
    {
        br_matrix34 mat;

        br_matrix34 expected = {{
            {  .9987616f,  -.0024768f,   .0496904f,  },
            { -.0024768f,   .99504639f,  .0993808f,  },
            { -.0496904f,  -.0993808f,   .99380799f, },
            {  .0f,         .0f,         .0f,         },
        }};
        BrMatrix34RollingBall(&mat, 5, 10, 100);
        TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-3, expected.m, mat.m, 12);
    }
    {
        br_matrix34 mat;
        br_matrix34 expected;
        BrMatrix34Identity(&expected);
        BrMatrix34RollingBall(&mat, 0, 0, 100);
        TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, mat.m, 12);
    }
}

static void test_matrix34_BrBoundsToMatrix34() {
    br_matrix34 matout;
    br_bounds bounds = {
        { -2.f, -3.f, -4.f,  },
        {  6.f,  9.f,  12.f, },
    };
    br_matrix34 expected = {{
        { 4.f, 0.f, 0.f, },
        { 0.f, 6.f, 0.f, },
        { 0.f, 0.f, 8.f, },
        { 2.f, 3.f, 4.f, },
    }};
    BrBoundsToMatrix34(&matout, &bounds);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, matout.m, 12);
}

static void test_matrix34_BrMatrix34Copy4() {
    br_matrix34 matout;
    br_matrix4 matin = {{
        {  1.f,  2.f,  3.f, 0.f, },
        {  4.f,  5.f,  6.f, 0.f, },
        {  7.f,  8.f,  9.f, 0.f, },
        { 10.f, 11.f, 12.f, 1.f, },
    }};
    br_matrix34 expected = {{
        {  1.f,  2.f,  3.f, },
        {  4.f,  5.f,  6.f, },
        {  7.f,  8.f,  9.f, },
        { 10.f, 11.f, 12.f, },
    }};
    BrMatrix34Copy4(&matout, &matin);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, matout.m, 12);
}

static void test_matrix34_BrMatrix34TApplyFV() {
    br_vector3 vecout;
    br_fvector3 vecin = {
        { 1.f, 2.f, 3.f, },
    };
    br_matrix34 matin = {{
        {  1.f,  2.f,  3.f, },
        {  4.f,  5.f,  6.f, },
        {  7.f,  8.f,  9.f, },
        { 10.f, 11.f, 12.f, },
    }};
    br_vector3 expected = {
        { 14.f, 32.f, 50.f, },
    };
    BrMatrix34TApplyFV(&vecout, &vecin, &matin);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vecout.v, 3);
}

static void test_matrix34_BrMatrix34Apply() {
    br_vector3 vecout;
    br_vector4 vecin = {
        { 1.f, 2.f, 3.f, 1.f, },
    };
    br_matrix34 matin = {{
        {  1.f,  2.f,  3.f, },
        {  4.f,  5.f,  6.f, },
        {  7.f,  8.f,  9.f, },
        { 10.f, 11.f, 12.f, },
    }};
    br_vector3 expected = {
        { 40.f, 47.f, 54.f, },
    };
    BrMatrix34Apply(&vecout, &vecin, &matin);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vecout.v, 3);
}

static void test_matrix34_BrMatrix34ApplyP() {
    br_vector3 vecout;
    br_vector3 vecin = {
        { 1.f, 2.f, 3.f, },
    };
    br_matrix34 matin = {{
        {  1.f,  2.f,  3.f, },
        {  4.f,  5.f,  6.f, },
        {  7.f,  8.f,  9.f, },
        { 10.f, 11.f, 12.f, },
    }};
    br_vector3 expected = {
        { 40.f, 47.f, 54.f, },
    };
    BrMatrix34ApplyP(&vecout, &vecin, &matin);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vecout.v, 3);
}

static void test_matrix34_BrMatrix34ApplyV() {
    br_vector3 vecout;
    br_vector3 vecin = {
        { 1.f, 2.f, 3.f, },
    };
    br_matrix34 matin = {{
        {  1.f,  2.f,  3.f, },
        {  4.f,  5.f,  6.f, },
        {  7.f,  8.f,  9.f, },
        { 10.f, 11.f, 12.f, },
    }};
    br_vector3 expected = {
        { 30.f, 36.f, 42.f, },
    };
    BrMatrix34ApplyV(&vecout, &vecin, &matin);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vecout.v, 3);
}

static void test_matrix34_BrMatrix34TApply() {
    br_vector4 vecout;
    br_vector4 vecin = {
        { 1.f, 2.f, 3.f, 4.f, },
    };
    br_matrix34 matin = {{
        {  1.f,  2.f,  3.f, },
        {  4.f,  5.f,  6.f, },
        {  7.f,  8.f,  9.f, },
        { 10.f, 11.f, 12.f, },
    }};
    br_vector4 expected = {
        { 14.f, 32.f, 50.f, 72.f, },
    };
    BrMatrix34TApply(&vecout, &vecin, &matin);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vecout.v, 4);
}

static void test_matrix34_BrMatrix34TApplyP() {
    br_vector3 vecout;
    br_vector3 vecin = {
        { 1.f, 2.f, 3.f, },
    };
    br_matrix34 matin = {{
        {  1.f,  2.f,  3.f, },
        {  4.f,  5.f,  6.f, },
        {  7.f,  8.f,  9.f, },
        { 10.f, 11.f, 12.f, },
    }};
    br_vector3 expected = {
        { 14.f, 32.f, 50.f, },
    };
    BrMatrix34TApplyP(&vecout, &vecin, &matin);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vecout.v, 3);
}

static void test_matrix34_BrMatrix34TApplyV() {
    br_vector3 vecout;
    br_vector3 vecin = {
        { 1.f, 2.f, 3.f, },
    };
    br_matrix34 matin = {{
        {  1.f,  2.f,  3.f, },
        {  4.f,  5.f,  6.f, },
        {  7.f,  8.f,  9.f, },
        { 10.f, 11.f, 12.f, },
    }};
    br_vector3 expected = {
        { 14.f, 32.f, 50.f, },
    };
    BrMatrix34TApplyV(&vecout, &vecin, &matin);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vecout.v, 3);
}

static void test_matrix34_BrMatrix34Pre() {
    br_matrix34 mat = {{
        {  1.f,  2.f,  3.f, },
        {  4.f,  5.f,  6.f, },
        {  7.f,  8.f,  9.f, },
        { 10.f, 11.f, 12.f, },
    }};
    br_matrix34 a = {{
        { 13.f, 14.f, 15.f, },
        { 16.f, 17.f, 18.f, },
        { 19.f, 20.f, 21.f, },
        { 22.f, 23.f, 24.f, },
    }};
    br_matrix34 expected = {{
        { 174.f, 216.f, 258.f, },
        { 210.f, 261.f, 312.f, },
        { 246.f, 306.f, 366.f, },
        { 292.f, 362.f, 432.f, },
    }};
    BrMatrix34Pre(&mat, &a);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, mat.m, 12);
}

static void test_matrix34_BrMatrix34Post() {
    br_matrix34 mat = {{
        {  1.f,  2.f,  3.f, },
        {  4.f,  5.f,  6.f, },
        {  7.f,  8.f,  9.f, },
        { 10.f, 11.f, 12.f, },
    }};
    br_matrix34 a = {{
        { 13.f, 14.f, 15.f, },
        { 16.f, 17.f, 18.f, },
        { 19.f, 20.f, 21.f, },
        { 22.f, 23.f, 24.f, },
    }};
    br_matrix34 expected = {{
        { 102.f, 108.f, 114.f, },
        { 246.f, 261.f, 276.f, },
        { 390.f, 414.f, 438.f, },
        { 556.f, 590.f, 624.f, },
    }};
    BrMatrix34Post(&mat, &a);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, mat.m, 12);
}

static void test_matrix34_BrMatrix34PreRotateX() {
    br_matrix34 mat = {{
        {  1.f,  0.f,  0.f, },
        {  0.f,  1.f,  0.f, },
        {  0.f,  0.f,  1.f, },
        {  2.f,  3.f,  4.f, },
    }};
    br_matrix34 expected = {{
        {  1.f,  0.f, 0.f, },
        {  0.f,  0.f, 1.f, },
        {  0.f, -1.f, 0.f, },
        {  2.f,  3.f, 4.f, },
    }};
    BrMatrix34PreRotateX(&mat, BR_ANGLE_DEG(90));
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-3, expected.m, mat.m, 12);
}

static void test_matrix34_BrMatrix34PostRotateX() {
    br_matrix34 mat = {{
        {  1.f,  0.f,  0.f, },
        {  0.f,  1.f,  0.f, },
        {  0.f,  0.f,  1.f, },
        {  2.f,  3.f,  4.f, },
    }};
    br_matrix34 expected = {{
        {  1.f,  0.f, 0.f, },
        {  0.f,  0.f, 1.f, },
        {  0.f, -1.f, 0.f, },
        {  2.f, -4.f, 3.f, },
    }};
    BrMatrix34PostRotateX(&mat, BR_ANGLE_DEG(90));
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, mat.m, 12);
}

static void test_matrix34_BrMatrix34PreRotateY() {
    br_matrix34 mat = {{
        { 1.f, 0.f, 0.f, },
        { 0.f, 1.f, 0.f, },
        { 0.f, 0.f, 1.f, },
        { 3.f, 2.f, 4.f, },
    }};
    br_matrix34 expected = {{
        { 0.f, 0.f, -1.f, },
        { 0.f, 1.f,  0.f, },
        { 1.f, 0.f,  0.f, },
        { 3.f, 2.f,  4.f, },
    }};
    BrMatrix34PreRotateY(&mat, BR_ANGLE_DEG(90));
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-3, expected.m, mat.m, 12);
}

static void test_matrix34_BrMatrix34PostRotateY() {
    br_matrix34 mat = {{
        { 1.f, 0.f, 0.f, },
        { 0.f, 1.f, 0.f, },
        { 0.f, 0.f, 1.f, },
        { 3.f, 2.f, 4.f, },
    }};
    br_matrix34 expected = {{
        { 0.f, 0.f, -1.f, },
        { 0.f, 1.f,  0.f, },
        { 1.f, 0.f,  0.f, },
        { 4.f, 2.f, -3.f, },
    }};
    BrMatrix34PostRotateY(&mat, BR_ANGLE_DEG(90));
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, mat.m, 12);
}

static void test_matrix34_BrMatrix34PreRotateZ() {
    br_matrix34 mat = {{
        { 1.f, 0.f, 0.f, },
        { 0.f, 1.f, 0.f, },
        { 0.f, 0.f, 1.f, },
        { 3.f, 4.f, 2.f, },
    }};
    br_matrix34 expected = {{
        {  0.f, 1.f, 0.f, },
        { -1.f, 0.f, 0.f, },
        {  0.f, 0.f, 1.f, },
        {  3.f, 4.f, 2.f, },
    }};
    BrMatrix34PreRotateZ(&mat, BR_ANGLE_DEG(90));
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-3, expected.m, mat.m, 12);
}

static void test_matrix34_BrMatrix34PostRotateZ() {
    br_matrix34 mat = {{
        { 1.f, 0.f, 0.f, },
        { 0.f, 1.f, 0.f, },
        { 0.f, 0.f, 1.f, },
        { 3.f, 4.f, 2.f, },
    }};
    br_matrix34 expected = {{
        {  0.f, 1.f, 0.f, },
        { -1.f, 0.f, 0.f, },
        {  0.f, 0.f, 1.f, },
        { -4.f, 3.f, 2.f, },
    }};
    BrMatrix34PostRotateZ(&mat, BR_ANGLE_DEG(90));
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, mat.m, 12);
}

void test_matrix34_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_matrix34_BrMatrix34Copy);
    RUN_TEST(test_matrix34_BrMatrix34Mul);
    RUN_TEST(test_matrix34_BrMatrix34Identity);
    RUN_TEST(test_matrix34_BrMatrix34RotateX);
    RUN_TEST(test_matrix34_BrMatrix34RotateY);
    RUN_TEST(test_matrix34_BrMatrix34RotateZ);
    RUN_TEST(test_matrix34_BrMatrix34Rotate);
    RUN_TEST(test_matrix34_BrMatrix34Translate);
    RUN_TEST(test_matrix34_BrMatrix34Scale);
    RUN_TEST(test_matrix34_BrMatrix34ShearX);
    RUN_TEST(test_matrix34_BrMatrix34ShearY);
    RUN_TEST(test_matrix34_BrMatrix34ShearZ);
    RUN_TEST(test_matrix34_BrMatrix34Inverse);
    RUN_TEST(test_matrix34_BrMatrix34LPInverse);
    RUN_TEST(test_matrix34_BrMatrix34LPNormalise);
    RUN_TEST(test_matrix34_BrMatrix34RollingBall);
    RUN_TEST(test_matrix34_BrBoundsToMatrix34);
    RUN_TEST(test_matrix34_BrMatrix34Copy4);
    RUN_TEST(test_matrix34_BrMatrix34TApplyFV);
    RUN_TEST(test_matrix34_BrMatrix34Apply);
    RUN_TEST(test_matrix34_BrMatrix34ApplyP);
    RUN_TEST(test_matrix34_BrMatrix34ApplyV);
    RUN_TEST(test_matrix34_BrMatrix34TApply);
    RUN_TEST(test_matrix34_BrMatrix34TApplyP);
    RUN_TEST(test_matrix34_BrMatrix34TApplyV);
    RUN_TEST(test_matrix34_BrMatrix34Pre);
    RUN_TEST(test_matrix34_BrMatrix34Post);
    RUN_TEST(test_matrix34_BrMatrix34PreRotateX);
    RUN_TEST(test_matrix34_BrMatrix34PostRotateX);
    RUN_TEST(test_matrix34_BrMatrix34PreRotateY);
    RUN_TEST(test_matrix34_BrMatrix34PostRotateY);
    RUN_TEST(test_matrix34_BrMatrix34PreRotateZ);
    RUN_TEST(test_matrix34_BrMatrix34PostRotateZ);
}
