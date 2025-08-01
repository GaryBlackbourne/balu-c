#include "worker-thread-functions.h"
#include "job-queue.h"
#include "job.h"
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>


void* worker_thread_function(void* arg) {

    /* JobQueue* job_queue = (JobQueue*)arg; */
    /* Job       job; */

    while(1) { // how to stop threads?

        /* pthread_mutex_lock(&job_queue->new_job_cond_mux); */
        /* pthread_cond_wait(&job_queue->new_job_cond_v, &job_queue->new_job_cond_mux); */
        /* pthread_mutex_unlock(&job_queue->new_job_cond_mux); */

        /* int ret = job_queue_pop(job_queue, &job); */
        /* if (ret < 0) { */
        /*     pthread_exit(NULL); */
        /* } */

        /* // process http request */
        printf("Thread %lu is running!\n", pthread_self());
        sleep(rand() % 3);

    }

    pthread_exit(NULL);
}
