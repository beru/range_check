
#include "print.h"

#include "node.h"
#include "token.h"

#include <stdio.h>
#include <string.h>

static
const char* extractString(const Token* token, char* buff)
{
	memcpy(buff, token->str, token->len);
	buff[token->len] = 0;
	return buff;
}

struct Printer
{
private:
	void print(const char* str) {
		for (size_t i=0; i<depth; ++i) putchar('\t');
		puts(str);
	}
	void Print(const Token* token) {
		char buff[256];
		sprintf(buff, "%s", ToString(token->type));
		char buff2[256];
		char buff3[128];
		switch (token->type) {
		case TokenType::IntegerConstant:
		case TokenType::Identifier:
			sprintf(buff2, " %s", extractString(token, buff3));
			break;
		default:
			*buff2 = 0;
			break;
		}
		strcat(buff, buff2);
		print(buff);
	}
public:

	void Print(const TokenNode* node) {
		Print(node->token);
	}

	void Print(const UnaryNode* node) {
		++depth;
			Print(node->lhs);
		--depth;
		++depth;
			Print(node->rhs);
		--depth;
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

	void Print(const CastNode* node) {
		++depth;
			Print(node->typeToken);
		--depth;
		print("Cast");
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
		case NodeType::Unary:
			Print((UnaryNode*)node);
			break;
		case NodeType::Token:
			Print((TokenNode*)node);
			break;
		case NodeType::Cast:
			Print((CastNode*)node);
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

