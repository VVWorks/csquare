#ifndef CSQR_UTILS
#define CSQR_UTILS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define OPERATOR_COUNT 1
#define DEBUG
#define LINUX

typedef enum {
	SUCCES_EXIT = 0,
	NO_ARGS_EXIT = -1,
	NO_FILE_EXIT = -2,
	UNKNOUN_FLAG_EXIT = -3,
	NULL_REF_EXIT = -4,
	TOO_MANY_ARGS_EXIT = -5,
	COMPILATION_ERROR_EXIT = -6
} CSQR_EXIT;

typedef enum {
	SUCCES = 0,
	INTERNAL_ERROR = -1,
	MISSING_PARENTHESES = -2
} COMP_ERROR;

typedef enum {
	FLAG_VERBOSE = 0 // -v
} FLAGS;

typedef struct {
	unsigned int flags;
	// if the i-th bit is on, then the i-th flag is on

	char* source_code;
	// path to the csqr source file
} csqr_task_t;


unsigned int is_flag_on(unsigned int flags, FLAGS id);
void set_flag_on(unsigned int* flags, FLAGS id);

#endif