#include "../include/csqr_utils.h"

unsigned int is_flag_on(unsigned int flags, FLAGS id) {
	return (flags & (1 << id));
}
void set_flag_on(unsigned int* flags, FLAGS id) {
	*flags |= (1 << id);
}