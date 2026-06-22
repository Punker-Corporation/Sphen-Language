#include "../parser.h"

i32_t get_precedence(Token_k type) {
    switch(type) {
        case SUM_EQUAL_OP:
        case SUB_EQUAL_OP:
        case DIV_EQUAL_OP:
        case MUL_EQUAL_OP:
        case MOD_EQUAL_OP:
        case EQUAL_OP: return PREC_ASSIGN;
        case LOGIC_OR_OP: return PREC_OR;
        case LOGIC_AND_OP: return PREC_AND;
        case EQU_OP:
        case NEQ_OP: return PREC_EQUALITY;
        case GRE_OP:
        case LSE_OP:
        case LST_OP:
        case GRT_OP: return PREC_COMPARE;
        case SUM_OP:
        case SUB_OP: return PREC_TERM;
        case MOD_OP:
        case MUL_OP:
        case DIV_OP: return PREC_FACTOR;
        default: return PREC_NONE;
    }
}
static Node_id parse_literal(Parser* p, Token_t t){
	AST_node node = parse_node(p, AST_LITERAL);
	node.literal.type = t.kind;
	node.literal.data = t.data;

	pushnode(&p->nodes, node);
	return node.id;

}
Node_id parse_primary(Parser* p){
    Token_t t = parse_next(p);
    AST_node node;
	if(t.group == IDENT){
		return parse_call(p, t);
	}
    if(t.group == LITERAL){
    	return parse_literal(p, t);
    }
    if(t.kind == L_PAREN_OP){
            node.id = parse_expr(p, PREC_NONE);
            expect_token(p, OPERATOR, R_PAREN_OP, "expected a$<cyan clear bold: )>$ to close expression");
            return node.id;
    }
    if(t.kind == SUB_OP || t.kind == LOGIC_NOT_OP){
        Node_id right = parse_expr(p, PREC_UNARY);

        node = parse_node(p, AST_EXPR);
        node.expr.left = 0;
        node.expr.op = t.kind;
        node.expr.right = right;
        pushnode(&p->nodes, node);
        return node.id;
        
    }
    parse_debug(p, t.line, t.col, ERROR, "expected some $<cyan clear bold:value>$ to expression");
    return 0;
}

Node_id parse_expr(Parser* p, int prec){
    Node_id left = parse_primary(p);
    while (prec < get_precedence(parse_peek(p).kind)) {
        Token_t op = parse_next(p);
        int next_prec = get_precedence(op.kind);

        if(op.kind == EQUAL_OP) next_prec--;
        
        AST_node node = parse_node(p, AST_EXPR);
        Node_id right = parse_expr(p, next_prec);
        node.expr.left = left;
        node.expr.op = op.kind;
        node.expr.right = right;
        left = node.id;
        pushnode(&p->nodes, node);
    }

    return left;	
}
Node_id parse_tuple_expr(Parser* p){
    AST_node node = parse_node(p, AST_TUPLE_EXPR);

    parse_next(p);
    bool expect_comma = false;

    node.tuple.values.begin = p->nodes.len;

    Token_t t = parse_peek(p);
    while(true){
        if(expect_comma){
            if(expect_token(p, OPERATOR, COMMA_OP, "expected a $<cyan clear bold:,>$ to set new value to $<cyan clear bold:tuple>$").kind == COMMA_OP)
                expect_comma = false;
            else
                break;
        }
        if(match_token(p, OPERATOR, R_PAREN_OP) || match_token(p, OPERATOR, COMMA_OP)){ 
            parse_debug(p, t.line, t.col, ERROR, "expected a $<cyan clear bold:value>$ at $<cyan clear bold:tuple>$"); 
            parse_next(p);
            break;
        }
        
        parse_get_value(p);
        node.tuple.count++;
        if(match_token(p, OPERATOR, COMMA_OP)) expect_comma = true;
        else break;
    }

    node.tuple.values.count = p->nodes.len - node.tuple.values.begin;
    if(node.tuple.count < 2) parse_debug(p, t.line, t.col, ERROR, "$<cyan clear bold:tuples>$ needs $<cyan clear bold:2 or more>$ values");
    expect_token(p, OPERATOR, R_PAREN_OP, "expected a $<cyan clear bold:)>$ to close $<cyan clear bold:tuple>$");

    pushnode(&p->nodes, node);
    return _NODE(p, node).id;
}

Node_id parse_get_value(Parser* p){
    if(match_token(p, OPERATOR, L_PAREN_OP)) {
        size_t i = p->pos;
        bool isTuple = false;
        while(i < p->lexer.vec.size && p->lexer.vec.data[i].kind != R_PAREN_OP){
            if(p->lexer.vec.data[i].kind == COMMA_OP ){isTuple=true; break;}
            i++;
        }
        if(isTuple) return parse_tuple_expr(p);
    }
    
    return parse_expr(p, get_precedence(parse_peek(p).kind));
}

