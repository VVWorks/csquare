#include "../include/csqr_reader.h"



// STRUCTS

struct trie_node_s {
	char key;

	trie_node_t** child;
	int child_count;
	int child_max;

	trie_node_end_t* is_end_of_word;
	// NULL if not
};


struct trie_node_end_s {
	char word_type;
	// 0 - not a word
	// 1 - keyword
	// 2 - variable name
	// 3 - operator
	// 4 - function name

	csqr_obj_t* object;
	// is not NULL if word_type == 2 or 4

	unsigned int id;
	// used as operator id if word_type == 3
	// used as keyword id if word_type == 1
	// used as variable id if word_type = 2
	// not used otherwise
};


struct word_trie_s {
	trie_node_t* root;
	unsigned int word_count;
};


// data type info
struct csqr_obj_type_s {
	size_t data_size;
	unsigned int id;

	program_t* program;

	init_func init_obj;
	free_func free_obj;
	operator_func apply_operator[OPERATOR_COUNT];
	conversion_func *convert_obj;
};


// run time object
struct csqr_obj_s {
	void* data;
	csqr_obj_type_t* type;
	unsigned int object_id;
	unsigned int scope_id;
};


// binary expresion tree
struct expresion_s {
	csqr_obj_t* object;
	char operator_id;
	expresion_t* left;
	expresion_t* right;
};


// tree of operations
struct program_tree_s {

};


// program to interpret
struct program_s {
	unsigned int data_type_count;

	csqr_obj_t** objects;
	expresion_t** expresions;

	int curr_obj_count;
	int max_obj_count;
};


// WORD TRIE

trie_node_t* trie_create_node(char key, int child_capacity, trie_node_end_t* is_end_of_word) {
	trie_node_t* node = malloc(sizeof(trie_node_t));
	if (!node) {
		return NULL;
	}

	node->key = key;
	node->is_end_of_word = is_end_of_word;
	node->child_count = 0;
	node->child_max = child_capacity;

	node->child = malloc(child_capacity * sizeof(trie_node_t*));
	if (!node->child) {
		free(node);
		return NULL;
	}

	return node;
}

// 0 - succes, -1 - error
int trie_node_add_child(trie_node_t* node, trie_node_t* child) {
	if (node->child_count == node->child_max) {
		trie_node_t** tmp = realloc(node->child, (node->child_max * 2) * sizeof(trie_node_t*));
		if (!tmp) {
			return -1;
		}

		node->child_max *= 2;

		node->child = tmp;
	}

	node->child[node->child_count++] = child;

	return 0;
}

word_trie_t* trie_create() {
	word_trie_t* trie = malloc(sizeof(word_trie_t));
	if (!trie) {
		return NULL;
	}

	trie->root = trie_create_node(0, 26, NULL);
	if (!trie->root) {
		free(trie);
		return NULL;
	}

	trie->word_count = 0;

	return trie;
}

// 1 - already in trie, 0 - succes, -1 - error
int trie_insert(word_trie_t* trie, char* string, unsigned int l, unsigned int r, trie_node_end_t* word_type) {
	if (!trie || !string)
		return -1;

	trie_node_t* curr = trie->root;

	for (int i = l; i <= r; i++) {
		char found = 0;
		for (int j = 0; j < curr->child_count; j++) {
			if (curr->child[j]->key == string[i]) {
				found = 1;
				curr = curr->child[j];
			}
		}

		if (!found) {
			trie_node_t* new_node = trie_create_node(string[i], 4, NULL);
			if (!new_node) {
				return -1;
			}

			if (trie_node_add_child(curr, new_node)) {
				return -1;
			}

			curr = new_node;
		}

		if (i == r) {
			if (curr->is_end_of_word) {
				return 1;
			}

			curr->is_end_of_word = word_type;
		}
	}

	return 0;
}


// returns  pointer to given node if found
trie_node_t* trie_search(word_trie_t* trie, char* string, unsigned int l, unsigned int r) {
	if (!trie || !string) {
		return NULL;
	}

	trie_node_t* curr = trie->root;

	for (int i = l; i <=r; i++) {
		char found = 0;
		for (int j = 0; j < curr->child_count; j++) {
			if (curr->child[j]->key == string[i]) {
				found = 1;
				curr = curr->child[j];
			}
		}

		if (!found) {
			return NULL;
		}
	}

	if (!(curr->is_end_of_word)) {
		return NULL;
	}

	return curr;
}

void _trie_node_del(trie_node_t* node) {
	if (!node)
		return;

	if (node->is_end_of_word) {
		free(node->is_end_of_word);
	}

	for(int i = 0; i < node->child_count; i++) {
		_trie_node_del(node->child[i]);
	}

	if (node->child)
		free(node->child);
	free(node);
}

void trie_delete(word_trie_t* trie) {
	if (!trie) {
		return;
	}

	_trie_node_del(trie->root);

	free(trie);
}

void trie_print(trie_node_t* root, char* curr, int len) {
	if (root->is_end_of_word) {
		curr[len] = '\0';
		printf("<%s>\n", curr);
	}

	for (int i = 0; i < root->child_count; i++) {
		curr[len] = root->child[i]->key;
		trie_print(root->child[i], curr, len + 1);
	}
}

// CSQR READER

void _clean_up_to_now(char** lines, int curr_line) {
	for (int i = 0; i <= curr_line; i++) {
		free(lines[i]);
	}
	free(lines);
}

COMP_ERROR create_program(FILE* file, program_t* out) {
	if (!file)
		return INTERNAL_ERROR;

	// break into words

	int curr_line = 0;
	int max_lines = 2;
	char** lines = malloc(max_lines * sizeof(char*));
	if (!lines) {
		return INTERNAL_ERROR;
	}

	int curr_char = 0;
	int max_chars = 2;
	lines[0] = malloc(max_chars * sizeof(char));
	if (!lines[0]) {
		free(lines);
		return INTERNAL_ERROR;
	}

	char last_c = 0;
	char c = 0;

	while ((c = fgetc(file)) != EOF) {
		if (curr_char == max_chars) {
			char* tmp = realloc(lines[curr_line], (max_chars *= 2) * sizeof(char));
			if (!tmp) {
				_clean_up_to_now(lines, curr_line);
				return INTERNAL_ERROR;
			}
			lines[curr_line] = tmp;
		}

		last_c = c;
		lines[curr_line][curr_char] = c;

		if (c == '\n') {
			lines[curr_line][curr_char] = '\0';
			curr_line++;
			curr_char = -1;
			max_chars = 2;

			if (curr_line == max_lines) {
				char** tmp = realloc(lines, (max_lines *= 2) * sizeof(char*));
				if (!tmp) {
					_clean_up_to_now(lines, curr_line);
					return INTERNAL_ERROR;
				}
				lines = tmp;
			}

			lines[curr_line] = malloc(max_chars * sizeof(char));
			if (!lines[curr_line]) {
				_clean_up_to_now(lines, curr_line - 1);
				return INTERNAL_ERROR;
			}
		}

		curr_char++;
	}

	lines[curr_line][curr_char] = '\0';
	if (last_c != '\n') {
		curr_line++;
	} else {
		free(lines[curr_line]);
	}

	char** tmp = realloc(lines, curr_line * sizeof(char*));
	if (!tmp) {
		_clean_up_to_now(lines, curr_line - 1);
		free(lines);
		return INTERNAL_ERROR;
	}
	lines = tmp;
	max_lines = curr_line;

	// At this point lines stores {curr_lines};

	//DEBUG
#ifdef DEBUG
	printf("DEBUG:\n");
	printf("	Lines: <%d>\n", curr_line);
	for(int i = 0; i < curr_line; i++) {
		printf("	Line #%d:<%s>\n", i, lines[i]);
	}
	printf("\n");
#endif
	// DEBUG

	word_trie_t* trie = trie_create();
	if (!trie) {
		_clean_up_to_now(lines, curr_line - 1);
		free(lines);
		return INTERNAL_ERROR;
	}

	// adding keywords to the word trie

	// createing objects and expresion trees

	//int scope_id = 0;
	//int space_count = 0;
	//int l = 0, r = 0;
	for (int i = 0; i < curr_line; i++) {
		int spaces = 0;
		char* this_line = lines[i];
		int j = 0;

		while (this_line[j] != ' ') {
			spaces++;
			j++;
		}
	}

	return SUCCES;
}

COMP_ERROR create_expresion_tree(program_t* program, char* expresion, unsigned int l, unsigned int r, expresion_t* out) {
	if (expresion == NULL)
		return INTERNAL_ERROR;

	int open = 0;
	int close = 0;
	int first = -1;
	int last = -1;
	int n = strlen(expresion);

	if (l > r || r >= n)
		return INTERNAL_ERROR;

	for (int i = l; i <= r; i++) {
		if (expresion[i] == '(') {
			open++;
			if (first == -1)
				first = i;
		}
		if (expresion[i] == ')') {
			close++;
			last = i;
		}
		if (close > open) {
			return MISSING_PARENTHESES;
		}
	}

	if (open != close)
		return MISSING_PARENTHESES;

	expresion_t* parentheses = NULL;
	if (open > 0) {
		COMP_ERROR comp = create_expresion_tree(program, expresion, first + 1, last - 1, parentheses);
		if (comp) {
			if (parentheses)
				delete_expresion_tree(parentheses);
			return comp;
		}

		if (!parentheses)
			return INTERNAL_ERROR;
	}

	// transform each element into an expresion node

	return SUCCES;
}

COMP_ERROR delete_expresion_tree(expresion_t* expresion) {
	return SUCCES;
}