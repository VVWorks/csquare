#include "./../include/stack.h"

// initialize an empty stack
stack_t* stack_init(size_t data_size) {
	stack_t* stack = malloc(sizeof(stack_t));
	if (!stack)
		return NULL;

	stack->count = 0;
	stack->root = NULL;
	stack->data_size = data_size;

	return stack;
}


// delete a stack
clib_exit_code_t stack_delete(stack_t* stack) {
	if (!stack) {
		return NULL_REF;
	}

	stack_node_t* node = stack->root;

	while (node != NULL) {
		stack_node_t* now = node->next;
		if (node->data)
			free(node->data);
		free(node);
		node = now;
	}

	free(stack);

	return OK;
}


// swap the memory of 2 stacks
clib_exit_code_t stack_swap(stack_t* a, stack_t* b) {
	stack_node_t* root_a = a->root;
	size_t data_size_a = a->data_size;
	size_t count_a = a->count;

	a->root = b->root;
	a->data_size = b->data_size;
	a->count = b->count;

	b->root = root_a;
	b->data_size = data_size_a;
	b->count = count_a;

	return OK;
}


stack_node_t* _stack_create_node(stack_t* stack, void* data) {
	stack_node_t* node = malloc(sizeof(stack_node_t*));
	if (!node) {
		return NULL;
	}

	node->next = NULL;
	node->data = malloc(stack->data_size);
	if (!node->data) {
		free(node);
		return NULL;
	}

	memcpy(node->data, data, stack->data_size);

	return node;
}


// make an exact copy of a stack
stack_t* stack_copy(stack_t* stack) {
	if (!stack) {
		return NULL;
	}

	stack_t* copy = stack_init(stack->data_size);
	if (!copy) {
		return NULL;
	}

	stack_node_t* curr = stack->root;
	copy->root = _stack_create_node(copy, curr->data);
	stack_node_t* copy_curr = copy->root;
	curr = curr->next;

	while (curr != NULL) {
		copy_curr->next = _stack_create_node(copy, curr->data);
		if (!copy_curr->next) {
			stack_delete(copy);
			return NULL;
		}

		copy_curr = copy_curr->next;
		curr = curr->next;
	}

	copy->count = stack->count;

	return copy;
}


// clear all elements in stack
clib_exit_code_t stack_clear(stack_t* stack) {
	stack_node_t* node = stack->root;

	while (node != NULL) {
		stack_node_t* now = node->next;
		if (node->data)
			free(node->data);
		free(node);
		node = now;
	}

	stack->root = NULL;
	stack->count = 0;

	return OK;
}


// count the number of elements in stack
size_t stack_count(stack_t* stack) {
	if (!stack)
		return 0;
	return stack->count;
}


// push an element at the top of the stack
clib_exit_code_t stack_push(stack_t* stack, void* data) {
	if (!stack || !data)
		return NULL_REF;

	stack_node_t* node = _stack_create_node(stack, data);
	if (!node) {
		return OUT_OF_MEM;
	}

	stack->count++;

	if (!stack->root) {
		stack->root = node;
		return OK;
	}

	node->next = stack->root;
	stack->root = node;

	return OK;
}


// remove the top of the stack
clib_exit_code_t stack_pop(stack_t* stack) {
	if (!stack)
		return NULL_REF;

	if (!stack->root)
		return OK;

	stack_node_t* new_root = stack->root->next;
	free(stack->root->data);
	free(stack->root);
	stack->count--;
	stack->root = new_root;

	return OK;
}


// get the value of the top of the stack
const void* stack_top(stack_t* stack) {
	if (!stack)
		return NULL;

	return stack->root->data;
}