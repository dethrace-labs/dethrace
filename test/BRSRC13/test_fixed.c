#include "CORE/MATH/fixed.h"
#include "tests.h"
#include <math.h>
#include <string.h>

static void test_fixed_conversions() {
    TEST_ASSERT_EQUAL_INT(1<<16, BR_ONE_LS);
    TEST_ASSERT_EQUAL_INT(1<<16, BR_ONE_LU);
    TEST_ASSERT_EQUAL_INT(1<<8, BR_ONE_SS);
    TEST_ASSERT_EQUAL_INT(1<<8, BR_ONE_SU);

    TEST_ASSERT_EQUAL_INT(1<<15, BR_ONE_LSF);
    TEST_ASSERT_EQUAL_INT(1<<16, BR_ONE_LUF);
    TEST_ASSERT_EQUAL_INT(1<<7, BR_ONE_SSF);
    TEST_ASSERT_EQUAL_INT(1<<8, BR_ONE_SUF);

    TEST_ASSERT_EQUAL_INT(0, BrIntToFixed(0));
    TEST_ASSERT_EQUAL_INT((br_fixed_ls)0xffff0000, BrIntToFixed(-1));
    TEST_ASSERT_EQUAL_INT((br_fixed_ls)(42<<16), BrIntToFixed(42));

    TEST_ASSERT_EQUAL_INT(42<<16, BrFloatToFixed(42.f));
    TEST_ASSERT_EQUAL_INT((br_fixed_ls)0x00008000, BrFloatToFixed(.5f));
    TEST_ASSERT_EQUAL_INT((br_fixed_ls)0xffff8000, BrFloatToFixed(-.5f));
    TEST_ASSERT_EQUAL_INT((br_fixed_ls)0x00005555, BrFloatToFixed(.333333333333f));

    TEST_ASSERT_EQUAL_INT(0x00005555, BrScalarToFixed(BR_SCALAR(.333333333333f)));

    TEST_ASSERT_EQUAL_INT(0x42, BrFixedToInt(0x0042ffff));

    TEST_ASSERT_EQUAL_FLOAT(.5f, BrFixedToFloat(0x00008000));
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, .333333333333f, BrFixedToFloat(0x00005555));

    TEST_ASSERT_FLOAT_WITHIN(1e-5f, .333333333333f, BrFixedToScalar(0x00005555));

    TEST_ASSERT_EQUAL_UINT16(0x0000, BrFloatToFixedFraction(0.f));
    TEST_ASSERT_EQUAL_UINT16(0x7fff, BrFloatToFixedFraction(0.99999f));
    TEST_ASSERT_EQUAL_UINT16(0x8000, BrFloatToFixedFraction(-1.f));
    TEST_ASSERT_EQUAL_UINT16(0x6000, BrFloatToFixedFraction(0.75f));
    TEST_ASSERT_EQUAL_UINT16(0xa000, BrFloatToFixedFraction(-0.75f));

    TEST_ASSERT_EQUAL_FLOAT(0.f, BrFixedFractionToFloat(0x0000));
    TEST_ASSERT_EQUAL_FLOAT(0.9999695f, BrFixedFractionToFloat(0x7fff));
    TEST_ASSERT_EQUAL_FLOAT(-1.f, BrFixedFractionToFloat(0x8000));
    TEST_ASSERT_EQUAL_FLOAT(0.75f, BrFixedFractionToFloat(0x6000));
    TEST_ASSERT_EQUAL_FLOAT(-0.75f, BrFixedFractionToFloat(0xa000));
}

static void test_fixed_BrFixedAbs() {
    TEST_ASSERT_EQUAL(BrFloatToFixed(42.f), BrFixedAbs(BrFloatToFixed(42.f)));
    TEST_ASSERT_EQUAL(BrFloatToFixed(42.f), BrFixedAbs(BrFloatToFixed(-42.f)));
    TEST_ASSERT_EQUAL(BrFloatToFixed(1.234f), BrFixedAbs(BrFloatToFixed(-1.234f)));
}

static void test_fixed_BrFixedMul() {
    TEST_ASSERT_EQUAL(BrFloatToFixed(63.f), BrFixedMul(BrFloatToFixed(1.5f), BrFloatToFixed(42.f)));
    TEST_ASSERT_EQUAL(BrFloatToFixed(-63.f), BrFixedMul(BrFloatToFixed(1.5f), BrFloatToFixed(-42.f)));
    TEST_ASSERT_EQUAL(BrFloatToFixed(63.f), BrFixedMul(BrFloatToFixed(-1.5f), BrFloatToFixed(-42.f)));
}

static void test_fixed_BrFixedMac2() {
    TEST_ASSERT_EQUAL(BrFloatToFixed(108.f), BrFixedMac2(BrFloatToFixed(1.5f), BrFloatToFixed(42.f),
                                                BrFloatToFixed(.5f), BrFloatToFixed(90.f)));

}

static void test_fixed_BrFixedMac3() {
    TEST_ASSERT_EQUAL(BrFloatToFixed(133.f), BrFixedMac3(BrFloatToFixed(1.5f), BrFloatToFixed(42.f),
                                                BrFloatToFixed(.5f), BrFloatToFixed(90.f),
                                                BrFloatToFixed(.25f), BrFloatToFixed(100.f)));

}

static void test_fixed_BrFixedMac4() {
    TEST_ASSERT_EQUAL(BrFloatToFixed(138.f), BrFixedMac4(BrFloatToFixed(1.5f), BrFloatToFixed(42.f),
                                                BrFloatToFixed(.5f), BrFloatToFixed(90.f),
                                                BrFloatToFixed(.25f), BrFloatToFixed(100.f),
                                                BrFloatToFixed(.125f), BrFloatToFixed(40.f)));

}

static void test_fixed_BrISqrt32() {
    TEST_ASSERT_EQUAL_UINT16(0, _BrISqrt32(0));
    TEST_ASSERT_EQUAL_UINT16(1, _BrISqrt32(1));
    TEST_ASSERT_EQUAL_UINT16(1, _BrISqrt32(2));
    TEST_ASSERT_EQUAL_UINT16(2, _BrISqrt32(4));
    TEST_ASSERT_EQUAL_UINT16(2, _BrISqrt32(8));
    TEST_ASSERT_EQUAL_UINT16(3, _BrISqrt32(9));
    TEST_ASSERT_EQUAL_UINT16(3, _BrISqrt32(15));
    TEST_ASSERT_EQUAL_UINT16(4, _BrISqrt32(16));
    TEST_ASSERT_EQUAL_UINT16(4, _BrISqrt32(24));
    TEST_ASSERT_EQUAL_UINT16(5, _BrISqrt32(25));
    TEST_ASSERT_EQUAL_UINT16(15, _BrISqrt32(255));
    TEST_ASSERT_EQUAL_UINT16(16, _BrISqrt32(256));
    TEST_ASSERT_EQUAL_UINT16(99, _BrISqrt32(9999));
    TEST_ASSERT_EQUAL_UINT16(100, _BrISqrt32(10000));
    TEST_ASSERT_EQUAL_UINT16(17320, _BrISqrt32(300000000));
    TEST_ASSERT_EQUAL_UINT16(0xffff, _BrISqrt32(0xffffffff));
}

static void test_fixed_BrISqrt64() {
    TEST_ASSERT_EQUAL_UINT32(0, _BrISqrt64(0));
    TEST_ASSERT_EQUAL_UINT32(1, _BrISqrt64(1));
    TEST_ASSERT_EQUAL_UINT32(1, _BrISqrt64(2));
    TEST_ASSERT_EQUAL_UINT32(2, _BrISqrt64(4));
    TEST_ASSERT_EQUAL_UINT32(2, _BrISqrt64(8));
    TEST_ASSERT_EQUAL_UINT32(3, _BrISqrt64(9));
    TEST_ASSERT_EQUAL_UINT32(3, _BrISqrt64(15));
    TEST_ASSERT_EQUAL_UINT32(4, _BrISqrt64(16));
    TEST_ASSERT_EQUAL_UINT32(4, _BrISqrt64(24));
    TEST_ASSERT_EQUAL_UINT32(5, _BrISqrt64(25));
    TEST_ASSERT_EQUAL_UINT32(15, _BrISqrt64(255));
    TEST_ASSERT_EQUAL_UINT32(16, _BrISqrt64(256));
    TEST_ASSERT_EQUAL_UINT32(99, _BrISqrt64(9999));
    TEST_ASSERT_EQUAL_UINT32(100, _BrISqrt64(10000));
    TEST_ASSERT_EQUAL_UINT32(17320, _BrISqrt64(300000000));
    TEST_ASSERT_EQUAL_UINT32(0xffff, _BrISqrt64(0xffffffff));
    TEST_ASSERT_EQUAL_UINT32(0xffffffff, _BrISqrt64(0xffffffffffffffff));
}

static void test_fixed_BrFixedLength2() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(5.f), BrFixedLength2(BrFloatToFixed(3.f), BrFloatToFixed(4.f)));
    TEST_ASSERT_INT32_WITHIN(BrFloatToFixed(1e-4f), BrFloatToFixed(.5f), BrFixedLength2(BrFloatToFixed(.3f), BrFloatToFixed(.4f)));
}

static void test_fixed_BrFixedLength3() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(7.f), BrFixedLength3(BrFloatToFixed(2.f), BrFloatToFixed(3.f), BrFloatToFixed(6.f)));
    TEST_ASSERT_INT32_WITHIN(BrFloatToFixed(1.e-4f), BrFloatToFixed(.7f), BrFixedLength3(BrFloatToFixed(.2f), BrFloatToFixed(.3f), BrFloatToFixed(.6f)));
}

static void test_fixed_BrFixedLength4() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(9.f), BrFixedLength4(BrFloatToFixed(2.f), BrFloatToFixed(4.f), BrFloatToFixed(5.f), BrFloatToFixed(6.f)));
    TEST_ASSERT_INT32_WITHIN(BrFloatToFixed(1e-4f), BrFloatToFixed(.9f), BrFixedLength4(BrFloatToFixed(.2f), BrFloatToFixed(.4f), BrFloatToFixed(.5f), BrFloatToFixed(.6f)));
}

static void test_fixed_BrFixedDiv() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(1.f), BrFixedDiv(BrFloatToFixed(1.f), BrFloatToFixed(1.f)));
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(2.f), BrFixedDiv(BrFloatToFixed(1.f), BrFloatToFixed(.5f)));
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(5.f), BrFixedDiv(BrFloatToFixed(5.f), BrFloatToFixed(1.f)));
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(10.f), BrFixedDiv(BrFloatToFixed(5.f), BrFloatToFixed(.5f)));
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(20.f), BrFixedDiv(BrFloatToFixed(5.f), BrFloatToFixed(.25f)));
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(2.f), BrFixedDiv(BrFloatToFixed(.5f), BrFloatToFixed(.25f)));
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(-5000.f), BrFixedDiv(BrFloatToFixed(20000.f), BrFloatToFixed(-4.f)));
    TEST_ASSERT_EQUAL_UINT32(0xfff1b6dc, BrFixedDiv(-100, 7));
}

static void test_fixed_BrFixedDivR() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(5.f), BrFixedDivR(BrFloatToFixed(5.f), BrFloatToFixed(1.f)));
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(10.f), BrFixedDivR(BrFloatToFixed(5.f), BrFloatToFixed(.5f)));
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(20.f), BrFixedDivR(BrFloatToFixed(5.f), BrFloatToFixed(.25f)));
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(2.f), BrFixedDivR(BrFloatToFixed(.5f), BrFloatToFixed(.25f)));
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(-5000.f), BrFixedDivR(BrFloatToFixed(20000.f), BrFloatToFixed(-4.f)));
    TEST_ASSERT_EQUAL_UINT32(0xfff1db6e, BrFixedDivR(-100, 7));
}

static void test_fixed_BrFixedDivF() {
    TEST_ASSERT_EQUAL_UINT32(1 << 31, BrFixedDivF(BrFloatToFixed(1.f), BrFloatToFixed(1.f)));
    TEST_ASSERT_EQUAL_UINT32(1 << 30, BrFixedDivF(BrFloatToFixed(1.f), BrFloatToFixed(2.f)));
}

static void test_fixed_BrFixedMulDiv() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(7.5f), BrFixedMulDiv(BrFloatToFixed(3.f), BrFloatToFixed(5.f),
                                        BrFloatToFixed(2.f)));
}

static void test_fixed_BrFixedMac2Div() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(13.5f), BrFixedMac2Div(BrFloatToFixed(3.f), BrFloatToFixed(5.f),
                                        BrFloatToFixed(2.f), BrFloatToFixed(6.f),
                                        BrFloatToFixed(2.f)));
}

static void test_fixed_BrFixedMac3Div() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(-18.f), BrFixedMac3Div(BrFloatToFixed(3.f), BrFloatToFixed(5.f),
                                        BrFloatToFixed(2.f), BrFloatToFixed(6.f),
                                        BrFloatToFixed(-7.f), BrFloatToFixed(9.f),
                                        BrFloatToFixed(2.f)));
}

static void test_fixed_BrFixedMac4Div() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(4.5f), BrFixedMac4Div(BrFloatToFixed(3.f), BrFloatToFixed(5.f),
                                        BrFloatToFixed(2.f), BrFloatToFixed(6.f),
                                        BrFloatToFixed(-7.f), BrFloatToFixed(9.f),
                                        BrFloatToFixed(9.f), BrFloatToFixed(5.f),
                                        BrFloatToFixed(2.f)));
}

static void test_fixed_BrFixedFMac2() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(2.75f), BrFixedFMac2(BrFloatToFixedFraction(.25f), BrFloatToFixed(5.f),
                                        BrFloatToFixedFraction(.5f), BrFloatToFixed(3.f)));
}

static void test_fixed_BrFixedFMac3() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(5.75f), BrFixedFMac3(BrFloatToFixedFraction(.25f), BrFloatToFixed(5.f),
                                        BrFloatToFixedFraction(.5f), BrFloatToFixed(3.f),
                                        BrFloatToFixedFraction(.75f), BrFloatToFixed(4.f)));
}

static void test_fixed_BrFixedFMac4() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(6.625f), BrFixedFMac4(BrFloatToFixedFraction(.25f), BrFloatToFixed(5.f),
                                        BrFloatToFixedFraction(.5f), BrFloatToFixed(3.f),
                                        BrFloatToFixedFraction(.75f), BrFloatToFixed(4.f),
                                        BrFloatToFixedFraction(.125f), BrFloatToFixed(7.f)));
}

static void test_fixed_BrFixedRcp() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(4.f), BrFixedRcp(BrFloatToFixed(.25f)));
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(.25f), BrFixedRcp(BrFloatToFixed(4.f)));
}

static void test_fixed_BrFixedSqr() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(.0625f), BrFixedSqr(BrFloatToFixed(.25f)));
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(16.f), BrFixedSqr(BrFloatToFixed(4.f)));
}

static void test_fixed_BrFixedSqr2() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(1.0625f), BrFixedSqr2(BrFloatToFixed(.25f), BrFloatToFixed(1.f)));
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(20.f), BrFixedSqr2(BrFloatToFixed(4.f), BrFloatToFixed(2.f)));
}

static void test_fixed_BrFixedSqr3() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(5.0625f), BrFixedSqr3(BrFloatToFixed(.25f), BrFloatToFixed(1.f), BrFloatToFixed(2.f)));
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(29.f), BrFixedSqr3(BrFloatToFixed(4.f), BrFloatToFixed(2.f), BrFloatToFixed(3.f)));
}

static void test_fixed_BrFixedSqr4() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(14.0625f), BrFixedSqr4(BrFloatToFixed(.25f), BrFloatToFixed(1.f), BrFloatToFixed(2.f), BrFloatToFixed(-3.f)));
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(45.f), BrFixedSqr4(BrFloatToFixed(4.f), BrFloatToFixed(2.f), BrFloatToFixed(3.f), BrFloatToFixed(-4.f)));
}

static void test_fixed_BrFixedSin() {
    TEST_ASSERT_EQUAL_UINT32(BrFloatToFixed(.0f), BrFixedSin(BrRadianToAngle(0.0f * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(.5f),                   BrFixedSin(BrRadianToAngle((1.f/6.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(.7071067811865476f),    BrFixedSin(BrRadianToAngle((1.f/4.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(.8660254037844386f),    BrFixedSin(BrRadianToAngle((1.f/3.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(1.f),                   BrFixedSin(BrRadianToAngle((1.f/2.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(.8660254037844386f),    BrFixedSin(BrRadianToAngle((2.f/3.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(.7071067811865476f),    BrFixedSin(BrRadianToAngle((3.f/4.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(.5f),                   BrFixedSin(BrRadianToAngle((5.f/6.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(.0f),                   BrFixedSin(BrRadianToAngle(1.0f * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(-.5f),                  BrFixedSin(BrRadianToAngle((7.f/6.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(-.7071067811865476f),   BrFixedSin(BrRadianToAngle((5.f/4.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(-.8660254037844386f),   BrFixedSin(BrRadianToAngle((4.f/3.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(-1.f),                  BrFixedSin(BrRadianToAngle((-1.f/2.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(-.8660254037844386f),   BrFixedSin(BrRadianToAngle((5.f/3.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(-.7071067811865476f),   BrFixedSin(BrRadianToAngle((7.f/4.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(-.5f),                  BrFixedSin(BrRadianToAngle((11.f/6.f) * PI)));
}

static void test_fixed_BrFixedCos() {
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(1.f),                   BrFixedCos(BrRadianToAngle(0.0f * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(.8660254037844386f),    BrFixedCos(BrRadianToAngle((1.f/6.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(.7071067811865476f),    BrFixedCos(BrRadianToAngle((1.f/4.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(.5f),                   BrFixedCos(BrRadianToAngle((1.f/3.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(0.f),                   BrFixedCos(BrRadianToAngle((1.f/2.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(-.5f),                  BrFixedCos(BrRadianToAngle((2.f/3.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(-.7071067811865476f),   BrFixedCos(BrRadianToAngle((3.f/4.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(-.8660254037844386f),   BrFixedCos(BrRadianToAngle((5.f/6.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(-1.f),                  BrFixedCos(BrRadianToAngle(1.0f * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(-.8660254037844386f),   BrFixedCos(BrRadianToAngle((7.f/6.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(-.7071067811865476f),   BrFixedCos(BrRadianToAngle((5.f/4.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(-.5f),                  BrFixedCos(BrRadianToAngle((4.f/3.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(0.f),                   BrFixedCos(BrRadianToAngle((-1.f/2.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(.5f),                   BrFixedCos(BrRadianToAngle((5.f/3.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(.7071067811865476f),    BrFixedCos(BrRadianToAngle((7.f/4.f) * PI)));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(.8660254037844386f),    BrFixedCos(BrRadianToAngle((11.f/6.f) * PI)));
}

static void test_fixed_BrFixedASin() {
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle(( 3.f / 2.f) * PI), BrFixedASin(BrFloatToFixed(-1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle(( 5.f / 3.f) * PI), BrFixedASin(BrFloatToFixed(-.8660254037844386f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle(( 7.f / 4.f) * PI), BrFixedASin(BrFloatToFixed(-.7071067811865476f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle((11.f / 6.f) * PI), BrFixedASin(BrFloatToFixed(-.5f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 0.0f        * PI), BrFixedASin(BrFloatToFixed(.0f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle(( 1.f / 6.f) * PI), BrFixedASin(BrFloatToFixed(.5f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle(( 1.f / 4.f) * PI), BrFixedASin(BrFloatToFixed(.7071067811865476f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle(( 1.f / 3.f) * PI), BrFixedASin(BrFloatToFixed(.8660254037844386f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle(( 1.f / 2.f) * PI), BrFixedASin(BrFloatToFixed(1.f)));
}

static void test_fixed_BrFixedACos() {
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 1.f        * PI), BrFixedACos(BrFloatToFixed(-1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle((5.f / 6.f) * PI), BrFixedACos(BrFloatToFixed(-.8660254037844386f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle((3.f / 4.f) * PI), BrFixedACos(BrFloatToFixed(-.7071067811865476f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle((2.f / 3.f) * PI), BrFixedACos(BrFloatToFixed(-.5f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle((1.f / 2.f) * PI), BrFixedACos(BrFloatToFixed(.0f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle((1.f / 3.f) * PI), BrFixedACos(BrFloatToFixed(.5f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle((1.f / 4.f) * PI), BrFixedACos(BrFloatToFixed(.7071067811865476f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle((1.f / 6.f) * PI), BrFixedACos(BrFloatToFixed(.8660254037844386f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 0.f        * PI), BrFixedACos(BrFloatToFixed(1.f)));
}

static void test_fixed_BrFixedATan2() {
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 0.f       * PI), BrFixedATan2(BrFloatToFixed( 1.f), BrFloatToFixed(0.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 1.f / 6.f * PI), BrFixedATan2(BrFloatToFixed( 1.f), BrFloatToFixed(0.5773502691896258f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 1.f / 4.f * PI), BrFixedATan2(BrFloatToFixed( 1.f), BrFloatToFixed(1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 1.f / 3.f * PI), BrFixedATan2(BrFloatToFixed( 1.f), BrFloatToFixed(1.7320508075688772f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 1.f / 2.f * PI), BrFixedATan2(BrFloatToFixed( 0.f), BrFloatToFixed(1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 2.f / 3.f * PI), BrFixedATan2(BrFloatToFixed(-1.f), BrFloatToFixed(1.7320508075688772f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 3.f / 4.f * PI), BrFixedATan2(BrFloatToFixed(-1.f), BrFloatToFixed(1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 5.f / 6.f * PI), BrFixedATan2(BrFloatToFixed(-1.f), BrFloatToFixed(0.5773502691896258f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 1.f       * PI), BrFixedATan2(BrFloatToFixed(-1.f), BrFloatToFixed(0.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 7.f / 6.f * PI), BrFixedATan2(BrFloatToFixed(-1.f), BrFloatToFixed(-0.5773502691896258f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 5.f / 4.f * PI), BrFixedATan2(BrFloatToFixed(-1.f), BrFloatToFixed(-1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 4.f / 3.f * PI), BrFixedATan2(BrFloatToFixed(-1.f), BrFloatToFixed(-1.7320508075688772f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 3.f / 2.f * PI), BrFixedATan2(BrFloatToFixed( 0.f), BrFloatToFixed(-1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 5.f / 3.f * PI), BrFixedATan2(BrFloatToFixed( 1.f), BrFloatToFixed(-1.7320508075688772f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle( 7.f / 4.f * PI), BrFixedATan2(BrFloatToFixed( 1.f), BrFloatToFixed(-1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle(11.f / 6.f * PI), BrFixedATan2(BrFloatToFixed( 1.f), BrFloatToFixed(-0.5773502691896258f)));

    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle(1.f / 2.f * PI), BrFixedATan2(BrFloatToFixed(1.f), BrFloatToFixed(16000.f)));

    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle(1.f / 6.f * PI), BrFixedATan2(BrFloatToFixed(1.7320508075688772f), BrFloatToFixed(1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle(1.f / 3.f * PI), BrFixedATan2(BrFloatToFixed(0.5773502691896258f), BrFloatToFixed(1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle(1.f / 6.f * PI), BrFixedATan2(BrFloatToFixed(1.7320508075688772f), BrFloatToFixed(1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-3f), BrRadianToAngle(1.f / 3.f * PI), BrFixedATan2(BrFloatToFixed(0.5773502691896258f), BrFloatToFixed(1.f)));
}

static void test_fixed_BrFixedATan2Fast() {
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle( 0.f       * PI),     BrFixedATan2Fast(BrFloatToFixed( 1.f), BrFloatToFixed(0.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle( 1.f / 6.f * PI),     BrFixedATan2Fast(BrFloatToFixed( 1.f), BrFloatToFixed(0.5773502691896258f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle( 1.f / 4.f * PI),     BrFixedATan2Fast(BrFloatToFixed( 1.f), BrFloatToFixed(1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle( 1.f / 3.f * PI),     BrFixedATan2Fast(BrFloatToFixed( 1.f), BrFloatToFixed(1.7320508075688772f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle( 1.f / 2.f * PI),     BrFixedATan2Fast(BrFloatToFixed( 0.f), BrFloatToFixed(1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle( 2.f / 3.f * PI),     BrFixedATan2Fast(BrFloatToFixed(-1.f), BrFloatToFixed(1.7320508075688772f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle( 3.f / 4.f * PI),     BrFixedATan2Fast(BrFloatToFixed(-1.f), BrFloatToFixed(1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle( 5.f / 6.f * PI),     BrFixedATan2Fast(BrFloatToFixed(-1.f), BrFloatToFixed(0.5773502691896258f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle( 1.f       * PI),     BrFixedATan2Fast(BrFloatToFixed(-1.f), BrFloatToFixed(0.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle( 7.f / 6.f * PI),     BrFixedATan2Fast(BrFloatToFixed(-1.f), BrFloatToFixed(-0.5773502691896258f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle( 5.f / 4.f * PI),     BrFixedATan2Fast(BrFloatToFixed(-1.f), BrFloatToFixed(-1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle( 4.f / 3.f * PI),     BrFixedATan2Fast(BrFloatToFixed(-1.f), BrFloatToFixed(-1.7320508075688772f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle( 3.f / 2.f * PI),     BrFixedATan2Fast(BrFloatToFixed( 0.f), BrFloatToFixed(-1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle( 5.f / 3.f * PI),     BrFixedATan2Fast(BrFloatToFixed( 1.f), BrFloatToFixed(-1.7320508075688772f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle( 7.f / 4.f * PI),     BrFixedATan2Fast(BrFloatToFixed( 1.f), BrFloatToFixed(-1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle(11.f / 6.f * PI),     BrFixedATan2Fast(BrFloatToFixed( 1.f), BrFloatToFixed(-0.5773502691896258f)));

    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle(1.f / 2.f * PI), BrFixedATan2Fast(BrFloatToFixed(1.f), BrFloatToFixed(30000.f)));

    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle(1.f / 6.f * PI), BrFixedATan2Fast(BrFloatToFixed(1.7320508075688772f), BrFloatToFixed(1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle(1.f / 3.f * PI), BrFixedATan2Fast(BrFloatToFixed(0.5773502691896258f), BrFloatToFixed(1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle(1.f / 6.f * PI), BrFixedATan2Fast(BrFloatToFixed(1.7320508075688772f), BrFloatToFixed(1.f)));
    TEST_ASSERT_UINT16_WITHIN(BrRadianToAngle(1e-1f), BrRadianToAngle(1.f / 3.f * PI), BrFixedATan2Fast(BrFloatToFixed(0.5773502691896258f), BrFloatToFixed(1.f)));
}

static void test_fixed_BrFastSqrt32() {
    TEST_ASSERT_UINT16_WITHIN(5, 0, _BrFastSqrt32(0));
    TEST_ASSERT_UINT16_WITHIN(5, 1, _BrFastSqrt32(1));
    TEST_ASSERT_UINT16_WITHIN(5, 1, _BrFastSqrt32(2));
    TEST_ASSERT_UINT16_WITHIN(5, 2, _BrFastSqrt32(4));
    TEST_ASSERT_UINT16_WITHIN(5, 2, _BrFastSqrt32(8));
    TEST_ASSERT_UINT16_WITHIN(5, 3, _BrFastSqrt32(9));
    TEST_ASSERT_UINT16_WITHIN(5, 3, _BrFastSqrt32(15));
    TEST_ASSERT_UINT16_WITHIN(5, 4, _BrFastSqrt32(16));
    TEST_ASSERT_UINT16_WITHIN(5, 4, _BrFastSqrt32(24));
    TEST_ASSERT_UINT16_WITHIN(5, 5, _BrFastSqrt32(25));
    TEST_ASSERT_UINT16_WITHIN(5, 15, _BrFastSqrt32(255));
    TEST_ASSERT_UINT16_WITHIN(5, 16, _BrFastSqrt32(256));
    TEST_ASSERT_UINT16_WITHIN(5, 99, _BrFastSqrt32(9999));
    TEST_ASSERT_UINT16_WITHIN(5, 100, _BrFastSqrt32(10000));
    TEST_ASSERT_UINT16_WITHIN(0x100, 17320, _BrFastSqrt32(300000000));
    TEST_ASSERT_UINT16_WITHIN(0x100, 0xffff, _BrFastSqrt32(0xffffffff));
}

static void test_fixed_BrFastRSqrt32() {
    TEST_ASSERT_UINT32_WITHIN(0, 0x00000, _BrFastRSqrt32(0));
    TEST_ASSERT_UINT32_WITHIN(1, 0x10000, _BrFastRSqrt32(1));
    TEST_ASSERT_UINT32_WITHIN(1, (uint32_t)(0x10000 / sqrtf(2.f)), _BrFastRSqrt32(2));
    TEST_ASSERT_UINT32_WITHIN(1, 0x08000, _BrFastRSqrt32(4));
    TEST_ASSERT_UINT32_WITHIN(1, (uint32_t)(0x10000 / sqrtf(8.f)), _BrFastRSqrt32(8));
    TEST_ASSERT_UINT32_WITHIN(1, (uint32_t)(0x10000 / sqrtf(9.f)), _BrFastRSqrt32(9));
    TEST_ASSERT_UINT32_WITHIN(1, (uint32_t)(0x10000 / sqrtf(15.f)), _BrFastRSqrt32(15));
    TEST_ASSERT_UINT32_WITHIN(1, 0x04000, _BrFastRSqrt32(16));
    TEST_ASSERT_UINT32_WITHIN(1, (uint32_t)(0x10000 / sqrtf(24.f)), _BrFastRSqrt32(24));
    TEST_ASSERT_UINT32_WITHIN(1, (uint32_t)(0x10000 / sqrtf(25.f)), _BrFastRSqrt32(25));
    TEST_ASSERT_UINT32_WITHIN(10, (uint32_t)(0x10000 / sqrtf(255.f)), _BrFastRSqrt32(255));
    TEST_ASSERT_UINT32_WITHIN(10, 0x01000, _BrFastRSqrt32(256));
    TEST_ASSERT_UINT32_WITHIN(10, 0x00800, _BrFastRSqrt32(1024));
    TEST_ASSERT_UINT32_WITHIN(10, (uint32_t)(0x10000 / sqrtf(9999.f)), _BrFastRSqrt32(9999));
    TEST_ASSERT_UINT32_WITHIN(10, (uint32_t)(0x10000 / sqrtf(10000.f)), _BrFastRSqrt32(10000));
    TEST_ASSERT_UINT32_WITHIN(10, (uint32_t)(0x10000 / sqrtf(300000000.f)), _BrFastRSqrt32(300000000));
    TEST_ASSERT_UINT32_WITHIN(10, (uint32_t)(0x10000 / sqrtf((float)0xffffffff)), _BrFastRSqrt32(0xffffffff));
}

static void test_fixed_BrFastRSqrt64() {
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), 0x00000000, _BrFastRSqrt64(0));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-0f), 0xffffffff, _BrFastRSqrt64(1));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-0f), 0x80000000, _BrFastRSqrt64(4));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-0f), 0x40000000, _BrFastRSqrt64(16));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-0f), 0x20000000, _BrFastRSqrt64(64));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-0f), 0x10000000, _BrFastRSqrt64(256));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-1f), 0x08000000, _BrFastRSqrt64(1024));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-2f), 0x1000000, _BrFastRSqrt64(0x00010000ULL));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), 0x000100000, _BrFastRSqrt64(0x01000000ULL));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-1f), BrFloatToFixed(1.f / sqrtf(2.f)), _BrFastRSqrt64(2ULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), 0x00008000, _BrFastRSqrt64(4ULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), BrFloatToFixed(1.f / sqrtf(8.f)), _BrFastRSqrt64(8ULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), BrFloatToFixed(1.f / sqrtf(9.f)), _BrFastRSqrt64(9ULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), BrFloatToFixed(1.f / sqrtf(15.f)), _BrFastRSqrt64(15ULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), 0x4000, _BrFastRSqrt64(16ULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), BrFloatToFixed(1.f / sqrtf(24.f)), _BrFastRSqrt64(24ULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), BrFloatToFixed(1.f / sqrtf(25.f)), _BrFastRSqrt64(25ULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), BrFloatToFixed(1.f / sqrtf(81.f)), _BrFastRSqrt64(81ULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), BrFloatToFixed(1.f / sqrtf(255.f)), _BrFastRSqrt64(255ULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), 0x1000, _BrFastRSqrt64(256ULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), 0x00800, _BrFastRSqrt64(1024ULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), BrFloatToFixed(1.f / sqrtf(9999.f)), _BrFastRSqrt64(9999ULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), BrFloatToFixed(1.f / sqrtf(10000.f)), _BrFastRSqrt64(10000ULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), BrFloatToFixed(1.f / sqrtf(300000000.f)), _BrFastRSqrt64(300000000ULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), BrFloatToFixed(1.f / sqrtf(1600000000000.f)), _BrFastRSqrt64(1600000000000ULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), BrFloatToFixed(1.f / sqrtf((float)0xffffffff)), _BrFastRSqrt64(0xffffffffULL<<32));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), 0x10000, _BrFastRSqrt64(0x100000000));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e-3f), 0x8000, _BrFastRSqrt64(0x400000000));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e1f), BrFloatToFixed(1.f / sqrtf((float)0x1000000000)), _BrFastRSqrt64(0x1000000000ULL));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e1f), BrFloatToFixed(1.f / sqrtf((float)0x100000000000)), _BrFastRSqrt64(0x100000000000ULL));
    TEST_ASSERT_UINT32_WITHIN(BrFloatToFixed(1.e1f), BrFloatToFixed(1.f / sqrtf((float)0xffffffffffffffffULL)), _BrFastRSqrt64(0xffffffffffffffffULL));
}

static void test_fixed_BrFixedRLength2() {
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(.2f), BrFixedRLength2(BrFloatToFixed(3.f), BrFloatToFixed(4.f)));
    TEST_ASSERT_INT32_WITHIN(BrFloatToFixed(1e-2f), BrFloatToFixed(2.f), BrFixedRLength2(BrFloatToFixed(.3f), BrFloatToFixed(.4f)));
}

static void test_fixed_BrFixedRLength3() {
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(1/7.f), BrFixedRLength3(BrFloatToFixed(2.f), BrFloatToFixed(3.f), BrFloatToFixed(6.f)));
    TEST_ASSERT_INT32_WITHIN(BrFloatToFixed(1.e-2f), BrFloatToFixed(1/.7f), BrFixedRLength3(BrFloatToFixed(.2f), BrFloatToFixed(.3f), BrFloatToFixed(.6f)));
}

static void test_fixed_BrFixedRLength4() {
    TEST_ASSERT_EQUAL_INT32(BrFloatToFixed(1/9.f), BrFixedRLength4(BrFloatToFixed(2.f), BrFloatToFixed(4.f), BrFloatToFixed(5.f), BrFloatToFixed(6.f)));
    TEST_ASSERT_INT32_WITHIN(BrFloatToFixed(1e-3f), BrFloatToFixed(1/.9f), BrFixedRLength4(BrFloatToFixed(.2f), BrFloatToFixed(.4f), BrFloatToFixed(.5f), BrFloatToFixed(.6f)));
}

void test_fixed_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_fixed_conversions);
    RUN_TEST(test_fixed_BrFixedAbs);
    RUN_TEST(test_fixed_BrFixedMul);
    RUN_TEST(test_fixed_BrFixedMac2);
    RUN_TEST(test_fixed_BrFixedMac3);
    RUN_TEST(test_fixed_BrFixedMac4);
    RUN_TEST(test_fixed_BrISqrt32);
    RUN_TEST(test_fixed_BrISqrt64);
    RUN_TEST(test_fixed_BrFixedLength2);
    RUN_TEST(test_fixed_BrFixedLength3);
    RUN_TEST(test_fixed_BrFixedLength4);
    RUN_TEST(test_fixed_BrFixedDiv);
    RUN_TEST(test_fixed_BrFixedDivR);
    RUN_TEST(test_fixed_BrFixedDivF);
    RUN_TEST(test_fixed_BrFixedMulDiv);
    RUN_TEST(test_fixed_BrFixedMac2Div);
    RUN_TEST(test_fixed_BrFixedMac3Div);
    RUN_TEST(test_fixed_BrFixedMac4Div);
    RUN_TEST(test_fixed_BrFixedFMac2);
    RUN_TEST(test_fixed_BrFixedFMac3);
    RUN_TEST(test_fixed_BrFixedFMac4);
    RUN_TEST(test_fixed_BrFixedRcp);
    RUN_TEST(test_fixed_BrFixedSqr);
    RUN_TEST(test_fixed_BrFixedSqr2);
    RUN_TEST(test_fixed_BrFixedSqr3);
    RUN_TEST(test_fixed_BrFixedSqr4);
    RUN_TEST(test_fixed_BrFixedSin);
    RUN_TEST(test_fixed_BrFixedCos);
    RUN_TEST(test_fixed_BrFixedASin);
    RUN_TEST(test_fixed_BrFixedACos);
    RUN_TEST(test_fixed_BrFixedATan2);
    RUN_TEST(test_fixed_BrFixedATan2Fast);
    RUN_TEST(test_fixed_BrFastSqrt32);
    RUN_TEST(test_fixed_BrFastRSqrt32);
    RUN_TEST(test_fixed_BrFastRSqrt64);
    RUN_TEST(test_fixed_BrFixedRLength2);
    RUN_TEST(test_fixed_BrFixedRLength3);
    RUN_TEST(test_fixed_BrFixedRLength4);
}
