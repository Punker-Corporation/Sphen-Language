#ifndef SPHEN_AST_H
#define SPHEN_AST_H


#include "ast/common.h"
#include "ast/expr.h"
#include "ast/data.h"
#include "ast/control.h"
#include "ast/pattern.h"
#include "ast/function.h"
#include "ast/match.h"
#include "ast/generics.h"


typedef struct {
	Node_id* data;
	size_t len, cap;
	Arena* arena;
} Vec_id;
typedef struct {
	char* path;
} AST_file;


typedef struct {
    AST_k kind;
    Node_id id;

    union {
		AST_file file;

		AST_param param;
		AST_func func;
    	AST_fn_call fn_call;

		AST_expr expr;
		
		AST_literal literal;
		AST_data_type data_type;
		AST_tuple tuple;

		AST_var var;
		AST_pattern var_decl;
    	AST_var_call var_call;
		
		AST_while while_st;
		AST_do_while dowhile_st;
		AST_if if_st;
		AST_else else_st;
		AST_for for_st;
		AST_return return_st;
		AST_match match_st;
		AST_case case_st;
    };
} AST_node;


typedef struct {
	AST_node* data;
	size_t len, cap;
} AST_vec;

extern int newastvec(AST_vec*);
extern AST_node getnode(AST_vec*, Node_id);
extern int pushnode(AST_vec*, const AST_node);
extern int delast(AST_vec*);

extern int newvecid(Vec_id*, Arena*, size_t);
extern Node_id getvecid(Vec_id*, size_t);
extern void pushvecid(Vec_id*, Node_id);
extern int delvecid(Vec_id*);

#endif
