#include "worker-thread-functions.h"
#include "job-queue.h"
#include "job.h"
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <string.h>

void* worker_thread_function(void* arg) {

    JobQueue* job_queue = (JobQueue*)arg;

    while(1) {

        pthread_mutex_lock(&job_queue->new_job_cond_mux);
        pthread_cond_wait(&job_queue->new_job_cond_v, &job_queue->new_job_cond_mux);
        pthread_mutex_unlock(&job_queue->new_job_cond_mux);

        Job job;
        int ret = job_queue_pop(job_queue, &job);
        if (ret < 0) {
            perror("job_queue_pop");
            pthread_exit(NULL);
        }


        // parse requests
        //   - protocol
        //   - request type
        //   - header?
        //   - content?

        // generate response
        //   - file read
        //   - standard errors? (unsupported req type)
        //   - logger?

        const char* response = "\
HTTP/1.0 200 OK\r\n\
Content-Type: text/html\r\n\
Content-Length: 106\r\n\
\r\n\r\n\
<!DOCTYPE html>\r\n\
<html>\r\n\
<body>\r\n\
<h1>My First Heading</h1>\r\n\
<p>My first paragraph.</p>\r\n\
</body>\r\n\
</html>\r\n\
            ";
        size_t response_len = strlen(response);
        ret = write(job.connection.socket, response, response_len);
        if (ret < 0) {
            perror("write");
            pthread_exit(NULL);
        } else {
            char address_string[20];
            memset(address_string, '\0', 20);
            inet_ntop(AF_INET, &job.connection.address, address_string, 20);
            printf("response sent to: %s\n", address_string);
        }
        
        /* // process http request */
        sleep(rand() % 3);

    }

    pthread_exit(NULL);
}
