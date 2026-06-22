#include "../parser.h"
#include "../../helpers/diag.h"


static void parse_var_name(Parser* p, bool is_const){
	while(true){
		AST_node node = parse_node(p, is_const ? AST_CONST_DECL : AST_LET_DECL);
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
		if(match_kind(p, COMMA_OP)) break;
		parse_next(p); //consume ','
	}
}

static void parse_var_values(Parser* p){
	parse_next(p);
	while(true){
		parse_expr(p, get_precedence(parse_peek(p).kind));
			
		if(!match_kind(p, COMMA_OP))
			break;
		
		parse_next(p);
	}
}

void parse_let_decl(Parser* p) {
    parse_next(p);
    parse_var_name(p, false);
    
	if(match_kind(p, EQUAL_OP)){
		parse_next(p); // consume '='
		parse_expr(p, get_precedence(parse_peek(p).kind));	
	}
}
void parse_const_decl(Parser* p) {
    parse_next(p);
    parse_var_name(p, true);
    
	expect_kind(p, EQUAL_OP, "expected a $<cyan, clear, bold:assigned value>$");
	parse_expr(p, get_precedence(parse_peek(p).kind));	
}
