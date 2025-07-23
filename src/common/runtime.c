#include "runtime.h"
#include <signal.h>
#include <stdio.h>

static bool run = true;

void program_start() {
    run = true;
}

void program_stop() {
    run = false;
}

bool program_get_run_state() {
    return run;
}

static void sigint_handler(int sig) {
    printf("signal: %d received!\n", (sig)); // todo: this should be string
    program_stop();
}

void setup_sigint() {
    signal(SIGINT, sigint_handler);
}
