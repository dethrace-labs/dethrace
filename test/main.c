#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "framework/unity.h"

void setUp(void) {

}

void tearDown(void) {

}

int main(int pArgc, char **pArgv) {
    UNITY_BEGIN();
    test_utility_suite();
    test_controls_suite();
    test_input_suite();
    test_errors_suite();
    return UNITY_END();
}
