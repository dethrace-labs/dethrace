#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tests.h"

#include "common/controls.h"
#include "common/errors.h"
#include "common/globvars.h"
#include "common/input.h"
#include "common/loading.h"
#include "common/utility.h"

void test_controls_CheckKevKeys() {
    int i;
    char* input = "spamfritters";
    tU32* result;
    for (i = 0; i < strlen(input); i++) {
        gKeys_pressed = input[i] - 75; // 0x1e;
        result = KevKeyService();
        gKeys_pressed = 0;
        result = KevKeyService();
    }
    sleep(1);
    gKeys_pressed = 0;

    CheckKevKeys();

    // 'spamfritters' cheat code should enable powerup #8
    TEST_ASSERT_EQUAL_INT(8, _unittest_controls_lastGetPowerup);
}

void test_controls_suite() {
    RUN_TEST(test_controls_CheckKevKeys);
}
