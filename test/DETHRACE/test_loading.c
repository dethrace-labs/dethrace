#include "tests.h"

#include <string.h>

#include "brender.h"
#include "common/globvars.h"
#include "common/graphics.h"
#include "common/init.h"
#include "common/loading.h"
#include "common/newgame.h"
#include "common/utility.h"
#include "common/world.h"
#include "formats.h" // required for v11model

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
    TEST_ASSERT_EQUAL_STRING("NEWEAGLE.TXT", gBasic_car_names[0]);
    TEST_ASSERT_EQUAL_FLOAT(0.2f, gDefault_default_water_spec_vol.gravity_multiplier);
    TEST_ASSERT_EQUAL_FLOAT(50.0f, gDefault_default_water_spec_vol.viscosity_multiplier);

    TEST_ASSERT_EQUAL_INT(0, gInitial_net_credits[0]);
    TEST_ASSERT_EQUAL_INT(2000, gInitial_net_credits[1]);
    TEST_ASSERT_EQUAL_INT(1, gDefault_gravity);
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
    TEST_ASSERT_EQUAL_STRING("Sinthea", o->name);
    TEST_ASSERT_EQUAL_STRING("Sinthea", o->abbrev_name);
    TEST_ASSERT_EQUAL_INT(73, o->car_number);
    TEST_ASSERT_EQUAL_INT(4, o->strength_rating);
    TEST_ASSERT_EQUAL_INT(eNet_avail_never, o->network_availability);
    TEST_ASSERT_EQUAL_STRING("SINTHMUG.FLI", o->mug_shot_name);
    TEST_ASSERT_EQUAL_STRING("SLED.TXT", o->car_file_name);
    TEST_ASSERT_EQUAL_STRING("SLED.FLI", o->stolen_car_flic_name);
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

    pm = DRPixelmapLoad("DATA/32X20X8/PIXELMAP/FRANKF.PIX");
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
    REQUIRES_DATA_DIRECTORY();
    tCar_spec car_spec;
    tBrender_storage storage;
    InitialiseStorageSpace(&storage, 50, 50, 50, 50);
    LoadCar("NEWEAGLE.TXT", eDriver_local_human, &car_spec, eFrankie, "playerName", &storage);
    TEST_ASSERT_TRUE(car_spec.active);
    TEST_ASSERT_FALSE(car_spec.disabled);

    br_model* model = car_spec.car_model_actors[car_spec.principal_car_actor].actor->model;
    TEST_ASSERT_EQUAL_STRING("NEWEAGLE.DAT", model->identifier);

    TEST_ASSERT_EQUAL_FLOAT(1.0, car_spec.M);

    float e_cmpos[] = { 0, 0.345, 0 };
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(e_cmpos, car_spec.cmpos.v, 3);

    TEST_ASSERT_EQUAL_INT(2, car_spec.number_of_steerable_wheels);
    TEST_ASSERT_EQUAL_INT(7, car_spec.steering_ref[0]);
    TEST_ASSERT_EQUAL_INT(8, car_spec.steering_ref[1]);

    TEST_ASSERT_EQUAL_INT(-1, car_spec.driven_wheels_spin_ref_1);
    TEST_ASSERT_EQUAL_INT(-1, car_spec.driven_wheels_spin_ref_2);
    TEST_ASSERT_EQUAL_INT(2, car_spec.driven_wheels_spin_ref_3);
    TEST_ASSERT_EQUAL_INT(1, car_spec.driven_wheels_spin_ref_4);

    TEST_ASSERT_EQUAL_FLOAT(36.231884, car_spec.sk[0]);
    TEST_ASSERT_EQUAL_FLOAT(36.231884, car_spec.sk[1]);
    TEST_ASSERT_EQUAL_FLOAT(4.2562828, car_spec.sb[0]);
    TEST_ASSERT_EQUAL_FLOAT(4.2562828, car_spec.sb[1]);
    TEST_ASSERT_EQUAL_FLOAT(0.41400003, car_spec.ride_height);

    TEST_ASSERT_EQUAL_FLOAT(2.1244245, car_spec.mu[0]);
    TEST_ASSERT_EQUAL_FLOAT(2.2325289, car_spec.mu[1]);
    TEST_ASSERT_EQUAL_FLOAT(2.3518364, car_spec.mu[2]);

    TEST_ASSERT_NOT_NULL(car_spec.car_model_actors[0].actor->model);

    // Test prepared mesh creation
    TEST_ASSERT_EQUAL_INT(17, V11MODEL(model)->ngroups);
    int found_test_prep_group = 0;
    for (int i = 0; i < V11MODEL(model)->ngroups; i++) {
        if (V11MODEL(model)->groups[i].nfaces == 47) {
            found_test_prep_group = 1;
            TEST_ASSERT_EQUAL_INT(99, V11MODEL(model)->groups[i].nvertices);
            TEST_ASSERT_EQUAL_FLOAT(-0.0844900012, V11MODEL(model)->groups[i].position[0].v[0]);
            TEST_ASSERT_EQUAL_FLOAT(0.0440330058, V11MODEL(model)->groups[i].position[0].v[1]);
            TEST_ASSERT_EQUAL_FLOAT(0.1230980009, V11MODEL(model)->groups[i].position[0].v[2]);

            TEST_ASSERT_EQUAL_FLOAT(1.0, V11MODEL(model)->groups[i].map[0].v[0]);
            TEST_ASSERT_EQUAL_FLOAT(0.664765000, V11MODEL(model)->groups[i].map[0].v[1]);

            TEST_ASSERT_EQUAL_FLOAT(-0.0071384655, V11MODEL(model)->groups[i].normal[0].v[0]);
            TEST_ASSERT_EQUAL_FLOAT(0.9597771764, V11MODEL(model)->groups[i].normal[0].v[1]);
            TEST_ASSERT_EQUAL_FLOAT(-0.2806721628, V11MODEL(model)->groups[i].normal[0].v[2]);
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

void test_loading_LoadOpponentCar() {
    REQUIRES_DATA_DIRECTORY();
    tCar_spec car_spec;
    tBrender_storage storage;
    InitialiseStorageSpace(&storage, 50, 50, 50, 50);
    for (int i = 0; i < 10; i++) {
        LOG_DEBUG("iteration %d", i);
        LoadCar("NEWEAGLE.TXT", eDriver_oppo, &car_spec, eFrankie, "playerName", &storage);
        ClearOutStorageSpace(&storage);
    }
}

void test_loading_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_loading_GetCDPathFromPathsTxtFile);
    RUN_TEST(test_loading_OldDRfopen);
    RUN_TEST(test_loading_LoadGeneralParameters);
    RUN_TEST(test_loading_brfont);
    RUN_TEST(test_loading_opponents);
    RUN_TEST(test_loading_memread);
    RUN_TEST(test_loading_ConvertPixToStripMap);
    RUN_TEST(test_loading_LoadCar);
    RUN_TEST(test_loading_LoadOpponentCar);
}
