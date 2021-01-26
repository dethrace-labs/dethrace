#include "tests.h"

#include "CORE/PIXELMAP/pmfile.h"
#include "CORE/V1DB/actsupt.h"
#include "CORE/V1DB/modsupt.h"
#include "CORE/V1DB/prepmesh.h"
#include "CORE/V1DB/regsupt.h"
#include "CORE/V1DB/v1dbfile.h"

void test_v1dbfile_BrModelLoad() {
    REQUIRES_DATA_DIRECTORY();

    br_model* m;
    m = BrModelLoad("DATA/MODELS/CPOINT.DAT");
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL_STRING("DIRECT.DAT", m->identifier);
    TEST_ASSERT_NOT_NULL(m->vertices);
    TEST_ASSERT_EQUAL_INT(54, m->nvertices);
    TEST_ASSERT_EQUAL_FLOAT(-0.0950154, m->vertices[0].p.v[0]);
    TEST_ASSERT_EQUAL_FLOAT(-0.030865, m->vertices[0].p.v[1]);
    TEST_ASSERT_EQUAL_FLOAT(-0.0157602, m->vertices[0].p.v[2]);
    TEST_ASSERT_EQUAL_INT(44, m->nfaces);
    TEST_ASSERT_NOT_NULL(m->faces);
    TEST_ASSERT_EQUAL_INT(26, m->faces[0].vertices[0]);
    TEST_ASSERT_EQUAL_INT(53, m->faces[0].vertices[1]);
    TEST_ASSERT_EQUAL_INT(43, m->faces[0].vertices[2]);
    BrModelFree(m);
}

void test_v1dbfile_BrActorLoad() {
    REQUIRES_DATA_DIRECTORY();

    br_actor* a;
    a = BrActorLoad("DATA/ACTORS/CPOINT.ACT");
    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_EQUAL_STRING("CPOINT.ACT", a->identifier);
    TEST_ASSERT_EQUAL_INT(1, a->type);
    BrActorFree(a);
}

void test_v1dbfile_BrMaterialLoad() {
    REQUIRES_DATA_DIRECTORY();

    br_material* m;
    br_pixelmap* pm;

    pm = BrPixelmapLoad("DATA/REG/PIXELMAP/OILSMEAR.PIX");
    TEST_ASSERT_NOT_NULL(pm);
    TEST_ASSERT_EQUAL_STRING("oilsmear.pix", pm->identifier);
    BrMapAdd(pm);

    m = BrMaterialLoad("DATA/REG/MATERIAL/OILSMEAR.MAT");
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL_INT(1, m->map_transform.m[0][0]);
    TEST_ASSERT_EQUAL_INT(0, m->map_transform.m[0][1]);
    TEST_ASSERT_EQUAL_INT(0, m->map_transform.m[1][0]);
    TEST_ASSERT_EQUAL_INT(1, m->map_transform.m[1][1]);
    TEST_ASSERT_EQUAL_INT(0, m->map_transform.m[2][0]);
    TEST_ASSERT_EQUAL_INT(0, m->map_transform.m[2][1]);

    TEST_ASSERT_EQUAL_PTR(pm, m->colour_map);
}

void test_v1dbfile_suite() {
    RUN_TEST(test_v1dbfile_BrModelLoad);
    RUN_TEST(test_v1dbfile_BrActorLoad);
    RUN_TEST(test_v1dbfile_BrMaterialLoad);
}