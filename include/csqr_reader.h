#ifndef CSQR_READER
#define CSQR_READER

#include "csqr_utils.h"

typedef struct csqr_obj_s csqr_obj_t;
typedef struct csqr_obj_type_t csqr_obj_type_t;

typedef struct expresion_s expresion_t;

typedef struct program_tree_s program_tree_t;
typedef struct program_s program_t;

typedef struct word_trie_s word_trie_t;
typedef struct trie_node_s trie_node_t;
typedef struct trie_node_end_s trie_node_end_t;


// function prot
typedef void (*init_func)(csqr_obj_t*);
typedef void (*free_func)(csqr_obj_t*);
typedef void (*operator_func)(csqr_obj_t*);
typedef void (*conversion_func)(csqr_obj_t*);



COMP_ERROR create_program(FILE* file, program_t* out);

COMP_ERROR create_expresion_tree(program_t* program, char* expresion, unsigned int l, unsigned int r, expresion_t* out);

COMP_ERROR delete_expresion_tree(expresion_t* expresion);

#endif