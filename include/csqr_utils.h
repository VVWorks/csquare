#ifndef CSQR_UTILS
#define CSQR_UTILS

typedef enum {
	SUCCES_EXIT = 0,
	NO_ARGS_EXIT = -1,
	NO_FILE_EXIT = -2,
	UNKNOUN_FLAG_EXIT = -3,
	NULL_REF_EXIT = -4
} CSQR_EXIT;

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