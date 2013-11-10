#pragma once

#include "lexer.h"

enum class NodeType
{
	Token,
	Declare,
	Assign,
	Unary,
	Binary,
	Conditional,
};
struct Node {
	NodeType nodeType;
};
struct TokenNode : public Node {
	const Token* token;
};
struct DeclareNode : public Node {
	const Token* type;
	const Token* id;
	const Node* rhs;
};
struct AssignNode : public Node {
	const Token* id;
	const Token* op;
	const Node* rhs;
};
struct UnaryNode : public Node {
	const Token* lhs;
	const Token* rhs;
};
struct BinaryNode : public Node {
	const Node* lhs;
	const Token* op;
	const Node* rhs;
};
struct ConditionalNode : public Node {
	const Node* condition;
	const Node* trueNode;
	const Node* falseNode;
};
union AnyNode {
	NodeType type;
	TokenNode token;
	DeclareNode declare;
	AssignNode assign;
	UnaryNode unary;
	BinaryNode binary;
	ConditionalNode conditional;
};
size_t ParseTokens(
	const Token* ts, const Token* te,
	AnyNode* nodes,
	Node** expressions
);

