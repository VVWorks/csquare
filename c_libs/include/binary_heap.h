/* ================================= 
Copyright (C) 2023 Vornicescu Vasile
Two binary heap implementations in C
One may work better on some systems then the other
*/

#ifndef CBHEAP_H
#define CBHEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "utils.h"

// Declaration #1 =================================================================================

typedef struct bheap_s {
	size_t capacity;
	size_t count;
	size_t data_size;
	void* data;
	uint8_t	(*compare)(const void*, const void*);
} bheap_t;


// creates a binary heap ready to use
bheap_t* bheap_init(size_t capacity, size_t data_size, uint8_t (*compare)(const void*, const void*));


// deletes a binary heap from memory
clib_exit_code_t bheap_delete(bheap_t* heap);


// clears all the data from a heap
clib_exit_code_t bheap_clear(bheap_t* heap);


// swaps two heaps
clib_exit_code_t bheap_swap(bheap_t* heap_a, bheap_t* heap_b);


// copy a heap
bheap_t* bheap_copy(bheap_t* heap);


// inserts an element in the heap
clib_exit_code_t bheap_insert(bheap_t* heap, void* data);


// remove the top of the heap
clib_exit_code_t bheap_pop(bheap_t* heap);


// get the element at the top of the heap
const void* bheap_get_top(bheap_t* heap);


// sorts an array of data using heap sort
clib_exit_code_t bheap_sort(void* array, size_t elem_size, size_t begin, size_t end, uint8_t (*compare)(const void*, const void*));



// Declaration #2 =================================================================================

typedef struct bheap2_s{
	size_t data_size;
	size_t capacity;
	size_t count;
 
	uint8_t	(*compare)(const void*, const void*);
	void** data;
} bheap2_t;

// creates a binary heap ready to use 
bheap2_t* bheap2_init(size_t capacity, size_t data_size, uint8_t (*compare)(const void*, const void*));


// deletes a binary heap from memory
clib_exit_code_t bheap2_delete(bheap2_t* heap);


// clears all the data from a heap
clib_exit_code_t bheap2_clear(bheap2_t* heap);


// copy a heap
bheap2_t* bheap2_copy(bheap2_t* heap);


// swaps two heaps
clib_exit_code_t bheap2_swap(bheap2_t* heap_a, bheap2_t* heap_b);


// inserts an element in the heap
clib_exit_code_t bheap2_insert(bheap2_t* heap, void* data);


// remove the top of the heap
clib_exit_code_t bheap2_pop(bheap2_t* heap);


// get the element at the top of the heap
void* const	bheap2_get_top(bheap2_t* heap);


// sorts an array of data using heap sort
clib_exit_code_t bheap2_sort(void* array, size_t elem_size, size_t begin, size_t end, uint8_t (*compare)(const void*, const void*));

#endif
