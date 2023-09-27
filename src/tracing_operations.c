#include "failure_codes.h"
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <asm-generic/unistd.h>
#include <sys/user.h>
#include <stdio.h>

#define MAX_SYSCALL_NUMBER __NR_syscalls

int setup_tracer(pid_t child_pid)
{
	// PTRACE_SEIZE is deliberately chosen over PTRACE_ATTACH
	if (ptrace(PTRACE_SEIZE, child_pid, NULL, NULL) == -1) {
		return PTRACE_ATTACHMENT_FAILURE;
	}

	// Before following up with tracing, we need to wait on the child
	// to make sure that the child is in a stable state to be tracec.
	if (waitpid(child_pid, (int *)NULL, 0) == -1) {
		return CHILD_WAIT_FAILURE;
	}

	if (ptrace(PTRACE_SETOPTIONS, child_pid, NULL, PTRACE_O_TRACESYSGOOD) == -1) {
		return PTRACE_OPTIONS_TRACESYSGOOD_FAILURE;
	}

	return SUCCESS_NO_FAILURE;
}

void print_call_counts(u_int64_t *call_count_array)
{
	printf("CALL_COUNT, SYSCALL_NUM\n");
	for (u_int64_t i = 0; i < MAX_SYSCALL_NUMBER; i++) {
		if (!(call_count_array[i] == 0U)) {
			printf("%lu, %lu\n", (u_int64_t)(call_count_array[i] / 2), i);
		}
	}
}

int trace_child(pid_t child_pid)
{
	u_int64_t syscall_call_counts[MAX_SYSCALL_NUMBER] = { 0 };
	int child_status = 0;
	struct user_regs_struct GP_registers = { 0 };
	while (1) {
		if (ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1) {
			return PTRACE_CONTINUE_FAILURE;
		}

		if (waitpid(child_pid, &child_status, 0) == -1) {
			return CHILD_WAIT_FAILURE;
		}

		if (WIFEXITED(child_status)) {
			// before exiting, print the syscall count array, minus the syscalls that were never called.
			print_call_counts(syscall_call_counts);
			break;
		} else if (WIFSTOPPED(child_status) &&
			   (WSTOPSIG(child_status) & 0x80)) {
			// this runs only if the reason for the stop was a syscall
			if (ptrace(PTRACE_GETREGS, child_pid, NULL, &GP_registers) == -1) {
				return REGISTER_VALUE_RETRIEVAL_FAILURE;
			}

			if (GP_registers.orig_rax < MAX_SYSCALL_NUMBER) {
				syscall_call_counts[GP_registers.orig_rax]++;
			}
		}
	}

	return SUCCESS_NO_FAILURE;
}