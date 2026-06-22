#ifndef SPHEN_AST_DATA_H
#define SPHEN_AST_DATA_H

#include "common.h"

typedef struct {
	AST_gap values;
	unsigned long int count;
} AST_tuple;

typedef struct {
	Token_value data;
	Token_k type;
} AST_literal;

typedef struct {
	union {
		Node_id typeId;
		Token_k typeP;
	};
	int is_id;
} AST_data_type;

#endif
