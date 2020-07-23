#include "tests.h"

#include "common/input.h"
#include "common/globvars.h"
#include <string.h>
#include <unistd.h>

void test_input_KevKeyService() {
    int i;
    char* input = "iwanttofiddle";
    tU32* result;
    for (i = 0; i < strlen(input); i++) {
        gKeys_pressed = input[i] - 75;
        result = KevKeyService();
        gKeys_pressed = 0;
        result = KevKeyService();
    }
    sleep(1);
    gKeys_pressed = 0;
    result = KevKeyService();

    TEST_ASSERT_EQUAL_UINT32(0x33f75455, result[0]);
    TEST_ASSERT_EQUAL_UINT32(0xc10aaaf2, result[1]);
}

void test_input_suite() {
    RUN_TEST(test_input_KevKeyService);
}
