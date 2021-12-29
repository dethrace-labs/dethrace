#include "tests.h"

#include "CORE/MATH/matrix23.h"
#include <stdlib.h>
#include <string.h>

void test_matrix23_BrMatrix23Copy() {
    br_matrix23 matin = {{
        {  1.f,  2.f, },
        {  3.f,  4.f, },
        {  5.f,  6.f, },
    }};
    br_matrix23 matout;
    BrMatrix23Copy(&matout, &matin);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(matin.m, matout.m, 6);
}

void test_matrix23_BrMatrix23Identity() {
    br_matrix23 mat;
    br_matrix23 expected = {{
        { 1.f, 0.f, },
        { 0.f, 1.f, },
        { 0.f, 0.f, },
    }};
    BrMatrix23Identity(&mat);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(mat.m, expected.m, 6);
}

void test_matrix23_BrMatrix23Rotate() {
    br_matrix23 mat;
    br_matrix23 expected = {{
        { .5f,           .8660254037f, },
        { -.8660254037f, .5f, },
        { .0f,           .0f, },
    }};
    BrMatrix23Rotate(&mat, BR_ANGLE_DEG(60));
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-3, expected.m, mat.m, 6);
}

void test_matrix23_BrMatrix23Translate() {
    br_matrix23 mat;
    br_matrix23 expected = {{
        { 1.f, 0.f, },
        { 0.f, 1.f, },
        { 3.f, 5.f, },
    }};
    BrMatrix23Translate(&mat, 3.f, 5.f);;
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, mat.m, 6);
}

void test_matrix23_BrMatrix23Scale() {
    br_matrix23 mat;
    br_matrix23 expected = {{
        { 3.f, 0.f, },
        { 0.f, 5.f, },
        { 0.f, 0.f, },
    }};
    BrMatrix23Scale(&mat, 3.f, 5.f);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, mat.m, 6);
}

void test_matrix23_BrMatrix23ShearX() {
    br_matrix23 mat;
    br_matrix23 expected = {{
        { 1.f, 5.f, },
        { 0.f, 1.f, },
        { 0.f, 0.f, },
    }};
    BrMatrix23ShearX(&mat, 5.f);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, mat.m, 6);
}

void test_matrix23_BrMatrix23ShearY() {
    br_matrix23 mat;
    br_matrix23 expected = {{
        { 1.f, 0.f, },
        { 5.f, 1.f, },
        { 0.f, 0.f, },
    }};
    BrMatrix23ShearY(&mat, 5.f);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, mat.m, 6);
}

void test_matrix23_BrMatrix23Inverse() {
    {
        br_matrix23 b;
        br_matrix23 a = {{
            { 1.f, 2.f},
            { 3.f, 4.f},
            { 5.f, 6.f},
        }};
        br_matrix23 expected = {{
            { -2.f,   1.f, },
            {  1.5f, -0.5f, },
            {  1.f,  -2.f, },
        }};
        br_scalar det = BrMatrix23Inverse(&b, &a);
        TEST_ASSERT_FLOAT_WITHIN(1.e-5, -2.f, det);
        TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, b.m, 6);
    }
    {
        br_matrix23 b;
        br_matrix23 a = {{
            { 1.f, 2.f, },
            { 2.f, 4.f, },
            { 2.f, 1.f, },
        }};
        br_scalar det = BrMatrix23Inverse(&b, &a);
        TEST_ASSERT_FLOAT_WITHIN(1.e-5, 0.f, det);
    }
}

void test_matrix23_BrMatrix23LPInverse() {
    br_matrix23 b;
    br_matrix23 a = {{
        { 0.7071067f, -0.7071067f, },
        { 0.7071067f,  0.7071067f, },
        { 3.f,         5.f, },
    }};
    br_matrix23 expected = {{
        {  0.7071067f,  0.7071067f, },
        { -0.7071067f,  0.7071067f, },
        {  1.4142135f, -5.6568542f, },
    }};
    BrMatrix23LPInverse(&b, &a);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-5, expected.m, b.m, 6);
}

void test_matrix23_BrMatrix23LPNormalise() {
    br_matrix23 b;
    br_matrix23 a = {{
        { 1.f,   1e-9f, },
        { 0.f,   1.f, },
        { 3.f,   5.f, },
    }};
    br_matrix23 expected = {{
        { 1.f,   0.f, },
        { 0.f, 1.f, },
        { 3.f,   5.f, },
    }};
    BrMatrix23LPNormalise(&b, &a);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, b.m, 6);
}

void test_matrix23_BrMatrix23ApplyP() {
    br_vector2 vout;
    br_matrix23 mat = {{
        {  1.f,  2.f, },
        {  5.f,  6.f, },
        {  9.f, 10.f, },
    }};
    br_vector2 vin = {
        { 1.f, 2.f, },
    };
    br_vector2 expected = {
        { 20.f, 24.f, },
    };
    BrMatrix23ApplyP(&vout, &vin, &mat);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vout.v, 2);
}

void test_matrix23_BrMatrix23ApplyV() {
    br_vector2 vout;
    br_matrix23 mat = {{
        {  1.f,  2.f, },
        {  5.f,  6.f, },
        {  9.f, 10.f, },
    }};
    br_vector2 vin = {
        { 1.f, 2.f, },
    };
    br_vector2 expected = {
        { 11.f, 14.f, },
    };
    BrMatrix23ApplyV(&vout, &vin, &mat);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vout.v, 2);
}

void test_matrix23_BrMatrix23TApplyP() {
    br_vector2 vout;
    br_matrix23 mat = {{
        {  1.f,  2.f, },
        {  5.f,  6.f, },
        {  9.f, 10.f, },
    }};
    br_vector2 vin = {
        { 1.f, 2.f, },
    };
    br_vector2 expected = {
        { 5.f, 17.f, },
    };
    BrMatrix23TApplyP(&vout, &vin, &mat);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vout.v, 2);
}

void test_matrix23_BrMatrix23TApplyV() {
    br_vector2 vout;
    br_matrix23 mat = {{
        {  1.f,  2.f, },
        {  5.f,  6.f, },
        {  9.f, 10.f, },
    }};
    br_vector2 vin = {
        { 1.f, 2.f, },
    };
    br_vector2 expected = {
        { 5.f, 17.f, },
    };
    BrMatrix23TApplyV(&vout, &vin, &mat);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vout.v, 2);
}

void test_matrix23_BrMatrix23Pre() {
    br_matrix23 mat = {{
        { 1.f,  2.f, },
        { 5.f,  6.f, },
        { 9.f, 10.f, },
    }};
    br_matrix23 a = {{
        { 17.f, 18.f, },
        { 21.f, 22.f, },
        { 25.f, 26.f, },
    }};
    br_matrix23 expected = {{
        { 107.f, 142.f, },
        { 131.f, 174.f, },
        { 164.f, 216.f, },
    }};
    BrMatrix23Pre(&mat, &a);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, mat.m, 6);
}

void test_matrix23_BrMatrix23Post() {
    br_matrix23 mat = {{
        { 1.f,  2.f, },
        { 5.f,  6.f, },
        { 9.f, 10.f, },
    }};
    br_matrix23 a = {{
        { 17.f, 18.f, },
        { 21.f, 22.f, },
        { 25.f, 26.f, },
    }};
    br_matrix23 expected = {{
        {  59.f,  62.f, },
        { 211.f, 222.f, },
        { 388.f, 408.f, },
    }};
    BrMatrix23Post(&mat, &a);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, mat.m, 6);
}

void test_matrix23_BrMatrix23PreRotate() {
    br_matrix23 mat = {{
        { 0.70710678f, -0.70710678f, },
        { 0.70710678f,  0.70710678f, },
        { 3.f, 5.f, },
    }};
    br_matrix23 expected = {{
        { 1.f, 0.f, },
        { 0.f, 1.f, },
        { 3.f, 5.f, },
    }};
    BrMatrix23PreRotate(&mat, BR_ANGLE_DEG(45));
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, mat.m, 6);
}

void test_matrix23_BrMatrix23PostRotate() {
    br_matrix23 mat = {{
        { 0.70710678f, -0.70710678f, },
        { 0.70710678f,  0.70710678f, },
        { 3.f, 5.f, },
    }};
    br_matrix23 expected = {{
        {  1.f,         0.f, },
        {  0.f,         1.f, },
        { -1.41421356f, 5.65685425f, },
    }};
    BrMatrix23PostRotate(&mat, BR_ANGLE_DEG(45));
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, mat.m, 6);
}

void test_matrix23_BrMatrix23PreTranslate() {
    br_matrix23 mat = {{
        { 0.f, -1.f, },
        { 1.f,  0.f, },
        { 3.f,  5.f, },
    }};
    br_matrix23 expected = {{
        { 0.f, -1.f, },
        { 1.f,  0.f, },
        { 8.f,  2.f, },
    }};
    BrMatrix23PreTranslate(&mat, 3.f, 5.f);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, mat.m, 6);
}

void test_matrix23_BrMatrix23PostTranslate() {
    br_matrix23 mat = {{
        { 0.f, -1.f, },
        { 1.f,  0.f, },
        { 3.f,  5.f, },
    }};
    br_matrix23 expected = {{
        { 0.f, -1.f, },
        { 1.f,  0.f, },
        { 6.f, 10.f, },
    }};
    BrMatrix23PostTranslate(&mat, 3.f, 5.f);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, mat.m, 6);
}

void test_matrix23_BrMatrix23PreScale() {
    br_matrix23 mat = {{
        { 0.f, -1.f, },
        { 1.f,  0.f, },
        { 3.f,  5.f, },
    }};
    br_matrix23 expected = {{
        { 0.f, -3.f, },
        { 5.f,  0.f, },
        { 3.f,  5.f, },
    }};
    BrMatrix23PreScale(&mat, 3.f, 5.f);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, mat.m, 6);
}

void test_matrix23_BrMatrix23PostScale() {
    br_matrix23 mat = {{
        { 0.f, -1.f, },
        { 1.f,  0.f, },
        { 3.f,  5.f, },
    }};
    br_matrix23 expected = {{
        { 0.f, -5.f, },
        { 3.f,  0.f, },
        { 9.f, 25.f, },
    }};
    BrMatrix23PostScale(&mat, 3.f, 5.f);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, mat.m, 6);
}

void test_matrix23_BrMatrix23PreShearX() {
    br_matrix23 mat = {{
        { 0.f, -1.f, },
        { 1.f,  0.f, },
        { 3.f,  5.f, },
    }};
    br_matrix23 expected = {{
        { 5.f, -1.f, },
        { 1.f,  0.f, },
        { 3.f,  5.f, },
    }};
    BrMatrix23PreShearX(&mat, 5.f);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, mat.m, 6);
}

void test_matrix23_BrMatrix23PostShearX() {
    br_matrix23 mat = {{
        { 0.f, -1.f, },
        { 1.f,  0.f, },
        { 3.f,  5.f, },
    }};
    br_matrix23 expected = {{
        { 0.f, -1.f, },
        { 1.f,  5.f, },
        { 3.f, 20.f, },
    }};
    BrMatrix23PostShearX(&mat, 5.f);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, mat.m, 6);
}

void test_matrix23_BrMatrix23PreShearY() {
    br_matrix23 mat = {{
        { 0.f, -1.f, },
        { 1.f,  0.f, },
        { 3.f,  5.f, },
    }};
    br_matrix23 expected = {{
        { 0.f, -1.f, },
        { 1.f, -5.f, },
        { 3.f,  5.f, },
    }};
    BrMatrix23PreShearY(&mat, 5.f);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, mat.m, 6);
}

void test_matrix23_BrMatrix23PostShearY() {
    br_matrix23 mat = {{
        { 0.f, -1.f, },
        { 1.f,  0.f, },
        { 3.f,  5.f, },
    }};
    br_matrix23 expected = {{
        { -5.f, -1.f, },
        {  1.f,  0.f, },
        { 28.f,  5.f, },
    }};
    BrMatrix23PostShearY(&mat, 5.f);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-2, expected.m, mat.m, 6);
}

void test_matrix23_suite() {
    RUN_TEST(test_matrix23_BrMatrix23Copy);
    RUN_TEST(test_matrix23_BrMatrix23Identity);
    RUN_TEST(test_matrix23_BrMatrix23Rotate);
    RUN_TEST(test_matrix23_BrMatrix23Translate);
    RUN_TEST(test_matrix23_BrMatrix23Scale);
    RUN_TEST(test_matrix23_BrMatrix23ShearX);
    RUN_TEST(test_matrix23_BrMatrix23ShearY);
    RUN_TEST(test_matrix23_BrMatrix23Inverse);
    RUN_TEST(test_matrix23_BrMatrix23LPInverse);
    RUN_TEST(test_matrix23_BrMatrix23LPNormalise);
    RUN_TEST(test_matrix23_BrMatrix23ApplyP);
    RUN_TEST(test_matrix23_BrMatrix23ApplyV);
    RUN_TEST(test_matrix23_BrMatrix23TApplyP);
    RUN_TEST(test_matrix23_BrMatrix23TApplyV);
    RUN_TEST(test_matrix23_BrMatrix23Pre);
    RUN_TEST(test_matrix23_BrMatrix23Post);
    RUN_TEST(test_matrix23_BrMatrix23PreRotate);
    RUN_TEST(test_matrix23_BrMatrix23PostRotate);
    RUN_TEST(test_matrix23_BrMatrix23PreTranslate);
    RUN_TEST(test_matrix23_BrMatrix23PostTranslate);
    RUN_TEST(test_matrix23_BrMatrix23PreScale);
    RUN_TEST(test_matrix23_BrMatrix23PostScale);
    RUN_TEST(test_matrix23_BrMatrix23PreShearX);
    RUN_TEST(test_matrix23_BrMatrix23PostShearX);
    RUN_TEST(test_matrix23_BrMatrix23PreShearY);
    RUN_TEST(test_matrix23_BrMatrix23PostShearY);
}
