#ifndef SPHEN_AST_MATCH_H
#define SPHEN_AST_MATCH_H

#include "common.h"


typedef enum {
    MATCH_NULL=0,
    MATCH_LITERAL,
    MATCH_IDENT,
    MATCH_RANGE,
    MATCH_OR,
    MATCH_IF,
    MATCH_DEFAULT,
} Pattern_match_k;


typedef struct {
    Pattern_match_k kind;

    union {
        Node_id literal;

        struct {
            Node_id left;
            Node_id right;
        } or_pattern;

        struct {
            Node_id start;
            Node_id end;
            bool inclusive;
        } range;

        Node_id condition;
    };
} Match_Pattern;
typedef struct {
	Match_Pattern pattern;
	AST_block body;
} AST_case;
typedef struct {
	Node_id expr;
	AST_gap body;
} AST_match;



#endif
