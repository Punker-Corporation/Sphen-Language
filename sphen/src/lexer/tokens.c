#include "tokens.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>

bool cmptoken(const Token_t a, const Token_t b){
    return a.group == b.group && a.kind == b.kind;
}

Token_vec newvec(){
    Token_vec vec = {
        .data = NULL,
        .size = 0,
        .cap = 32
    };
    vec.data = (Token_t*)calloc(vec.cap, sizeof(Token_t));
    return vec;
}

void vecdel(Token_vec* vec){
    free(vec->data);
    vec->data = NULL;
    vec->cap = 0;
    vec->size = 0;
}

void vecadd(Token_vec* vec, const Token_t token){
    if(vec->size >= vec->cap){
        vec->cap *= 2;
        Token_t* tmp = (Token_t*)realloc(vec->data, vec->cap * sizeof(Token_t));
        if(!tmp){
            puts("TOKEN:VECTOR:REALLOC:ERROR\n");
            exit(EXIT_FAILURE);
        }
        vec->data = tmp;
    }
    vec->data[vec->size++] = token;
}

typedef struct {
    const char* data;
    Token_group group;
    Token_k kind;
    u32_t len;
} Token_element;

typedef struct {
    Token_element items[8];
    u8_t count;
} Token_bucket;

static Token_bucket tokens[64];

static inline size_t _hash_id(const char* s, size_t len){
	size_t i = (s[0]*0x21 + s[len/2]*0x21 + s[len-1]*0x21 + len) & 63;
	return i;
}
static inline void hash_token(const char* _name, u32_t _len, Token_group _group, Token_k _kind){
    Token_bucket* b = &tokens[_hash_id(_name, _len)];
    
    if (b->count < 4) {
        b->items[b->count++] = (Token_element){.data=_name, .len=_len, .group=_group, .kind=_kind};
    }
}

void init_tokens(){
#define X(kind, group, name, len) hash_token(name, len, group, kind);
	TOKEN_LIST
#undef X    
}

static inline Token_bucket* hash(const char* s, size_t len){
    return &tokens[_hash_id(s, len)];
}

Token_t get_keyword_token(const char* str, size_t len){
    Token_bucket* b = hash(str, len);
    for(int i = 0; i < b->count; i++){
        Token_element* k = &b->items[i];
        if(k->len != len) continue;
        if(k->data && memcmp(k->data, str, len) == 0)
            return (Token_t){.group = k->group, .kind = k->kind};
    }
    Token_t tok = {
    	.group = IDENT,
    	.kind = ID,
    	.data.str = (char*)str
    };
    return tok;
}

Token_t get_punct_token(const char* str, size_t len){
    Token_bucket* b = hash(str, len);
    for(int i = 0; i < b->count; i++){
        Token_element* k = &b->items[i];
        if(k->len != len) continue;
        if(k->data && memcmp(k->data, str, len) == 0)
            return (Token_t){.group = k->group, .kind = k->kind};
    }
    return (Token_t){.group = UNKNOWN, .kind = NULL_TOK};
}


static char* tokenNames[] = {
#define X(kind, group, name, len)[kind]=name,
	TOKEN_LIST
#undef X
};

void get_token_name(Token_t tok, char* name, size_t len){
	
	switch(tok.kind){
		case INTEGER: 
			snprintf(name, len, "%lli", tok.data.i);
			break;
		case FLOATING:
			snprintf(name, len, "%.6f", tok.data.f);
			break;
		case CHARACTER:
			snprintf(name, len, "%c", tok.data.c);
			break;
		case ID:
		case STRING:
			snprintf(name, len, "%s", tok.data.str);
			break;
		default:
			snprintf(name, len, "%s", (tokenNames[tok.kind]) ? tokenNames[tok.kind] : "NULL_TOKEN");
			break;
	}
}
