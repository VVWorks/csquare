/* ================================= 
Copyright (C) 2023 Vornicescu Vasile
Two binary heap implementations in C
One may work better on some systems then the other
*/


#include "../include/binary_heap.h"


// Implementation #1 ==================================================================================


clib_exit_code_t inc_cap(bheap_t* heap, size_t count) {
	if(count == 0)
		return OK;

	void* tmp = realloc(heap->data, (count + heap->capacity) * heap->data_size);
	if(tmp == NULL)
		return OUT_OF_MEM;

	heap->data = tmp;
	heap->capacity = count + heap->capacity;

	return OK;
}

bheap_t* bheap_init(size_t capacity, size_t data_size, uint8_t (*compare)(const void*, const void*)) {
	bheap_t* heap = malloc(sizeof(*heap));
	if(heap == NULL) {
		printf("\n[bheap2->bheap_init]: Not enough memory to create heap!\n");
		return NULL;
	}

	capacity = _max(capacity, 1);

	heap->data = calloc(capacity, data_size);
	if(heap->data == NULL) {
		printf("\n[bheap2->bheap_init]: Not enough memory to create heap!\n");
		free(heap);
		return NULL;
	}

	heap->count = 0;
	heap->data_size = data_size;
	heap->compare = compare;

	return heap;
}


clib_exit_code_t bheap_delete(bheap_t* heap) {
	if(heap == NULL) {
		printf("\n[bheap2->bheap_delete]: Null reference to heap!\n");
		return NULL_REF;
	}

	if(heap->data)
		free(heap->data);

	free(heap);

	return OK;
}

clib_exit_code_t bheap_clear(bheap_t* heap) {
	if(heap == NULL) {
		printf("\n[bheap2->bheap_clear]: Null reference to heap!\n");
		return NULL_REF;
	}

	if(heap->data) {
		free(heap->data);
		heap->data = calloc(1, heap->data_size);

		if(heap->data == NULL) {
			printf("\n[bheap2->bheap_clear]: Could not reinitialize the heap! Deleting it!\n");
			bheap_delete(heap);
			return UNSAFE;
		}
	}

	heap->capacity = 1;
	heap->count = 0;

	return OK;
}


clib_exit_code_t bheap_swap(bheap_t* heap_a, bheap_t* heap_b) {
	if(heap_a == NULL || heap_b == NULL) {
		printf("\n[bheap2->bheap_swap]: Null reference to heap!\n");
		return NULL_REF;
	}

	void* a_data = heap_a->data;
	size_t a_cap = heap_a->capacity;
	size_t a_count = heap_a->count;
	size_t a_data_size = heap_a->data_size;

	heap_a->data = heap_b->data;
	heap_a->capacity = heap_b->capacity;
	heap_a->count = heap_b->count;
	heap_a->data_size = heap_b->count;

	heap_b->data = a_data;
	heap_b->capacity = a_cap;
	heap_b->count = a_count;
	heap_b->data_size = a_data_size;

	return OK;
}


bheap_t* bheap_copy(bheap_t* heap){
	if(heap == NULL) {
		printf("\n[bheap2->bheap_copy]: Null reference to heap! Returning NULL.\n");
		return NULL;
	}

	bheap_t* copy = bheap_init(heap->capacity, heap->data_size, heap->compare);
	if(copy == NULL){
		printf("\n[bheap2->bheap_copy]: Not enough memory to create heap! Returning NULL.\n");
		return NULL;
	}

	for(size_t i = 0; i < heap->count; i++) {
		memcpy((char*)(copy->data) + i * heap->data_size, (char*)(heap->data) + i * heap->data_size, heap->data_size);
	}

	return copy;
}


clib_exit_code_t bheap_insert(bheap_t* heap, void* data) {
	if(heap == NULL || data == NULL) {
		printf("\n[bheap2->bheap_insert]: Null reference to heap or data.\n");
		return NULL_REF;
	}
 
	if(heap->count + 1 > heap->capacity) {
		clib_exit_code_t err = inc_cap(heap, 2);
		if(err != OK){
			printf("\n[bheap2->bheap_insert]: Not enough memory to create heap!\n");
			return err;
		}
	}
 
	memcpy((char*)heap->data + heap->count * heap->data_size, data, heap->data_size);
	heap->count++;
 
	size_t it = heap->count - 1;
	void* tmp = malloc(heap->data_size);
	if(tmp == NULL) {
		printf("\n[bheap2->bheap_insert]: Not enough memory to performe action!\n");
		heap->count--;
		return OUT_OF_MEM;
	}
 
	while(it != 0 && heap->compare((void*)((char*)heap->data + ((it - 1) / 2) * heap->data_size), (void*)((char*)heap->data + it * heap->data_size))) {
		memcpy(tmp, (char*)(heap->data) + it * heap->data_size, heap->data_size);
		memcpy((char*)(heap->data) + it * heap->data_size, (char*)(heap->data) + ((it - 1) / 2) * heap->data_size, heap->data_size);
		memcpy((char*)(heap->data) + ((it - 1) / 2) * heap->data_size, tmp, heap->data_size);
		it = (it - 1) / 2;
	}
 
	return OK;
}


clib_exit_code_t bheap_pop(bheap_t* heap) {
	if(heap == NULL) {
		printf("\n[bheap2->bheap_pop]: Null reference to heap!\n");
		return NULL_REF;
	}
 
	if(heap->count <= 1){
		heap->count = 0;
		return OK;
	}
 	
	void* tmp = malloc(heap->data_size);
	if(tmp == NULL){
		printf("\n[bheap2->bheap_pop]: Not enough memory to performe action!\n");
		return OUT_OF_MEM;
	}
	memcpy(tmp, heap->data, heap->data_size);
	memcpy(heap->data, (char*)heap->data + (heap->count - 1) * heap->data_size, heap->data_size);
	memcpy((char*)heap->data + (heap->count - 1) * heap->data_size, tmp, heap->data_size);
	heap->count--;
 
	size_t it = 0;
 
	while(it < heap->count){
		size_t left = 2 * it + 1;
		size_t right = 2 * it + 2;
 
		memcpy(tmp, (char*)heap->data + it * heap->data_size, heap->data_size);
		size_t next = it;
 
		if(left < heap->count && heap->compare(tmp, (void*)((char*)heap->data + left * heap->data_size))) {
			memcpy(tmp, (char*)heap->data + left * heap->data_size, heap->data_size);
			next = left;
		}
		if(right < heap->count && heap->compare(tmp, (void*)((char*)heap->data + right * heap->data_size))) {
			memcpy(tmp, (char*)heap->data + right * heap->data_size, heap->data_size);
			next = right;
		}
 
		if(next == it) {
			break;
		}
 
		memcpy((char*)heap->data + next * heap->data_size, (char*)heap->data + it * heap->data_size, heap->data_size);
 		memcpy((char*)heap->data + it * heap->data_size, tmp, heap->data_size);

		it = next;
	}
 
	return OK;
}


const void* bheap_get_top(bheap_t* heap) {
	if(heap == NULL){
		printf("\n[bheap2->bheap_insert]: Null reference to heap! Returning NULL.\n");
		return NULL;
	}

	if(heap->data && heap->count)
		return heap->data;
	return NULL;
}


clib_exit_code_t bheap_sort(void* array, size_t elem_size, size_t begin, size_t end, uint8_t (*compare)(const void*, const void*)) {
	if(array == NULL || begin > end || compare == NULL) {
		return NULL_REF;
	}

	size_t num = end - begin + 1;
	bheap_t* heap = bheap_init(num, elem_size, compare);
	if(heap == NULL) {
		return OUT_OF_MEM;
	}

	for(size_t i = begin; i <= end; i++) {
		bheap_insert(heap, (void*)((char*)array + i * elem_size));
	}

	for(size_t i = begin; i <= end; i++) {
		const void* top = bheap_get_top(heap);
		memcpy((char*)array + i * elem_size, top, elem_size);
		bheap_pop(heap);
	}

	bheap_delete(heap);

	return OK;
}

// Implementation #2 ==================================================================================


clib_exit_code_t inc_cap2(bheap2_t* heap, size_t factor){
	factor = _max(factor, 1);
 
	if(0 == heap->capacity){
		heap->data = malloc(factor * sizeof(void*));
		if(NULL == heap->data){
			return OUT_OF_MEM;
		}
 
		for(int i = 0; i < factor; i++){
			heap->data[i] = malloc(sizeof(heap->data_size));
			if(NULL == heap->data[i]){
				for(int j = 0; j <= i; j++){
					free(heap->data[i]);
				}
				free(heap->data);
				heap->data = NULL;
 
				return OUT_OF_MEM;
			}
		}
 
		heap->capacity = factor;
 
		return OK;
	}
 
	void** new_array = malloc(factor * heap->capacity * sizeof(void*));
	if(NULL== new_array){
		return OUT_OF_MEM;
	}
 
	for(int i = 0; i < heap->capacity; i++){
		new_array[i] = heap->data[i];
	}
 
	for(int i = heap->capacity; i < factor * heap->capacity; i++){
		new_array[i] = malloc(sizeof(heap->data_size));
		if(NULL == new_array[i]){
			for(int j = 0; j <= i; j++){
				free(new_array[i]);
			}
			free(new_array);
 
			return OUT_OF_MEM;
		}
	}
 
	free(heap->data);
	heap->data = new_array;
 
	heap->capacity = factor * heap->capacity;
 
	return OK;
}


bheap2_t* bheap2_init(size_t capacity, size_t data_size, uint8_t (*compare)(const void*, const void*)){
	if(0 == data_size){
		return NULL;
	}
 
	bheap2_t* heap = malloc(sizeof(*heap));	
	if(NULL == heap){
		return NULL;
	}
 
	heap->data_size = data_size;
	heap->capacity = 0;
	if(capacity != 0){
		clib_exit_code_t err = inc_cap2(heap, capacity);
		if(err != OK){
			free(heap);
			heap = NULL;
			return NULL;
		}
	}
	heap->count = 0;
	heap->compare = compare;
 
	return heap;
}
 
clib_exit_code_t bheap2_delete(bheap2_t* heap){
	if(NULL == heap){
		return NULL_REF;
	}
 
	for(size_t i = 0; i < heap->capacity; i++){
		free(heap->data[i]);
		heap->data[i] = NULL;
	}
 
	free(heap->data);
	heap->data = NULL;
 
	free(heap);
	heap = NULL;
 
	return OK;
}

clib_exit_code_t bheap2_clear(bheap2_t* heap) {
	if(heap == NULL) {
		return NULL_REF;
	}

	for(size_t i = 0; i < heap->capacity; i++){
		free(heap->data[i]);
		heap->data[i] = NULL;
	}

	free(heap->data);
	heap->data = NULL;

	heap->count = 0;
	heap->capacity = 0;

	return OK;
}
 
bheap2_t* bheap2_copy(bheap2_t* heap){
	bheap2_t* copy = bheap2_init(heap->data_size, heap->capacity, heap->compare);
	if(NULL == copy){
		return NULL;
	}
 
	for(size_t i = 0; i < heap->count; i++){
		memcpy(copy->data[i], heap->data[i], heap->data_size);
	}
 
	copy->count = heap->count;
 
	return copy;
}
 
clib_exit_code_t bheap2_swap(bheap2_t* heap_a, bheap2_t* heap_b){
	if(NULL == heap_a || NULL == heap_b){
		return NULL_REF;
	}
 
	size_t data_size_a = heap_a->data_size;
	size_t count_a = heap_a->count;
	size_t capacity_a = heap_a->capacity;
	uint8_t	(*comp_a)(const void*, const void*) = heap_a->compare;
	void** arr_a = heap_a->data;
 
	heap_a->data_size = heap_b->data_size;
	heap_a->count = heap_b->count;
	heap_a->capacity = heap_b->capacity;
	heap_a->compare = heap_b->compare;
	heap_a->data = heap_b->data;
 
	heap_b->data_size = data_size_a;
	heap_b->count = count_a;
	heap_b->capacity = capacity_a;
	heap_b->compare = comp_a;
	heap_b->data = arr_a;
 
	return OK;
}
 
clib_exit_code_t bheap2_insert(bheap2_t* heap, void* data){
	if(NULL == heap || NULL == data){
		return NULL_REF;
	}
 
	if(heap->count + 1 > heap->capacity){
		clib_exit_code_t err = inc_cap2(heap, 2);
		if(err != OK){
			return err;
		}
	}
 
	memcpy(heap->data[heap->count], data, heap->data_size);
	heap->count++;
 
	size_t it = heap->count - 1;
 
	while(it != 0 && heap->compare(heap->data[(it - 1) / 2], heap->data[it]) ) {
		void* tmp = heap->data[it];
		heap->data[it] = heap->data[(it - 1) / 2];
		heap->data[(it - 1) / 2] = tmp;
		it = (it - 1) / 2;
	}
 
	return OK;
}
 
clib_exit_code_t bheap2_pop(bheap2_t* heap){
	if(NULL == heap){
		return NULL_REF;
	}
 
	if(heap->count <= 1){
		heap->count = 0;
		return OK;
	}
 
	void* tmp = heap->data[0];
	heap->data[0] = heap->data[heap->count - 1];
	heap->data[heap->count - 1] = tmp;
	heap->count--;
 
	size_t it = 0;
 
	while(it < heap->count){
		size_t left = 2 * it + 1;
		size_t right = 2 * it + 2;
 
		void* minimal = heap->data[it];
		size_t next = it;
 
		if(left < heap->count && heap->compare(minimal, heap->data[left])){
			minimal = heap->data[left];
			next = left;
		}
		if(right < heap->count && heap->compare(minimal, heap->data[right])){
			minimal = heap->data[right];
			next = right;
		}
 
		if(next == it){
			break;
		}
 
		heap->data[next] = heap->data[it];
		heap->data[it] = minimal;
 
		it = next;
	}
 
	return OK;
}
 
void* const	bheap2_get_top(bheap2_t* heap){
	if(NULL == heap || heap->count == 0){
		return NULL;
	}
 
	return heap->data[0];
}

// sorts an array of data using heap sort
clib_exit_code_t bheap2_sort(void* array, size_t elem_size, size_t begin, size_t end, uint8_t (*compare)(const void*, const void*)) {
	if(array == NULL || begin > end || compare == NULL) {
		return NULL_REF;
	}

	size_t num = end - begin + 1;
	bheap2_t* heap = bheap2_init(num, elem_size, compare);
	if(heap == NULL) {
		return OUT_OF_MEM;
	}

	for(size_t i = begin; i <= end; i++) {
		bheap2_insert(heap, (void*)((char*)array + i * elem_size));
	}

	for(size_t i = begin; i <= end; i++) {
		const void* top = bheap2_get_top(heap);
		memcpy((char*)array + i * elem_size, top, elem_size);
		bheap2_pop(heap);
	}

	bheap2_delete(heap);

	return OK;
}