#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

typedef struct stack_node_s {
	void* data;
	struct stack_node_s* next;
} stack_node_t;

typedef struct stack_s {
	stack_node_t* root;
	size_t data_size;
	size_t count;
} stack_t;


// initialize an empty stack
stack_t* stack_init(size_t data_size);


// delete a stack
clib_exit_code_t stack_delete(stack_t* stack);


// swap the memory of 2 stacks
clib_exit_code_t stack_swap(stack_t* a, stack_t* b);


// make an exact copy of a stack
stack_t* stack_copy(stack_t* stack);


// clear all elements in stack
clib_exit_code_t stack_clear(stack_t* stack);


// count the number of elements in stack
size_t stack_count(stack_t* stack);


// push an element at the top of the stack
clib_exit_code_t stack_push(stack_t* stack, void* data);


// remove the top of the stack
clib_exit_code_t stack_pop(stack_t* stack);


// get the value of the top of the stack
const void* stack_top(stack_t* stack);

#endif