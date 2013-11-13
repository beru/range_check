
#include <stdio.h>
#include "scan.h"
#include "parse.h"
#include "print.h"
#include "node.h"

#include <vector>
#include "util.h"

int main(int argc, char* argv[])
{
	if (argc < 2) {
		printf("specify textfile.\n");
		return 1;
	}
	std::vector<char> text;
	if (!readFile(argv[1], text)) {
		printf("failed to read file.\n");
		return 1;
	}

	const char* buf = &text[0];

	std::vector<Token> tokens;
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
	size_t cnt = Parse(
		&tokens[0], &tokens[tokens.size()-1],
		&nodes[0],
		&exps[0]
		);
	for (size_t i=0; i<cnt; ++i) {
		Print(exps[i]);
		puts("\n");
	}
}

