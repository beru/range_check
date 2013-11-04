
#include <stdio.h>
#include "lexer.h"

int main(int argc, char* argv[])
{
	const char* buf = "S8 a = (123 + 245)\n 356 - 456 * 567 / 255";

	TokenType t;
	while ((t = scan(buf)) != TokenType::EOS){
		printf("%s\n", ToString(t));
	}
}

