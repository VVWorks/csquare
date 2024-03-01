#include "./../include/avl.h"

avl_tree_t* avl_tree_init(unsigned int data_size, unsigned int key_size, int (*comparation)(const void*, const void*)) {
	avl_tree_t* tree = (avl_tree_t*)malloc(sizeof(avl_tree_t));
	if(tree == NULL)
		return NULL;

	tree->count = 0;
	tree->root = NULL;
	tree->data_size = data_size;
	tree->key_size = key_size;
	tree->comparation = comparation;

	return tree;
}

int avl_tree_count(avl_tree_t* tree) {
	if(tree)
		return tree->count;
	return 0;
}

void _avl_free_node(avl_node_t* node) {
	if(node) {
		free(node->data);
		free(node->key);
		free(node);
	}
}

void _avl_node_delete(avl_node_t* node) {
	if(node) {
		_avl_node_delete(node->left);
		_avl_node_delete(node->right);

		free(node->data);
		free(node->key);
		free(node);
	}
}

void avl_tree_delete(avl_tree_t* tree) {
	if(tree) {
		_avl_node_delete(tree->root);
		free(tree);
	}
}

int _avl_node_height(avl_node_t *node) {
	if(node == NULL)
		return 0;

	return node->height;
}

void _avl_node_update_height(avl_node_t *node) {
	if(node == NULL)
		return;
	int left = _avl_node_height(node->left);
	int right = _avl_node_height(node->right);

	node->height = (left > right ? left : right) + 1;
}

int _avl_node_balance_factor(avl_node_t* node) {
	if(node == NULL)
		return 0;

	return (_avl_node_height(node->left) - _avl_node_height(node->right));
}

avl_node_t* _avl_node_rotate_left(avl_node_t* node) {
	avl_node_t *y = node->right;
    avl_node_t *tmp = y->left;

    y->left = node;
    node->right = tmp;

    _avl_node_update_height(node);
	_avl_node_update_height(y);
 
    return y;
}

avl_node_t* _avl_node_rotate_right(avl_node_t* node) {
	avl_node_t *x = node->left;
    avl_node_t *tmp = x->right;
 
    x->right = node;
    node->left = tmp;
 
    _avl_node_update_height(node);
	_avl_node_update_height(x);
 
    return x;
}

avl_node_t* _avl_node_new(avl_tree_t* tree, void* key, void* data) {
	avl_node_t* new_node = (avl_node_t*)malloc(sizeof(avl_node_t));
	if(new_node == NULL)
		return NULL;

	new_node->key = malloc(tree->key_size);
	if(new_node->key == NULL) {
		free(new_node);
		return NULL;
	}

	new_node->data = malloc(tree->data_size);
	if(new_node->data == NULL) {
		free(new_node->key);
		free(new_node);
		return NULL;
	}

	memcpy(new_node->data, data, tree->data_size);
	memcpy(new_node->key, key, tree->key_size);
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->height = 1;

	return new_node;
}

avl_node_t* _avl_node_insert(avl_tree_t* tree, avl_node_t* node, void* key, void* data) {
	if(node == NULL) {
		tree->count++;
		return _avl_node_new(tree, key, data);
	}

	int diff = tree->comparation(key, node->key);

	if (diff < 0)
		node->left = _avl_node_insert(tree, node->left, key, data);
	else if (diff > 0)
		node->right = _avl_node_insert(tree, node->right, key, data);
	else {
		memcpy(node->data, data, tree->data_size);
		return node;
	}

	_avl_node_update_height(node);

	int balance_factor = _avl_node_balance_factor(node);

	int diff_1 = 0;
	if(balance_factor > 1)
		diff_1 = tree->comparation(key, node->left->key);
	int diff_2 = 0;
	if(balance_factor < -1)
		diff_2 = tree->comparation(key, node->right->key);

	if(balance_factor > 1 && diff_1 < 0)
		return _avl_node_rotate_right(node);

	if(balance_factor < -1 && diff_2 > 0)
		return _avl_node_rotate_left(node);

	if(balance_factor > 1 &&  diff_1 > 0) {
		node->left = _avl_node_rotate_left(node->left);
		return _avl_node_rotate_right(node);
	}

	if(balance_factor < -1 && diff_2 < 0) {
		node->right = _avl_node_rotate_right(node->right);
		return _avl_node_rotate_left(node);
	}

	return node;
}

void avl_tree_insert(avl_tree_t* tree, void* key, void* data) {
	if(tree == NULL)
		return;
	tree->root = _avl_node_insert(tree, tree->root, key, data);
}

avl_node_t* _avl_node_min(avl_node_t* node) {
	if(node == NULL)
		return 0;
	if(node->left)
		return _avl_node_min(node->left);
	return node;
}

avl_node_t* _avl_node_max(avl_node_t* node) {
	if(node == NULL)
		return 0;
	if(node->right)
		return _avl_node_max(node->right);
	return node;
}

const void* avl_min_key(avl_tree_t* tree) {
	if(tree == NULL)
		return 0;
	avl_node_t* node = _avl_node_min(tree->root);
	if(node)
		return node->key;
	else 
		return 0;
}

const void* avl_max_key(avl_tree_t* tree) {
	if(tree == NULL)
		return 0;
	avl_node_t* node = _avl_node_max(tree->root);
	if(node)
		return node->key;
	else
		return 0;
}

avl_node_t* _avl_node_erase(avl_tree_t* tree, avl_node_t* node, void* key) {
	if(node == NULL)
		return NULL;

	int diff = tree->comparation(key, node->key);

	if(diff < 0)
		node->left = _avl_node_erase(tree, node->left, key);
	else if(diff > 0)
		node->right = _avl_node_erase(tree, node->right, key);
	else {
		if((node->left == NULL) || (node->right == NULL)) {
			avl_node_t* next = node->left;
			if(next == NULL)
				next = node->right;

			if(next == NULL) {
				next = node;
				node = NULL;
			} else {
				node->left = next->left;
				node->right = next->right;
				node->height = next->height;
				memcpy(node->key, next->key, tree->key_size);
				memcpy(node->data, next->data, tree->data_size);
			}

			_avl_free_node(next);
			tree->count--;
		} else {
			avl_node_t* right_min_key = _avl_node_min(node->right);

			memcpy(node->key, right_min_key->key, tree->key_size);
			memcpy(node->data, right_min_key->data, tree->data_size);

			node->right = _avl_node_erase(tree, node->right, right_min_key->key);
		}
	}

	if(node == NULL)
		return NULL;

	_avl_node_update_height(node);

	int balance_factor = _avl_node_balance_factor(node);

	if (balance_factor > 1 && _avl_node_balance_factor(node->left) >= 0)
        return _avl_node_rotate_right(node);

    if (balance_factor < -1 && _avl_node_balance_factor(node->right) <= 0)
        return _avl_node_rotate_left(node);
 
    if (balance_factor > 1 && _avl_node_balance_factor(node->left) < 0) {
        node->left =  _avl_node_rotate_left(node->left);
        return _avl_node_rotate_right(node);
    }
 
    if (balance_factor < -1 && _avl_node_balance_factor(node->right) > 0) {
        node->right = _avl_node_rotate_right(node->right);
        return _avl_node_rotate_left(node);
    }

	return node;
}

void avl_tree_erase(avl_tree_t* tree, void* key) {
	if(tree == NULL)
		return;
	tree->root = _avl_node_erase(tree, tree->root, key);
}

const void* avl_tree_search(avl_tree_t* tree, void* key) {
	if(tree == NULL || key == NULL)
		return NULL;

	avl_node_t* node = tree->root;
	if(node == NULL)
		return NULL;

	int diff = tree->comparation(key, node->key);

	while(diff != 0) {
		if(diff < 0)
			node = node->left;
		else
			node = node->right;
		if(node) 
			diff = tree->comparation(key, node->key);
		else
			return NULL;
	}

	return node->data;
}

avl_node_t* _avl_node_copy(avl_tree_t* tree, avl_node_t* node) {
	if(node == NULL)
		return NULL;

	avl_node_t* copy = _avl_node_new(tree, node->key, node->data);
	if(copy == NULL) {
		return NULL;
	}

	copy->height = node->height;

	copy->left = _avl_node_copy(tree, node->left);
	copy->right = _avl_node_copy(tree, node->right);

	return copy;
}

avl_tree_t* avl_tree_copy(avl_tree_t* tree) {
	avl_tree_t* copy = avl_tree_init(tree->data_size, tree->key_size, tree->comparation);

	copy->root = _avl_node_copy(copy, tree->root);

	copy->count = tree->count;

	return copy;
}

void avl_tree_clear(avl_tree_t* tree) {
	if(tree) {
		_avl_node_delete(tree->root);
		tree->count = 0;
		tree->root = NULL;
	}
}

void avl_tree_swap(avl_tree_t* a, avl_tree_t* b) {
	if(a == NULL || b == NULL)
		return;

	avl_node_t* root_a = a->root;
	unsigned int data_size_a = a->data_size;
	unsigned int key_size_a = a->key_size;
	unsigned int count_a = a->count;
	int (*comparation_a)(const void*, const void*) = a->comparation;

	a->root = b->root;
	a->data_size = b->data_size;
	a->key_size = b->key_size;
	a->count = b->count;
	a->comparation = b->comparation;

	b->root = root_a;
	b->data_size = data_size_a;
	b->key_size = key_size_a;
	b->count = count_a;
	b->comparation = comparation_a;
}


avl_node_t* _avl_lower_bound(avl_tree_t* tree, void* key) {
	if (tree == NULL || key == NULL) {
		return NULL;
	}

	avl_node_t* node = tree->root;
	avl_node_t* result = NULL;
	int diff = 0;

	while (node != NULL) {
		diff = tree->comparation(key, node->key);
		if (diff == 0) {
			return node;
		}

		if (diff < 0) {
			result = node;
			node = node->left;
		} else {
			node = node->right;
		}
	}

	if(result == NULL)
		return NULL;

	return result;
}


// return the first element bigger or equal to given key
const void* avl_lower_bound(avl_tree_t* tree, void* key) {
	avl_node_t* node = _avl_lower_bound(tree, key);
	if (node == NULL) {
		return NULL;
	}

	return node->key;
}