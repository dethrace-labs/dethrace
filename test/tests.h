#ifndef TESTS_H
#define TESTS_H

#include "framework/unity.h"

extern int has_data_directory();

#define REQUIRES_DATA_DIRECTORY() \
    if (!has_data_directory())    \
        TEST_IGNORE();

#endif