#ifndef _BALU_WORKERPOOL_H
#define _BALU_WORKERPOOL_H

#include <bits/pthreadtypes.h>
#include <stdint.h>
#include "configuration.h"

#include "job-queue.h"
#include "worker.h"
#include "fifo.h"

/**
 * @brief Workerpool struct for managing running worker threads and jobs.
 */
typedef struct {
    Worker*         pool;
    uint16_t        pool_size;
    JobQueue*       job_queue;  // this is a reference not a stored and managed data
} Workerpool;

int workerpool_init(Workerpool* workerpool, JobQueue* job_queue,
                    const Configuration* config);

int workerpool_start(Workerpool* workerpool);

/* int workerpool_stop(Workerpool* workerpool); */

int workerpool_destroy(Workerpool* workerpool);

#endif
