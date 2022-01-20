#include "CORE/PIXELMAP/genclip.h"
#include "tests.h"
#include <stddef.h>

void test_genclip_PixelmapRectangleClip() {
    br_rectangle out;
    br_rectangle in;
    br_pixelmap pm;
    br_clip_result result;

    pm.origin_x = 0;
    pm.origin_y = 0;
    pm.height = 10;
    pm.width = 20;

    in.x = 100; // out of bounds
    in.w = 5;
    in.h = 5;
    result = PixelmapRectangleClip(&out, &in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_REJECT, result);

    in.x = 0;
    in.y = 21; // out of bounds
    result = PixelmapRectangleClip(&out, &in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_REJECT, result);

    in.x = 1;
    in.y = 1;
    result = PixelmapRectangleClip(&out, &in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_PARTIAL, result);

    in.w = 100;
    in.h = 100;
    result = PixelmapRectangleClip(&out, &in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_PARTIAL, result);
    TEST_ASSERT_EQUAL(19, out.w);
    TEST_ASSERT_EQUAL(9, out.h);
    TEST_ASSERT_EQUAL(1, out.x);
    TEST_ASSERT_EQUAL(1, out.y);
}

void test_genclip_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_genclip_PixelmapRectangleClip);
}
