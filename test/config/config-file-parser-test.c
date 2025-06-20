#include "string.h"
#include "unity.h"
#include "config-file-parser-test.h"
#include "_config-file-parser-private.h"

void test_is_matching_line_match_exact() {
    const char* line = "test_line";
    const int line_len = strlen(line);

    int ret = 55;
    ret = is_matching_line(line, line_len, "test_line");
    TEST_ASSERT_EQUAL_INT(1, ret);
}

void test_is_matching_line_match_equation() {
    const char* line = "test_line=12345";
    const int line_len = 10;

    int ret = 55;

    ret = 55;
    ret = is_matching_line(line, line_len, "test_line=");
    TEST_ASSERT_EQUAL_INT(1, ret);

    const char* line2 = "test_line=valamimasargumentum";
    const int line2_len = 10;

    ret = 55;
    ret = is_matching_line(line2, line2_len, "test_line=");
    TEST_ASSERT_EQUAL_INT(1, ret);
}

void test_is_matching_line_no_match_bad_char() {
    const char* line = "test_line=12345";
    const int line_len = 10;

    int ret = 55;
    ret = is_matching_line(line, line_len, "test_bad_line=");
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_is_matching_line_no_match_bad_len() {
    const char* line = "test_line=12345";
    const int line_len = 10;

    int ret = 55;
    ret = is_matching_line(line, line_len, "ke");
    TEST_ASSERT_EQUAL_INT(0, ret);
}
