/* ================================= 
Copyright (C) 2023 Vornicescu Vasile
A vector implementation in C
*/

#ifndef CVECTOR_H
#define CVECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

typedef struct vector_s {
	size_t count;
	size_t capacity;
	size_t data_size;
	void* data;
} vector_t;


// initialize a vector
vector_t* vec_init(size_t capacity, size_t count, size_t data_size);


// delete a vector
clib_exit_code_t vec_delete(vector_t* vec);


// swap two vectors
clib_exit_code_t vec_swap(vector_t* vec_1, vector_t* vec_2);


// make a copy to a given vector
vector_t* vec_copy(vector_t* vec);


// clear all elements in the vector.
// does NOT resize the vector capacity to 0
// call vec_fit after this operation for that
clib_exit_code_t vec_clear(vector_t* vec);


// resize given vector to given count
clib_exit_code_t vec_resize(vector_t* vec, size_t new_count);


// insert a new element at the end of the vector
clib_exit_code_t vec_push_back(vector_t* vec, void* data);


// remove the last element in the vector
clib_exit_code_t vec_remove_back(vector_t* vec);


// inserts a given element imediatly before given index
clib_exit_code_t vec_insert(vector_t* vec, void* data, size_t index);


// erase elements in range [begin, end]
clib_exit_code_t vec_erase(vector_t* vec, size_t begin, size_t end);


// reverse the order of the vector
clib_exit_code_t vec_reverse(vector_t* vec);


// reduces the vector capacity to the vector count
clib_exit_code_t vec_fit(vector_t* vec);


// return a pointer to the element at the given index in vector
const void* vec_get(vector_t* vec, size_t index);


// sets the value at given index
clib_exit_code_t vec_set(vector_t* vec, void* data, size_t index);


// get the count of a vector
size_t vec_count(vector_t* vec);


// get the capacity of a vector
size_t vec_capacity(vector_t* vec);


// prints an integer vector
void vec_int_print(vector_t* vec);

#endif
