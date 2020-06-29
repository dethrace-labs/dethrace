#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define UNITY_USE_COMMAND_LINE_ARGS 1

#include "framework/unity.h"

#include "CORE/V1DB/dbsetup.h"

#include "common/globvars.h"
#include "stack_trace_handler.h"

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

void setUp(void) {
}

void tearDown(void) {
}

void setupGlobalVars() {
    strcpy(gDir_separator, "/");
    getcwd(gApplication_path, 256);
    strcat(gApplication_path, "/DATA");
}

int main(int argc, char** argv) {

    UNITY_BEGIN();

    if (UnityParseOptions(argc, argv) != 0) {
        exit(1);
    }

    set_signal_handler(argv[0]);

    setupGlobalVars();

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

    // DETHRACE
    test_utility_suite();
    test_init_suite();
    test_loading_suite();
    test_controls_suite();
    test_input_suite();
    test_errors_suite();
    test_dossys_suite();

    return UNITY_END();
}
