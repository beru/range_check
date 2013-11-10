
#include "print.h"

#include "node.h"
#include "token.h"

#include <stdio.h>
#include <string.h>

struct Printer
{
	void Print(const Token* token) {
		for (size_t i=0; i<depth; ++i) putchar('\t');
		printf("%s", ToString(token->type));
		if (token->type == TokenType::IntegerConstant) {
			char buff[64];
			size_t len = (token+1)->str - token->str;
			memcpy(buff, token->str, len);
			buff[len] = 0;
			printf(" %s", buff);
		}
		puts("");
	}

	void Print(const TokenNode* node) {
		Print(node->token);
	}

	void Print(const BinaryNode* node) {
		++depth;
			Print(node->lhs);
		--depth;
		Print(node->op);
		++depth;
			Print(node->rhs);
		--depth;
	}

	void Print(const Node* node) {
		switch (node->nodeType) {
		case NodeType::Binary:
			Print((BinaryNode*)node);
			break;
		case NodeType::Token:
			Print((TokenNode*)node);
			break;
		}
	}

	size_t depth;
};

void Print(const Node* ast)
{
	Printer p;
	p.depth = 0;
	p.Print(ast);
}

