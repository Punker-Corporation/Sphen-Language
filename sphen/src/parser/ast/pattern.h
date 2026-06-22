#ifndef SPHEN_AST_PATTERN_H
#define SPHEN_AST_PATTERN_H

#include "common.h"

typedef enum {
    PAT_IDENT=0,
    PAT_TYPED,
    PAT_TUPLE
} Pattern_k;

typedef struct AST_pattern {
    Pattern_k type;
    union {
        struct {
            char* name;
        } ident;

        struct {
            char* name;
            Node_id type;
        } typed;

        struct {
            struct Pattern** items;
            unsigned long int len;
        } tuple;
    };
} AST_pattern;

typedef struct {
	bool is_const;
	Node_id type;
	char* name;
	Node_id value;
} AST_var;

typedef struct {
    char* name;
} AST_var_call;


#endif
