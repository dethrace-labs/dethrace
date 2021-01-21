#include "tests.h"

#include <string.h>
#include <unistd.h>

#include "CORE/PIXELMAP/pixelmap.h"
#include "common/globvars.h"
#include "common/graphics.h"
#include "common/init.h"
#include "common/loading.h"
#include "common/newgame.h"
#include "common/utility.h"
#include "common/world.h"

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

void test_loading_memread() {
    int32_t int_array[] = { 1, 2, 3, 4 };
    char* data = (char*)int_array;
    TEST_ASSERT_EQUAL_INT(1, MemReadU32(&data));
    TEST_ASSERT_EQUAL_INT(2, MemReadU32(&data));
    MemSkipBytes(&data, sizeof(int32_t));
    TEST_ASSERT_EQUAL_INT(4, MemReadU32(&data));

    int16_t short_array[] = { 1, 2, 3, 4, -5, -6 };
    data = (char*)short_array;
    TEST_ASSERT_EQUAL_INT(1, MemReadU16(&data));
    TEST_ASSERT_EQUAL_INT(2, MemReadU16(&data));
    MemSkipBytes(&data, sizeof(int16_t));
    TEST_ASSERT_EQUAL_INT(4, MemReadU16(&data));
    TEST_ASSERT_EQUAL_INT(-5, MemReadS16(&data));
    TEST_ASSERT_EQUAL_INT(-6, MemReadS16(&data));

    int8_t char_array[] = { 1, 2, 3, -4 };
    data = (char*)char_array;
    TEST_ASSERT_EQUAL_INT(1, MemReadU8(&data));
    TEST_ASSERT_EQUAL_INT(2, MemReadU8(&data));
    MemSkipBytes(&data, sizeof(int8_t));
    TEST_ASSERT_EQUAL_INT(252, MemReadU8(&data)); // (unsigned)

    data = (char*)char_array;
    TEST_ASSERT_EQUAL_INT(1, MemReadS8(&data));
    TEST_ASSERT_EQUAL_INT(2, MemReadS8(&data));
    MemSkipBytes(&data, sizeof(int8_t));
    TEST_ASSERT_EQUAL_INT(-4, MemReadS8(&data));
}

void test_loading_ConvertPixToStripMap() {
    REQUIRES_DATA_DIRECTORY();
    br_pixelmap* pm;
    tS8* strip_data;
    br_pixelmap* pm_unstripped;
    int x;
    int y;
    br_uint_8* src_pix;
    br_uint_8* dst_pix;
    pm = DRPixelmapLoad("DATA/32x20x8/PIXELMAP/FRANKF.PIX");
    TEST_ASSERT_NOT_NULL(pm);
    strip_data = ConvertPixToStripMap(pm);
    TEST_ASSERT_NOT_NULL(strip_data);
    pm_unstripped = BrPixelmapAllocate(BR_MEMORY_PIXELS, pm->width, pm->height, NULL, BR_PMAF_NORMAL);
    CopyStripImage(pm_unstripped, 0, 0, 0, 0, strip_data, 0, 0, pm->width, pm->height);
    src_pix = (br_uint_8*)pm->pixels;
    dst_pix = (br_uint_8*)pm_unstripped->pixels;
    for (y = 0; y < pm->height; y++) {
        for (x = 0; x < pm->width; x++) {
            if (src_pix[y * pm->row_bytes + x] != dst_pix[y * pm_unstripped->row_bytes + x]) {
                printf("Original and unstripped images do not match at (%d %d)\n", x, y);
                TEST_FAIL();
            }
        }
    }
}

void test_loading_LoadCar() {
    tCar_spec car_spec;
    tBrender_storage storage;
    InitialiseStorageSpace(&storage, 50, 50, 50, 50);
    LoadCar("NEWEAGLE.TXT", eDriver_local_human, &car_spec, eFrankie, "playerName", &storage);
    TEST_ASSERT_TRUE(car_spec.active);
    TEST_ASSERT_FALSE(car_spec.disabled);
    //TEST_ASSERT_EQUAL_FLOAT(0.06f, car_spec.ride_height);

    br_model* model = car_spec.car_model_actors[car_spec.principal_car_actor].actor->model;
    TEST_ASSERT_EQUAL_STRING("NEWEAGLE.DAT", model->identifier);

    // Test prepared mesh creation
    TEST_ASSERT_EQUAL_INT(17, V11MODEL(model)->ngroups);
    int found_test_prep_group = 0;
    for (int i = 0; i < V11MODEL(model)->ngroups; i++) {
        if (V11MODEL(model)->groups[i].nfaces == 47) {
            found_test_prep_group = 1;
            TEST_ASSERT_EQUAL_INT(99, V11MODEL(model)->groups[i].nvertices);
            TEST_ASSERT_EQUAL_FLOAT(-0.0844900012, V11MODEL(model)->groups[i].vertices[0].p.v[0]);
            TEST_ASSERT_EQUAL_FLOAT(0.0440330058, V11MODEL(model)->groups[i].vertices[0].p.v[1]);
            TEST_ASSERT_EQUAL_FLOAT(0.1230980009, V11MODEL(model)->groups[i].vertices[0].p.v[2]);

            TEST_ASSERT_EQUAL_FLOAT(1.0, V11MODEL(model)->groups[i].vertices[0].map.v[0]);
            TEST_ASSERT_EQUAL_FLOAT(0.664765000, V11MODEL(model)->groups[i].vertices[0].map.v[1]);

            TEST_ASSERT_EQUAL_FLOAT(-0.0071384655, V11MODEL(model)->groups[i].vertices[0].n.v[0]);
            TEST_ASSERT_EQUAL_FLOAT(0.9597771764, V11MODEL(model)->groups[i].vertices[0].n.v[1]);
            TEST_ASSERT_EQUAL_FLOAT(-0.2806721628, V11MODEL(model)->groups[i].vertices[0].n.v[2]);
            break;
        }
    }
    if (!found_test_prep_group) {
        TEST_FAIL_MESSAGE("expected prep group not found");
    }

    // for (int i = 0; i < V11MODEL(model)->ngroups; i++) {
    //     printf("group %d, nfaces=%d, verts=%d\n", i, V11MODEL(model)->groups[i].nfaces, V11MODEL(model)->groups[i].nvertices);
    //     for (int j = 0; j < V11MODEL(model)->groups[i].nvertices; j++) {
    //         printf("(%.10f %.10f %.10f), (%.9f %.9f), (%.10f %.10f %.10f)\n",
    //             V11MODEL(model)->groups[i].vertices[j].p.v[j], V11MODEL(model)->groups[i].vertices[j].p.v[1], V11MODEL(model)->groups[i].vertices[j].p.v[2],
    //             V11MODEL(model)->groups[i].vertices[j].map.v[j], V11MODEL(model)->groups[i].vertices[j].map.v[1],
    //             V11MODEL(model)->groups[i].vertices[j].n.v[j], V11MODEL(model)->groups[i].vertices[j].n.v[1], V11MODEL(model)->groups[i].vertices[j].n.v[2]);
    //     }
    // }
    // for (int j = 0; j < V11MODEL(model)->groups[i].nfaces; j++) {
    //     printf("(%d %d %d)\n", V11MODEL(model)->groups[i].faces[j].vertices[0], V11MODEL(model)->groups[i].faces[j].vertices[1], V11MODEL(model)->groups[i].faces[j].vertices[2]);
    // }
}

void test_loading_suite() {
    RUN_TEST(test_loading_GetCDPathFromPathsTxtFile);
    RUN_TEST(test_loading_OldDRfopen);
    RUN_TEST(test_loading_LoadGeneralParameters);
    RUN_TEST(test_loading_brfont);
    RUN_TEST(test_loading_opponents);
    RUN_TEST(test_loading_memread);
    RUN_TEST(test_loading_ConvertPixToStripMap);
    RUN_TEST(test_loading_LoadCar);
}
