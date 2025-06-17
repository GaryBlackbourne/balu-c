#ifndef _BALU_CONNECTOR_H
#define _BALU_CONNECTOR_H

#include "fifo.h"
#include "configuration.h"
#include "job-queue.h"

/**
 * @brief Connector struct to handle requests.
 *
 * This struct contains a socket which can be polled for requests and a job
 * queue, which is loaded as requests are received. The  handler threads
 * read this queue and process the incomming requests. The job queue contains
 * Job structs behind a void pointer. More at src/networking/job.h
 */
typedef struct {
    int  socket;
} Connector;

/**
 * @brief Initializer for the connector struct.
 *
 * This function initializes the connector struct based on the configuration
 * given as the second argument.
 *
 * @param connector - Connector* pointer to the struct to be initialized.
 * @param config - const Configuration* pointer to the configuration.
 *
 * @return integer, indicating error or success. On success returns 0, on error
 * returns an error code.
 */
int connector_init(Connector* connector, const Configuration* config);

/**
 * @brief Destructor for a connector instance.
 *
 * Destructs the connector struct and all its content.
 *
 * @param connector - Connector* pointer to the struct to be destroyed.
 *
 * @return integer, indicating error or success. On success returns 0, on
 * error returns an error code.
 */
int connector_destroy(Connector* connector);

/**
 * @brief Start the connector.
 *
 * This function starts the connector, initializes the job queue and the
 * socket, then sets the socket to listen mode.
 *
 * @param connector - connector* pointer to the connector instance.
 *
 * @return integer, indicating error or success. on success returns 0, on
 * error returns an error code.
 */
int connector_start(Connector* connector);

int connector_iterate(Connector* connector, JobQueue* job_queue);

#endif
