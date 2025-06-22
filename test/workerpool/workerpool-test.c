#include "unity.h"

#include "workerpool-test.h"
#include "workerpool.h"
#include "job-queue.h"
#include <pthread.h>
#include <unistd.h>

extern Configuration config;

static void* do_nothing_function(void*) {
    return NULL;
}

void test_workerpool_init(void) {
    Workerpool workerpool;
    JobQueue   job_queue;

    job_queue_init(&job_queue, &config);
    TEST_ASSERT_EQUAL(0, workerpool_init(&workerpool, &job_queue, &config,
                                         do_nothing_function));
    TEST_ASSERT_EQUAL(&job_queue, workerpool.job_queue);
    TEST_ASSERT_EQUAL(config.workerpool_size, workerpool.pool_size);
    TEST_ASSERT_NOT_EQUAL(NULL, workerpool.pool);
    TEST_ASSERT_EQUAL(do_nothing_function, workerpool.pool[0].function);

    job_queue_destroy(&job_queue);
    workerpool_destroy(&workerpool);
}

static int test_data[3] = { 1, 2, 3 };

static void* wf (void* arg) {
    JobQueue* jq = (JobQueue*)arg;
    Job job = {0};
    job_queue_pop(jq, &job);

    int data = job.connection.socket;

    test_data[data % 3] = data;

    return NULL;
}

void test_workerpool_start() {
    Workerpool workerpool;
    JobQueue   job_queue;

    Configuration custom_config   = config;
    custom_config.workerpool_size = 3;

    Job job6 = {
        .connection = {
            .address = { 0 },
            .address_length = 0,
            .socket = 6,
        }
    };
    Job job7 = {
        .connection = {
            .address = { 0 },
            .address_length = 0,
            .socket = 7,
        }
    };
    Job job8 = {
        .connection = {
            .address = { 0 },
            .address_length = 0,
            .socket = 8,
        }
    };

    job_queue_init(&job_queue, &custom_config);
    job_queue_push(&job_queue, job6);
    job_queue_push(&job_queue, job7);
    job_queue_push(&job_queue, job8);

    workerpool_init(&workerpool, &job_queue, &custom_config, wf);

    TEST_ASSERT_EQUAL(1, test_data[0]);
    TEST_ASSERT_EQUAL(2, test_data[1]);
    TEST_ASSERT_EQUAL(3, test_data[2]);

    TEST_ASSERT_EQUAL(0, workerpool_start(&workerpool));

    sleep(1);

    TEST_ASSERT_EQUAL(6, test_data[0]);
    TEST_ASSERT_EQUAL(7, test_data[1]);
    TEST_ASSERT_EQUAL(8, test_data[2]);

    workerpool_destroy(&workerpool);
    job_queue_destroy(&job_queue);
}

void test_workerpool_destroy(void) {
    Workerpool workerpool;
    JobQueue   job_queue;
    job_queue_init(&job_queue, &config);
    workerpool_init(&workerpool, &job_queue, &config, do_nothing_function);

    TEST_ASSERT_EQUAL(0, workerpool_destroy(&workerpool));
    TEST_ASSERT_EQUAL(NULL, workerpool.job_queue);
    TEST_ASSERT_EQUAL(NULL, workerpool.pool);
    TEST_ASSERT_EQUAL(0, workerpool.pool_size);

    job_queue_destroy(&job_queue);
}
