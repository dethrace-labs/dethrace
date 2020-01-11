#include "framework/unity.h"

#include <string.h>
#include <unistd.h>

#include "common/globvars.h"
#include "pc-dos/dossys.h"

void test_dossys_PDInitialiseSystem() {

    FILE* file = fopen("/tmp/KEYBOARD.COK", "wt");
    fprintf(file, "abcdef");
    fclose(file);

    PDInitialiseSystem();
    TEST_ASSERT_EQUAL_STRING("abc", gASCII_table);
    TEST_ASSERT_EQUAL_STRING("def", gASCII_shift_table);
}

void test_dossys_suite() {
    RUN_TEST(test_dossys_PDInitialiseSystem);
}
