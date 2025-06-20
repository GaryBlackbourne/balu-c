#ifndef _BALU_FIFO_TEST_H
#define _BALU_FIFO_TEST_H

void test_fifo_init(void);
void test_fifo_destroy_empty(void);
void test_fifo_destroy_non_empty(void);

void test_fifo_push_normal(void);
void test_fifo_push_full(void);

void test_fifo_pop_normal(void);
void test_fifo_pop_empty(void);

void test_fifo_pop_null_data(void);
void test_fifo_pop_null_size(void);
void test_fifo_pop_null_data_null_size(void);

#endif
