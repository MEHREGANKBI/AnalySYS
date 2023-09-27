#ifndef TRACING_OPERATIONS_H
#define TRACING_OPERATIONS_H

#include <unistd.h>

int setup_tracer(pid_t child_pid);

int trace_child(pid_t child_pid);

#endif