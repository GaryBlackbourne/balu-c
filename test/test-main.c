#include "unity.h"
#include "unity_internals.h"

#include "configuration.h"


#include "fifo-test.h"
#include "common-test.h"
#include "config-file-parser-test.h"
#include "cmdline-parser-test.h"
#include "configuration-test.h"
#include "connector-test.h"
#include "job-queue-test.h"
#include "worker-test.h"
#include "workerpool-test.h"

Configuration config;

void setUp(void) {
    // Set up before each test, if needed
    config = configuration_generate_default_config();
}

void tearDown(void) {
    // Clean up after each test, if needed
    configuration_destroy(&config);
}

int main(void) {
    UNITY_BEGIN();

    //-------------------- common --------------------
    RUN_TEST(test_imax);
    RUN_TEST(test_imin);

    RUN_TEST(test_str_to_i32_normal_behavior);
    RUN_TEST(test_str_to_i32_bad_input);

    RUN_TEST(test_str_to_float_normal_behavior);
    RUN_TEST(test_str_to_float_bad_input);
    RUN_TEST(test_str_to_float_multiple_decimals);

    RUN_TEST(test_fifo_init);

    RUN_TEST(test_fifo_destroy_empty);
    RUN_TEST(test_fifo_destroy_non_empty);

    RUN_TEST(test_fifo_push_normal);
    RUN_TEST(test_fifo_push_full);

    RUN_TEST(test_fifo_pop_normal);
    RUN_TEST(test_fifo_pop_empty);

    RUN_TEST(test_fifo_pop_null_size);
    RUN_TEST(test_fifo_pop_null_data);
    RUN_TEST(test_fifo_pop_null_data_null_size);

    //-------------------- config --------------------
    RUN_TEST(test_is_matching_line_match_exact);
    RUN_TEST(test_is_matching_line_match_equation);
    RUN_TEST(test_is_matching_line_no_match_bad_char);
    RUN_TEST(test_is_matching_line_no_match_bad_len);

    RUN_TEST(test_is_matching_arg_matching);
    RUN_TEST(test_is_matching_arg_non_matching_shorter);
    RUN_TEST(test_is_matching_arg_non_matching_longer);
    RUN_TEST(test_is_matching_arg_non_matching_bad_char);

    //-------------------- connection --------------------
    RUN_TEST(test_connector_init_is_correct);
    RUN_TEST(test_connector_destroy_is_correct);

    //-------------------- workerpool --------------------
    RUN_TEST(test_job_queue_init);

    RUN_TEST(test_job_queue_destroy);
    RUN_TEST(test_job_queue_destroy_non_empty);

    RUN_TEST(test_job_queue_push_non_full);
    RUN_TEST(test_job_queue_push_full);

    RUN_TEST(test_job_queue_pop_non_empty);
    RUN_TEST(test_job_queue_pop_empty);

    RUN_TEST(test_worker_can_initialize);

    RUN_TEST(test_worker_properly_destroyed_stopped);
    RUN_TEST(test_worker_properly_destroyed_running);

    RUN_TEST(test_worker_can_start);

    RUN_TEST(test_workerpool_init);
    RUN_TEST(test_workerpool_destroy);

    RUN_TEST(test_workerpool_start);

    return UNITY_END();
}
