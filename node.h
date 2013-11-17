#pragma once

struct Token;

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
	static const NodeType NodeType = NodeType::Token;
	const Token* token;
};
struct DeclareNode : public Node {
	static const NodeType NodeType = NodeType::Declare;
	const Token* type;
	const Token* id;
	const Node* rhs;
};
struct AssignNode : public Node {
	static const NodeType NodeType = NodeType::Assign;
	const Token* id;
	const Token* op;
	const Node* rhs;
};
struct UnaryNode : public Node {
	static const NodeType NodeType = NodeType::Unary;
	const Node* lhs;
	const Node* rhs;
};
struct BinaryNode : public Node {
	static const NodeType NodeType = NodeType::Binary;
	const Node* lhs;
	const Token* op;
	const Node* rhs;
};
struct ConditionalNode : public Node {
	static const NodeType NodeType = NodeType::Conditional;
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
