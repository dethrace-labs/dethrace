#include "tests.h"

#include <string.h>
#include <unistd.h>

#include "common/globvars.h"
#include "common/init.h"
#include "common/loading.h"
#include "common/newgame.h"

void test_loading_GetCDPathFromPathsTxtFile() {
    REQUIRES_DATA_DIRECTORY();
    int result;
    tPath_name cd_path;

    result = GetCDPathFromPathsTxtFile(cd_path);
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_STRING(".\\DATA\\MINICD", cd_path);
}

void test_loading_OldDRfopen() {
    REQUIRES_DATA_DIRECTORY();
    FILE* f;

    f = OldDRfopen("DATA/i-dont-exist", "rt");
    TEST_ASSERT_NULL(f);

    f = OldDRfopen("DATA/GENERAL.TXT", "rt");
    TEST_ASSERT_NOT_NULL(f);
}

void test_loading_LoadGeneralParameters() {
    REQUIRES_DATA_DIRECTORY();

    LoadGeneralParameters();
    TEST_ASSERT_EQUAL_FLOAT(0.02f, gCamera_hither);
    TEST_ASSERT_EQUAL_INT(7500, gInitial_credits[0]);
    TEST_ASSERT_EQUAL_INT(5000, gInitial_credits[1]);
    TEST_ASSERT_EQUAL_INT(3000, gInitial_credits[2]);
    TEST_ASSERT_EQUAL_STRING("BLKEAGLE.TXT", gBasic_car_names[0]);
    TEST_ASSERT_EQUAL_FLOAT(0.2f, gDefault_default_water_spec_vol.gravity_multiplier);
    TEST_ASSERT_EQUAL_FLOAT(50.0f, gDefault_default_water_spec_vol.viscosity_multiplier);

    TEST_ASSERT_EQUAL_INT(0, gInitial_net_credits[0]);
    TEST_ASSERT_EQUAL_INT(2000, gInitial_net_credits[1]);
    TEST_ASSERT_EQUAL_INT(1, gGravity_multiplier);
}

void test_loading_brfont() {
    REQUIRES_DATA_DIRECTORY();
    br_font* font;

    font = LoadBRFont("FONT7.FNT");
    TEST_ASSERT_NOT_NULL(font);
    TEST_ASSERT_EQUAL_INT(1, font->flags);
    TEST_ASSERT_EQUAL_INT(7, font->glyph_y); // font height
    TEST_ASSERT_EQUAL_INT(0, font->width[0]);
    TEST_ASSERT_EQUAL_INT(3, font->width[32]);
}

void test_loading_opponents() {
    REQUIRES_DATA_DIRECTORY();
    tOpponent* o;

    LoadOpponents();
    TEST_ASSERT_EQUAL_INT(40, gNumber_of_racers);
    o = &gOpponents[2];
    TEST_ASSERT_EQUAL_STRING("Agent Orange", o->name);
    TEST_ASSERT_EQUAL_STRING("Orange", o->abbrev_name);
    TEST_ASSERT_EQUAL_INT(12, o->car_number);
    TEST_ASSERT_EQUAL_INT(4, o->strength_rating);
    TEST_ASSERT_EQUAL_INT(eNet_avail_never, o->network_availability);
    TEST_ASSERT_EQUAL_STRING("LEADPUMP.FLI", o->mug_shot_name);
    TEST_ASSERT_EQUAL_STRING("AGENTO.TXT", o->car_file_name);
    TEST_ASSERT_EQUAL_STRING("AGENTO.FLI", o->stolen_car_flic_name);
    TEST_ASSERT_EQUAL_INT(2, o->text_chunk_count);
    TEST_ASSERT_EQUAL_INT(5, o->text_chunks[0].x_coord);
    TEST_ASSERT_EQUAL_INT(3, o->text_chunks[0].y_coord);
    TEST_ASSERT_EQUAL_INT(3, o->text_chunks[0].line_count);
    TEST_ASSERT_EQUAL_STRING("TOP SPEED: 150MPH", o->text_chunks[0].text[0]);
}

void test_loading_powerups() {
    REQUIRES_DATA_DIRECTORY();

    LoadPowerups();
}

void test_loading_suite() {
    RUN_TEST(test_loading_GetCDPathFromPathsTxtFile);
    RUN_TEST(test_loading_OldDRfopen);
    RUN_TEST(test_loading_LoadGeneralParameters);
    RUN_TEST(test_loading_brfont);
    RUN_TEST(test_loading_opponents);
}
