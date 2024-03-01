/* =================================
Copyright (C) 2023 Vornicescu Vasile
A vector implementation in C
*/

#include "../include/vector.h"

clib_exit_code_t add_capacity(vector_t* vec, size_t count) {
	if(count == 0)
		return OK;

	void* tmp = realloc(vec->data, (count + vec->capacity) * vec->data_size);
	if(tmp == NULL)
		return OUT_OF_MEM;

	vec->data = tmp;
	vec->capacity = count + vec->capacity;

	return OK;
}

vector_t* vec_init(size_t capacity, size_t count, size_t data_size) {
	capacity = _max(capacity, count);
	if(capacity == 0)
			capacity = 1;

	vector_t* vec = malloc(sizeof(*vec));
	if(vec == NULL) {
		printf("\n[vector->vec_init]: Not enough memory to create vector!");
		printf(" Returned NULL\n");
		return NULL;
	}

	vec->data = calloc(capacity, data_size);
	if(vec->data == NULL) {
		printf("\n[vector->vec_init]: Not enough memory to create vector!");
		printf(" Returned NULL\n");
		free(vec);
		return NULL;
	}

	vec->data_size = data_size;
	vec->count = count;
	vec->capacity = capacity;

	return vec;
}

clib_exit_code_t vec_delete(vector_t* vec) {
	if(vec == NULL) {
		printf("\n[vector->vec_delete]: Null reference to vector!\n");
		return NULL_REF;
	}

	if(vec->data)
		free(vec->data);

	free(vec);

	return OK;
}

clib_exit_code_t vec_swap(vector_t* vec_1, vector_t* vec_2) {
	if(vec_1 == NULL || vec_2 == NULL) {
		printf("\n[vector->vec_swap]: Null reference to vector!\n");
		return NULL_REF;
	}

	void* vec_1_data = vec_1->data;
	size_t vec_1_cap = vec_1->capacity;
	size_t vec_1_count = vec_1->count;
	size_t vec_1_data_size = vec_1->data_size;

	vec_1->data = vec_2->data;
	vec_1->capacity = vec_2->capacity;
	vec_1->count = vec_2->count;
	vec_1->data_size = vec_2->data_size;

	vec_2->data = vec_1_data;
	vec_2->capacity = vec_1_cap;
	vec_2->count = vec_1_count;
	vec_2->data_size = vec_1_data_size;

	return OK;
}

vector_t* vec_copy(vector_t* vec) {
	if(vec == NULL) {
		printf("\n]vector->vec_copy]: Null reference to vector! Returning NULL\n");
		return NULL;
	}

	vector_t* copy = vec_init(vec->capacity, vec->count, vec->data_size);
	if(copy == NULL) {
		printf("\n]vector->vec_copy]: Could not init copy! Returning NULL\n");
		return NULL;
	}

	if(vec->data) {
		memcpy(copy->data, vec->data, vec->capacity * vec->data_size);
	}

	return copy;
}

clib_exit_code_t vec_clear(vector_t* vec) {
	if(vec == NULL) {
		printf("\n[vector->vec_clear]: Null reference to vector!\n");
		return NULL_REF;
	}

	vec->count = 0;

	return OK;
}

clib_exit_code_t vec_resize(vector_t* vec, size_t new_count) {
	if(vec == NULL) {
		printf("\n[vector->vec_resize]: Null reference to vector!\n");
		return NULL_REF;
	}

	if(vec->capacity < new_count) {
		clib_exit_code_t err = add_capacity(vec, new_count - vec->capacity);
		if(err != OK) {
			printf("\n[vector->vec_resize]: Not enough memory to resize!");
			printf(" The vector will be kept as it is.\n");
			return err;
		}
		vec->count = new_count;
	} else {
		void* tmp = realloc(vec->data, new_count * vec->data_size);
		if(tmp == NULL) {
			printf("\n[vector->vec_resize]: Permision denied! Cannot resize the vector!");
			printf(" The vector will be kept as it is.\n");
			return OUT_OF_MEM;
		}
		vec->capacity = vec->count = new_count;
		vec->data = tmp;
	}

	return OK;
}

clib_exit_code_t vec_push_back(vector_t* vec, void* data) {
	if(vec == NULL || data == NULL) {
		printf("\n[vector->vec_push_back]: Null reference to vector or data!\n");
		return NULL_REF;
	}

	if(vec->count == vec->capacity) {
		clib_exit_code_t err = add_capacity(vec, vec->capacity);
		if(err != OK) {
			printf("\n[vector->vec_push_back]: Not enough memory to resize!");
			printf("The vector will be kept as it is.\n");
			return err;
		}
	}

	vec->count++;

	memcpy((char*)(vec->data) + (vec->count - 1) * vec->data_size, data, vec->data_size);

	return OK;
}

clib_exit_code_t vec_remove_back(vector_t* vec) {
	if(vec == NULL) {
		printf("\n[vector->vec_remove_back]: Null reference to vector!\n");
		return NULL_REF;
	}

	if(vec->count)
		vec->count--;

	return OK;
}

clib_exit_code_t vec_insert(vector_t* vec, void* data, size_t index) {
	if(vec == NULL || data == NULL) {
		printf("\n[vector->vec_insert]: Null reference to vector or data!\n");
		return NULL_REF;
	}

	if(vec->count == vec->capacity) {
		clib_exit_code_t err = add_capacity(vec, vec->capacity);
		if(err != OK) {
			printf("\n[vector->vec_insert]: Not enough memory to resize!");
			printf(" The vector will be kept as it is.\n");
			return err;
		}
	}

	vec->count++;
	for(int i = vec->count - 1; i > index; i--) {
		memcpy((char*)(vec->data) + i * vec->data_size, (char*)(vec->data) + (i - 1) * vec->data_size, vec->data_size);
	}

	memcpy((char*)(vec->data) + index * vec->data_size, data, vec->data_size);

	return OK;
}

clib_exit_code_t vec_erase(vector_t* vec, size_t begin, size_t end) {
	if(vec == NULL) {
		printf("\n[vector->vec_erase]: Null reference to vector!\n");
		return NULL_REF;
	}

	if(begin > end || begin >= vec->count)
		return OK;

	end = _min(end, vec->count - 1);

	for(int i = begin; i < vec->count; i++) {
		memcpy((char*)(vec->data) + i * vec->data_size, (char*)(vec->data) + (i + end - begin + 1) * vec->data_size, vec->data_size);
	}

	vec->count -= end - begin + 1;

	return OK;
}

clib_exit_code_t vec_reverse(vector_t* vec) {
	if(vec == NULL || vec->data == NULL) {
		printf("\n[vector->vec_reverse]: Null reference to vector!\n");
		return NULL_REF;
	}

	if(vec->count == 0)
		return OK;

	size_t l = 0, r = vec->count - 1;
	void* tmp = malloc(vec->data_size);
	if(tmp == NULL) {
		printf("\n[vector->vec_reverse]: Out of memory!\n");
		return OUT_OF_MEM;
	}

	while(l != r) {
		memcpy(tmp, (char*)(vec->data) + l * vec->data_size, vec->data_size);
		memcpy((char*)(vec->data) + l * vec->data_size, (char*)(vec->data) + r * vec->data_size, vec->data_size);
		memcpy((char*)(vec->data) + r * vec->data_size, tmp, vec->data_size);
		--r;
		++l;
	}

	free(tmp);

	return OK;
}

clib_exit_code_t vec_fit(vector_t* vec) {
	if(vec == NULL) {
		return NULL_REF;
	}

	clib_exit_code_t err = vec_resize(vec, vec->count);
	if(err != OK) {
		printf("\n[vector->vec_fit]: Could not fit the vector!\n");
	}

	return err;
}

const void* vec_get(vector_t* vec, size_t index) {
	if(vec == NULL || vec->data == NULL) {
		printf("\n[vector->vec_get]: Null reference to vector!\n");
		return NULL;
	}

	if(index >= vec->count) {
		printf("\n[vector->vec_get]: Index out of bounds!\n");
		return NULL;
	}

	return (void*)((char*)(vec->data) + index * vec->data_size);
}

clib_exit_code_t vec_set(vector_t* vec, void* data, size_t index) {
	if(vec == NULL || vec->data == NULL) {
		printf("\n[vector->vec_set]: Null reference to vector!\n");
		return NULL_REF;
	}

	if(index >= vec->count) {
		printf("\n[vector->vec_set]: Index out of bounds!\n");
		return OUT_OF_BOUNDS;
	}

	memcpy((char*)(vec->data) + index * vec->data_size, data, vec->data_size);

	return OK;
}

// get the count of a vector
size_t vec_count(vector_t* vec) {
	if(vec == NULL)
		return 0;
	return vec->count;
}

// get the capacity of a vector
size_t vec_capacity(vector_t* vec) {
	if(vec == NULL)
		return 0;
	return vec->capacity;
}

void vec_int_print(vector_t* vec) {
	size_t n = vec_count(vec);
	
	printf("\n");

	for(size_t i = 0; i < n; i++) {
		const void* elem = vec_get(vec, i);
		printf("%d ", *(int*)elem);
	}

	printf("\n");
}
