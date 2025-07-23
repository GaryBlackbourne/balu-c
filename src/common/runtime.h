#ifndef _BALU_RUNTIME_H
#define _BALU_RUNTIME_H

#include <stdbool.h>

void program_start();
void program_stop();
bool program_get_run_state();

void setup_sigint();

#endif /* _BALU_RUNTIME */
