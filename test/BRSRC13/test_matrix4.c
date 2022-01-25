#include "tests.h"

#include "CORE/MATH/matrix4.h"
#include <stdlib.h>
#include <string.h>

void test_matrix4_BrMatrix4Copy() {
    br_matrix4 matin = {{
        {  1.f,  2.f,  3.f,  4.f, },
        {  5.f,  6.f,  7.f,  8.f, },
        {  9.f, 10.f, 11.f, 12.f, },
        { 13.f, 14.f, 15.f, 16.f, },
    }};
    br_matrix4 matout;
    BrMatrix4Copy(&matout, &matin);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(matin.m, matout.m, 16);
}

void test_matrix4_BrMatrix4Mul() {
    br_matrix4 a;
    br_matrix4 identity;
    // import numpy
    // b = numpy.matrix([[1,2,3,4],[5,6,7,8],[9,10,11,12],[13,14,15,16]])
    // c = numpy.matrix([[17,18,19,20],[21,22,23,24],[25,26,27,28],[29,30,31,32]])
    // b * c
    br_matrix4 b = {{
        {  1.f,  2.f,  3.f,  4.f, },
        {  5.f,  6.f,  7.f,  8.f, },
        {  9.f, 10.f, 11.f, 12.f, },
        { 13.f, 14.f, 15.f, 16.f, },
    }};
    br_matrix4 c = {{
        { 17.f, 18.f, 19.f, 20.f, },
        { 21.f, 22.f, 23.f, 24.f, },
        { 25.f, 26.f, 27.f, 28.f, },
        { 29.f, 30.f, 31.f, 32.f, },
    }};
    br_matrix4 expected = {{
        {  250.f,  260.f,  270.f,  280.f, },
        {  618.f,  644.f,  670.f,  696.f, },
        {  986.f, 1028.f, 1070.f, 1112.f, },
        { 1354.f, 1412.f, 1470.f, 1528.f, },
    }};
    BrMatrix4Identity(&identity);

    BrMatrix4Mul(&a, &b, &c);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, a.m, 16);

    BrMatrix4Mul(&a, &identity, &b);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(b.m, a.m, 16);
}

void test_matrix4_BrMatrix4Identity() {
    br_matrix4 mat;
    br_matrix4 expected = {{
        { 1.f, 0.f, 0.f, 0.f, },
        { 0.f, 1.f, 0.f, 0.f, },
        { 0.f, 0.f, 1.f, 0.f, },
        { 0.f, 0.f, 0.f, 1.f, },
    }};
    BrMatrix4Identity(&mat);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(mat.m, expected.m, 16);
}

void test_matrix4_BrMatrix4Scale() {
    br_matrix4 mat;
    BrMatrix4Scale(&mat, 2.f, 3.f, 5.f);
    br_matrix4 expected = {{
        { 2.f, 0.f, 0.f, 0.f, },
        { 0.f, 3.f, 0.f, 0.f, },
        { 0.f, 0.f, 5.f, 0.f, },
        { 0.f, 0.f, 0.f, 1.f, },
    }};
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(mat.m, expected.m, 16);
}

void test_matrix4_BrMatrix4Inverse() {
    br_matrix4 matout;
    br_matrix4 matin = {{
        {  0.f, -1.f,  0.f,  0.f, },
        {  1.f,  0.f,  0.f,  0.f, },
        {  0.f,  0.f,  0.f, -1.f, },
        {  0.f,  0.f,  1.f,  0.f, },
    }};
    br_matrix4 expected = {{
        {  0.f,  1.f,  0.f,  0.f, },
        { -1.f,  0.f,  0.f,  0.f, },
        {  0.f,  0.f,  0.f,  1.f, },
        {  0.f,  0.f, -1.f,  0.f, },
    }};
    BrMatrix4Inverse(&matout, &matin);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, matout.m, 16);
}

void test_matrix4_BrMatrix4Determinant() {
    br_matrix4 mat = {{
        {  1.f, 2.f, 3.f, 4.f, },
        {  2.f, 3.f, 4.f, 1.f, },
        {  3.f, 4.f, 1.f, 2.f, },
        {  4.f, 1.f, 2.f, 3.f, },
    }};
    float expected = 160.f;
    float determinant = BrMatrix4Determinant(&mat);
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, expected, determinant);
}

void test_matrix4_BrMatrix4Adjoint() {
    br_matrix4 matout;
    br_matrix4 matin = {{
        {  1.f, 2.f, 3.f, 4.f, },
        {  2.f, 3.f, 4.f, 1.f, },
        {  3.f, 4.f, 1.f, 2.f, },
        {  4.f, 1.f, 2.f, 3.f, },
    }};
    br_matrix4 expected = {{
        {-36.f,   4.f,   4.f,  44.f, },
        {  4.f,   4.f,  44.f, -36.f, },
        {  4.f,  44.f, -36.f,   4.f, },
        { 44.f, -36.f,   4.f,   4.f, },
    }};
    BrMatrix4Adjoint(&matout, &matin);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, matout.m, 16);
}

void test_matrix4_BrMatrix4Perspective() {
    br_matrix4 mat;
    float aspect = 4.f / 3.f;
    br_matrix4 expected = {{
        {0.75f,   0.f,   0.f,   0.f, },
        {  0.f,   1.f,   0.f,   0.f, },
        {  0.f,   0.f,   3.f,  -1.f, },
        {  0.f,   0.f,   4.f,   0.f, },
    }};
    BrMatrix4Perspective(&mat, BR_ANGLE_DEG(90), aspect, -1.f, -2.f);
    TEST_ASSERT_FLOAT_ARRAY_WITHIN(1e-3, expected.m, mat.m, 16);
}

void test_matrix4_BrMatrix4Apply() {
    br_vector4 vout;
    br_matrix4 mat = {{
        {  1.f,  2.f,  3.f,  4.f, },
        {  5.f,  6.f,  7.f,  8.f, },
        {  9.f, 10.f, 11.f, 12.f, },
        { 13.f, 14.f, 15.f, 16.f, },
    }};
    br_vector4 vin = {
        { 1.f, 2.f, 3.f, 4.f, },
    };
    BrMatrix4Apply(&vout, &vin, &mat);
    br_vector4 expected = {
        { 90.f, 100.f, 110.f, 120.f, },
    };
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vout.v, 4);
}

void test_matrix4_BrMatrix4ApplyP() {
    br_vector4 vout;
    br_matrix4 mat = {{
        {  1.f,  2.f,  3.f,  4.f, },
        {  5.f,  6.f,  7.f,  8.f, },
        {  9.f, 10.f, 11.f, 12.f, },
        { 13.f, 14.f, 15.f, 16.f, },
    }};
    br_vector3 vin = {
        { 1.f, 2.f, 3.f, },
    };
    br_vector4 expected = {
        { 51.f, 58.f, 65.f, 72.f, },
    };
    BrMatrix4ApplyP(&vout, &vin, &mat);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vout.v, 4);
}

void test_matrix4_BrMatrix4ApplyV() {
    br_vector4 vout;
    br_matrix4 mat = {{
        { 1.f,  2.f,  3.f,  4.f, },
        { 5.f,  6.f,  7.f,  8.f, },
        { 9.f, 10.f, 11.f, 12.f, },
        { 13.f, 14.f, 15.f, 16.f, },
    }};
    br_vector3 vin = {
        { 1.f, 2.f, 3.f, },
    };
    br_vector4 expected = {
        { 38.f, 44.f, 50.f, 56.f, },
    };
    BrMatrix4ApplyV(&vout, &vin, &mat);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vout.v, 4);
}

void test_matrix4_BrMatrix4TApply() {
    br_vector4 vout;
    br_matrix4 mat = {{
        { 1.f,  2.f,  3.f,  4.f, },
        { 5.f,  6.f,  7.f,  8.f, },
        { 9.f, 10.f, 11.f, 12.f, },
        { 13.f, 14.f, 15.f, 16.f, },
    }};
    br_vector4 vin = {
        { 1.f, 2.f, 3.f, 4.f, },
    };
    br_vector4 expected = {
        { 30.f, 70.f, 110.f, 150.f, },
    };
    BrMatrix4TApply(&vout, &vin, &mat);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vout.v, 4);
}

void test_matrix4_BrMatrix4TApplyP() {
    br_vector4 vout;
    br_matrix4 mat = {{
        {  1.f,  2.f,  3.f,  4.f, },
        {  5.f,  6.f,  7.f,  8.f, },
        {  9.f, 10.f, 11.f, 12.f, },
        { 13.f, 14.f, 15.f, 16.f, },
    }};
    br_vector3 vin = {
        { 1.f, 2.f, 3.f, },
    };
    br_vector4 expected = {
        { 18.f, 46.f, 74.f, 102.f, },
    };
    BrMatrix4TApplyP(&vout, &vin, &mat);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vout.v, 4);
}

void test_matrix4_BrMatrix4TApplyV() {
    br_vector4 vout;
    br_matrix4 mat = {{
        {  1.f,  2.f,  3.f,  4.f, },
        {  5.f,  6.f,  7.f,  8.f, },
        {  9.f, 10.f, 11.f, 12.f, },
        { 13.f, 14.f, 15.f, 16.f, },
    }};
    br_vector3 vin = {
        { 1.f, 2.f, 3.f, },
    };
    br_vector4 expected = {
        { 14.f, 38.f, 62.f, 86.f, },
    };
    BrMatrix4TApplyV(&vout, &vin, &mat);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.v, vout.v, 4);
}

void test_matrix4_BrMatrix4Copy34() {
    br_matrix4 matout;
    br_matrix34 matin = {{
        {  1.f,  2.f,  3.f, },
        {  5.f,  6.f,  7.f, },
        {  9.f, 10.f, 11.f, },
        { 13.f, 14.f, 15.f, },
    }};
    br_matrix4 expected = {{
        {  1.f,  2.f,  3.f, 0.f, },
        {  5.f,  6.f,  7.f, 0.f, },
        {  9.f, 10.f, 11.f, 0.f, },
        { 13.f, 14.f, 15.f, 1.f, },
    }};
    BrMatrix4Copy34(&matout, &matin);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, matout.m, 16);
}

void test_matrix4_BrMatrix4Mul34() {
    br_matrix4 a;
    br_matrix34 b = {{
        {  1.f,  2.f,  3.f, },
        {  5.f,  6.f,  7.f, },
        {  9.f, 10.f, 11.f, },
        { 13.f, 14.f, 15.f, },
    }};
    br_matrix4 c = {{
        { 17.f, 18.f, 19.f, 20.f, },
        { 21.f, 22.f, 23.f, 24.f, },
        { 25.f, 26.f, 27.f, 28.f, },
        { 29.f, 30.f, 31.f, 32.f, },
    }};
    br_matrix4 expected = {{
        { 134.f, 140.f,  146.f,  152.f, },
        { 386.f, 404.f,  422.f,  440.f, },
        { 638.f, 668.f,  698.f,  728.f, },
        { 919.f, 962.f, 1005.f, 1048.f, },
    }};
    BrMatrix4Mul34(&a, &b, &c);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, a.m, 16);
}

void test_matrix4_BrMatrix4Pre34() {
    br_matrix34 b = {{
        {  1.f,  2.f,  3.f, },
        {  5.f,  6.f,  7.f, },
        {  9.f, 10.f, 11.f, },
        { 13.f, 14.f, 15.f, },
    }};
    br_matrix4 a = {{
        { 17.f, 18.f, 19.f, 20.f, },
        { 21.f, 22.f, 23.f, 24.f, },
        { 25.f, 26.f, 27.f, 28.f, },
        { 29.f, 30.f, 31.f, 32.f, },
    }};
    br_matrix4 expected = {{
        { 134.f, 140.f,  146.f,  152.f, },
        { 386.f, 404.f,  422.f,  440.f, },
        { 638.f, 668.f,  698.f,  728.f, },
        { 919.f, 962.f, 1005.f, 1048.f, },
    }};
    BrMatrix4Pre34(&a, &b);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, a.m, 16);
}

void test_matrix4_BrMatrix4ShearZ() {
    br_matrix4 a;
    br_matrix4 expected = {{
        { 1.f, 0.f, 0.f, 0.f, },
        { 0.f, 1.f, 0.f, 0.f, },
        { 3.f, 5.f, 1.f, 0.f, },
        { 0.f, 0.f, 0.f, 1.f, },
    }};
    BrMatrix4ShearZ(&a, 3.f, 5.f);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.m, a.m, 16);
}

void test_matrix4_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_matrix4_BrMatrix4Copy);
    RUN_TEST(test_matrix4_BrMatrix4Mul);
    RUN_TEST(test_matrix4_BrMatrix4Identity);
    RUN_TEST(test_matrix4_BrMatrix4Scale);
    RUN_TEST(test_matrix4_BrMatrix4Inverse);
    RUN_TEST(test_matrix4_BrMatrix4Determinant);
    RUN_TEST(test_matrix4_BrMatrix4Adjoint);
    RUN_TEST(test_matrix4_BrMatrix4Perspective);
    RUN_TEST(test_matrix4_BrMatrix4Apply);
    RUN_TEST(test_matrix4_BrMatrix4ApplyP);
    RUN_TEST(test_matrix4_BrMatrix4ApplyV);
    RUN_TEST(test_matrix4_BrMatrix4TApply);
    RUN_TEST(test_matrix4_BrMatrix4TApplyP);
    RUN_TEST(test_matrix4_BrMatrix4TApplyV);
    RUN_TEST(test_matrix4_BrMatrix4Copy34);
    RUN_TEST(test_matrix4_BrMatrix4Mul34);
    RUN_TEST(test_matrix4_BrMatrix4Pre34);
    RUN_TEST(test_matrix4_BrMatrix4ShearZ);
}
