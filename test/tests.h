#ifndef TESTS_H
#define TESTS_H

#include "framework/unity.h"
#include "harness/trace.h"

extern int has_data_directory();
extern void sleep_s(int sec);

#define REQUIRES_DATA_DIRECTORY() \
    if (!has_data_directory())    \
        TEST_IGNORE();

#endif
