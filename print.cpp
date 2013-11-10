
#include "print.h"

#include "node.h"
#include "token.h"

#include <stdio.h>
#include <string.h>

static
size_t slen(const Token* token)
{
	return (token+1)->str - token->str;
}

static
const char* extractString(const Token* token, char* buff)
{
	size_t len = slen(token);
	memcpy(buff, token->str, len);
	buff[len] = 0;
	return buff;
}

struct Printer
{
	void Print(const Token* token) {
		for (size_t i=0; i<depth; ++i) putchar('\t');
		printf("%s", ToString(token->type));
		char buff[128];
		switch (token->type) {
		case TokenType::IntegerConstant:
		case TokenType::Identifier:
			printf(" %s", extractString(token, buff));
			break;
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

	void Print(const AssignNode* node) {
		++depth;
			Print(node->id);
		--depth;
		Print(node->op);
		++depth;
			Print(node->rhs);
		--depth;
	}

	void Print(const DeclareNode* node) {
		++depth;
			Print(node->id);
		--depth;
		Print(node->type);
		++depth;
			Print(node->rhs);
		--depth;
	}

	void Print(const Node* node) {
		switch (node->nodeType) {
		case NodeType::Declare:
			Print((DeclareNode*)node);
			break;
		case NodeType::Assign:
			Print((AssignNode*)node);
			break;
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

