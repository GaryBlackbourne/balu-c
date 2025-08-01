#include <asm-generic/socket.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "config-file-parser.h"
#include "cmdline-parser.h"
#include "configuration.h"
#include "connector.h"
#include "job-queue.h"
#include "runtime.h"
#include "worker-thread-functions.h"
#include "workerpool.h"

int main(int argc, const char* argv[]) {

    // Generate default configuration
    Configuration main_config = configuration_generate_default_config();

    // parse configuration file and override defaults
    // if file is non existent, just go with the defaults
    if (config_file_exists(main_config.configuration.configuration_file_path)) {
        int conf_ret = parse_configuration_file(
            &main_config, main_config.configuration.configuration_file_path);
        if (conf_ret != 0) { return -1; }
    }

    // parse command line arguments as final, top priority settings
    int cmd_ret = parse_cmdline_arguments(argc, argv, &main_config);
    if (cmd_ret != 0) { return -1; }

    printf("Default config path: %s\n",
           main_config.configuration.configuration_file_path);
    printf("Default executable name: %s\n", main_config.general.binary_name);

    JobQueue job_queue;
    int      jq_ret = job_queue_init(&job_queue, &main_config);
    if (jq_ret != 0) { return -1; }

    Connector connector;
    if (connector_init(&connector, &main_config) != 0) { return -1; }

    Workerpool workerpool;
    if (workerpool_init(&workerpool, &job_queue, &main_config,
                        worker_thread_function) != 0) {
        return -1;
    }

    if(workerpool_start(&workerpool) != 0) {
        return -1;
    }

    if (connector_start(&connector) != 0) { return -1; }

    setup_sigint();
    program_start();
    while (program_get_run_state()) {
        if (connector_iterate(&connector, &job_queue) != 0) { break; }
    }

    connector_destroy(&connector);
    workerpool_destroy(&workerpool);
    configuration_destroy(&main_config);

    return 0;
}
