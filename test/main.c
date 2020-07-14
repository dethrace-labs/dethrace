#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define UNITY_USE_COMMAND_LINE_ARGS 1

#include "framework/unity.h"

#include "CORE/V1DB/dbsetup.h"
#include "common/utility.h"

#include "common/globvars.h"
#include "harness.h"
#include "renderers/null_renderer.h"

#define debug(format_, ...) fprintf(stderr, format_, __VA_ARGS__)

#define debug(format_, ...) fprintf(stderr, format_, __VA_ARGS__)

extern void test_utility_suite();
extern void test_loading_suite();
extern void test_controls_suite();
extern void test_input_suite();
extern void test_errors_suite();
extern void test_dossys_suite();
extern void test_init_suite();
extern void test_brlists_suite();
extern void test_fwsetup_suite();
extern void test_resource_suite();
extern void test_actsupt_suite();
extern void test_genclip_suite();
extern void test_datafile_suite();
extern void test_v1dbfile_suite();
extern void test_register_suite();
extern void test_pattern_suite();
extern void test_pmfile_suite();
extern void test_graphics_suite();
extern void test_regsupt_suite();

char* root_dir;

void setUp(void) {
    gEncryption_method = 0;
}

void tearDown(void) {
}

void setup_global_vars() {
    strcpy(gDir_separator, "/");

    root_dir = getenv("DETHRACE_ROOT_DIR");
    if (root_dir != NULL) {
        printf("DETHRACE_ROOT_DIR: %s\n", root_dir);
        chdir(root_dir);
        strncpy(gApplication_path, root_dir, 256);
        strcat(gApplication_path, "/DATA");
    } else {
        printf("WARN: DETHRACE_ROOT_DIR is not defined. Skipping tests which require it\n");
        strcpy(gApplication_path, ".");
    }
}

int has_data_directory() {
    return root_dir != NULL;
}

int main(int argc, char** argv) {

    UNITY_BEGIN();

    if (UnityParseOptions(argc, argv) != 0) {
        exit(1);
    }

    Harness_Init("tests", &NullRenderer);

    setup_global_vars();

    BrV1dbBeginWrapper_Float();

    printf("Completed setup\n");

    // BRSRC13
    test_brlists_suite();
    test_fwsetup_suite();
    test_resource_suite();
    test_actsupt_suite();
    test_genclip_suite();
    test_register_suite();
    test_datafile_suite();

    test_pattern_suite();
    test_pmfile_suite();
    test_v1dbfile_suite();
    test_regsupt_suite();

    // DETHRACE
    test_utility_suite();
    test_init_suite();
    test_loading_suite();
    test_controls_suite();
    test_input_suite();
    test_errors_suite();
    test_dossys_suite();
    test_graphics_suite();

    return UNITY_END();
}
