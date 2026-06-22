#include <stdarg.h>
#include <float.h>
#include <limits.h>
#include "../parser.h"
#include "../../helpers/diag.h"


void parse_debug(Parser* p, size_t line, size_t col, const unsigned char type, const char* message){
	char* fileName = p->lexer.file.name;
	switch(type){
        case ERROR: 
			DIAG_FORMAT("$<red, clear, bold:ERROR>$ in $<clear, clear, bold:%s>$ at $<cyan, clear:Line %zu>$, $<cyan, clear:Col %zu>$:\n\t%s", 
				fileName, line, col, message); 
			p->errors++;
            break;
        case WARN: 
            DIAG_FORMAT("$<yellow, clear, bold:WARN>$ in $<clear, clear, bold:%s>$ at $<cyan, clear:Line %zu>$, $<cyan, clear:Col %zu>$:\n\t%s", 
				fileName, line, col, message); 
			p->warns++;
            break;
    }
}

Parser parser_init(const char* filename){
    Parser p = {0};
    
    p.id = p.pos = p.count = 0;
    
    newastvec(&p.nodes);
	arena_init(&p.arena, 4096);
	arena_init(&p.temp, 2048);

    lexer_init(&p.lexer, filename);
	return p;
}
void parser_close(Parser* p){
	lexer_close(&p->lexer);
    
    arena_free(&p->temp);
    arena_free(&p->arena);

    delast(&p->nodes);
}
int parse(Parser* p){
    tokenize(&p->lexer);
    
    parse_program(p);

	if(p->errors) DIAG_FORMAT("$<red, clear, bold:Errors>$: %u", p->errors);
	if(p->warns) DIAG_FORMAT("$<yellow, clear, bold:Warns>$: %u", p->warns);
    return 0;
}

Token_t expect_token(Parser* p, Token_group group, Token_k type, const char* err){
    if(match_token(p, group, type)) return parse_next(p);
	Token_t t = parse_peek(p);
    parse_debug(p, t.line, t.col, ERROR, err);
    return parse_peek(p);
}
Token_t expect_group(Parser* p, Token_group group, const char* err){
    if(match_group(p, group)) return parse_next(p);
	Token_t t = parse_peek(p);
    parse_debug(p, t.line, t.col, ERROR, err);
    return parse_peek(p);
}
Token_t expect_kind(Parser* p, Token_k kind, const char* err){
    if(match_kind(p, kind)) return parse_next(p);
	Token_t t = parse_peek(p);
    parse_debug(p, t.line, t.col, ERROR, err);
    return parse_peek(p);
}


Node_id parse_get_AST(Parser* p){
	Token_t tok = parse_peek(p);
    switch(tok.kind){
        case 	  LET_K:	parse_let_decl(p); break;
        case 	CONST_K:	parse_const_decl(p); break;
        case 	   FN_K:	return parse_func_decl(p);
        //case OBJECT_K:	return parse_object_decl(p);
        case 	   IF_K:	return parse_if_statement(p);
        //case 	  FOR_K:	return parse_for_statement(p);
        case 	MATCH_K:	return parse_match_statement(p); 
        case 	WHILE_K:	return parse_while_statement(p); 
        case 	   DO_K:	return parse_do_while_statement(p);
        case   RETURN_K:	return parse_return_statement(p);
        case 	BREAK_K:	return parse_single_statement(p, AST_BREAK_STMT);
        case CONTINUE_K:	return parse_single_statement(p, AST_CONTINUE_STMT);
    	default		   :	return parse_expr(p, get_precedence(tok.kind));
    }
    return 0;
}
bool parse_program(Parser* p){
    while(!parse_is_eof(p))
        parse_get_AST(p);
    
    return true;
}

AST_block parse_block(Parser* p, Token_k endToken, char* err){
    AST_block block;
    block.begin = p->nodes.len;
    block.last = 0;
    while(parse_peek(p).kind != endToken && !parse_is_eof(p))
        block.last = parse_get_AST(p);
    block.count = p->nodes.len - block.begin;
    expect_kind(p, endToken, err);
    return block;
}

