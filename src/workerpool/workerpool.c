#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

#include "workerpool.h"
#include "worker.h"

int workerpool_init(Workerpool* workerpool, JobQueue* job_queue,
                    const Configuration* config, void* (*worker_function)(void*)) {
    assert(workerpool != NULL);
    assert(job_queue != NULL);
    assert(config != NULL);
    assert(worker_function != NULL);

    workerpool->job_queue = job_queue;
    workerpool->pool_size = config->workerpool_size;
    if (workerpool->pool_size == 0) { return -1; }

    workerpool->pool = (Worker*)malloc(workerpool->pool_size * sizeof(Worker));
    if (workerpool->pool == NULL) { return -1; }

    int fail_index = 0;
    for (int i = 0; i < workerpool->pool_size; i++) {

        // init worker
        int ret = worker_init(&workerpool->pool[i], job_queue,
                              worker_function);
        if (ret != 0) {
            fail_index = i;
            goto error_on_worker_init;
        }
    }

    return 0;

error_on_worker_init:
    for (int i = 0; i < fail_index; i++) {
        worker_destroy(&workerpool->pool[i]);
    }
    free(workerpool->pool);
    workerpool->pool = NULL;
    return -1;
}

int workerpool_destroy(Workerpool* workerpool) {
    assert(workerpool != NULL);

    for (int i = 0; i < workerpool->pool_size; i++) {
        worker_destroy(&workerpool->pool[i]);
    }
    free(workerpool->pool);
    workerpool->pool = NULL;
    workerpool->pool_size = 0;
    workerpool->job_queue = NULL;

    return 0;
}

int workerpool_start(Workerpool* workerpool) {
    assert(workerpool != NULL);

    for (int i = 0; i < workerpool->pool_size; i++) {
        int ret = worker_start(&(workerpool->pool[i]));
        if (ret != 0) { return ret; }
    }

    return 0;
}

/* int workerpool_stop(Workerpool* workerpool) { */
/*     assert(workerpool != NULL); */
/*     return 0; */
/* } */
