#include "../parser.h"

Node_id parse_else_statement(Parser* p){
    AST_node node = parse_node(p, AST_ELSE_STMT);
    parse_next(p);

    expect_token(p, OPERATOR, COLON_OP, "expected a $<cyan clear bold::>$ after $<cyan clear bold:else>$");
    node.else_st.block = parse_block(p, END_K, "expected a $<cyan clear bold:end>$ to close else");
    pushnode(&p->nodes, node);
    
    return node.id;
}
Node_id parse_elif_statement(Parser* p){
    AST_node node = parse_node(p, AST_ELIF_STMT);

    parse_next(p);
    node.if_st.cond = parse_expr(p, get_precedence(parse_peek(p).kind));
    expect_token(p, OPERATOR, COLON_OP, "expected a $<cyan clear bold::>$ after $<cyan clear bold:elif>$ condition");
    
    node.if_st.then_block.begin = p->nodes.len;
    while(!(match_kind(p, END_K) || match_kind(p, ELIF_K) || match_kind(p, ELSE_K)))
        node.if_st.then_block.last = parse_get_AST(p);
    node.if_st.then_block.count = p->nodes.len - node.if_st.then_block.begin;

	Token_t tok = parse_peek(p);
    switch(tok.kind){
        case ELIF_K: node.if_st.else_block = parse_elif_statement(p); break;
        case ELSE_K: node.if_st.else_block = parse_else_statement(p); break;
        case END_K: parse_next(p); break;
        default: parse_debug(p, tok.line, tok.col, ERROR, "expected at least $<cyan clear bold:end>$, $<cyan clear bold:else>$, or $<cyan clear bold:elif>$ at the end of the $<cyan clear bold:elif>$"); break;
    }
    
    pushnode(&p->nodes, node);
    return node.id;
}
Node_id parse_if_statement(Parser* p){
    AST_node node = parse_node(p, AST_IF_STMT);

    parse_next(p);
    node.if_st.cond = parse_expr(p, get_precedence(parse_peek(p).kind));
    expect_token(p, OPERATOR, COLON_OP, "expected a $<cyan clear bold::>$ after $<cyan clear bold:if>$ condition");
    
    node.if_st.then_block.begin = p->nodes.len;
    while(!(match_kind(p, END_K) || match_kind(p, ELIF_K) || match_kind(p, ELSE_K)))
        node.if_st.then_block.last = parse_get_AST(p);
    node.if_st.then_block.count = p->nodes.len - node.if_st.then_block.begin;
    
	Token_t tok = parse_peek(p);
    switch(tok.kind){
        case ELIF_K: node.if_st.else_block = parse_elif_statement(p); break;
        case ELSE_K: node.if_st.else_block = parse_else_statement(p); break;
        case END_K: parse_next(p); break;
        default: parse_debug(p, -1, tok.line, tok.col, "expected at least $<cyan clear bold:end>$, $<cyan clear bold:else>$, or $<cyan clear bold:elif>$ at the end of the $<cyan clear bold:if>$"); break;
    }
    
    pushnode(&p->nodes, node);
    return node.id;
}
