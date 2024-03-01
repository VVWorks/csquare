#include "../include/utils.h"

size_t _min(size_t a, size_t b) {
	return a * (a < b) + b * (a >= b);
}

size_t _max(size_t a, size_t b) {
	return a * (a > b) + b * (a <= b);
}

uint8_t	int_comparator(const void* a, const void* b) {
	return *(int*)a > *(int*)b;
}