#include "CORE/PIXELMAP/genclip.h"
#include "tests.h"
#include <stddef.h>

static void test_genclip_PixelmapPointClip() {
    br_point in;
    br_point out;
    br_pixelmap pm;
    br_clip_result result;

    // Pixelmap at (10, 20) with dimension (30, 40)
    pm.origin_x = -10;
    pm.origin_y = -20;
    pm.width = 30;
    pm.height = 40;

    // Pixel (5, 40) is out (too left)
    in.x = 5;
    in.y = 40;
    result = PixelmapPointClip(&out, &in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_REJECT, result);

    // Pixel (45, 40) is out (too right)
    in.x = 45;
    in.y = 40;
    result = PixelmapPointClip(&out, &in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_REJECT, result);

    // Pixel (25, 15) is out (too low)
    in.x = 25;
    in.y = 15;
    result = PixelmapPointClip(&out, &in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_REJECT, result);

    // Pixel (25, 65) is out (too high)
    in.x = 25;
    in.y = 65;
    result = PixelmapPointClip(&out, &in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_REJECT, result);

    // Pixel (25, 40) is in
    in.x = 25;
    in.y = 40;
    result = PixelmapPointClip(&out, &in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_ACCEPT, result);
    TEST_ASSERT_EQUAL_INT(15, out.x);
    TEST_ASSERT_EQUAL_INT(20, out.y);
}

static void test_genclip_PixelmapLineClip() {
    br_point s_out, e_out;
    br_point s_in, e_in;
    br_pixelmap pm;
    br_clip_result result;

    // Pixelmap at (10, 20) with dimension (30, 40)
    pm.origin_x = -10;
    pm.origin_y = -20;
    pm.width = 30;
    pm.height = 40;

    // Line left of pixelmap
    s_in.x = 5;
    s_in.y = 5;
    e_in.x = 5;
    e_in.y = 45;
    result = PixelmapLineClip(&s_out, &e_out, &s_in, &e_in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_REJECT, result);

    // Line right of pixelmap
    s_in.x = 40;
    s_in.y = 5;
    e_in.x = 40;
    e_in.y = 65;
    result = PixelmapLineClip(&s_out, &e_out, &s_in, &e_in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_REJECT, result);

    // Line below pixelmap
    s_in.x = 5;
    s_in.y = 5;
    e_in.x = 40;
    e_in.y = 5;
    result = PixelmapLineClip(&s_out, &e_out, &s_in, &e_in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_REJECT, result);

    // Line above pixelmap
    s_in.x = 5;
    s_in.y = 65;
    e_in.x = 40;
    e_in.y = 65;
    result = PixelmapLineClip(&s_out, &e_out, &s_in, &e_in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_REJECT, result);

    // Line crosses pixelmap once (on left side)
    s_in.x = 0;
    s_in.y = 20;
    e_in.x = 20;
    e_in.y = 40;
    result = PixelmapLineClip(&s_out, &e_out, &s_in, &e_in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_PARTIAL, result);
    TEST_ASSERT_EQUAL(0, s_out.x);
    TEST_ASSERT_EQUAL(10, s_out.y);
    TEST_ASSERT_EQUAL(10, e_out.x);
    TEST_ASSERT_EQUAL(20, e_out.y);

    // Line crosses pixelmap once (on right side)
    s_in.x = 50;
    s_in.y = 20;
    e_in.x = 30;
    e_in.y = 40;
    result = PixelmapLineClip(&s_out, &e_out, &s_in, &e_in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_PARTIAL, result);
    TEST_ASSERT_EQUAL(20, s_out.x);
    TEST_ASSERT_EQUAL(20, s_out.y);
    TEST_ASSERT_EQUAL(29, e_out.x);
    TEST_ASSERT_EQUAL(11, e_out.y);

    // Line crosses pixelmap once (on bottom side)
    s_in.x = 10;
    s_in.y = 10;
    e_in.x = 30;
    e_in.y = 30;
    result = PixelmapLineClip(&s_out, &e_out, &s_in, &e_in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_PARTIAL, result);
    TEST_ASSERT_EQUAL(10, s_out.x);
    TEST_ASSERT_EQUAL(0, s_out.y);
    TEST_ASSERT_EQUAL(20, e_out.x);
    TEST_ASSERT_EQUAL(10, e_out.y);

    // Line crosses pixelmap once (on top side)
    s_in.x = 30;
    s_in.y = 50;
    e_in.x = 10;
    e_in.y = 70;
    result = PixelmapLineClip(&s_out, &e_out, &s_in, &e_in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_PARTIAL, result);
    TEST_ASSERT_EQUAL(11, s_out.x);
    TEST_ASSERT_EQUAL(39, s_out.y);
    TEST_ASSERT_EQUAL(20, e_out.x);
    TEST_ASSERT_EQUAL(30, e_out.y);

    // Line crosses pixelmap twice (left and right)
    s_in.x = 0;
    s_in.y = 30;
    e_in.x = 50;
    e_in.y = 55;
    result = PixelmapLineClip(&s_out, &e_out, &s_in, &e_in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_PARTIAL, result);
    TEST_ASSERT_EQUAL(0, s_out.x);
    TEST_ASSERT_EQUAL(15, s_out.y);
    TEST_ASSERT_EQUAL(29, e_out.x);
    TEST_ASSERT_EQUAL(30, e_out.y);

    // Line crosses pixelmap twice (top and bottom)
    s_in.x = 20;
    s_in.y = 0;
    e_in.x = 20;
    e_in.y = 70;
    result = PixelmapLineClip(&s_out, &e_out, &s_in, &e_in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_PARTIAL, result);
    TEST_ASSERT_EQUAL(10, s_out.x);
    TEST_ASSERT_EQUAL(0, s_out.y);
    TEST_ASSERT_EQUAL(10, e_out.x);
    TEST_ASSERT_EQUAL(39, e_out.y);
}

static void test_genclip_PixelmapRectangleClip() {
    br_rectangle out;
    br_rectangle in;
    br_pixelmap pm;
    br_clip_result result;

    // Pixelmap at (10, 20) with dimension (30, 40)
    pm.origin_x = -10;
    pm.origin_y = -20;
    pm.width = 30;
    pm.height = 40;

    // rectangle completely left of pm
    in.x = -10;
    in.y = 20;
    in.w = 20;
    in.h = 10;
    result = PixelmapRectangleClip(&out, &in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_REJECT, result);

    // rectangle completely right of pm
    in.x = 45;
    in.y = 20;
    in.w = 20;
    in.h = 10;
    result = PixelmapRectangleClip(&out, &in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_REJECT, result);

    // rectangle completely below pm
    in.x = 25;
    in.y = 0;
    in.w = 20;
    in.h = 10;
    result = PixelmapRectangleClip(&out, &in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_REJECT, result);

    // rectangle completely above pm
    in.x = 25;
    in.y = 65;
    in.w = 20;
    in.h = 10;
    result = PixelmapRectangleClip(&out, &in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_REJECT, result);

    // rectangle clips pm on complete width
    in.x = 5;
    in.y = 30;
    in.w = 50;
    in.h = 10;
    result = PixelmapRectangleClip(&out, &in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_PARTIAL, result);
    TEST_ASSERT_EQUAL_INT(0, out.x);
    TEST_ASSERT_EQUAL_INT(10, out.y);
    TEST_ASSERT_EQUAL_INT(30, out.w);
    TEST_ASSERT_EQUAL_INT(10, out.h);

    // rectangle clips pm on complete height
    in.x = 15;
    in.y = 10;
    in.w = 10;
    in.h = 100;
    result = PixelmapRectangleClip(&out, &in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_PARTIAL, result);
    TEST_ASSERT_EQUAL_INT(5, out.x);
    TEST_ASSERT_EQUAL_INT(0, out.y);
    TEST_ASSERT_EQUAL_INT(10, out.w);
    TEST_ASSERT_EQUAL_INT(40, out.h);

    // rectangle completely overlaps pm
    in.x = 5;
    in.y = 5;
    in.w = 60;
    in.h = 60;
    result = PixelmapRectangleClip(&out, &in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_PARTIAL, result);
    TEST_ASSERT_EQUAL_INT(0, out.x);
    TEST_ASSERT_EQUAL_INT(0, out.y);
    TEST_ASSERT_EQUAL_INT(30, out.w);
    TEST_ASSERT_EQUAL_INT(40, out.h);
}

static void test_genclip_PixelmapRectangleClipTwo() {
    br_rectangle r_out;
    br_point p_out;
    br_rectangle r_in;
    br_point p_in;
    br_pixelmap pm_dst;
    br_pixelmap pm_src;
    br_clip_result result;

    // src pixelmap at (10, 20) with dimension (30, 40)
    pm_src.origin_x = -10;
    pm_src.origin_y = -20;
    pm_src.width = 30;
    pm_src.height = 40;

    // dst pixelmap at (10, 30) with dimension (40, 20)
    pm_dst.origin_x = -10;
    pm_dst.origin_y = -30;
    pm_dst.width = 40;
    pm_dst.height = 20;

    // src rect is out of bounds
    r_in.x = -10;
    r_in.y = 20;
    r_in.w = 20;
    r_in.h = 15;
    p_in.x = 15;
    p_in.y = 35;
    result = PixelmapRectangleClipTwo(&r_out, &p_out, &r_in, &p_in, &pm_dst, &pm_src);
    TEST_ASSERT_EQUAL(BR_CLIP_REJECT, result);

    // src rect is partially in pm_src, dest point is ok
    r_in.x = 20;
    r_in.y = 30;
    r_in.w = 40;
    r_in.h = 40;
    p_in.x = 15;
    p_in.y = 35;
    result = PixelmapRectangleClipTwo(&r_out, &p_out, &r_in, &p_in, &pm_dst, &pm_src);
    TEST_ASSERT_EQUAL(BR_CLIP_PARTIAL, result);
    TEST_ASSERT_EQUAL_INT(10, r_out.x);
    TEST_ASSERT_EQUAL_INT(10, r_out.y);
    TEST_ASSERT_EQUAL_INT(20, r_out.w);
    TEST_ASSERT_EQUAL_INT(15, r_out.h);
    TEST_ASSERT_EQUAL_INT(5, p_out.x);
    TEST_ASSERT_EQUAL_INT(5, p_out.y);
}

static void test_genclip_PixelmapCopyBitsClip() {
    br_rectangle r_out;
    br_point p_out;
    br_rectangle r_in;
    br_point p_in;
    br_pixelmap pm;
    br_clip_result result;

    // Pixelmap at (10, 20) with dimension (30, 40)
    pm.origin_x = -10;
    pm.origin_y = -20;
    pm.width = 30;
    pm.height = 40;

    // point is out of bounds
    r_in.x = 15;
    r_in.y = 25;
    r_in.w = 20;
    r_in.h = 15;
    p_in.x = 40;
    p_in.y = 30;
    result = PixelmapCopyBitsClip(&r_out, &p_out, &r_in, &p_in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_REJECT, result);

    // rect is completely in pm
    r_in.x = 0;
    r_in.y = 20;
    r_in.w = 5;
    r_in.h = 15;
    p_in.x = 15;
    p_in.y = 35;
    result = PixelmapCopyBitsClip(&r_out, &p_out, &r_in, &p_in, &pm);
    TEST_ASSERT_EQUAL(BR_CLIP_PARTIAL, result);
    TEST_ASSERT_EQUAL_INT(0, r_out.x);
    TEST_ASSERT_EQUAL_INT(20, r_out.y);
    TEST_ASSERT_EQUAL_INT(5, r_out.w);
    TEST_ASSERT_EQUAL_INT(15, r_out.h);
    TEST_ASSERT_EQUAL_INT(5, p_out.x);
    TEST_ASSERT_EQUAL_INT(15, p_out.y);
}

void test_genclip_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_genclip_PixelmapPointClip);
    RUN_TEST(test_genclip_PixelmapLineClip);
    RUN_TEST(test_genclip_PixelmapRectangleClip);
    RUN_TEST(test_genclip_PixelmapRectangleClipTwo);
    RUN_TEST(test_genclip_PixelmapCopyBitsClip);
}
