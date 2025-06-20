#include <assert.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>

#include "job-queue.h"
#include "job.h"
#include "worker.h"

int worker_init(Worker* worker, JobQueue* job_queue, void*(*function)(void* vp)) {
    assert(worker != NULL);
    assert(job_queue != NULL);

    worker->job_queue = job_queue;
    worker->function  = function;

    return 0;
}

int worker_start(Worker* worker) {
    assert(worker != NULL);

    // create thread with default attributes
    int ret = pthread_create(&worker->handler, NULL, worker->function,
                             (void*)&worker->job_queue);

    // check for errors
    if (ret < 0) {
        perror("pthread_create");
        return ret;
    }

    return 0;
}


int worker_destroy(Worker* worker) {
    assert(worker != NULL);

    // cancel running thread
    pthread_cancel(worker->handler);

    // we do not deallocate resources, because we only store
    // references, the ownership is not ours

    return 0;
}

