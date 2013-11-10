#pragma once

struct Token;
struct Node;
union AnyNode;

size_t Parse(
	const Token* ts, const Token* te,
	AnyNode* nodes,
	Node** expressions
);

