#include "cmdline-parser-test.h"
#include "_cmdline-parser-private.h"
#include "unity.h"

void test_is_matching_arg_matching() {
    const char* arg = "--version";
    int ret;

    ret = 55;
    ret = is_matching_arg(arg, "--version");
    TEST_ASSERT_EQUAL_INT(1, ret);
}


void test_is_matching_arg_non_matching_shorter() {
    const char* arg = "--v";
    int ret;

    ret = 55;
    ret = is_matching_arg(arg, "--version");
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_is_matching_arg_non_matching_longer() {
    const char* arg = "--version-full";
    int ret;

    ret = 55;
    ret = is_matching_arg(arg, "--version");
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_is_matching_arg_non_matching_bad_char() {
    const char* arg = "--get-version";
    int ret;

    ret = 55;
    ret = is_matching_arg(arg, "--version");
    TEST_ASSERT_EQUAL_INT(0, ret);
}
