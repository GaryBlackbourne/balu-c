#include "unity.h"

#include "worker.h"
#include "job-queue.h"
#include <unistd.h>
#include "worker-test.h"

extern Configuration config;

static void* test_fun(void*) { return NULL; }

void worker_can_initialize(void) {
    Worker worker;
    JobQueue job_queue;
    TEST_ASSERT_EQUAL(0, job_queue_init(&job_queue, &config));

    int ret = worker_init(&worker, &job_queue, test_fun);
    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_EQUAL(&job_queue, worker.job_queue);
    TEST_ASSERT_EQUAL(0, worker.handler);
    TEST_ASSERT_EQUAL(test_fun, worker.function);
}

void worker_properly_destroyed(void) {
    Worker worker;
    JobQueue job_queue;
    TEST_ASSERT_EQUAL(0, job_queue_init(&job_queue, &config));

    worker_init(&worker, &job_queue, test_fun);
    int ret = worker_destroy(&worker);
    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_EQUAL(0, worker.handler);
    TEST_ASSERT_EQUAL(NULL, worker.function);
    TEST_ASSERT_EQUAL(NULL, worker.job_queue);
    TEST_ASSERT_NOT_NULL(&job_queue);
}

static volatile int testvalue = 5;

static void* adds_seven(void* arg) {
    (void)arg;
    testvalue = 69;
    return NULL;
}

void worker_can_start(void) {
    Worker worker;
    JobQueue job_queue;
    TEST_ASSERT_EQUAL(0, job_queue_init(&job_queue, &config));

    worker_init(&worker, &job_queue, adds_seven);
    TEST_ASSERT_EQUAL(5, testvalue);
    worker_start(&worker);
    sleep(1);
    TEST_ASSERT_EQUAL(69, testvalue);
}
