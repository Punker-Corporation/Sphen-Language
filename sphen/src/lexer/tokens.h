#ifndef SPHEN_TOKENS_H
#define SPHEN_TOKENS_H


#include "../helpers/arena.h"
#include "token_list.h"


typedef enum {
	NULL_TOK=0,
#define X(kind, group, name, len) kind,
	TOKEN_LIST
#undef X 
    INTEGER, FLOATING, STRING, CHARACTER,
    ID,
} TOKEN_KIND;

typedef enum {
    UNKNOWN = 0,
    IDENT,
    LITERAL, 
    DATA_TYPE, 
    OPERATOR,
    CONSTRAINT_CAP,
    CONSTRAINT_DATA,
    KEYWORD,
    END_OF_FILE
} TOKEN_GROUP;

typedef union {
	i64_t 		i;		//integer
	f64_t 		f;		//float
	char* 		str;	//string
	u8_t 		c;		//char
	bool		b;		//boolean
} Token_value;

typedef unsigned short int Token_k;
typedef unsigned char Token_group;

typedef struct {
	Token_value data;
	Token_group group;
    Token_k 	kind;
    u32_t col, line;
} Token_t;

typedef struct {
    Token_t* data;
    size_t size, cap;
} Token_vec;

extern bool cmptoken(const Token_t, const Token_t);
extern Token_vec newvec();
extern void vecdel(Token_vec*);
extern void vecadd(Token_vec*, const Token_t);

extern void init_tokens();
extern Token_t get_keyword_token(const char* str, size_t len);
extern Token_t get_punct_token(const char* str, size_t len);
extern void get_token_name(Token_t tok, char* name, size_t len);


#endif
