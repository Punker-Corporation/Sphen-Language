#include "../parser.h"
#include "../../helpers/diag.h"


static Node_id parse_func_call(Parser* p, Token_t ident){
    AST_node node = parse_node(p, AST_FN_CALL);

    node.fn_call.name = ident.data.str;

	Token_t t = parse_peek(p);
    parse_next(p);

	node.fn_call.args.begin = p->id;

	
    while(!match_kind(p, R_PAREN_OP)){
    	Token_t tok = parse_peek(p);
        parse_expr(p, get_precedence(tok.kind));

        node.fn_call.args.count ++;
        if(!match_kind(p, COMMA_OP))
            break;
    	parse_next(p);
    }

	if(!match_kind(p, R_PAREN_OP)){
		char errorLog[LOG_SIZE];
		DIAG_FORMAT(errorLog, LOG_SIZE, 
			"expected a $<cyan, clear, bold:)>$ to close $<green, clear, bold:%s>$ $<cyan, clear, bold:function call>$", node.fn_call.name);
		parse_debug(p, t.line, t.col, ERROR, errorLog);
	}
	parse_next(p);
    
    pushnode(&p->nodes, node);
    return node.id;
}

Node_id parse_call(Parser* p, Token_t ident){
    AST_node node;
	
    if(parse_peek(p).kind == L_PAREN_OP)
        return parse_func_call(p, ident);
    

    node = parse_node(p, AST_VAR_CALL);
    node.var_call.name = ident.data.str;

    pushnode(&p->nodes, node);
    return node.id;
}
