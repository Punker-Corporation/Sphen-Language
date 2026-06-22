#ifndef SPHEN_AST_COMMON_H
#define SPHEN_AST_COMMON_H

#include "../../helpers/types.h"
#include "../../lexer/tokens.h"

typedef enum {
	AST_UNKNOWN = 0,
	AST_BLOCK,
	AST_FILE,

    AST_LET_DECL, AST_CONST_DECL, AST_ASSIGN,
    AST_FN_DECL, AST_FN_PARAM,
	AST_CLASS_DECL, AST_OBJ_CONSTRUCT_DECL, AST_OBJ_DESTRUCT_DECL,
	AST_STRUCT_DECL, AST_OPERATOR_DECL,
	
    AST_VAR_CALL,
    AST_FN_CALL,

    AST_IF_STMT, AST_ELIF_STMT, AST_ELSE_STMT,
    AST_FOR_STMT, AST_FOR_ITERATOR, AST_FOR_STEP_STMT,
    AST_WHILE_STMT, AST_DOWHILE_STMT,
    AST_MATCH_STMT, AST_CASE_STMT, AST_CASE_IF_STMT, AST_CASE_EXPR,
    AST_RETURN_STMT, AST_BREAK_STMT, AST_CONTINUE_STMT,
    AST_IMPORT_STMT, AST_EXPORT_STMT,

    AST_LITERAL,
    AST_EXPR, AST_TYPE,
    AST_TUPLE_EXPR, AST_TUPLE_TYPE,
    AST_UNARY,
    AST_RETURN
} AST_k;

typedef size_t Node_id;

typedef struct {
	size_t begin;
	size_t count;
	Node_id last;
} AST_block;

typedef struct {
	size_t begin;
	size_t count;
} AST_gap;

#endif
