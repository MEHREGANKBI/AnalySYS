#include "tracing_operations.h"
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <linux/ptrace.h>
#include <signal.h>
#include <string.h>

#include "failure_codes.h"
#include "child_operations.h"

enum failure_codes failure_code;

const char *failure_messages[] = {
	NULL, // On success, this array will not be accessed.
	"Not enough arguments\nUSAGE: analySYS your-CMD [your-cmd-args]",
	"Forking Failed.",
	"Failed to exec your command.",
	"Failed to attach to the child process.",
	"Failed to set the TRACESYSGOOD option for tracing.",
	"Failed to wait for child.",
	"Failed to continue the child process.",
	"Failed to obtain the child process' register values.",
};

int main(int argc, char **argv)
{
	if (argc < 2) {
		errno = EINVAL;
		failure_code = INSUFFICENT_ARGS_FAILURE;
		goto terminal_failure_handler;
	}

	pid_t fork_pid = fork();
	if (fork_pid == -1) {
		failure_code = FORK_FAILURE;
		goto terminal_failure_handler;
	} else if (fork_pid == 0) {
		raise(SIGSTOP);
		if ((failure_code = child_exec(argv + 1)) !=
		    SUCCESS_NO_FAILURE) {
			goto terminal_failure_handler;
		}
	}

	if ((failure_code = setup_tracer(fork_pid)) != SUCCESS_NO_FAILURE) {
		goto kill_child;
	}

	if ((failure_code = trace_child(fork_pid)) != SUCCESS_NO_FAILURE) {
		goto kill_child;
	}

	return EXIT_SUCCESS;


kill_child:
	if (kill(fork_pid, SIGTERM) == -1) {
			printf("Failed to kill the child. Error code: %s\n",
				strerror(errno));
		}

terminal_failure_handler:
	printf("%s\nError code: %s\n", failure_messages[failure_code],
	       strerror(errno));
	return EXIT_FAILURE;
}
