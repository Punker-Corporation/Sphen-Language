#ifndef SPHEN_PARSER_H
#define SPHEN_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../helpers/arena.h"
#include "../lexer/lexer.h"
#include "ast.h"

#define ERROR 1
#define WARN 2
#define _NODE(parser, node) parser->nodes.data[node.id]

typedef struct {
	AST_vec nodes;
	Arena arena, temp;
	Lexer lexer;
	u64_t id, pos, count;
	u32_t warns, errors;
} Parser;



extern Parser parser_init(const char*);
extern void parser_close(Parser*);
extern int parse(Parser*);

extern void parse_debug(Parser* p, size_t line, size_t col, const u8_t type, const char* message);

// forward declaration
extern Node_id parse_get_value(Parser* p);
extern Node_id parse_expr(Parser* p, int prec);
extern Node_id parse_tuple_expr(Parser* p);
extern Node_id parse_literal(Parser* p);

extern AST_block parse_block(Parser* p, Token_k endToken, char* err);

extern Node_id parse_type(Parser* p, char* errorLog);
extern void parse_let_decl(Parser* p);
extern void parse_const_decl(Parser* p);
extern Node_id parse_func_decl(Parser* p);

// extern Node_id parse_class_decl(Parser* p);
// extern Node_id parse_struct_decl(Parser* p);
// extern Node_id parse_construct_decl(Parser* p);
// extern Node_id parse_destruct_decl(Parser* p);
// extern Node_id parse_operator_decl(Parser* p);

extern Node_id parse_call(Parser* p, Token_t ident);

extern Node_id parse_if_statement(Parser* p);
// extern Node_id parse_for_statement(Parser* p);
extern Node_id parse_while_statement(Parser* p);
extern Node_id parse_do_while_statement(Parser* p);

extern Node_id parse_match_statement(Parser* p);

extern Node_id parse_return_statement(Parser* p);
extern Node_id parse_single_statement(Parser* p, AST_k kind);
// extern Node_id parse_import_statement(Parser* p);
// extern Node_id parse_export_statement(Parser* p);




static inline AST_node parse_node(Parser* p, AST_k kind){
    return (AST_node){.kind = kind, .id = p->id++};
}
static inline Token_t parse_peek(Parser* p){
    return (p->pos < p->lexer.vec.size) ? p->lexer.vec.data[p->pos] : (Token_t){.group = END_OF_FILE};
}
static inline Token_t parse_next(Parser* p){
    return (p->pos < p->lexer.vec.size) ? p->lexer.vec.data[p->pos++] : (Token_t){.group = END_OF_FILE};
}
static inline bool parse_is_eof(Parser* p){
	return !(p->pos < p->lexer.vec.size) || p->lexer.vec.data[p->pos].group == END_OF_FILE;
}
static inline bool match_token(Parser* p, Token_group group, Token_k kind){
	return parse_peek(p).group == group && parse_peek(p).kind == kind && !parse_is_eof(p);
}
static inline bool match_group(Parser* p, Token_group group){
	return parse_peek(p).group == group && !parse_is_eof(p);
}
static inline bool match_kind(Parser* p, Token_k kind){
	return parse_peek(p).kind == kind && !parse_is_eof(p);
}



extern bool parse_program(Parser* p);
extern Token_t expect_token(Parser* p, Token_group group, Token_k kind, const char* err);
extern Token_t expect_group(Parser* p, Token_group group, const char* err);
extern Token_t expect_kind(Parser* p, Token_k kind, const char* err);

extern Node_id parse_get_AST(Parser* p);
extern i32_t get_precedence(Token_k kind);

#ifdef __cplusplus
}
#endif

#endif
