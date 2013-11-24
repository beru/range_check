#pragma once

struct Token;

enum class NodeType {
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

template <NodeType NT>
struct NodeImpl : public Node {
	static const NodeType NodeType = NT;
};

struct TokenNode : public NodeImpl<NodeType::Token> {
	const Token* token;
};

struct DeclareNode : public NodeImpl<NodeType::Declare> {
	const Token* type;
	const Token* id;
	const Node* rhs;
};

struct AssignNode : public NodeImpl<NodeType::Assign> {
	const Token* id;
	const Token* op;
	const Node* rhs;
};

struct UnaryNode : public NodeImpl<NodeType::Unary> {
	const Node* lhs;
	const Node* rhs;
};

struct BinaryNode : public NodeImpl<NodeType::Binary> {
	const Node* lhs;
	const Token* op;
	const Node* rhs;
};

struct ConditionalNode : public NodeImpl<NodeType::Conditional> {
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

