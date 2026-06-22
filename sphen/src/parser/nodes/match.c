#include "../parser.h"


static Match_Pattern parse_case_pattern(Parser* p, Pattern_match_k kind){
	Match_Pattern pattern = {.kind = kind};
    
	while(parse_peek(p).kind != COLON_OP && !parse_is_eof(p)){
		Token_t t = parse_peek(p);
		if(!kind)
			switch(t.kind){
				case INTEGER	:
				case FLOATING	: 
				case STRING		:
				case CHARACTER	: 
				case TRUE_LIT	: 
				case FALSE_LIT	: 
					pattern.kind = MATCH_LITERAL; 
					parse_next(p);
					break;
				case ID: 
					parse_next(p);
					pattern.kind = MATCH_IDENT; 
					break;
					
				default: 
					parse_debug(p, t.line, t.col, ERROR, "Unexpected token in $<cyan clear bold:match case pattern>$");
					parse_next(p);
					continue;
			}
		if(match_token(p, OPERATOR, LOGIC_OR_OP))
			pattern.kind = MATCH_OR; 
		
	}

	expect_token(p, OPERATOR, COLON_OP, "Expected a $<cyan, clear, bold::>$ to open $<cyan, clear, bold:match case block>$");

    
	return pattern;
}

static AST_block parse_case_block(Parser* p){
	AST_block block;
    block.begin = p->nodes.len;
    block.last = 0;
    while((parse_peek(p).kind != COMMA_OP && parse_peek(p).kind != END_K) && !parse_is_eof(p))
		block.last = parse_get_AST(p);
    block.count = p->nodes.len - block.begin;

	Token_t t = parse_peek(p);
	if(!(parse_peek(p).kind == COMMA_OP || parse_peek(p).kind == END_K) || parse_is_eof(p))
		parse_debug(p, t.line, t.col, ERROR, "expected a $<cyan, clear, bold:end>$ or $<cyan, clear, bold:,>$ to close $<cyan, clear, bold:case block>$");
	
	return block;
}

static void parse_case_if(Parser* p){
	AST_node node = parse_node(p, AST_CASE_IF_STMT);
	
	parse_next(p);
	Token_t tok = parse_peek(p);
	node.case_st.pattern.kind = MATCH_IF;
	node.case_st.pattern.condition = parse_expr(p, get_precedence(tok.kind));

	expect_token(p, OPERATOR, COLON_OP, "Expected a $<cyan, clear, bold::>$ to open $<cyan, clear, bold:if match case block>$");

	node.case_st.body = parse_case_block(p);
	
	pushnode(&p->nodes, node);
}

static void parse_case(Parser* p){
	AST_node node = parse_node(p, AST_CASE_STMT);
	
	node.case_st.pattern = parse_case_pattern(p, MATCH_NULL);
	node.case_st.body = parse_case_block(p);
	pushnode(&p->nodes, node);
}

static AST_gap parse_match_cases(Parser* p){
	AST_gap body;
    body.begin = p->nodes.len;
    bool next = false;
    while(parse_peek(p).kind != END_K && !parse_is_eof(p)){
		if(next)
			expect_token(p, OPERATOR, COMMA_OP, "Expected a $<cyan, clear, bold:,>$ beetwen $<cyan, clear, bold:cases>$");
		next = true;
    	if(match_kind(p, IF_K)) parse_case_if(p);
    	else parse_case(p);
    }
    body.count = p->nodes.len - body.begin;
   	expect_token(p, KEYWORD, END_K, "Expected a $<cyan, clear, bold:end>$ to close $<cyan, clear, bold:match block>$");

    return body;
}

Node_id parse_match_statement(Parser* p){
	AST_node node = parse_node(p, AST_MATCH_STMT);

    parse_next(p);
	node.match_st.expr = parse_expr(p, get_precedence(parse_peek(p).kind));
	expect_token(p, OPERATOR, COLON_OP, "Expected a $<cyan, clear, bold::>$ to open $<cyan, clear, bold:match block>$");
	node.match_st.body = parse_match_cases(p);
	
    pushnode(&p->nodes, node);
    return node.id;
}
