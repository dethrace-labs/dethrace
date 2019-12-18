#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "framework/unity.h"

#include "CORE/V1DB/dbsetup.h"

#include "common/globvars.h"

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
    strcpy(gApplication_path, "/tmp");
    strcpy(gDir_separator, "/");

    BrV1dbBeginWrapper_Float();
}

void tearDown(void) {
}

int main(int pArgc, char** pArgv) {

    debug("hi %d\n", 3);

    UNITY_BEGIN();

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
