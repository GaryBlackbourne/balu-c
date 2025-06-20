#ifndef _BALU_JOB_QUEUE_TEST_H
#define _BALU_JOB_QUEUE_TEST_H

void test_job_queue_init(void);

void test_job_queue_destroy(void);
void test_job_queue_destroy_non_empty(void);

void test_job_queue_push_non_full(void);
void test_job_queue_push_full(void);

void test_job_queue_pop_non_empty(void);
void test_job_queue_pop_empty(void);

#endif /* _BALU_JOB_QUEUE_TEST */
