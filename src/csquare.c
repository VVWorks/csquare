#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/csqr_reader.h"
#include "../include/csqr_utils.h"


CSQR_EXIT solve_task(csqr_task_t* task) {
	if (!task)
		return NULL_REF_EXIT;

	if (!task->source_code) {
		printf("Error: No source file given\n\n");
		return NO_ARGS_EXIT;
	}

	if (is_flag_on(task->flags, FLAG_VERBOSE)) {
		printf("Trying to open sourcefile: %s\n", task->source_code);
	}

	FILE* src = fopen(task->source_code, "r");
	if (!src) {
		printf("Error: Could not open file %s\n\n", task->source_code);
		return NO_FILE_EXIT;
	}

	if (is_flag_on(task->flags, FLAG_VERBOSE)) {
		printf("Sourcefile opened\n\n");

		printf("Preprocessing sourcefile: %s\n\n", task->source_code);
	}

	program_t* program = NULL;
	COMP_ERROR comp = create_program(src, program);

	if (comp) {
		printf("Error while compiling program. error code = <%d>\n\n", comp);
		return COMPILATION_ERROR;
	}

	fclose(src);

	if (is_flag_on(task->flags, FLAG_VERBOSE)) {
		printf("Program succesfuly proccessed with exit code = <%d>\n\n", comp);

		printf("Executing the program\n\n");
	}

	return SUCCES_EXIT;
}


int main(int argc, char *argv[]) {
	csqr_task_t task;
	task.flags = 0;
	task.source_code = NULL;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (strlen(argv[i]) > 2) {
				printf("Unknoun flag %s, exiting\n", argv[i]);
				return UNKNOUN_FLAG_EXIT;
			}

			// flag detected
			char flag_type = argv[i][1];
			switch (flag_type) {
				case 'v':
					set_flag_on(&(task.flags), FLAG_VERBOSE);
				break;
				default:
					printf("Unknoun flag %s, exiting\n", argv[i]);
					return UNKNOUN_FLAG_EXIT;
				break;
			}

			continue;
		}

		// if not flag, argument must be source file name
		if (task.source_code) {
			printf("Please provide only one source file, exiting!\n");
			return TOO_MANY_ARGS;
		}
		
		task.source_code = argv[i];
	}

	return solve_task(&task);
}
