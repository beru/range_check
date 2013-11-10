
#include <stdio.h>
#include "scan.h"
#include "parse.h"
#include "print.h"
#include "node.h"

#include <vector>

int main(int argc, char* argv[])
{
	const char* buf = "S1 a = (1 + 2 * 3) / (4 + 7 / 3);";

	std::vector<Token> tokens;
	TokenType tt;
	Token t;
	do {
		t.str = buf;
		t.type = Scan(buf);
		tokens.push_back(t);
		printf("%s\n", ToString(t.type));
	} while (t.type != TokenType::EOS);
	puts("\n");

	std::vector<AnyNode> nodes(tokens.size());
	std::vector<Node*> exps(nodes.size());
	Parse(
		&tokens[0], &tokens[tokens.size()-1],
		&nodes[0],
		&exps[0]
		);
	Print(exps[0]);
}

