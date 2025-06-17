#ifndef _BALU_WORKER_H
#define _BALU_WORKER_H

#include <bits/pthreadtypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

#include "job-queue.h"

typedef struct {
    pthread_t handler;           // pthread handler for the thread
    void* (*function)(void* vp); // function to be executed
    JobQueue* job_queue;         // stored as reference
} Worker;

int worker_init(Worker* worker, JobQueue* job_queue, void* (*function)(void* vp));

int worker_destroy(Worker* worker);

int worker_start(Worker* worker);

void* worker_thread_function(void* arg /*Fifo of Jobs*/);

#endif
