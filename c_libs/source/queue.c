#include "../include/queue.h"


clib_exit_code_t _queue_free_node(queue_node_t* node) {
	if(node == NULL) {
		return NULL_REF;
	}

	if(node->data) {
		free(node->data);
	}

	return OK;
}


// initialize a queue
queue_t* queue_init(size_t data_size) {
	if(data_size == 0)
		return NULL;

	queue_t* queue = malloc(sizeof(queue_t));
	if(queue == NULL) {
		return NULL;
	}

	queue->data_size = data_size;
	queue->count = 0;
	queue->head = NULL;
	queue->tail = NULL;	

	return queue;
}


// delete a queue
clib_exit_code_t queue_delete(queue_t* queue) {
	if(queue == NULL) {
		return NULL_REF;
	}

	queue_node_t* node = queue->head;
	queue_node_t* tmp = NULL;

	while(node != NULL) {
		tmp = node;
		node = node->next;

		_queue_free_node(tmp);
		free(tmp);
	}

	free(queue);

	return OK;
}


// swap two different queues
clib_exit_code_t queue_swap(queue_t* queue_1, queue_t* queue_2) {
	if(queue_1 == NULL || queue_2 == NULL) {
		return NULL_REF;
	}

	size_t data_size_1 = queue_1->data_size;
	size_t count_1 = queue_1->count;
	queue_node_t* head_1 = queue_1->head;
	queue_node_t* tail_1 = queue_1->tail;

	queue_1->data_size = queue_2->data_size;
	queue_1->count = queue_2->count;
	queue_1->head = queue_2->head;
	queue_1->tail = queue_2->tail;

	queue_2->data_size = data_size_1;
	queue_2->count = count_1;
	queue_2->head = head_1;
	queue_2->tail = tail_1;

	return OK;
}


// copy a queue
queue_t* queue_copy(queue_t* queue) {
	if(queue == NULL) {
		return NULL;
	}

	queue_t* copy = queue_init(queue->data_size);

	queue_node_t* node = queue->head;

	while (node != NULL) {
		clib_exit_code_t err = queue_push(copy, node->data);
		if (err) {
			return copy;
		}

		node = node->next;
	}

	return copy;
}


// delete all elements from the queue
clib_exit_code_t queue_clear(queue_t* queue) {
	if(queue == NULL) {
		return NULL_REF;
	}

	queue_node_t* node = queue->head;
	queue_node_t* tmp = NULL;

	while(node != NULL) {
		tmp = node;
		node = node->next;

		_queue_free_node(tmp);
		free(tmp);
	}

	queue->head = queue->tail = NULL;
	queue->count = 0;

	return OK;
}


// get the number of elements in the queue
size_t queue_count(queue_t* queue) {
	if(queue == NULL) {
		return 0;
	}

	return queue->count;
}


queue_node_t* _queue_new_node(queue_t* queue, void* data) {
	if(data == NULL) {
		return NULL;
	}

	queue_node_t* node = malloc(sizeof(queue_node_t));
	if(node == NULL) {
		return NULL;
	}

	node->data = malloc(queue->data_size);
	if(node->data == NULL) {
		return NULL;
	}

	memcpy(node->data, data, queue->data_size);

	node->next = NULL;

	return node;
}


// push an element on the queue tail
clib_exit_code_t queue_push(queue_t* queue, void* data) {
	if(queue == NULL || data == NULL) {
		return NULL_REF;
	}

	if(queue->head == NULL) {
		queue->head = queue->tail = _queue_new_node(queue, data);
		if(queue->head == NULL) {
			return OUT_OF_MEM;
		}

		queue->count++;
		return OK;
	}

	if(queue->head == queue->tail) {
		queue->tail = _queue_new_node(queue, data);
		if(queue->tail == NULL) {
			queue->tail = queue->head;
			return OUT_OF_MEM;
		}

		queue->head->next = queue->tail;

		queue->count++;
		return OK;
	}

	queue->tail->next = _queue_new_node(queue, data);
	if(queue->tail->next == NULL) {
		return OUT_OF_MEM;
	}

	queue->tail = queue->tail->next;

	queue->count++;
	return OK;
}


// erase the element at the head of the queue
clib_exit_code_t queue_pop(queue_t* queue) {
	if(queue == NULL) {
		return NULL_REF;
	}

	if(queue->head == NULL) {
		return OK;
	}

	queue->count--;

	if(queue->head == queue->tail) {
		_queue_free_node(queue->head);
		free(queue->head);

		queue->head = queue->tail = NULL;

		return OK;
	}

	queue_node_t* node = queue->head;
	queue->head = queue->head->next;

	_queue_free_node(node);
	free(node);

	return OK;
}


// get the head of the queue
const void* queue_head(queue_t* queue) {
	if(queue == NULL || queue->head == NULL) {
		return NULL;
	}

	return queue->head->data;
}