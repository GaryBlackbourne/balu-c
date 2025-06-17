#ifndef _BALU_FIFO_TEST_H
#define _BALU_FIFO_TEST_H

void fifo_init_test(void);
void fifo_destroy_empty_test(void);
void fifo_destroy_non_empty_test(void);

void fifo_push_normal(void);
void fifo_push_full(void);

void fifo_pop_normal(void);
void fifo_pop_empty(void);

void fifo_pop_null_data(void);
void fifo_pop_null_size(void);
void fifo_pop_null_data_null_size(void);

#endif
