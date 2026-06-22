#ifndef SPHEN_AST_EXPR_H
#define SPHEN_AST_EXPR_H

#include "common.h"


typedef enum {
	EXPR_UNKNOWN = 0, EXPR_INTEGER,
	EXPR_FLOATING, EXPR_STRING,
	EXPR_BOOLEAN, EXPR_TUPLE
} Expr_res;
typedef enum {
    PREC_NONE = 0,
    PREC_ASSIGN,     // = += -= /= *=
    PREC_OR,         // ||
    PREC_AND,        // &&
    PREC_EQUALITY,   // == !=
    PREC_COMPARE,    // < > <= >=
    PREC_TERM,       // + -
    PREC_FACTOR,     // * / %
    PREC_UNARY,      // not -
    PREC_PRIMARY
} AST_precedence;

typedef struct {
	Node_id left;
	Token_k op;
	Node_id right;
	Expr_res res;
} AST_expr;

#endif
