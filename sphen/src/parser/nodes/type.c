#include "../parser.h"


Node_id parse_tuple_type(Parser* p){
    parse_next(p);
    AST_node node = parse_node(p, AST_TUPLE_TYPE);
    
    bool expect_comma = false;
    node.tuple.values.begin = p->nodes.len;
	Token_t tok = parse_peek(p);
    while(true){
        if(expect_comma){
            if(expect_token(p, OPERATOR, COMMA_OP, "expected a $<cyan clear bold:,>$ to set new type to $<cyan clear bold:tuple type>$").kind == COMMA_OP)
                expect_comma = false;
            else
                break;
        }
        Token_t t = parse_peek(p);
        if(tok.kind == R_PAREN_OP || tok.kind == COMMA_OP){ 
            parse_debug(p, ERROR, t.line, t.col, "expected a $<cyan clear bold:type>$ at $<cyan clear bold:tuple type>$"); 
            parse_next(p);
            break;
        }

        parse_type(p, "expected a $<cyan clear bold:valid type>$ at $<cyan clear bold:tuple type>$");

        node.tuple.count ++;
        if(match_token(p, OPERATOR, COMMA_OP)) expect_comma = true;
        else break;
    }
    
    node.tuple.values.count = p->nodes.len - node.tuple.values.begin;
    if(node.tuple.count < 2) parse_debug(p, ERROR, tok.line, tok.col, "$<cyan clear bold:tuple types>$ needs $<cyan clear bold:2 or more>$ types");
    expect_token(p, OPERATOR, R_PAREN_OP, "expected a $<cyan clear bold:)>$ to close $<cyan clear bold:tuple type>$");

    pushnode(&p->nodes, node);
    return node.id;
}

Node_id parse_single_type(Parser* p){
    AST_node node = parse_node(p, AST_TYPE);
    node.data_type.is_id = (parse_peek(p).group == IDENT) ? true : false;

    if(node.data_type.is_id){
    //	_NODE(p, node).data_type.typeId = parse_peek(p).data.str;
        parse_next(p);
        printf("we dont have id type yet\n");
    } else
        node.data_type.typeP = expect_group(p, DATA_TYPE, "expected some $<cyan clear bold:type>$").kind;
    
    pushnode(&p->nodes, node);
    return node.id;
}

Node_id parse_type(Parser* p, char* errorLog){
	Token_t t = parse_peek(p);
    if(t.group == DATA_TYPE || t.group == IDENT)
        return parse_single_type(p);
    if(t.kind == L_PAREN_OP)	
        return parse_tuple_type(p);
    parse_debug(p, t.line, t.col, ERROR, errorLog);
    return 0;
}

