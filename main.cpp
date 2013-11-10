
#include <stdio.h>
#include "lexer.h"
#include "parser.h"

#include <vector>

int main(int argc, char* argv[])
{
	const char* buf = "(1 + 2) * 5 - 3 / 4";

	std::vector<Token> tokens;
	TokenType tt;
	Token t;
	do {
		t.str = buf;
		t.type = scan(buf);
		tokens.push_back(t);
		printf("%s\n", ToString(t.type));
	} while (t.type != TokenType::EOS);

	std::vector<AnyNode> nodes(tokens.size());
	std::vector<Node*> exps(nodes.size());
	ParseTokens(
		&tokens[0], &tokens[tokens.size()-1],
		&nodes[0],
		&exps[0]
		);
}


