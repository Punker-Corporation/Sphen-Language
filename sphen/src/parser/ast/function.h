#ifndef SPHEN_AST_FUNCTION_H
#define SPHEN_AST_FUNCTION_H

#include "common.h"


typedef struct {
	bool is_const;
	Node_id type;
	char* name;
	Node_id std_value;
} AST_param;
typedef struct {
	char* name;
	AST_gap param;
	Node_id ret_type;
	AST_block block;
} AST_func;

typedef struct {
    char* name;
    AST_gap args;
    unsigned long int arg_count;
} AST_fn_call;

#endif
