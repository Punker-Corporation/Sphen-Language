#include "../parser.h"

Node_id parse_return_statement(Parser* p){
    AST_node node = parse_node(p, AST_RETURN_STMT);
    parse_next(p);
    node.return_st.expr = parse_expr(p, get_precedence(parse_peek(p).kind));
    pushnode(&p->nodes, node);
    return node.id;
}
Node_id parse_single_statement(Parser* p, AST_k type){
    AST_node node = parse_node(p, type);
    pushnode(&p->nodes, node);
    parse_next(p);
    return node.id;
}
