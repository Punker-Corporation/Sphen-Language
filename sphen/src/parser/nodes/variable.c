#include "../parser.h"
#include "../../helpers/diag.h"


static void parse_var_name(Parser* p, bool is_const){
	AST_node node; 
	bool next = false;
	Token_t tok = parse_peek(p);
	do {
		node = parse_node(p, is_const ? AST_CONST_DECL : AST_LET_DECL);
		node.var.is_const = is_const;
		node.var.name = expect_group(p, IDENT, "expected a $<cyan, clear, bold: variable name>$").data.str;
		node.var.type = 0;
		
		if(match_kind(p, COLON_OP)){
			parse_next(p);
			char errorLog[LOG_SIZE];
			DIAG_FORMAT(errorLog, "expected a $<cyan, clear, bold:type>$ to $<cyan, clear, bold:%s>$", node.var.name);
			node.var.type = parse_type(p, errorLog);
		}
		
		pushnode(&p->nodes, node);
		if(next)expect_token(p, OPERATOR, COMMA_OP, "expected a $<cyan, clear, bold:,>$ between $<cyan, clear, bold:variables>$");
		next = true;
		tok = parse_next(p);
	} while(tok.kind == ID );
	
}

static void parse_var_values(Parser* p){
	parse_next(p);
	AST_node node; 
	while(true){
		parse_expr(p, get_precedence(parse_peek(p).kind));
		
		pushnode(&p->nodes, node);
		parse_peek(p);
		
		if(!match_kind(p, COMMA_OP))
			break;
	}
}

void parse_let_decl(Parser* p) {
    parse_next(p);
    parse_var_name(p, false);
    
	if(match_kind(p, EQUAL_OP))
		parse_expr(p, get_precedence(parse_peek(p).kind));	
}
void parse_const_decl(Parser* p) {
    parse_next(p);
    parse_var_name(p, true);
    
	expect_kind(p, EQUAL_OP, "expected a $<cyan, clear, bold:assigned value>$");
	parse_expr(p, get_precedence(parse_peek(p).kind));	
}
