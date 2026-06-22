#include "../parser.h"
#include "../../helpers/diag.h"

void parse_func_params(Parser* p){
    bool first = true;
    while((parse_peek(p).kind != R_PAREN_OP && parse_peek(p).kind != COLON_OP) && !parse_is_eof(p)){
        if(!first)
			if(expect_token(p, OPERATOR, COMMA_OP, "expected $<cyan clear bold:,>$ beetwen parameters").kind != COMMA_OP) break;
		
        first = false;

        AST_node node = parse_node(p, AST_FN_PARAM);
        
        switch(parse_peek(p).kind){
            case CONST_K: node.param.is_const = true; parse_next(p); break;
            default: node.param.is_const = false; break;
        } 
        node.param.name = expect_group(p, IDENT, "expected a parameter $<cyan clear bold:name>$").data.str;

       	char errorLog[LOG_SIZE];
        DIAG_FORMAT(errorLog, LOG_SIZE, "expected a $<cyan, clear, bold:explicit type>$ to parameter $<green, clear, bold:%s>$", 
        	node.param.name);
        expect_token(p, OPERATOR, COLON_OP, errorLog);			

		DIAG_FORMAT(errorLog, LOG_SIZE, "expected a $<cyan, clear, bold:valid type>$ after $<cyan, clear, bold::>$ to parameter $<green, clear, bold:%s>$", 
			node.param.name);
        node.param.type = parse_type(p, errorLog);
        
        if(match_token(p, OPERATOR, EQUAL_OP)){
            parse_next(p);
            node.param.std_value = parse_get_value(p);
        }
        pushnode(&p->nodes, node);
	}
    parse_next(p);
}

void parse_func_generics(Parser* p){
	
}

Node_id parse_func_decl(Parser* p){
    AST_node node = parse_node(p, AST_FN_DECL);

    Token_t tok = parse_next(p);
    if(match_token(p, OPERATOR, UNDERLINE_OP)){ 
        parse_debug(p, ERROR, tok.line, tok.col, "expected a letter or digit after $<cyan clear bold:_>$ at function declaration");
        parse_next(p);
    } else node.func.name = expect_group(p, IDENT, "expected function $<cyan clear bold:name>$").data.str;

	if(match_token(p, OPERATOR, LST_OP))
		parse_func_generics(p);
	
	char errorLog[LOG_SIZE];
	if(!match_token(p, OPERATOR, L_PAREN_OP)){
		DIAG_FORMAT(errorLog, LOG_SIZE, "expected a $<cyan, clear, bold:(>$ to open $<cyan, clear, bold:%s>$ args",
			node.func.name);
		parse_debug(p, tok.line, tok.col, ERROR, errorLog);
	}
	parse_next(p);
	    
    node.func.param.begin = p->nodes.len;
    parse_func_params(p);
    node.func.param.count = p->nodes.len - node.func.param.begin;
    
    node.func.ret_type = 0;
    if(match_token(p, OPERATOR, COLON_OP)){
		parse_next(p); // consume ':' before return type
		DIAG_FORMAT(errorLog, LOG_SIZE, "expected a $<cyan, clear, bold:valid return type>$ to $<cyan, clear, bold:%s>$ $<cyan, clear, bold:function>$",
 			node.func.name);
		node.func.ret_type = parse_type(p, errorLog);
	}
    expect_token(p, OPERATOR, COLON_OP, "expected $<cyan, clear, bold::>$ to open function block");
    node.func.block = parse_block(p, END_K, "expected a $<cyan, clear, bold:end>$ to close function block");
    pushnode(&p->nodes, node);
    return node.id;
}
