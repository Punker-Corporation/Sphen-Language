#include "../parser.h"

Node_id parse_while_statement(Parser* p){
    AST_node node = parse_node(p, AST_WHILE_STMT);

    parse_next(p);
    node.while_st.cond = parse_expr(p, get_precedence(parse_peek(p).kind));
    expect_token(p, OPERATOR, COLON_OP, "expected a $<cyan clear bold::>$ after the $<cyan clear bold:while>$ condition");
    node.while_st.block = parse_block(p, END_K, "expected a $<cyan clear bold:end>$ to close $<cyan clear bold:while>$");
    
    pushnode(&p->nodes, node);
    return node.id;
}

Node_id parse_do_while_statement(Parser* p){
    AST_node node = parse_node(p, AST_DOWHILE_STMT);

    parse_next(p);
    node.dowhile_st.block = parse_block(p, WHILE_K, "expected a $<cyan clear bold:while>$ to close $<cyan clear bold:do-while>$");
    
    node.dowhile_st.cond = parse_expr(p, get_precedence(parse_peek(p).kind));
    expect_token(p, OPERATOR, COLON_OP, "expected a $<cyan clear bold::>$ after the $<cyan clear bold:do-while>$ condition");

    pushnode(&p->nodes, node);
    return node.id;
}
