#ifndef _BALU_JOB_QUEUE_TEST_HPP
#define _BALU_JOB_QUEUE_TEST_HPP

void job_queue_init_test(void);

void job_queue_destroy_test(void);
void job_queue_destroy_non_empty_test(void);

void job_queue_push_non_full_test(void);
void job_queue_push_full_test(void);

void job_queue_pop_non_empty_test(void);
void job_queue_pop_empty_test(void);

#endif /* _BALU_JOB_QUEUE_TEST */
