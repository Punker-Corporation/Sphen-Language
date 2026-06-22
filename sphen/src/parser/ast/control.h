#ifndef SPHEN_AST_CONTROL_H
#define SPHEN_AST_CONTROL_H

#include "common.h"

typedef struct {
	AST_block block;
	Node_id cond;
} AST_while;

typedef struct {
	AST_block block;
	Node_id cond;
} AST_do_while;

typedef struct {
	AST_block then_block;
	Node_id else_block;
	Node_id cond;
} AST_if;

typedef struct {
	AST_block block;
} AST_else;

typedef struct {
	Node_id expr;
} AST_return;

typedef struct {
	AST_gap vars;
	AST_gap iterator;
	AST_gap steps;
	AST_block body;
} AST_for;

#endif
