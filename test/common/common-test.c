#include "common-test.h"
#include "common.h"
#include "unity.h"
#include <stdint.h>

void test_imin(void) {
    int a = 4;
    int b = 9;
    int res = 0;

    res = imin(a, b);
    TEST_ASSERT_EQUAL(a, res);

    b = -1 * b;

    res = imin(a, b);
    TEST_ASSERT_EQUAL(b, res);
}

void test_imax(void) {
    int a = 4;
    int b = 9;
    int res = 0;

    res = imax(a, b);
    TEST_ASSERT_EQUAL(b, res);

    b = -1 * b;

    res = imax(a, b);
    TEST_ASSERT_EQUAL(a, res);
}

void test_str_to_i32_normal_behavior() {
    int32_t value = 0;
    int ret = 0;

    ret = str_to_i32("45", 2, &value);

    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_INT(45, value);
}

void test_str_to_i32_bad_input() {
    int32_t value = 0;
    int ret = 0;

    ret = str_to_i32("45badinput", 10, &value);

    TEST_ASSERT_EQUAL_INT(-1, ret);
    TEST_ASSERT_EQUAL_INT(0, value);
}

void test_str_to_float_normal_behavior() {
    float value = 0;
    int ret = 0;

    ret = str_to_float("14.46", 5, &value);

    TEST_ASSERT_EQUAL_FLOAT(0, ret);
    TEST_ASSERT_EQUAL_FLOAT(14.46, value);
}

void test_str_to_float_bad_input() {
    float value = 0;
    int ret = 0;

    ret = str_to_float("14.46badvalue", 13, &value);

    TEST_ASSERT_EQUAL_FLOAT(-1, ret);
    TEST_ASSERT_EQUAL_FLOAT(0, value);
}

void test_str_to_float_multiple_decimals() {
    float value = 0;
    int ret = 0;

    ret = str_to_float("14.46.88", 8, &value);

    TEST_ASSERT_EQUAL_FLOAT(-1, ret);
    TEST_ASSERT_EQUAL_FLOAT(0, value);
}
