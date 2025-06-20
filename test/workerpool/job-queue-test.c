#include "fifo.h"
#include "unity.h"

#include "job-queue-test.h"
#include "job-queue.h"

#include <errno.h>
#include <pthread.h>

extern Configuration config;

void test_job_queue_init(void) {
    JobQueue job_queue;
    errno = 0;
    int ret = job_queue_init(&job_queue, &config);
    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_EQUAL(0, errno);

    // fifo initialized correctly
    TEST_ASSERT_EQUAL(0, job_queue.fifo.items_num);
    TEST_ASSERT_EQUAL(config.job_queue_length, job_queue.fifo.length);

    // mutex constructed correctly
    ret = 0;
    ret = pthread_mutex_trylock(&job_queue.fifo_lock);
    TEST_ASSERT_EQUAL(0, ret);
    pthread_mutex_unlock(&job_queue.fifo_lock);

    // cond var mutex constructed correctly
    ret = 0;
    ret = pthread_mutex_trylock(&job_queue.new_job_cond_mux);
    TEST_ASSERT_EQUAL(0, ret);
    pthread_mutex_unlock(&job_queue.new_job_cond_mux);
}

void test_job_queue_destroy(void) {
    JobQueue job_queue;
    job_queue_init(&job_queue, &config);

    int ret = job_queue_destroy(&job_queue);
    TEST_ASSERT_EQUAL(0, ret);

    TEST_ASSERT_NULL(job_queue.fifo.list);
    TEST_ASSERT_EQUAL(0, job_queue.fifo.length);
    TEST_ASSERT_EQUAL(0, job_queue.fifo.index);
    TEST_ASSERT_EQUAL(0, job_queue.fifo.items_num);

    TEST_ASSERT_EQUAL(EINVAL, pthread_mutex_trylock(&job_queue.fifo_lock));
    TEST_ASSERT_EQUAL(EINVAL, pthread_mutex_trylock(&job_queue.new_job_cond_mux));
}

void test_job_queue_destroy_non_empty(void) {
    JobQueue job_queue;
    job_queue_init(&job_queue, &config);

    Job job = {
        .connection = {
            .socket = 4,
            .address = {0},
            .address_length = 7,
        },
    };

    job_queue_push(&job_queue, job);

    int ret = job_queue_destroy(&job_queue);
    TEST_ASSERT_EQUAL(0, ret);

    TEST_ASSERT_NULL(job_queue.fifo.list);
    TEST_ASSERT_EQUAL(0, job_queue.fifo.length);
    TEST_ASSERT_EQUAL(0, job_queue.fifo.index);
    TEST_ASSERT_EQUAL(0, job_queue.fifo.items_num);

    TEST_ASSERT_EQUAL(EINVAL, pthread_mutex_trylock(&job_queue.fifo_lock));
    TEST_ASSERT_EQUAL(EINVAL, pthread_mutex_trylock(&job_queue.new_job_cond_mux));
}

void test_job_queue_push_non_full(void) {
    JobQueue job_queue;
    job_queue_init(&job_queue, &config);

    Job job = {
        .connection = {
            .socket = 4,
            .address = {0},
            .address_length = 7,
        },
    };

    int ret = -1;
    for (int i = 0; i < config.job_queue_length; i++) {
        ret = job_queue_push(&job_queue, job);
        TEST_ASSERT_EQUAL(0, ret);
        TEST_ASSERT_EQUAL(i + 1, job_queue.fifo.items_num);
    }

    job_queue_destroy(&job_queue);
}

void test_job_queue_push_full(void) {
    JobQueue job_queue;
    job_queue_init(&job_queue, &config);

    Job job = {
        .connection = {
            .socket = 4,
            .address = {0},
            .address_length = 7,
        },
    };

    int ret = -1;
    for (int i = 0; i < config.job_queue_length; i++) {
        ret = job_queue_push(&job_queue, job);
        TEST_ASSERT_EQUAL(0, ret);
        TEST_ASSERT_EQUAL(i + 1, job_queue.fifo.items_num);
    }

    ret = job_queue_push(&job_queue, job);
    TEST_ASSERT_EQUAL(-1, ret);
    job_queue_pop(&job_queue, NULL);
    TEST_ASSERT_EQUAL(config.job_queue_length - 1, job_queue.fifo.items_num);
}

void test_job_queue_pop_non_empty(void) {
    JobQueue job_queue;
    job_queue_init(&job_queue, &config);

    Job job = {
        .connection = {
            .socket = 4,
            .address = {0},
            .address_length = 7,
        },
    };

    job_queue_push(&job_queue, job);
    job_queue_push(&job_queue, job);
    job_queue_push(&job_queue, job);

    int ret = -1;
    for (int i = 0; i < 3; i++) {
        ret = job_queue_pop(&job_queue, NULL);
        TEST_ASSERT_EQUAL_INT(0, ret);
        TEST_ASSERT_EQUAL_INT(2 - i, job_queue.fifo.items_num);
    }

    job_queue_destroy(&job_queue);
}

void test_job_queue_pop_empty(void) {
    JobQueue job_queue;
    job_queue_init(&job_queue, &config);

    Job job = {
        .connection = {
            .socket = 4,
            .address = {0},
            .address_length = 7,
        },
    };

    job_queue_push(&job_queue, job);
    job_queue_push(&job_queue, job);
    job_queue_push(&job_queue, job);

    int ret = -1;
    for (int i = 0; i < 3; i++) {
        ret = job_queue_pop(&job_queue, NULL);
        TEST_ASSERT_EQUAL_INT(0, ret);
        TEST_ASSERT_EQUAL_INT(2 - i, job_queue.fifo.items_num);
    }

    ret = job_queue_pop(&job_queue, NULL);
    TEST_ASSERT_EQUAL_INT(-1, ret);
    TEST_ASSERT_EQUAL_INT(0, job_queue.fifo.items_num);

    job_queue_destroy(&job_queue);
}
