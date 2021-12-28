#include "tests.h"

#include "CORE/FW/pattern.h"

void test_pattern_BrNamePatternMatch() {
    br_boolean result;

    result = BrNamePatternMatch("hello", "Hello");
    TEST_ASSERT_EQUAL_INT(1, result);

    result = BrNamePatternMatch("hello", "world");
    TEST_ASSERT_EQUAL_INT(0, result);
}

// Star should match any number of characters
void test_pattern_BrNamePatternMatch_Star() {
    br_boolean result;
    result = BrNamePatternMatch("he*", "hello");
    TEST_ASSERT_EQUAL_INT(1, result);

    result = BrNamePatternMatch("he*o", "hello");
    TEST_ASSERT_EQUAL_INT(1, result);

    result = BrNamePatternMatch("he*z", "hello");
    TEST_ASSERT_EQUAL_INT(0, result);
}

// Question mark should match any single character
void test_pattern_BrNamePatternMatch_QuestionMark() {
    br_boolean result;
    result = BrNamePatternMatch("he?lo", "hello");
    TEST_ASSERT_EQUAL_INT(1, result);
    result = BrNamePatternMatch("he?o", "hello");
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_pattern_suite() {
    UnitySetTestFile(__FILE__);
    RUN_TEST(test_pattern_BrNamePatternMatch);
    RUN_TEST(test_pattern_BrNamePatternMatch_Star);
    RUN_TEST(test_pattern_BrNamePatternMatch_QuestionMark);
}
