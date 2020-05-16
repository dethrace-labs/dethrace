#include "framework/unity.h"

#include "CORE/V1DB/modsupt.h"
#include "CORE/V1DB/v1dbfile.h"

void test_v1dbfile_BrModelLoad() {
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

void test_v1dbfile_suite() {
    RUN_TEST(test_v1dbfile_BrModelLoad);
}