#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define UNITY_USE_COMMAND_LINE_ARGS 1

#include "framework/unity.h"

#include "CORE/V1DB/dbsetup.h"

#include "common/globvars.h"
#include "new/stack_trace_handler.h"

#define debug(format_, ...) fprintf(stderr, format_, __VA_ARGS__)

#define debug(format_, ...) fprintf(stderr, format_, __VA_ARGS__)

extern void test_utility_suite();
extern void test_loading_suite();
extern void test_controls_suite();
extern void test_input_suite();
extern void test_errors_suite();
extern void test_dossys_suite();

extern void test_brlists_suite();
extern void test_fwsetup_suite();
extern void test_resource_suite();

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

    // BRSRC13
    test_brlists_suite();
    test_fwsetup_suite();
    test_resource_suite();

    // DETHRACE
    test_utility_suite();
    test_loading_suite();
    test_controls_suite();
    test_input_suite();
    test_errors_suite();
    test_dossys_suite();

    return UNITY_END();
}
