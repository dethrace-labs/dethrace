#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "framework/unity.h"

#include "common/globvars.h"

extern void test_utility_suite();
extern void test_loading_suite();
extern void test_controls_suite();
extern void test_input_suite();
extern void test_errors_suite();

void setUp(void) {
    strcpy(gApplication_path, "/tmp");
    strcpy(gDir_separator, "/");
}

void tearDown(void) {

}

int main(int pArgc, char **pArgv) {
    UNITY_BEGIN();
    test_utility_suite();
    test_loading_suite();
    test_controls_suite();
    test_input_suite();
    test_errors_suite();
    return UNITY_END();
}
