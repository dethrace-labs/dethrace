#include "tests.h"
#include "CORE/FW/bswap.h"

#include <string.h>

static void test_bswap_BrSwap32() {
    TEST_ASSERT_EQUAL_UINT32(0x00000001, BrSwap32(0x01000000));
    TEST_ASSERT_EQUAL_UINT32(0x12345678, BrSwap32(0x78563412));
    TEST_ASSERT_EQUAL_UINT32(0x01020304, BrSwap32(0x04030201));
    TEST_ASSERT_EQUAL_UINT32(0x10000000, BrSwap32(0x00000010));
    TEST_ASSERT_EQUAL_UINT32(0x00100000, BrSwap32(0x00001000));
}

static void test_bswap_BrSwap16() {
    TEST_ASSERT_EQUAL_UINT16(0x1234, BrSwap16(0x3412));
    TEST_ASSERT_EQUAL_UINT16(0x0102, BrSwap16(0x0201));
    TEST_ASSERT_EQUAL_UINT16(0x1000, BrSwap16(0x0010));
    TEST_ASSERT_EQUAL_UINT16(0x0100, BrSwap16(0x0001));
    TEST_ASSERT_EQUAL_UINT16(0x0010, BrSwap16(0x1000));
    TEST_ASSERT_EQUAL_UINT16(0x0001, BrSwap16(0x0100));
}

static float swap_float(float f) {
    union {
        uint8_t c[4];
        float f;
    } u;
    uint8_t tmp;

    u.f = f;
    tmp = u.c[0]; u.c[0] = u.c[3]; u.c[3] = tmp;
    tmp = u.c[1]; u.c[1] = u.c[2]; u.c[2] = tmp;
    return u.f;
}

static uint32_t float_long_representation(float f) {
    union {
        float f;
        uint32_t l;
    } u;
    u.f = f;
    return u.l;
}

static void test_bswap_BrSwapFloat() {
    TEST_ASSERT_EQUAL_UINT32(0x3f800000, float_long_representation(1.f));

    TEST_ASSERT_EQUAL_UINT32(float_long_representation(swap_float(1.f)), float_long_representation(BrSwapFloat(1.f)));
    TEST_ASSERT_EQUAL_UINT32(float_long_representation(swap_float(900.f)), float_long_representation(BrSwapFloat(900.f)));
    TEST_ASSERT_EQUAL_UINT32(float_long_representation(swap_float(-42.f)), float_long_representation(BrSwapFloat(-42.f)));
    TEST_ASSERT_EQUAL_UINT32(float_long_representation(swap_float(1.2545f)), float_long_representation(BrSwapFloat(1.2545f)));
    TEST_ASSERT_EQUAL_UINT32(float_long_representation(swap_float(5555555e5f)), float_long_representation(BrSwapFloat(5555555e5f)));
    TEST_ASSERT_EQUAL_UINT32(float_long_representation(swap_float(-5152.7e3f)), float_long_representation(BrSwapFloat(-5152.7e3f)));
    TEST_ASSERT_EQUAL_UINT32(float_long_representation(swap_float(7.5515151f)), float_long_representation(BrSwapFloat(7.5515151f)));
}

static void test_bswap_BrSwapBlock_size1() {
    char buffer[256];
    int i;

    for (i = 0; i < BR_ASIZE(buffer); i++) {
        buffer[i] = i;
    }

    BrSwapBlock(buffer, BR_ASIZE(buffer)/1, 1);

    for (i = 0; i < BR_ASIZE(buffer); i++) {
        TEST_ASSERT_EQUAL_UINT8(i, buffer[i]);
    }
}

static void test_bswap_BrSwapBlock_size2() {
    char buffer[256];
    int i;

    for (i = 0; i < BR_ASIZE(buffer); i++) {
        buffer[i] = i;
    }

    BrSwapBlock(buffer, BR_ASIZE(buffer)/2, 2);

    for (i = 0; i < (BR_ASIZE(buffer)/2); i++) {
        TEST_ASSERT_EQUAL_UINT8(2*i+0, buffer[2*i+1]);
        TEST_ASSERT_EQUAL_UINT8(2*i+1, buffer[2*i+0]);
    }
}

static void test_bswap_BrSwapBlock_size3() {
    char buffer[256];
    int i;

    for (i = 0; i < BR_ASIZE(buffer); i++) {
        buffer[i] = i;
    }

    BrSwapBlock(buffer, BR_ASIZE(buffer)/3, 3);

    for (i = 0; i < (BR_ASIZE(buffer)/3); i++) {
        TEST_ASSERT_EQUAL_UINT8(3*i+0, buffer[3*i+2]);
        TEST_ASSERT_EQUAL_UINT8(3*i+1, buffer[3*i+1]);
        TEST_ASSERT_EQUAL_UINT8(3*i+2, buffer[3*i+0]);
    }
}

static void test_bswap_BrSwapBlock_size4() {
    char buffer[256];
    int i;

    for (i = 0; i < BR_ASIZE(buffer); i++) {
        buffer[i] = i;
    }

    BrSwapBlock(buffer, BR_ASIZE(buffer)/4, 4);

    for (i = 0; i < (BR_ASIZE(buffer)/4); i++) {
        TEST_ASSERT_EQUAL_UINT8(4*i+0, buffer[4*i+3]);
        TEST_ASSERT_EQUAL_UINT8(4*i+1, buffer[4*i+2]);
        TEST_ASSERT_EQUAL_UINT8(4*i+2, buffer[4*i+1]);
        TEST_ASSERT_EQUAL_UINT8(4*i+3, buffer[4*i+0]);
    }
}

static void test_bswap_BrSwapBlock_size5() {
    char buffer[256];
    int i;

    for (i = 0; i < BR_ASIZE(buffer); i++) {
        buffer[i] = i;
    }

    BrSwapBlock(buffer, BR_ASIZE(buffer)/5, 5);

    for (i = 0; i < (BR_ASIZE(buffer)/5); i++) {
        TEST_ASSERT_EQUAL_UINT8(5*i+0, buffer[5*i+4]);
        TEST_ASSERT_EQUAL_UINT8(5*i+1, buffer[5*i+3]);
        TEST_ASSERT_EQUAL_UINT8(5*i+2, buffer[5*i+2]);
        TEST_ASSERT_EQUAL_UINT8(5*i+3, buffer[5*i+1]);
        TEST_ASSERT_EQUAL_UINT8(5*i+4, buffer[5*i+0]);
    }
}

static void test_bswap_BrSwapBlock_size6() {
    char buffer[256];
    int i;

    for (i = 0; i < BR_ASIZE(buffer); i++) {
        buffer[i] = i;
    }

    BrSwapBlock(buffer, BR_ASIZE(buffer)/6, 6);

    for (i = 0; i < (BR_ASIZE(buffer)/6); i++) {
        TEST_ASSERT_EQUAL_UINT8(6*i+0, buffer[6*i+5]);
        TEST_ASSERT_EQUAL_UINT8(6*i+1, buffer[6*i+4]);
        TEST_ASSERT_EQUAL_UINT8(6*i+2, buffer[6*i+3]);
        TEST_ASSERT_EQUAL_UINT8(6*i+3, buffer[6*i+2]);
        TEST_ASSERT_EQUAL_UINT8(6*i+4, buffer[6*i+1]);
        TEST_ASSERT_EQUAL_UINT8(6*i+5, buffer[6*i+0]);
    }
}

void test_bswap_suite() {
    RUN_TEST(test_bswap_BrSwap32);
    RUN_TEST(test_bswap_BrSwap16);
    RUN_TEST(test_bswap_BrSwapFloat);
    RUN_TEST(test_bswap_BrSwapBlock_size1);
    RUN_TEST(test_bswap_BrSwapBlock_size2);
    RUN_TEST(test_bswap_BrSwapBlock_size3);
    RUN_TEST(test_bswap_BrSwapBlock_size4);
    RUN_TEST(test_bswap_BrSwapBlock_size5);
    RUN_TEST(test_bswap_BrSwapBlock_size6);
}
