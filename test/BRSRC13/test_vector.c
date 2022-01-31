#include "CORE/MATH/vector.h"
#include "tests.h"
#include <math.h>
#include <string.h>

static void test_vector_BrVector2Copy() {
    br_vector2 vin;
    br_vector2 vout;

    vin.v[0] = 2.25f;
    vin.v[1] = -4.25f;
    memset(&vout, 0, sizeof(br_vector2));

    TEST_ASSERT(vin.v[0] != vout.v[0]);
    TEST_ASSERT(vin.v[1] != vout.v[1]);

    BrVector2Copy(&vout, &vin);

    TEST_ASSERT_EQUAL_FLOAT(vin.v[0], vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(vin.v[1], vout.v[1]);
}

static void test_vector_BrVector2Set() {
    br_vector2 vout;

    memset(&vout, 0, sizeof(br_vector2));

    BrVector2Set(&vout, BR_SCALAR(4.75f), BR_SCALAR(-2.75));

    TEST_ASSERT_EQUAL_FLOAT(4.75f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-2.75f, vout.v[1]);
}

static void test_vector_BrVector2SetInt() {
    br_vector2 vout;

    memset(&vout, 0, sizeof(br_vector2));

    BrVector2SetInt(&vout, 9, -3);

    TEST_ASSERT_EQUAL_FLOAT(9.f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-3.f, vout.v[1]);
}

static void test_vector_BrVector2SetFloat() {
    br_vector2 vout;

    memset(&vout, 0, sizeof(br_vector2));

    BrVector2SetFloat(&vout, BR_SCALAR(4.75f), BR_SCALAR(-2.75));

    TEST_ASSERT_EQUAL_FLOAT(4.75f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-2.75f, vout.v[1]);
}

static void test_vector_BrVector2Negate() {
    br_vector2 vin;
    br_vector2 vout;

    vin.v[0] = 4.75f;
    vin.v[1] = -2.75f;
    memset(&vout, 0, sizeof(br_vector2));

    BrVector2Negate(&vout, &vin);

    TEST_ASSERT_EQUAL_FLOAT(-4.75f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(2.75f, vout.v[1]);
}

static void test_vector_BrVector2Add() {
    br_vector2 vin1;
    br_vector2 vin2;
    br_vector2 vout;

    vin1.v[0] = 4.75f;
    vin1.v[1] = -2.75f;
    vin2.v[0] = 1.5f;
    vin2.v[1] = 0.5f;
    memset(&vout, 0, sizeof(br_vector2));

    BrVector2Add(&vout, &vin1, &vin2);

    TEST_ASSERT_EQUAL_FLOAT(6.25f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-2.25f, vout.v[1]);
}

static void test_vector_BrVector2Accumulate() {
    br_vector2 vin;
    br_vector2 vout;
    
    vin.v[0] = 4.75f;
    vin.v[1] = -2.75f;
    vout.v[0] = 1.5f;
    vout.v[1] = 0.5f;

    BrVector2Accumulate(&vout, &vin);

    TEST_ASSERT_EQUAL_FLOAT(6.25f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-2.25f, vout.v[1]);
}

static void test_vector_BrVector2Sub() {
    br_vector2 vin1;
    br_vector2 vin2;
    br_vector2 vout;
    
    vin1.v[0] = 4.75f;
    vin1.v[1] = -2.75f;
    vin2.v[0] = 1.5f;
    vin2.v[1] = 0.5f;
    memset(&vout, 0, sizeof(br_vector2));

    BrVector2Sub(&vout, &vin1, &vin2);

    TEST_ASSERT_EQUAL_FLOAT(3.25f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-3.25f, vout.v[1]);
}

static void test_vector_BrVector2Scale() {
    br_vector2 vin;
    br_vector2 vout;
    
    vin.v[0] = 4.75f;
    vin.v[1] = -2.75f;
    memset(&vout, 0, sizeof(br_vector2));

    BrVector2Scale(&vout, &vin, BR_SCALAR(1.5f));

    TEST_ASSERT_EQUAL_FLOAT(7.125f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-4.125f, vout.v[1]);
}

static void test_vector_BrVector2InvScale() {
    br_vector2 vin;
    br_vector2 vout;
    
    vin.v[0] = 4.75f;
    vin.v[1] = -2.75f;
    memset(&vout, 0, sizeof(br_vector2));

    BrVector2InvScale(&vout, &vin, BR_SCALAR(2.f));

    TEST_ASSERT_EQUAL_FLOAT(2.375f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-1.375f, vout.v[1]);
}

static void test_vector_BrVector2Dot() {
    br_vector2 vin1;
    br_vector2 vin2;
    br_scalar out;
    
    vin1.v[0] = 4.75f;
    vin1.v[1] = -2.75f;
    vin2.v[0] = 1.5f;
    vin2.v[1] = -0.75f;

    out = BrVector2Dot(&vin1, &vin2);

    TEST_ASSERT_EQUAL_FLOAT(9.1875f, out);
}

static void test_vector_BrVector2Length() {
    br_vector2 vin;
    br_scalar out;
    
    vin.v[0] = 1.5f;
    vin.v[1] = 2.0f;

    out = BrVector2Length(&vin);

    TEST_ASSERT_EQUAL_FLOAT(2.5f, out);
}

static void test_vector_BrVector2LengthSquared() {
    br_vector2 vin;
    br_scalar out;
    
    vin.v[0] = 1.5f;
    vin.v[1] = 2.0f;

    out = BrVector2LengthSquared(&vin);

    TEST_ASSERT_EQUAL_FLOAT(6.25f, out);
}

static void test_vector_BrVector3Copy() {
    br_vector3 vin;
    br_vector3 vout;

    vin.v[0] = 2.25f;
    vin.v[1] = -4.25f;
    vin.v[2] = 9.5f;
    memset(&vout, 0, sizeof(br_vector3));

    TEST_ASSERT(vin.v[0] != vout.v[0]);
    TEST_ASSERT(vin.v[1] != vout.v[1]);
    TEST_ASSERT(vin.v[2] != vout.v[2]);

    BrVector3Copy(&vout, &vin);

    TEST_ASSERT_EQUAL_FLOAT(vin.v[0], vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(vin.v[1], vout.v[1]);
    TEST_ASSERT_EQUAL_FLOAT(vin.v[2], vout.v[2]);
}

static void test_vector_BrVector3Set() {
    br_vector3 vout;

    memset(&vout, 0, sizeof(br_vector3));

    BrVector3Set(&vout, BR_SCALAR(4.75f), BR_SCALAR(-2.75), BR_SCALAR(1.5f));

    TEST_ASSERT_EQUAL_FLOAT(4.75f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-2.75f, vout.v[1]);
    TEST_ASSERT_EQUAL_FLOAT(1.5f, vout.v[2]);
}

static void test_vector_BrVector3SetInt() {
    br_vector3 vout;

    memset(&vout, 0, sizeof(br_vector3));

    BrVector3SetInt(&vout, 9, -3, 5);

    TEST_ASSERT_EQUAL_FLOAT(9.f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-3.f, vout.v[1]);
    TEST_ASSERT_EQUAL_FLOAT(5.f, vout.v[2]);
}

static void test_vector_BrVector3SetFloat() {
    br_vector3 vout;

    memset(&vout, 0, sizeof(br_vector3));

    BrVector3SetFloat(&vout, 4.75f, -2.75, 1.5f);

    TEST_ASSERT_EQUAL_FLOAT(4.75f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-2.75f, vout.v[1]);
    TEST_ASSERT_EQUAL_FLOAT(1.5f, vout.v[2]);
}

static void test_vector_BrVector3Negate() {
    br_vector3 vin;
    br_vector3 vout;

    vin.v[0] = 4.75f;
    vin.v[1] = -2.75f;
    vin.v[2] = 7.5f;
    memset(&vout, 0, sizeof(br_vector3));

    BrVector3Negate(&vout, &vin);

    TEST_ASSERT_EQUAL_FLOAT(-4.75f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(2.75f, vout.v[1]);
    TEST_ASSERT_EQUAL_FLOAT(-7.5f, vout.v[2]);
}

static void test_vector_BrVector3Add() {
    br_vector3 vin1;
    br_vector3 vin2;
    br_vector3 vout;

    vin1.v[0] = 4.75f;
    vin1.v[1] = -2.75f;
    vin1.v[2] = 1.675f;
    vin2.v[0] = 1.5f;
    vin2.v[1] = 0.5f;
    vin2.v[2] = -4.5f;
    memset(&vout, 0, sizeof(br_vector3));

    BrVector3Add(&vout, &vin1, &vin2);

    TEST_ASSERT_EQUAL_FLOAT(6.25f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-2.25f, vout.v[1]);
    TEST_ASSERT_EQUAL_FLOAT(-2.825f, vout.v[2]);
}

static void test_vector_BrVector3Accumulate() {
    br_vector3 vin;
    br_vector3 vout;
    
    vin.v[0] = 4.75f;
    vin.v[1] = -2.75f;
    vin.v[2] = 1.5f;
    vout.v[0] = 1.5f;
    vout.v[1] = 0.5f;
    vout.v[2] = -4.5f;

    BrVector3Accumulate(&vout, &vin);

    TEST_ASSERT_EQUAL_FLOAT(6.25f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-2.25f, vout.v[1]);
    TEST_ASSERT_EQUAL_FLOAT(-3.f, vout.v[2]);
}

static void test_vector_BrVector3Sub() {
    br_vector3 vin1;
    br_vector3 vin2;
    br_vector3 vout;
    
    vin1.v[0] = 4.75f;
    vin1.v[1] = -2.75f;
    vin1.v[2] = 1.75f;
    vin2.v[0] = 1.5f;
    vin2.v[1] = 0.5f;
    vin2.v[2] = -0.5f;
    memset(&vout, 0, sizeof(br_vector3));

    BrVector3Sub(&vout, &vin1, &vin2);

    TEST_ASSERT_EQUAL_FLOAT(3.25f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-3.25f, vout.v[1]);
    TEST_ASSERT_EQUAL_FLOAT(2.25f, vout.v[2]);
}

static void test_vector_BrVector3Scale() {
    br_vector3 vin;
    br_vector3 vout;
    
    vin.v[0] = 4.75f;
    vin.v[1] = -2.75f;
    vin.v[2] = 1.75f;
    memset(&vout, 0, sizeof(br_vector3));

    BrVector3Scale(&vout, &vin, BR_SCALAR(1.5f));

    TEST_ASSERT_EQUAL_FLOAT(7.125f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-4.125f, vout.v[1]);
    TEST_ASSERT_EQUAL_FLOAT(2.625f, vout.v[2]);
}

static void test_vector_BrVector3InvScale() {
    br_vector3 vin;
    br_vector3 vout;
    
    vin.v[0] = 4.75f;
    vin.v[1] = -2.75f;
    vin.v[2] = -5.25f;
    memset(&vout, 0, sizeof(br_vector3));

    BrVector3InvScale(&vout, &vin, BR_SCALAR(2.f));

    TEST_ASSERT_EQUAL_FLOAT(2.375f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-1.375f, vout.v[1]);
    TEST_ASSERT_EQUAL_FLOAT(-2.625f, vout.v[2]);
}

static void test_vector_BrVector3Dot() {
    br_vector3 vin1;
    br_vector3 vin2;
    br_scalar out;
    
    vin1.v[0] = 4.75f;
    vin1.v[1] = -2.75f;
    vin1.v[2] = -9.5f;
    vin2.v[0] = 1.5f;
    vin2.v[1] = -0.75f;
    vin2.v[2] = 1.5f;

    out = BrVector3Dot(&vin1, &vin2);

    TEST_ASSERT_EQUAL_FLOAT(-5.0625f, out);
}

static void test_vector_BrVector3Cross() {
    br_vector3 vin1;
    br_vector3 vin2;
    br_vector3 vout;
    
    vin1.v[0] = 1.f;
    vin1.v[1] = 1.f;
    vin1.v[2] = 0.f;
    vin2.v[0] = -1.f;
    vin2.v[1] = 1.f;
    vin2.v[2] = 0.f;
    memset(&vout, 0, sizeof(br_vector3));

    BrVector3Cross(&vout, &vin1, &vin2);

    TEST_ASSERT_EQUAL_FLOAT(0.f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(0.f, vout.v[1]);
    TEST_ASSERT_EQUAL_FLOAT(2.f, vout.v[2]);
}

static void test_vector_BrVector3Length() {
    br_vector3 vin;
    br_scalar out;
    
    vin.v[0] = 2.f;
    vin.v[1] = 6.5f;
    vin.v[2] = 8.0f;

    out = BrVector3Length(&vin);

    TEST_ASSERT_EQUAL_FLOAT(10.5, out);
}

static void test_vector_BrVector3LengthSquared() {
    br_vector3 vin;
    br_scalar out;

    vin.v[0] = 1.f;
    vin.v[1] = 2.f;
    vin.v[2] = 3.f;

    out = BrVector3LengthSquared(&vin);

    TEST_ASSERT_EQUAL_FLOAT(14.f, out);
}

static void test_vector_BrVector3Normalise() {
    br_vector3 vin;
    br_vector3 vout;

    vin.v[0] = 3.f;
    vin.v[1] = 4.f;
    vin.v[2] = 5.f;
    memset(&vout, 0, sizeof(br_vector3));

    BrVector3Normalise(&vout, &vin);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 0.4242640687119285f, vout.v[0]);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 0.565685424949238f, vout.v[1]);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 0.7071067811865475f, vout.v[2]);
}

static void test_vector_BrVector3NormaliseLP() {
    br_vector3 vin;
    br_vector3 vout;

    vin.v[0] = 0.999;
    vin.v[1] = 0.001f;
    vin.v[2] = 0.001f;
    memset(&vout, 0, sizeof(br_vector3));

    BrVector3NormaliseLP(&vout, &vin);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 1.f, BrVector3Length(&vout));
}

static void test_vector_BrVector4Dot() {
    br_vector4 vin1;
    br_vector4 vin2;
    br_scalar out;
    
    vin1.v[0] = 4.75f;
    vin1.v[1] = -2.75f;
    vin1.v[2] = -9.5f;
    vin1.v[3] = 1.5f;
    vin2.v[0] = 1.5f;
    vin2.v[1] = -0.75f;
    vin2.v[2] = 1.5f;
    vin2.v[3] = 7.75f;

    out = BrVector4Dot(&vin1, &vin2);

    TEST_ASSERT_EQUAL_FLOAT(6.5625f, out);
}

static void test_vector_BrVector4Copy() {
    br_vector4 vin;
    br_vector4 vout;

    vin.v[0] = 2.25f;
    vin.v[1] = -4.25f;
    vin.v[2] = 9.5f;
    vin.v[3] = 4.5;
    memset(&vout, 0, sizeof(br_vector4));

    TEST_ASSERT(vin.v[0] != vout.v[0]);
    TEST_ASSERT(vin.v[1] != vout.v[1]);
    TEST_ASSERT(vin.v[2] != vout.v[2]);
    TEST_ASSERT(vin.v[3] != vout.v[3]);

    BrVector4Copy(&vout, &vin);

    TEST_ASSERT_EQUAL_FLOAT(vin.v[0], vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(vin.v[1], vout.v[1]);
    TEST_ASSERT_EQUAL_FLOAT(vin.v[2], vout.v[2]);
    TEST_ASSERT_EQUAL_FLOAT(vin.v[3], vout.v[3]);
}

static void test_vector_BrFVector2Dot() {
    br_fvector2 vin1;
    br_vector2 vin2;
    br_scalar out;
    
    vin1.v[0] = 4.75f;
    vin1.v[1] = -2.75f;
    vin2.v[0] = 1.5f;
    vin2.v[1] = -0.75f;

    out = BrFVector2Dot(&vin1, &vin2);

    TEST_ASSERT_EQUAL_FLOAT(9.1875f, out);
}

static void test_vector_BrFVector3Copy() {
    br_vector3 vin;
    br_fvector3 vout;

    vin.v[0] = 2.25f;
    vin.v[1] = -4.25f;
    vin.v[2] = 9.5f;
    memset(&vout, 0, sizeof(br_fvector3));

    TEST_ASSERT(vin.v[0] != vout.v[0]);
    TEST_ASSERT(vin.v[1] != vout.v[1]);
    TEST_ASSERT(vin.v[2] != vout.v[2]);

    BrFVector3Copy(&vout, &vin);

    TEST_ASSERT_EQUAL_FLOAT(vin.v[0], vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(vin.v[1], vout.v[1]);
    TEST_ASSERT_EQUAL_FLOAT(vin.v[2], vout.v[2]);
}

static void test_vector_BrVector3ScaleF() {
    br_fvector3 vin;
    br_vector3 vout;
    
    vin.v[0] = 4.75f;
    vin.v[1] = -2.75f;
    vin.v[2] = 1.75f;
    memset(&vout, 0, sizeof(br_vector3));

    BrVector3ScaleF(&vout, &vin, BR_SCALAR(1.5f));

    TEST_ASSERT_EQUAL_FLOAT(7.125f, vout.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-4.125f, vout.v[1]);
    TEST_ASSERT_EQUAL_FLOAT(2.625f, vout.v[2]);
}

static void test_vector_BrFVector3Dot() {
    br_fvector3 vin1;
    br_vector3 vin2;
    br_scalar out;
    
    vin1.v[0] = 4.75f;
    vin1.v[1] = -2.75f;
    vin1.v[2] = -9.5f;
    vin2.v[0] = 1.5f;
    vin2.v[1] = -0.75f;
    vin2.v[2] = 1.5f;

    out = BrFVector3Dot(&vin1, &vin2);

    TEST_ASSERT_EQUAL_FLOAT(-5.0625f, out);
}

static void test_vector_BrFVector3Normalise() {
    br_vector3 vin;
    br_fvector3 vout;

    vin.v[0] = 3.f;
    vin.v[1] = 4.f;
    vin.v[2] = 5.f;
    memset(&vout, 0, sizeof(br_fvector3));

    BrFVector3Normalise(&vout, &vin);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 0.4242640687119285f, vout.v[0]);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 0.565685424949238f, vout.v[1]);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 0.7071067811865475f, vout.v[2]);
}

static void test_vector_BrFVector3NormaliseLP() {
    br_vector3 vin;
    br_fvector3 vout;
    br_scalar length;

    vin.v[0] = 0.999;
    vin.v[1] = 0.001f;
    vin.v[2] = 0.001f;
    memset(&vout, 0, sizeof(br_fvector3));

    BrFVector3NormaliseLP(&vout, &vin);
    length = sqrtf(vout.v[0] * vout.v[0] + vout.v[1] * vout.v[1] + vout.v[2] * vout.v[2]);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 1.f, length);
}

static void test_vector_BrVector2Normalise() {
    br_vector2 vin;
    br_vector2 vout;

    vin.v[0] = 3.f;
    vin.v[1] = 4.f;
    memset(&vout, 0, sizeof(br_vector2));

    BrVector2Normalise(&vout, &vin);

    TEST_ASSERT_FLOAT_WITHIN(1e-5, 0.6f, vout.v[0]);
    TEST_ASSERT_FLOAT_WITHIN(1e-5, 0.8f, vout.v[1]);
}

void test_vector_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_vector_BrVector2Copy);
    RUN_TEST(test_vector_BrVector2Set);
    RUN_TEST(test_vector_BrVector2SetInt);
    RUN_TEST(test_vector_BrVector2SetFloat);
    RUN_TEST(test_vector_BrVector2Negate);
    RUN_TEST(test_vector_BrVector2Add);
    RUN_TEST(test_vector_BrVector2Accumulate);
    RUN_TEST(test_vector_BrVector2Sub);
    RUN_TEST(test_vector_BrVector2Scale);
    RUN_TEST(test_vector_BrVector2InvScale);
    RUN_TEST(test_vector_BrVector2Dot);
    RUN_TEST(test_vector_BrVector2Length);
    RUN_TEST(test_vector_BrVector2LengthSquared);
    RUN_TEST(test_vector_BrVector3Copy);
    RUN_TEST(test_vector_BrVector3Set);
    RUN_TEST(test_vector_BrVector3SetInt);
    RUN_TEST(test_vector_BrVector3SetFloat);
    RUN_TEST(test_vector_BrVector3Negate);
    RUN_TEST(test_vector_BrVector3Add);
    RUN_TEST(test_vector_BrVector3Accumulate);
    RUN_TEST(test_vector_BrVector3Sub);
    RUN_TEST(test_vector_BrVector3Scale);
    RUN_TEST(test_vector_BrVector3InvScale);
    RUN_TEST(test_vector_BrVector3Dot);
    RUN_TEST(test_vector_BrVector3Cross);
    RUN_TEST(test_vector_BrVector3Length);
    RUN_TEST(test_vector_BrVector3LengthSquared);
    RUN_TEST(test_vector_BrVector3Normalise);
    RUN_TEST(test_vector_BrVector3NormaliseLP);
    RUN_TEST(test_vector_BrVector4Dot);
    RUN_TEST(test_vector_BrVector4Copy);
    RUN_TEST(test_vector_BrFVector2Dot);
    RUN_TEST(test_vector_BrFVector3Copy);
    RUN_TEST(test_vector_BrVector3ScaleF);
    RUN_TEST(test_vector_BrFVector3Dot);
    RUN_TEST(test_vector_BrFVector3Normalise);
    RUN_TEST(test_vector_BrFVector3NormaliseLP);
    RUN_TEST(test_vector_BrVector2Normalise);
}
