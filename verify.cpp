#include "verify.h"

#include <map>
#include <vector>
#include <string>

#include "node.h"
#include "token.h"

struct Identifier
{
	bool isConstant;
	bool isPointer;
	bool isSigned;
	unsigned char byteSize;

	union {


	} value;

	const Token* type;
	const Token* id;
};

struct Verifier
{
	bool verifyIdentifier(const char* id, bool regist) {
		if (regist) {
			if (ids.find(id) != ids.end()) {
				errors.push_back("id overlap");
				return false;
			}
		}else {
			if (ids.find(id) == ids.end()) {
				errors.push_back("id not registered");
				return false;
			}

		}
		return true;
	}

	void Verify(const DeclareNode* node) {

		Identifier id;
		id.isConstant = false;
		id.isPointer = false;
		id.byteSize = 0;
		id.type = node->type;
		id.id = node->id;
		char buff[256];
		ExtractString(node->id, buff);
		if (verifyIdentifier(buff, true)) {
			ids[buff] = id;
		}

		if (node->rhs) {
			Verify(node->rhs);
		}
	}

	void Verify(const AssignNode* node) {
		char buff[256];
		ExtractString(node->id, buff);
		verifyIdentifier(buff, false);
		Verify(node->rhs);
	}

	void Verify(const BinaryNode* node) {
		Verify(node->lhs);
		Verify(node->rhs);
	}

	void Verify(const TokenNode* node) {
		if (node->token->type == TokenType::Identifier) {
			char buff[256];
			ExtractString(node->token, buff);
			verifyIdentifier(buff, false);
		}
	}

	void Verify(const Node* node) {
		switch (node->nodeType) {
		case NodeType::Declare:
			Verify((DeclareNode*)node);
			break;
		case NodeType::Assign:
			Verify((AssignNode*)node);
			break;
		case NodeType::Binary:
			Verify((BinaryNode*)node);
			break;
		case NodeType::Token:
			Verify((TokenNode*)node);
			break;
		}
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
