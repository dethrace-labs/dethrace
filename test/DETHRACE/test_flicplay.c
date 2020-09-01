#include "brender.h"
#include "common/flicplay.h"
#include "common/graphics.h"
#include "tests.h"

int nbr_frames_rendered;

void frame_render_callback() {
    nbr_frames_rendered++;
}
void test_flicplay_playflic() {
    int pIndex = 31; // main menu swing in
    br_pixelmap* target;

    gCurrent_palette_pixels = malloc(0x400);
    FlicPaletteAllocate();
    TEST_ASSERT_EQUAL_INT(1, LoadFlic(pIndex));

    target = BrPixelmapAllocate(BR_MEMORY_PIXELS, 320, 200, NULL, 0);
    PlayFlic(
        pIndex,
        gMain_flic_list[pIndex].the_size,
        gMain_flic_list[pIndex].data_ptr,
        target,
        gMain_flic_list[pIndex].x_offset,
        gMain_flic_list[pIndex].y_offset,
        frame_render_callback,
        gMain_flic_list[pIndex].interruptable,
        gMain_flic_list[pIndex].frame_rate);

    TEST_ASSERT_EQUAL_INT(10, nbr_frames_rendered);
}

void test_flicplay_suite() {
    RUN_TEST(test_flicplay_playflic);
}