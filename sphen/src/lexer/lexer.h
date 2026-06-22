#ifndef SPHEN_LEXER_H
#define SPHEN_LEXER_H

#include <stdio.h>
#include "tokens.h"
#include "../helpers/str.h"

typedef struct {
	char* content;
	char* name;
    size_t len;
} File_t;

typedef struct {
    size_t cur;
    size_t line, col;
    str_t buffer;
    Arena arena;
    File_t file;
    Token_vec vec;
} Lexer;

extern void lexer_init(Lexer*, const char*);
extern void lexer_close(Lexer*);

extern void tokenize(Lexer*);

#endif
