#include "connector.h"
#include "connection.h"
#include "fifo.h"
#include "job-queue.h"

#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <asm-generic/socket.h>
#include <unistd.h>
#include <poll.h>

int connector_init(Connector* connector, const Configuration* config) {
    assert(config != NULL);
    assert(connector != NULL);

    connector->socket = socket(AF_INET, SOCK_STREAM, 0);
    int reuse         = 1;
    int setsockopt_ret =
        setsockopt(connector->socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &reuse, sizeof(reuse));
    if (setsockopt_ret != 0) {
        perror("setsockopt");
        return -1;
    }

    struct sockaddr_in address = {.sin_family      = AF_INET,
                                  .sin_addr.s_addr = INADDR_ANY,
                                  .sin_port = htons(config->connection.port)};

    int bind_ret =
        bind(connector->socket, (struct sockaddr*)&address, sizeof(address));
    if (bind_ret < 0) {
        perror("bind");
        return -1;
    }
    return 0;
}

int connector_destroy(Connector* connector) {
    assert(connector != NULL);
    int ret = close(connector->socket);
    if (ret != 0) { perror("close"); }
    return ret;
}

int connector_start(Connector* connector) {
    assert(connector != NULL);

    // listen on connector socket
    int ret = listen(connector->socket, 5);  // why 5?
    if (ret < 0) {
        perror("listen");
        return ret;
    }

    return 0;
}

int connector_iterate(Connector* connector, JobQueue* job_queue) {
    assert(connector != NULL);
    assert(job_queue != NULL);

    // poll on connector socket
    struct pollfd p = {
        .events = POLLIN,
        .fd     = connector->socket,
    };

    // receive connection, accept it and push to job queue fifo
    if (poll(&p, 1, -1) > 0) {
        Connection conn = {0};
        conn.socket =
            accept(connector->socket, (struct sockaddr*)&conn.address,
                   &conn.address_length);
        Job job = {
            .connection = conn,
        };
        int ret = job_queue_push(job_queue, job);
        if (ret < 0) {
            close(job.connection.socket);
        }
    }
    
    return 0;
}
