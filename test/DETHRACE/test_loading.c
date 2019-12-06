#include "framework/unity.h"

#include <string.h>
#include <unistd.h>

#include "common/globvars.h"
#include "common/loading.h"


void test_loading_GetCDPathFromPathsTxtFile() {
    int result;
    tPath_name cd_path;

    unlink("/tmp/PATHS.TXT");

    // should return false, path does not exist
    result = GetCDPathFromPathsTxtFile(cd_path);
    TEST_ASSERT_EQUAL_INT(0, result);

    FILE *file = fopen("/tmp/PATHS.TXT","wt");
    fprintf(file,"test_cd_path\r\n");
    fclose(file);

    result = GetCDPathFromPathsTxtFile(cd_path);
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_STRING("test_cd_path", cd_path);
}

void test_loading_suite() {
    RUN_TEST(test_loading_GetCDPathFromPathsTxtFile);
}
