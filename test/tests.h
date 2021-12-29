#ifndef TESTS_H
#define TESTS_H

#include "framework/unity.h"
#include "harness/trace.h"

extern int has_data_directory();

#define REQUIRES_DATA_DIRECTORY() \
    if (!has_data_directory())    \
        TEST_IGNORE();

#define TEST_ASSERT_FLOAT_ARRAY_WITHIN(delta, expected, actual, num_elements) {     \
    float *priv_expected = (float*)(expected);                                      \
    float *priv_actual = (float*)(actual);                                          \
    for(int it = (num_elements); it != 0; --it, ++priv_expected, ++priv_actual) {   \
        TEST_ASSERT_FLOAT_WITHIN((delta), *priv_expected, *priv_actual);            \
    }                                                                               \
}

#endif
