#ifndef CUTILS_H
#define CUTILS_H

#include <stdlib.h>
#include <stdint.h>

typedef enum clib_exit_code_e {
	OK = 0,
	OUT_OF_MEM = -1,
	NULL_REF = -2,
	OUT_OF_BOUNDS = -3,
	UNSAFE = -4
} clib_exit_code_t;

size_t _min(size_t a, size_t b);

size_t _max(size_t a, size_t b);

uint8_t	int_comparator(const void* a, const void* b);

#endif