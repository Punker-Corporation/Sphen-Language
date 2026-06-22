//#include "parser/parser.h"
#define SPHEN_DEBUG
#include "parser/parser.h"

int main(int argc, char* argv[]) {
	Parser p = parser_init(argv[1]);

	puts("code:\n\n");
	puts(p.lexer.file.content);

	parse(&p);

	parser_close(&p);
    return 0;
}
