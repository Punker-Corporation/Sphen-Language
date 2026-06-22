#include "lexer.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>

static inline void lexer_error(const char* msg){
    puts(msg);
    exit(EXIT_FAILURE);
}
static void read_file(const char* filename, File_t* file){
	FILE* f = fopen(filename, "rb");
    if(!f) lexer_error("Can not read/open file\n");

    fseek(f, 0, SEEK_END);
    size_t fsize = file->len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* fcode = malloc(fsize+1);

    if(!fcode) lexer_error("Can not allocate file content buffer");
    file->content = fcode;
	size_t nread = fread(fcode, 1, fsize, f);
	if(nread != fsize) lexer_error("Can not read file content");
	fcode[fsize] = '\0';

    file->name = malloc(strlen(filename) + 1);
    if(!file->name) lexer_error("Can not allocate file name buffer");
    memcpy(file->name, filename, strlen(filename + 1));
}

void lexer_init(Lexer* lexer, const char* filename){
    init_tokens();
    lexer->cur = lexer->col = lexer->line = 0;
    read_file(filename, &lexer->file);
    lexer->buffer = str_new(NULL);
    lexer->vec = newvec();
    arena_init(&lexer->arena, 4096);
}
void lexer_close(Lexer* lexer){
	arena_free(&lexer->arena);
	free(lexer->file.content);
	free(lexer->file.name);
    lexer->file.content = lexer->file.name = NULL;
    lexer->file.len = 0;
    vecdel(&lexer->vec);
    str_free(&lexer->buffer);
}

static inline bool is_eof(Lexer* lexer, u8_t offset){
	return lexer->cur + offset >= lexer->file.len;
}
static inline u32_t peek(Lexer* lexer){
    return (is_eof(lexer, 0)) ? '\0' : lexer->file.content[lexer->cur];
}
static inline u32_t next(Lexer* lexer){
    return (is_eof(lexer, 1)) ? '\0' : lexer->file.content[lexer->cur + 1];
}
static inline void consume(Lexer* lexer){
	str_push(&lexer->buffer, lexer->file.content[lexer->cur]);
}
static u32_t advance(Lexer* lexer){
	if(is_eof(lexer, 0)) return '\0';
	
    lexer->col++;
    if(peek(lexer) == '\n'){
        lexer->col = 1;
        lexer->line ++;
    }
    lexer->cur++;
    return peek(lexer);
}
static void fabric_token(Lexer* lexer, Token_t token){
	if(token.kind == ID || token.kind == STRING){
		char* str = (char*)arena_alloc(&lexer->arena, lexer->buffer.len+1);
		strncpy(str, lexer->buffer.data, lexer->buffer.len+1);
		str[lexer->buffer.len] = '\0';
		token.data.str = str;
	}
	vecadd(&lexer->vec, (Token_t){
        .data = token.data,
        .group = token.group,
        .kind = token.kind,
        .line = lexer->line+1,
        .col = lexer->col+1
    });
    
    //printf("%s %d\n", lexer->buffer.data, token.type);
    lexer->buffer.len = 0;
}


static inline bool is_punct(const char c){
	return (!c) ? false : strchr("(){}[].,;:?_", c) != NULL;
}
static inline bool is_operator(const char c){
	return (!c) ? false : strchr("+-/*%=!<>~^&|", c) != NULL;
}

static inline i64_t str_to_int(const char* str){
	return strtoll(str, NULL, 10); 
}
static inline f64_t str_to_double(const char* str){
	return strtod(str, NULL);
}


static bool is_ident(u32_t cur){
	return isalpha(cur) || cur == '_';
}
static void get_ident(Lexer** lexer){
	u32_t cur = peek(*lexer);
	do {
		consume(*lexer);
		cur = advance(*lexer);
	} while(isalnum(cur) || cur == '_');
	
	Token_t t = get_keyword_token((*lexer)->buffer.data, (*lexer)->buffer.len);
	
	if(t.kind == TRUE_LIT) t.data.b = true;
	if(t.kind == FALSE_LIT) t.data.b = false;
	
	fabric_token(*lexer, t);
}

static void get_operator(Lexer** lexer){
	consume(*lexer);
	advance(*lexer);
	if(is_operator(peek(*lexer))){
		consume(*lexer);
		advance(*lexer);
	}
	Token_t op = get_punct_token((*lexer)->buffer.data, (*lexer)->buffer.len);
	
	fabric_token(*lexer, op);
}

static inline bool is_number(u32_t cur, u32_t next_digit){
	return isdigit(cur) || (cur == '.' && isdigit(next_digit));
}
static void get_number(Lexer** lexer){
	bool isFloat = false;
	char c = peek(*lexer);
	do {
		if(c == '.') {
			if(isFloat) break;
			isFloat = true;
		}
		consume(*lexer);
		c = advance(*lexer);
	} while(isdigit(c) || c == '.');

	Token_t t;
	t.group = LITERAL;
	if(isFloat){
		t.kind = FLOATING;
		t.data.f = str_to_double((*lexer)->buffer.data);
	} else {
		t.kind = INTEGER;
		t.data.i = str_to_int((*lexer)->buffer.data);	
	}
	
	fabric_token(*lexer, t);
}

static void get_char(Lexer** lexer){
	advance(*lexer);
	consume(*lexer);
	advance(*lexer);
	if(peek(*lexer) != '\''){
		fabric_token(*lexer, (Token_t){
			.group = UNKNOWN,
			.kind = NULL_TOK
		});
		advance(*lexer);
		return;
	}
	Token_t t = {
		.group = LITERAL,
		.kind = CHARACTER,
		.data.c = (*lexer)->buffer.data[0]
	};
	
	
	fabric_token(*lexer, t);
	advance(*lexer);
}
static void get_string(Lexer** lexer){
	char c = advance(*lexer);
	while(c != '\0'){
		if(c == '"') break;
		consume(*lexer);
		c = advance(*lexer);
	}
	if (peek(*lexer) != '"'){
		fabric_token(*lexer, (Token_t){
			.group = UNKNOWN,
			.kind = NULL_TOK
		});
		return;
	}
	advance(*lexer);
	fabric_token(*lexer, (Token_t){
		.data.str = (*lexer)->buffer.data,
		.group = LITERAL,
		.kind = STRING
	});
}

static inline bool is_comment(Lexer** lexer){
    return peek(*lexer)=='#';
}
static void ignore_line(Lexer** lexer){
	while(advance(*lexer) != '\0')
		if(peek(*lexer) == '\n'){
			advance(*lexer);
			break;
		}
}
static void ignore_multiline(Lexer** lexer){
	char c = peek(*lexer);
	while(c != '\0'){
		if(c == ']' && next(*lexer) == '#'){
			advance(*lexer);
			advance(*lexer);
			break;
   		}
		c = advance(*lexer);
	}
}

static void get_punct(Lexer** lexer){
char first = peek(*lexer);
	consume(*lexer);
	advance(*lexer);
	
	if(first == '.' && peek(*lexer) == '.'){
		consume(*lexer);
		advance(*lexer);
		if(peek(*lexer) == '='){
		    consume(*lexer);
		    advance(*lexer);
		}
	}
	Token_t punct = get_punct_token((*lexer)->buffer.data, (*lexer)->buffer.len);
	fabric_token(*lexer, punct);
}

typedef enum {
	CHAR_UNKNOWN = 0,
	CHAR_COMMENT,
	CHAR_IDENT,
	CHAR_DIGIT,
	CHAR_CHARACTER,
	CHAR_STRING,
	CHAR_OPERATOR,
	CHAR_PUNCT,
} CHAR_ENUM;

static int check_char_group(u32_t c, u32_t next_c){
	if(is_ident(c)) 		 return CHAR_IDENT;
	if(is_number(c, next_c)) return CHAR_DIGIT;
	if(c=='#') 				 return CHAR_COMMENT;
	if(c=='\'') 			 return CHAR_CHARACTER;
	if(c=='"') 				 return CHAR_STRING;
	if(is_operator(c)) 		 return CHAR_OPERATOR;
	if(is_punct(c)) 		 return CHAR_PUNCT;
							 return CHAR_UNKNOWN;
}

void tokenize(Lexer* lexer){
	u32_t c = peek(lexer), c_next = 0;
	while(c && !is_eof(lexer, 0)){
		while(isspace(c) && c != '\0') c = advance(lexer);
		
		c = peek(lexer);
		c_next = next(lexer);
		switch(check_char_group(c, c_next)){
			case CHAR_COMMENT:
				if(c_next=='[') ignore_multiline(&lexer);
				else ignore_line(&lexer);
				break;
			case     CHAR_IDENT: get_ident(&lexer);  	break;
			case     CHAR_DIGIT: get_number(&lexer); 	break;
			case CHAR_CHARACTER: get_char(&lexer); 	 	break;
			case    CHAR_STRING: get_string(&lexer); 	break;
			case     CHAR_PUNCT: get_punct(&lexer);  	break;
			case  CHAR_OPERATOR: get_operator(&lexer); 	break;
			
			case   CHAR_UNKNOWN:
			default			   :
				advance(lexer);
				break;
		}
	}
	fabric_token(lexer, (Token_t){
		.data.b = 0,
		.kind = NULL_TOK,
		.group = END_OF_FILE
	});
}
