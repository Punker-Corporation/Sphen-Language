#ifndef SPHEN_TOKEN_LIST_H
#define SPHEN_TOKEN_LIST_H



#define TOKEN_LIST\
	X(TRUE_LIT, 		LITERAL,			"true", 		4)	/*	values	*/\
	X(FALSE_LIT, 		LITERAL,			"false", 		5)\
	X(BYTE_T, 			DATA_TYPE,			"byte", 		4)	/*	data types	*/\
	X(CHAR_T, 			DATA_TYPE,			"char", 		4)\
	X(STR_T, 			DATA_TYPE,			"str", 			3)\
	X(BOOL_T, 			DATA_TYPE,			"bool", 		4)\
	X(INT_T, 			DATA_TYPE,			"int", 			3)\
	X(FLOAT_T, 			DATA_TYPE,			"float", 		5)\
	X(I8_T, 			DATA_TYPE,			"i8", 			2)\
	X(U8_T, 			DATA_TYPE,			"u8", 			2)\
	X(I16_T, 			DATA_TYPE,			"i16", 			3)\
	X(U16_T, 			DATA_TYPE,			"u16", 			3)\
	X(I32_T, 			DATA_TYPE,			"i32", 			3)\
	X(U32_T, 			DATA_TYPE,			"u32", 			3)\
	X(I64_T, 			DATA_TYPE,			"i64", 			3)\
	X(U64_T, 			DATA_TYPE,			"u64", 			3)\
	X(F32_T, 			DATA_TYPE,			"f32", 			3)\
	X(F64_T, 			DATA_TYPE,			"f64", 			3)\
	X(USIZE_T, 			DATA_TYPE,			"usize", 		5)\
	X(END_K, 			KEYWORD,			"end", 			3)	/*	keywords	*/\
	X(IF_K, 			KEYWORD,			"if", 			2)\
	X(ELIF_K, 			KEYWORD,			"elif", 		4)\
	X(ELSE_K, 			KEYWORD,			"else", 		4)\
	X(DO_K, 			KEYWORD,			"do", 			2)\
	X(FOR_K, 			KEYWORD,			"for", 			3)\
	X(WHILE_K, 			KEYWORD,			"while", 		5)\
	X(RETURN_K, 		KEYWORD,			"return", 		6)\
	X(CONTINUE_K, 		KEYWORD,			"continue", 	8)\
	X(BREAK_K, 			KEYWORD,			"break", 		5)\
	X(STEP_K, 			KEYWORD,			"step", 		4)\
	X(FN_K, 			KEYWORD,			"fn", 			2)\
	X(IMPORT_K, 		KEYWORD,			"import", 		6)\
	X(EXPORT_K, 		KEYWORD,			"export", 		6)\
	X(LET_K, 			KEYWORD,			"let", 			3)\
	X(CONST_K, 			KEYWORD,			"const", 		5)\
	X(ENUM_K, 			KEYWORD,			"enum", 		4)\
	X(MATCH_K, 			KEYWORD,			"match", 		5)\
	X(TYPE_K, 			KEYWORD,			"type", 		4)\
	X(SELF_K, 			KEYWORD,			"self", 		4)\
	X(CLASS_K, 			KEYWORD,			"class", 		5)\
	X(STRUCT_K, 		KEYWORD,			"struct", 		6)\
	X(OPERATOR_K, 		KEYWORD,			"operator", 	8)\
	X(CONSTRUCT_K, 		KEYWORD,			"construct",	9)\
	X(DESTRUCT_K, 		KEYWORD,			"destruct", 	8)\
	X(ADD_CAP, 			CONSTRAINT_CAP,		"__add", 		5)	/*	capacity constraints	*/\
	X(SUB_CAP, 			CONSTRAINT_CAP,		"__sub", 		5)\
	X(DIV_CAP, 			CONSTRAINT_CAP,		"__div", 		5)\
	X(MUL_CAP, 			CONSTRAINT_CAP,		"__mul", 		5)\
	X(MOD_CAP, 			CONSTRAINT_CAP,		"__bit", 		5)\
	X(EQU_CAP, 			CONSTRAINT_CAP,		"__equ", 		5)\
	X(ORD_CAP, 			CONSTRAINT_CAP,		"__ord", 		5)\
	X(BASIC_DATA, 		CONSTRAINT_DATA,	"__basic", 		7)	/*	data constraints	*/\
	X(OBJ_DATA, 		CONSTRAINT_DATA,	"__obj", 		5)\
	X(STRUCT_DATA, 		CONSTRAINT_DATA,	"__struct", 	8)\
	X(CLASS_DATA, 		CONSTRAINT_DATA,	"__class", 		7)\
	X(NUM_DATA, 		CONSTRAINT_DATA,	"__num", 		5)\
	X(INT_DATA, 		CONSTRAINT_DATA,	"__int", 		5)\
	X(SIGNED_DATA, 		CONSTRAINT_DATA,	"__signed", 	8)\
	X(UNSIGNED_DATA,	CONSTRAINT_DATA,	"__unsigned", 	10)\
	X(FLOAT_DATA, 		CONSTRAINT_DATA,	"__float", 		7)\
	X(TEXT_DATA, 		CONSTRAINT_DATA,	"__text", 		6)\
	X(LIST_DATA, 		CONSTRAINT_DATA,	"__list", 		6)\
	X(AS_OP, 			OPERATOR,			"as", 			2)	/*	operators	*/\
	X(IS_OP, 			OPERATOR,			"is", 			2)\
	X(L_PAREN_OP, 		OPERATOR,			"(", 			1)\
	X(R_PAREN_OP, 		OPERATOR,			")", 			1)\
	X(L_BRACKET_OP, 	OPERATOR,			"[", 			1)\
	X(R_BRACKET_OP, 	OPERATOR,			"]", 			1)\
	X(L_KEY_OP, 		OPERATOR,			"{", 			1)\
	X(R_KEY_OP, 		OPERATOR,			"}", 			1)\
	X(DOT_OP, 			OPERATOR,			".", 			1)\
	X(COMMA_OP, 		OPERATOR,			",", 			1)\
	X(SEMICOLON_OP, 	OPERATOR,			";", 			1)\
	X(COLON_OP, 		OPERATOR,			":", 			1)\
	X(QUESTION_OP, 		OPERATOR,			"?", 			1)\
	X(DOLLAR_OP, 		OPERATOR,			"$", 			1)\
	X(UNDERLINE_OP, 	OPERATOR,			"_", 			1)\
	X(RANGE_EXC_OP, 	OPERATOR,			"..", 			2)\
	X(RANGE_INC_OP, 	OPERATOR,			"..=", 			3)\
	X(SUM_OP, 			OPERATOR,			"+", 			1)\
	X(SUB_OP, 			OPERATOR,			"-", 			1)\
	X(MUL_OP, 			OPERATOR,			"*", 			1)\
	X(DIV_OP, 			OPERATOR,			"/", 			1)\
	X(MOD_OP, 			OPERATOR,			"%", 			1)\
	X(EQU_OP, 			OPERATOR,			"==", 			2)\
	X(NEQ_OP, 			OPERATOR,			"!=", 			2)\
	X(GRT_OP, 			OPERATOR,			">", 			1)\
	X(GRE_OP, 			OPERATOR,			">=", 			2)\
	X(LST_OP, 			OPERATOR,			"<", 			1)\
	X(LSE_OP, 			OPERATOR,			"<=", 			2)\
	X(EQUAL_OP, 		OPERATOR,			"=", 			1)\
	X(SUM_EQUAL_OP, 	OPERATOR,			"+=", 			2)\
	X(SUB_EQUAL_OP, 	OPERATOR,			"-=", 			2)\
	X(DIV_EQUAL_OP, 	OPERATOR,			"/=", 			2)\
	X(MUL_EQUAL_OP, 	OPERATOR,			"*=", 			2)\
	X(MOD_EQUAL_OP, 	OPERATOR,			"%=", 			2)\
	X(LOGIC_NOT_OP, 	OPERATOR,			"not", 			3)\
	X(LOGIC_AND_OP, 	OPERATOR,			"and", 			3)\
	X(LOGIC_OR_OP,		OPERATOR,			"or", 			2)\
	X(L_SHIFT_BIT_OP,	OPERATOR,			"<<", 			2)\
	X(R_SHIFT_BIT_OP,	OPERATOR,			">>", 			2)\
	X(AND_BIT_OP,		OPERATOR,			"&", 			1)\
	X(OR_BIT_OP,		OPERATOR,			"!", 			1)\
	X(NOT_BIT_OP,		OPERATOR,			"~", 			1)\
	X(XOR_BIT_OP,		OPERATOR,			"^", 			1)\
	X(AND_EQUAL_BIT_OP,	OPERATOR,			"&=", 			2)\
	X(OR_EQUAL_BIT_OP,	OPERATOR,			"|=", 			2)\
	X(XOR_EQUAL_BIT_OP,	OPERATOR,			"^=", 			2)

#endif
