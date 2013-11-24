#include "verify.h"

#include <map>
#include <vector>
#include <string>
#include <stdint.h>

#include "node.h"
#include "token.h"
#include "value.h"

struct Identifier
{
	bool isConstant;
	bool isPointer;
	bool isArray;
	bool isSigned;
	bool isFunction;
	unsigned char byteSize;

	Value value;

	const Token* type;
	const Token* id;
};

struct Verifier
{
	bool findId(const char* id) {
		if (ids.find(id) != ids.end()) {
			return true;
		}
		return false;
	}

	bool findId(const Token* id) {
		char buff[256];
		ExtractString(id, buff);
		return findId(buff);
	}

	Value Verify(const DeclareNode* node) {

		Identifier id;
		id.isConstant = false;
		id.isPointer = false;
		id.byteSize = 0;
		id.type = node->type;
		id.id = node->id;
		char buff[256];
		ExtractString(node->id, buff);
		if (!findId(buff)) {
			ids[buff] = id;
		}else {
			errors.push_back("id overlap");
		}
		Value& lhs = getValue(node->id);
		if (node->rhs) {
			Value rhs = Verify(node->rhs);
			lhs = rhs;
		}
		return lhs;
	}

	Value Verify(const AssignNode* node) {
		if (!findId(node->id)) {
			errors.push_back("id not found");
		}
		Value& lhs = getValue(node->id);
		Value rhs = Verify(node->rhs);
		switch (node->op->type) {
		case TokenType::AddAssign:
			return lhs += rhs;
			break;
		case TokenType::SubAssign:
			return lhs -= rhs;
			break;
		case TokenType::MulAssign:
			return lhs *= rhs;
			break;
		case TokenType::DivAssign:
			return lhs /= rhs;
			break;
		case TokenType::RightShiftAssign:
			return lhs >>= rhs;
			break;
		case TokenType::LeftShiftAssign:
			return lhs <<= rhs;
			break;
		}
	}

	Value Verify(const BinaryNode* node) {
		Value lhs = Verify(node->lhs);
		Value rhs = Verify(node->rhs);
		switch (node->op->type) {
		case TokenType::Add:
			return lhs + rhs;
			break;
		case TokenType::Sub:
			return lhs - rhs;
			break;
		case TokenType::Mul:
			return lhs * rhs;
			break;
		case TokenType::Div:
			return lhs / rhs;
			break;
		case TokenType::RightShift:
			return lhs >> rhs;
			break;
		case TokenType::LeftShift:
			return lhs << rhs;
			break;
		}
	}

	Value Verify(const UnaryNode* node) {
		Value lhs = Verify(node->lhs);
		Value rhs = Verify(node->rhs);
		// TODO: rewrite..
		return lhs;
	}

	Value Verify(const TokenNode* node) {
		switch (node->token->type) {
		case TokenType::Identifier:
			if (!findId(node->token)) {
				errors.push_back("id not found");
			}
			return getValue(node->token);
			break;
		case TokenType::IntegerConstant:
			{
				char buff[256];
				Value v;
				v.s8 = atoll(ExtractString(node->token, buff));
				return v;
			}
			break;
		}
	}

	Value Verify(const Node* node) {
		switch (node->nodeType) {
		case NodeType::Declare:
			return Verify((DeclareNode*)node);
			break;
		case NodeType::Assign:
			return Verify((AssignNode*)node);
			break;
		case NodeType::Binary:
			return Verify((BinaryNode*)node);
			break;
		case NodeType::Unary:
			return Verify((UnaryNode*)node);
			break;
		case NodeType::Token:
			return Verify((TokenNode*)node);
			break;
		}
	}

	Value& getValue(const Token* id) {
		char buff[256];
		ExtractString(id, buff);
		Identifier& identifier = ids[buff];
		return identifier.value;
	}

	std::map<std::string, Identifier> ids;
	std::vector<std::string> errors;
};

void Verify(Node** nb, size_t cnt)
{
	Verifier v;
	for (size_t i=0; i<cnt; ++i) {
		Node* nd = nb[i];
		if (!nd) {
			break;
		}
		v.Verify(nd);
	}
	
}
