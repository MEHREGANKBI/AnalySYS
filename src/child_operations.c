#include <unistd.h>
#include "failure_codes.h"

int child_exec(char **exec_argv)
{
	int exec_status = execvp(exec_argv[0], exec_argv);
	if (exec_status == -1) {
		return CHILD_EXEC_FALIURE;
	}
	return SUCCESS_NO_FAILURE;
}