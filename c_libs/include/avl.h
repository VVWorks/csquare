/* ================================= 
Copyright (C) 2023 Vornicescu Vasile
An AVL tree implementation in C
*/


#ifndef CAVL_H
#define CAVL_H

#include "stdlib.h"
#include "string.h"

typedef struct avl_node_s {
	struct avl_node_s *left;
	struct avl_node_s *right;
	void* key;
	void* data;
	int height;
} avl_node_t;

typedef struct avl_tree_s {
	avl_node_t* root;
	unsigned int data_size;
	unsigned int key_size;
	unsigned int count;
	int (*comparation)(const void*, const void*);
} avl_tree_t;


// initialize an empty avl tree
avl_tree_t* avl_tree_init(unsigned int data_size, unsigned int key_size, int (*comparation)(const void*, const void*));


// count the number of elements in the tree
int avl_tree_count(avl_tree_t* tree);


// empty the memory of an avl tree
void avl_tree_delete(avl_tree_t* tree);


// insert an element in the avl tree
void avl_tree_insert(avl_tree_t* tree, void* key, void* data);


// get the minimum value key in the tree
const void* avl_min_key(avl_tree_t* tree);


// get the maximum value key in the tree
const void* avl_max_key(avl_tree_t* tree);


// erase a specific key from the tree
void avl_tree_erase(avl_tree_t* tree, void* key);


// search a specific key in the tree
const void* avl_tree_search(avl_tree_t* tree, void* key);


// copy the structure of the tree
avl_tree_t* avl_tree_copy(avl_tree_t* tree);


// clears all elements from the tree
void avl_tree_clear(avl_tree_t* tree);


// swaps all the data between two different trees
void avl_tree_swap(avl_tree_t* a, avl_tree_t* b);


// return the first element bigger or equal to given key
const void* avl_lower_bound(avl_tree_t* tree, void* key);

#endif