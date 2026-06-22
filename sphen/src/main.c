//#include "parser/parser.h"
#define SPHEN_DEBUG
#include "parser/parser.h"


int main(int argc, char* argv[]) {
	if(argc < 2){
		fputs("Usage: sphen <source-file>\n", stderr);
		return 1;
	}

	Parser p = parser_init(argv[1]);

	puts("code:\n\n");
	puts(p.lexer.file.content);

	parse(&p);

	parser_close(&p);
    return 0;
}
