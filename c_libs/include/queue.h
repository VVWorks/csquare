/* ================================= 
Copyright (C) 2023 Vornicescu Vasile
A queue implementation in C
*/

#ifndef CQUEUE_H
#define CQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

typedef struct queue_node_s {
	struct queue_node_s* next;
	void* data;
} queue_node_t;

typedef struct queue_s {
	size_t data_size;
	size_t count;
	queue_node_t* head;
	queue_node_t* tail;
} queue_t;


// initialize a queue
queue_t* queue_init(size_t data_size);


// delete a queue
clib_exit_code_t queue_delete(queue_t* queue);


// swap two different queues
clib_exit_code_t queue_swap(queue_t* queue_1, queue_t* queue_2);


// copy a queue
queue_t* queue_copy(queue_t* queue);


// delete all elements from the queue
clib_exit_code_t queue_clear(queue_t* queue);


// get the number of elements in the queue
size_t queue_count(queue_t* queue);


// push an element on the queue tail
clib_exit_code_t queue_push(queue_t* queue, void* data);


// erase the element at the head of the queue
clib_exit_code_t queue_pop(queue_t* queue);


// get the head of the queue
const void* queue_head(queue_t* queue);


#endif